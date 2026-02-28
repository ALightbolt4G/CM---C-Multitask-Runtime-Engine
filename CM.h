/*
 * ============================================================================
 * CM.h - C Multitask Intelligent Library (مصحح بالكامل)
 * Header File - مع ترتيب صحيح للتعريفات
 * ============================================================================
 */

#ifndef CM_H
#define CM_H

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

/* ============================================================================
 * CONFIGURATION
 * ============================================================================ */
#define CM_VERSION "4.2.2"
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
#define CM_ERROR_TEST                   20

/* ============================================================================
 * FORWARD DECLARATIONS
 * ============================================================================ */
struct CMObject;
struct CMArena;
struct cm_string;
struct cm_array;
struct cm_map_entry;
struct cm_map;
struct String;
struct Array;
struct Map;

/* ============================================================================
 * TYPE DEFINITIONS (typedefs first)
 * ============================================================================ */
typedef struct CMObject CMObject;
typedef struct CMArena CMArena;
typedef struct cm_string cm_string_t;
typedef struct cm_array cm_array_t;
typedef struct cm_map_entry cm_map_entry_t;
typedef struct cm_map cm_map_t;
typedef struct String String;
typedef struct Array Array;
typedef struct Map Map;

/* ============================================================================
 * STRUCTURE DEFINITIONS - بالترتيب الصحيح
 * ============================================================================ */

// 1. GC Object Structure
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

// 2. Arena Structure
struct CMArena {
    void* block;
    size_t block_size;
    size_t offset;
    struct CMArena* next;
    const char* name;
    size_t peak_usage;
};

// 3. String Structure
struct cm_string {
    char* data;
    size_t length;
    size_t capacity;
    int ref_count;
    uint32_t hash;
    time_t created;
    int flags;
};

// 4. Array Structure
struct cm_array {
    void* data;
    size_t element_size;
    size_t length;
    size_t capacity;
    int* ref_counts;
    void (*element_destructor)(void*);
    int flags;
};

// 5. Map Entry Structure (يجب تعريفه قبل cm_map)
struct cm_map_entry {
    char* key;
    void* value;
    size_t value_size;
    uint32_t hash;
    struct cm_map_entry* next;
};

// 6. Map Structure (بعد تعريف cm_map_entry)
struct cm_map {
    struct cm_map_entry** buckets;  // استخدام struct بدلاً من typedef
    int bucket_count;
    int size;
    float load_factor;
    int growth_factor;
};

// 7. OOP String Class
struct String {
    char* data;
    int length;
    int capacity;
    struct String* (*concat)(struct String* self, const char* other);
    struct String* (*upper)(struct String* self);
    struct String* (*lower)(struct String* self);
    void (*print)(struct String* self);
    int (*length_func)(struct String* self);
    char (*charAt)(struct String* self, int index);
};

// 8. OOP Array Class
struct Array {
    void* data;
    int element_size;
    int length;
    int capacity;
    struct Array* (*push)(struct Array* self, void* value);
    void* (*pop)(struct Array* self);
    void* (*get)(struct Array* self, int index);
    int (*size)(struct Array* self);
};

// 9. OOP Map Class
struct Map {
    void* map_data;
    int size;
    struct Map* (*set)(struct Map* self, const char* key, void* value);
    void* (*get)(struct Map* self, const char* key);
    int (*has)(struct Map* self, const char* key);
    int (*size_func)(struct Map* self);
};

/* ============================================================================
 * MACROS
 * ============================================================================ */
#define CM_WITH_ARENA(size) \
    for (CMArena* _a __attribute__((cleanup(cm_arena_cleanup))) = cm_arena_create(size); \
         _a; _a = NULL) \
        for (int _i = (cm_arena_push(_a), 0); _i < 1; _i++)

// في CM.h
extern __thread jmp_buf* cm_exception_buffer;  // Thread-local storage

#define CM_TRY() \
    jmp_buf __cm_buf; \
    jmp_buf* __old = cm_exception_buffer; \
    cm_exception_buffer = &__cm_buf; \
    if (setjmp(__cm_buf) == 0)

#define CM_CATCH() \
    else { cm_exception_buffer = __old; }

#define CM_THROW(error, message) \
    do { \
        cm_last_error = error; \
        strncpy(cm_error_message, message, sizeof(cm_error_message) - 1); \
        if (cm_exception_buffer) { \
            longjmp(*cm_exception_buffer, error); \
        } else { \
            fprintf(stderr, "FATAL: Uncaught exception: %s\n", message); \
            exit(error); \
        } \
    } while(0)

