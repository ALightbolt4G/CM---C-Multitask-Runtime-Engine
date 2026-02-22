📚 CM Library Documentation v3.1.1

The Intelligent C Programming Library

---

```markdown
# CM Library v3.1.1 Documentation

<div align="center">
  
![CM Library](https://img.shields.io/badge/C%20Library-v3.1.1-blue)
![Author](https://img.shields.io/badge/Author-Adham%20Hossam-green)
![License](https://img.shields.io/badge/License-MIT-orange)
![Size](https://img.shields.io/badge/Size-26KB-lightgrey)

**C Multitask - Intelligent Layer for C Programming**

</div>

---

## 📋 Table of Contents
1. [Introduction](#introduction)
2. [Quick Start](#quick-start)
3. [Installation](#installation)
4. [Core Features](#core-features)
5. [Garbage Collector](#garbage-collector)
6. [String Class](#string-class)
7. [Array Class](#array-class)
8. [Map Class](#map-class)
9. [Error Handling](#error-handling)
10. [Utility Functions](#utility-functions)
11. [Memory Management](#memory-management)
12. [Examples](#examples)
13. [Best Practices](#best-practices)
14. [Troubleshooting](#troubleshooting)
15. [API Reference](#api-reference)
16. [Version History](#version-history)

---

## 📖 Introduction

CM Library is a **complete intelligent layer** over C programming language that adds modern features:

```c
// What you get:
✅ Automatic Garbage Collection  (like Java)
✅ Smart Strings                 (like Python)
✅ Dynamic Arrays                (like C++ vector)
✅ Hash Maps                     (like Python dict)
✅ Try/Catch Error Handling      (like C++/Java)
✅ Memory Leak Detection         (like Valgrind)
✅ OOP-style Classes             (like C++/Java)
✅ Zero external dependencies    (pure C)
✅ Single header file            (easy to use)
✅ 26KB binary size              (incredibly small)
```

Author: Adham Hossam
Version: 3.1.1
Released: 2026
Lines of Code: 3000+

---

⚡ Quick Start

10 seconds to first program:

Step 1: Create main.c

```c
#define CM_IMPLEMENTATION   // Required once
#include "CM_full.h"        // Include library

int main() {
    CM_ABOUT();  // Show library info
    
    // Create a string
    String* str = String_new("Hello World");
    str->print(str);           // Output: Hello World
    str->upper(str);           // Convert to uppercase
    str->print(str);           // Output: HELLO WORLD
    
    String_delete(str);        // Cleanup
    cmStats();                 // Show memory stats
    
    return 0;
}
```

Step 2: Compile and run

```bash
clang main.c -o program
./program
```

Step 3: Enjoy! 🎉

---

📦 Installation

Option 1: Single File (Easiest)

```
your_project/
├── CM_full.h      ← Download and place here
└── main.c
```

Option 2: Organized Folder

```
your_project/
├── CM/
│   └── CM_full.h
└── main.c
```

Option 3: Multiple Files (Advanced)

```
your_project/
├── CM/
│   ├── CM.h
│   ├── CM_core.h
│   ├── CM_string.h
│   ├── CM_array.h
│   ├── CM_map.h
│   ├── CM_error.h
│   ├── CM_utils.h
│   └── CM_init.h
└── main.c
```

Download Links:

· 📥 CM_full.h (Single file)
· 📥 CM Library Pack

---

🎯 Core Features

Feature Comparison

Feature Standard C CM Library
Memory Management Manual malloc/free Automatic GC
Strings char* with risks Safe String class
Arrays Fixed size Dynamic Array class
Dictionaries None Map class
Error Handling errno try/catch macros
Object-Oriented No Classes with methods
Memory Leak Detection Manual Automatic
Binary Size 16KB 26KB (+10KB)

Memory Usage per Object

Object Type Size Overhead
String 32 bytes + data 16 bytes
Array 24 bytes + data 16 bytes
Map Entry 48 bytes 24 bytes
GC Object 48 bytes -

---

♻️ Garbage Collector

Core Functions

```c
// Memory allocation
void* cmAlloc(size_t size);                    // Allocate memory
void cmFree(void* ptr);                         // Free memory
void cmRetain(void* ptr);                       // Increment ref count
void cmRelease(void* ptr);                       // Decrement ref count

