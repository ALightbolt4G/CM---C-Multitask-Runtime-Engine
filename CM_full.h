#ifndef CM_FULL_H
#define CM_FULL_H

/*
 * ============================================================================
 * CM_full.h - C Multitask Intelligent Library
 * Complete Single-File Implementation
 * Author: Adham Hossam
 * Version: 3.0.0
 * ============================================================================
 */

/* ============================================================================
 * INCLUDES
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>
#include <setjmp.h>

/* ============================================================================
 * CONFIGURATION
 * ============================================================================ */
#define CM_VERSION "3.0.0"
#define CM_AUTHOR "Adham Hossam"
#define CM_GC_THRESHOLD (1024 * 1024)
#define CM_LOG_LEVEL 3

/* ============================================================================
 * ERROR CODES
 * ============================================================================ */
#define CM_SUCCESS                   0
#define CM_ERROR_MEMORY               1
#define CM_ERROR_NULL_POINTER          2
#define CM_ERROR_OUT_OF_BOUNDS         3
#define CM_ERROR_DIVISION_BY_ZERO      4
#define CM_ERROR_OVERFLOW               5
#define CM_ERROR_UNDERFLOW              6
#define CM_ERROR_INVALID_ARGUMENT       7
#define CM_ERROR_NOT_FOUND              8
#define CM_ERROR_ALREADY_EXISTS         9
#define CM_ERROR_PERMISSION_DENIED     10
#define CM_ERROR_IO                     11
#define CM_ERROR_NETWORK                12
#define CM_ERROR_TIMEOUT                13
#define CM_ERROR_THREAD                 14
#define CM_ERROR_SYNC                   15
#define CM_ERROR_PARSE                  16
#define CM_ERROR_TYPE                   17
#define CM_ERROR_UNIMPLEMENTED          18
#define CM_ERROR_UNKNOWN                19

/* ============================================================================
 * TYPE DEFINITIONS
 * ============================================================================ */
typedef struct CMObject CMObject;
typedef struct CMGC CMGC;
typedef struct cm_string cm_string_t;
typedef struct cm_array cm_array_t;
typedef struct cm_map cm_map_t;
typedef struct cm_map_entry cm_map_entry_t;

/* Garbage Collector Structures */
struct CMObject {
    void* ptr;
    size_t size;
    const char* type;
    const char* file;
    int line;
    time_t alloc_time;
    int ref_count;
    int marked;
    uint32_t hash;
    struct CMObject* next;
    struct CMObject* prev;
    void (*destructor)(void*);
    void (*mark_cb)(void*);
};

struct CMGC {
    CMObject* head;
    CMObject* tail;
    size_t total_objects;
    size_t total_memory;
    size_t peak_memory;
    size_t collections;
    size_t allocations;
    size_t frees;
    double avg_collection_time;
    double total_collection_time;
    int gc_enabled;
    int gc_threshold;
};

/* String Structure */
struct cm_string {
    char* data;
    size_t length;
    size_t capacity;
    int ref_count;
    uint32_t hash;
    time_t created;
    int flags;
};

/* Array Structure */
struct cm_array {
    void* data;
    size_t element_size;
    size_t length;
    size_t capacity;
    int* ref_counts;
    void (*element_destructor)(void*);
    int flags;
};

/* Map Structures */
struct cm_map_entry {
    char* key;
    void* value;
    size_t value_size;
    uint32_t hash;
    struct cm_map_entry* next;
};

struct cm_map {
    cm_map_entry_t** buckets;
    int bucket_count;
    int size;
    float load_factor;
    int growth_factor;
};

/* ============================================================================
 * GLOBAL VARIABLES
 * ============================================================================ */
static CMGC cm_gc = {0};
static jmp_buf* cm_exception_buffer = NULL;
static int cm_last_error = 0;
static char cm_error_message[1024] = {0};

/* ============================================================================
 * MACROS
 * ============================================================================ */
 /* Error handling macros */
#define CM_TRY() \
    jmp_buf __cm_buf; \
    cm_exception_buffer = &__cm_buf; \
    if (setjmp(__cm_buf) == 0)

#define CM_CATCH() \
    else

