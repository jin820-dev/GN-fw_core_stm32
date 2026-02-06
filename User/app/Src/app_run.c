/***************************************************************
 * FILENAME    : app_run.c
 * DESCRIPTION : Main application loop implementation
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "app_run.h"

#include "utils_log.h"
#include "plat_gpio.h"
#include "plat_timebase.h"


#define TAG "APP"

void app_run_forever(void)
{
  LOG_INFO(TAG, "while loop start");  
  while (1)
  {
    plat_led_set(PLAT_LED_GREEN, true);
    plat_delay_ms(250);
    plat_led_toggle(PLAT_LED_RED);
    plat_led_toggle(PLAT_LED_YELLOW);
    plat_led_set(PLAT_LED_GREEN, false);
    plat_delay_ms(250);
    
    //plat_delay_ms(500);
    //(void)logger_append(1234U, 1.234f, 5.678f, "boot");
  }
}

