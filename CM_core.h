#ifndef CM_CORE_H
#define CM_CORE_H

#include "CM_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * CM_core.h - Core Memory Management System
 * Author: Adham Hossam
 * 
 * Responsibilities:
 * - Garbage Collection
 * - Memory Allocation/Deallocation
 * - Object Tracking
 * - Memory Statistics
 */

/* ===== Core Types ===== */
typedef struct CMObject {
    void* ptr;                    /* Pointer to allocated memory */
    size_t size;                   /* Size in bytes */
    const char* type;              /* Type name for debugging */
    const char* file;              /* File where allocated */
    int line;                       /* Line where allocated */
    time_t alloc_time;              /* Allocation timestamp */
    int ref_count;                  /* Reference count */
    int marked;                     /* Mark for GC */
    struct CMObject* next;          /* Next in linked list */
    struct CMObject* prev;          /* Previous in linked list */
    void (*destructor)(void*);      /* Custom destructor */
} CMObject;

typedef struct {
    CMObject* head;                 /* First object */
    CMObject* tail;                 /* Last object */
    size_t total_objects;           /* Current object count */
    size_t total_memory;             /* Current memory usage */
    size_t peak_memory;              /* Peak memory usage */
    size_t collections;              /* Number of GC collections */
    size_t allocations;              /* Total allocations */
    size_t frees;                    /* Total frees */
    double avg_collection_time;       /* Average GC time */
    double total_collection_time;     /* Total GC time */
    int gc_enabled;                   /* Is GC enabled? */
    int gc_threshold;                  /* Memory threshold */
    CM_MUTEX_T lock;                    /* Thread safety lock */
} CMGC;

/* Global GC instance */
static CMGC cm_gc = {0};

/* ===== Function Declarations ===== */
void cm_gc_init(void);
void cm_gc_cleanup(void);
void* cm_alloc(size_t size, const char* type, const char* file, int line);
void cm_free(void* ptr);
void cm_retain(void* ptr);
void cm_release(void* ptr);
void cm_gc_collect(void);
void cm_gc_stats(void);
void cm_set_destructor(void* ptr, void (*destructor)(void*));
const char* cm_get_type(void* ptr);
size_t cm_get_size(void* ptr);

/* ===== Implementation ===== */
#ifdef CM_IMPLEMENTATION_CORE

/* Initialize Garbage Collector */
void cm_gc_init(void) {
    memset(&cm_gc, 0, sizeof(CMGC));
    cm_gc.gc_enabled = 1;
    cm_gc.gc_threshold = CM_GC_THRESHOLD;
    
    #ifdef CM_ENABLE_THREAD_SAFETY
        CM_MUTEX_INIT(&cm_gc.lock);
    #endif
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 2) {
        printf("[CORE] GC initialized with threshold: %d bytes\n", cm_gc.gc_threshold);
    }
}

/* Cleanup Garbage Collector */
void cm_gc_cleanup(void) {
    cm_gc_collect();
    
    #ifdef CM_ENABLE_THREAD_SAFETY
        CM_MUTEX_DESTROY(&cm_gc.lock);
    #endif
}

/* Allocate memory with tracking */
void* cm_alloc(size_t size, const char* type, const char* file, int line) {
    if (size == 0) return NULL;
    
    CM_LOCK(&cm_gc.lock);
    
    void* ptr = malloc(size);
    if (!ptr) {
        CM_UNLOCK(&cm_gc.lock);
        return NULL;
    }
    
    CMObject* obj = (CMObject*)malloc(sizeof(CMObject));
    if (!obj) {
        free(ptr);
        CM_UNLOCK(&cm_gc.lock);
        return NULL;
    }
    
    obj->ptr = ptr;
    obj->size = size;
    obj->type = type;
    obj->file = file;
    obj->line = line;
    obj->alloc_time = time(NULL);
    obj->ref_count = 1;
    obj->marked = 0;
    obj->next = NULL;
    obj->prev = NULL;
    obj->destructor = NULL;
    
    /* Add to linked list */
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
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
        printf("[CORE] Allocated %zu bytes at %s:%d (type: %s)\n", 
               size, file, line, type ? type : "unknown");
    }
    
    /* Auto GC if threshold exceeded */
    if (cm_gc.gc_enabled && cm_gc.total_memory > cm_gc.gc_threshold) {
        CM_UNLOCK(&cm_gc.lock);
        cm_gc_collect();
    } else {
        CM_UNLOCK(&cm_gc.lock);
    }
    
    return ptr;
}

