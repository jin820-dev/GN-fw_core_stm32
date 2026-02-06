/***************************************************************
 * FILENAME    : app_test.c
 * DESCRIPTION : Application test dispatcher (run selected tests)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "app_test.h"

#define RUN_TEST_SD_SPI   1

void app_test_run(void)
{
#if RUN_TEST_SD_SPI
  app_sd_test();
  app_sd_fs_test();
  app_logger_test();
#endif

}
