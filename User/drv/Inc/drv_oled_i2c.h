/***************************************************************
 * FILENAME    : drv_oled_i2c.h
 * DESCRIPTION : OLED(SSD1306) I2C driver public interface
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#pragma once

#include <stdbool.h>
#include <stdint.h>

bool oled_init(void);
bool oled_is_available(void);

void oled_clear(void);
void oled_update(void);
void oled_draw_str(uint8_t x, uint8_t y, const char* s);  // まずは文字だけ
