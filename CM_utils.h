#ifndef CM_UTILS_H
#define CM_UTILS_H

#include "CM_config.h"
#include "CM_string.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * CM_utils.h - Utility Functions
 * Author: Adham Hossam
 * 
 * Responsibilities:
 * - Random numbers
 * - Date/time utilities
 * - Math helpers
 * - System information
 */

/* ===== Random Number Functions ===== */
void cm_random_seed(unsigned int seed);
int cm_random_int(int min, int max);
double cm_random_double(double min, double max);
float cm_random_float(float min, float max);
void cm_random_string(char* buffer, size_t length);

/* ===== Date/Time Functions ===== */
cm_string_t* cm_time_now(void);
cm_string_t* cm_date_now(void);
long cm_timestamp(void);
void cm_sleep_ms(int milliseconds);
double cm_time_diff(struct timespec start, struct timespec end);

/* ===== Math Helpers ===== */
#define CM_MIN(a, b) ((a) < (b) ? (a) : (b))
#define CM_MAX(a, b) ((a) > (b) ? (a) : (b))
#define CM_CLAMP(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))
#define CM_ABS(x) ((x) < 0 ? -(x) : (x))
#define CM_SWAP(a, b, type) do { type __tmp = a; a = b; b = __tmp; } while(0)

int cm_is_power_of_two(unsigned int x);
unsigned int cm_next_power_of_two(unsigned int x);
double cm_deg_to_rad(double deg);
double cm_rad_to_deg(double rad);
int cm_gcd(int a, int b);
int cm_lcm(int a, int b);
int cm_factorial(int n);
int cm_fibonacci(int n);

/* ===== System Information ===== */
cm_string_t* cm_system_info(void);
cm_string_t* cm_cpu_info(void);
cm_string_t* cm_memory_info(void);
long cm_available_memory(void);
int cm_cpu_cores(void);

/* ===== String Utilities ===== */
cm_string_t* cm_uuid_generate(void);
cm_string_t* cm_base64_encode(const unsigned char* data, size_t len);
unsigned char* cm_base64_decode(const char* data, size_t* out_len);
cm_string_t* cm_url_encode(const char* str);
cm_string_t* cm_url_decode(const char* str);

/* ===== Implementation ===== */
#ifdef CM_IMPLEMENTATION_UTILS

/* Random seed */
void cm_random_seed(unsigned int seed) {
    srand(seed);
}

/* Random integer */
int cm_random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

/* Random double */
double cm_random_double(double min, double max) {
    double scale = rand() / (double)RAND_MAX;
    return min + scale * (max - min);
}

/* Random float */
float cm_random_float(float min, float max) {
    float scale = rand() / (float)RAND_MAX;
    return min + scale * (max - min);
}

/* Random string */
void cm_random_string(char* buffer, size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    size_t charset_size = sizeof(charset) - 1;
    
    for (size_t i = 0; i < length - 1; i++) {
        buffer[i] = charset[rand() % charset_size];
    }
    buffer[length - 1] = '\0';
}

/* Current time string */
cm_string_t* cm_time_now(void) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    char buffer[9];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", tm_info);
    return cm_string_new(buffer);
}

/* Current date string */
cm_string_t* cm_date_now(void) {
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm_info);
    return cm_string_new(buffer);
}

/* Unix timestamp */
long cm_timestamp(void) {
    return (long)time(NULL);
}

/* Sleep in milliseconds */
void cm_sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

/* Time difference in seconds */
double cm_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

/* Check if number is power of two */
int cm_is_power_of_two(unsigned int x) {
    return (x != 0) && ((x & (x - 1)) == 0);
}

/* Next power of two */
unsigned int cm_next_power_of_two(unsigned int x) {
    if (x == 0) return 1;
    x--;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x + 1;
}

/* Degrees to radians */
double cm_deg_to_rad(double deg) {
    return deg * 3.14159265358979323846 / 180.0;
}

/* Radians to degrees */
double cm_rad_to_deg(double rad) {
    return rad * 180.0 / 3.14159265358979323846;
}

/* Greatest common divisor */
int cm_gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/* Least common multiple */
int cm_lcm(int a, int b) {
    return a * b / cm_gcd(a, b);
}

/* Factorial */
int cm_factorial(int n) {
    if (n <= 1) return 1;
    return n * cm_factorial(n - 1);
}

/* Fibonacci */
int cm_fibonacci(int n) {
    if (n <= 1) return n;
    return cm_fibonacci(n - 1) + cm_fibonacci(n - 2);
}

/* System info (simplified) */
cm_string_t* cm_system_info(void) {
    return cm_string_format("System Information\n"
                           "OS: %s\n"
                           "Compiler: %s\n"
                           "Architecture: %zu-bit",
                           #ifdef _WIN32
                               "Windows",
                           #elif __APPLE__
                               "macOS",
                           #elif __linux__
                               "Linux",
                           #else
                               "Unknown",
                           #endif
                           #ifdef __GNUC__
                               "GCC",
                           #elif __clang__
                               "Clang",
                           #elif _MSC_VER
                               "MSVC",
                           #else
                               "Unknown",
                           #endif
                           sizeof(void*) * 8);
}

/* CPU cores (simplified) */
int cm_cpu_cores(void) {
    #ifdef _WIN32
        SYSTEM_INFO sysinfo;
        GetSystemInfo(&sysinfo);
        return sysinfo.dwNumberOfProcessors;
    #elif __linux__
        return sysconf(_SC_NPROCESSORS_ONLN);
    #elif __APPLE__
        return sysconf(_SC_NPROCESSORS_ONLN);
    #else
        return 1;
    #endif
}

/* Available memory (simplified) */
long cm_available_memory(void) {
    #ifdef _WIN32
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return (long)status.ullAvailPhys;
    #elif __linux__
        long pages = sysconf(_SC_AVPHYS_PAGES);
        long page_size = sysconf(_SC_PAGESIZE);
        return pages * page_size;
    #elif __APPLE__
        // Simplified - returns total memory
        return sysconf(_SC_PHYS_PAGES) * sysconf(_SC_PAGESIZE);
    #else
        return -1;
    #endif
}

/* Generate UUID (simplified) */
cm_string_t* cm_uuid_generate(void) {
    char buffer[37];
    sprintf(buffer, "%08x-%04x-%04x-%04x-%08x%04x",
            rand(), rand() & 0xFFFF,
            (rand() & 0x0FFF) | 0x4000,
            (rand() & 0x3FFF) | 0x8000,
            rand(), rand() & 0xFFFF);
    return cm_string_new(buffer);
}

#endif /* CM_IMPLEMENTATION_UTILS */
#endif /* CM_UTILS_H */