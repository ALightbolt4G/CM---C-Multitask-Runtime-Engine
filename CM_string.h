#ifndef CM_STRING_H
#define CM_STRING_H

#include "CM_config.h"
#include "CM_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * CM_string.h - Smart String System
 * Author: Adham Hossam
 * 
 * Responsibilities:
 * - String creation/destruction
 * - String manipulation functions
 * - Unicode support
 * - Formatting
 */

/* ===== String Type ===== */
typedef struct {
    char* data;          /* String data */
    size_t length;        /* Current length */
    size_t capacity;      /* Allocated capacity */
    int ref_count;        /* Reference count */
    uint32_t hash;        /* Cached hash value */
} cm_string_t;

/* ===== Function Declarations ===== */
cm_string_t* cm_string_new(const char* initial);
cm_string_t* cm_string_new_len(const char* data, size_t len);
void cm_string_free(cm_string_t* s);
void cm_string_set(cm_string_t* s, const char* value);
void cm_string_set_len(cm_string_t* s, const char* value, size_t len);
cm_string_t* cm_string_concat(cm_string_t* s1, cm_string_t* s2);
cm_string_t* cm_string_append(cm_string_t* s, const char* suffix);
cm_string_t* cm_string_prepend(cm_string_t* s, const char* prefix);
cm_string_t* cm_string_slice(cm_string_t* s, int start, int end);
int cm_string_compare(cm_string_t* s1, cm_string_t* s2);
cm_string_t* cm_string_format(const char* format, ...);
cm_string_t** cm_string_split(cm_string_t* s, const char* delimiter, int* count);
int cm_string_find(cm_string_t* s, const char* substring);
int cm_string_find_last(cm_string_t* s, const char* substring);
cm_string_t* cm_string_replace(cm_string_t* s, const char* old, const char* new_str);
cm_string_t* cm_string_replace_all(cm_string_t* s, const char* old, const char* new_str);
void cm_string_upper(cm_string_t* s);
void cm_string_lower(cm_string_t* s);
void cm_string_reverse(cm_string_t* s);
void cm_string_trim(cm_string_t* s);
void cm_string_trim_left(cm_string_t* s);
void cm_string_trim_right(cm_string_t* s);
uint32_t cm_string_hash(cm_string_t* s);
cm_string_t* cm_string_dup(cm_string_t* s);
int cm_string_starts_with(cm_string_t* s, const char* prefix);
int cm_string_ends_with(cm_string_t* s, const char* suffix);
cm_string_t* cm_string_substring(cm_string_t* s, size_t start, size_t count);
char cm_string_char_at(cm_string_t* s, size_t index);
int cm_string_to_int(cm_string_t* s);
double cm_string_to_double(cm_string_t* s);
long cm_string_to_long(cm_string_t* s);
int cm_string_is_number(cm_string_t* s);
int cm_string_is_alpha(cm_string_t* s);
int cm_string_is_alnum(cm_string_t* s);
int cm_string_is_space(cm_string_t* s);
int cm_string_is_upper(cm_string_t* s);
int cm_string_is_lower(cm_string_t* s);
void cm_string_clear(cm_string_t* s);
size_t cm_string_count(cm_string_t* s, const char* substring);

/* ===== Implementation ===== */
#ifdef CM_IMPLEMENTATION_STRING

/* Create new string */
cm_string_t* cm_string_new(const char* initial) {
    cm_string_t* s = (cm_string_t*)cm_alloc(sizeof(cm_string_t), "string", __FILE__, __LINE__);
    if (!s) return NULL;
    
    size_t len = initial ? strlen(initial) : 0;
    s->length = len;
    s->capacity = len + 1;
    s->data = (char*)cm_alloc(s->capacity, "string_data", __FILE__, __LINE__);
    s->ref_count = 1;
    s->hash = 0;
    
    if (s->data) {
        if (initial && len > 0) {
            strcpy(s->data, initial);
        } else {
            s->data[0] = '\0';
        }
    }
    
    if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
        printf("[STRING] Created at %p: '%s'\n", (void*)s, initial ? initial : "");
    }
    
    return s;
}

/* Create string with specific length */
cm_string_t* cm_string_new_len(const char* data, size_t len) {
    if (!data) return cm_string_new(NULL);
    
    cm_string_t* s = (cm_string_t*)cm_alloc(sizeof(cm_string_t), "string", __FILE__, __LINE__);
    if (!s) return NULL;
    
    s->length = len;
    s->capacity = len + 1;
    s->data = (char*)cm_alloc(s->capacity, "string_data", __FILE__, __LINE__);
    s->ref_count = 1;
    s->hash = 0;
    
    if (s->data) {
        strncpy(s->data, data, len);
        s->data[len] = '\0';
    }
    
    return s;
}

