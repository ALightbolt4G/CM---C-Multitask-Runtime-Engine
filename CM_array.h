#ifndef CM_ARRAY_H
#define CM_ARRAY_H

#include "CM_config.h"
#include "CM_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * CM_array.h - Smart Array System
 * Author: Adham Hossam
 * Version: 3.0
 */

/* ===== Array Type ===== */
typedef struct {
    void* data;                  /* Array data */
    size_t element_size;         /* Size of each element */
    size_t length;               /* Current number of elements */
    size_t capacity;             /* Allocated capacity */
    int* ref_counts;             /* Reference counts per element */
    void (*element_destructor)(void*);  /* Element destructor */
} cm_array_t;

/* ===== Function Declarations ===== */
cm_array_t* cm_array_new(size_t element_size, size_t initial_capacity);
void cm_array_free(cm_array_t* arr);
void* cm_array_get(cm_array_t* arr, size_t index);
void cm_array_set(cm_array_t* arr, size_t index, const void* value);
void cm_array_push(cm_array_t* arr, const void* value);
void* cm_array_pop(cm_array_t* arr);
void cm_array_insert(cm_array_t* arr, size_t index, const void* value);
void cm_array_remove(cm_array_t* arr, size_t index);
void cm_array_clear(cm_array_t* arr);
void cm_array_resize(cm_array_t* arr, size_t new_capacity);
void cm_array_sort(cm_array_t* arr, int (*compar)(const void*, const void*));
int cm_array_find(cm_array_t* arr, const void* value, int (*compar)(const void*, const void*));
void cm_array_set_destructor(cm_array_t* arr, void (*destructor)(void*));

/* ===== CORRECTED MACROS (take 3 arguments) ===== */
#define CM_ARR(type, size) cm_array_new(sizeof(type), size)
#define CM_ARR_FREE(a) cm_array_free(a)
#define CM_ARR_LEN(a) ((a)->length)
#define CM_ARR_CAP(a) ((a)->capacity)
#define CM_ARR_GET(a, i, type) (*(type*)cm_array_get(a, i))
#define CM_ARR_SET(a, i, type, v) do { type __tmp = (v); cm_array_set(a, i, &__tmp); } while(0)
#define CM_ARR_PUSH(a, type, v) do { type __tmp = (v); cm_array_push(a, &__tmp); } while(0)
#define CM_ARR_POP(a) cm_array_pop(a)
#define CM_ARR_INSERT(a, i, type, v) do { type __tmp = (v); cm_array_insert(a, i, &__tmp); } while(0)
#define CM_ARR_REMOVE(a, i) cm_array_remove(a, i)
#define CM_ARR_CLEAR(a) cm_array_clear(a)
#define CM_ARR_SORT(a, cmp) cm_array_sort(a, cmp)

/* ===== Implementation ===== */
#ifdef CM_IMPLEMENTATION_ARRAY

/* Create new array */
cm_array_t* cm_array_new(size_t element_size, size_t initial_capacity) {
    cm_array_t* arr = (cm_array_t*)cm_alloc(sizeof(cm_array_t), "array", __FILE__, __LINE__);
    if (!arr) return NULL;
    
    arr->element_size = element_size;
    arr->capacity = initial_capacity > 0 ? initial_capacity : 16;
    arr->length = 0;
    arr->data = cm_alloc(element_size * arr->capacity, "array_data", __FILE__, __LINE__);
    arr->ref_counts = (int*)cm_alloc(sizeof(int) * arr->capacity, "array_refs", __FILE__, __LINE__);
    arr->element_destructor = NULL;
    
    if (arr->ref_counts) {
        memset(arr->ref_counts, 0, sizeof(int) * arr->capacity);
    }
    
    return arr;
}

/* Free array */
void cm_array_free(cm_array_t* arr) {
    if (!arr) return;
    
    if (arr->element_destructor) {
        for (size_t i = 0; i < arr->length; i++) {
            void* elem = (char*)arr->data + (i * arr->element_size);
            arr->element_destructor(elem);
        }
    }
    
    if (arr->data) cm_free(arr->data);
    if (arr->ref_counts) cm_free(arr->ref_counts);
    cm_free(arr);
}

/* Resize array */
void cm_array_resize(cm_array_t* arr, size_t new_capacity) {
    if (!arr || new_capacity <= arr->capacity) return;
    
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

/* Get element */
void* cm_array_get(cm_array_t* arr, size_t index) {
    if (!arr || index >= arr->length) return NULL;
    
    arr->ref_counts[index]++;
    return (char*)arr->data + (index * arr->element_size);
}

/* Set element */
void cm_array_set(cm_array_t* arr, size_t index, const void* value) {
    if (!arr || !value) return;
    
    if (index >= arr->capacity) {
        cm_array_resize(arr, arr->capacity * 2);
    }
    
    void* dest = (char*)arr->data + (index * arr->element_size);
    memcpy(dest, value, arr->element_size);
    
    if (index >= arr->length) {
        arr->length = index + 1;
    }
}

/* Push element */
void cm_array_push(cm_array_t* arr, const void* value) {
    if (!arr || !value) return;
    
    if (arr->length >= arr->capacity) {
        cm_array_resize(arr, arr->capacity * 2);
    }
    
    void* dest = (char*)arr->data + (arr->length * arr->element_size);
    memcpy(dest, value, arr->element_size);
    arr->length++;
}

/* Pop element */
void* cm_array_pop(cm_array_t* arr) {
    if (!arr || arr->length == 0) return NULL;
    
    arr->length--;
    return (char*)arr->data + (arr->length * arr->element_size);
}

/* Insert element */
void cm_array_insert(cm_array_t* arr, size_t index, const void* value) {
    if (!arr || !value || index > arr->length) return;
    
    if (arr->length >= arr->capacity) {
        cm_array_resize(arr, arr->capacity * 2);
    }
    
    void* dest = (char*)arr->data + (index * arr->element_size);
    void* src = (char*)arr->data + (index * arr->element_size);
    size_t elements_to_move = arr->length - index;
    
    memmove((char*)dest + arr->element_size, src, elements_to_move * arr->element_size);
    memcpy(dest, value, arr->element_size);
    arr->length++;
}

/* Remove element */
void cm_array_remove(cm_array_t* arr, size_t index) {
    if (!arr || index >= arr->length) return;
    
    void* dest = (char*)arr->data + (index * arr->element_size);
    void* src = (char*)arr->data + ((index + 1) * arr->element_size);
    size_t elements_to_move = arr->length - index - 1;
    
    memmove(dest, src, elements_to_move * arr->element_size);
    arr->length--;
}

/* Clear array */
void cm_array_clear(cm_array_t* arr) {
    if (!arr) return;
    arr->length = 0;
}

/* Sort array */
void cm_array_sort(cm_array_t* arr, int (*compar)(const void*, const void*)) {
    if (!arr || !compar || arr->length <= 1) return;
    
    qsort(arr->data, arr->length, arr->element_size, compar);
}

/* Find element */
int cm_array_find(cm_array_t* arr, const void* value, int (*compar)(const void*, const void*)) {
    if (!arr || !value) return -1;
    
    for (size_t i = 0; i < arr->length; i++) {
        void* elem = (char*)arr->data + (i * arr->element_size);
        if (compar(elem, value) == 0) {
            return (int)i;
        }
    }
    
    return -1;
}

/* Set element destructor */
void cm_array_set_destructor(cm_array_t* arr, void (*destructor)(void*)) {
    if (!arr) return;
    arr->element_destructor = destructor;
}

#endif /* CM_IMPLEMENTATION_ARRAY */
#endif /* CM_ARRAY_H */