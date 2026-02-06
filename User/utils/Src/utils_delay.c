/***************************************************************
 * FILENAME    : utils_delay.c
 * DESCRIPTION : Generic delay utilities (busy wait helpers)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "utils_delay.h"

void util_busy_delay(uint32_t n)
{
    while (n--) {
        __asm volatile ("nop");
    }
}
