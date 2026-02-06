/***************************************************************
 * FILENAME    : plat_sd_spi.c
 * DESCRIPTION : SD card driver (SPI mode)
 * AUTHOR      : jin
 * DATE        : 2026/02/01
 ****************************************************************/


// --- include ---
#include "plat_sd_spi.h"

#include "plat_spi_bus.h"
#include "plat_timebase.h"
#include "utils_log.h"


// --- define ---
#define TAG "SD"
// log.h 側に LOG_DEBUG が無い場合の保険（無ければ INFO 扱いにする）
#ifndef LOG_DEBUG
#define LOG_DEBUG(tag, fmt, ...) LOG_INFO(tag, fmt, ##VA_ARGS)
#endif

// SDカードコマンド（SPI mode）
#define SD_CMD0   (0)
#define SD_CMD8   (8)
#define SD_CMD17  (17)
#define SD_CMD24  (24)
#define SD_CMD55  (55)
#define SD_ACMD41 (41)
#define SD_CMD58  (58)


// --- typedef ---
// --- variable declaration ---
// --- function prototype ---
static void sd_clock_idle(uint32_t nbytes);
static uint8_t sd_r1_wait(uint32_t timeout_ms);
static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc);
static bool sd_wait_data_token(uint8_t token, uint32_t timeout_ms);
static bool sd_wait_not_busy(uint32_t timeout_ms);

// --- functions
static void sd_clock_idle(uint32_t nbytes)
{
    // CS=Highでダミークロック（最低74clks）
    plat_spi_sd_cs(true);
    for (uint32_t i=0;i<nbytes;i++) {
        (void)plat_spi_txrx_u8(0xFF);
    }
}

// -------------------------------------------------- //

static uint8_t sd_r1_wait(uint32_t timeout_ms)
{
    uint32_t start = plat_millis();
    uint8_t r1;
    do {
        r1 = plat_spi_txrx_u8(0xFF);
        if ((r1 & 0x80) == 0) return r1;
    } while ((plat_millis() - start) < timeout_ms);

    return 0xFF; // timeout
}

// -------------------------------------------------- //

static uint8_t sd_send_cmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    // CS Low中に送る
    uint8_t pkt[6];
    pkt[0] = 0x40 | cmd;
    pkt[1] = (uint8_t)(arg >> 24);
    pkt[2] = (uint8_t)(arg >> 16);
    pkt[3] = (uint8_t)(arg >> 8);
    pkt[4] = (uint8_t)(arg);
    pkt[5] = crc;

    // コマンド前に1byte空転
    (void)plat_spi_txrx_u8(0xFF);

    plat_spi_txrx(pkt, 0, 6);

    // R1待ち（カード状態によっては少し時間がかかる）
    return sd_r1_wait(1000);
}

// -------------------------------------------------- //

static bool sd_wait_data_token(uint8_t token, uint32_t timeout_ms)
{
    uint32_t start = plat_millis();
    uint8_t b = 0xFF;
    do {
        b = plat_spi_txrx_u8(0xFF);
        if (b == token){
             return true;
        }
    } while ((plat_millis() - start) < timeout_ms);
    return false;
}

// -------------------------------------------------- //

static bool sd_wait_not_busy(uint32_t timeout_ms)
{
    uint32_t start = plat_millis();
    while ((plat_millis() - start) < timeout_ms) {
        if (plat_spi_txrx_u8(0xFF) == 0xFF){
            return true;
        }
    }
    return false;
}

// -------------------------------------------------- //

sd_err_t sd_spi_init_card(void)
{
    // 低速で開始
    plat_spi_set_speed(PLAT_SPI_SPEED_SLOW);

    // CS=Highのままダミークロック（80clks）
    sd_clock_idle(10);

    // CMD0: GO_IDLE_STATE
    plat_spi_sd_cs(false);
    uint8_t r1 = sd_send_cmd(SD_CMD0, 0, 0x95);
    plat_spi_sd_cs(true);
    (void)plat_spi_txrx_u8(0xFF);
    
    LOG_DEBUG(TAG, "CMD0 r1=0x%02X", r1);
    if (r1 != 0x01) {
        LOG_ERROR(TAG, "CMD0 failed r1=0x%02X", r1);
        return SD_ERR_R1;
    }

    // CMD8: voltage check
    plat_spi_sd_cs(false);
    r1 = sd_send_cmd(SD_CMD8, 0x000001AA, 0x87);

    uint8_t r7[4] = {0};
    for (int i=0;i<4;i++){
        r7[i] = plat_spi_txrx_u8(0xFF);
    }
    
    plat_spi_sd_cs(true);
    (void)plat_spi_txrx_u8(0xFF);
    
    LOG_DEBUG(TAG, "CMD8 r1=0x%02X r7=%02X %02X %02X %02X", r1, r7[0], r7[1], r7[2], r7[3]);
    if (r1 != 0x01) {
        LOG_ERROR(TAG, "CMD8 failed r1=0x%02X", r1);
        return SD_ERR_R1;
    }
    if ((r7[2] != 0x01) || (r7[3] != 0xAA)) {
        LOG_ERROR(TAG, "CMD8 param mismatch r7=%02X %02X %02X %02X", r7[0], r7[1], r7[2], r7[3]);
        return SD_ERR_PARAM;
    }

    // ACMD41 loop
    uint32_t start = plat_millis();
    do {
        // CMD55
        plat_spi_sd_cs(false);
        r1 = sd_send_cmd(SD_CMD55, 0, 0x01);
        plat_spi_sd_cs(true);
        (void)plat_spi_txrx_u8(0xFF);
        
        LOG_DEBUG(TAG, "CMD55 r1=0x%02X", r1);
        if (r1 > 0x01) {
            LOG_ERROR(TAG, "CMD55 failed r1=0x%02X", r1);
            return SD_ERR_R1;
        }

        // ACMD41 (HCS=1)
        plat_spi_sd_cs(false);
        r1 = sd_send_cmd(SD_ACMD41, 0x40000000, 0x01); // HCS=1
        plat_spi_sd_cs(true);
        (void)plat_spi_txrx_u8(0xFF);
        
        LOG_DEBUG(TAG, "ACMD41 r1=0x%02X", r1);
        
        if (r1 == 0x00) {
            break;
        }
    
    } while ((plat_millis() - start) < 2000);



    if (r1 != 0x00) {
        LOG_ERROR(TAG, "ACMD41 timeout r1=0x%02X", r1);
        return SD_ERR_TIMEOUT;
    }

    // CMD58: OCR read（SDHC確認）
    plat_spi_sd_cs(false);
    r1 = sd_send_cmd(SD_CMD58, 0, 0x01);
    
    uint8_t ocr[4] = {0};
    for (int i = 0; i < 4; i++) {
        ocr[i] = plat_spi_txrx_u8(0xFF);
    }
    
    plat_spi_sd_cs(true);
    (void)plat_spi_txrx_u8(0xFF);
    
    LOG_DEBUG(TAG, "CMD58 r1=0x%02X ocr=%02X %02X %02X %02X", r1, ocr[0], ocr[1], ocr[2], ocr[3]);
    if (r1 != 0x00) {
        LOG_ERROR(TAG, "CMD58 failed r1=0x%02X", r1);
        return SD_ERR_R1;
    }

    // 高速へ（まず控えめでOK）
    //plat_spi_set_speed(PLAT_SPI_SPEED_FAST);

    LOG_INFO(TAG, "init OK");
    return SD_OK;
}

