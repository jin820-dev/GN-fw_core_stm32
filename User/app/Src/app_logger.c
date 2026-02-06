/***************************************************************
 * FILENAME    : app_logger.c
 * DESCRIPTION : Simple CSV logger interface (append / header handling)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "app_logger.h"

// --- include ---
#include <string.h>
#include <stdio.h>

#include "drv_sd.h"

// --- define ---
#define LOGGER_PATH "log.csv"

// --- variable declaration ---
static bool s_header_checked = false;

// --- function prototype ---
static logger_err_t ensure_header(void);

// --- functions ---
static logger_err_t ensure_header(void)
{
    if (s_header_checked) {
        return LOGGER_OK;
    }

    // 先頭1バイトだけ読んで「空ファイルかどうか」を判定する
    // - 読めて rsz > 0 なら既に何か入っているのでヘッダ不要
    // - 読めて rsz == 0 なら空なのでヘッダ作成
    // - 読めない（ファイルなし等）ならヘッダ作成
    uint8_t b = 0U;
    uint32_t rsz = 0U;
    bool ok = drv_sd_read_file(LOGGER_PATH, &b, 1U, &rsz);

    if ((ok == true) && (rsz > 0U)) {
        s_header_checked = true;
        return LOGGER_OK;
    }

    // ヘッダを書き込む（ファイルが無ければ作成される）
    {
        const char* header = "ms,ch1,ch2,note\r\n";
        if (drv_sd_write_file(LOGGER_PATH, (const uint8_t*)header, (uint32_t)strlen(header)) == false) {
            return LOGGER_ERR_WRITE;
        }
    }

    s_header_checked = true;
    return LOGGER_OK;
}

// ----------------------------------------------------------- //

logger_err_t logger_init(void)
{
    // mount は外で済ませる想定（boot / selftest など）
    return ensure_header();
}

// ----------------------------------------------------------- //

logger_err_t logger_append(uint32_t ms, float ch1, float ch2, const char* note)
{
    int32_t ch1_mV = (int32_t)(ch1 * 1000.0f);
    int32_t ch2_mV = (int32_t)(ch2 * 1000.0f);
    
    logger_err_t e = ensure_header();
    if (e != LOGGER_OK) {
        return e;
    }

    if (note == NULL) {
        note = "";
    }

    // CSV 1行生成
    char line[128];
    int n = snprintf(line, sizeof(line), "%lu,%ld,%ld,%s\r\n",
                    (unsigned long)ms, (long)ch1_mV, (long)ch2_mV, note);
    if ((n <= 0) || (n >= (int)sizeof(line))) {
        return LOGGER_ERR_WRITE;
    }

    // 追記保存（末尾追記は drv_sd 側で実現済み）
    if (drv_sd_append_file(LOGGER_PATH, (const uint8_t*)line, (uint32_t)n) == false) {
        return LOGGER_ERR_WRITE;
    }

    return LOGGER_OK;
}
