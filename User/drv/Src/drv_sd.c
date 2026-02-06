/***************************************************************
 * FILENAME    : drv_sd.c
 * DESCRIPTION : SD card driver (mount / filesystem / disk control)
 *               Wraps FatFs + SPI platform layer
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


// --- include ---
#include "drv_sd.h"

#include <stdio.h>
#include "ff.h"

// --- define ---
#define DRV_SD_MOUNT_PATH  ""

// --- variable declaration ---
static FATFS s_fs;
static bool s_mounted = false;

// --- functions ---
bool drv_sd_init(void)
{
    // 現状の構成（user_diskio + sd_spi）ではここは最小でOK
    return true;
}

// ----------------------------------------------------------- //

bool drv_sd_mount(void)
{
    FRESULT fr = f_mount(&s_fs, DRV_SD_MOUNT_PATH, 1);
    if (fr != FR_OK) {
        printf("[SD] f_mount failed: %d\r\n", (int)fr);
        return false;
    }

    s_mounted = true;
    return true;
}

// ----------------------------------------------------------- //

bool drv_sd_unmount(void)
{
    FRESULT fr = f_mount(NULL, DRV_SD_MOUNT_PATH, 1);
    if (fr != FR_OK) {
        printf("[SD] unmount failed: %d\r\n", (int)fr);
        return false;
    }

    s_mounted = false;
    return true;
}

// ----------------------------------------------------------- //

bool drv_sd_write_file(const char* path, const uint8_t* buf, uint32_t size)
{
    FRESULT fr;
    FIL fil;
    UINT bw = 0U;

    if ((s_mounted == false) || (path == NULL) || (buf == NULL)) {
        return false;
    }

    fr = f_open(&fil, path, FA_CREATE_ALWAYS | FA_WRITE);
    if (fr != FR_OK) {
        printf("[SD] f_open failed: %s (%d)\r\n", path, (int)fr);
        return false;
    }

    fr = f_write(&fil, buf, (UINT)size, &bw);
    if ((fr != FR_OK) || (bw != (UINT)size)) {
        printf("[SD] f_write failed: %s fr=%d bw=%lu/%lu\r\n",
               path, (int)fr, (unsigned long)bw, (unsigned long)size);
        (void)f_close(&fil);
        return false;
    }

    fr = f_close(&fil);
    if (fr != FR_OK) {
        printf("[SD] f_close failed: %s (%d)\r\n", path, (int)fr);
        return false;
    }

    return true;
}

// ----------------------------------------------------------- //

bool drv_sd_append_file(const char* path, const uint8_t* buf, uint32_t size)
{
    FRESULT fr;
    FIL fil;
    UINT bw = 0U;

    if ((s_mounted == false) || (path == NULL) || (buf == NULL)) {
        return false;
    }

    fr = f_open(&fil, path, FA_OPEN_ALWAYS | FA_WRITE);
    if (fr != FR_OK) {
        printf("[SD] f_open(append) failed: %s (%d)\r\n", path, (int)fr);
        return false;
    }

    fr = f_lseek(&fil, f_size(&fil));
    if (fr != FR_OK) {
        printf("[SD] f_lseek failed: %s (%d)\r\n", path, (int)fr);
        (void)f_close(&fil);
        return false;
    }

    fr = f_write(&fil, buf, (UINT)size, &bw);
    if ((fr != FR_OK) || (bw != (UINT)size)) {
        printf("[SD] append failed: %s fr=%d bw=%lu/%lu\r\n",
               path, (int)fr, (unsigned long)bw, (unsigned long)size);
        (void)f_close(&fil);
        return false;
    }

    fr = f_close(&fil);
    if (fr != FR_OK) {
        printf("[SD] f_close failed: %s (%d)\r\n", path, (int)fr);
        return false;
    }

    return true;
}


// ----------------------------------------------------------- //

bool drv_sd_read_file(const char* path, uint8_t* buf, uint32_t max_size, uint32_t* out_read_size)
{
    FRESULT fr;
    FIL fil;
    UINT br = 0U;

    if ((s_mounted == false) || (path == NULL) || (buf == NULL) || (out_read_size == NULL)) {
        return false;
    }

    *out_read_size = 0U;

    fr = f_open(&fil, path, FA_READ);
    if (fr != FR_OK) {
        printf("[SD] f_open(read) failed: %s (%d)\r\n", path, (int)fr);
        return false;
    }

    fr = f_read(&fil, buf, (UINT)max_size, &br);
    if (fr != FR_OK) {
        printf("[SD] f_read failed: %s (%d)\r\n", path, (int)fr);
        (void)f_close(&fil);
        return false;
    }

    fr = f_close(&fil);
    if (fr != FR_OK) {
        printf("[SD] f_close failed: %s (%d)\r\n", path, (int)fr);
        return false;
    }

    *out_read_size = (uint32_t)br;
    return true;
}