// GC Control
void cmGC();                                     // Force collection
void cmStats();                                  // Show statistics
void cmGCSetThreshold(int bytes);                // Set GC threshold
void cmGCEnable(int enable);                      // Enable/disable GC
```

GC Statistics Output

```
╔════════════════════════════════════════════════════════════╗
║              GARBAGE COLLECTOR STATISTICS                 ║
╠════════════════════════════════════════════════════════════╣
║ Total objects:                       0                    ║
║ Total memory:                        0 bytes              ║
║ Peak memory:                       248 bytes              ║
║ Allocations:                         9                    ║
║ Frees:                               9                    ║
║ Collections:                         1                    ║
╚════════════════════════════════════════════════════════════╝
```

How GC Works

```c
// 1. Allocation tracking
void* ptr = cmAlloc(100);  // Object tracked by GC

// 2. Reference counting
cmRetain(ptr);   // ref_count = 2
cmRelease(ptr);  // ref_count = 1
cmFree(ptr);     // ref_count = 0 → freed

// 3. Automatic collection at exit
// No leaks guaranteed!
```

---

📝 String Class

String Structure

```c
typedef struct String {
    char* data;           // String data
    int length;            // Current length
    int capacity;          // Allocated capacity
    
    // Methods
    struct String* (*concat)(struct String* this, const char* other);
    struct String* (*append)(struct String* this, const char* suffix);
    struct String* (*prepend)(struct String* this, const char* prefix);
    struct String* (*upper)(struct String* this);
    struct String* (*lower)(struct String* this);
    struct String* (*trim)(struct String* this);
    struct String* (*substring)(struct String* this, int start, int end);
    int (*find)(struct String* this, const char* substr);
    int (*length)(struct String* this);
    char (*charAt)(struct String* this, int index);
    void (*print)(struct String* this);
} String;
```

String Functions

Function Description Example
String_new(initial) Create new string String* s = String_new("Hi");
String_delete(s) Free string String_delete(s);
s->concat(s, " world") Concatenate s->concat(s, "!");
s->upper(s) Convert to uppercase s->upper(s);
s->lower(s) Convert to lowercase s->lower(s);
s->trim(s) Remove whitespace s->trim(s);
s->find(s, "sub") Find substring int pos = s->find(s, "lo");
s->length(s) Get length int len = s->length(s);
s->charAt(s, 0) Get character char c = s->charAt(s, 0);
s->print(s) Print string s->print(s);

String Examples

```c
String* s = String_new("  Hello World  ");

// Basic operations
s->print(s);              // Output:   Hello World  
printf("Length: %d\n", s->length(s));  // 14

// Transformations
s->trim(s);
s->print(s);              // Output: Hello World
s->upper(s);
s->print(s);              // Output: HELLO WORLD

// Concatenation
s->concat(s, "!!!");
s->print(s);              // Output: HELLO WORLD!!!

// Find
int pos = s->find(s, "WORLD");
printf("Found at: %d\n", pos);  // 6

// Substring
String* sub = s->substring(s, 0, 5);
sub->print(sub);          // Output: HELLO

// Cleanup
String_delete(s);
String_delete(sub);
```

---

📦 Array Class

Array Structure

```c
typedef struct Array {
    void* data;            // Array elements
    int element_size;       // Size of each element
    int length;             // Current number of elements
    int capacity;           // Allocated capacity
    
    // Methods
    struct Array* (*push)(struct Array* this, void* value);
    void* (*pop)(struct Array* this);
    void* (*get)(struct Array* this, int index);
    void (*set)(struct Array* this, int index, void* value);
    int (*size)(struct Array* this);
    int (*isEmpty)(struct Array* this);
    void (*clear)(struct Array* this);
} Array;
```

Array Functions

Function Description Example
Array_new(elem_size, cap) Create array Array* a = Array_new(sizeof(int), 10);
Array_delete(a) Free array Array_delete(a);
a->push(a, &value) Add element int x = 42; a->push(a, &x);
a->pop(a) Remove last int* p = a->pop(a);
a->get(a, i) Get element int* p = a->get(a, 0);
a->set(a, i, &v) Set element int v = 100; a->set(a, 1, &v);
a->size(a) Get size int len = a->size(a);
a->isEmpty(a) Check empty if (a->isEmpty(a)) ...
a->clear(a) Clear array a->clear(a);

Array Examples

```c
// Integer array
Array* arr = Array_new(sizeof(int), 5);

