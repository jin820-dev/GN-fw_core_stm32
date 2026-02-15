/***************************************************************
 * FILENAME    : app_adc_log.h
 * DESCRIPTION : ADC DMA capture helper for CSV logging and evaluation
 * AUTHOR      : jin
 * DATE        : 2026/02/14
 ****************************************************************/


#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t min;
    uint16_t max;
    uint32_t sum;
    uint32_t n;
} app_adc_stats_t;

/**
 * @brief ADC DMAの「前半」が揃ったら1回だけCSVを出力して停止するタスク
 * @return true: 1回吐き終わった / false: まだ
 */
bool app_adc_log_once_on_half_ready(app_adc_stats_t* out_stats);
