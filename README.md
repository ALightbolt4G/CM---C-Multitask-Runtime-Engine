📚 CM Library v4.2.3 Documentation

<div align="center">

🔷 C MULTITASK INTELLIGENT LIBRARY 🔷

The Ultimate C Programming Intelligence Layer

Version 4.2.3 | By Adham Hossam | 3000+ LOC | 100% Memory Safe

```
⚡ FAST  |  🛡️ SAFE  |  🧠 SMART  |  🔒 THREAD-SAFE
```

https://img.shields.io/badge/build-passing-brightgreen
https://img.shields.io/badge/version-4.2.2-blue
https://img.shields.io/badge/license-MIT-orange
https://img.shields.io/badge/thread_safe-yes-success

</div>

---

📑 TABLE OF CONTENTS

Section Description
1. Introduction What is CM Library?
2. Quick Start Your first CM program in 10 seconds
3. Installation How to set up
4. Memory Management System The unified memory architecture
5. Arena Allocator ⚡ NEW - Ultra-fast allocations
6. Garbage Collector Automatic memory management
7. String Class Smart strings like Python
8. Array Class Dynamic arrays like C++ vector
9. Map Class Hash maps like Python dict
10. Error Handling Try/Catch for C
11. OOP Macros Object-oriented programming
12. Utility Functions Random, Input, etc.
13. Thread Safety Multi-threading support
14. Complete Examples Real-world examples
15. API Reference All functions at a glance
16. Best Practices How to use effectively
17. Troubleshooting Common issues solved
18. What's New in v4.2.2 Latest improvements

---

📖 INTRODUCTION

<div style="background-color: #f0f8ff; padding: 15px; border-radius: 5px;">

CM Library is a revolutionary single-header library that transforms C into a modern programming language with:

</div>

```c
✅ Automatic Garbage Collection   ✅ Arena Allocator (Ultra-fast)
✅ Thread Safety                  ✅ Object-Oriented Programming
✅ Smart Strings (like Python)    ✅ Dynamic Arrays (like C++ vector)
✅ Hash Maps (like Python dict)   ✅ Try/Catch Error Handling
✅ Zero External Dependencies     ✅ Single Header File
✅ Memory Leak Detection          ✅ Arena/GC Hybrid System
✅ C++ Compatible OOP             ✅ Zero Warnings
```

Feature Comparison

Feature CM Library Standard C C++
GC/Arena Hybrid ✅ YES ❌ No ❌ No
Single Header ✅ YES ❌ No ❌ No
OOP Support ✅ YES ❌ No ✅ Yes
Try/Catch ✅ YES ❌ No ✅ Yes
Thread Safe ✅ YES ⚠️ Manual ⚠️ Manual
Memory Safety ✅ 100% ⚠️ Manual ⚠️ Manual
Zero Warnings ✅ YES ❌ No ⚠️ Partial

---

⚡ QUICK START

Your First CM Program (10 seconds)

```c
#include "CM.h"  // Just one header!

int main() {
    CM_ABOUT();  // Show library info
    
    // 🔷 Create a string
    String* name = String_new("Adham Hossam");
    name->print(name);  // Output: Adham Hossam
    printf("\n");
    
    // Cleanup
    String_delete(name);
    
    // 🔍 Check for leaks
    CM_REPORT();
    
    return 0;
}
```

Compile and run:

```bash
# Compile with pthread support
clang main.c CM.c -o program -lpthread -lm
# or
gcc main.c CM.c -o program -lpthread -lm

./program
```

Expected Output:

```
_________________________________________________________
                                                     
        C MULTITASK INTELLIGENT LIBRARY             
                 by Adham Hossam                     
                                                     
--------------------------------------------------------
  Version : 4.2.2
  Author  : Adham Hossam
_________________________________________________________

Adham Hossam

══════════════════════════════════════════════════════════════
              GARBAGE COLLECTOR STATISTICS
──────────────────────────────────────────────────────────────
  🔷 Total objects    │                    0
  💾 Total memory     │                    0 bytes
  📊 Peak memory      │                   64 bytes
  📈 Allocations      │                    2
  📉 Frees            │                    2
  🔄 Collections      │                    0
══════════════════════════════════════════════════════════════
```

---

📦 INSTALLATION

Option 1: Two Files (Recommended)

```
your_project/
├── CM.h          ← Header file (public API)
├── CM.c          ← Implementation (compile once)
└── main.c
```

Option 2: Organized Folder

```
your_project/
├── CM/
│   ├── CM.h
│   └── CM.c
└── main.c
```

In your code:

```c
#include "CM.h"  // Just include the header

int main() {
    // CM functions are ready to use!
    return 0;
}
```

