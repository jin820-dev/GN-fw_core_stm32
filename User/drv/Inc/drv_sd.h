/***************************************************************
 * FILENAME    : drv_sd.h
 * DESCRIPTION : SD card driver public interface
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#pragma once

// --- include ---
#include <stdbool.h>
#include <stdint.h>

// --- define ---
// --- function declaration ---
bool drv_sd_init(void);
bool drv_sd_mount(void);
bool drv_sd_unmount(void);

bool drv_sd_write_file(const char* path, const uint8_t* buf, uint32_t size);
bool drv_sd_append_file(const char* path, const uint8_t* buf, uint32_t size);
bool drv_sd_read_file(const char* path, uint8_t* buf, uint32_t max_size, uint32_t* out_read_size);

