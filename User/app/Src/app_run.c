/***************************************************************
 * FILENAME    : app_run.c
 * DESCRIPTION : Main application loop implementation
 * AUTHOR      : jin
 * DATE        : 2026/02/14
 ****************************************************************/


#include "app_run.h"
#include "app_adc_log.h"
#include "utils_log.h"
#include "plat_gpio.h"
#include "plat_timebase.h"
#include "cmsis_gcc.h"

#define TAG "APP"

void app_run_forever(void)
{
  LOG_INFO(TAG, "while loop start");  
  app_adc_stats_t st = {0};
  
  while (1)
  {
        //// L chika
        // plat_led_set(PLAT_LED_GREEN, true);
        // plat_delay_ms(250);
        // plat_led_toggle(PLAT_LED_RED);
        // plat_led_toggle(PLAT_LED_YELLOW);
        // plat_led_set(PLAT_LED_GREEN, false);
        // plat_delay_ms(250);
        
        //// append csv log (debug)
        //(void)logger_append(1234U, 1.234f, 5.678f, "boot");

        //// get adc values
        //uint32_t half = drv_adc_trig_dma_get_half_count();
        //uint32_t full = drv_adc_trig_dma_get_full_count();
        //// half完了を検出 → 前半2048点を評価
        //if (half != prev_half)
        //{
        //    prev_half = half;
        //    const uint16_t* buf = drv_adc_trig_dma_get_buffer();
        //    calc_stats(&buf[0], DRV_ADC_TRIG_DMA_BUF_SIZE/2);
        //    __NOP(); // ここで止めて g_min/g_max を見る
        //}
        //
        //// full完了を検出 → 後半2048点を評価
        //if (full != prev_full)
        //{
        //    prev_full = full;
        //    const uint16_t* buf = drv_adc_trig_dma_get_buffer();
        //    calc_stats(&buf[DRV_ADC_TRIG_DMA_BUF_SIZE/2], DRV_ADC_TRIG_DMA_BUF_SIZE/2);
        //    __NOP();
        //}
        
        // store csv log
        // 1回だけCSV吐き（吐いたら止める）
        if (app_adc_log_once_on_half_ready(&st)) {
            LOG_INFO(TAG, "ADC CSV done. min=%u max=%u n=%lu",
                     (unsigned)st.min, (unsigned)st.max, (unsigned long)st.n);

            while (1) { __NOP(); }
        }
        
    }
}
