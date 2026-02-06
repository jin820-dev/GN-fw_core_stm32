/***************************************************************
 * FILENAME    : utils_i2c_scan.c
 * DESCRIPTION : I2C device scanner utility (debug helper)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "i2c.h"
#include <stdio.h>
#include "utils_log.h"

#define I2C_SCAN_TIMEOUT (5)

void utils_i2c_scan(I2C_HandleTypeDef *hi2c)
{
  uint8_t addr;

  if (hi2c == NULL)
  {
    return;
  }

  //printf("I2C scan start (7bit)\n");
  LOG_INFO("I2C", "I2C scan start (7bit)");

  for (addr = 1; addr < 0x7F; addr++)
  {
    if (HAL_I2C_IsDeviceReady(hi2c, (uint16_t)(addr << 1), 1, I2C_SCAN_TIMEOUT) == HAL_OK)
    {
      //printf("found device: 0x%02X\n", addr);
      LOG_INFO("I2C", "found device: 0x%02X", addr);
    }
  }

  //printf("I2C scan end\n");
  LOG_INFO("I2C", "I2C scan end");
}