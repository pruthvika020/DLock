# 🔐 DLock — Multithreaded Resource Locking System

A C-based client-server application that provides synchronized resource locking using TCP sockets, POSIX threads, mutexes, and condition variables.

DLock demonstrates how multiple clients can request access to shared named resources while the server coordinates lock ownership and access.

## 🚀 Features

- **TCP Client-Server Communication:** Uses TCP sockets for reliable communication between clients and the server.
- **Multithreaded Server:** Handles multiple client connections using POSIX threads.
- **Resource Locking:** Clients can request exclusive access to named resources.
- **Thread Synchronization:** Uses pthread mutexes and condition variables to coordinate concurrent lock requests.
- **Lock Ownership:** Only the client that owns a lock can release it.
- **Automatic Lock Cleanup:** Releases locks owned by a client when its connection ends.
- **Resource Status:** Displays whether registered resources are locked or free.
- **Event Logging:** Records lock acquisition, release, and client disconnection events in `dlock.log`.
- **Makefile Build:** Compiles the client and server using GCC.

## 🛠️ Technologies Used

- C
- GCC
- POSIX Threads (Pthreads)
- TCP/IP Sockets
- Linux / Ubuntu
- GNU Make
- Git and GitHub

## 📂 Project Structure

```text
DLock/
├── src/
│   ├── server.c       # Multithreaded locking server
│   └── client.c       # Interactive TCP client
├── include/           # Header files
├── logs/              # Log directory
├── Makefile           # Build configuration
├── README.md          # Project documentation
└── dlock.log          # Server event log, generated at runtime
```

The `dlock_server` and `dlock_client` executables are generated during compilation.

## ⚙️ Prerequisites

Install GCC, GNU Make, and Git on Ubuntu:

```bash
sudo apt update
sudo apt install build-essential git -y
```

## 📥 Installation

Clone the repository:

```bash
git clone https://github.com/pruthvika020/DLock.git
cd DLock
```

## 🔨 Build

Compile both the server and client:

```bash
make
```

The Makefile uses the following compiler options:

```bash
-Wall -Wextra -pthread
```

These enable additional compiler warnings and POSIX thread support.

To remove the compiled executables:

```bash
make clean
```

## ▶️ Running DLock

### 1. Start the server

In the first terminal:

```bash
./dlock_server
```

The server listens on TCP port `5000`.

### 2. Start a client

In a second terminal, from the same project directory:

```bash
./dlock_client
```

The client connects to `127.0.0.1:5000`, so the default setup runs the client and server on the same machine.

You can launch additional client instances in separate terminals to test concurrent access.

## 💻 Available Commands

Enter the following commands at the `dlock>` prompt.

| Command | Description |
|---|---|
| `LOCK file1` | Acquire the lock for a named resource |
| `UNLOCK file1` | Release a lock owned by the current client |
| `STATUS` | Display registered resources and their lock states |
| `QUIT` | Disconnect the client |

### Example Session

```text
dlock> LOCK file1
Server: GRANTED file1

dlock> STATUS
Server: file1=LOCKED

dlock> UNLOCK file1
Server: UNLOCKED file1

dlock> STATUS
Server: file1=FREE

dlock> QUIT
Server: BYE
```

## 🧠 How It Works

1. The server creates a TCP socket and listens for incoming client connections.
2. Each accepted connection is handled by a separate server thread.
3. A client sends commands to request, release, or inspect resource locks.
4. The server protects its shared lock table with a mutex.
5. If a resource is locked by another client, the requesting thread waits on a condition variable until the resource becomes available.
6. When a lock is released, waiting threads are notified so they can compete to acquire it.
7. When a client disconnects, the server releases the locks owned by that connection.

## 🎯 Learning Outcomes

- Socket programming using TCP/IP.
- Multithreaded server development in C.
- Synchronization using mutexes and condition variables.
- Coordination of concurrent access to shared resources.
- Client-server protocol design.
- Build automation using GNU Make.
- Source control with Git and GitHub.

## 🔮 Future Improvements

- Add configurable server addresses and ports.
- Introduce authentication and access control.
- Add lock timeouts and client request limits.
- Improve error handling and network protocol robustness.
- Add automated tests for concurrent lock requests.
- Support persistent resource management and more detailed monitoring.

## 👩‍💻 Author

**Pruthvika Pappula**

GitHub: [@pruthvika020](https://github.com/pruthvika020)