#define CM_THROW(error, message) \
    do { \
        cm_last_error = error; \
        strncpy(cm_error_message, message, sizeof(cm_error_message) - 1); \
        if (cm_exception_buffer) { \
            longjmp(*cm_exception_buffer, error);  /* ← غير 1 إلى error */ \
        } else { \
            fprintf(stderr, "FATAL: Uncaught exception: %s\n", message); \
            exit(error); \
        } \
    } while(0)

#define CM_TRY_FINALLY() \
    jmp_buf __cm_buf; \
    cm_exception_buffer = &__cm_buf; \
    if (setjmp(__cm_buf) == 0)

#define CM_FINALLY() \
    else
#define CM_ABOUT() \
    do { \
        printf("\n"); \
        printf("╔════════════════════════════════════════════════════════════════════╗\n"); \
        printf("║                 C MULTITASK INTELLIGENT LIBRARY                   ║\n"); \
        printf("║                         by Adham Hossam                            ║\n"); \
        printf("╠════════════════════════════════════════════════════════════════════╣\n"); \
        printf("║ Version: %-45s ║\n", CM_VERSION); \
        printf("║ Author:  %-45s ║\n", CM_AUTHOR); \
        printf("╚════════════════════════════════════════════════════════════════════╝\n"); \
        printf("\n"); \
    } while(0)

/* String macros */
#define CM_STR(s) cm_string_new(s)
#define CM_STR_FREE(s) cm_string_free(s)
#define CM_STR_LEN(s) ((s) ? (s)->length : 0)

/* Array macros */
#define CM_ARR(type, size) cm_array_new(sizeof(type), size)
#define CM_ARR_FREE(a) cm_array_free(a)
#define CM_ARR_LEN(a) ((a) ? (a)->length : 0)
#define CM_ARR_GET(a, i, type) (*(type*)cm_array_get(a, i))
#define CM_ARR_PUSH(a, type, v) do { type __tmp = (v); cm_array_push(a, &__tmp); } while(0)

/* Map macros */
#define CM_MAP() cm_map_new()
#define CM_MAP_FREE(m) cm_map_free(m)
#define CM_MAP_SET_INT(m, k, v) do { int __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(int)); } while(0)
#define CM_MAP_SET_STRING(m, k, v) do { const char* __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(const char*)); } while(0)
#define CM_MAP_GET_INT(m, k) (*(int*)cm_map_get(m, k))
#define CM_MAP_GET_STRING(m, k) (*(char**)cm_map_get(m, k))
#define CM_MAP_HAS(m, k) cm_map_has(m, k)

/* Random macros */
#define CM_RAND_INT(min, max) (min + rand() % ((max) - (min) + 1))

/* Debug macros */
#define CM_REPORT() cm_gc_stats()

/* ============================================================================
 * FUNCTION DECLARATIONS
 * ============================================================================ */
void cm_gc_init(void);
void cm_gc_collect(void);
void cm_gc_stats(void);
void* cm_alloc(size_t size, const char* type, const char* file, int line);
void cm_free(void* ptr);
void cm_retain(void* ptr);

cm_string_t* cm_string_new(const char* initial);
void cm_string_free(cm_string_t* s);
size_t cm_string_length(cm_string_t* s);
cm_string_t* cm_string_format(const char* format, ...);
void cm_string_set(cm_string_t* s, const char* value);
void cm_string_upper(cm_string_t* s);
void cm_string_lower(cm_string_t* s);

cm_array_t* cm_array_new(size_t element_size, size_t initial_capacity);
void cm_array_free(cm_array_t* arr);
void* cm_array_get(cm_array_t* arr, size_t index);
void cm_array_push(cm_array_t* arr, const void* value);
void* cm_array_pop(cm_array_t* arr);
size_t cm_array_length(cm_array_t* arr);

cm_map_t* cm_map_new(void);
void cm_map_free(cm_map_t* map);
void cm_map_set(cm_map_t* map, const char* key, const void* value, size_t value_size);
void* cm_map_get(cm_map_t* map, const char* key);
int cm_map_has(cm_map_t* map, const char* key);
size_t cm_map_size(cm_map_t* map);

void cm_random_seed(unsigned int seed);
void cm_random_string(char* buffer, size_t length);
/* Error handling functions */
const char* cm_error_string(int error);
const char* cm_error_get_message(void);
int cm_error_get_last(void);
void cm_error_clear(void);
void cm_error_set(int error, const char* message);

/* ============================================================================
 * IMPLEMENTATION
 * ============================================================================ */