Compile:

```bash
# Just compile CM.c with your program
gcc main.c CM.c -o program -lpthread -lm

# Or with CMake
add_executable(program main.c CM.c)
target_link_libraries(program pthread m)
```

---

🧠 MEMORY MANAGEMENT SYSTEM

The Unified Memory Architecture

```
┌─────────────────────────────────────────────────────┐
│                    YOUR APPLICATION                   │
└─────────────────────────────────────────────────────┘
                           │
            ┌──────────────┴──────────────┐
            ▼                              ▼
┌───────────────────────┐      ┌───────────────────────┐
│   🚀 ARENA TIER       │      │   ♻️ GC TIER          │
│   (ULTRA FAST)        │      │   (FLEXIBLE)          │
├───────────────────────┤      ├───────────────────────┤
│ • No tracking         │      │ • Full tracking       │
│ • Instant free        │      │ • Auto cleanup        │
│ • Cache local         │      │ • Thread safe         │
│ • Bump allocator      │      │ • Mark & sweep        │
└───────────────────────┘      └───────────────────────┘
         │                              │
         └──────────────┬──────────────┘
                        ▼
┌─────────────────────────────────────────────────────┐
│              SINGLE GLOBAL SYSTEM                     │
│                static CMMemorySystem cm_mem           │
└─────────────────────────────────────────────────────┘
```

The Core Structure

```c
typedef struct {
    // 🔷 GC Fields
    CMObject* head;           // Linked list of tracked objects
    CMObject* tail;           
    size_t total_memory;      // Current memory usage
    pthread_mutex_t gc_lock;  // Thread safety for GC
    
    // 🚀 Arena Fields
    CMArena* current_arena;   // Active arena
    pthread_mutex_t arena_lock; // Thread safety for arena
    
    // 📊 Statistics
    size_t peak_memory;       // Peak memory usage
    size_t allocations;       // Total allocations
    size_t frees;             // Total frees
    size_t collections;       // GC collections count
    size_t total_objects;     // Current object count
} CMMemorySystem;
```

---

🚀 ARENA ALLOCATOR

NEW in v4.2.2 - Lightning-Fast Memory Allocations

<div style="background-color: #e6f3ff; padding: 15px; border-radius: 5px;">

What is an Arena? An arena is a memory region where allocations are just pointer bumps - no tracking, no locking, no overhead!

</div>

Arena Structure

```c
typedef struct CMArena {
    void* block;           // 📦 Memory block
    size_t block_size;      // 📏 Total size
    size_t offset;          // 📍 Current position
    struct CMArena* next;   // 🔗 Next arena
    const char* name;       // 🏷️ Arena name
    size_t peak_usage;      // 📊 Peak usage
} CMArena;
```

Arena Functions

Function Description Complexity
cm_arena_create(size) Create new arena O(1)
cm_arena_destroy(arena) Destroy arena (frees all) O(1)
cm_arena_push(arena) Set as current arena O(1)
cm_arena_pop() Remove current arena O(1)
CM_WITH_ARENA(size) Auto-cleanup arena block O(1)

---

🎯 ARENA EXAMPLES

Example 1: Basic Arena Usage

```c
#include "CM.h"

int main() {
    CM_ABOUT();
    
    // 🔷 Create a 1MB arena
    CMArena* arena = cm_arena_create(1024 * 1024);
    cm_arena_push(arena);  // Set as current
    
    // 🚀 Fast allocations (no GC tracking!)
    char* buffer = (char*)cm_alloc(100, "temp", __FILE__, __LINE__);
    int* numbers = (int*)cm_alloc(10 * sizeof(int), "temp", __FILE__, __LINE__);
    
    // Use them...
    strcpy(buffer, "⚡ Fast allocation!");
    printf("Buffer: %s\n", buffer);
    
    numbers[0] = 42;
    printf("numbers[0] = %d\n", numbers[0]);
    
    // 💥 Destroy arena → frees ALL memory instantly!
    cm_arena_pop();
    cm_arena_destroy(arena);
    
    CM_REPORT();  // Should show 0 objects
    return 0;
}
```

Output:

```
Buffer: ⚡ Fast allocation!
numbers[0] = 42

══════════════════════════════════════════════════════════════
              GARBAGE COLLECTOR STATISTICS
──────────────────────────────────────────────────────────────
  🔷 Total objects    │                    0
  ...
```

Example 2: Arena with Macro (Cleanest!)

