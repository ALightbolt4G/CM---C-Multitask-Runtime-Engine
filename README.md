📚 CM Library v4.2.1 Documentation

<div align="center">

🔷 C MULTITASK INTELLIGENT LIBRARY 🔷

The Ultimate C Programming Intelligence Layer

Version 4.2.1 | By Adham Hossam | 3000+ LOC | 100% Memory Safe

```c
⚡ FAST  |  🛡️ SAFE  |  🧠 SMART  |  🔒 THREAD-SAFE
```

---

https://img.shields.io/badge/build-passing-brightgreen
https://img.shields.io/badge/version-4.2.1-blue
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
12. Utility Functions Random, etc.
13. Thread Safety Multi-threading support
14. Complete Examples Real-world examples
15. API Reference All functions at a glance
16. Best Practices How to use effectively
17. Troubleshooting Common issues solved

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
```

<div align="center">

Feature CM Library Standard C C++
GC/Arena Hybrid ✅ YES ❌ No ❌ No
Single Header ✅ YES ❌ No ❌ No
OOP Support ✅ YES ❌ No ✅ Yes
Try/Catch ✅ YES ❌ No ✅ Yes
Thread Safe ✅ YES ⚠️ Manual ⚠️ Manual
Memory Safety ✅ 100% ⚠️ Manual ⚠️ Manual

</div>

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
clang main.c CM.c -o program -lpthread
./program
```

Expected Output:

```
_________________________________________________________
                                                     
        C MULTITASK INTELLIGENT LIBRARY             
                 by Adham Hossam                     
                                                     
--------------------------------------------------------
  Version : 4.2.1
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
# or
clang main.c CM.c -o program -lpthread -lm
```

---

🧠 MEMORY MANAGEMENT SYSTEM

The Unified Memory Architecture

<div align="center">

```
┌─────────────────────────────────────────────────────┐
│                    YOUR APPLICATION                   │
└─────────────────────────────────────────────────────┘
                           │
            ┌──────────────┴──────────────┐
            ▼                              ▼
┌───────────────────────┐      ┌───────────────────────┐
│   🚀 **ARENA TIER**   │      │   ♻️ **GC TIER**      │
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
│              **SINGLE GLOBAL SYSTEM**                 │
│                `static CMMemorySystem cm_mem`         │
└─────────────────────────────────────────────────────┘
```

</div>

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

// 🔷 Single global instance
static CMMemorySystem cm_mem = {0};
```

---

🚀 ARENA ALLOCATOR (NEW in v4.2.1)

Lightning-Fast Memory Allocations

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

---

Example 2: Multiple Arenas

```c
int main() {
    // 🔷 Arena for temporary data
    CMArena* temp = cm_arena_create(4096);
    cm_arena_push(temp);
    
    printf("📝 Generating temporary strings:\n");
    for (int i = 0; i < 100; i++) {
        char* str = (char*)cm_alloc(50, "temp", __FILE__, __LINE__);
        sprintf(str, "Iteration %d", i);
        if (i % 20 == 0) printf("  %s\n", str);  // Print every 20th
    }
    
    // 💥 All 100 allocations freed at once!
    printf("\n✅ All temporary strings freed!\n");
    cm_arena_pop();
    cm_arena_destroy(temp);
    
    CM_REPORT();
    return 0;
}
```

---

Example 3: Arena with Macro (Cleanest!)

```c
int main() {
    // 🎯 Automatic arena cleanup!
    CM_WITH_ARENA(4096) {
        // Inside this block, arena is active
        char* temp = (char*)cm_alloc(500, "temp", __FILE__, __LINE__);
        strcpy(temp, "📦 This is temporary");
        printf("%s\n", temp);
        
        // More temporary allocations...
        int* data = (int*)cm_alloc(100 * sizeof(int), "temp", __FILE__, __LINE__);
        for (int i = 0; i < 100; i++) data[i] = i;
        printf("data[50] = %d\n", data[50]);
        
    } // ← 🧹 Arena automatically destroyed here!
    
    CM_REPORT();  // Clean!
    return 0;
}
```

---

📊 PERFORMANCE COMPARISON

Allocation Type Speed Tracking Free Method Use Case
Arena ⚡⚡⚡⚡⚡ None Instant (bulk) Temporary data
GC ⚡⚡⚡ Full Automatic Long-lived objects
malloc ⚡⚡ None Manual Legacy code

---

🔄 WHEN TO USE ARENA VS GC

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

---

🔍 GC EXAMPLE

```c
int main() {
    // 🔷 Allocate with tracking
    String* s1 = String_new("Hello");
    String* s2 = String_new("World");
    
    printf("📊 After allocations:\n");
    cm_gc_stats();
    
    // 🔁 Reference counting
    cm_retain(s1);  // ref_count = 2
    printf("\n🔁 After retain s1:\n");
    cm_gc_stats();
    
    // 🧹 Force collection (won't free s1 because ref_count > 0)
    cm_gc_collect();
    
    // 💣 Release
    cm_free(s1);  // ref_count = 1 (still alive)
    printf("\n💣 After first free s1:\n");
    cm_gc_stats();
    
    cm_free(s1);  // ref_count = 0 → freed
    String_delete(s2);  // ref_count = 0 → freed
    
    printf("\n✅ After all frees:\n");
    cm_gc_stats();
    
    return 0;
}
```

---

📊 GC STATISTICS OUTPUT

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
    struct String* (*concat)(struct String* this, const char* other);
    struct String* (*upper)(struct String* this);
    struct String* (*lower)(struct String* this);
    void (*print)(struct String* this);
    int (*length_func)(struct String* this);
    char (*charAt)(struct String* this, int index);
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

---

🎯 STRING EXAMPLES

Example 1: Basic String Operations

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
    
    // 🔍 Character at
    char c = s->charAt(s, 0);
    printf("🔍 First char: '%c'\n", c);
    
    String_delete(s);
    return 0;
}
```

