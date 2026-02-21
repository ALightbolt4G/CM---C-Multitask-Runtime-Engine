#ifndef CM_MAP_H
#define CM_MAP_H

#include "CM_config.h"
#include "CM_core.h"
#include "CM_string.h"
#include "CM_array.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * CM_map.h - Hash Map/Dictionary System
 * Author: Adham Hossam
 */
#define CM_MAP_INITIAL_SIZE 16
#define CM_MAP_LOAD_FACTOR 0.75

typedef struct cm_map_entry {
    char* key;
    void* value;
    size_t value_size;
    uint32_t hash;
    struct cm_map_entry* next;
} cm_map_entry_t;

typedef struct {
    cm_map_entry_t** buckets;
    int bucket_count;
    int size;
    float load_factor;
} cm_map_t;

/* Function declarations */
cm_map_t* cm_map_new(void);
void cm_map_free(cm_map_t* map);
void cm_map_set(cm_map_t* map, const char* key, const void* value, size_t value_size);
void* cm_map_get(cm_map_t* map, const char* key);
int cm_map_has(cm_map_t* map, const char* key);
void cm_map_remove(cm_map_t* map, const char* key);
void cm_map_clear(cm_map_t* map);
size_t cm_map_size(cm_map_t* map);
int cm_map_is_empty(cm_map_t* map);
cm_array_t* cm_map_keys(cm_map_t* map);
cm_array_t* cm_map_values(cm_map_t* map);

/* ===== CORRECTED MACROS (no typeof) ===== */
#define CM_MAP() cm_map_new()
#define CM_MAP_FREE(m) cm_map_free(m)

/* Simple type-specific macros instead of complex generic ones */
#define cm_map_set_int(m, k, v) do { int __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(int)); } while(0)
#define cm_map_set_double(m, k, v) do { double __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(double)); } while(0)
#define cm_map_set_float(m, k, v) do { float __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(float)); } while(0)
#define cm_map_set_string(m, k, v) do { const char* __tmp = (v); cm_map_set(m, k, &__tmp, sizeof(const char*)); } while(0)

/* For convenience, keep the generic name but user must specify type */
#define CM_MAP_SET_INT(m, k, v) cm_map_set_int(m, k, v)
#define CM_MAP_SET_DOUBLE(m, k, v) cm_map_set_double(m, k, v)
#define CM_MAP_SET_FLOAT(m, k, v) cm_map_set_float(m, k, v)
#define CM_MAP_SET_STRING(m, k, v) cm_map_set_string(m, k, v)

#define CM_MAP_GET_INT(m, k) (*(int*)cm_map_get(m, k))
#define CM_MAP_GET_DOUBLE(m, k) (*(double*)cm_map_get(m, k))
#define CM_MAP_GET_FLOAT(m, k) (*(float*)cm_map_get(m, k))
#define CM_MAP_GET_STRING(m, k) (*(char**)cm_map_get(m, k))

#define CM_MAP_HAS(m, k) cm_map_has(m, k)
#define CM_MAP_REMOVE(m, k) cm_map_remove(m, k)
#define CM_MAP_SIZE(m) cm_map_size(m)
#define CM_MAP_EMPTY(m) cm_map_is_empty(m)

#ifdef CM_IMPLEMENTATION_MAP

/* Hash function (djb2) */
static uint32_t cm_hash_string(const char* str) {
    uint32_t hash = 5381;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    
    return hash;
}

/* Create new map */
cm_map_t* cm_map_new(void) {
    cm_map_t* map = (cm_map_t*)cm_alloc(sizeof(cm_map_t), "map", __FILE__, __LINE__);
    if (!map) return NULL;
    
    map->bucket_count = CM_MAP_INITIAL_SIZE;
    map->size = 0;
    map->load_factor = CM_MAP_LOAD_FACTOR;
    map->buckets = (cm_map_entry_t**)cm_alloc(sizeof(cm_map_entry_t*) * map->bucket_count, 
                                               "map_buckets", __FILE__, __LINE__);
    
    if (map->buckets) {
        memset(map->buckets, 0, sizeof(cm_map_entry_t*) * map->bucket_count);
    }
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
        printf("[MAP] Created at %p (%d buckets)\n", (void*)map, map->bucket_count);
    }
    
    return map;
}