```c
int main() {
    // 🎯 Automatic arena cleanup with break/return support!
    CM_WITH_ARENA(4096) {
        // Inside this block, arena is active
        char* temp = (char*)cm_alloc(500, "temp", __FILE__, __LINE__);
        strcpy(temp, "📦 This is temporary");
        printf("%s\n", temp);
        
        // More temporary allocations...
        int* data = (int*)cm_alloc(100 * sizeof(int), "temp", __FILE__, __LINE__);
        for (int i = 0; i < 100; i++) data[i] = i;
        printf("data[50] = %d\n", data[50]);
        
        // Even with break or return, arena is cleaned!
        if (data[50] > 0) {
            break;  // 🧹 Arena auto-destroyed here!
        }
        
    } // ← 🧹 Arena automatically destroyed here if no break/return
    
    CM_REPORT();  // Clean!
    return 0;
}
```

Performance Comparison

Allocation Type Speed Tracking Free Method Use Case
Arena ⚡⚡⚡⚡⚡ None Instant (bulk) Temporary data
GC ⚡⚡⚡ Full Automatic Long-lived objects
malloc ⚡⚡ None Manual Legacy code

When to Use Arena vs GC

Scenario Use Arena Use GC
🎮 Game loop (per-frame) ✅ PERFECT ❌ Too slow
📝 Parse/compile ✅ PERFECT ❌ Overkill
🔄 Temporary buffers ✅ PERFECT ❌ Wasteful
🏗️ Long-lived objects ❌ Wrong ✅ PERFECT
🌐 Server/daemon ❌ Wrong ✅ PERFECT
📚 Data structures ❌ Wrong ✅ PERFECT

---

♻️ GARBAGE COLLECTOR

Automatic Memory Management

<div style="background-color: #f0fff0; padding: 15px; border-radius: 5px;">

The GC uses mark-and-sweep with reference counting - fully thread-safe!

</div>

GC Algorithm

```
┌─────────────────┐
│   ALLOCATE      │  cm_alloc() → object with ref_count = 1
└────────┬────────┘
         ▼
┌─────────────────┐
│    RETAIN       │  cm_retain() → ref_count++
└────────┬────────┘
         ▼
┌─────────────────┐
│    RELEASE      │  cm_free() → ref_count--
└────────┬────────┘
         ▼
┌─────────────────┐
│   COLLECT       │  cm_gc_collect() → sweep unmarked (ref_count=0)
└─────────────────┘
```

GC Functions

Function Description
cm_alloc(size, type, file, line) Allocate tracked memory
cm_free(ptr) Decrement ref count (free if 0)
cm_retain(ptr) Increment reference count
cm_gc_collect() Force garbage collection
cm_gc_stats() Show GC statistics

GC Statistics Output

```
══════════════════════════════════════════════════════════════
              GARBAGE COLLECTOR STATISTICS
──────────────────────────────────────────────────────────────
  🔷 Total objects    │                   12
  💾 Total memory     │                 1024 bytes
  📊 Peak memory      │                 2048 bytes
  📈 Allocations      │                   24
  📉 Frees            │                   12
  🔄 Collections      │                    3
──────────────────────────────────────────────────────────────
  ⏱️  Avg collection  │              0.456 ms
  🗑️  Last freed      │                  512 bytes
──────────────────────────────────────────────────────────────
  🚀 ARENA STATISTICS
  📦 Arena name       │         dynamic_arena
  📏 Arena size       │              1048576 bytes
  📍 Arena used       │                24576 bytes
  📊 Arena peak       │                32768 bytes
══════════════════════════════════════════════════════════════
```

---

📝 STRING CLASS

Smart Strings Like Python

<div style="background-color: #fff5e6; padding: 15px; border-radius: 5px;">

String class provides dynamic strings with automatic memory management and useful methods.

</div>

String Structure

```c
typedef struct String {
    char* data;          // 📝 String data
    int length;           // 📏 Current length
    int capacity;         // 📦 Allocated capacity
    
    // 🔷 Methods
    struct String* (*concat)(struct String* self, const char* other);
    struct String* (*upper)(struct String* self);
    struct String* (*lower)(struct String* self);
    void (*print)(struct String* self);
    int (*length_func)(struct String* self);
    char (*charAt)(struct String* self, int index);
} String;
```

String Methods

Method Description Example
String_new(initial) Create string String* s = String_new("Hi");
String_delete(s) Free string String_delete(s);
s->concat(s, " world") Concatenate s->concat(s, "!");
s->upper(s) To uppercase s->upper(s);
s->lower(s) To lowercase s->lower(s);
s->print(s) Print string s->print(s);
s->length_func(s) Get length int len = s->length_func(s);
s->charAt(s, 0) Get character char c = s->charAt(s, 0);

String Examples

