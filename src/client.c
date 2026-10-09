#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 512

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

int main(void)
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    if (connect(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("connect");
        close(fd);
        return 1;
    }

    char buffer[BUFFER_SIZE];

    for (int i = 0; i < 2; i++) {
        if (read_line(fd, buffer, sizeof(buffer)) <= 0) {
            close(fd);
            return 1;
        }
        printf("Server: %s\n", buffer);
    }

    while (1) {
        printf("dlock> ");
        fflush(stdout);

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '\0')
            continue;

        if (send_text(fd, buffer) < 0 ||
            send_text(fd, "\n") < 0) {
            printf("Connection to server lost.\n");
            break;
        }

        if (read_line(fd, buffer, sizeof(buffer)) <= 0) {
            printf("Connection to server lost.\n");
            break;
        }

        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "BYE") == 0)
            break;
    }

    close(fd);
    return 0;
}
