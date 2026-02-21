CM Library Documentation 📚

A Smart Layer for C Programming Language

---

📋 Table of Contents

1. Introduction
2. Installation & Setup
3. Quick Start
4. Core Concepts
5. String Functions
6. Array Functions
7. Map Functions
8. Error Handling
9. Utility Functions
10. Garbage Collector
11. Examples
12. Best Practices
13. Troubleshooting

---

📖 Introduction

```
╔═══════════════════════════════════════════════════════════════╗
║            CM - C Multitask Intelligent Library               ║
║                   Version 3.0.0                               ║
║                    by Adham Hossam                            ║
╠═══════════════════════════════════════════════════════════════╣
║  "Made for fun, built for power!"                             ║
║                                                               ║
║  CM adds modern programming features to C:                    ║
║  • Automatic Garbage Collection                               ║
║  • Smart Strings (like Python)                                ║
║  • Dynamic Arrays (like C++ vector)                           ║
║  • Hash Maps (like Python dict)                               ║
║  • Try/Catch Error Handling                                   ║
║  • Memory Leak Detection                                      ║
╚═══════════════════════════════════════════════════════════════╝
```

Note: This library was created for educational purposes and fun! 🎮

---

💻 Installation & Setup

Option 1: Single File in Root Directory

```
your_project/
├── CM_full.h          # ← Put this file here
├── main.c             # ← Your code
└── program.exe        # ← Compiled program
```

In main.c:

```c
#define CM_IMPLEMENTATION
#include "CM_full.h"    // Just include the file!

int main() {
    // Your code here
    return 0;
}
```

Option 2: Organized Folder Structure

```
your_project/
├── CM/                 # ← Create CM folder
│   └── CM_full.h      # ← Put file here
└── main.c             # ← Your code
```

In main.c:

```c
#define CM_IMPLEMENTATION
#include "CM/CM_full.h"  // Include from folder

int main() {
    // Your code here
    return 0;
}
```

⚙️ Compilation

```bash
# Simple compilation
clang main.c -o program

# With warnings enabled
clang -Wall -Wextra main.c -o program

# Run the program
./program
```

---

🚀 Quick Start

```c
#define CM_IMPLEMENTATION
#include "CM_full.h"

int main() {
    // Show library info
    CM_ABOUT();
    
    // === STRING EXAMPLE ===
    cm_string_t* name = CM_STR("Adham");
    cm_string_t* greeting = cm_string_format("Hello, %s!", name->data);
    printf("%s\n", greeting->data);  // Output: Hello, Adham!
    
    // === ARRAY EXAMPLE ===
    cm_array_t* numbers = CM_ARR(int, 5);
    CM_ARR_PUSH(numbers, int, 42);
    CM_ARR_PUSH(numbers, int, 100);
    
    int first = CM_ARR_GET(numbers, 0, int);
    printf("First: %d\n", first);    // Output: First: 42
    
    // === MAP EXAMPLE ===
    cm_map_t* dict = CM_MAP();
    CM_MAP_SET_INT(dict, "age", 25);
    
    if (CM_MAP_HAS(dict, "age")) {
        int age = CM_MAP_GET_INT(dict, "age");
        printf("Age: %d\n", age);     // Output: Age: 25
    }
    
    // === ERROR HANDLING ===
    CM_TRY() {
        int x = -1;
        if (x < 0) {
            CM_THROW(CM_ERROR_INVALID_ARGUMENT, "No negatives!");
        }
    } CM_CATCH() {
        printf("Error: %s\n", cm_error_get_message());
    }
    
    // === CLEANUP ===
    CM_STR_FREE(name);
    CM_STR_FREE(greeting);
    CM_ARR_FREE(numbers);
    CM_MAP_FREE(dict);
    CM_REPORT();  // Check for memory leaks
    
    return 0;
}
```

---

🎯 Core Concepts