```c
int main() {
    // 🔷 Create a string
    String* s = String_new("  Hello World  ");
    
    printf("📝 Original: ");
    s->print(s);
    printf("\n📏 Length: %d\n", s->length_func(s));
    
    // ⬆️ Uppercase
    s->upper(s);
    printf("⬆️ Uppercase: ");
    s->print(s);
    printf("\n");
    
    // ⬇️ Lowercase
    s->lower(s);
    printf("⬇️ Lowercase: ");
    s->print(s);
    printf("\n");
    
    // ➕ Concatenate
    s->concat(s, "!!!");
    printf("➕ Concatenated: ");
    s->print(s);
    printf("\n");
    
    String_delete(s);
    return 0;
}
```

---

📊 ARRAY CLASS

Dynamic Arrays Like C++ Vector

<div style="background-color: #e6f7ff; padding: 15px; border-radius: 5px;">

Array class provides dynamic arrays that grow automatically.

</div>

Array Structure

```c
typedef struct Array {
    void* data;           // 📦 Array elements
    int element_size;      // 📏 Size of each element
    int length;            // 🔢 Current number of elements
    int capacity;          // 📦 Allocated capacity
    
    // 🔷 Methods
    struct Array* (*push)(struct Array* self, void* value);
    void* (*pop)(struct Array* self);
    void* (*get)(struct Array* self, int index);
    int (*size)(struct Array* self);
} Array;
```

Array Methods

Method Description Example
Array_new(elem_size, cap) Create array Array* a = Array_new(sizeof(int), 10);
Array_delete(a) Free array Array_delete(a);
a->push(a, &value) Add element int x = 42; a->push(a, &x);
a->pop(a) Remove last int* p = (int*)a->pop(a);
a->get(a, i) Get element int* p = (int*)a->get(a, 0);
a->size(a) Get size int len = a->size(a);

Array Examples

```c
int main() {
    // 🔷 Create array for integers
    Array* numbers = Array_new(sizeof(int), 5);
    
    // 📥 Push elements
    printf("📥 Pushing elements:\n");
    for (int i = 0; i < 10; i++) {
        int val = i * 10;
        numbers->push(numbers, &val);
        printf("  Pushed: %d, size: %d\n", val, numbers->size(numbers));
    }
    
    // 📤 Access elements
    printf("\n📤 Array contents:\n");
    for (int i = 0; i < numbers->size(numbers); i++) {
        int* val = (int*)numbers->get(numbers, i);
        printf("  numbers[%d] = %d\n", i, *val);
    }
    
    Array_delete(numbers);
    return 0;
}
```

---

🗺️ MAP CLASS

Hash Maps Like Python Dict

<div style="background-color: #f0e6ff; padding: 15px; border-radius: 5px;">

Map class provides key-value storage with automatic resizing.

</div>

Map Structure

```c
typedef struct Map {
    void* map_data;       // 🗺️ Internal data
    int size;              // 🔢 Number of entries
    
    // 🔷 Methods
    struct Map* (*set)(struct Map* self, const char* key, void* value);
    void* (*get)(struct Map* self, const char* key);
    int (*has)(struct Map* self, const char* key);
    int (*size_func)(struct Map* self);
} Map;
```

Map Methods

Method Description Example
Map_new() Create map Map* m = Map_new();
Map_delete(m) Free map Map_delete(m);
m->set(m, "key", &value) Set value int x = 42; m->set(m, "age", &x);
m->get(m, "key") Get value int* p = (int*)m->get(m, "age");
m->has(m, "key") Check key if (m->has(m, "age")) {...}
m->size_func(m) Get size int sz = m->size_func(m);

Map Examples

```c
int main() {
    // 🔷 Create map
    Map* dict = Map_new();
    
    // 📦 Store different types
    int age = 25;
    float pi = 3.14159f;
    char* name = "Adham";
    
    dict->set(dict, "age", &age);
    dict->set(dict, "pi", &pi);
    dict->set(dict, "name", &name);
    
    printf("🗺️ Map size: %d\n", dict->size_func(dict));
    
    // 🔍 Retrieve values
    if (dict->has(dict, "age")) {
        int* age_ptr = (int*)dict->get(dict, "age");
        printf("  age = %d\n", *age_ptr);
    }
    
    if (dict->has(dict, "name")) {
        char** name_ptr = (char**)dict->get(dict, "name");
        printf("  name = %s\n", *name_ptr);
    }
    
    Map_delete(dict);
    return 0;
}
```

---

🚨 ERROR HANDLING

Try/Catch for C

<div style="background-color: #ffe6e6; padding: 15px; border-radius: 5px;">

