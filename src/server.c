#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5000
#define MAX_LOCKS 100
#define NAME_SIZE 100
#define BUFFER_SIZE 512

typedef struct {
    char name[NAME_SIZE];
    int locked;
    int owner;
} Lock;

typedef struct {
    int fd;
    int id;
} Client;

static Lock locks[MAX_LOCKS];
static int lock_count = 0;
static int next_client_id = 1;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t changed = PTHREAD_COND_INITIALIZER;
static FILE *log_file = NULL;

static void log_event(const char *message)
{
    if (log_file != NULL) {
        fprintf(log_file, "%s\n", message);
        fflush(log_file);
    }
}

static int send_text(int fd, const char *text)
{
    size_t length = strlen(text);
    size_t sent = 0;

    while (sent < length) {
        ssize_t n = send(fd, text + sent, length - sent, 0);
        if (n <= 0)
            return -1;
        sent += (size_t)n;
    }

    return 0;
}

static int read_line(int fd, char *buffer, size_t size)
{
    size_t used = 0;

    while (used + 1 < size) {
        char ch;
        ssize_t n = recv(fd, &ch, 1, 0);

        if (n <= 0)
            return (int)n;

        if (ch == '\n')
            break;

        if (ch != '\r')
            buffer[used++] = ch;
    }

    buffer[used] = '\0';
    return 1;
}

static int find_lock(const char *name)
{
    for (int i = 0; i < lock_count; i++) {
        if (strcmp(locks[i].name, name) == 0)
            return i;
    }

    return -1;
}

static int get_lock(const char *name)
{
    int index = find_lock(name);

    if (index >= 0)
        return index;

    if (lock_count >= MAX_LOCKS)
        return -1;

    index = lock_count++;
    snprintf(locks[index].name, NAME_SIZE, "%s", name);
    locks[index].locked = 0;
    locks[index].owner = -1;

    return index;
}

static void release_client_locks(int client_id)
{
    pthread_mutex_lock(&mutex);

    for (int i = 0; i < lock_count; i++) {
        if (locks[i].locked && locks[i].owner == client_id) {
            locks[i].locked = 0;
            locks[i].owner = -1;
        }
    }

    pthread_cond_broadcast(&changed);
    pthread_mutex_unlock(&mutex);
}

static void *handle_client(void *argument)
{
    Client client = *(Client *)argument;
    free(argument);

    char buffer[BUFFER_SIZE];
    char response[BUFFER_SIZE];

    send_text(client.fd,
        "WELCOME DLOCK\n"
        "Commands: LOCK name | UNLOCK name | STATUS | QUIT\n");

    while (read_line(client.fd, buffer, sizeof(buffer)) > 0) {
        char command[NAME_SIZE] = "";
        char name[NAME_SIZE] = "";

        int fields = sscanf(buffer, "%99s %99s", command, name);

        if (fields < 1)
            continue;

        if (strcmp(command, "LOCK") == 0 && fields == 2) {
            if (strchr(name, '/') != NULL) {
                send_text(client.fd, "ERROR Invalid resource name\n");
                continue;
            }

            pthread_mutex_lock(&mutex);

            int index = get_lock(name);

            if (index < 0) {
                pthread_mutex_unlock(&mutex);
                send_text(client.fd, "ERROR Lock table full\n");
                continue;
            }

            while (locks[index].locked &&
                   locks[index].owner != client.id) {
                pthread_cond_wait(&changed, &mutex);
            }

            locks[index].locked = 1;
            locks[index].owner = client.id;

            snprintf(response, sizeof(response),
                     "GRANTED %s\n", name);

            char event[BUFFER_SIZE];
            snprintf(event, sizeof(event),
                     "Client %d acquired %s", client.id, name);
            log_event(event);

            pthread_mutex_unlock(&mutex);
            if (send_text(client.fd, response) < 0)
                break;
        }
        else if (strcmp(command, "UNLOCK") == 0 && fields == 2) {
            pthread_mutex_lock(&mutex);

            int index = find_lock(name);

            if (index >= 0 && locks[index].locked &&
                locks[index].owner == client.id) {
                locks[index].locked = 0;
                locks[index].owner = -1;

                snprintf(response, sizeof(response),
                         "UNLOCKED %s\n", name);

                char event[BUFFER_SIZE];
                snprintf(event, sizeof(event),
                         "Client %d released %s", client.id, name);
                log_event(event);

                pthread_cond_broadcast(&changed);
                pthread_mutex_unlock(&mutex);
                if (send_text(client.fd, response) < 0)
                    break;
            } else {
                pthread_mutex_unlock(&mutex);
                send_text(client.fd,
                          "ERROR You do not own that lock\n");
            }
        }
        else if (strcmp(command, "STATUS") == 0) {
            pthread_mutex_lock(&mutex);

            response[0] = '\0';

            if (lock_count == 0) {
                snprintf(response, sizeof(response),
                         "STATUS No resources\n");
            } else {
                for (int i = 0; i < lock_count; i++) {
                    char item[160];

                    snprintf(item, sizeof(item), "%s=%s%s",
                        locks[i].name,
                        locks[i].locked ? "LOCKED" : "FREE",
                        i == lock_count - 1 ? "\n" : "; ");

                    if (strlen(response) + strlen(item) <
                        sizeof(response)) {
                        strcat(response, item);
                    }
                }
                if (response[0] == '\0')
                    snprintf(response, sizeof(response),
                             "STATUS unavailable\n");
            }

            pthread_mutex_unlock(&mutex);
            if (send_text(client.fd, response) < 0)
                break;
        }
        else if (strcmp(command, "QUIT") == 0) {
            send_text(client.fd, "BYE\n");
            break;
        }
        else {
            send_text(client.fd,
                "ERROR Use LOCK name, UNLOCK name, STATUS or QUIT\n");
        }
    }

    release_client_locks(client.id);

    char event[BUFFER_SIZE];
    snprintf(event, sizeof(event), "Client %d disconnected", client.id);
    log_event(event);

    close(client.fd);
    return NULL;
}

int main(void)
{
    signal(SIGPIPE, SIG_IGN);

    log_file = fopen("dlock.log", "a");
    if (log_file == NULL) {
        perror("dlock.log");
        return 1;
    }

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        fclose(log_file);
        return 1;
    }

    int option = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               &option, sizeof(option));

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0) {
        perror("bind");
        close(server_fd);
        fclose(log_file);
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        fclose(log_file);
        return 1;
    }

    printf("DLock server running on port %d\n", PORT);
    printf("Press Ctrl+C to stop.\n");
    fflush(stdout);

    while (1) {
        struct sockaddr_in client_address;
        socklen_t address_length = sizeof(client_address);

        int client_fd = accept(server_fd,
            (struct sockaddr *)&client_address, &address_length);

        if (client_fd < 0) {
            if (errno == EINTR)
                continue;
            perror("accept");
            continue;
        }

        Client *client = malloc(sizeof(Client));
        if (client == NULL) {
            close(client_fd);
            continue;
        }

        pthread_mutex_lock(&mutex);
        client->id = next_client_id++;
        pthread_mutex_unlock(&mutex);
        client->fd = client_fd;

        pthread_t thread;
        if (pthread_create(&thread, NULL, handle_client, client) != 0) {
            close(client_fd);
            free(client);
            continue;
        }

        pthread_detach(thread);
    }

    close(server_fd);
    fclose(log_file);
    return 0;
}