// Push elements
int values[] = {10, 20, 30, 40, 50};
for (int i = 0; i < 5; i++) {
    arr->push(arr, &values[i]);
}

// Access elements
printf("Size: %d\n", arr->size(arr));  // 5
int* first = (int*)arr->get(arr, 0);
printf("First: %d\n", *first);          // 10

// Modify element
int new_val = 99;
arr->set(arr, 2, &new_val);

// Pop last
int* last = (int*)arr->pop(arr);
printf("Popped: %d\n", *last);           // 50

// Iterate
for (int i = 0; i < arr->size(arr); i++) {
    int* val = (int*)arr->get(arr, i);
    printf("%d ", *val);
}
// Output: 10 20 99 40

Array_delete(arr);
```

---

🗺️ Map Class

Map Structure

```c
typedef struct Map {
    void* map_data;        // Internal data
    int size;               // Number of entries
    
    // Methods
    struct Map* (*set)(struct Map* this, const char* key, void* value);
    void* (*get)(struct Map* this, const char* key);
    int (*has)(struct Map* this, const char* key);
    void (*remove)(struct Map* this, const char* key);
    Array* (*keys)(struct Map* this);
    Array* (*values)(struct Map* this);
    int (*size)(struct Map* this);
    int (*isEmpty)(struct Map* this);
    void (*clear)(struct Map* this);
} Map;
```

Map Functions

Function Description Example
Map_new() Create map Map* m = Map_new();
Map_delete(m) Free map Map_delete(m);
m->set(m, "key", &value) Set value int x = 42; m->set(m, "age", &x);
m->get(m, "key") Get value int* p = m->get(m, "age");
m->has(m, "key") Check key if (m->has(m, "age")) ...
m->remove(m, "key") Remove key m->remove(m, "age");
m->size(m) Get size int sz = m->size(m);
m->isEmpty(m) Check empty if (m->isEmpty(m)) ...
m->clear(m) Clear map m->clear(m);
m->keys(m) Get all keys Array* keys = m->keys(m);

Map Examples

```c
Map* map = Map_new();

// Store different types
int age = 25;
float pi = 3.14159f;
char* name = "Adham";

map->set(map, "age", &age);
map->set(map, "pi", &pi);
map->set(map, "name", &name);

// Check and retrieve
if (map->has(map, "age")) {
    int* age_ptr = (int*)map->get(map, "age");
    printf("Age: %d\n", *age_ptr);        // 25
}

if (map->has(map, "name")) {
    char** name_ptr = (char**)map->get(map, "name");
    printf("Name: %s\n", *name_ptr);      // Adham
}

printf("Map size: %d\n", map->size(map));  // 3

// Get all keys
Array* keys = map->keys(map);
printf("Keys: ");
for (int i = 0; i < keys->size(keys); i++) {
    char** key = (char**)keys->get(keys, i);
    printf("%s ", *key);
}
printf("\n");

Array_delete(keys);
Map_delete(map);
```

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
CM_ERROR_ALREADY_EXISTS 9 Already exists
... ... ...

Try/Catch Macros

```c
#define cmTry CM_TRY()
#define cmCatch CM_CATCH()
#define cmThrow(e, m) CM_THROW(e, m)
```

Error Functions

Function Description
cmErrorMsg() Get last error message
cmErrorCode() Get last error code
cmErrorString(code) Get error description
cmErrorClear() Clear last error
cmErrorSet(code, msg) Set error manually

Error Handling Examples

```c
// Basic try-catch
cmTry {
    printf("Trying risky operation...\n");
    
    int* ptr = NULL;
    if (!ptr) {
        cmThrow(CM_ERROR_NULL_POINTER, "Null pointer detected!");
    }
    
    printf("This won't print\n");
    
} cmCatch {
    printf("Caught: %s\n", cmErrorMsg());
    printf("Code: %d - %s\n", cmErrorCode(), 
           cmErrorString(cmErrorCode()));
}