/* Resize map */
static void cm_map_resize(cm_map_t* map, int new_size) {
    if (!map) return;
    
    cm_map_entry_t** new_buckets = (cm_map_entry_t**)cm_alloc(
        sizeof(cm_map_entry_t*) * new_size, "map_buckets", __FILE__, __LINE__);
    
    if (!new_buckets) return;
    
    memset(new_buckets, 0, sizeof(cm_map_entry_t*) * new_size);
    
    /* Rehash all entries */
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
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
        printf("[MAP] Resized to %d buckets\n", new_size);
    }
}

/* Set key-value pair */
void cm_map_set(cm_map_t* map, const char* key, const void* value, size_t value_size) {
    if (!map || !key || !value) return;
    
    /* Check load factor */
    if (map->size >= map->bucket_count * map->load_factor) {
        cm_map_resize(map, map->bucket_count * 2);
    }
    
    uint32_t hash = cm_hash_string(key);
    int index = hash % map->bucket_count;
    
    /* Check if key exists */
    cm_map_entry_t* entry = map->buckets[index];
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            /* Update existing entry */
            cm_free(entry->value);
            entry->value = cm_alloc(value_size, "map_value", __FILE__, __LINE__);
            memcpy(entry->value, value, value_size);
            entry->value_size = value_size;
            return;
        }
        entry = entry->next;
    }
    
    /* Create new entry */
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
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
        printf("[MAP] Set '%s' at %p\n", key, (void*)map);
    }
}

/* Get value by key */
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

/* Check if key exists */
int cm_map_has(cm_map_t* map, const char* key) {
    return cm_map_get(map, key) != NULL;
}

/* Remove key */
void cm_map_remove(cm_map_t* map, const char* key) {
    if (!map || !key) return;
    
    uint32_t hash = cm_hash_string(key);
    int index = hash % map->bucket_count;
    
    cm_map_entry_t** prev = &map->buckets[index];
    cm_map_entry_t* entry = map->buckets[index];
    
    while (entry) {
        if (entry->hash == hash && strcmp(entry->key, key) == 0) {
            *prev = entry->next;
            free(entry->key);
            cm_free(entry->value);
            cm_free(entry);
            map->size--;
            
            if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
                printf("[MAP] Removed '%s'\n", key);
            }
            return;
        }
        prev = &entry->next;
        entry = entry->next;
    }
}

/* Clear all entries */
void cm_map_clear(cm_map_t* map) {
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
        map->buckets[i] = NULL;
    }
    
    map->size = 0;
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
        printf("[MAP] Cleared\n");
    }
}

/* Get map size */
size_t cm_map_size(cm_map_t* map) {
    return map ? (size_t)map->size : 0;
}

/* Check if map is empty */
int cm_map_is_empty(cm_map_t* map) {
    return !map || map->size == 0;
}

/* Get all keys */
cm_array_t* cm_map_keys(cm_map_t* map) {
    if (!map) return NULL;
    
    cm_array_t* keys = cm_array_new(sizeof(char*), (size_t)map->size);
    if (!keys) return NULL;
    
    for (int i = 0; i < map->bucket_count; i++) {
        cm_map_entry_t* entry = map->buckets[i];
        while (entry) {
            cm_array_push(keys, &entry->key);
            entry = entry->next;
        }
    }
    
    return keys;
}

/* Get all values */
cm_array_t* cm_map_values(cm_map_t* map) {
    if (!map) return NULL;
    
    cm_array_t* values = cm_array_new(sizeof(void*), (size_t)map->size);
    if (!values) return NULL;
    
    for (int i = 0; i < map->bucket_count; i++) {
        cm_map_entry_t* entry = map->buckets[i];
        while (entry) {
            cm_array_push(values, &entry->value);
            entry = entry->next;
        }
    }
    
    return values;
}

/* Apply function to each entry */
void cm_map_foreach(cm_map_t* map, void (*func)(const char* key, void* value)) {
    if (!map || !func) return;
    
    for (int i = 0; i < map->bucket_count; i++) {
        cm_map_entry_t* entry = map->buckets[i];
        while (entry) {
            func(entry->key, entry->value);
            entry = entry->next;
        }
    }
}

/* Free map */
void cm_map_free(cm_map_t* map) {
    if (!map) return;
    
    cm_map_clear(map);
    cm_free(map->buckets);
    cm_free(map);
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
        printf("[MAP] Freed at %p\n", (void*)map);
    }
}

#endif 

#endif /* CM_MAP_H */