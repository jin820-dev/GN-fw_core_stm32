/***************************************************************
 * FILENAME    : app_panic.c
 * DESCRIPTION : Application panic handler (fatal error loop)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "app_panic.h"
#include "stm32h7xx.h"
#include "plat_gpio.h"
#include "plat_timebase.h"

void app_panic(app_panic_reason_t reason)
{
  (void)reason;
  __disable_irq();

  while (1) {
    plat_led_toggle(PLAT_LED_RED);
    plat_delay_ms(250);
  }
}
