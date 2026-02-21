#ifndef CM_ERROR_H
#define CM_ERROR_H

#include "CM_config.h"
#include <setjmp.h>

/*
 * CM_error.h - Error Handling System
 * Author: Adham Hossam
 * 
 * Responsibilities:
 * - Error codes
 * - Exception handling (try/catch)
 * - Error messages
 */

/* ===== Error Codes ===== */
typedef enum {
    CM_SUCCESS = 0,
    CM_ERROR_MEMORY,
    CM_ERROR_NULL_POINTER,
    CM_ERROR_OUT_OF_BOUNDS,
    CM_ERROR_DIVISION_BY_ZERO,
    CM_ERROR_OVERFLOW,
    CM_ERROR_UNDERFLOW,
    CM_ERROR_INVALID_ARGUMENT,
    CM_ERROR_NOT_FOUND,
    CM_ERROR_ALREADY_EXISTS,
    CM_ERROR_PERMISSION_DENIED,
    CM_ERROR_IO,
    CM_ERROR_NETWORK,
    CM_ERROR_TIMEOUT,
    CM_ERROR_THREAD,
    CM_ERROR_SYNC,
    CM_ERROR_PARSE,
    CM_ERROR_TYPE,
    CM_ERROR_UNIMPLEMENTED,
    CM_ERROR_UNKNOWN
} CMError;

/* ===== Global Error State ===== */
static jmp_buf* cm_exception_buffer = NULL;
static CMError cm_last_error = CM_SUCCESS;
static char cm_error_message[512] = {0};

/* ===== Macros ===== */
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
            longjmp(*cm_exception_buffer, 1); \
        } else { \
            fprintf(stderr, "[ERROR] Uncaught exception: %s\n", message); \
            exit(error); \
        } \
    } while(0)

#define CM_FINALLY() \
    jmp_buf __cm_buf; \
    cm_exception_buffer = &__cm_buf; \
    if (setjmp(__cm_buf) == 0)

/* ===== Function Declarations ===== */
const char* cm_error_string(CMError error);
void cm_error_clear(void);
CMError cm_error_get_last(void);
const char* cm_error_get_message(void);
void cm_error_set(CMError error, const char* message);

/* ===== Implementation ===== */
#ifdef CM_IMPLEMENTATION_ERROR

/* Convert error code to string */
const char* cm_error_string(CMError error) {
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

/* Clear last error */
void cm_error_clear(void) {
    cm_last_error = CM_SUCCESS;
    cm_error_message[0] = '\0';
}

/* Get last error code */
CMError cm_error_get_last(void) {
    return cm_last_error;
}

/* Get last error message */
const char* cm_error_get_message(void) {
    return cm_error_message;
}

/* Set error */
void cm_error_set(CMError error, const char* message) {
    cm_last_error = error;
    if (message) {
        strncpy(cm_error_message, message, sizeof(cm_error_message) - 1);
    }
}

#endif /* CM_IMPLEMENTATION_ERROR */
#endif /* CM_ERROR_H */