/* Free string */
void cm_string_free(cm_string_t* s) {
    if (!s) return;
    
    s->ref_count--;
    if (s->ref_count <= 0) {
        if (s->data) cm_free(s->data);
        cm_free(s);
        
        if (CM_ENABLE_LOGGING && CM_LOG_LEVEL >= 4) {
            printf("[STRING] Freed at %p\n", (void*)s);
        }
    }
}

/* Set string value */
void cm_string_set(cm_string_t* s, const char* value) {
    if (!s) return;
    if (!value) value = "";
    
    size_t len = strlen(value);
    
    if (len + 1 > s->capacity) {
        char* new_data = (char*)cm_alloc(len + 1, "string_data", __FILE__, __LINE__);
        if (!new_data) return;
        
        cm_free(s->data);
        s->data = new_data;
        s->capacity = len + 1;
    }
    
    strcpy(s->data, value);
    s->length = len;
    s->hash = 0;  /* Invalidate hash */
}

/* Set string with specific length */
void cm_string_set_len(cm_string_t* s, const char* value, size_t len) {
    if (!s || !value) return;
    
    if (len + 1 > s->capacity) {
        char* new_data = (char*)cm_alloc(len + 1, "string_data", __FILE__, __LINE__);
        if (!new_data) return;
        
        cm_free(s->data);
        s->data = new_data;
        s->capacity = len + 1;
    }
    
    strncpy(s->data, value, len);
    s->data[len] = '\0';
    s->length = len;
    s->hash = 0;
}

/* Concatenate two strings */
cm_string_t* cm_string_concat(cm_string_t* s1, cm_string_t* s2) {
    if (!s1 || !s2) return NULL;
    
    size_t new_len = s1->length + s2->length;
    char* temp = (char*)malloc(new_len + 1);
    if (!temp) return NULL;
    
    strcpy(temp, s1->data);
    strcat(temp, s2->data);
    
    cm_string_t* result = cm_string_new(temp);
    free(temp);
    
    return result;
}

/* Append to string */
cm_string_t* cm_string_append(cm_string_t* s, const char* suffix) {
    if (!s || !suffix) return NULL;
    
    cm_string_t* temp = cm_string_new(suffix);
    if (!temp) return NULL;
    
    cm_string_t* result = cm_string_concat(s, temp);
    cm_string_free(temp);
    
    return result;
}

/* Prepend to string */
cm_string_t* cm_string_prepend(cm_string_t* s, const char* prefix) {
    if (!s || !prefix) return NULL;
    
    cm_string_t* temp = cm_string_new(prefix);
    if (!temp) return NULL;
    
    cm_string_t* result = cm_string_concat(temp, s);
    cm_string_free(temp);
    
    return result;
}

/* Slice string (Python-like slicing) */
cm_string_t* cm_string_slice(cm_string_t* s, int start, int end) {
    if (!s) return NULL;
    
    /* Handle negative indices */
    if (start < 0) start = (int)s->length + start;
    if (end < 0) end = (int)s->length + end;
    
    /* Clamp to bounds */
    if (start < 0) start = 0;
    if (end > (int)s->length) end = (int)s->length;
    if (start >= end) return cm_string_new("");
    
    size_t slice_len = end - start;
    return cm_string_new_len(s->data + start, slice_len);
}

/* Compare strings */
int cm_string_compare(cm_string_t* s1, cm_string_t* s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    return strcmp(s1->data, s2->data);
}

/* Format string (like sprintf) */
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

/* Split string by delimiter */
cm_string_t** cm_string_split(cm_string_t* s, const char* delimiter, int* count) {
    if (!s || !s->data || !delimiter) return NULL;
    
    char* str_dup = strdup(s->data);
    if (!str_dup) return NULL;
    
    char* token;
    char* saveptr;
    cm_string_t** result = NULL;
    int capacity = 0;
    *count = 0;
    
    token = strtok_r(str_dup, delimiter, &saveptr);
    while (token) {
        if (*count >= capacity) {
            capacity = capacity ? capacity * 2 : 8;
            cm_string_t** new_result = (cm_string_t**)realloc(result, capacity * sizeof(cm_string_t*));
            if (!new_result) {
                for (int i = 0; i < *count; i++) {
                    cm_string_free(result[i]);
                }
                free(result);
                free(str_dup);
                return NULL;
            }
            result = new_result;
        }
        
        result[*count] = cm_string_new(token);
        if (!result[*count]) {
            for (int i = 0; i < *count; i++) {
                cm_string_free(result[i]);
            }
            free(result);
            free(str_dup);
            return NULL;
        }
        
        (*count)++;
        token = strtok_r(NULL, delimiter, &saveptr);
    }
    
    free(str_dup);
    return result;
}