/* GC Implementation */
void cm_gc_init(void) {
    memset(&cm_gc, 0, sizeof(CMGC));
    cm_gc.gc_enabled = 1;
    cm_gc.gc_threshold = CM_GC_THRESHOLD;
    printf("[GC] Initialized with threshold: %d bytes\n", cm_gc.gc_threshold);
}

void* cm_alloc(size_t size, const char* type, const char* file, int line) {
    if (size == 0) return NULL;
    
    void* ptr = malloc(size);
    if (!ptr) return NULL;
    
    CMObject* obj = (CMObject*)malloc(sizeof(CMObject));
    if (!obj) {
        free(ptr);
        return NULL;
    }
    
    obj->ptr = ptr;
    obj->size = size;
    obj->type = type ? type : "unknown";
    obj->file = file ? file : "unknown";
    obj->line = line;
    obj->alloc_time = time(NULL);
    obj->ref_count = 1;
    obj->marked = 0;
    obj->hash = 0;
    obj->next = NULL;
    obj->prev = NULL;
    obj->destructor = NULL;
    obj->mark_cb = NULL;
    
    if (cm_gc.tail) {
        cm_gc.tail->next = obj;
        obj->prev = cm_gc.tail;
        cm_gc.tail = obj;
    } else {
        cm_gc.head = cm_gc.tail = obj;
    }
    
    cm_gc.total_objects++;
    cm_gc.total_memory += size;
    cm_gc.allocations++;
    
    if (cm_gc.total_memory > cm_gc.peak_memory) {
        cm_gc.peak_memory = cm_gc.total_memory;
    }
    
    return ptr;
}

void cm_free(void* ptr) {
    if (!ptr) return;
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            obj->ref_count--;
            
            if (obj->ref_count <= 0) {
                if (obj->destructor) {
                    obj->destructor(ptr);
                }
                
                free(ptr);
                
                if (obj->prev) {
                    obj->prev->next = obj->next;
                } else {
                    cm_gc.head = obj->next;
                }
                
                if (obj->next) {
                    obj->next->prev = obj->prev;
                } else {
                    cm_gc.tail = obj->prev;
                }
                
                cm_gc.total_objects--;
                cm_gc.total_memory -= obj->size;
                cm_gc.frees++;
                
                free(obj);
            }
            return;
        }
    }
    
    free(ptr);
}

void cm_gc_collect(void) {
    if (!cm_gc.gc_enabled) return;
    
    printf("[GC] Starting collection...\n");
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        obj->marked = (obj->ref_count > 0) ? 1 : 0;
    }
    
    CMObject* current = cm_gc.head;
    size_t freed_memory = 0;
    int freed_objects = 0;
    
    while (current) {
        CMObject* next = current->next;
        
        if (!current->marked) {
            freed_memory += current->size;
            freed_objects++;
            
            if (current->destructor) {
                current->destructor(current->ptr);
            }
            free(current->ptr);
            
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                cm_gc.head = current->next;
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                cm_gc.tail = current->prev;
            }
            
            cm_gc.total_objects--;
            cm_gc.total_memory -= current->size;
            cm_gc.frees++;
            
            free(current);
        }
        
        current = next;
    }
    
    cm_gc.collections++;
    
    printf("[GC] Completed: freed %d objects (%zu bytes)\n", 
           freed_objects, freed_memory);
}

void cm_gc_stats(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              GARBAGE COLLECTOR STATISTICS                 ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ Total objects:    %20zu                              ║\n", cm_gc.total_objects);
    printf("║ Total memory:     %20zu bytes                         ║\n", cm_gc.total_memory);
    printf("║ Peak memory:      %20zu bytes                         ║\n", cm_gc.peak_memory);
    printf("║ Allocations:      %20zu                              ║\n", cm_gc.allocations);
    printf("║ Frees:            %20zu                              ║\n", cm_gc.frees);
    printf("║ Collections:      %20zu                              ║\n", cm_gc.collections);
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

void cm_retain(void* ptr) {
    if (!ptr) return;
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            obj->ref_count++;
            break;
        }
    }
}

/* ============================================================================
 * ERROR HANDLING IMPLEMENTATION
 * ============================================================================ */
