# DLock 🔐
### A C-Based Client-Server Locking Project

DLock is a C-based systems programming project featuring separate client and server programs. It uses GCC and POSIX thread support during compilation.

The project provides an opportunity to explore systems programming, concurrent programming, client-server design, and build automation in a Linux environment.

## ✨ Project Highlights

- **C Programming:** Implemented using C.
- **Client-Server Architecture:** Separate client and server executables.
- **Multithreading Support:** Compiled with POSIX thread support using `-pthread`.
- **Build Automation:** Uses a Makefile to simplify compilation.
- **Compiler Warnings:** Uses `-Wall` and `-Wextra` to help identify potential issues.
- **Modular Organization:** Separates source files and header files into dedicated directories.

## 🛠️ Technologies Used

- C
- GCC Compiler
- GNU Make
- POSIX Threads (Pthreads)
- Linux / Ubuntu
- Git and GitHub

## 📁 Project Structure

```text
DLock/
├── include/          # Header files
├── src/
│   ├── server.c      # Server implementation
│   └── client.c      # Client implementation
├── logs/             # Log directory
├── Makefile          # Build instructions
├── dlock_server      # Compiled server executable
├── dlock_client      # Compiled client executable
├── dlock.log         # Log file, if generated
└── README.md         # Project documentation
```

## 📋 Prerequisites

Install GCC, GNU Make, and Git on Ubuntu:

```bash
sudo apt update
sudo apt install build-essential git -y
```

## 🚀 Getting Started

### 1. Clone the repository

```bash
git clone https://github.com/pruthvika020/DLock.git
cd DLock
```

### 2. Compile the project

Build both executables using the Makefile:

```bash
make
```

The Makefile compiles the server and client using:

```bash
gcc -Wall -Wextra -pthread src/server.c -o dlock_server
gcc -Wall -Wextra -pthread src/client.c -o dlock_client
```

### 3. Clean generated executables

To remove the compiled binaries:

```bash
make clean
```

## ▶️ Running the Project

After compilation, run the server and client according to the program's requirements.

For example, if both programs run independently:

**Terminal 1 — Server**

```bash
./dlock_server
```

**Terminal 2 — Client**

```bash
./dlock_client
```

Check the source code for any required arguments, configuration, or startup order before running the programs.

## 🎯 Learning Objectives

- Practice C programming and Linux systems development.
- Understand the organization of client-server applications.
- Explore POSIX thread support and concurrent programming.
- Automate compilation with GNU Make.
- Use Git and GitHub for source-code management.

## 👩‍💻 Author

**Pruthvika Pappula**

GitHub: [@pruthvika020](https://github.com/pruthvika020)

## 📄 License

No license has been specified yet. Add a license if you intend to define the terms under which others may use or distribute this project.