Output:

```
📝 Original:   Hello World  
📏 Length: 14
⬆️ Uppercase:   HELLO WORLD  
⬇️ Lowercase:   hello world  
➕ Concatenated:   hello world  !!!
🔍 First char: ' '
```

---

Example 2: String with Arena (Fast!)

```c
int main() {
    // 🚀 Use arena for temporary strings
    CM_WITH_ARENA(4096) {
        String* s1 = String_new("Hello");
        String* s2 = String_new(" World");
        
        s1->concat(s1, s2->data);
        printf("📝 Result: ");
        s1->print(s1);  // Hello World
        printf("\n");
        
        // 🧹 All freed when arena destroyed!
    }
    
    CM_REPORT();  // Clean!
    return 0;
}
```

---

Example 3: Multiple String Operations

```c
int main() {
    // 🔷 Create strings
    String* first = String_new("Adham");
    String* last = String_new("Hossam");
    
    // 📝 Full name
    first->concat(first, " ");
    first->concat(first, last->data);
    
    printf("👤 Full name: ");
    first->print(first);
    printf("\n📏 Length: %d\n", first->length_func(first));
    
    // ⬆️ Uppercase version
    String* upper = String_new(first->data);
    upper->upper(upper);
    printf("⬆️ Uppercase: ");
    upper->print(upper);
    printf("\n");
    
    // Cleanup
    String_delete(first);
    String_delete(last);
    String_delete(upper);
    
    return 0;
}
```

Output:

```
👤 Full name: Adham Hossam
📏 Length: 12
⬆️ Uppercase: ADHAM HOSSAM
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
    struct Array* (*push)(struct Array* this, void* value);
    void* (*pop)(struct Array* this);
    void* (*get)(struct Array* this, int index);
    int (*size)(struct Array* this);
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

---

🎯 ARRAY EXAMPLES

Example 1: Integer Array

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
    
    // 📥📤 Pop elements
    printf("\n📤 Popping:\n");
    while (numbers->size(numbers) > 0) {
        int* val = (int*)numbers->pop(numbers);
        printf("  Popped: %d, remaining: %d\n", *val, numbers->size(numbers));
    }
    
    Array_delete(numbers);
    return 0;
}
```

Output:

```
📥 Pushing elements:
  Pushed: 0, size: 1
  Pushed: 10, size: 2
  ...
  Pushed: 90, size: 10

📤 Array contents:
  numbers[0] = 0
  numbers[1] = 10
  ...
  numbers[9] = 90

📤 Popping:
  Popped: 90, remaining: 9
  Popped: 80, remaining: 8
  ...
```

---

Example 2: String Array

```c
int main() {
    // 🔷 Create array for String pointers
    Array* strings = Array_new(sizeof(String*), 5);
    
    // 📝 Create strings
    String* names[] = {
        String_new("Adham"),
        String_new("Ahmed"),
        String_new("Mohamed")
    };
    
    // 📥 Push to array
    for (int i = 0; i < 3; i++) {
        strings->push(strings, &names[i]);
    }
    
    // 📤 Print all
    printf("👤 Names:\n");
    for (int i = 0; i < strings->size(strings); i++) {
        String** s = (String**)strings->get(strings, i);
        printf("  %d: ", i);
        (*s)->print(*s);
        printf("\n");
    }
    
    // 🧹 Cleanup
    for (int i = 0; i < 3; i++) {
        String_delete(names[i]);
    }
    Array_delete(strings);
    
    return 0;
}
```

Output:

```
👤 Names:
  0: Adham
  1: Ahmed
  2: Mohamed
```

---

Example 3: Array with Arena (1000 elements fast!)

```c
int main() {
    // 🚀 Arena for fast allocations
    CM_WITH_ARENA(8192) {
        Array* arr = Array_new(sizeof(int), 1000);
        
        // ⚡ Fast allocations (no GC tracking)
        for (int i = 0; i < 1000; i++) {
            int val = i;
            arr->push(arr, &val);
        }
        
        printf("📦 Array size: %d\n", arr->size(arr));
        printf("🔢 First: %d\n", *(int*)arr->get(arr, 0));
        printf("🔢 Last: %d\n", *(int*)arr->get(arr, 999));
        
        // 🧹 All freed when arena destroyed!
    }
    
    CM_REPORT();  // Clean!
    return 0;
}
```

Output:

```
📦 Array size: 1000
🔢 First: 0
🔢 Last: 999
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
    struct Map* (*set)(struct Map* this, const char* key, void* value);
    void* (*get)(struct Map* this, const char* key);
    int (*has)(struct Map* this, const char* key);
    int (*size_func)(struct Map* this);
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

---

🎯 MAP EXAMPLES

Example 1: Basic Map Operations

```c
int main() {
    // 🔷 Create map
    Map* dict = Map_new();
    
    // 📦 Store different types
    int age = 25;
    float pi = 3.14159f;
    double e = 2.71828;
    char* name = "Adham";
    
    dict->set(dict, "age", &age);
    dict->set(dict, "pi", &pi);
    dict->set(dict, "e", &e);
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
    
    if (dict->has(dict, "pi")) {
        float* pi_ptr = (float*)dict->get(dict, "pi");
        printf("  pi = %f\n", *pi_ptr);
    }
    
    Map_delete(dict);
    return 0;
}
```

Output:

```
🗺️ Map size: 4
  age = 25
  name = Adham
  pi = 3.141590
```

---

Example 2: Word Counter

```c
int main() {
    // 🔷 Create map for word counting
    Map* wordCount = Map_new();
    char text[] = "the quick brown fox jumps over the lazy dog";
    
    // 📝 Split and count words
    printf("📝 Counting words in: \"%s\"\n\n", text);
    
    char* token = strtok(text, " ");
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
    
    // 📊 Print word counts (simplified)
    printf("📊 Word frequencies:\n");
    // Note: Would need keys() method for full iteration
    
    Map_delete(wordCount);
    return 0;
}
```

---

Example 3: Config Map with Arena

```c
int main() {
    // 🚀 Arena for temporary config
    CM_WITH_ARENA(4096) {
        Map* config = Map_new();
        
        int port = 8080;
        int workers = 4;
        char* host = "localhost";
        int timeout = 30;
        int max_conn = 1000;
        
        config->set(config, "port", &port);
        config->set(config, "workers", &workers);
        config->set(config, "host", &host);
        config->set(config, "timeout", &timeout);
        config->set(config, "max_connections", &max_conn);
        
        printf("⚙️ Server config:\n");
        printf("  port = %d\n", *(int*)config->get(config, "port"));
        printf("  workers = %d\n", *(int*)config->get(config, "workers"));
        printf("  host = %s\n", *(char**)config->get(config, "host"));
        printf("  timeout = %d\n", *(int*)config->get(config, "timeout"));
        printf("  max_connections = %d\n", *(int*)config->get(config, "max_connections"));
        
        // 🧹 All freed when arena destroyed!
    }
    
    return 0;
}
```

Output:

```
⚙️ Server config:
  port = 8080
  workers = 4
  host = localhost
  timeout = 30
  max_connections = 1000