CM Library provides exception handling similar to C++/Java, now thread-safe with thread-local storage!

</div>

Error Codes

Code Value Description
CM_SUCCESS 0 Success
CM_ERROR_MEMORY 1 Memory allocation failed
CM_ERROR_NULL_POINTER 2 Null pointer dereference
CM_ERROR_OUT_OF_BOUNDS 3 Index out of bounds
CM_ERROR_DIVISION_BY_ZERO 4 Division by zero
CM_ERROR_OVERFLOW 5 Integer overflow
CM_ERROR_UNDERFLOW 6 Integer underflow
CM_ERROR_INVALID_ARGUMENT 7 Invalid argument
CM_ERROR_NOT_FOUND 8 Not found
CM_ERROR_ALREADY_EXISTS 9 Already exists
CM_ERROR_PERMISSION_DENIED 10 Permission denied
CM_ERROR_IO 11 I/O error
CM_ERROR_NETWORK 12 Network error
CM_ERROR_TIMEOUT 13 Operation timed out
CM_ERROR_THREAD 14 Thread error
CM_ERROR_SYNC 15 Synchronization error
CM_ERROR_PARSE 16 Parse error
CM_ERROR_TYPE 17 Type error
CM_ERROR_UNIMPLEMENTED 18 Unimplemented
CM_ERROR_UNKNOWN 19 Unknown error

Error Handling Macros

Macro Description
cmTry Start try block
cmCatch Catch errors
cmThrow(e, m) Throw error
cmErrorMsg() Get last error message
cmErrorCode() Get last error code

Error Handling Examples

```c
int main() {
    cmTry {
        printf("🔷 Trying risky operation...\n");
        
        int* ptr = NULL;
        if (!ptr) {
            cmThrow(CM_ERROR_NULL_POINTER, "Null pointer detected!");
        }
        
        printf("❌ This won't print\n");
        
    } cmCatch {
        printf("✅ Caught: %s\n", cmErrorMsg());
        printf("📊 Error code: %d - %s\n", cmErrorCode(), 
               cm_error_string(cmErrorCode()));
    }
    
    return 0;
}
```

Output:

```
🔷 Trying risky operation...
✅ Caught: Null pointer detected!
📊 Error code: 2 - Null pointer dereference
```

---

🎯 OOP MACROS

Object-Oriented Programming in C (C++ Compatible)

<div style="background-color: #e6ffe6; padding: 15px; border-radius: 5px;">

CM Library provides macros for class-based OOP using cmlass (not class) to avoid C++ conflicts.

</div>

OOP Macros

Macro Description Example
cmlass(name) Define a class cmlass(Person) { ... }
end_class End class definition } end_class;
property(type, name) Define property property(char*, name);
method0(ret, name) Method with no params method0(void, speak);
method(ret, name, ...) Method with params method(void, speak, const char* msg);
send(obj, method, ...) Call method send(p, speak, "Hello");

Custom Class Examples

```c
// 🔷 Define Person class (C++ compatible)
cmlass(Person) {
    property(char*, name);
    property(int, age);
    
    method0(void, introduce);
    method(void, speak, const char* msg);
} end_class;

// 📝 Implement methods
void Person_introduce(void* ptr) {
    Person* self = (Person*)ptr;
    printf("Hi, I'm %s and I'm %d years old\n", self->name, self->age);
}

void Person_speak(void* ptr, const char* msg) {
    Person* self = (Person*)ptr;
    printf("%s says: %s\n", self->name, msg);
}

int main() {
    // 🔷 Create and use Person
    Person p = {.name = "Adham", .age = 25};
    p.introduce = Person_introduce;
    p.speak = Person_speak;
    
    send(&p, introduce);
    send(&p, speak, "Hello CM Library!");
    
    return 0;
}
```

Output:

```
Hi, I'm Adham and I'm 25 years old
Adham says: Hello CM Library!
```

---

🛠️ UTILITY FUNCTIONS

Random Functions

Function Description Example
cm_random_seed(seed) Set random seed cm_random_seed(time(NULL));
cm_random_string(buf, len) Random string char s[20]; cm_random_string(s, 20);
cmRandInt(min, max) Random integer int r = cmRandInt(1, 100);
cmRandStr(buf, len) Random string macro cmRandStr(buffer, 20);

Other Utilities

Function Description
CM_ABOUT() Show library info
CM_REPORT() Show GC statistics
cm_input(prompt) Read user input (returns String*)

Random Examples

