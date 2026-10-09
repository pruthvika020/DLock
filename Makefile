CC = gcc
CFLAGS = -Wall -Wextra -pthread
TARGETS = dlock_server dlock_client

all: $(TARGETS)

dlock_server: src/server.c
	$(CC) $(CFLAGS) src/server.c -o dlock_server

dlock_client: src/client.c
	$(CC) $(CFLAGS) src/client.c -o dlock_client

clean:
	rm -f $(TARGETS)

.PHONY: all clean
