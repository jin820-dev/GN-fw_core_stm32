/***************************************************************
 * FILENAME    : utils_sd_selftest.c
 * DESCRIPTION : SD card quick self-test utility implementation
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


// --- include ---
#include <string.h>

#include "utils_sd_selftest.h"
#include "utils_log.h"
#include "plat_timebase.h"
#include "plat_sd_spi.h"
#include "ff.h"


// --- define ---
#define TAG "SDTST"
// デフォルト方針：安全テスト優先
#define ENABLE_FATFS_TEST    1
// 破壊の可能性があるので通常は 0 推奨
#define ENABLE_RAW_LBA_TEST  0
#define TEST_LBA             (1000U)


// --- typedef ---
// --- variable declaration ---
static uint8_t  s_tx[512];
static uint8_t  s_rx[512];
static FATFS    s_fs;
static FIL      s_fil;


// --- function prototype ---
static bool raw_lba_test(void);
static bool fatfs_test(void);


// --- functions ---
sd_selftest_result_t sd_selftest_run(void)
{
    sd_selftest_result_t r = {0};

    LOG_INFO(TAG, "selftest begin");

    sd_err_t e = sd_spi_init_card();
    if (e != SD_OK) {
        LOG_WARN(TAG, "sd init failed (e=%d) -> skip", (int)e);
        r.sd_init_ok = false;
        return r;
    }

    r.sd_init_ok = true;
    LOG_INFO(TAG, "sd init OK");

#if ENABLE_FATFS_TEST
    r.fatfs_ok = fatfs_test();
#else
    r.fatfs_ok = false;
    LOG_INFO(TAG, "fatfs test disabled");
#endif

#if ENABLE_RAW_LBA_TEST
    r.raw_ok = raw_lba_test();
#else
    r.raw_ok = false;
    LOG_INFO(TAG, "raw LBA test disabled");
#endif

    LOG_INFO(TAG, "selftest end (sd=%d fatfs=%d raw=%d)",
             r.sd_init_ok, r.fatfs_ok, r.raw_ok);

    return r;
}

// -------------------------------------------------------------

static bool raw_lba_test(void)
{
    LOG_WARN(TAG, "RAW LBA test enabled! lba=%lu", (unsigned long)TEST_LBA);

    for (int i = 0; i < 512; i++) {
        s_tx[i] = (uint8_t)i;
    }

    sd_err_t e = sd_spi_write_block(TEST_LBA, s_tx);
    if (e != SD_OK) {
        LOG_ERROR(TAG, "raw write failed e=%d", (int)e);
        return false;
    }

    memset(s_rx, 0, sizeof(s_rx));
    e = sd_spi_read_block(TEST_LBA, s_rx);
    if (e != SD_OK) {
        LOG_ERROR(TAG, "raw read failed e=%d", (int)e);
        return false;
    }

    if (memcmp(s_tx, s_rx, 512) != 0) {
        LOG_ERROR(TAG, "raw verify mismatch");
        return false;
    }

    LOG_INFO(TAG, "raw verify OK");
    return true;
}

// -------------------------------------------------------------

static bool fatfs_test(void)
{
    FRESULT fr;

    LOG_INFO(TAG, "fatfs mount...");
    fr = f_mount(&s_fs, "", 1);
    if (fr != FR_OK) {
        LOG_WARN(TAG, "fatfs mount failed fr=%d", (int)fr);
        return false;
    }
    LOG_INFO(TAG, "fatfs mount OK");

    // write
    LOG_INFO(TAG, "fatfs write...");
    fr = f_open(&s_fil, "hello.txt", FA_CREATE_ALWAYS | FA_WRITE);
    if (fr != FR_OK) {
        LOG_ERROR(TAG, "f_open(W) failed fr=%d", (int)fr);
        return false;
    }

    const char msg[] = "GN-beluga FatFs OK!\r\n";
    UINT bw = 0;
    fr = f_write(&s_fil, msg, (UINT)(sizeof(msg) - 1), &bw);
    (void)f_close(&s_fil);

    if ((fr != FR_OK) || (bw != (UINT)(sizeof(msg) - 1))) {
        LOG_ERROR(TAG, "f_write failed fr=%d bw=%u", (int)fr, (unsigned)bw);
        return false;
    }
    LOG_INFO(TAG, "fatfs write OK");

    // read/verify
    LOG_INFO(TAG, "fatfs read/verify...");
    char buf[64] = {0};
    UINT br = 0;

    fr = f_open(&s_fil, "hello.txt", FA_READ);
    if (fr != FR_OK) {
        LOG_ERROR(TAG, "f_open(R) failed fr=%d", (int)fr);
        return false;
    }

    fr = f_read(&s_fil, buf, (UINT)(sizeof(buf) - 1), &br);
    (void)f_close(&s_fil);

    if (fr != FR_OK) {
        LOG_ERROR(TAG, "f_read failed fr=%d", (int)fr);
        return false;
    }

    if (strcmp(buf, msg) != 0) {
        LOG_ERROR(TAG, "verify mismatch (got=%s)", buf);
        return false;
    }

    LOG_INFO(TAG, "fatfs read/verify OK");
    return true;
}
