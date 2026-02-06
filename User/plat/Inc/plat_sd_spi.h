/***************************************************************
 * FILENAME    : plat_sd_spi.h
 * DESCRIPTION : SD card driver (SPI mode)
 * AUTHOR      : jin
 * DATE        : 2026/02/01
 ****************************************************************/


#ifndef DRV_SD_SPI_H_
#define DRV_SD_SPI_H_

// --- include ---
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


// --- define ---
// --- typedef ---
typedef enum {
    SD_OK = 0,
    SD_ERR_TIMEOUT,
    SD_ERR_R1,
    SD_ERR_TOKEN,
    SD_ERR_PARAM,
} sd_err_t;

// --- function prototype ---
sd_err_t sd_spi_init_card(void);

// 512B ブロックI/O（SDHC前提：ブロックアドレス）
sd_err_t sd_spi_read_block(uint32_t lba, uint8_t* buf512);
sd_err_t sd_spi_write_block(uint32_t lba, const uint8_t* buf512);

#ifdef __cplusplus
}
#endif

#endif /* DRV_SD_SPI_H_ */
