# 📚 GetNextLine – Reading a Line from a File Descriptor

`GetNextLine` is one of the mandatory projects in the 2nd circle of the 42 core curriculum. It requires building a function that reads a line from a file descriptor, returning a full line ending with a newline character or EOF, regardless of how many `read()` calls it takes. You must carefully handle multiple buffers, static memory, edge cases, and precise memory management while respecting the behavior of the system `read()` call.

The function `get_next_line()` returns one complete line of input per call, even when reading from slow streams or fragmented data. The bonus version supports simultaneous reads from multiple file descriptors. This project serves as an introduction to file I/O, persistent data handling between calls, and buffer-based processing.

> 🔍 **Note:**  
> This README provides a comprehensive overview and the theoretical groundwork of common approaches to solving the project, such as using strings, structs, or queues. It is **not** a line-by-line tutorial. The `.c` files in my `srcs/` directory include function-specific comments detailing my queue-based implementation for line assembly and buffer management.

> ❗ **For a much deeper understanding of the implementation details, memory management strategies, and algorithmic choices behind this project, please see the full [GUIDE.md](./GUIDE.md).**  

If you want to explore broader C programming concepts or detailed explanations, feel free to check out my other projects.

<br>

---

# 📑 Table of Contents

* [📚 GetNextLine](#-getnextline--reading-a-line-from-a-file-descriptor)
* [📑 Table of Contents](#-table-of-contents)
* [📁 Project Structure](#-project-structure)
* [📄 About the Project](#-about-the-project)
    * [🧾 Inputs and Outputs](#-inputs-and-outputs)
    * [📦 Normal vs Bonus](#-normal-vs-bonus)
    * [🌀 Behavior with File Descriptors](#-behavior-with-file-descriptors)
    * [🚫 Known Edge Cases & Failure Scenarios](#-known-edge-cases--failure-scenarios)
    * [📋 Expected Outputs & Return Rules](#-expected-outputs--return-rules)
    * [⚙️ BUFFER\_SIZE Guidelines](#️-buffer_size-guidelines)
* [🧪 Compiling and Testing](#-compiling-and-testing)
* [📚 Additional Resources](#-additional-resources)

<br>

---

# 📁 Project Structure

```
📁 GetNextLine/
├── GUIDE.md
├── README.md
└── srcs
    ├── Bonus
    │   ├── get_next_line_bonus.c
    │   ├── get_next_line_bonus.h
    │   └── get_next_line_utils_bonus.c
    └── Mandatory
        ├── get_next_line.c
        ├── get_next_line.h
        └── get_next_line_utils.c
```

<br>

---

# 📄 About the Project

 Implement a function `get_nodeext_line(int fd)` that returns the next line (including the trailing newline `\n` if present) from a file descriptor `fd`. Successive calls to the function should return successive lines until the end of the file is reached, at which point it returns `NULL`.

## 🧾 Inputs and Outputs

```c
char *get_nodeext_line(int fd);
```

### 🛠️ Inputs

* `int fd` — a file descriptor opened for reading.
* `BUFFER_SIZE` — a compile-time constant that defines how many bytes to read at once.

### 📤 Outputs

* A pointer to a dynamically allocated string ending with `\n` (if present in file), or without it at EOF.
* Returns `NULL` on:

  * Read error
  * EOF **after** all lines have been returned
  * Invalid file descriptor
  * `BUFFER_SIZE < 1`


## 📦 Normal vs Bonus

| Feature                          | Mandatory                | Bonus                          |
| -------------------------------- | ------------------------ | ------------------------------ |
| Single file descriptor           | ✅                        | ✅                              |
| Multiple file descriptors        | ❌ Undefined behavior     | ✅ Handles up to 4096 FDs       |
| Static memory per FD             | `static t_queue *queue;` | `static t_queue *queue[4096];` |
| Internal memory freeing          | On errors and EOF        | Per-FD cleanup                 |
| Memory persistence between calls | ✅                        | ✅                              |


## 🌀 Behavior with File Descriptors

### 🔁 Normal

The function **reuses a static queue** between function calls. Calling with a new `fd` overwrites the previous queue state, leading to undefined behavior if multiple files are open.

### 🧠 Bonus

Each `fd` has its **own isolated queue**, allowing simultaneous use of up to `4096` file descriptors. Each index in `queue[4096]` represents a unique buffer state for a specific `fd`.

---

## 🚫 Known Edge Cases & Failure Scenarios

| Scenario                                     | Behavior                                                |
| -------------------------------------------- | ------------------------------------------------------- |
| `fd < 0` or `BUFFER_SIZE < 1`                | Returns `NULL` immediately                              |
| `read(fd, NULL, 0) < 0`                      | Returns `NULL` and frees queue                          |
| `read_file` fails or returns 0               | Cleanup + return `NULL` (empty read or allocation fail) |
| `mem_size(queue) == 0`                       | Nothing left to extract → cleanup + return `NULL`       |
| File with no newline                         | Returns the last chunk without `\n`                     |
| Empty file                                   | Returns `NULL` immediately                              |
| Last line ends without `\n`                  | Still returned correctly, then `NULL`                   |
| Calling `get_nodeext_line()` again after `NULL` | Still returns `NULL`, no memory leaks                   |

---

## 📋 Expected Outputs & Return Rules

| File Content                    | Successive Calls                   | Final Call |
| ------------------------------- | ---------------------------------- | ---------- |
| `"Hello\nWorld\n"`              | `"Hello\n"`, `"World\n"`           | `NULL`     |
| `"Hello\n"`                     | `"Hello\n"`                        | `NULL`     |
| `"Hello"` *(no newline)*        | `"Hello"`                          | `NULL`     |
| `""` *(empty file)*             | `NULL`                             | -          |
| `BUFFER_SIZE` smaller than line | Assembles across multiple reads    | -          |
| `read()` returns 0 or < 0       | `NULL` with internal queue cleanup | -          |

---

## ⚙️ BUFFER\_SIZE Guidelines

`BUFFER_SIZE` directly affects performance, correctness, and edge behavior.

* ✅ Should be **greater than 0** for GNL to function at all
* ⚠️ Must be **tested with small values and large** (like 1, 1000) to ensure multi-read assembly logic works
* 🧪 You should test with:

  * Very **long lines** (e.g. > 10,000 characters)
  * Files with **only one very long line**
  * `BUFFER_SIZE == 1` to simulate char-by-char reading
  * `BUFFER_SIZE == exact size of line` (ensure newline is respected)
  * `BUFFER_SIZE > line length` (still returns only one line at a time)
* ⚠️ Behavior is **undefined** if `BUFFER_SIZE` is changed between calls

<br>

---

# 🧪 Compiling and Testing

You can compile your code using gcc with -Wall -Wextra -Werror flags as required by 42 Norms.

📦 Mandatory Version
```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    srcs/Mandatory/get_next_line.c \
    srcs/Mandatory/get_next_line_utils.c \
    main.c -o gnl  # You can also write a main() function inside get_next_line.c
```

📦 Bonus Version
```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    srcs/Bonus/get_next_line_bonus.c \
    srcs/Bonus/get_next_line_utils_bonus.c \
    main_bonus.c -o gnl  # You can also write a main() function inside get_next_line_bonus.c
```
You must define BUFFER_SIZE using `-D BUFFER_SIZE=X` when compiling, unless it's already defined in the header.

<br>

---

# 📚 Additional Resources

* For an in-depth walkthrough of design decisions, implementation strategies, and C concepts behind this project, see [GUIDE.md](./GUIDE.md).
* `man 2 read`, `man 2 open`, `man 3 malloc`
* [YouTube explanations](https://www.youtube.com/watch?v=8E9siq7apUU&pp=ygUKI2xpbmVyZWFkcw%3D%3D&themeRefresh=1)
