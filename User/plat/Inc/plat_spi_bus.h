/***************************************************************
 * FILENAME    : plat_spi_bus.h
 * DESCRIPTION : Platform SPI bus abstraction (SD card CS & speed)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#ifndef PLAT_SPI_BUS_H_
#define PLAT_SPI_BUS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PLAT_SPI_SPEED_SLOW = 0,   // 初期化用（100〜400kHz目安）
    PLAT_SPI_SPEED_FAST        // 通常動作用（数MHz〜）
} plat_spi_speed_t;

void plat_spi_bus_init(void);

// CS制御（SD用）
void plat_spi_sd_cs(bool level);   // true=High, false=Low

// SPI速度切替
bool plat_spi_set_speed(plat_spi_speed_t speed);

// 1byte転送
uint8_t plat_spi_txrx_u8(uint8_t tx);

// バッファ転送（tx/rxどちらかNULL可）
bool plat_spi_txrx(const uint8_t* tx, uint8_t* rx, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* PLAT_SPI_BUS_H_ */
