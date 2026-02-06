/***************************************************************
 * FILENAME    : plat_gpio.h
 * DESCRIPTION : Platform GPIO abstraction (LED control mapping)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#ifndef PLAT_GPIO_H_
#define PLAT_GPIO_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    PLAT_LED_GREEN = 0,
    PLAT_LED_RED,
    PLAT_LED_YELLOW,
    PLAT_LED_MAX
} plat_led_t;

void plat_gpio_init(void);

void plat_led_set(plat_led_t led, bool on);
void plat_led_toggle(plat_led_t led);

#ifdef __cplusplus
}
#endif

#endif /* PLAT_GPIO_H_ */
