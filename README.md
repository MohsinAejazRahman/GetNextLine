# 📚 GetNextLine - Reading a Line from a File Descriptor

`GetNextLine` is 1/3 of the mandatory projects in the 2nd circle of the 42 core curriculum. You are required to build a function that reads a line from a file ending with a newline character or EOF from a file descriptor, regardless of how many reads it takes. You must handle multiple buffers, static memory, edge cases, and memory management precisely while respecting the behavior of the system `read()` call. 

`get_next_line()` returns one full line of input per call, even when reading from slow streams or fragmented data. The bonus supports simultaneous reads from multiple file descriptors. This project is an introduction to file I/O, retaining data between executions, and buffer-based data processing. 

> 🔍 Note: 
> This document is not a tutorial and does not walk you through code implementation line by line. Instead, it provides a comprehensive overview and the theoretical groundwork for different ways to tackle the project. All actual implementation logic should be written and understood by you. The .c files in my srcs/ directory will contain documentation specific to the functions used in my solution (which is implemented using a queue-based approach for line assembly and buffer management).

Although this isn't a tutorial, this README was written specifically with GetNextLine in mind. The aim is to present common patterns and approaches such as using strings, structs, or queues while staying aligned with what the project actually expects. If you're looking for deeper explanations or broader discussions on C programming techniques feel free to check out my other projects. 

---

# 📑 Table of Contents

* [📁 Project Structure](#-project-structure)
* [📄 About the Project](#-about-the-project)
* [🧠 Notes](#-notes)
* [🧵 Solving with Strings](#-solving-with-strings)
* [🏗️ Solving with Structs](#-solving-with-structs)
* [📬 Solving with Queues](#-solving-with-queues)
* [📚 Additional Resources](#-additional-resources)

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

---

# 📄 About the Project

> 🔍 **Objective**:
> Implement a function that reads from a file descriptor, returning one line at a time, with each call.

---

# 🧠 Notes

> ⚠️ **Guidelines, quirks, and common pitfalls:**

* ✅ Using `static` variables for buffer preservation
* 🌀 Behavior of buffer with different file descriptors
* 🧵 Delimiting by newline (`\n`) and handling EOF
* 🚫 Known edge cases where GNL can fail
* 📋 Expected outputs and return rules
* 🧪 Guidelines for `BUFFER_SIZE` testing

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