/* Free memory with tracking */
void cm_free(void* ptr) {
    if (!ptr) return;
    
    CM_LOCK(&cm_gc.lock);
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            obj->ref_count--;
            
            if (obj->ref_count <= 0) {
                /* Call custom destructor if exists */
                if (obj->destructor) {
                    obj->destructor(ptr);
                }
                
                free(ptr);
                
                /* Remove from linked list */
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
                
                if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
                    printf("[CORE] Freed %p (%zu bytes)\n", ptr, obj->size);
                }
                
                free(obj);
            }
            
            CM_UNLOCK(&cm_gc.lock);
            return;
        }
    }
    
    /* Pointer not tracked - free directly */
    free(ptr);
    CM_UNLOCK(&cm_gc.lock);
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 2) {
        printf("[CORE] Warning: Freed untracked pointer %p\n", ptr);
    }
}

/* Increment reference count */
void cm_retain(void* ptr) {
    if (!ptr) return;
    
    CM_LOCK(&cm_gc.lock);
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            obj->ref_count++;
            if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
                printf("[CORE] Retained %p (refs: %d)\n", ptr, obj->ref_count);
            }
            break;
        }
    }
    
    CM_UNLOCK(&cm_gc.lock);
}

/* Decrement reference count */
void cm_release(void* ptr) {
    cm_free(ptr);  /* Same as free */
}

/* Set custom destructor */
void cm_set_destructor(void* ptr, void (*destructor)(void*)) {
    if (!ptr || !destructor) return;
    
    CM_LOCK(&cm_gc.lock);
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            obj->destructor = destructor;
            break;
        }
    }
    
    CM_UNLOCK(&cm_gc.lock);
}

/* Get type of tracked pointer */
const char* cm_get_type(void* ptr) {
    if (!ptr) return NULL;
    
    CM_LOCK(&cm_gc.lock);
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            CM_UNLOCK(&cm_gc.lock);
            return obj->type;
        }
    }
    
    CM_UNLOCK(&cm_gc.lock);
    return NULL;
}

/* Get size of tracked pointer */
size_t cm_get_size(void* ptr) {
    if (!ptr) return 0;
    
    CM_LOCK(&cm_gc.lock);
    
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        if (obj->ptr == ptr) {
            CM_UNLOCK(&cm_gc.lock);
            return obj->size;
        }
    }
    
    CM_UNLOCK(&cm_gc.lock);
    return 0;
}

/* Garbage Collection - Mark & Sweep */
void cm_gc_collect(void) {
    if (!cm_gc.gc_enabled) return;
    
    CM_LOCK(&cm_gc.lock);
    
    clock_t start = clock();
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 2) {
        printf("[CORE] Starting garbage collection #%zu...\n", cm_gc.collections + 1);
    }
    
    /* Mark phase - mark all objects with ref_count > 0 */
    for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
        obj->marked = (obj->ref_count > 0) ? 1 : 0;
    }
    
    /* Sweep phase - remove unmarked objects */
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
            
            /* Remove from list */
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
    
    clock_t end = clock();
    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    cm_gc.total_collection_time += elapsed;
    cm_gc.collections++;
    cm_gc.avg_collection_time = cm_gc.total_collection_time / cm_gc.collections;
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 2) {
        printf("[CORE] GC completed: freed %d objects (%zu bytes) in %.3f ms\n",
               freed_objects, freed_memory, elapsed * 1000);
    }
    
    CM_UNLOCK(&cm_gc.lock);
}

/* Display GC Statistics */
void cm_gc_stats(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                 GARBAGE COLLECTOR STATISTICS              ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    printf("║ Total objects:    %20zu                              ║\n", cm_gc.total_objects);
    printf("║ Total memory:     %20zu bytes                         ║\n", cm_gc.total_memory);
    printf("║ Peak memory:      %20zu bytes                         ║\n", cm_gc.peak_memory);
    printf("║ Allocations:      %20zu                              ║\n", cm_gc.allocations);
    printf("║ Frees:            %20zu                              ║\n", cm_gc.frees);
    printf("║ Collections:      %20zu                              ║\n", cm_gc.collections);
    printf("║ Avg collection:   %19.3f ms                            ║\n", cm_gc.avg_collection_time * 1000);
    printf("╚════════════════════════════════════════════════════════════╝\n");
    
    if (cm_gc.total_objects > 0 && CM_LOG_LEVEL >= 3) {
        printf("\nActive Objects:\n");
        int i = 0;
        for (CMObject* obj = cm_gc.head; obj; obj = obj->next) {
            printf("  [%d] %s at %s:%d (%zu bytes, refs: %d)\n",
                   ++i, obj->type ? obj->type : "unknown",
                   obj->file ? obj->file : "unknown",
                   obj->line, obj->size, obj->ref_count);
        }
    }
}

#endif /* CM_IMPLEMENTATION_CORE */
#endif /* CM_CORE_H */