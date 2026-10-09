# DLock 🔐

**DLock** is a C-based systems programming project designed to explore locking mechanisms and client-server architecture.

## 📌 Project Overview

DLock aims to provide hands-on experience with low-level programming, modular code organization, build automation, and client-server components.

## ✨ Features

- Implemented in C
- Separate client and server components
- Modular source code organization
- Build automation using a Makefile
- Log files for debugging and monitoring, if enabled by the implementation

## 🛠️ Technologies Used

- **Language:** C
- **Build Tool:** GNU Make
- **Version Control:** Git and GitHub
- **Environment:** Linux / Ubuntu

## 📂 Project Structure

```text
DLock/
├── include/          # Header files
├── src/              # C source files
├── logs/             # Log directory, if used
├── Makefile          # Build configuration
├── dlock_client      # Client executable (after compilation)
├── dlock_server      # Server executable (after compilation)
├── dlock.log         # Log file, if generated
└── README.md         # Project documentation
```

## ⚙️ Prerequisites

Make sure you have the following installed:

- GCC compiler
- GNU Make
- Git

Install the required tools on Ubuntu:

```bash
sudo apt update
sudo apt install build-essential git -y
```

## 🚀 Build Instructions

Clone the repository:

```bash
git clone https://github.com/pruthvika020/DLock.git
cd DLock
```

Compile the project:

```bash
make
```

## ▶️ Running the Project

After a successful build, start the client and server according to the execution requirements defined in the project.

For example, if both executables are standalone programs:

```bash
./dlock_server
```

Open another terminal in the same project directory and run:

```bash
./dlock_client
```

**Note:** Update these commands if your implementation requires arguments, a particular startup order, or a specific configuration.

## 📚 Learning Outcomes

- Practiced C programming and systems-level development
- Worked with modular source code and header files
- Used Makefile-based compilation
- Learned Git and GitHub version control workflows
- Explored client-server program organization

## 👩‍💻 Author

**Pruthvika Pappula**

GitHub: [@pruthvika020](https://github.com/pruthvika020)

## 📄 License

No license has been specified yet. Add a license if you intend to define how others may use, modify, or distribute this project.
# DLock
