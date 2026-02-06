/***************************************************************
 * FILENAME    : plat_timebase.h
 * DESCRIPTION : Platform timebase abstraction (millis / delay)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#ifndef PLAT_TIMEBASE_H_
#define PLAT_TIMEBASE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void     plat_timebase_init(void);
uint32_t plat_millis(void);
void     plat_delay_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* PLAT_TIMEBASE_H_ */
