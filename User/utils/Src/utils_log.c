/***************************************************************
 * FILENAME    : utils_log.c
 * DESCRIPTION : Lightweight logging backend implementation
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/



// --- include ---
#include <stdio.h>
#include <stdarg.h>

#include "utils_log.h"
#include "stm32h7xx_hal.h"

// --- define ---
// --- typedef ---
// --- variable declaration ---
static volatile log_level_t s_runtime_level = LOG_LVL_DEBUG;

// --- function prototype ---
static const char* level_to_str(log_level_t lvl);

// --- functions ---
void log_init(void)
{
    // 将来：リングバッファ初期化など
}

void log_set_runtime_level(log_level_t lvl)
{
    s_runtime_level = lvl;
}

log_level_t log_get_runtime_level(void)
{
    return (log_level_t)s_runtime_level;
}

static const char* level_to_str(log_level_t lvl)
{
    switch (lvl)
    {
    case LOG_LVL_DEBUG: return "DBG";
    case LOG_LVL_INFO:  return "INF";
    case LOG_LVL_WARN:  return "WRN";
    case LOG_LVL_ERROR: return "ERR";
    case LOG_LVL_FATAL: return "FTL";
    default:            return "UNK";
    }
}

void log_printf(log_level_t lvl,
                const char* tag,
                const char* file,
                int line,
                const char* fmt, ...)
{
    uint32_t tick = HAL_GetTick();

    // ヘッダ
    printf("[%06lu][%s]",
           (unsigned long)tick,
           level_to_str(lvl));

    if (tag != NULL) {
        printf("[%s]", tag);
    } else {
        printf("[--]");
    }

    if (file != NULL) {
        printf("[%s:%d] ", file, line);
    } else {
        printf("[?:?] ");
    }

    // 本文
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);

    printf("\n");
}




















// // --- include ---
// #include <stdio.h>
// #include <stdarg.h>
// 
// #include "utils/log.h"
// #include "stm32h7xx_hal.h"
// 
// 
// // --- define ---
// // --- typedef ---
// // --- variable declaration ---
// static volatile log_level_t s_runtime_level = LOG_LVL_DEBUG;
// 
// 
// // --- function prototype ---
// static const char* level_to_str(log_level_t lvl);
// static int runtime_allowed(log_level_t lvl);
// 
// 
// // --- functions ---
// void log_init(void)
// {
//     // 将来：リングバッファ初期化など
// }
// 
// void log_set_runtime_level(log_level_t lvl)
// {
//     s_runtime_level = lvl;
// }
// 
// log_level_t log_get_runtime_level(void)
// {
//     return s_runtime_level;
// }
// 
// 
// static int runtime_allowed(log_level_t lvl)
// {
//     return (lvl >= s_runtime_level);
// }
// 
// 
// static const char* level_to_str(log_level_t lvl)
// {
//     switch (lvl)
//     {
//     case LOG_LVL_DEBUG: return "DBG";
//     case LOG_LVL_INFO:  return "INF";
//     case LOG_LVL_WARN:  return "WRN";
//     case LOG_LVL_ERROR: return "ERR";
//     case LOG_LVL_FATAL: return "FTL";
//     default:            return "UNK";
//     }
// }
// 
// 
// void log_printf(log_level_t lvl,
//                 const char* tag,
//                 const char* file,
//                 int line,
//                 const char* fmt, ...)
// {
//     if (!runtime_allowed(lvl)) return;
// 
//     uint32_t tick = HAL_GetTick();
// 
//     // ヘッダ
//     printf("[%06lu][%s]",
//            (unsigned long)tick,
//            level_to_str(lvl));
// 
//     if (tag) {
//         printf("[%s]", tag);
//     } else {
//         printf("[--]");
//     }
// 
//     if (file) {
//         printf("[%s:%d] ", file, line);
//     } else {
//         printf("[?:?] ");
//     }
// 
//     // 本文
//     va_list ap;
//     va_start(ap, fmt);
//     vprintf(fmt, ap);
//     va_end(ap);
// 
//     printf("\n");
// }
// 