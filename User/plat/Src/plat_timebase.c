/***************************************************************
 * FILENAME    : plat_timebase.c
 * DESCRIPTION : Platform timebase implementation (HAL tick wrapper)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "plat_timebase.h"
#include "stm32h7xx_hal.h"

void plat_timebase_init(void)
{
    // NOP（HAL_Init() が SysTick を用意する前提）
}

uint32_t plat_millis(void)
{
    return HAL_GetTick();
}

void plat_delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