```

---

🚨 ERROR HANDLING

Try/Catch for C

<div style="background-color: #ffe6e6; padding: 15px; border-radius: 5px;">

CM Library provides exception handling similar to C++/Java.

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

---

🎯 ERROR HANDLING EXAMPLES

Example 1: Basic Try/Catch

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

Example 2: Multiple Error Types

```c
int divide(int a, int b) {
    if (b == 0) {
        cmThrow(CM_ERROR_DIVISION_BY_ZERO, "Division by zero!");
    }
    return a / b;
}

int main() {
    cmTry {
        printf("🔷 Trying division...\n");
        
        int result = divide(10, 0);
        printf("Result: %d\n", result);
        
    } cmCatch {
        printf("✅ Caught: %s\n", cmErrorMsg());
        printf("📊 Error: %s\n", cm_error_string(cmErrorCode()));
    }
    
    return 0;
}
```

Output:

```
🔷 Trying division...
✅ Caught: Division by zero!
📊 Error: Division by zero
```

---

Example 3: Array Bounds Checking

```c
int safe_array_get(Array* arr, int index) {
    if (!arr) {
        cmThrow(CM_ERROR_NULL_POINTER, "Array is NULL");
    }
    if (index < 0 || index >= arr->size(arr)) {
        cmThrow(CM_ERROR_OUT_OF_BOUNDS, "Index out of bounds");
    }
    int* val = (int*)arr->get(arr, index);
    return *val;
}

int main() {
    Array* arr = Array_new(sizeof(int), 5);
    int x = 42;
    arr->push(arr, &x);
    
    cmTry {
        printf("🔷 Trying to get index 5...\n");
        int val = safe_array_get(arr, 5);
        printf("Value: %d\n", val);
        
    } cmCatch {
        printf("✅ Caught: %s\n", cmErrorMsg());
    }
    
    Array_delete(arr);
    return 0;
}
```

Output:

```
🔷 Trying to get index 5...
✅ Caught: Index out of bounds
```

---

🎯 OOP MACROS

Object-Oriented Programming in C

<div style="background-color: #e6ffe6; padding: 15px; border-radius: 5px;">

CM Library provides macros for class-based OOP similar to C++/Java.

</div>

OOP Macros

Macro Description Example
class(name) Define a class class(Person) { ... }
end_class End class definition } end_class;
property(type, name) Define property property(char*, name)
method(name, ret, ...) Define method method(speak, void)
send(obj, method, ...) Call method send(p, speak)

---

🎯 CUSTOM CLASS EXAMPLES

Example 1: Person Class

```c
// 🔷 Define Person class
class(Person) {
    property(char*, name);
    property(int, age);
    
    method(init, void, const char*, int);
    method(speak, void);
    method(haveBirthday, void);
} end_class;

// 📝 Implement methods
void Person_init(Person* this, const char* name, int age) {
    this->name = strdup(name);
    this->age = age;
}

void Person_speak(Person* this) {
    printf("👤 Hi, I'm %s and I'm %d years old\n", this->name, this->age);
}

void Person_haveBirthday(Person* this) {
    this->age++;
    printf("🎂 Happy Birthday! Now I'm %d\n", this->age);
}

int main() {
    // 🔷 Create and use Person
    Person p;
    Person_init(&p, "Adham", 25);
    
    send(&p, speak);
    send(&p, haveBirthday);
    
    free(p.name);
    return 0;
}
```

Output:

```
👤 Hi, I'm Adham and I'm 25 years old
🎂 Happy Birthday! Now I'm 26
```

---

Example 2: Rectangle Class

```c
// 🔷 Define Rectangle class
class(Rectangle) {
    property(int, width);
    property(int, height);
    
