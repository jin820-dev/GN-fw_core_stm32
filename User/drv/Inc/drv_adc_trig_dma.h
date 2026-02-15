/***************************************************************
 * FILENAME    : drv_adc_trig_dma.h
 * DESCRIPTION : ADC trigger by TIM6 + DMA circular driver
 * AUTHOR      : jin
 * DATE        : 2026/02/12
 ****************************************************************/
 
 
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define DRV_ADC_TRIG_DMA_BUF_SIZE 4096u

typedef enum {
    DRV_ADC_TRIG_DMA_OK = 0,
    DRV_ADC_TRIG_DMA_ERR_NOT_INIT,
    DRV_ADC_TRIG_DMA_ERR_ALREADY_RUNNING,
    DRV_ADC_TRIG_DMA_ERR_HAL,
} drv_adc_trig_dma_status_t;

drv_adc_trig_dma_status_t drv_adc_trig_dma_init(void);
drv_adc_trig_dma_status_t drv_adc_trig_dma_start(void);
drv_adc_trig_dma_status_t drv_adc_trig_dma_stop(void);

const uint16_t* drv_adc_trig_dma_get_buffer(void);
uint32_t drv_adc_trig_dma_get_buffer_size(void);
bool drv_adc_trig_dma_is_running(void);

/**
 * @brief DMA half-transfer event occurred?
 * @return true if occurred since last poll (and clears the flag)
 */
bool drv_adc_trig_dma_poll_half(void);

/**
 * @brief DMA full-transfer event occurred?
 * @return true if occurred since last poll (and clears the flag)
 */
bool drv_adc_trig_dma_poll_full(void);