// With array bounds checking
cmTry {
    Array* arr = Array_new(sizeof(int), 5);
    
    // This is fine
    int x = 42;
    arr->push(arr, &x);
    
    // This would cause error (but array auto-resizes!)
    for (int i = 0; i < 10; i++) {
        int val = i * 10;
        arr->push(arr, &val);
    }
    
    // Simulate error
    if (arr->size(arr) > 5) {
        cmThrow(CM_ERROR_INVALID_ARGUMENT, "Array too big!");
    }
    
    Array_delete(arr);
    
} cmCatch {
    printf("Error: %s\n", cmErrorMsg());
}
```

---

🛠️ Utility Functions

Random Numbers

```c
// Seed random
cmRandomSeed(time(NULL));

// Random integers
int r1 = cmRandomInt(1, 100);      // 1-100
int r2 = cmRandomInt(-10, 10);      // -10 to 10

// Random strings
char buffer[20];
cmRandomString(buffer, 20);         // Random alphanumeric
```

Time Functions

```c
// Get timestamp
long ts = cmTimestamp();

// Sleep
cmSleepMs(1000);  // Sleep 1 second
```

Math Utilities

```c
// Basic math
int gcd = cmGCD(12, 18);      // 6
int lcm = cmLCM(12, 18);      // 36

// Power of two
if (cmIsPowerOfTwo(16)) {     // true
    int next = cmNextPowerOfTwo(10);  // 16
}
```

Short Macros (Easy Mode)

```c
// Memory
#define cmAlloc(sz) cm_alloc(sz, "object", __FILE__, __LINE__)
#define cmFree(ptr) cm_free(ptr)
#define cmStats() cm_gc_stats()
#define cmGC() cm_gc_collect()

// Strings
#define cmStr(s) String_new(s)
#define cmStrFree(s) String_delete(s)

// Arrays
#define cmArr(type, cap) Array_new(sizeof(type), cap)
#define cmArrFree(a) Array_delete(a)

// Maps
#define cmMap() Map_new()
#define cmMapFree(m) Map_delete(m)

// Errors
#define cmTry CM_TRY()
#define cmCatch CM_CATCH()
#define cmThrow(e, m) CM_THROW(e, m)
#define cmErrMsg() cm_error_get_message()
```

---

💾 Memory Management

Best Practices

```c
// ✅ GOOD: Always free what you allocate
String* s = String_new("Hello");
// ... use string ...
String_delete(s);

// ✅ GOOD: Check for NULL
if (ptr) cmFree(ptr);

// ✅ GOOD: Use cmStats() to verify no leaks
cmStats();  // Should show 0 objects

// ❌ BAD: Forgetting to free
String* s = String_new("Leak");
// No delete → memory leak!

// ❌ BAD: Double free
cmFree(ptr);
cmFree(ptr);  // Error!
```

Memory Leak Detection

At program exit, you'll see:

```
✅ [CM] Clean shutdown - no memory leaks
```

Or if leaks exist:

```
⚠️ [CM] Warning: 3 objects still alive
```

GC Configuration

```c
// Set GC threshold (when to collect)
cm_gc.gc_threshold = 1024 * 1024;  // 1MB

// Enable/disable GC
cm_gc.gc_enabled = 1;  // or 0

// Force collection
cmGC();
```

---

📚 Examples

Example 1: Student Database

```c
#include "CM_full.h"

typedef struct Student {
    String* name;
    int age;
    float gpa;
} Student;

int main() {
    cmTry {
        // Create students
        Array* students = Array_new(sizeof(Student), 10);
        
        Student s1 = {String_new("Alice"), 20, 3.8f};
        Student s2 = {String_new("Bob"), 21, 3.5f};
        Student s3 = {String_new("Charlie"), 19, 4.0f};
        
        students->push(students, &s1);
        students->push(students, &s2);
        students->push(students, &s3);
        
        // Print all students
        printf("Student List:\n");
        for (int i = 0; i < students->size(students); i++) {
            Student* s = (Student*)students->get(students, i);
            printf("  %s (age %d, GPA %.2f)\n", 
                   s->name->data, s->age, s->gpa);
        }
        
        // Cleanup
        for (int i = 0; i < students->size(students); i++) {
            Student* s = (Student*)students->get(students, i);
            String_delete(s->name);
        }
        Array_delete(students);
        
    } cmCatch {
        printf("Error: %s\n", cmErrorMsg());
    }
    
    cmStats();
    return 0;
}
```

Example 2: Word Counter

```c
#include "CM_full.h"