```c
int main() {
    // 🎲 Seed random
    cm_random_seed(time(NULL));
    
    // 🔢 Random integers
    printf("🎲 Random numbers (1-100):\n");
    for (int i = 0; i < 5; i++) {
        printf("  %d\n", cmRandInt(1, 100));
    }
    
    // 📝 Random strings
    printf("\n📝 Random strings:\n");
    for (int i = 0; i < 3; i++) {
        char buffer[20];
        cm_random_string(buffer, sizeof(buffer));
        printf("  %s\n", buffer);
    }
    
    return 0;
}
```

---

🔒 THREAD SAFETY

Multi-threading Support

<div style="background-color: #fff0f0; padding: 15px; border-radius: 5px;">

CM Library v4.2.2 is fully thread-safe with:

· Mutex protection for all GC operations
· Mutex protection for arena switching
· Thread-local storage for exception handling
· No race conditions in multi-threaded code

</div>

Mutex Protection

```c
// 🔒 GC operations are protected
pthread_mutex_lock(&cm_mem.gc_lock);
// ... modify GC list ...
pthread_mutex_unlock(&cm_mem.gc_lock);

// 🔒 Arena operations are protected
pthread_mutex_lock(&cm_mem.arena_lock);
// ... switch arena ...
pthread_mutex_unlock(&cm_mem.arena_lock);
```

Thread-Safe Exception Handling

```c
// 🔷 Each thread has its own exception buffer
extern __thread jmp_buf* cm_exception_buffer;  // Thread-local!

#define CM_TRY() \
    jmp_buf __cm_buf; \
    jmp_buf* __old = cm_exception_buffer; \
    cm_exception_buffer = &__cm_buf; \
    if (setjmp(__cm_buf) == 0)
```

Multi-Threading Example

```c
#include <pthread.h>
#include "CM.h"

void* thread_func(void* arg) {
    int id = *(int*)arg;
    
    // 🔷 Each thread can use the library safely
    cmTry {
        String* s = String_new("Thread ");
        char num[10];
        sprintf(num, "%d", id);
        s->concat(s, num);
        
        printf("🧵 Thread %d: ", id);
        s->print(s);
        printf("\n");
        
        String_delete(s);
        
    } cmCatch {
        printf("Thread %d error: %s\n", id, cmErrorMsg());
    }
    
    return NULL;
}

int main() {
    pthread_t threads[5];
    int ids[5];
    
    printf("🚀 Starting 5 threads...\n\n");
    
    for (int i = 0; i < 5; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("\n✅ All threads completed\n");
    CM_REPORT();
    return 0;
}
```

---

📚 COMPLETE EXAMPLES

Example 1: Student Management System

```c
#include "CM.h"

// 🔷 Define Student class
cmlass(Student) {
    property(String*, name);
    property(int, age);
    property(float, gpa);
    
    method0(void, print);
    method(void, updateGPA, float);
} end_class;

// 📝 Implement methods
void Student_print(void* ptr) {
    Student* self = (Student*)ptr;
    printf("👤 Student: ");
    self->name->print(self->name);
    printf(", age: %d, GPA: %.2f\n", self->age, self->gpa);
}

void Student_updateGPA(void* ptr, float new_gpa) {
    Student* self = (Student*)ptr;
    self->gpa = new_gpa;
    printf("📊 GPA updated to: %.2f\n", self->gpa);
}

int main() {
    CM_ABOUT();
    
    cmTry {
        // 🔷 Create students
        Student s1 = {.name = String_new("Adham"), .age = 25, .gpa = 3.8f};
        Student s2 = {.name = String_new("Ahmed"), .age = 22, .gpa = 3.5f};
        
        s1.print = Student_print;
        s1.updateGPA = Student_updateGPA;
        s2.print = Student_print;
        
        printf("\n📋 --- Student List ---\n");
        send(&s1, print);
        send(&s2, print);
        
        // 📈 Update GPA
        printf("\n📈 --- Updating GPA ---\n");
        send(&s1, updateGPA, 3.9f);
        
        // 🧹 Cleanup
        String_delete(s1.name);
        String_delete(s2.name);
        
    } cmCatch {
        printf("❌ Error: %s\n", cmErrorMsg());
    }
    
    CM_REPORT();
    return 0;
}
```

Example 2: Game Entity System