    method(init, void, int, int);
    method(area, int);
    method(perimeter, int);
    method(print, void);
} end_class;

// 📝 Implement methods
void Rectangle_init(Rectangle* this, int w, int h) {
    this->width = w;
    this->height = h;
}

int Rectangle_area(Rectangle* this) {
    return this->width * this->height;
}

int Rectangle_perimeter(Rectangle* this) {
    return 2 * (this->width + this->height);
}

void Rectangle_print(Rectangle* this) {
    printf("📐 Rectangle[%dx%d]\n", this->width, this->height);
}

int main() {
    Rectangle r;
    Rectangle_init(&r, 10, 5);
    
    send(&r, print);
    printf("📏 Area: %d\n", send(&r, area));
    printf("📏 Perimeter: %d\n", send(&r, perimeter));
    
    return 0;
}
```

Output:

```
📐 Rectangle[10x5]
📏 Area: 50
📏 Perimeter: 30
```

---

Example 3: Stack Class

```c
// 🔷 Define Stack class
class(Stack) {
    property(Array*, data);
    
    method(init, void);
    method(push, void, int);
    method(pop, int);
    method(peek, int);
    method(isEmpty, int);
    method(print, void);
} end_class;

// 📝 Implement methods
void Stack_init(Stack* this) {
    this->data = Array_new(sizeof(int), 10);
}

void Stack_push(Stack* this, int value) {
    this->data->push(this->data, &value);
}

int Stack_pop(Stack* this) {
    int* val = (int*)this->data->pop(this->data);
    return val ? *val : -1;
}

int Stack_peek(Stack* this) {
    if (this->data->size(this->data) == 0) return -1;
    int* val = (int*)this->data->get(this->data, this->data->size(this->data) - 1);
    return *val;
}

int Stack_isEmpty(Stack* this) {
    return this->data->size(this->data) == 0;
}

void Stack_print(Stack* this) {
    printf("📚 Stack[");
    for (int i = 0; i < this->data->size(this->data); i++) {
        int* val = (int*)this->data->get(this->data, i);
        printf("%d", *val);
        if (i < this->data->size(this->data) - 1) printf(", ");
    }
    printf("]\n");
}

int main() {
    Stack s;
    Stack_init(&s);
    
    send(&s, push, 10);
    send(&s, push, 20);
    send(&s, push, 30);
    
    send(&s, print);
    
    printf("📤 Pop: %d\n", send(&s, pop));
    printf("👀 Peek: %d\n", send(&s, peek));
    
    send(&s, print);
    
    Array_delete(s.data);
    return 0;
}
```

Output:

```
📚 Stack[10, 20, 30]
📤 Pop: 30
👀 Peek: 20
📚 Stack[10, 20]
```

---

🛠️ UTILITY FUNCTIONS

Random and Helpers

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

---

🎯 RANDOM EXAMPLES

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

Output:

```
🎲 Random numbers (1-100):
  42
  17
  83
  56
  91

📝 Random strings:
  aB7kL9pQ2rX
  mN5vT8wY3zK
  xR2fG6hJ4sP
```

📝 User Input Example

```c
int main() {
    String* name = cm_input("👤 Enter your name: ");
    printf("Hello, ");
    name->print(name);
    printf("!\n");
    
    String_delete(name);
    return 0;
}
```

---

🔒 THREAD SAFETY

Multi-threading Support

<div style="background-color: #fff0f0; padding: 15px; border-radius: 5px;">

CM Library is fully thread-safe with mutex protection for all operations.

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

---

🎯 MULTI-THREADING EXAMPLE

```c
#include <pthread.h>

