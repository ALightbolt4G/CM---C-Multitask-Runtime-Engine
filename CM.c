/*
 * ============================================================================
 * CM.c - C Multitask Intelligent Library
 * Implementation File
 * Author: Adham Hossam
 * Version: 4.2.2
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
#include <pthread.h>
#include "CM.h"

/* ============================================================================
 * INTERNAL STRUCTURES (المعرفة محلياً فقط)
 * ============================================================================ */
// هذا الـ struct غير موجود في CM.h所以要定义在这里
typedef struct CMObject CMObject;  // تأكد من تعريف CMObject

typedef struct {
    CMObject* head;
    CMObject* tail;
    size_t total_memory;
    size_t gc_last_collection;
    pthread_mutex_t gc_lock;
    CMArena* current_arena;
    pthread_mutex_t arena_lock;

    size_t peak_memory;
    size_t allocations;
    size_t frees;
    size_t collections;
    double avg_collection_time;
    size_t total_objects;
} CMMemorySystem;

// المتغير العام الوحيد
static CMMemorySystem cm_mem = {0};

// المتغيرات العامة الأخرى (غير static لأنها extern في CM.h)
// CM.c - السطر 64
__thread jmp_buf* cm_exception_buffer = NULL;  // ✅ thread-local
int cm_last_error = 0;
char cm_error_message[1024] = {0};
/* ============================================================================
 * IMPLEMENTATION - كل الدوال كما هي من CM_full.h
 * ============================================================================ */

/* GC Implementation */
void cm_gc_init(void) {
    memset(&cm_mem, 0, sizeof(CMMemorySystem)); 
    pthread_mutex_init(&cm_mem.gc_lock, NULL);
    pthread_mutex_init(&cm_mem.arena_lock, NULL);
}

CMArena* cm_arena_create(size_t size) {
    CMArena* arena = (CMArena*)malloc(sizeof(CMArena));
    if (!arena) return NULL;
    arena->block = malloc(size);
    if (!arena->block) { free(arena); return NULL; }
    arena->block_size = size;
    arena->offset = 0;
    arena->name = "dynamic_arena";
    arena->next = NULL;
    arena->peak_usage = 0;
    return arena;
}

void cm_arena_destroy(CMArena* arena) {
    if (!arena) return;
    if (arena->block) free(arena->block);
    free(arena);
}

void cm_arena_push(CMArena* arena) {
    pthread_mutex_lock(&cm_mem.arena_lock);
    cm_mem.current_arena = arena;
    pthread_mutex_unlock(&cm_mem.arena_lock);
}

void cm_arena_pop(void) {
    pthread_mutex_lock(&cm_mem.arena_lock);
    cm_mem.current_arena = NULL;
    pthread_mutex_unlock(&cm_mem.arena_lock);
}

void* cm_alloc(size_t size, const char* type, const char* file, int line) {
    if (size == 0) return NULL;

    /* 🚀 1. Fast Path: Check Arena allocation system for maximum performance */
    if (cm_mem.current_arena) {
        /* Align memory to 8 bytes for CPU efficiency and to prevent alignment faults */
        size_t aligned_size = (size + 7) & ~7;

        if (cm_mem.current_arena->offset + aligned_size <= cm_mem.current_arena->block_size) {
            void* ptr = (char*)cm_mem.current_arena->block + cm_mem.current_arena->offset;
            cm_mem.current_arena->offset += aligned_size;

            /* Update Arena usage statistics */
            if (cm_mem.current_arena->offset > cm_mem.current_arena->peak_usage) {
                cm_mem.current_arena->peak_usage = cm_mem.current_arena->offset;
            }

            /* ✅ IMPORTANT: Arena objects are not tracked by GC to eliminate overhead */
            return ptr; /* Immediate return for maximum speed */
        }

        /* Fallback mechanism if the current arena is exhausted */
        fprintf(stderr, "[ARENA] Warning: Arena '%s' full, falling back to GC\n", 
                cm_mem.current_arena->name);
    }
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

    pthread_mutex_lock(&cm_mem.gc_lock);

    if (cm_mem.tail) {
        cm_mem.tail->next = obj;
        obj->prev = cm_mem.tail;
        cm_mem.tail = obj;
    } else {
        cm_mem.head = cm_mem.tail = obj;
    }

    cm_mem.total_objects++;
    cm_mem.total_memory += size;
    cm_mem.allocations++;

    if (cm_mem.total_memory > cm_mem.peak_memory) {
        cm_mem.peak_memory = cm_mem.total_memory;
    }

    pthread_mutex_unlock(&cm_mem.gc_lock);

    return ptr;
}

