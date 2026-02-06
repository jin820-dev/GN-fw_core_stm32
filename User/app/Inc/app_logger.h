/***************************************************************
 * FILENAME    : app_logger.h
 * DESCRIPTION : CSV logger interface (public API)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#ifndef APP_LOGGER_H_
#define APP_LOGGER_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOGGER_OK = 0,
    LOGGER_ERR_MOUNT,
    LOGGER_ERR_OPEN,
    LOGGER_ERR_WRITE,
    LOGGER_ERR_SYNC,
} logger_err_t;

logger_err_t logger_init(void);
logger_err_t logger_append(uint32_t ms, float ch1, float ch2, const char* note);

#ifdef __cplusplus
}
#endif

#endif /* APP_LOGGER_H_ */