const char* cm_error_string(int error) {
    switch(error) {
        case CM_SUCCESS: return "Success";
        case CM_ERROR_MEMORY: return "Memory allocation failed";
        case CM_ERROR_NULL_POINTER: return "Null pointer dereference";
        case CM_ERROR_OUT_OF_BOUNDS: return "Index out of bounds";
        case CM_ERROR_DIVISION_BY_ZERO: return "Division by zero";
        case CM_ERROR_OVERFLOW: return "Integer overflow";
        case CM_ERROR_UNDERFLOW: return "Integer underflow";
        case CM_ERROR_INVALID_ARGUMENT: return "Invalid argument";
        case CM_ERROR_NOT_FOUND: return "Not found";
        case CM_ERROR_ALREADY_EXISTS: return "Already exists";
        case CM_ERROR_PERMISSION_DENIED: return "Permission denied";
        case CM_ERROR_IO: return "I/O error";
        case CM_ERROR_NETWORK: return "Network error";
        case CM_ERROR_TIMEOUT: return "Operation timed out";
        case CM_ERROR_THREAD: return "Thread error";
        case CM_ERROR_SYNC: return "Synchronization error";
        case CM_ERROR_PARSE: return "Parse error";
        case CM_ERROR_TYPE: return "Type error";
        case CM_ERROR_UNIMPLEMENTED: return "Unimplemented";
        case CM_ERROR_UNKNOWN: return "Unknown error";
        default: return "Unknown error code";
    }
}

const char* cm_error_get_message(void) {
    return cm_error_message;
}

int cm_error_get_last(void) {
    return cm_last_error;
}

void cm_error_clear(void) {
    cm_last_error = 0;
    cm_error_message[0] = '\0';
}

void cm_error_set(int error, const char* message) {
    cm_last_error = error;
    if (message) {
        strncpy(cm_error_message, message, sizeof(cm_error_message) - 1);
    }
}


/* String Implementation */
#define CM_STRING_COPY 0x01
#define CM_STRING_NOCOPY 0x02

cm_string_t* cm_string_new(const char* initial) {
    cm_string_t* s = (cm_string_t*)cm_alloc(sizeof(cm_string_t), "string", __FILE__, __LINE__);
    if (!s) return NULL;
    
    size_t len = initial ? strlen(initial) : 0;
    s->length = len;
    s->capacity = len + 1;
    s->data = (char*)cm_alloc(s->capacity, "string_data", __FILE__, __LINE__);
    s->ref_count = 1;
    s->hash = 0;
    s->created = time(NULL);
    s->flags = CM_STRING_COPY;
    
    if (s->data) {
        if (initial && len > 0) {
            memcpy(s->data, initial, len + 1);
        } else {
            s->data[0] = '\0';
        }
    }
    
    return s;
}

void cm_string_free(cm_string_t* s) {
    if (!s) return;
    
    s->ref_count--;
    if (s->ref_count <= 0) {
        if (s->data && !(s->flags & CM_STRING_NOCOPY)) {
            cm_free(s->data);
        }
        cm_free(s);
    }
}

size_t cm_string_length(cm_string_t* s) {
    return s ? s->length : 0;
}

cm_string_t* cm_string_format(const char* format, ...) {
    if (!format) return NULL;
    
    va_list args;
    va_start(args, format);
    
    int size = vsnprintf(NULL, 0, format, args);
    va_end(args);
    
    if (size < 0) return NULL;
    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) return NULL;
    
    va_start(args, format);
    vsnprintf(buffer, size + 1, format, args);
    va_end(args);
    
    cm_string_t* result = cm_string_new(buffer);
    free(buffer);
    
    return result;
}

void cm_string_set(cm_string_t* s, const char* value) {
    if (!s) return;
    if (!value) value = "";
    
    size_t len = strlen(value);
    
    if (len + 1 > s->capacity || (s->flags & CM_STRING_NOCOPY)) {
        char* new_data = (char*)cm_alloc(len + 1, "string_data", __FILE__, __LINE__);
        if (!new_data) return;
        
        if (s->data && !(s->flags & CM_STRING_NOCOPY)) {
            cm_free(s->data);
        }
        
        s->data = new_data;
        s->capacity = len + 1;
        s->flags &= ~CM_STRING_NOCOPY;
    }
    
    memcpy(s->data, value, len + 1);
    s->length = len;
    s->hash = 0;
}

void cm_string_upper(cm_string_t* s) {
    if (!s || !s->data) return;
    
    for (size_t i = 0; i < s->length; i++) {
        s->data[i] = toupper(s->data[i]);
    }
    s->hash = 0;
}

