# UNIX File Operations System Calls

This document explains the fundamental system calls used for file handling in UNIX/Linux operating systems, as demonstrated in the accompanying C program `file_ops_syscalls.c`.

## Program Overview

The updated `file_ops_syscalls.c` is a **menu-driven program** that allows you to interactively perform file operations. Instead of running a fixed sequence of commands, you can choose which operation to perform at any time.

### Menu Options
1.  **Create/Open File**: Asks for a filename and opens it. If the file doesn't exist, it creates it. It uses `O_RDWR | O_CREAT` flags.
2.  **Write to File**: Takes user input and writes it to the currently open file.
3.  **Read from File**: Asks how many bytes to read and displays the content.
4.  **Lseek (Reposition Offset)**: Moves the file cursor to a specific position (Beginning, Current, or End).
5.  **Close File**: Closes the currently open file descriptor.
6.  **Exit**: Closes any open file and terminates the program.

---

## How to Check Which File is Open

### 1. In Your Program (Programmatic Approach)
The operating system gives you a **File Descriptor (FD)** when you open a file. This is just an integer (like an ID card).
- If `fd` is **-1**, no file is open (or the open failed).
- If `fd` is **> 2** (usually 3, 4, etc.), a file is open. (0, 1, and 2 are reserved for Standard Input, Output, and Error).

To know *which* file is open, your program must remember the filename string you passed to `open()`. In our C program, we store this in the `filename` variable and display it in the menu header.

### 2. In the Operating System (Command Line)
If you want to see which files a running process has open from the outside:
- **Linux/UNIX**: Use the `lsof` (List Open Files) command.
  ```bash
  lsof -p <Process_ID>
  ```
  Or check the `/proc` filesystem:
  ```bash
  ls -l /proc/<Process_ID>/fd
  ```
- **Windows**: You can use **Resource Monitor** or **Process Explorer** to search for file handles associated with a process.

---

## Deep Dive: `lseek()`

`lseek` stands for **L**ong **Seek**. It is used to change the **File Offset** (the "cursor" position) for the open file.

### Prototype
```c
off_t lseek(int fd, off_t offset, int whence);
```

### How it Works
Imagine a file as a long strip of paper. The "File Offset" is where your pen (for writing) or eye (for reading) is currently pointing.
- When you `open()` a file, the offset is usually at **0** (the beginning).
- When you `read()` or `write()` 10 bytes, the offset automatically moves forward by 10.
- `lseek()` lets you manually teleport that cursor without reading or writing.

### The `whence` Argument (Reference Point)
The third argument tells `lseek` where to start counting the offset from:

| Constant | Value | Meaning | Example |
| :--- | :--- | :--- | :--- |
| **SEEK_SET** | `0` | **Absolute position**. Offset is calculated from the **beginning** of the file. | `lseek(fd, 10, SEEK_SET)` moves to byte 10. |
| **SEEK_CUR** | `1` | **Relative position**. Offset is calculated from the **current** cursor position. | `lseek(fd, -5, SEEK_CUR)` moves back 5 bytes. |
| **SEEK_END** | `2` | **End of file**. Offset is calculated from the **end** of the file. | `lseek(fd, 0, SEEK_END)` moves to the very end. |

### Common Uses of `lseek`
1.  **Rewind**: Go back to the start to read the file again.
    ```c
    lseek(fd, 0, SEEK_SET);
    ```
2.  **Find File Size**: Go to the end and check the return value (which is the total number of bytes).
    ```c
    off_t file_size = lseek(fd, 0, SEEK_END);
    ```
3.  **Append (Manually)**: Go to the end before writing (though opening with `O_APPEND` is safer).
4.  **Create Sparse Files**: Seek way past the end of the file and write something. The gap in between will be filled with null bytes (0) and may not take up physical disk space.

---

## Key System Calls Explained

### 1. `open()`
- **Purpose**: Opens a file and returns a file descriptor.
- **Prototype**: `int open(const char *pathname, int flags, mode_t mode);`
- **Arguments**:
    - `pathname`: Path to the file.
    - `flags`: Access mode. We use `O_RDWR` (Read/Write) combined with `O_CREAT` (Create if missing).
    - `mode`: File permissions (e.g., `0644` - rw-r--r--) used when creating a new file.
- **Return**: File descriptor on success, `-1` on error.

### 2. `write()`
- **Purpose**: Writes data from a buffer to a file.
- **Prototype**: `ssize_t write(int fd, const void *buf, size_t count);`
- **Return**: Number of bytes written on success, `-1` on error.

### 3. `read()`
- **Purpose**: Reads data from a file into a buffer.
- **Prototype**: `ssize_t read(int fd, void *buf, size_t count);`
- **Return**: Number of bytes read on success (0 indicates EOF), `-1` on error.

### 4. `close()`
- **Purpose**: Closes a file descriptor, releasing the resource.
- **Prototype**: `int close(int fd);`
- **Return**: `0` on success, `-1` on error.

## Note for Windows Users
These system calls are native to UNIX/Linux (POSIX). On Windows, standard C compilers (like MSVC) might not support all of them directly or might use slightly different names (e.g., `_open`, `_write`). However, tools like MinGW or running inside WSL (Windows Subsystem for Linux) allow you to compile and run this code as intended.