#define CM_ABOUT() \
    do { \
        printf("\n"); \
        printf("_________________________________________________________\n"); \
        printf("                                                     \n"); \
        printf("        C MULTITASK INTELLIGENT LIBRARY             \n"); \
        printf("                 by Adham Hossam                     \n"); \
        printf("                                                     \n"); \
        printf("--------------------------------------------------------\n"); \
        printf("  Version : %s\n", CM_VERSION); \
        printf("  Author  : %s\n", CM_AUTHOR); \
        printf("_________________________________________________________\n"); \
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
#define CM_RAND_INT(min, max) ((min) + rand() % ((max) - (min) + 1))

/* Debug macros */
#define CM_REPORT() cm_gc_stats()

/* ============================================================================
 * OOP MACROS - الإصدار النهائي
 * ============================================================================ */
#define cmlass(name) \
    typedef struct name name; \
    struct name

#define end_class

#define property(type, name) type name;

// للدوال بدون parameters
#define method0(return_type, name) \
    return_type (*name)(void* self)

// للدوال مع parameters
#define method(return_type, name, ...) \
    return_type (*name)(void* self, __VA_ARGS__)

// ماكرو لنداء الدوال - بيشتغل مع وبدون parameters
#define send(obj, method, ...) \
    ((obj)->method ? (obj)->method(obj, ##__VA_ARGS__) : (void)0)

/* ============================================================================
 * FUNCTION DECLARATIONS
 * ============================================================================ */

/* GC Functions */
void cm_gc_init(void);
void cm_gc_collect(void);
void cm_gc_stats(void);
void* cm_alloc(size_t size, const char* type, const char* file, int line);
void cm_free(void* ptr);
void cm_retain(void* ptr);

/* Arena Functions */
CMArena* cm_arena_create(size_t size);
void cm_arena_destroy(CMArena* arena);
void cm_arena_push(CMArena* arena);
void cm_arena_pop(void);
// في CM.h - أضف هذا السطر مع الدوال التانية
void cm_arena_cleanup(void* ptr);


/* String Functions */
cm_string_t* cm_string_new(const char* initial);
void cm_string_free(cm_string_t* s);
size_t cm_string_length(cm_string_t* s);
cm_string_t* cm_string_format(const char* format, ...);
void cm_string_set(cm_string_t* s, const char* value);
void cm_string_upper(cm_string_t* s);
void cm_string_lower(cm_string_t* s);
String* cm_input(const char* prompt);

/* Array Functions */
cm_array_t* cm_array_new(size_t element_size, size_t initial_capacity);
void cm_array_free(cm_array_t* arr);
void* cm_array_get(cm_array_t* arr, size_t index);
void cm_array_push(cm_array_t* arr, const void* value);
void* cm_array_pop(cm_array_t* arr);
size_t cm_array_length(cm_array_t* arr);

/* Map Functions */
cm_map_t* cm_map_new(void);
void cm_map_free(cm_map_t* map);
void cm_map_set(cm_map_t* map, const char* key, const void* value, size_t value_size);
void* cm_map_get(cm_map_t* map, const char* key);
int cm_map_has(cm_map_t* map, const char* key);
size_t cm_map_size(cm_map_t* map);

/* Utility Functions */
void cm_random_seed(unsigned int seed);
void cm_random_string(char* buffer, size_t length);

/* OOP Class Functions */
String* String_new(const char* initial);
void String_delete(String* self);
String* string_concat(String* self, const char* other);
String* string_upper(String* self);
String* string_lower(String* self);
void string_print(String* self);
int string_length(String* self);
char string_charAt(String* self, int index);

Array* Array_new(int element_size, int capacity);
void Array_delete(Array* self);
Array* array_push(Array* self, void* value);
void* array_pop(Array* self);
void* array_get(Array* self, int index);
int array_size(Array* self);

Map* Map_new(void);
void Map_delete(Map* self);
Map* map_set(Map* self, const char* key, void* value);
void* map_get(Map* self, const char* key);
int map_has(Map* self, const char* key);
int map_size_func(Map* self);

/* Error Handling Functions */
const char* cm_error_string(int error);
const char* cm_error_get_message(void);
int cm_error_get_last(void);
void cm_error_clear(void);
void cm_error_set(int error, const char* message);
/* ============================================================================
 * SAFE I/O FUNCTIONS - للاستخدام العام
 * ============================================================================ */
void cm_printf(const char* format, ...);
void cm_error(const char* format, ...);
char* cm_gets(char* buffer, size_t size);


/* Short Macros */
#define cmAlloc(sz) cm_alloc(sz, "object", __FILE__, __LINE__)
#define cmFree(ptr) cm_free(ptr)
#define cmRetain(ptr) cm_retain(ptr)
#define cmGC() cm_gc_collect()
#define cmStats() cm_gc_stats()

#define cmStr(s) cm_string_new(s)
#define cmStrFree(s) cm_string_free(s)
#define cmStrLen(s) cm_string_length(s)
#define cmStrFmt(...) cm_string_format(__VA_ARGS__)
#define cmStrUpper(s) cm_string_upper(s)
#define cmStrLower(s) cm_string_lower(s)

#define cmArr(type, sz) cm_array_new(sizeof(type), sz)
#define cmArrFree(a) cm_array_free(a)
#define cmArrLen(a) cm_array_length(a)
#define cmArrGet(a, i, type) (*(type*)cm_array_get(a, i))
#define cmArrPush(a, type, v) do { type __tmp = (v); cm_array_push(a, &__tmp); } while(0)
#define cmArrPop(a) cm_array_pop(a)

#define cmMap() cm_map_new()
#define cmMapFree(m) cm_map_free(m)
#define cmMapSetInt(m, k, v) do { int __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(int)); } while(0)
#define cmMapSetStr(m, k, v) do { const char* __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(const char*)); } while(0)
#define cmMapGetInt(m, k) (*(int*)cm_map_get(m, k))
#define cmMapGetStr(m, k) (*(char**)cm_map_get(m, k))
#define cmMapHas(m, k) cm_map_has(m, k)

#define cmTry CM_TRY()
#define cmCatch CM_CATCH()
#define cmThrow(e, m) CM_THROW(e, m)
#define cmErrorMsg() cm_error_get_message()
#define cmErrorCode() cm_error_get_last()

#define cmRandInt(min, max) ((min) + rand() % ((max) - (min) + 1))
#define cmRandStr(buf, len) cm_random_string(buf, len)

/* ============================================================================
 * GLOBAL VARIABLES DECLARATIONS
 * ============================================================================ */
extern int cm_last_error;
extern char cm_error_message[1024];

#endif /* CM_H */

