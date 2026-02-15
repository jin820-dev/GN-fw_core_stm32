/***************************************************************
 * FILENAME    : drv_adc_trig_dma.c
 * DESCRIPTION : ADC trigger by TIM6 + DMA circular driver
 * AUTHOR      : jin
 * DATE        : 2026/02/12
 ****************************************************************/


#include "drv_adc_trig_dma.h"

#include "adc.h"
#include "tim.h"

static uint16_t s_buf[DRV_ADC_TRIG_DMA_BUF_SIZE] __attribute__((aligned(32)));

static bool s_init = false;
static bool s_running = false;

/* DMA event flags (set in HAL callbacks) */
static volatile bool s_half_ready = false;
static volatile bool s_full_ready = false;

// ------------------------------------------------------------- //

drv_adc_trig_dma_status_t drv_adc_trig_dma_init(void)
{
    /* Future: calibration options, parameter checks, etc. */
    if (HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED) != HAL_OK) {
        return DRV_ADC_TRIG_DMA_ERR_HAL;
    }

    s_init = true;
    return DRV_ADC_TRIG_DMA_OK;
}

// ------------------------------------------------------------- //

drv_adc_trig_dma_status_t drv_adc_trig_dma_start(void)
{
    if (!s_init) return DRV_ADC_TRIG_DMA_ERR_NOT_INIT;
    if (s_running) return DRV_ADC_TRIG_DMA_ERR_ALREADY_RUNNING;

    s_half_ready = false;
    s_full_ready = false;

    if (HAL_TIM_Base_Start(&htim6) != HAL_OK) return DRV_ADC_TRIG_DMA_ERR_HAL;

    if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)s_buf, DRV_ADC_TRIG_DMA_BUF_SIZE) != HAL_OK) {
        (void)HAL_TIM_Base_Stop(&htim6);
        return DRV_ADC_TRIG_DMA_ERR_HAL;
    }

    s_running = true;
    return DRV_ADC_TRIG_DMA_OK;
}

// ------------------------------------------------------------- //

drv_adc_trig_dma_status_t drv_adc_trig_dma_stop(void)
{
    if (!s_init) return DRV_ADC_TRIG_DMA_ERR_NOT_INIT;
    if (!s_running) return DRV_ADC_TRIG_DMA_OK;

    if (HAL_ADC_Stop_DMA(&hadc1) != HAL_OK) return DRV_ADC_TRIG_DMA_ERR_HAL;
    if (HAL_TIM_Base_Stop(&htim6) != HAL_OK) return DRV_ADC_TRIG_DMA_ERR_HAL;

    s_running = false;
    return DRV_ADC_TRIG_DMA_OK;
}

// ------------------------------------------------------------- //

const uint16_t* drv_adc_trig_dma_get_buffer(void)
{
    return s_buf;
}

// ------------------------------------------------------------- //

uint32_t drv_adc_trig_dma_get_buffer_size(void)
{
    return DRV_ADC_TRIG_DMA_BUF_SIZE;
}

// ------------------------------------------------------------- //

bool drv_adc_trig_dma_is_running(void)
{
    return s_running;
}

// ------------------------------------------------------------- //

bool drv_adc_trig_dma_poll_half(void)
{
    if (s_half_ready) {
        s_half_ready = false;
        return true;
    }
    return false;
}

// ------------------------------------------------------------- //

bool drv_adc_trig_dma_poll_full(void)
{
    if (s_full_ready) {
        s_full_ready = false;
        return true;
    }
    return false;
}

// ------------------------------------------------------------- //

/* HAL callbacks: keep minimal and local behavior */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1) {
        s_half_ready = true;
    }
}

// ------------------------------------------------------------- //

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1) {
        s_full_ready = true;
    }
}