void cm_string_lower(cm_string_t* s) {
    if (!s || !s->data) return;
    
    for (size_t i = 0; i < s->length; i++) {
        s->data[i] = tolower(s->data[i]);
    }
    s->hash = 0;
}

/* Array Implementation */
cm_array_t* cm_array_new(size_t element_size, size_t initial_capacity) {
    if (element_size == 0) return NULL;
    
    cm_array_t* arr = (cm_array_t*)cm_alloc(sizeof(cm_array_t), "array", __FILE__, __LINE__);
    if (!arr) return NULL;
    
    arr->element_size = element_size;
    arr->capacity = initial_capacity > 0 ? initial_capacity : 16;
    arr->length = 0;
    arr->data = cm_alloc(element_size * arr->capacity, "array_data", __FILE__, __LINE__);
    arr->ref_counts = (int*)cm_alloc(sizeof(int) * arr->capacity, "array_refs", __FILE__, __LINE__);
    arr->element_destructor = NULL;
    arr->flags = 0;
    
    if (!arr->data || !arr->ref_counts) {
        if (arr->data) cm_free(arr->data);
        if (arr->ref_counts) cm_free(arr->ref_counts);
        cm_free(arr);
        return NULL;
    }
    
    memset(arr->ref_counts, 0, sizeof(int) * arr->capacity);
    
    return arr;
}

void cm_array_free(cm_array_t* arr) {
    if (!arr) return;
    
    if (arr->element_destructor) {
        for (size_t i = 0; i < arr->length; i++) {
            void* elem = (char*)arr->data + (i * arr->element_size);
            arr->element_destructor(elem);
        }
    }
    
    cm_free(arr->data);
    cm_free(arr->ref_counts);
    cm_free(arr);
}

void* cm_array_get(cm_array_t* arr, size_t index) {
    if (!arr) return NULL;
    if (index >= arr->length) return NULL;
    
    arr->ref_counts[index]++;
    return (char*)arr->data + (index * arr->element_size);
}

void cm_array_push(cm_array_t* arr, const void* value) {
    if (!arr || !value) return;
    
    if (arr->length >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        void* new_data = cm_alloc(arr->element_size * new_capacity, "array_data", __FILE__, __LINE__);
        int* new_refs = (int*)cm_alloc(sizeof(int) * new_capacity, "array_refs", __FILE__, __LINE__);
        
        if (!new_data || !new_refs) {
            if (new_data) cm_free(new_data);
            if (new_refs) cm_free(new_refs);
            return;
        }
        
        memcpy(new_data, arr->data, arr->element_size * arr->length);
        memset(new_refs, 0, sizeof(int) * new_capacity);
        
        cm_free(arr->data);
        cm_free(arr->ref_counts);
        
        arr->data = new_data;
        arr->ref_counts = new_refs;
        arr->capacity = new_capacity;
    }
    
    
    
    void* dest = (char*)arr->data + (arr->length * arr->element_size);
    memcpy(dest, value, arr->element_size);
    arr->length++;
}


/* Pop element from back */
void* cm_array_pop(cm_array_t* arr) {
    if (!arr || arr->length == 0) return NULL;
    
    arr->length--;
    void* elem = (char*)arr->data + (arr->length * arr->element_size);
    
    return elem;
}


size_t cm_array_length(cm_array_t* arr) {
    return arr ? arr->length : 0;
}



/* Map Implementation */
#define CM_MAP_INITIAL_SIZE 16
#define CM_MAP_LOAD_FACTOR 0.75

