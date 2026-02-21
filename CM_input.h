#ifndef CM_INPUT_H
#define CM_INPUT_H

#include "CM_config.h"
#include "CM_string.h"
#include <stdio.h>

/*
 * CM_input.h - Input/Output System
 * Author: Adham Hossam
 * 
 * Responsibilities:
 * - User input handling
 * - Formatted output
 * - File I/O
 * - Console utilities
 */

/* ===== Input Macros ===== */
#define CM_INPUT(prompt, var) \
    do { \
        printf("%s", prompt); \
        char __buffer[1024]; \
        if (fgets(__buffer, sizeof(__buffer), stdin)) { \
            __buffer[strcspn(__buffer, "\n")] = 0; \
            var = cm_string_new(__buffer); \
        } \
    } while(0)

#define CM_INPUT_INT(prompt, var) \
    do { \
        printf("%s", prompt); \
        scanf("%d", &(var)); \
        while(getchar() != '\n'); \
    } while(0)

#define CM_INPUT_FLOAT(prompt, var) \
    do { \
        printf("%s", prompt); \
        scanf("%f", &(var)); \
        while(getchar() != '\n'); \
    } while(0)

#define CM_INPUT_DOUBLE(prompt, var) \
    do { \
        printf("%s", prompt); \
        scanf("%lf", &(var)); \
        while(getchar() != '\n'); \
    } while(0)

#define CM_INPUT_CHAR(prompt, var) \
    do { \
        printf("%s", prompt); \
        scanf("%c", &(var)); \
        while(getchar() != '\n'); \
    } while(0)

#define CM_PRINT(...) printf(__VA_ARGS__)
#define CM_PRINTLN(...) printf(__VA_ARGS__); printf("\n")

/* ===== File I/O ===== */
typedef struct {
    FILE* fp;
    char* path;
    char* mode;
    int is_open;
    size_t buffer_size;
    char* buffer;
    int auto_flush;
} cm_file_t;

/* File operations */
cm_file_t* cm_file_open(const char* path, const char* mode);
int cm_file_close(cm_file_t* file);
int cm_file_write(cm_file_t* file, const void* data, size_t size);
int cm_file_write_string(cm_file_t* file, cm_string_t* str);
int cm_file_printf(cm_file_t* file, const char* format, ...);
cm_string_t* cm_file_read_line(cm_file_t* file);
cm_string_t* cm_file_read_all(cm_file_t* file);
int cm_file_eof(cm_file_t* file);
void cm_file_flush(cm_file_t* file);
int cm_file_seek(cm_file_t* file, long offset, int whence);
long cm_file_tell(cm_file_t* file);

/* File utilities */
int cm_file_exists(const char* path);
long cm_file_size(const char* path);
int cm_file_delete(const char* path);
int cm_file_copy(const char* src, const char* dst);
int cm_file_move(const char* src, const char* dst);
char* cm_file_read_line_str(const char* path, int line);

/* ===== Console utilities ===== */
void cm_console_clear(void);
void cm_console_set_color(int color);
void cm_console_reset_color(void);
void cm_console_gotoxy(int x, int y);
void cm_console_hide_cursor(void);
void cm_console_show_cursor(void);

/* ===== Implementation ===== */
#ifdef CM_IMPLEMENTATION_INPUT

/* Open file */
cm_file_t* cm_file_open(const char* path, const char* mode) {
    if (!path || !mode) return NULL;
    
    cm_file_t* file = (cm_file_t*)cm_alloc(sizeof(cm_file_t), "file", __FILE__, __LINE__);
    if (!file) return NULL;
    
    file->fp = fopen(path, mode);
    if (!file->fp) {
        cm_free(file);
        return NULL;
    }
    
    file->path = strdup(path);
    file->mode = strdup(mode);
    file->is_open = 1;
    file->buffer_size = 4096;
    file->buffer = (char*)cm_alloc(file->buffer_size, "file_buffer", __FILE__, __LINE__);
    file->auto_flush = 1;
    
    return file;
}

/* Close file */
int cm_file_close(cm_file_t* file) {
    if (!file || !file->is_open) return -1;
    
    if (file->auto_flush) {
        fflush(file->fp);
    }
    
    int result = fclose(file->fp);
    if (result == 0) {
        file->is_open = 0;
        free(file->path);
        free(file->mode);
        cm_free(file->buffer);
        cm_free(file);
    }
    
    return result;
}

/* Write data to file */
int cm_file_write(cm_file_t* file, const void* data, size_t size) {
    if (!file || !file->is_open || !data) return -1;
    
    size_t written = fwrite(data, 1, size, file->fp);
    if (file->auto_flush) {
        fflush(file->fp);
    }
    
    return (int)written;
}

/* Write string to file */
int cm_file_write_string(cm_file_t* file, cm_string_t* str) {
    if (!file || !file->is_open || !str) return -1;
    return cm_file_write(file, str->data, str->length);
}

