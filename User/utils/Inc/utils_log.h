/***************************************************************
 * FILENAME    : utils_log.h
 * DESCRIPTION : Lightweight logging macros (INFO/WARN/ERROR)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#pragma once


// --- include ---
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


// --- define ---
// コンパイル時ログレベル（ここを変えると出力が消える）
#ifndef LOG_COMPILETIME_MIN_LEVEL
#define LOG_COMPILETIME_MIN_LEVEL  LOG_LVL_INFO
#endif


// --- typedef ---
typedef enum
{
    LOG_LVL_DEBUG = 0,
    LOG_LVL_INFO,
    LOG_LVL_WARN,
    LOG_LVL_ERROR,
    LOG_LVL_FATAL,
    LOG_LVL_NONE

} log_level_t;


// --- variable declaration ---
// --- function prototype ---
void log_init(void);

void log_set_runtime_level(log_level_t lvl);
log_level_t log_get_runtime_level(void);

void log_printf(log_level_t lvl,
                const char* tag,
                const char* file,
                int line,
                const char* fmt, ...);


// --- macros ---
#define LOG_ENABLED(lvl) ((lvl) >= LOG_COMPILETIME_MIN_LEVEL)

// file/line 自動付与
#define LOG_DEBUG(tag, fmt, ...) \
    do { if (LOG_ENABLED(LOG_LVL_DEBUG)) \
        log_printf(LOG_LVL_DEBUG, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_INFO(tag, fmt, ...) \
    do { if (LOG_ENABLED(LOG_LVL_INFO)) \
        log_printf(LOG_LVL_INFO, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_WARN(tag, fmt, ...) \
    do { if (LOG_ENABLED(LOG_LVL_WARN)) \
        log_printf(LOG_LVL_WARN, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

#define LOG_ERROR(tag, fmt, ...) \
    do { if (LOG_ENABLED(LOG_LVL_ERROR)) \
        log_printf(LOG_LVL_ERROR, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__); } while (0)

// FATALは停止（方針に合わせて変更OK）
#define LOG_FATAL(tag, fmt, ...) \
    do { if (LOG_ENABLED(LOG_LVL_FATAL)) \
        log_printf(LOG_LVL_FATAL, tag, __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
        while (1); } while (0)

#ifdef __cplusplus
}
#endif