Concept Description Example
CM_IMPLEMENTATION Must be defined once before including the library #define CM_IMPLEMENTATION
Memory Management Automatic GC tracks all allocations cm_alloc() / cm_free()
Reference Counting Objects are freed when ref_count hits 0 cm_retain() / cm_release()
Type Safety Macros preserve type information CM_ARR_GET(arr, i, int)

---

📝 String Functions

String Type

```c
typedef struct cm_string {
    char* data;      // Actual string data
    size_t length;   // Current length
    size_t capacity; // Allocated capacity
    int ref_count;   // Reference count
    // ... more fields
} cm_string_t;
```

String Macros

Macro Description Example
CM_STR(s) Create new string cm_string_t* s = CM_STR("hello");
CM_STR_FREE(s) Free string CM_STR_FREE(s);
CM_STR_LEN(s) Get string length size_t len = CM_STR_LEN(s);

String Functions

Function Description Example
cm_string_new(initial) Create new string cm_string_t* s = cm_string_new("Hi");
cm_string_free(s) Free string cm_string_free(s);
cm_string_format(fmt, ...) Formatted string cm_string_format("x=%d", 42);
cm_string_set(s, value) Set new value cm_string_set(s, "new");
cm_string_upper(s) Convert to uppercase cm_string_upper(s);
cm_string_lower(s) Convert to lowercase cm_string_lower(s);
cm_string_length(s) Get length size_t len = cm_string_length(s);

---

📊 Array Functions

Array Type

```c
typedef struct cm_array {
    void* data;             // Elements
    size_t element_size;    // Size of each element
    size_t length;          // Current number of elements
    size_t capacity;        // Allocated capacity
    int* ref_counts;        // Reference counts per element
} cm_array_t;
```

Array Macros

Macro Description Example
CM_ARR(type, size) Create array cm_array_t* a = CM_ARR(int, 10);
CM_ARR_FREE(a) Free array CM_ARR_FREE(a);
CM_ARR_LEN(a) Get length size_t len = CM_ARR_LEN(a);
CM_ARR_GET(a, i, type) Get element int x = CM_ARR_GET(a, 0, int);
CM_ARR_PUSH(a, type, v) Push element CM_ARR_PUSH(a, int, 42);

Array Functions

Function Description Example
cm_array_new(elem_size, cap) Create array cm_array_t* a = cm_array_new(sizeof(int), 5);
cm_array_free(a) Free array cm_array_free(a);
cm_array_get(a, i) Get element pointer int* p = cm_array_get(a, 0);
cm_array_push(a, value) Push element int x = 42; cm_array_push(a, &x);
cm_array_pop(a) Pop last element int* p = cm_array_pop(a);
cm_array_length(a) Get length size_t len = cm_array_length(a);

---

🗺️ Map Functions

Map Type

```c
typedef struct cm_map {
    cm_map_entry_t** buckets;  // Hash buckets
    int bucket_count;           // Number of buckets
    int size;                   // Number of entries
    float load_factor;          // Load factor for resizing
} cm_map_t;
```

Map Macros

Macro Description Example
CM_MAP() Create map cm_map_t* m = CM_MAP();
CM_MAP_FREE(m) Free map CM_MAP_FREE(m);
CM_MAP_SET_INT(m, k, v) Set int value CM_MAP_SET_INT(m, "age", 25);
CM_MAP_SET_STRING(m, k, v) Set string value CM_MAP_SET_STRING(m, "name", "Adham");
CM_MAP_GET_INT(m, k) Get int value int age = CM_MAP_GET_INT(m, "age");
CM_MAP_GET_STRING(m, k) Get string value char* name = CM_MAP_GET_STRING(m, "name");
CM_MAP_HAS(m, k) Check if key exists if (CM_MAP_HAS(m, "age")) {...}

Map Functions

Function Description Example
cm_map_new() Create map cm_map_t* m = cm_map_new();
cm_map_free(m) Free map cm_map_free(m);
cm_map_set(m, key, value, size) Set value int x = 42; cm_map_set(m, "key", &x, sizeof(int));
cm_map_get(m, key) Get value int* p = cm_map_get(m, "key");
cm_map_has(m, key) Check existence if (cm_map_has(m, "key")) {...}
cm_map_size(m) Get number of entries size_t n = cm_map_size(m);