/* Write formatted string to file */
int cm_file_printf(cm_file_t* file, const char* format, ...) {
    if (!file || !file->is_open) return -1;
    
    va_list args;
    va_start(args, format);
    int result = vfprintf(file->fp, format, args);
    va_end(args);
    
    if (file->auto_flush) {
        fflush(file->fp);
    }
    
    return result;
}

/* Read line from file */
cm_string_t* cm_file_read_line(cm_file_t* file) {
    if (!file || !file->is_open) return NULL;
    
    if (fgets(file->buffer, (int)file->buffer_size, file->fp) == NULL) {
        return NULL;
    }
    
    file->buffer[strcspn(file->buffer, "\n")] = '\0';
    return cm_string_new(file->buffer);
}

/* Read entire file */
cm_string_t* cm_file_read_all(cm_file_t* file) {
    if (!file || !file->is_open) return NULL;
    
    fseek(file->fp, 0, SEEK_END);
    long size = ftell(file->fp);
    fseek(file->fp, 0, SEEK_SET);
    
    if (size < 0) return NULL;
    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) return NULL;
    
    size_t read = fread(buffer, 1, size, file->fp);
    buffer[read] = '\0';
    
    cm_string_t* result = cm_string_new(buffer);
    free(buffer);
    
    return result;
}

/* Check if end of file */
int cm_file_eof(cm_file_t* file) {
    if (!file || !file->is_open) return 1;
    return feof(file->fp);
}

/* Flush file */
void cm_file_flush(cm_file_t* file) {
    if (!file || !file->is_open) return;
    fflush(file->fp);
}

/* Seek in file */
int cm_file_seek(cm_file_t* file, long offset, int whence) {
    if (!file || !file->is_open) return -1;
    return fseek(file->fp, offset, whence);
}

/* Get current position */
long cm_file_tell(cm_file_t* file) {
    if (!file || !file->is_open) return -1;
    return ftell(file->fp);
}

/* Check if file exists */
int cm_file_exists(const char* path) {
    FILE* fp = fopen(path, "r");
    if (fp) {
        fclose(fp);
        return 1;
    }
    return 0;
}

/* Get file size */
long cm_file_size(const char* path) {
    FILE* fp = fopen(path, "rb");
    if (!fp) return -1;
    
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    
    return size;
}

/* Delete file */
int cm_file_delete(const char* path) {
    return remove(path);
}

/* Copy file */
int cm_file_copy(const char* src, const char* dst) {
    FILE* src_fp = fopen(src, "rb");
    if (!src_fp) return -1;
    
    FILE* dst_fp = fopen(dst, "wb");
    if (!dst_fp) {
        fclose(src_fp);
        return -1;
    }
    
    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
        fwrite(buffer, 1, bytes, dst_fp);
    }
    
    fclose(src_fp);
    fclose(dst_fp);
    return 0;
}

/* Move/rename file */
int cm_file_move(const char* src, const char* dst) {
    return rename(src, dst);
}

/* Read specific line from file */
char* cm_file_read_line_str(const char* path, int line) {
    FILE* fp = fopen(path, "r");
    if (!fp) return NULL;
    
    char buffer[4096];
    int current_line = 1;
    
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (current_line == line) {
            buffer[strcspn(buffer, "\n")] = '\0';
            fclose(fp);
            return strdup(buffer);
        }
        current_line++;
    }
    
    fclose(fp);
    return NULL;
}

/* Clear console */
void cm_console_clear(void) {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[2J\033[H");
    #endif
}

/* Set console color */
void cm_console_set_color(int color) {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    #else
        printf("\033[%dm", color);
    #endif
}

/* Reset console color */
void cm_console_reset_color(void) {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 7);
    #else
        printf("\033[0m");
    #endif
}

/* Move cursor to position */
void cm_console_gotoxy(int x, int y) {
    #ifdef _WIN32
        COORD coord = {x, y};
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hConsole, coord);
    #else
        printf("\033[%d;%dH", y, x);
    #endif
}

/* Hide cursor */
void cm_console_hide_cursor(void) {
    #ifdef _WIN32
        CONSOLE_CURSOR_INFO info;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleCursorInfo(hConsole, &info);
        info.bVisible = FALSE;
        SetConsoleCursorInfo(hConsole, &info);
    #else
        printf("\033[?25l");
    #endif
}

/* Show cursor */
void cm_console_show_cursor(void) {
    #ifdef _WIN32
        CONSOLE_CURSOR_INFO info;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        GetConsoleCursorInfo(hConsole, &info);
        info.bVisible = TRUE;
        SetConsoleCursorInfo(hConsole, &info);
    #else
        printf("\033[?25h");
    #endif
}

#endif /* CM_IMPLEMENTATION_INPUT */
#endif /* CM_INPUT_H */