void* thread_func(void* arg) {
    int id = *(int*)arg;
    
    // 🔷 Each thread can use the library safely
    String* s = String_new("Thread ");
    char num[10];
    sprintf(num, "%d", id);
    s->concat(s, num);
    
    printf("🧵 Thread %d: ", id);
    s->print(s);
    printf("\n");
    
    String_delete(s);
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

Output:

```
🚀 Starting 5 threads...

🧵 Thread 0: Thread 0
🧵 Thread 1: Thread 1
🧵 Thread 2: Thread 2
🧵 Thread 3: Thread 3
🧵 Thread 4: Thread 4

✅ All threads completed
```

---

📚 COMPLETE EXAMPLES

Example 1: Student Management System

```c
#include "CM.h"

// 🔷 Define Student class
class(Student) {
    property(String*, name);
    property(int, age);
    property(float, gpa);
    
    method(init, void, const char*, int, float);
    method(print, void);
    method(updateGPA, void, float);
} end_class;

// 📝 Implement methods
void Student_init(Student* this, const char* name, int age, float gpa) {
    this->name = String_new(name);
    this->age = age;
    this->gpa = gpa;
}

void Student_print(Student* this) {
    printf("👤 Student: ");
    this->name->print(this->name);
    printf(", age: %d, GPA: %.2f\n", this->age, this->gpa);
}

void Student_updateGPA(Student* this, float new_gpa) {
    this->gpa = new_gpa;
    printf("📊 GPA updated to: %.2f\n", this->gpa);
}

int main() {
    CM_ABOUT();
    
    cmTry {
        // 🔷 Create students array
        Array* students = Array_new(sizeof(Student), 5);
        
        // 👤 Create students
        Student s1, s2, s3;
        Student_init(&s1, "Adham", 25, 3.8f);
        Student_init(&s2, "Ahmed", 22, 3.5f);
        Student_init(&s3, "Mohamed", 24, 3.9f);
        
        students->push(students, &s1);
        students->push(students, &s2);
        students->push(students, &s3);
        
        printf("\n📋 --- Student List ---\n");
        for (int i = 0; i < students->size(students); i++) {
            Student* s = (Student*)students->get(students, i);
            send(s, print);
        }
        
        // 📈 Update GPA
        printf("\n📈 --- Updating GPA ---\n");
        Student_updateGPA(&s2, 3.7f);
        
        // 🏆 Find top student
        printf("\n🏆 --- Top Student ---\n");
        Student* top = &s1;
        for (int i = 1; i < students->size(students); i++) {
            Student* s = (Student*)students->get(students, i);
            if (s->gpa > top->gpa) top = s;
        }
        printf("🏆 Top student: ");
        top->name->print(top->name);
        printf(" (GPA: %.2f)\n", top->gpa);
        
        // 🧹 Cleanup
        String_delete(s1.name);
        String_delete(s2.name);
        String_delete(s3.name);
        Array_delete(students);
        
    } cmCatch {
        printf("❌ Error: %s\n", cmErrorMsg());
    }
    
    CM_REPORT();
    return 0;
}
```

---

Example 2: HTTP Request Parser (Simulated)

```c
typedef struct {
    String* method;
    String* path;
    Map* headers;
    String* body;
} Request;

Request* Request_new(const char* raw) {
    Request* req = (Request*)cm_alloc(sizeof(Request), "request", __FILE__, __LINE__);
    
    // 🔷 Simple parsing simulation
    req->method = String_new("GET");
    req->path = String_new("/index.html");
    req->headers = Map_new();
    req->body = String_new("");
    
    // 📦 Add some headers
    char* host = "localhost";
    char* agent = "CM-Library/4.2.1";
    char* accept = "*/*";
    
    req->headers->set(req->headers, "Host", &host);
    req->headers->set(req->headers, "User-Agent", &agent);
    req->headers->set(req->headers, "Accept", &accept);
    
    return req;
}

void Request_print(Request* req) {
    printf("🌐 %s %s HTTP/1.1\n", req->method->data, req->path->data);
    printf("   Host: %s\n", *(char**)req->headers->get(req->headers, "Host"));
    printf("   User-Agent: %s\n", *(char**)req->headers->get(req->headers, "User-Agent"));
    printf("   Accept: %s\n", *(char**)req->headers->get(req->headers, "Accept"));
    printf("\n   Body: %s\n", req->body->data);
}

void Request_delete(Request* req) {
    String_delete(req->method);
    String_delete(req->path);
    Map_delete(req->headers);
    String_delete(req->body);
    cm_free(req);
}

int main() {
    CM_WITH_ARENA(8192) {
        Request* req = Request_new("GET /index.html HTTP/1.1");
        Request_print(req);
        Request_delete(req);
    }
    
    return 0;
}
```

Output:

```
🌐 GET /index.html HTTP/1.1
   Host: localhost
   User-Agent: CM-Library/4.2.1
   Accept: */*

   Body:
```

---

Example 3: Game Entity System

```c
typedef struct {
    float x, y;
    float vx, vy;
    String* name;
    int health;
} Entity;

Entity* Entity_new(const char* name, float x, float y) {
    Entity* e = (Entity*)cm_alloc(sizeof(Entity), "entity", __FILE__, __LINE__);
    e->name = String_new(name);
    e->x = x; e->y = y;
    e->vx = e->vy = 0;
    e->health = 100;
    return e;
}

void Entity_update(Entity* e, float dt) {
    e->x += e->vx * dt;
    e->y += e->vy * dt;
}

void Entity_damage(Entity* e, int amount) {
    e->health -= amount;
    if (e->health < 0) e->health = 0;
    printf("💥 %s took %d damage, health now %d\n", 
           e->name->data, amount, e->health);
}

void Entity_print(Entity* e) {
    printf("👾 Entity[%s] at (%.1f, %.1f) health: %d\n",
           e->name->data, e->x, e->y, e->health);
}

int main() {
    Array* entities = Array_new(sizeof(Entity*), 10);
    
    // 👾 Create entities
    Entity* player = Entity_new("Player", 0, 0);
    Entity* enemy = Entity_new("Enemy", 10, 10);
    
    entities->push(entities, &player);
    entities->push(entities, &enemy);
    
    // 🎮 Game loop simulation
    for (int frame = 0; frame < 3; frame++) {
        printf("\n🎮 --- Frame %d ---\n", frame);
        
        for (int i = 0; i < entities->size(entities); i++) {
            Entity** e = (Entity**)entities->get(entities, i);
            Entity_update(*e, 0.1f);
            Entity_print(*e);
        }
        
        if (frame == 1) {
            Entity_damage(player, 10);
        }
    }
    
    // 🧹 Cleanup
    for (int i = 0; i < entities->size(entities); i++) {
        Entity** e = (Entity**)entities->get(entities, i);
        String_delete((*e)->name);
        cm_free(*e);
    }
    Array_delete(entities);
    
    return 0;
}
```

Output:

```
🎮 --- Frame 0 ---
👾 Entity[Player] at (0.0, 0.0) health: 100
👾 Entity[Enemy] at (10.0, 10.0) health: 100

🎮 --- Frame 1 ---
👾 Entity[Player] at (0.0, 0.0) health: 100
👾 Entity[Enemy] at (10.0, 10.0) health: 100
💥 Player took 10 damage, health now 90

🎮 --- Frame 2 ---
👾 Entity[Player] at (0.0, 0.0) health: 90
👾 Entity[Enemy] at (10.0, 10.0) health: 100
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

Error Handling

Function/Macro Description
cmTry Start try block
cmCatch Catch errors
cmThrow(e, m) Throw error
cmErrorMsg() Get error message
cmErrorCode() Get error code
cm_error_string(code) Get error description

Utility

Function Description
cm_random_seed(seed) Set random seed
cm_random_string(buf, len) Generate random string
CM_ABOUT() Show library info
CM_REPORT() Show GC statistics
cm_input(prompt) Read user input

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

Thread Safety

```c
// ✅ GOOD: Library is thread-safe by default
void* thread_func(void* arg) {
    String* s = String_new("Hello");  // Safe in threads
    // ... use ...
    String_delete(s);  // Safe in threads
    return NULL;
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

📦 VERSION HISTORY

Version Date Changes
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

 
GitHub github.com/ALightbolt4G
Email adham.hossam5020@gmail.com
Project github.com/ALightbolt4G/CM

</div>

---

<div align="center">

🚀 CM Library v4.2.1

The Ultimate C Programming Intelligence Layer

---

⭐ If you find this library useful, please star it on GitHub! ⭐

https://img.shields.io/badge/Download-CM_Library-blue

---

Happy Coding! 🎉

Documentation generated on February 23, 2026

</div>