/***************************************************************
 * FILENAME    : utils_sd_selftest.h
 * DESCRIPTION : SD card quick self-test utility (SPI + FatFs)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/



#ifndef UTILS_SD_SELFTEST_H_
#define UTILS_SD_SELFTEST_H_

// --- include ---
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif


// --- define ---
// --- typedef ---
typedef struct
{
    bool sd_init_ok;
    bool fatfs_ok;
    bool raw_ok;

} sd_selftest_result_t;


// --- function prototype ---
/**
 * @brief 起動時セルフテスト（短時間で終わる版）
 *
 * - SD init
 * - FatFs mount + ファイル read/write
 * - （任意）生LBA read/write/verify（破壊の可能性があるので通常OFF）
 */
sd_selftest_result_t sd_selftest_run(void);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_SD_SELFTEST_H_ */