// -------------------------------------------------- //

sd_err_t sd_spi_read_block(uint32_t lba, uint8_t* buf512)
{
    if (!buf512) {
        LOG_ERROR(TAG, "read param error (buf=NULL)");
        return SD_ERR_PARAM;
    }

    // CS=Highで80clocks（カードをidleに戻す）
    plat_spi_sd_cs(true);
    for (int i=0;i<10;i++) {
        (void)plat_spi_txrx_u8(0xFF);
    }
    
    plat_spi_sd_cs(false);
    
    uint8_t r1 = sd_send_cmd(SD_CMD17, lba, 0x01);
    LOG_DEBUG(TAG, "CMD17 lba=%lu r1=0x%02X", (unsigned long)lba, r1);
    
    if (r1 != 0x00) {
        plat_spi_sd_cs(true);
        LOG_ERROR(TAG, "read CMD17 failed lba=%lu r1=0x%02X", (unsigned long)lba, r1);
        return SD_ERR_R1;
    }

    if (!sd_wait_data_token(0xFE, 200)) {
        plat_spi_sd_cs(true);
        LOG_ERROR(TAG, "read token timeout lba=%lu", (unsigned long)lba);
        return SD_ERR_TOKEN;
    }

    // 512B + CRC16(2)
    for (int i=0;i<512;i++) {
        buf512[i] = plat_spi_txrx_u8(0xFF);
    }
    (void)plat_spi_txrx_u8(0xFF);
    (void)plat_spi_txrx_u8(0xFF);

    plat_spi_sd_cs(true);
    (void)plat_spi_txrx_u8(0xFF);
    
    return SD_OK;
}

// -------------------------------------------------- //

sd_err_t sd_spi_write_block(uint32_t lba, const uint8_t* buf512)
{
    if (!buf512) {
         LOG_ERROR(TAG, "write param error (buf=NULL)");
         return SD_ERR_PARAM;
    }

    plat_spi_sd_cs(false);
    
    uint8_t r1 = sd_send_cmd(SD_CMD24, lba, 0x01);
    LOG_DEBUG(TAG, "CMD24 lba=%lu r1=0x%02X", (unsigned long)lba, r1);
    
    if (r1 != 0x00) {
        plat_spi_sd_cs(true);
        LOG_ERROR(TAG, "write CMD24 failed lba=%lu r1=0x%02X", (unsigned long)lba, r1);
        return SD_ERR_R1;
    }

    // Start token
    (void)plat_spi_txrx_u8(0xFF);
    (void)plat_spi_txrx_u8(0xFE);

    for (int i=0;i<512;i++) {
        (void)plat_spi_txrx_u8(buf512[i]);
    }

    // Dummy CRC
    (void)plat_spi_txrx_u8(0xFF);
    (void)plat_spi_txrx_u8(0xFF);

    // Data response token: 0bxxx0_0101 accepted
    uint8_t resp = plat_spi_txrx_u8(0xFF);
    if ((resp & 0x1F) != 0x05) {
        plat_spi_sd_cs(true);
        LOG_ERROR(TAG, "write rejected resp=0x%02X", resp);
        return SD_ERR_R1;
    }

    // Busy wait
    if (!sd_wait_not_busy(500)) {
        plat_spi_sd_cs(true);
        LOG_ERROR(TAG, "write busy timeout");
        return SD_ERR_TIMEOUT;
    }

    plat_spi_sd_cs(true);
    (void)plat_spi_txrx_u8(0xFF);
    
    return SD_OK;
}