static uint32_t cm_hash_string(const char* str) {
    uint32_t hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

cm_map_t* cm_map_new(void) {
    cm_map_t* map = (cm_map_t*)cm_alloc(sizeof(cm_map_t), "map", __FILE__, __LINE__);
    if (!map) return NULL;
    
    map->bucket_count = CM_MAP_INITIAL_SIZE;
    map->size = 0;
    map->load_factor = CM_MAP_LOAD_FACTOR;
    map->growth_factor = 2;
    map->buckets = (cm_map_entry_t**)cm_alloc(sizeof(cm_map_entry_t*) * map->bucket_count, 
                                               "map_buckets", __FILE__, __LINE__);
    
    if (!map->buckets) {
        cm_free(map);
        return NULL;
    }
    
    memset(map->buckets, 0, sizeof(cm_map_entry_t*) * map->bucket_count);
    
    return map;
}

static void cm_map_resize(cm_map_t* map, int new_size) {
    if (!map) return;
    
    cm_map_entry_t** new_buckets = (cm_map_entry_t**)cm_alloc(
        sizeof(cm_map_entry_t*) * new_size, "map_buckets", __FILE__, __LINE__);
    
    if (!new_buckets) return;
    
    memset(new_buckets, 0, sizeof(cm_map_entry_t*) * new_size);
    
    for (int i = 0; i < map->bucket_count; i++) {
        cm_map_entry_t* entry = map->buckets[i];
        while (entry) {
            cm_map_entry_t* next = entry->next;
            
            int new_index = entry->hash % new_size;
            entry->next = new_buckets[new_index];
            new_buckets[new_index] = entry;
            
            entry = next;
        }
    }
    
    cm_free(map->buckets);
    map->buckets = new_buckets;
    map->bucket_count = new_size;
}

void cm_map_set(cm_map_t* map, const char* key, const void* value, size_t value_size) {
    if (!map || !key || !value) return;
    
    if (map->size >= map->bucket_count * map->load_factor) {
        cm_map_resize(map, map->bucket_count * map->growth_factor);
    }
    
    uint32_t hash = cm_hash_string(key);
    int index = hash % map->bucket_count;
    
    cm_map_entry_t* entry = map->buckets[index];
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            cm_free(entry->value);
            entry->value = cm_alloc(value_size, "map_value", __FILE__, __LINE__);
            memcpy(entry->value, value, value_size);
            entry->value_size = value_size;
            return;
        }
        entry = entry->next;
    }
    
    entry = (cm_map_entry_t*)cm_alloc(sizeof(cm_map_entry_t), "map_entry", __FILE__, __LINE__);
    if (!entry) return;
    
    entry->key = strdup(key);
    entry->value = cm_alloc(value_size, "map_value", __FILE__, __LINE__);
    memcpy(entry->value, value, value_size);
    entry->value_size = value_size;
    entry->hash = hash;
    entry->next = map->buckets[index];
    
    map->buckets[index] = entry;
    map->size++;
}

void* cm_map_get(cm_map_t* map, const char* key) {
    if (!map || !key) return NULL;
    
    uint32_t hash = cm_hash_string(key);
    int index = hash % map->bucket_count;
    
    cm_map_entry_t* entry = map->buckets[index];
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            return entry->value;
        }
        entry = entry->next;
    }
    
    return NULL;
}

int cm_map_has(cm_map_t* map, const char* key) {
    return cm_map_get(map, key) != NULL;
}

void cm_map_free(cm_map_t* map) {
    if (!map) return;
    
    for (int i = 0; i < map->bucket_count; i++) {
        cm_map_entry_t* entry = map->buckets[i];
        while (entry) {
            cm_map_entry_t* next = entry->next;
            free(entry->key);
            cm_free(entry->value);
            cm_free(entry);
            entry = next;
        }
    }
    
    cm_free(map->buckets);
    cm_free(map);
}

size_t cm_map_size(cm_map_t* map) {
    return map ? (size_t)map->size : 0;
}

/* Utility Implementation */
void cm_random_seed(unsigned int seed) {
    srand(seed);
}

void cm_random_string(char* buffer, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t charset_size = sizeof(charset) - 1;
    
    for (size_t i = 0; i < length - 1; i++) {
        buffer[i] = charset[rand() % charset_size];
    }
    buffer[length - 1] = '\0';
}

/* ============================================================================
 * INITIALIZATION
 * ============================================================================ */
__attribute__((constructor)) void cm_init_all(void) {
    cm_gc_init();
    cm_random_seed((unsigned int)time(NULL));
    printf("\n🔷 [CM] Library v%s initialized by %s\n", CM_VERSION, CM_AUTHOR);
}

__attribute__((destructor)) void cm_cleanup_all(void) {
    cm_gc_collect();
    if (cm_gc.total_objects > 0) {
        printf("\n⚠️ [CM] Warning: %zu objects still alive\n", cm_gc.total_objects);
        cm_gc_stats();
    } else {
        printf("\n✅ [CM] Clean shutdown - no memory leaks\n");
    }
}

#endif /* CM_FULL_H */