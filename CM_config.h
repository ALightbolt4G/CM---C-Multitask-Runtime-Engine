#ifndef CM_CONFIG_H
#define CM_CONFIG_H

/*
 * CM_config.h - Library Configuration
 * Author: Adham Hossam
 */

/* Version Information */
#define CM_VERSION_MAJOR    3
#define CM_VERSION_MINOR    0
#define CM_VERSION_PATCH    0
#define CM_VERSION_STRING   "3.0.0"
#define CM_AUTHOR           "Adham Hossam"

/* Feature Flags (User Configurable) */
#ifndef CM_ENABLE_GC
    #define CM_ENABLE_GC 1
#endif

#ifndef CM_ENABLE_LOGGING
    #define CM_ENABLE_LOGGING 1
#endif

#ifndef CM_ENABLE_THREAD_SAFETY
    #define CM_ENABLE_THREAD_SAFETY 0  /* Disabled by default */
#endif

#ifndef CM_ENABLE_PROFILING
    #define CM_ENABLE_PROFILING 0
#endif

#ifndef CM_ENABLE_NETWORKING
    #define CM_ENABLE_NETWORKING 0
#endif

/* Performance Tuning */
#ifndef CM_GC_THRESHOLD
    #define CM_GC_THRESHOLD (1024 * 1024)  /* 1MB */
#endif

#ifndef CM_LOG_LEVEL
    #define CM_LOG_LEVEL 3  /* 0:ERROR, 1:WARN, 2:INFO, 3:DEBUG, 4:TRACE */
#endif

#ifndef CM_MAX_STRING_LENGTH
    #define CM_MAX_STRING_LENGTH (1024 * 1024 * 100)  /* 100MB */
#endif

/* Platform Detection */
#if defined(__GNUC__) || defined(__clang__)
    #define CM_COMPILER_GCC
    #define CM_INLINE static inline __attribute__((always_inline))
    #define CM_UNUSED __attribute__((unused))
    #define CM_PACKED __attribute__((packed))
    #define CM_ALIGNED(x) __attribute__((aligned(x)))
    #define CM_DEPRECATED __attribute__((deprecated))
    #define CM_WARN_UNUSED __attribute__((warn_unused_result))
#elif defined(_MSC_VER)
    #define CM_COMPILER_MSVC
    #define CM_INLINE static __inline
    #define CM_UNUSED
    #define CM_PACKED __pragma(pack(push, 1)) 
    #define CM_ALIGNED(x) __declspec(align(x))
    #define CM_DEPRECATED __declspec(deprecated)
    #define CM_WARN_UNUSED _Check_return_
#else
    #define CM_INLINE static inline
    #define CM_UNUSED
    #define CM_PACKED
    #define CM_ALIGNED(x)
    #define CM_DEPRECATED
    #define CM_WARN_UNUSED
#endif

/* Thread Safety */
#ifdef CM_ENABLE_THREAD_SAFETY
    #include <pthread.h>
    #define CM_LOCK(m) pthread_mutex_lock(m)
    #define CM_UNLOCK(m) pthread_mutex_unlock(m)
    #define CM_MUTEX_T pthread_mutex_t
    #define CM_MUTEX_INIT(m) pthread_mutex_init(m, NULL)
    #define CM_MUTEX_DESTROY(m) pthread_mutex_destroy(m)
#else
    #define CM_LOCK(m)
    #define CM_UNLOCK(m)
    #define CM_MUTEX_T int
    #define CM_MUTEX_INIT(m)
    #define CM_MUTEX_DESTROY(m)
#endif

#endif /* CM_CONFIG_H */