/* Find first occurrence */
int cm_string_find(cm_string_t* s, const char* substring) {
    if (!s || !s->data || !substring) return -1;
    
    char* pos = strstr(s->data, substring);
    if (!pos) return -1;
    
    return (int)(pos - s->data);
}

/* Find last occurrence */
int cm_string_find_last(cm_string_t* s, const char* substring) {
    if (!s || !s->data || !substring) return -1;
    
    char* last = NULL;
    char* current = s->data;
    int sub_len = strlen(substring);
    
    while ((current = strstr(current, substring))) {
        last = current;
        current += sub_len;
    }
    
    if (!last) return -1;
    return (int)(last - s->data);
}

/* Replace first occurrence */
cm_string_t* cm_string_replace(cm_string_t* s, const char* old, const char* new_str) {
    if (!s || !s->data || !old || !new_str) return NULL;
    
    int pos = cm_string_find(s, old);
    if (pos < 0) return cm_string_dup(s);
    
    int old_len = strlen(old);
    int new_len = strlen(new_str);
    
    size_t new_total_len = s->length - old_len + new_len;
    char* temp = (char*)malloc(new_total_len + 1);
    if (!temp) return NULL;
    
    /* Copy before match */
    strncpy(temp, s->data, pos);
    temp[pos] = '\0';
    
    /* Copy replacement */
    strcat(temp, new_str);
    
    /* Copy after match */
    strcat(temp, s->data + pos + old_len);
    
    cm_string_t* result = cm_string_new(temp);
    free(temp);
    
    return result;
}

/* Replace all occurrences */
cm_string_t* cm_string_replace_all(cm_string_t* s, const char* old, const char* new_str) {
    if (!s || !s->data || !old || !new_str) return NULL;
    
    cm_string_t* current = cm_string_dup(s);
    if (!current) return NULL;
    
    int pos;
    while ((pos = cm_string_find(current, old)) >= 0) {
        cm_string_t* next = cm_string_replace(current, old, new_str);
        cm_string_free(current);
        if (!next) return NULL;
        current = next;
    }
    
    return current;
}

/* Convert to uppercase */
void cm_string_upper(cm_string_t* s) {
    if (!s || !s->data) return;
    
    for (size_t i = 0; i < s->length; i++) {
        s->data[i] = toupper(s->data[i]);
    }
    s->hash = 0;
}

/* Convert to lowercase */
void cm_string_lower(cm_string_t* s) {
    if (!s || !s->data) return;
    
    for (size_t i = 0; i < s->length; i++) {
        s->data[i] = tolower(s->data[i]);
    }
    s->hash = 0;
}

/* Reverse string */
void cm_string_reverse(cm_string_t* s) {
    if (!s || !s->data || s->length <= 1) return;
    
    char* start = s->data;
    char* end = s->data + s->length - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    s->hash = 0;
}

/* Trim whitespace from both ends */
void cm_string_trim(cm_string_t* s) {
    if (!s || !s->data) return;
    
    char* start = s->data;
    char* end = s->data + s->length - 1;
    
    while (*start && isspace(*start)) start++;
    while (end > start && isspace(*end)) end--;
    
    size_t new_len = end - start + 1;
    
    if (start > s->data) {
        memmove(s->data, start, new_len);
    }
    
    s->data[new_len] = '\0';
    s->length = new_len;
    s->hash = 0;
}

/* Trim left */
void cm_string_trim_left(cm_string_t* s) {
    if (!s || !s->data) return;
    
    char* start = s->data;
    while (*start && isspace(*start)) start++;
    
    if (start > s->data) {
        size_t new_len = s->length - (start - s->data);
        memmove(s->data, start, new_len);
        s->data[new_len] = '\0';
        s->length = new_len;
        s->hash = 0;
    }
}

/* Trim right */
void cm_string_trim_right(cm_string_t* s) {
    if (!s || !s->data) return;
    
    char* end = s->data + s->length - 1;
    while (end >= s->data && isspace(*end)) {
        *end = '\0';
        end--;
        s->length--;
    }
    s->hash = 0;
}

