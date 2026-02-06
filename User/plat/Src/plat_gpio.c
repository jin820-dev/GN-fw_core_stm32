/***************************************************************
 * FILENAME    : plat_gpio.c
 * DESCRIPTION : Platform GPIO implementation (LED control mapping)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "plat_gpio.h"
#include "main.h"
#include "stm32h7xx_hal.h"

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t      pin;
} plat_gpio_pin_t;


/* ===== ボード依存テーブル ===== */
static const plat_gpio_pin_t led_table[PLAT_LED_MAX] =
{
    [PLAT_LED_GREEN]  = { GREEN_LED_GPIO_Port,  GREEN_LED_Pin  },
    [PLAT_LED_RED]    = { RED_LED_GPIO_Port,    RED_LED_Pin    },
    [PLAT_LED_YELLOW] = { YELLOW_LED_GPIO_Port, YELLOW_LED_Pin }
};


void plat_gpio_init(void)
{
    // CubeMXが初期化するので何もしない
}


void plat_led_set(plat_led_t led, bool on)
{
    if (led >= PLAT_LED_MAX) return;

    HAL_GPIO_WritePin(
        led_table[led].port,
        led_table[led].pin,
        on ? GPIO_PIN_SET : GPIO_PIN_RESET
    );
}


void plat_led_toggle(plat_led_t led)
{
    if (led >= PLAT_LED_MAX) return;

    HAL_GPIO_TogglePin(
        led_table[led].port,
        led_table[led].pin
    );
}
