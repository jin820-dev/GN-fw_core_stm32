/***************************************************************
 * FILENAME    : utils_i2c_scan.h
 * DESCRIPTION : I2C device scanner utility (debug helper)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#pragma once
#include "i2c.h"

void utils_i2c_scan(I2C_HandleTypeDef *hi2c);