/* Compute hash (FNV-1a) */
uint32_t cm_string_hash(cm_string_t* s) {
    if (!s || !s->data) return 0;
    if (s->hash != 0) return s->hash;
    
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < s->length; i++) {
        hash ^= (uint8_t)s->data[i];
        hash *= 16777619;
    }
    
    s->hash = hash;
    return hash;
}

/* Duplicate string */
cm_string_t* cm_string_dup(cm_string_t* s) {
    if (!s) return NULL;
    return cm_string_new(s->data);
}

/* Check if starts with prefix */
int cm_string_starts_with(cm_string_t* s, const char* prefix) {
    if (!s || !s->data || !prefix) return 0;
    
    size_t prefix_len = strlen(prefix);
    if (prefix_len > s->length) return 0;
    
    return strncmp(s->data, prefix, prefix_len) == 0;
}

/* Check if ends with suffix */
int cm_string_ends_with(cm_string_t* s, const char* suffix) {
    if (!s || !s->data || !suffix) return 0;
    
    size_t suffix_len = strlen(suffix);
    if (suffix_len > s->length) return 0;
    
    return strncmp(s->data + s->length - suffix_len, suffix, suffix_len) == 0;
}

/* Get substring */
cm_string_t* cm_string_substring(cm_string_t* s, size_t start, size_t count) {
    if (!s || !s->data || start >= s->length) return cm_string_new("");
    
    if (start + count > s->length) {
        count = s->length - start;
    }
    
    return cm_string_new_len(s->data + start, count);
}

/* Get character at index */
char cm_string_char_at(cm_string_t* s, size_t index) {
    if (!s || !s->data || index >= s->length) return '\0';
    return s->data[index];
}

/* Convert to int */
int cm_string_to_int(cm_string_t* s) {
    if (!s || !s->data) return 0;
    return atoi(s->data);
}

/* Convert to double */
double cm_string_to_double(cm_string_t* s) {
    if (!s || !s->data) return 0.0;
    return atof(s->data);
}

/* Convert to long */
long cm_string_to_long(cm_string_t* s) {
    if (!s || !s->data) return 0;
    return strtol(s->data, NULL, 10);
}

/* Check if string is a number */
int cm_string_is_number(cm_string_t* s) {
    if (!s || !s->data || s->length == 0) return 0;
    
    for (size_t i = 0; i < s->length; i++) {
        if (!isdigit(s->data[i]) && s->data[i] != '.' && s->data[i] != '-') {
            return 0;
        }
    }
    return 1;
}

/* Check if all alphabetic */
int cm_string_is_alpha(cm_string_t* s) {
    if (!s || !s->data || s->length == 0) return 0;
    
    for (size_t i = 0; i < s->length; i++) {
        if (!isalpha(s->data[i])) return 0;
    }
    return 1;
}

/* Check if alphanumeric */
int cm_string_is_alnum(cm_string_t* s) {
    if (!s || !s->data || s->length == 0) return 0;
    
    for (size_t i = 0; i < s->length; i++) {
        if (!isalnum(s->data[i])) return 0;
    }
    return 1;
}

/* Check if all whitespace */
int cm_string_is_space(cm_string_t* s) {
    if (!s || !s->data) return 0;
    if (s->length == 0) return 1;
    
    for (size_t i = 0; i < s->length; i++) {
        if (!isspace(s->data[i])) return 0;
    }
    return 1;
}

/* Check if all uppercase */
int cm_string_is_upper(cm_string_t* s) {
    if (!s || !s->data || s->length == 0) return 0;
    
    for (size_t i = 0; i < s->length; i++) {
        if (isalpha(s->data[i]) && !isupper(s->data[i])) return 0;
    }
    return 1;
}

/* Check if all lowercase */
int cm_string_is_lower(cm_string_t* s) {
    if (!s || !s->data || s->length == 0) return 0;
    
    for (size_t i = 0; i < s->length; i++) {
        if (isalpha(s->data[i]) && !islower(s->data[i])) return 0;
    }
    return 1;
}

/* Clear string */
void cm_string_clear(cm_string_t* s) {
    if (!s || !s->data) return;
    
    s->data[0] = '\0';
    s->length = 0;
    s->hash = 0;
}

/* Count occurrences of substring */
size_t cm_string_count(cm_string_t* s, const char* substring) {
    if (!s || !s->data || !substring) return 0;
    
    size_t count = 0;
    const char* pos = s->data;
    size_t sub_len = strlen(substring);
    
    if (sub_len == 0) return 0;
    
    while ((pos = strstr(pos, substring))) {
        count++;
        pos += sub_len;
    }
    
    return count;
}

#endif /* CM_IMPLEMENTATION_STRING */
#endif /* CM_STRING_H */