int main() {
    cmTry {
        // Sample text
        String* text = String_new(
            "the quick brown fox jumps over the lazy dog"
        );
        
        // Split into words
        char* text_copy = strdup(text->data);
        char* token = strtok(text_copy, " ");
        
        Map* wordCount = Map_new();
        
        while (token) {
            if (wordCount->has(wordCount, token)) {
                int* count = (int*)wordCount->get(wordCount, token);
                (*count)++;
            } else {
                int initial = 1;
                wordCount->set(wordCount, token, &initial);
            }
            token = strtok(NULL, " ");
        }
        
        // Print results
        Array* keys = wordCount->keys(wordCount);
        for (int i = 0; i < keys->size(keys); i++) {
            char** key = (char**)keys->get(keys, i);
            int* count = (int*)wordCount->get(wordCount, *key);
            printf("%s: %d\n", *key, *count);
        }
        
        // Cleanup
        free(text_copy);
        String_delete(text);
        Array_delete(keys);
        Map_delete(wordCount);
        
    } cmCatch {
        printf("Error: %s\n", cmErrorMsg());
    }
    
    return 0;
}
```

Example 3: Stack Implementation

```c
#include "CM_full.h"

int main() {
    cmTry {
        // Create stack
        Array* stack = Array_new(sizeof(int), 2);
        
        // Push elements
        printf("Pushing: ");
        for (int i = 1; i <= 5; i++) {
            int val = i * 10;
            stack->push(stack, &val);
            printf("%d ", val);
        }
        printf("\n");
        
        // Pop elements
        printf("Popping: ");
        while (!stack->isEmpty(stack)) {
            int* val = (int*)stack->pop(stack);
            printf("%d ", *val);
        }
        printf("\n");
        
        Array_delete(stack);
        
    } cmCatch {
        printf("Error: %s\n", cmErrorMsg());
    }
    
    return 0;
}
```

---

✅ Best Practices

Do's and Don'ts

Do ✅ Don't ❌
Always free what you allocate Forget to delete objects
Check for NULL before using Use freed pointers
Use cmStats() to verify Ignore memory leaks
Wrap risky code in cmTry Leave errors unhandled
Set random seed once Use uninitialized memory
Use type-specific macros Cast without checking

Performance Tips

```c
// 1. Reuse objects when possible
String* s = String_new("");
for (int i = 0; i < 1000; i++) {
    char temp[20];
    sprintf(temp, "%d", i);
    s->set(s, temp);  // Reuse same string
}

// 2. Pre-allocate arrays with expected size
Array* arr = Array_new(sizeof(int), 1000);  // Good!

// 3. Use stack for small objects
Person p;  // Stack allocation (fast)
p.init(&p, "Adham", 25);

// 4. Use heap only for large/persistent objects
Person* p = (Person*)cmAlloc(sizeof(Person));  // Heap
```

Memory Leak Prevention

```c
// Always pair allocation with deletion
String* s = String_new("data");
// ... code ...
String_delete(s);  // Don't forget!

// Use cmStats() to verify
cmStats();  // Should show 0 objects

// RAII-style (manual but consistent)
void process() {
    String* s = String_new("temp");
    Array* a = Array_new(sizeof(int), 10);
    
    // ... use them ...
    
    // Cleanup in reverse order
    Array_delete(a);
    String_delete(s);
}
```

---

🔧 Troubleshooting

Common Errors

Error Cause Solution
undefined reference to 'cm_...' Missing CM_IMPLEMENTATION Add #define CM_IMPLEMENTATION
multiple definition of 'cm_...' CM_IMPLEMENTATION in multiple files Put in ONLY ONE .c file
Total objects > 0 at exit Memory leak Check you freed all objects
segmentation fault Using freed memory Don't use after delete
cmTry not found Missing error macros Update to latest version

Debug Mode

```c
// Enable verbose logging
#define CM_LOG_LEVEL 4  // 0-4 (4 = most verbose)

