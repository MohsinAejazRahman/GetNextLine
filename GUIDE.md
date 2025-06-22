# 📖 Introduction

This guide provides an in-depth exploration of various **strategies and implementation** details for the `GetNextLine` project. It complements the README by focusing on the core programming concepts, different data structures, and algorithmic approaches used to solve the problem effectively.

While the README covers project overview, compiling, and testing instructions, this document is aimed at students looking to **deepen their understanding** of memory management, buffer handling, and coding patterns applicable to this and similar projects.

Feel free to switch between both files as you progress and use the README for setup and project scope, and this guide for learning and implementation insights.

<br>

---

# 📑 Table of Contents

* [📖 Introduction](#-introduction)
* [📑 Table of Contents](#-table-of-contents)
* [🧠 Notes](#-notes)
    * [🧩 Static](#-static)
    * [🔧 Allowed External Functions](#-allowed-external-functions)
    * [🧪 Testing Functions](#-testing-functions)
    * [🧠 Memory Management](#-memory-management)
* [🧵 Solving with Strings](#-solving-with-strings)
    * [❓ Why strings?](#-why-strings)
    * [🤔 What to consider](#-what-to-consider)
    * [🧰 Typical functions you will write or reuse](#-typical-functions-you-will-write-or-reuse)
    * [👣 Step-by-step process](#-step-by-step-process)
* [🏗️ Solving with Structs](#️-solving-with-structs)
    * [❓ Why structs?](#-why-structs)
    * [🤔 What to consider](#-what-to-consider-1)
    * [🧰 Typical struct fields you will design](#-typical-struct-fields-you-will-design)
    * [👣 Step-by-step process](#-step-by-step-process-1)
* [📬 Solving with Queues](#-solving-with-queues)
    * [❓ Why queues?](#-why-queues)
    * [🤔 What to consider](#-what-to-consider-2)
    * [🧰 Typical queue data structures](#-typical-queue-data-structures)
    * [👣 Step-by-step process](#-step-by-step-process-2)

<br>

---

# 🧠 Notes

## 🧩 Static
In C, the keyword static is a storage class specifier that alters the lifetime, visibility (linkage), and storage location of variables and functions. 

- `static` tells the compiler that the variable or function it modifies should persist for the **entire duration of the program, not just the scope where it is declared.**
- It `changes the storage duration` **from automatic (stack-based) to static storage duration**, meaning the variable is allocated once and retains its value across multiple function calls or scopes.
- It affects `linkage`: **whether the symbol is visible** outside its translation unit (file).

Unlike ordinary local variables which are typically stored on the stack (automatic storage), static variables are stored in a data segment of the program's memory. The data segment is divided into:
- `Initialized data segment`: For static variables initialized with a value.
- `BSS segment`: For static variables initialized to zero or left uninitialized.

This memory is allocated once at program startup and freed only when the program terminates. Heap allocation (malloc etc.) is separate and unrelated to static. static variables have fixed memory reserved at compile/load time, not dynamic at runtime.

### ⚖️ Differences between static and non-static variables
| Aspect                  | Non-static local variable               | `static` local variable                |
| ----------------------- | --------------------------------------- | -------------------------------------- |
| **Storage duration**    | Automatic (stack)                       | Static (data segment)                  |
| **Lifetime**            | Until function returns                  | Entire program run                     |
| **Initialization**      | Undefined if not explicitly set         | Zero-initialized by default if omitted |
| **Value preservation**  | Does **not** retain value between calls | Retains value between function calls   |
| **Scope (visibility)**  | Only within the block/function          | Only within the block/function (scope) |
| **Linkage (if global)** | N/A                                     | Internal linkage (file scope)          |

### 🏢 Static at file/global scope

When used outside of a function, static changes linkage rather than storage duration (which is static by default for globals):

```c
static int counter = 0; // Visible only in this file (internal linkage)
int global_var = 5;     // Visible externally (external linkage)
```

Static global variables or functions are private to the file and cannot be accessed from other files, avoiding naming conflicts. Non-static globals have external linkage and can be referenced from other files.
 
`Functions` which are labelled static, they "remember" a value between calls without exposing it globally. `Global variables and fucntion` labelled static restrict the variable/function visibility to one source file, improving encapsulation and avoiding symbol collisions. Static `variables` are initialized only once at program startup. If you don't provide explicit initialization, they are zero-initialized automatically. 

Using static does not allocate memory on the stack, so you cannot take their address expecting stack lifetime behavior. Therefore, avoid overusing static in headers or shared libraries because it limits reusability and can cause unexpected duplication if used incorrectly.

## 🔧 Allowed External Functions
In the GetNextLine project, your implementation is restricted to using only a few standard library functions to manage reading and memory. Understanding how these functions behave is crucial for writing a correct and efficient solution.

### 📥 read()
The `read()` system call is fundamental to the GetNextLine project. It attempts to read a specified number of bytes from a file descriptor (fd) into a buffer. Its prototype is: 

```c
ssize_t read(int fd, void *buf, size_t count);
```

Input:
- `fd (file descriptor)` - (which must be open for reading)
- `*buf` where the read bytes will be stored
- `count` - the maximum number of bytes to read 

Output
- `number of bytes read` - which can be less than count
- `0` - end-of-file (EOF)
- `-1` if an error occurs (e.g., invalid fd)

In GNL, read() is called repeatedly until a newline is found or EOF is reached. Handling partial reads is essential since lines may be fragmented across multiple reads. Always check the return value to detect errors or EOF conditions.

```c
char buffer[BUFFER_SIZE];
ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);

if (bytes_read < 0)
    // Handle error
else if (bytes_read == 0)
    // EOF reached
else
    // Process buffer contents
```

### 🧠 malloc()
`malloc()` dynamically allocates memory from the heap and returns a pointer to it, or NULL if allocation fails. Its prototype is:

```c
void *malloc(size_t size);
```

Input:
- `size` -the number of bytes allocated

Output:
- `pointer to allocated block` - the returned memory is uninitialized

In GNL, malloc() is used to allocate buffers for assembling lines or storing temporary data. Checking for NULL is critical to prevent dereferencing invalid pointers. Always pair allocations with appropriate free() calls to avoid leaks.

Example snippet:

```c
char *line = malloc(100);
if (!line)
    // Handle allocation failure
```

### 🧹 free()
The `free()` function releases memory previously allocated by malloc() or related functions. Its prototype is:

```c
void free(void *ptr);
```

Input:
- `ptr` - a pointer to the allocated memory to be freed
- Passing invalid or already freed pointers leads to undefined behavior. 

In GNL, free() is used extensively to release temporary buffers, leftover memory, and queues when lines are fully processed or an error /EOF occurs. Proper freeing is crucial to maintain memory hygiene and avoid leaks.

Example snippet:

```c
free(line);
line = NULL; // Good practice to avoid dangling pointers
```

## 🧪 Testing Functions

### 🗂️ open()
`open()` is used to open files and obtain a file descriptor. Its prototype is:

```c
int open(const char *pathname, int flags);
```

Input:
- `pathname` - file path 
- `flags` - flags such as O_RDONLY for read-only access. 

Output:
- `fd` - a non-negative integer (the file descriptor), 
- `-1` - on failure (e.g., file not found or permission denied)

```c
int fd = open("file.txt", O_RDONLY);
if (fd < 0)
    // Handle open failure
```

Flags in open() are used to define the access mode, file creation rules, and additional settings that determine how the file is opened and accessed. This is done to ensure the file behaves exactly as needed for the specific operation, such as reading, writing, appending, or creating new files safely. These flags are defined in `<fcntl.h>`.

Flags can be combined with bitwise OR (`|`) to specify multiple behaviors. When using `O_CREAT`, a third argument specifying the file permission mode (e.g., `0644`) is required.

| Flag              | Description                                                      | Typical Usage                       |
|-------------------|------------------------------------------------------------------|-------------------------------------|
| `O_RDONLY`        | Open file for read-only access                                   | Read only                           |
| `O_WRONLY`        | Open file for write-only access                                  | Write only                          |
| `O_RDWR`          | Open file for both reading and writing                           | Read and write                      |
| `O_CREAT`         | Create file if it does not exist                                 | Requires a mode argument            |
| `O_EXCL`          | Ensure that `O_CREAT` fails if file exists                       | Used for exclusive file creation    |
| `O_TRUNC`         | Truncate file to zero length if it exists                        | Clear contents when opening         |
| `O_APPEND`        | Append data to the end of the file on each write                 | Writes will always add to file end  |
| `O_NONBLOCK`      | Open file in non-blocking mode                                   | For non-blocking I/O (e.g., pipes)  |
| `O_SYNC`          | Writes are synchronized (blocking until data is written to disk) | Ensure data integrity               |

In GNL testing, open() is used to acquire the file descriptor needed to call get_nodeext_line(). Always verify that open() succeeded before proceeding.

### 🛑 close()
`close()` closes an open file descriptor, freeing system resources. Its prototype is:

```c
int close(int fd);
```

Input:
- `fd` - file descriptor to the file to be closed

Output:
- `0` - on success 
- `-1` on failure (e.g., invalid fd). 

Closing file descriptors after use is a best practice that enhances system stability, security, and data correctness key factors that professional grade software must address. Properly closing file descriptors after use prevents resource leaks and ensures stability during multiple tests or long-running programs. 

`System resource management`: Each open file descriptor consumes a finite system resource. Most operating systems impose limits on how many file descriptors a single process or the entire system can have open simultaneously. If you fail to close fds properly, your program risks exhausting these limits, which leads to the inability to open new files or sockets and may cause unexpected program crashes or hangs. This issue becomes more pronounced in applications that handle many files or network connections concurrently.

`Security considerations`: Open file descriptors can inadvertently expose sensitive data or files if not closed promptly. Leaving files open longer than necessary can increase the attack surface, as malicious actors or buggy code might access or manipulate these files unexpectedly. Properly closing files ensures you minimize the window during which unauthorized access could occur.

`Data integrity`: Closing a file descriptor typically triggers the flushing of buffered data to disk, ensuring all writes are fully committed. Neglecting to close files properly may result in partial writes, data corruption, or loss, especially in applications where data consistency is critical.

`Clean and maintainable code`: Explicitly closing file descriptors signals to other developers (and future you) that resources are being managed consciously and responsibly. It helps avoid "resource leaks," which are often subtle bugs that degrade performance over time and are notoriously difficult to track down in large codebases.

In GNL tests, it is essential to always close file descriptors (fd) after you have finished reading or writing. This practice is crucial for several reasons, especially when scaling to larger projects or working in professional environments.

```c
if (close(fd) < 0)
    // Handle close error
```

## 🧠 Memory Management

Mastering memory management in C is essential for writing robust, efficient, and leak-free programs. Understanding memory types is crutial to manage memory properly:
- `Stack Memory`: Automatically managed, local variables are created and destroyed as functions enter and exit. You don't explicitly allocate or free stack memory
- `Heap Memory`: Manually managed via malloc(), calloc(), realloc(), and free(). Memory stays allocated until you explicitly free it

### ✅ Good Practices
`Always Check Allocation Results`

This is crutial to avoid undefined behaviour. Never assume memory allocation succeeds and always verify pointers before use:

```c
char *buffer = malloc(size);

if (!buffer)
{
    // Handle allocation failure immediately
    perror("malloc failed");
    exit(EXIT_FAILURE);
}
```

<br>

`Free Every Allocated Block`

Every malloc() or related call must be paired with a free() once the memory is no longer needed. Forgetting to free leads to memory leaks which cause programs to consume increasing memory over time.

```c
char *line = malloc(100);

if (!line)
    return NULL;

// Use line for processing...

free(line);  // Free memory as soon as done
line = NULL; // Avoid dangling pointer
```

<br>


`Avoid Double Freeing`

Calling free() on the same pointer more than once causes undefined behavior, often crashes. After freeing a pointer, set it to NULL to avoid accidental reuse.

```c
free(buffer);
buffer = NULL;
```

<br>


`Use Clear Ownership and Lifetimes`

Define clear ownership of memory in your program. Know which function or module is responsible for allocating and which for freeing. Passing ownership implicitly leads to confusion and leaks.

```c
// Function allocates memory and transfers ownership to the caller
char *read_line(int fd)
{
    char *line = malloc(1024);
    if (!line)
        return NULL;
    // read into line
    return line; // Caller must free
}

// Caller is responsible for freeing
char *line = read_line(fd);
if (line)
{
    // Use line
    free(line);  // Clear ownership: caller frees
}
```

<br>


`Minimize Dynamic Allocation Where Possible`

Don’t overuse dynamic memory if stack variables or static buffers suffice. Excessive allocations can fragment memory and reduce performance.

```c
// Using stack buffer instead of malloc
void process()
{
    char buffer[256];  // Automatically freed on function return
    // Use buffer safely without malloc/free overhead
}
```

<br>


`Keep Allocation and Deallocation Close`

Try to malloc() and free() in the same function or module, if possible. This makes tracking memory easier and reduces bugs.

```c
char *duplicate_string(const char *s)
{
    char *copy = malloc(strlen(s) + 1);
    if (!copy)
        return NULL;
    strcpy(copy, s);

    // Do some processing...

    free(copy);  // Allocation and deallocation in the same scope
    return NULL; // or return something else, no leaks here
}

```

<br>


`Use Tools to Detect Memory Issues`

Use Valgrind or similar memory checkers to detect leaks, invalid frees, or memory corruption. Regularly test your program with these tools during development.

```bash
valgrind --leak-check=full --show-leak-kinds=all ./your_program
```



<br>

---

# 🧵 Solving with Strings

This is the most `common solution` to GetNextLine using helper string functions from libft like ft_strchr, ft_strjoin, and ft_strlen. This approach is straightforward and accessible because many string utilities are already implemented or easy to adapt.

## ❓ Why strings?
Using strings to solve GetNextLine is a simple approach as many of the string manipulation functions you need are already implemented in libft and are easy to write yourself. Functions like `ft_strchr`, `ft_strjoin`, and `ft_strlen` make handling leftover buffers and incoming data easy. It allows you to treat the input as continuous text that you can search, join, and slice easily. 

This method keeps your code simple and intuitive because it relies on familiar operations. It’s good for those who prefer working with basic data types without introducing additional complexity through custom data structures. Additionally, debugging string based solutions tends to be easier to deal with text buffers.

## 🤔 What to consider
While the string-based approach is simple, it requires **careful management of memory**. You need to **ensure that every allocation has a corresponding free to avoid leaks**, especially when concatenating buffers or slicing lines. Edge cases like files without newlines, empty inputs, or very large lines demand thorough handling to avoid reading errors or crashes. The leftover buffer must always be properly null-terminated and consistent, or your searches for newline characters may fail or cause undefined behavior. Since you use repeated concatenations, be careful of efficiency and avoid unnecessary copies where possible.

## 🧰 Typical functions you will write or reuse:
- `ft_strchr` - to find \n in a buffer or leftover string.
- `ft_strjoin` - to concatenate leftover + new buffer content.
- `ft_substr` - to extract line or remainder.
- `Utility functions` - safe memory allocation and cleanup.

## 👣 Step-by-step process

1. `Validate inputs`: Check that file descriptor is valid, buffer size > 0, and accessible.
2. `Initialize static leftover buffer`: If none exists, start with empty or NULL.
3. `Search leftover for \n`: If found, split leftover into the line to return and new leftover.
    - If no \n in leftover, read from fd: Use read() to fill a temporary buffer.
4. `Append buffer to leftover`: Join the new buffer to leftover string.
5. `Repeat search for \n`: If found, split and return line; else continue reading.
    - If EOF reached: Return whatever is left as the last line or NULL if empty.
6. `Manage memory carefully`: Free old leftover strings after joining, and free returned line after use.
7. `Return line with newline` if present; else return last partial line on EOF.

<br>

---

# 🏗️ Solving with Structs

This approach `encapsulates` all relevant information (fd, buffer, indexes, length) into one place, making the code `cleaner and more maintainable` and is useful for the bonus part with multiple FDs. This implementation is also a great opportunity to practice defining, creating, and using structs that will be valuable for many other projects later on in the 42 curriculum.

## ❓ Why structs?
Choosing structs to organize your GetNextLine solution offers a clear, modular way to **encapsulate** all relevant state information for each file descriptor. By bundling the file descriptor, its buffer, the current read index, and the length of valid data into one structure, your code becomes cleaner, easier to maintain, and better organized. 

This encapsulation is particularly helpful when handling multiple file descriptors simultaneously (the bonus part), as you can store each FD’s state separately and avoid global variables or confusing static buffers. Structs help improve code clarity and make it simpler to debug, as all related data for an FD is kept in one place. It also makes memory cleanup easier because you can free the entire struct when done.

## 🤔 What to consider
When working with structs, the main considerations revolve around how to manage multiple file descriptors effectively. You’ll need a strategy for storing and retrieving each FD’s struct, such as a dynamic array, which introduces some complexity in your implementation. **Memory management is crucial** and you must allocate memory for buffers and structs and ensure you free everything properly when a file is closed or EOF is reached to avoid leaks. 

Keeping track of buffer indexes and lengths within the struct is important to correctly handle partial reads and leftover data. Although more complex than the string-only method, this approach scales better for real-world use cases involving multiple simultaneous file streams.

## 🧰 Typical struct fields you will design
```c
typedef struct s_fd_buffer {
    int fd;             // file descriptor
    char *buffer;       // leftover or current read buffer
    size_t buf_len;     // length of data in buffer
    size_t index;       // current read position in buffer
    struct s_fd_buffer *next; // pointer to next FD buffer (if linked list - bonus)
} t_fd_buffer;
```

## 👣 Step-by-step process
1. `Validate inputs`: Check that file descriptor is valid, buffer size > 0, and accessible.
2. `Find or create struct for FD`: Search your FD list or array; create if not found.
3. `Check leftover buffer inside struct for \n`: If newline found, split into return line and leftover.
    - If no newline, read from fd into a temporary buffer: Append this to the struct’s buffer.
4. `Update struct fields`: Adjust indexes and lengths to reflect consumed data.
6. `Repeat reading and scanning for newline`: Until a line is ready or EOF reached.
    - On EOF, return last remaining buffer content if any.
7. `Free struct`: remove from list when EOF or error occurs.
8. `Return malloc’d line`: including newline character if present.

<br>

---

# 📬 Solving with Queues

This is my own implementation of GetNextLine using linked lists as character queues. I chose this approach because I wanted to challenge myself and explore a different way to manage the input buffer dynamically. Big thanks to [AzerSD](https://github.com/AzerSD) for suggesting this method. I had a lot of fun working with linked lists, building the queue structure, and separating the reading logic from the storage, which gave me a fresh perspective on solving the problem.

## ❓ Why queues?
Using queues (implemented as linked lists) for GetNextLine offers a flexible and dynamic way to handle incoming characters one by one. Instead of managing a large contiguous string buffer, you store each character in a node that links to the next, forming a queue. This approach:
- **Avoids large memory reallocations** required by string concatenation.
- **Allows appending read characters** efficiently at the tail and consuming from the head.
- **Clearly separates reading input** (enqueueing) from line extraction (dequeuing).
- **Fits naturally with line-by-line reading** because you can stop appending once you hit \n.
- **Simplifies memory management** for partial reads and leftover data since each node can be freed individually.

This method also serves well for handling multiple file descriptors by maintaining separate queues for each FD (commonly implemented as linked lists of queues).

## 🤔 What to consider

While using queues offers great flexibility, it also introduces a fair amount of complexity. You need to define custom data structures—such as `t_node` nodes and `t_queue` queues and implement supporting functions to initialize the queue, append characters, extract lines, and free memory. `Careful memory management is essential`, as every node you malloc must eventually be freed to prevent leaks. Rebuilding the final line involves iterating through the queue and copying each character into a new, contiguous string, which adds extra overhead compared to using direct array indexing with strings. Additionally, the logic for buffering and reading becomes more intricate, since you have to determine exactly when to enqueue new characters and when to stop reading based on the presence of a newline character.

## 🧰 Typical queue data structures
```c
typedef struct s_node
{
    char            value;    // single character
    struct s_node   *next;    // pointer to next node in queue
}   t_node;

typedef struct s_queue
{
    t_node     *head;    // front of the queue (next char to read)
    t_node     *tail;    // end of the queue (where new chars are appended)
    int         size;     // number of chars currently stored
}   t_queue;

```

## 👣 Step-by-step process
1. `Validate inputs`: Check that file descriptor is valid, buffer size > 0, and accessible.
2. `Initialize queue`: Create a new queue if none exists for the current FD.
3. `Read from fd into buffer`: Use read() to fill a temporary buffer.
4. `Append buffer chars to queue`: For each character read, create a new node and add it to the queue’s tail.
5. `Stop reading when newline found`: Use a helper like ft_strchr to detect if \n is in the buffer; if yes, break reading loop.
6. `Calculate line length`: Count nodes in queue up to and including the first \n if present.
7. `Allocate line string`: Allocate a char * of size equal to the counted length + 1 for \0.
8. `Extract line from queue`: Remove nodes from the head of the queue, copying their characters into the allocated string.
9. `Free consumed nodes`: As you dequeue chars, free each node.
10. `Return the assembled line`: The returned string contains the line including the newline character if present.
11. `Free all on EOF or error`: If read returns 0 (EOF) and queue is empty, free the queue structure and return NULL.