```c
cmlass(Entity) {
    property(float, x);
    property(float, y);
    property(String*, name);
    property(int, health);
    
    method0(void, print);
    method(void, damage, int);
} end_class;

void Entity_print(void* ptr) {
    Entity* self = (Entity*)ptr;
    printf("👾 %s at (%.1f, %.1f) health: %d\n",
           self->name->data, self->x, self->y, self->health);
}

void Entity_damage(void* ptr, int amount) {
    Entity* self = (Entity*)ptr;
    self->health -= amount;
    if (self->health < 0) self->health = 0;
    printf("💥 %s took %d damage, health now %d\n", 
           self->name->data, amount, self->health);
}

int main() {
    // 🚀 Use arena for temporary entities
    CM_WITH_ARENA(4096) {
        Entity player = {
            .x = 0, .y = 0, 
            .name = String_new("Player"), 
            .health = 100
        };
        player.print = Entity_print;
        player.damage = Entity_damage;
        
        send(&player, print);
        send(&player, damage, 10);
        send(&player, print);
    } // 🧹 All freed here!
    
    return 0;
}
```

---

📚 API REFERENCE

Core Functions

Function Description
cm_alloc(size, type, file, line) Allocate tracked memory
cm_free(ptr) Free memory
cm_retain(ptr) Increment reference count
cm_gc_init() Initialize GC (auto-called)
cm_gc_collect() Force garbage collection
cm_gc_stats() Show GC statistics

Arena Functions

Function Description
cm_arena_create(size) Create new arena
cm_arena_destroy(arena) Destroy arena
cm_arena_push(arena) Set active arena
cm_arena_pop() Remove active arena
CM_WITH_ARENA(size) Auto-cleanup arena block

String Class

Method Description
String_new(initial) Constructor
String_delete(self) Destructor
self->concat(self, other) Concatenate
self->upper(self) Convert to uppercase
self->lower(self) Convert to lowercase
self->print(self) Print string
self->length_func(self) Get length
self->charAt(self, index) Get character

Array Class

Method Description
Array_new(elem_size, cap) Constructor
Array_delete(self) Destructor
self->push(self, value) Add element
self->pop(self) Remove last
self->get(self, index) Get element
self->size(self) Get size

Map Class

Method Description
Map_new() Constructor
Map_delete(self) Destructor
self->set(self, key, value) Set key-value
self->get(self, key) Get value
self->has(self, key) Check key
self->size_func(self) Get size

---

✅ BEST PRACTICES

Memory Management

```c
// ✅ GOOD: Always free what you allocate
String* s = String_new("Hello");
// ... use ...
String_delete(s);

// ✅ GOOD: Use arena for temporary data
CM_WITH_ARENA(1024) {
    // Allocations here are automatically freed
    // Even with break/return!
}

// ✅ GOOD: Check for NULL
if (ptr) cmFree(ptr);

// ✅ GOOD: Use CM_REPORT() to verify no leaks
CM_REPORT();  // Should show 0 objects
```

Performance Tips

```c
// 1️⃣ Use Arena for temporary/per-frame allocations
CM_WITH_ARENA(1024 * 1024) {
    // Game loop allocations here
}

// 2️⃣ Pre-allocate arrays with expected size
Array* arr = Array_new(sizeof(int), 1000);  // Better than growing

// 3️⃣ Use stack for small objects
Person p;  // Stack allocation (fastest)

// 4️⃣ Use heap only for persistent objects
Person* p = (Person*)cm_alloc(sizeof(Person), "person", __FILE__, __LINE__);
```

Error Handling

```c
// ✅ GOOD: Wrap risky code
cmTry {
    // Code that might fail
} cmCatch {
    printf("❌ Error: %s\n", cmErrorMsg());
}

// ✅ GOOD: Check return values
if (!ptr) {
    cmThrow(CM_ERROR_MEMORY, "Allocation failed");
}
```

---

🔧 TROUBLESHOOTING

Common Errors

Error Cause Solution
undefined reference Missing CM.c Compile CM.c with your program
multiple definition CM.c in multiple files Include CM.c in ONLY ONE file
Total objects > 0 Memory leak Check you freed all objects
segmentation fault Using freed memory Don't use after delete
cmTry not found Missing macros Update to latest version

Memory Leak Detection

```
⚠️ [CM] Warning: 50 objects still alive
```

This means you forgot to free 50 objects! Steps to fix:

1. 🔍 Run CM_REPORT() to see active objects
2. 📋 Check each allocation site
3. 🧹 Add missing _delete calls
4. ✅ Verify with CM_REPORT() again

Arena Full Warning

```
[ARENA] Warning: Arena 'dynamic_arena' full, falling back to GC
```

Solution: Increase arena size or use multiple arenas

```c
CMArena* arena = cm_arena_create(1024 * 1024);  // 1MB
```

---

🎯 WHAT'S NEW IN v4.2.2

✨ New Features

Feature Description
Thread-local exceptions __thread jmp_buf* for thread safety
Fixed CM_WITH_ARENA Now works with break/return
cmlass macro Replaced class for C++ compatibility
method0 macro For methods without parameters
Fixed send macro Works with all parameter combinations
Zero warnings All warnings fixed
Zero memory leaks 100% memory safe

