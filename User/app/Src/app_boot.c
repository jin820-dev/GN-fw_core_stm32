/***************************************************************
 * FILENAME    : app_boot.c
 * DESCRIPTION : Application boot sequence (POST / init / health check)
 * AUTHOR      : jin
 * DATE        : 2026/02/01
 ****************************************************************/


// --- include ---
#include "app_boot.h"

#include "i2c.h"
#include "app_logger.h"
#include "drv_oled_i2c.h"
#include "drv_sd.h"
#include "utils_log.h"
#include "utils_sd_selftest.h"
#include "plat_uart_stdio.h"
#include "utils_i2c_scan.h"


// --- define ---
#define TAG "BOOT"


// --- variable declaration ---
bool logger_ok = false;
bool fat_ok = false;

// --- functions ---
void app_boot(void)
{
    plat_uart_stdio_init();
    LOG_INFO(TAG, "boot begin");
    
    utils_i2c_scan(&hi2c1);
  
    // 1) SDセルフテスト（短時間で終わる版）
    sd_selftest_result_t sd = sd_selftest_run();
    
    // 1-1) state of sd mount
    if (drv_sd_mount() == true) {
        fat_ok = true;
        LOG_INFO("BOOT", "fatfs mount OK (boot)");
    } else {
        LOG_ERROR("BOOT", "fatfs mount FAIL (boot)");
    }

    // 2) OLED（無ければ無言でスキップできる設計になっている前提）
    (void)oled_init();
    oled_clear();
    oled_draw_str(0, 0, "GN-beluga");

    if (sd.sd_init_ok) {
        oled_draw_str(0, 16, "SD OK");
    } else {
        oled_draw_str(0, 16, "SD NG");
    }
    
    oled_update();
    
    
    // 3) 追加の初期化があればここに集約していく
    if (fat_ok == true) {
        if (logger_init() == LOGGER_OK) {
            LOG_INFO("BOOT", "logger init OK");
            logger_ok = true;
        } else {
            LOG_ERROR("BOOT", "logger init FAIL");
        }
    }
    // adc_init();
    // ...

    LOG_INFO("BOOT", "POST: SD=%s FAT=%s LOGGER=%s OLED=%s",
             sd.sd_init_ok ? "OK" : "NG",
             sd.fatfs_ok? "OK" : "NG",
             logger_ok ? "OK" : "NG",
             oled_is_available() ? "OK" : "NG");

    LOG_INFO(TAG, "boot end");
}

