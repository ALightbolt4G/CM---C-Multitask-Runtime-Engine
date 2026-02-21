#ifndef CM_DEBUG_H
#define CM_DEBUG_H

#include "CM_config.h"
#include "CM_core.h"
#include <stdio.h>
#include <time.h>

/*
 * CM_debug.h - Debugging and Profiling System
 * Author: Adham Hossam
 * 
 * Responsibilities:
 * - Logging
 * - Assertions
 * - Profiling
 * - Memory tracking
 * - Performance monitoring
 */

/* ===== Logging Levels ===== */
typedef enum {
    CM_LOG_ERROR = 0,
    CM_LOG_WARN  = 1,
    CM_LOG_INFO  = 2,
    CM_LOG_DEBUG = 3,
    CM_LOG_TRACE = 4
} CMLogLevel;

/* Global log level */
static CMLogLevel cm_current_log_level = CM_LOG_DEBUG;

/* ===== Logging Macros ===== */
#define CM_LOG(level, ...) \
    do { \
        if (level <= cm_current_log_level && CM_ENABLE_LOGGING) { \
            time_t __cm_now = time(NULL); \
            struct tm* __cm_tm = localtime(&__cm_now); \
            fprintf(stderr, "[%02d:%02d:%02d] ", \
                    __cm_tm->tm_hour, __cm_tm->tm_min, __cm_tm->tm_sec); \
            fprintf(stderr, "[%s:%d] ", __FILE__, __LINE__); \
            fprintf(stderr, __VA_ARGS__); \
            fprintf(stderr, "\n"); \
        } \
    } while(0)

#define CM_ERROR(...)   CM_LOG(CM_LOG_ERROR, "❌ ERROR: " __VA_ARGS__)
#define CM_WARN(...)    CM_LOG(CM_LOG_WARN,  "⚠️ WARNING: " __VA_ARGS__)
#define CM_INFO(...)    CM_LOG(CM_LOG_INFO,  "ℹ️ INFO: " __VA_ARGS__)
#define CM_DEBUG(...)   CM_LOG(CM_LOG_DEBUG, "🔍 DEBUG: " __VA_ARGS__)
#define CM_TRACE(...)   CM_LOG(CM_LOG_TRACE, "📌 TRACE: " __VA_ARGS__)

/* ===== Assertion Macros ===== */
#define CM_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            CM_ERROR("Assertion failed: %s", #cond); \
            exit(1); \
        } \
    } while(0)

