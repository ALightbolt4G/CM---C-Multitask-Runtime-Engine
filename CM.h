#ifndef CM_H
#define CM_H

/*
 * ============================================================================
 * CM.h - C Multitask Intelligent Library
 * Master Header File
 * ============================================================================
 * 
 * Author:  Adham Hossam
 * Version: 3.0.0
 * 
 * This is the main header file that includes all CM library components.
 * To use the library, define CM_IMPLEMENTATION in ONE source file and
 * include this header.
 * 
 * Example:
 *   #define CM_IMPLEMENTATION
 *   #include "CM.h"
 * ============================================================================
 */

/* ===== Library Information ===== */
#define CM_VERSION "3.0.0"
#define CM_AUTHOR "Adham Hossam"

/* ===== Include all component headers ===== */
#include "CM_string.h"
#include "CM_config.h"
#include "CM_core.h"
#include "CM_error.h"
#include "CM_debug.h"
#include "CM_string.h"
#include "CM_array.h"
#include "CM_map.h"
#include "CM_input.h"
#include "CM_utils.h"

/* ===== Convenience Macros ===== */
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

/* ===== Initialize all components ===== */
#ifdef CM_IMPLEMENTATION

    /* Include implementations */
    #define CM_IMPLEMENTATION_CORE
    #include "CM_core.h"
    
    #define CM_IMPLEMENTATION_ERROR
    #include "CM_error.h"
    
    #define CM_IMPLEMENTATION_DEBUG
    #include "CM_debug.h"
    
    #define CM_IMPLEMENTATION_STRING
    #include "CM_string.h"
    
    #define CM_IMPLEMENTATION_ARRAY
    #include "CM_array.h"
    
    #define CM_IMPLEMENTATION_MAP
    #include "CM_map.h"
    
    #define CM_IMPLEMENTATION_INPUT
    #include "CM_input.h"
    
    #define CM_IMPLEMENTATION_UTILS
    #include "CM_utils.h"

    /* Constructor - runs at program startup */
    __attribute__((constructor)) void cm_init_all(void) {
        cm_gc_init();
        cm_random_seed((unsigned int)time(NULL));
        CM_INFO("CM Library v%s initialized", CM_VERSION);
    }

    /* Destructor - runs at program exit */
    __attribute__((destructor)) void cm_cleanup_all(void) {
        cm_gc_collect();
        if (cm_gc.total_objects > 0) {
            CM_WARN("Memory leaks detected: %zu objects", cm_gc.total_objects);
            cm_gc_stats();
        } else {
            CM_INFO("Clean shutdown - no memory leaks");
        }
    }

#endif /* CM_IMPLEMENTATION */

#endif /* CM_H */