# 🐚 kaushell — A Custom Linux Shell in C

Welcome to **kaushell**, a minimalist and extensible Unix shell written in C, created as part of my systems programming journey. This project demonstrates low-level process control, I/O redirection, and piping — built entirely from scratch.

---

## 🚀 Features

- 🧠 **Command Parsing**
  - Supports basic command execution just like Bash or Sh.
  
- 🔄 **I/O Redirection**
  - Handles both input (`<`) and output (`>`) redirection.

- 🧵 **Pipelining**
  - Supports command chaining using pipes (`|`), e.g., `ls | sort`.

- 📜 **Command Logging**
  - All commands entered are logged in `logFile.txt`.
  - View logs using the built-in `logs` command.

- 🔚 **Quit Command**
  - Type `quit` to exit the shell gracefully.

---

## 🛠 How It Works

- Uses `fork()`, `execvp()`, and `dup2()` for process creation and I/O stream management.
- Commands Technologies Used
C Language

Linux System Calls

File Descriptors, Forking, Exec Family

stdio.h, unistd.h, fcntl.h, errno.h are tokenized manually via `strtok()` for maximum control.
- Redirection is handled using low-level file descriptors.
- Command history is logged using `fopen()` and stored persistently.

---

## 🖥 Sample Usage

kaushell$ ls
kaushell$ ls | sort
kaushell$ cat < file.txt > output.txt
kaushell$ logs

---

## 🧪 How to Run

```bash
gcc kaushell.c -o kaushell
./kaushell

Technologies Used
- C Language
- Linux System Calls
- File Descriptors, Forking, Exec Family
- stdio.h, unistd.h, fcntl.h, errno.h

Learning Objectives
- Understanding Unix process control (fork, exec, wait)
- Handling inter-process communication via pipes
- File redirection using system-level APIs
- Shell parsing and tokenization
- Working with file I/O (fopen, fputc, fgetc)