🔧 Bug Fixes

Issue Fixed in v4.2.2
Thread-unsafe exceptions ✅ Thread-local storage
Arena leak on break/return ✅ Cleanup attribute
this vs self confusion ✅ Unified to self
GNU extensions ✅ Standard C
C++ class conflict ✅ cmlass instead

---

## 🎯 WHAT'S NEW IN v4.2.3 new update

This release focuses on hardening the library, fixing critical bugs, and ensuring full compatibility with C++ environments. The key improvements include complete thread-safety for exceptions, a leak-proof arena macro, and a more robust, portable object-oriented programming system.

### ✨ Key Improvements & New Features

| Feature | Description |
|---------|-------------|
| **🔒 Thread-Local Exceptions** | The exception handling system (`CM_TRY`/`CM_THROW`) is now fully thread-safe. It uses a `__thread`-local buffer, eliminating the previous race condition where one thread could overwrite another's exception context. |
| **🧹 Fixed `CM_WITH_ARENA`** | The arena macro has been redesigned to guarantee cleanup even when the block is exited via `break`, `return`, or `goto`. It now uses a cleanup attribute, preventing memory leaks in all scenarios. |
| **🏷️ `cmlass` Macro** | The OOP macro `class` has been renamed to `cmlass` to avoid conflicts with the reserved keyword in C++. This makes the library safe and easy to use in mixed C/C++ projects. |
| **➕ `method0` Macro** | A new macro has been introduced for defining class methods that take zero parameters, solving the previous limitation of the variadic `method` macro. |
| **🛡️ Safe `send` Macro** | The `send` macro is now safer and checks if the method pointer is `NULL` before calling, preventing potential crashes. It also handles methods with and without parameters correctly. |
| **📝 Unified `self` Parameter** | All method signatures in the OOP classes (`String`, `Array`, `Map`) now consistently use `self` as the first parameter name, replacing the confusing and C++-conflicting `this`. |
| **🔧 Portable Code** | Removed all GNU-specific `##__VA_ARGS__` extensions from the macros, ensuring the library is fully portable and compiles with any standard C compiler. |
| **📱 Android Support** | Added safe I/O functions (`cm_printf`, `cm_error`, `cm_gets`) with proper fallbacks for Android, ensuring library functions work correctly in environments where `stdout`/`stdin` might behave differently. |

### ✅ Bug Fixes in v4.2.2

| Issue | Location | Status |
|-------|----------|--------|
| Thread-unsafe exception buffer (`jmp_buf`) | `CM_TRY`/`CM_THROW` | ✅ **FIXED** with `__thread` storage |
| Memory leak in `CM_WITH_ARENA` on early exit | Arena macros | ✅ **FIXED** with cleanup attribute |
| C++ conflict with `class` keyword | OOP Macros | ✅ **FIXED** using `cmlass` |
| Inability to define zero-parameter methods | `method` macro | ✅ **FIXED** with new `method0` macro |
| GNU extensions (`##__VA_ARGS__`) | Macros | ✅ **FIXED** by using standard `__VA_ARGS__` |
| Confusing `this` parameter name | OOP Classes | ✅ **FIXED** by renaming to `self` |

### 🎯 Migration Guide from v4.2.1

If you are upgrading from version 4.2.1, the only breaking change is the OOP macro:

*   **Old (v4.2.1):** `class(Person) { ... };`
*   **New (v4.2.2):** `cmlass(Person) { ... };`

Simply replace all `class` definitions with `cmlass`. All other APIs remain backward compatible.



📦 VERSION HISTORY

Version Date Changes
4.2.2 2026 🚀 Thread-local exceptions, fixed arena cleanup, cmlass macro
4.2.1 2026 🚀 Arena allocator, unified memory system
4.0.0 2026 🔒 Thread safety, OOP macros
3.0.0 2026 ♻️ Complete rewrite, GC, classes
2.0.0 2025 📊 Basic memory tracking
1.0.0 2025 🎯 Initial release

---

📝 LICENSE

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

👨‍💻 AUTHOR

<div align="center">

Adham Hossam

GitHub: github.com/ALightbolt4G
Email: adham.hossam5020@gmail.com
Project: github.com/ALightbolt4G/CM

</div>

---

<div align="center">

🚀 CM Library v4.2.2

The Ultimate C Programming Intelligence Layer

---

⭐ If you find this library useful, please star it on GitHub! ⭐

https://img.shields.io/badge/Download-CM_Library_v4.2.3-blue

---

Happy Coding! 🎉

Documentation generated on February 27, 2026

</div>