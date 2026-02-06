/***************************************************************
 * FILENAME    : app_panic.h
 * DESCRIPTION : Application panic interface (public API)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include <stdint.h>

typedef enum {
  APP_PANIC_HAL_ERROR = 1,
} app_panic_reason_t;

void app_panic(app_panic_reason_t reason);
