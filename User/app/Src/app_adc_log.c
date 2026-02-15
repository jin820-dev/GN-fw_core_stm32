/***************************************************************
 * FILENAME    : app_adc_log.c
 * DESCRIPTION : ADC DMA capture helper for CSV logging and evaluation
 * AUTHOR      : jin
 * DATE        : 2026/02/14
 ****************************************************************/


#include "app_adc_log.h"

#include "drv_adc_trig_dma.h"
#include <stdio.h>

#define CSV_N   (DRV_ADC_TRIG_DMA_BUF_SIZE/2)

static uint16_t s_csv_buf[CSV_N];

static void calc_stats(app_adc_stats_t* st, const uint16_t* p, uint32_t n)
{
    uint16_t mn = 0xFFFF, mx = 0;
    uint32_t sum = 0;

    for (uint32_t i = 0; i < n; i++) {
        uint16_t v = p[i];
        if (v < mn) mn = v;
        if (v > mx) mx = v;
        sum += v;
    }

    st->min = mn;
    st->max = mx;
    st->sum = sum;
    st->n   = n;
}

static void dump_csv_u16(const uint16_t* p, uint32_t n)
{
    for (uint32_t i = 0; i < n; i++) {
        printf("%u\r\n", (unsigned)p[i]);
    }
}

bool app_adc_log_once_on_half_ready(app_adc_stats_t* out_stats)
{
    static bool s_done = false;
    if (s_done) return true;


    if (!drv_adc_trig_dma_poll_half()) {
        return false;
    }

    // 前半2048点をスナップショット
    const uint16_t* buf = drv_adc_trig_dma_get_buffer();
    for (uint32_t i = 0; i < CSV_N; i++) {
        s_csv_buf[i] = buf[i];
    }

    if (out_stats) {
        calc_stats(out_stats, s_csv_buf, CSV_N);
    }

    dump_csv_u16(s_csv_buf, CSV_N);

    s_done = true;
    return true;
}
