/***************************************************************
 * FILENAME    : plat_spi_bus.c
 * DESCRIPTION : Platform SPI bus implementation (SD card CS & speed)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "plat_spi_bus.h"
#include "main.h"
#include "stm32h7xx_hal.h"

extern SPI_HandleTypeDef hspi1;

// SDのCSピン（main.h定義を使う）
static inline void sd_cs_high(void){ HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET); }
static inline void sd_cs_low(void) { HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET); }

void plat_spi_bus_init(void)
{
    // CubeMXのMX_SPI1_Init() / MX_GPIO_Init()が完了している前提
    sd_cs_high();
}

void plat_spi_sd_cs(bool level)
{
    if (level) sd_cs_high();
    else       sd_cs_low();
}

bool plat_spi_set_speed(plat_spi_speed_t speed)
{
    // H7: SPIのBaudRatePrescalerを書き換えるにはDisable→設定→Enableが安全
    // ここは「とりあえず動かす」ので単純にやる（後で洗練可）

    HAL_SPI_DeInit(&hspi1);

    if (speed == PLAT_SPI_SPEED_SLOW) {
        // 初期化は低速（例：Prescaler 256 / 128などに）
        hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    } else {
        // 通常動作は高速（まずは控えめに 8〜16 あたりから）
        hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
        
    }

    return (HAL_SPI_Init(&hspi1) == HAL_OK);
}

uint8_t plat_spi_txrx_u8(uint8_t tx)
{
    uint8_t rx = 0xFF;
    (void)HAL_SPI_TransmitReceive(&hspi1, &tx, &rx, 1, HAL_MAX_DELAY);
    return rx;
}

bool plat_spi_txrx(const uint8_t* tx, uint8_t* rx, size_t len)
{
    // 送信だけ/受信だけでも動くように、0xFF埋めを用意
    static uint8_t dummy_tx[32];
    static uint8_t dummy_rx[32];

    if (len == 0) return true;

    // lenが大きい場合はチャンク分割（簡易）
    size_t pos = 0;
    while (pos < len) {
        size_t n = (len - pos > sizeof(dummy_tx)) ? sizeof(dummy_tx) : (len - pos);

        const uint8_t* ptx = tx ? (tx + pos) : dummy_tx;
        uint8_t* prx = rx ? (rx + pos) : dummy_rx;

        if (!tx) { for (size_t i=0;i<n;i++) dummy_tx[i]=0xFF; }

        if (HAL_SPI_TransmitReceive(&hspi1, (uint8_t*)ptx, prx, (uint16_t)n, HAL_MAX_DELAY) != HAL_OK)
            return false;

        pos += n;
    }
    return true;
}