#define CM_ASSERT_MSG(cond, msg) \
    do { \
        if (!(cond)) { \
            CM_ERROR("Assertion failed: %s - %s", #cond, msg); \
            exit(1); \
        } \
    } while(0)

#define CM_ASSERT_EQ(a, b) CM_ASSERT((a) == (b))
#define CM_ASSERT_NE(a, b) CM_ASSERT((a) != (b))
#define CM_ASSERT_LT(a, b) CM_ASSERT((a) < (b))
#define CM_ASSERT_GT(a, b) CM_ASSERT((a) > (b))
#define CM_ASSERT_LE(a, b) CM_ASSERT((a) <= (b))
#define CM_ASSERT_GE(a, b) CM_ASSERT((a) >= (b))
#define CM_ASSERT_NOT_NULL(p) CM_ASSERT((p) != NULL)
#define CM_ASSERT_NULL(p) CM_ASSERT((p) == NULL)

/* ===== Profiling ===== */
#ifdef CM_ENABLE_PROFILING

typedef struct {
    char name[64];
    struct timespec start;
    struct timespec end;
    double elapsed;
    int count;
    double total;
    double min;
    double max;
} CMProfile;

#define CM_MAX_PROFILES 100
static CMProfile cm_profiles[CM_MAX_PROFILES];
static int cm_profile_count = 0;

#define CM_PROFILE_START(name) \
    do { \
        if (cm_profile_count < CM_MAX_PROFILES) { \
            CMProfile* p = &cm_profiles[cm_profile_count]; \
            strncpy(p->name, name, sizeof(p->name) - 1); \
            clock_gettime(CLOCK_MONOTONIC, &p->start); \
        } \
    } while(0)

#define CM_PROFILE_STOP() \
    do { \
        if (cm_profile_count < CM_MAX_PROFILES) { \
            CMProfile* p = &cm_profiles[cm_profile_count]; \
            clock_gettime(CLOCK_MONOTONIC, &p->end); \
            p->elapsed = (p->end.tv_sec - p->start.tv_sec) + \
                         (p->end.tv_nsec - p->start.tv_nsec) / 1e9; \
            p->count = 1; \
            p->total = p->elapsed; \
            p->min = p->elapsed; \
            p->max = p->elapsed; \
            cm_profile_count++; \
        } \
    } while(0)

#define CM_PROFILE_FUNC(func, ...) \
    do { \
        CM_PROFILE_START(#func); \
        func(__VA_ARGS__); \
        CM_PROFILE_STOP(); \
    } while(0)

/* Show profiling results */
void cm_profile_show(void) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    PROFILING RESULTS                       ║\n");
    printf("╠════════════════════════════════════════════════════════════╣\n");
    
    for (int i = 0; i < cm_profile_count; i++) {
        CMProfile* p = &cm_profiles[i];
        printf("║ %-30s: %8.3f ms                                     ║\n", 
               p->name, p->elapsed * 1000);
    }
    
    printf("╚════════════════════════════════════════════════════════════╝\n");
}

/* Reset profiles */
void cm_profile_reset(void) {
    cm_profile_count = 0;
}

#else /* CM_ENABLE_PROFILING */

#define CM_PROFILE_START(name)
#define CM_PROFILE_STOP()
#define CM_PROFILE_FUNC(func, ...) func(__VA_ARGS__)
#define cm_profile_show()
#define cm_profile_reset()

#endif /* CM_ENABLE_PROFILING */

/* ===== Memory Tracking ===== */
#define CM_TRACK(var) \
    CM_DEBUG("Variable '%s' at %p (size: %zu)", #var, (void*)&(var), sizeof(var))

#define CM_TRACK_PTR(ptr) \
    CM_DEBUG("Pointer %p (type: %s, size: %zu)", \
             (void*)(ptr), cm_get_type(ptr), cm_get_size(ptr))

/* ===== Benchmarking ===== */
typedef struct {
    struct timespec start;
    struct timespec end;
    double elapsed;
} CMTimer;

#define CM_TIMER_START(timer) clock_gettime(CLOCK_MONOTONIC, &(timer).start)
#define CM_TIMER_STOP(timer) clock_gettime(CLOCK_MONOTONIC, &(timer).end)
#define CM_TIMER_ELAPSED(timer) \
    ((timer.end.tv_sec - timer.start.tv_sec) + \
     (timer.end.tv_nsec - timer.start.tv_nsec) / 1e9)

#define CM_TIME_BLOCK(name, code) \
    do { \
        CMTimer __timer; \
        CM_TIMER_START(__timer); \
        code; \
        CM_TIMER_STOP(__timer); \
        CM_INFO("Block '%s' took %.3f ms", name, CM_TIMER_ELAPSED(__timer) * 1000); \
    } while(0)

/* ===== Function Declarations ===== */
void cm_set_log_level(CMLogLevel level);
CMLogLevel cm_get_log_level(void);
void cm_hex_dump(const void* data, size_t size);
void cm_memory_barrier(void);

/* ===== Implementation ===== */
#ifdef CM_IMPLEMENTATION_DEBUG

/* Set log level */
void cm_set_log_level(CMLogLevel level) {
    cm_current_log_level = level;
    CM_INFO("Log level set to %d", level);
}

/* Get current log level */
CMLogLevel cm_get_log_level(void) {
    return cm_current_log_level;
}

/* Hex dump memory */
void cm_hex_dump(const void* data, size_t size) {
    const unsigned char* bytes = (const unsigned char*)data;
    
    for (size_t i = 0; i < size; i += 16) {
        printf("%08zx  ", i);
        
        /* Hex */
        for (size_t j = 0; j < 16; j++) {
            if (i + j < size) {
                printf("%02x ", bytes[i + j]);
            } else {
                printf("   ");
            }
            if (j == 7) printf(" ");
        }
        
        printf(" |");
        
        /* ASCII */
        for (size_t j = 0; j < 16 && i + j < size; j++) {
            unsigned char c = bytes[i + j];
            printf("%c", (c >= 32 && c <= 126) ? c : '.');
        }
        
        printf("|\n");
    }
}

/* Memory barrier (for multi-threading) */
void cm_memory_barrier(void) {
    #ifdef CM_ENABLE_THREAD_SAFETY
        __sync_synchronize();
    #endif
}

#endif /* CM_IMPLEMENTATION_DEBUG */
#endif /* CM_DEBUG_H */