---

🚨 Error Handling

Error Codes

Code Value Description
CM_SUCCESS 0 Success
CM_ERROR_MEMORY 1 Memory allocation failed
CM_ERROR_NULL_POINTER 2 Null pointer dereference
CM_ERROR_OUT_OF_BOUNDS 3 Index out of bounds
CM_ERROR_DIVISION_BY_ZERO 4 Division by zero
CM_ERROR_INVALID_ARGUMENT 7 Invalid argument
CM_ERROR_NOT_FOUND 8 Not found
... ... ...

Error Macros

Macro Description Example
CM_TRY() Start try block CM_TRY() { ... }
CM_CATCH() Catch errors } CM_CATCH() { ... }
CM_THROW(error, msg) Throw error CM_THROW(7, "Invalid!");

Error Functions

Function Description Example
cm_error_string(code) Get error description cm_error_string(7);
cm_error_get_message() Get last error message const char* msg = cm_error_get_message();
cm_error_get_last() Get last error code int code = cm_error_get_last();
cm_error_clear() Clear last error cm_error_clear();
cm_error_set(code, msg) Set error manually cm_error_set(7, "Oops!");

---

🎲 Utility Functions

Function Description Example
cm_random_seed(seed) Set random seed cm_random_seed(time(NULL));
cm_random_int(min, max) Random integer int r = cm_random_int(1, 100);
cm_random_string(buf, len) Random string char s[20]; cm_random_string(s, 20);
CM_RAND_INT(min, max) Random int macro int r = CM_RAND_INT(1, 100);

---

🧹 Garbage Collector

GC Functions

Function Description Example
cm_gc_init() Initialize GC Called automatically
cm_gc_collect() Force collection cm_gc_collect();
cm_gc_stats() Show GC statistics cm_gc_stats();
CM_REPORT() Show GC report CM_REPORT();
cm_alloc(size, type, file, line) Allocate tracked memory Used internally
cm_free(ptr) Free memory Used internally
cm_retain(ptr) Increment ref count cm_retain(obj);

GC Statistics Output

```
╔════════════════════════════════════════════════════════════╗
║              GARBAGE COLLECTOR STATISTICS                 ║
╠════════════════════════════════════════════════════════════╣
║ Total objects:                       0                    ║
║ Total memory:                        0 bytes              ║
║ Peak memory:                       104 bytes              ║
║ Allocations:                         5                    ║
║ Frees:                               5                    ║
║ Collections:                         0                    ║
╚════════════════════════════════════════════════════════════╝
```

---

💡 Examples

Example 1: Stack (LIFO)

```c
cm_array_t* stack = CM_ARR(int, 2);

// Push elements
for(int i = 0; i < 5; i++) {
    CM_ARR_PUSH(stack, int, i * 10);
}

// Pop elements
while (CM_ARR_LEN(stack) > 0) {
    int* val = cm_array_pop(stack);
    printf("Popped: %d\n", *val);
}

CM_ARR_FREE(stack);
```

Example 2: Dictionary

```c
cm_map_t* dict = CM_MAP();

// Store different types
CM_MAP_SET_INT(dict, "age", 25);
CM_MAP_SET_STRING(dict, "name", "Adham");
CM_MAP_SET_INT(dict, "score", 100);

// Retrieve values
if (CM_MAP_HAS(dict, "name")) {
    char* name = CM_MAP_GET_STRING(dict, "name");
    int age = CM_MAP_GET_INT(dict, "age");
    printf("%s is %d years old\n", name, age);
}

CM_MAP_FREE(dict);
```

Example 3: String Manipulation