void cm_free(void* ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&cm_mem.gc_lock);

    for (CMObject* obj = cm_mem.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            obj->ref_count--;

            if (obj->ref_count <= 0) {
                if (obj->destructor) {
                    obj->destructor(ptr);
                }

                free(ptr);

                // ✅ تحديث الـ linked list
                if (obj->prev) {
                    obj->prev->next = obj->next;
                } else {
                    cm_mem.head = obj->next;
                }

                if (obj->next) {
                    obj->next->prev = obj->prev;
                } else {
                    cm_mem.tail = obj->prev;
                }

                // ✅ تحديث الإحصائيات
                cm_mem.total_objects--;
                cm_mem.total_memory -= obj->size;
                cm_mem.frees++;

                free(obj);
            }

            pthread_mutex_unlock(&cm_mem.gc_lock);
            return;
        }
    }

    // ✅ من Arena = اتجاهله
    pthread_mutex_unlock(&cm_mem.gc_lock);
}

void cm_gc_collect(void) {
    pthread_mutex_lock(&cm_mem.gc_lock);

    printf("[GC] Starting collection...\n");

    for (CMObject* obj = cm_mem.head; obj; obj = obj->next) {
        obj->marked = (obj->ref_count > 0) ? 1 : 0;
    }

    CMObject* current = cm_mem.head;
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
                cm_mem.head = current->next;
            }

            if (current->next) {
                current->next->prev = current->prev;
            } else {
                cm_mem.tail = current->prev;
            }

            cm_mem.total_objects--;
            cm_mem.total_memory -= current->size;
            cm_mem.frees++;

            free(current);
        }

        current = next;
    }
    cm_mem.gc_last_collection = freed_memory;
    cm_mem.collections++;

    printf("[GC] Completed: freed %d objects (%zu bytes)\n", 
           freed_objects, freed_memory);

    pthread_mutex_unlock(&cm_mem.gc_lock);
}

void cm_gc_stats(void) {
    pthread_mutex_lock(&cm_mem.gc_lock);

    printf("\n");
    printf("══════════════════════════════════════════════════════════════\n");
    printf("              GARBAGE COLLECTOR STATISTICS\n");
    printf("──────────────────────────────────────────────────────────────\n");
    printf("  Total objects    │ %20zu\n", cm_mem.total_objects);
    printf("  Total memory     │ %20zu bytes\n", cm_mem.total_memory);
    printf("  Peak memory      │ %20zu bytes\n", cm_mem.peak_memory);
    printf("  Allocations      │ %20zu\n", cm_mem.allocations);
    printf("  Frees            │ %20zu\n", cm_mem.frees);
    printf("  Collections      │ %20zu\n", cm_mem.collections);
    printf("──────────────────────────────────────────────────────────────\n");
    printf("  Avg collection   │ %19.3f ms\n", cm_mem.avg_collection_time * 1000);
    printf("  Last freed       │ %20zu bytes\n", cm_mem.gc_last_collection);

    // ✅ عرض معلومات Arena لو موجودة
    if (cm_mem.current_arena) {
        printf("──────────────────────────────────────────────────────────────\n");
        printf("  ARENA STATISTICS\n");
        printf("  Arena name       │ %20s\n", cm_mem.current_arena->name);
        printf("  Arena size       │ %20zu bytes\n", cm_mem.current_arena->block_size);
        printf("  Arena used       │ %20zu bytes\n", cm_mem.current_arena->offset);
        printf("  Arena peak       │ %20zu bytes\n", cm_mem.current_arena->peak_usage);
    }

    printf("══════════════════════════════════════════════════════════════\n");

    if (cm_mem.total_objects > 0 && CM_LOG_LEVEL >= 3) {
        printf("\nACTIVE OBJECTS:\n");
        printf("──────────────────────────────────────────────────────────────\n");
        int i = 0;
        for (CMObject* obj = cm_mem.head; obj; obj = obj->next) {
            printf("  [%d] %s (%zu bytes) at %s:%d [refs: %d]\n",
                   ++i,
                   obj->type ? obj->type : "unknown",
                   obj->size,
                   obj->file ? obj->file : "unknown",
                   obj->line,
                   obj->ref_count);
        }
    }

    pthread_mutex_unlock(&cm_mem.gc_lock);
}

