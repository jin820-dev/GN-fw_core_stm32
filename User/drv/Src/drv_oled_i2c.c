/***************************************************************
 * FILENAME    : drv_oled_i2c.c
 * DESCRIPTION : OLED(SSD1306) I2C driver wrapper
 *               Provides simple display control API for app layer
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


// --- include ---
#include "i2c.h"
#include "utils_log.h"

#include "drv_oled_i2c.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"


// --- define ---
#define TAG "OLED"

#ifndef OLED_I2C_ADDR_7BIT
#define OLED_I2C_ADDR_7BIT  (0x3C)
#endif

#ifndef OLED_I2C_TIMEOUT_MS
#define OLED_I2C_TIMEOUT_MS (50)
#endif


// --- typedef ---
// --- variable declaration ---
//extern I2C_HandleTypeDef hi2c1;
static bool s_oled_available = false;

// --- function prototype ---
static bool i2c_probe_oled(void);

// --- functions ---
static bool i2c_probe_oled(void)
{
    return (HAL_I2C_IsDeviceReady(&hi2c1,
                                  (OLED_I2C_ADDR_7BIT << 1),
                                  2,
                                  OLED_I2C_TIMEOUT_MS) == HAL_OK);
}

bool oled_is_available(void)
{
    return s_oled_available;
}

bool oled_init(void)
{
    LOG_INFO(TAG, "init begin (addr=0x%02X)", OLED_I2C_ADDR_7BIT);
    if (!i2c_probe_oled()) {
        if (s_oled_available) {
            s_oled_available = false;
            LOG_WARN(TAG, "device lost -> disabled");
        } else {
            LOG_WARN(TAG, "not found -> disabled (skip ssd1306_Init)");
        }
        return false;
    }
    
    // ここから先は「見つかった」ので初期化を実行
    // ※もしssd1306_Init()の内部がHAL_MAX_DELAYで詰まる実装なら、
    // 3rd-party側のI2C送信timeout化が必要（必要になったら一緒に潰そう）
    
    ssd1306_Init();
    s_oled_available = true;

    LOG_INFO(TAG, "init OK.");
    return true;
}

void oled_clear(void)
{
    if (!s_oled_available){
         return;
    }
    ssd1306_Fill(Black);
}

void oled_update(void)
{
    if (!s_oled_available) {
        return;
    }
    ssd1306_UpdateScreen();
}

void oled_draw_str(uint8_t x, uint8_t y, const char* s)
{
    if (!s_oled_available) {
        return;
    }
    
    if (s == NULL) {
        LOG_WARN(TAG, "oled_draw_str: NULL string (ignored)");
        return;
    }
    
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString((char*)s, Font_7x10, White);
}
