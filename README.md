# 📚 GetNextLine - Reading a Line from a File Descriptor

`GetNextLine` is 1/3 of the mandatory projects in the 2nd circle of the 42 core curriculum. You are required to build a function that reads a line from a file ending with a newline character or EOF from a file descriptor, regardless of how many reads it takes. You must handle multiple buffers, static memory, edge cases, and memory management precisely while respecting the behavior of the system `read()` call. 

`get_next_line()` returns one full line of input per call, even when reading from slow streams or fragmented data. The bonus supports simultaneous reads from multiple file descriptors. This project is an introduction to file I/O, retaining data between executions, and buffer-based data processing. 

> 🔍 Note: 
> This document is not a tutorial and does not walk you through code implementation line by line. Instead, it provides a comprehensive overview and the theoretical groundwork for different ways to tackle the project. All actual implementation logic should be written and understood by you. The .c files in my srcs/ directory will contain documentation specific to the functions used in my solution (which is implemented using a queue-based approach for line assembly and buffer management).

Although this isn't a tutorial, this README was written specifically with GetNextLine in mind. The aim is to present common patterns and approaches such as using strings, structs, or queues while staying aligned with what the project actually expects. If you're looking for deeper explanations or broader discussions on C programming techniques feel free to check out my other projects. 

<br>

---

# 📑 Table of Contents

* [📁 Project Structure](#-project-structure)
* [📄 About the Project](#-about-the-project)
* [🧠 Notes](#-notes)
* [🧵 Solving with Strings](#-solving-with-strings)
* [🏗️ Solving with Structs](#-solving-with-structs)
* [📬 Solving with Queues](#-solving-with-queues)
* [📚 Additional Resources](#-additional-resources)

<br>

---

# 📁 Project Structure

```
📁 GetNextLine/
├── README.md
└── srcs/
    ├── get_next_line.c
    ├── get_next_line.h
    ├── get_next_line_bonus.c
    ├── get_next_line_bonus.h
    ├── get_next_line_utils.c
    └── get_next_line_utils_bonus.c
```

<br>

---

# 📄 About the Project

 Implement a function `get_next_line(int fd)` that returns the next line (including the trailing newline `\n` if present) from a file descriptor `fd`. Successive calls to the function should return successive lines until the end of the file is reached, at which point it returns `NULL`.

## 🧾 Inputs and Outputs

```c
char *get_next_line(int fd);
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
| Calling `get_next_line()` again after `NULL` | Still returns `NULL`, no memory leaks                   |

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

## 🧪 Compiling and Testing

You can compile your code using gcc with -Wall -Wextra -Werror flags as required by 42 Norms.

📦 Mandatory Version
```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    srcs/get_next_line.c \
    srcs/get_next_line_utils.c \
    main.c -o gnl #You can also write a main() function inside get_next_line.c
```

📦 Bonus Version
```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    srcs/get_next_line_bonus.c \
    srcs/get_next_line_utils_bonus.c \
    main_bonus.c -o gnl #You can also write a main() function inside get_next_line_bonus.c
```
You must define BUFFER_SIZE using `-D BUFFER_SIZE=X` when compiling, unless it's already defined in the header.


<br>

---

# 🧠 Notes

* ✅ Using `static` variables for buffer preservation


---

# 🧵 Solving with Strings

> A traditional, flat-buffer solution using helper string functions like `strchr`, `strjoin`, and `strlen`.

* 📦 How to use a `static` leftover buffer
* 🔄 Manual memory management of each new line
* 🔍 Searching for `\n` and substring slicing
* 💡 Tips for minimizing memory leaks

---

# 🏗️ Solving with Structs

> Modularize your state across file descriptors using custom-defined structs.

* 📐 Struct layout to store fd, buffer, length, and index
* 🧼 Easier memory cleanup through one container
* 🧠 Better clarity and isolation for each FD
* 🔄 Maintaining linked-lists or arrays of structs

---

# 📬 Solving with Queues

> A dynamic, flexible approach using linked lists as character queues.

* 📫 Appending characters into a line queue
* 🧺 Rebuilding the line with `malloc` after `\n`
* 🔄 Decouples read logic from storage logic
* ✂️ Easy implementation of multi-FD logic

---

# 📚 Additional Resources

> 📌 Recommended docs, man pages, and 42 threads:

* `man 2 read`, `man 2 open`, `man 3 malloc`
* YouTube explanations: \[GNL with structs], \[GNL queue walkthrough]
* Memory visualization tools: \[Valgrind], \[malloc\_debugger]

---

Would you like me to begin writing the full content for any of the sections above?