void cm_retain(void* ptr) {
    if (!ptr) return;

    pthread_mutex_lock(&cm_mem.gc_lock);

    for (CMObject* obj = cm_mem.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            obj->ref_count++;
            break;
        }
    }

    pthread_mutex_unlock(&cm_mem.gc_lock);
}


// في ملف CM.c - أضف هذه الدالة
void cm_arena_cleanup(void* ptr) {
    CMArena** arena_ptr = (CMArena**)ptr;
    if (*arena_ptr) {
        // POP الأول
        pthread_mutex_lock(&cm_mem.arena_lock);
        cm_mem.current_arena = NULL;
        pthread_mutex_unlock(&cm_mem.arena_lock);
        
        // بعد كده Destroy
        cm_arena_destroy(*arena_ptr);
        *arena_ptr = NULL;
        
        printf("[ARENA] Cleanup auto-destroyed arena\n"); // لل debugging
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

/* ============================================================================
 * STRING IMPLEMENTATION
 * ============================================================================ */
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

/* ============================================================================
 * ARRAY IMPLEMENTATION
 * ============================================================================ */
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

void* cm_array_pop(cm_array_t* arr) {
    if (!arr || arr->length == 0) return NULL;

    arr->length--;
    return (char*)arr->data + (arr->length * arr->element_size);
}

size_t cm_array_length(cm_array_t* arr) {
    return arr ? arr->length : 0;
}

/* ============================================================================
 * MAP IMPLEMENTATION
 * ============================================================================ */
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

/* ============================================================================
 * UTILITY IMPLEMENTATION
 * ============================================================================ */
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
 * CLASS IMPLEMENTATION - OOP
 * ============================================================================ */

// ===== String Class Implementation =====
String* string_concat(String* this, const char* other) {
    if (!this || !other) return this;
    int new_len = this->length + strlen(other);
    char* new_data = (char*)cm_alloc(new_len + 1, "string_data", __FILE__, __LINE__);
    strcpy(new_data, this->data);
    strcat(new_data, other);
    cm_free(this->data);
    this->data = new_data;
    this->length = new_len;
    return this;
}

String* string_upper(String* this) {
    if (!this || !this->data) return this;
    for (int i = 0; i < this->length; i++) {
        this->data[i] = toupper(this->data[i]);
    }
    return this;
}

String* string_lower(String* this) {
    if (!this || !this->data) return this;
    for (int i = 0; i < this->length; i++) {
        this->data[i] = tolower(this->data[i]);
    }
    return this;
}

void string_print(String* this) {
    if (this && this->data) printf("%s", this->data);
}

int string_length(String* this) {
    return this ? this->length : 0;
}

char string_charAt(String* this, int index) {
    if (!this || !this->data || index < 0 || index >= this->length) return '\0';
    return this->data[index];
}

String* String_new(const char* initial) {
    String* self = (String*)cm_alloc(sizeof(String), "String", __FILE__, __LINE__);

    int len = initial ? strlen(initial) : 0;
    self->length = len;
    self->capacity = len + 1;
    self->data = (char*)cm_alloc(self->capacity, "string_data", __FILE__, __LINE__);
    if (self->data && initial) {
        strcpy(self->data, initial);
    }

    self->concat = string_concat;
    self->upper = string_upper;
    self->lower = string_lower;
    self->print = string_print;
    self->length_func = string_length;
    self->charAt = string_charAt;

    return self;
}

void String_delete(String* self) {
    if (!self) return;
    if (self->data) cm_free(self->data);
    cm_free(self);
}

// ===== Array Class Implementation =====
Array* array_push(Array* this, void* value) {
    if (!this || !value) return this;

    if (this->length >= this->capacity) {
        int new_cap = this->capacity * 2;
        void* new_data = cm_alloc(this->element_size * new_cap, "array_data", __FILE__, __LINE__);
        memcpy(new_data, this->data, this->element_size * this->length);
        cm_free(this->data);
        this->data = new_data;
        this->capacity = new_cap;
    }

    void* dest = (char*)this->data + (this->length * this->element_size);
    memcpy(dest, value, this->element_size);
    this->length++;
    return this;
}

void* array_pop(Array* this) {
    if (!this || this->length == 0) return NULL;
    this->length--;
    return (char*)this->data + (this->length * this->element_size);
}

void* array_get(Array* this, int index) {
    if (!this || index < 0 || index >= this->length) return NULL;
    return (char*)this->data + (index * this->element_size);
}

int array_size(Array* this) {
    return this ? this->length : 0;
}

Array* Array_new(int element_size, int capacity) {
    Array* self = (Array*)cm_alloc(sizeof(Array), "Array", __FILE__, __LINE__);

    self->element_size = element_size;
    self->capacity = capacity > 0 ? capacity : 16;
    self->length = 0;
    self->data = cm_alloc(element_size * self->capacity, "array_data", __FILE__, __LINE__);

    self->push = array_push;
    self->pop = array_pop;
    self->get = array_get;
    self->size = array_size;

    return self;
}

void Array_delete(Array* self) {
    if (!self) return;
    if (self->data) cm_free(self->data);
    cm_free(self);
}

// ===== Map Class Implementation =====
Map* map_set(Map* this, const char* key, void* value) {
    if (!this || !key || !value) return this;
    cm_map_set((cm_map_t*)this->map_data, key, value, sizeof(void*));
    this->size = cm_map_size((cm_map_t*)this->map_data);
    return this;
}

void* map_get(Map* this, const char* key) {
    if (!this || !key) return NULL;
    return cm_map_get((cm_map_t*)this->map_data, key);
}

int map_has(Map* this, const char* key) {
    if (!this || !key) return 0;
    return cm_map_has((cm_map_t*)this->map_data, key);
}

int map_size_func(Map* this) {
    return this ? this->size : 0;
}

Map* Map_new(void) {
    Map* self = (Map*)cm_alloc(sizeof(Map), "Map", __FILE__, __LINE__);

    self->map_data = cm_map_new();
    self->size = 0;

    self->set = map_set;
    self->get = map_get;
    self->has = map_has;
    self->size_func = map_size_func;

    return self;
}

void Map_delete(Map* self) {
    if (!self) return;
    if (self->map_data) cm_map_free((cm_map_t*)self->map_data);
    cm_free(self);
}

/* ============================================================================
 * INPUT IMPLEMENTATION
 * ============================================================================ */
String* cm_input(const char* prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }

    char buffer[1024]; 
    if (fgets(buffer, sizeof(buffer), stdin)) {
        buffer[strcspn(buffer, "\n")] = 0;
        return String_new(buffer);
    }
    return String_new("");
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
    pthread_mutex_lock(&cm_mem.gc_lock);
    for (CMObject* obj = cm_mem.head; obj; obj = obj->next) {
        obj->ref_count = 0; 
    }
    pthread_mutex_unlock(&cm_mem.gc_lock);

    cm_gc_collect();

    if (cm_mem.total_objects > 0) {
        printf("\n⚠️ [CM] Warning: %zu objects still alive\n", cm_mem.total_objects);
        cm_gc_stats();
    } else {
        printf("\n✅ [CM] Clean shutdown - all memory recovered!\n");
    }

    pthread_mutex_destroy(&cm_mem.gc_lock);
    pthread_mutex_destroy(&cm_mem.arena_lock);
}
