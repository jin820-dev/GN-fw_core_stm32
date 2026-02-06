/***************************************************************
 * FILENAME    : app_sd_spi_test.c
 * DESCRIPTION : SD(SPI) test routines (raw read/write, fs, logger test)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include <stdio.h>
#include <string.h>

#include "plat_timebase.h"
#include "plat_spi_bus.h"
#include "plat_sd_spi.h"
#include "app_logger.h"
#include "drv_sd.h"

static uint8_t buf[512];


void app_sd_test(void)
{
    printf("sd init...\r\n");
    sd_err_t e = sd_spi_init_card();
    printf("sd init=%d\r\n", (int)e);
    if (e != SD_OK) return;

    printf("read lba0...\r\n");
    e = sd_spi_read_block(0, buf);
    printf("read=%d\r\n", (int)e);
    if (e != SD_OK) return;

    // 先頭16Bだけダンプ
    printf("dump:");
    for (int i=0;i<16;i++) printf(" %02X", buf[i]);
    printf("\r\n");

    // 書き込みテスト（安全のため、LBAは0じゃなく適当な場所に！）
    // 例：LBA 1000 とか（カード内容が壊れるので要注意）
    for (int i=0;i<512;i++) buf[i] = (uint8_t)i;

    printf("write lba1000...\r\n");
    e = sd_spi_write_block(1000, buf);
    printf("write=%d\r\n", (int)e);

    // write後にreadして一致確認
    uint8_t rd[512];
    sd_spi_read_block(1000, rd);

    int diff = 0;
    for (int i = 0; i < 512; i++) {
        if (rd[i] != (uint8_t)i) { diff = i+1; break; }
    }
    printf("verify %s (first diff=%d)\r\n", diff ? "NG" : "OK", diff);

}

void app_sd_fs_test(void)
{
    const char path[] = "hello.txt";
    const char msg[]  = "GN-beluga FatFs OK!\r\n";
    uint8_t rb[64] = {0};
    uint32_t rsz = 0U;

    printf("drv_sd_init...\r\n");
    (void)drv_sd_init();

    printf("drv_sd_mount...\r\n");
    if (drv_sd_mount() == false) {
        printf("mount NG\r\n");
        return;
    }

    printf("drv_sd_write_file...\r\n");
    if (drv_sd_write_file(path, (const uint8_t*)msg, (uint32_t)strlen(msg)) == false) {
        printf("write NG\r\n");
        (void)drv_sd_unmount();
        return;
    }

    printf("drv_sd_read_file...\r\n");
    if (drv_sd_read_file(path, rb, sizeof(rb) - 1U, &rsz) == false) {
        printf("read NG\r\n");
        (void)drv_sd_unmount();
        return;
    }

    printf("read OK (%lu bytes): %s\r\n", (unsigned long)rsz, (char*)rb);

    (void)drv_sd_unmount();
    printf("unmount OK\r\n");
}


void app_logger_test(void)
{
    logger_err_t e = logger_init();
    printf("logger_init=%d\r\n", (int)e);

    for (int i = 0; i < 5; i++) {
        uint32_t ms = plat_millis();
        e = logger_append(ms, 1.23f + i, 4.56f + i, "test");
        printf("logger_append=%d\r\n", (int)e);
        plat_delay_ms(200);
    }
}