// Track specific variables
int x = 42;
CM_TRACK(x);  // Prints debug info

// Assertions
CM_ASSERT(x > 0);
CM_ASSERT_MSG(ptr != NULL, "Pointer cannot be NULL");
```

Memory Leak Detection

If you see:

```
⚠️ [CM] Warning: 3 objects still alive
```

Follow these steps:

1. Run cmStats() to see active objects
2. Check each allocation site
3. Add missing _delete calls
4. Verify with cmStats() again

---

📚 API Reference

Core Functions

Function Description
cm_alloc(size, type, file, line) Allocate tracked memory
cm_free(ptr) Free memory
cm_retain(ptr) Increment reference count
cm_gc_init() Initialize GC (auto-called)
cm_gc_collect() Force garbage collection
cm_gc_stats() Show GC statistics

String Class

Method Description
String_new(initial) Constructor
String_delete(self) Destructor
self->concat(self, other) Concatenate
self->upper(self) Convert to uppercase
self->lower(self) Convert to lowercase
self->trim(self) Remove whitespace
self->find(self, substr) Find substring
self->length(self) Get length
self->charAt(self, index) Get character
self->print(self) Print string

Array Class

Method Description
Array_new(elem_size, cap) Constructor
Array_delete(self) Destructor
self->push(self, value) Add element
self->pop(self) Remove last
self->get(self, index) Get element
self->set(self, index, value) Set element
self->size(self) Get size
self->isEmpty(self) Check if empty
self->clear(self) Clear all

Map Class

Method Description
Map_new() Constructor
Map_delete(self) Destructor
self->set(self, key, value) Set key-value
self->get(self, key) Get value
self->has(self, key) Check key
self->remove(self, key) Remove key
self->size(self) Get size
self->isEmpty(self) Check if empty
self->keys(self) Get all keys
self->values(self) Get all values
self->clear(self) Clear all

Error Handling

Function Description
cmErrorMsg() Get last error message
cmErrorCode() Get last error code
cmErrorString(code) Get error description
cmErrorClear() Clear last error
cmTry Start try block
cmCatch Catch errors
cmThrow(e, m) Throw error

Utility Functions

Function Description
cmRandomSeed(seed) Set random seed
cmRandomInt(min, max) Random integer
cmRandomString(buf, len) Random string
cmTimestamp() Get timestamp
cmSleepMs(ms) Sleep milliseconds
cmGCD(a, b) Greatest common divisor
cmLCM(a, b) Least common multiple
cmIsPowerOfTwo(x) Check if power of two
cmNextPowerOfTwo(x) Next power of two

---

📅 Version History

Version 3.1.1 (Current)

· ✅ Fixed memory leak detection
· ✅ Improved error messages
· ✅ Added more examples

Version 3.1.0

· ✅ Added OOP-style classes
· ✅ Added method chaining
· ✅ Improved documentation

Version 3.0.0

· ✅ Complete rewrite
· ✅ Garbage Collector
· ✅ String, Array, Map classes
· ✅ Error handling
· ✅ Single header file

Version 2.0.0

· ✅ Basic memory tracking
· ✅ Simple string functions

Version 1.0.0

· ✅ Initial release
· ✅ Basic utilities

---

📝 License

```text
MIT License

Copyright (c) 2026 Adham Hossam

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

---

🙏 Acknowledgments

· Thanks to all C programmers who inspired this library
· Built with ❤️ on Android using CXXDroid
· Special thanks to the C community

---

📬 Contact & Support

· Author: Adham Hossam
· Email: adham.hossam5020@gmail.com
· GitHub: github.com/ALightbolt4G
· portfilo: https://ALIGHTBOLT4G.GITHUB.IO/Adham-website/

---

<div align="center">

⭐ If you find this library useful, please star it on GitHub!

Download CM Library v3.1.1

---

Happy Coding! 🚀

Documentation generated on February 22, 2026

</div>
```