```c
cm_string_t* text = CM_STR("  Hello World  ");

printf("Original: '%s'\n", text->data);
printf("Length: %zu\n", CM_STR_LEN(text));

cm_string_upper(text);
printf("Uppercase: '%s'\n", text->data);

cm_string_lower(text);
printf("Lowercase: '%s'\n", text->data);

// Trim (not implemented in basic version)
// cm_string_trim(text);
// printf("Trimmed: '%s'\n", text->data);

CM_STR_FREE(text);
```

Example 4: Error Handling

```c
CM_TRY() {
    cm_array_t* arr = CM_ARR(int, 3);
    
    // This is fine
    CM_ARR_PUSH(arr, int, 10);
    
    // This will cause error (out of bounds? no, array auto-resizes)
    for(int i = 0; i < 10; i++) {
        CM_ARR_PUSH(arr, int, i * 10);
    }
    
    // Simulate an error
    if (CM_ARR_LEN(arr) > 5) {
        CM_THROW(CM_ERROR_INVALID_ARGUMENT, "Array too big!");
    }
    
    CM_ARR_FREE(arr);
    
} CM_CATCH() {
    printf("Error: %s\n", cm_error_get_message());
    printf("Code: %d - %s\n", cm_error_get_last(), 
           cm_error_string(cm_error_get_last()));
}
```

---

✅ Best Practices

Practice Why? Example
Always free what you allocate Prevent memory leaks CM_ARR_FREE(arr);
Check for NULL Avoid crashes if (ptr) cm_free(ptr);
Use type-specific macros Type safety CM_ARR_GET(arr, i, int)
Check map keys exist Avoid NULL dereference if (CM_MAP_HAS(map, "key"))
Use error handling Robust code CM_TRY() { ... } CM_CATCH()
Call CM_REPORT() at end Verify no leaks CM_REPORT();
One CM_IMPLEMENTATION Avoid multiple definitions Only in one .c file

---

🔧 Troubleshooting

Common Errors

Error Cause Solution
undefined reference to 'cm_...' Missing CM_IMPLEMENTATION Add #define CM_IMPLEMENTATION before include
multiple definition of 'cm_...' CM_IMPLEMENTATION in multiple files Put it in ONLY ONE .c file
Total objects > 0 at exit Memory leak Check you freed all objects with CM_..._FREE
segmentation fault Using freed memory Don't use pointers after freeing
CM_TRY() not found Missing error macros Update to latest version

Memory Leak Detection

```
⚠️ [CM] Warning: 3 objects still alive
```

This means you forgot to free 3 objects! Find them with CM_REPORT() and add CM_..._FREE.

---

📦 File Structure Reference

Complete CM_full.h Structure

```
CM_full.h (2500+ lines)
├── Headers & Includes
├── Configuration
├── Error Codes
├── Type Definitions
├── Global Variables
├── Macros
├── Function Declarations
├── GC Implementation
├── Error Handling
├── String Implementation
├── Array Implementation
├── Map Implementation
├── Utilities
└── Initialization
```

---

🎮 Fun Facts

· This library was built on Android using CXXDroid! 📱
· Over 2500 lines of pure C code 💪
· No external dependencies - pure C standard library 📚
· Memory safe - automatic GC prevents leaks 🛡️
· Type safe - macros preserve type information 🔒
· Educational - learn how memory management works 🎓

---

📝 License

```
This library is free to use, modify, and distribute.
Created for educational purposes and fun!
No warranty, use at your own risk.
```

---

👨‍💻 Author

Adham Hossam

· Created: 2026
· Version: 3.0.0
· Made with ❤️ for C programmers

---

📬 Contact & Contributions

Found a bug? Want to contribute? Feel free to:

· ⭐ Star the project
· 🐛 Report issues
· 🔧 Submit fixes
· 📚 Improve docs

---

🏁 Final Words

```
╔═══════════════════════════════════════════════════════════════╗
║  "I made this for fun, but it became a real library!         ║
║   Hope you enjoy using it as much as I enjoyed making it."   ║
║                    - Adham Hossam                            ║
╚═══════════════════════════════════════════════════════════════╝
```

Happy Coding! 🚀

---

Documentation generated with ❤️ for CM Library v3.0.0