/***************************************************************
 * FILENAME    : plat_uart_stdio.h
 * DESCRIPTION : Platform UART stdio bridge (printf redirection)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#ifndef PLAT_UART_STDIO_H_
#define PLAT_UART_STDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/**

@brief printf の出力先を UART に接続する（必要なら将来拡張用）

現状は NOP でもOK。

DMA化/リングバッファ化する場合に、この関数で初期化を入れる想定。
*/
void plat_uart_stdio_init(void);

#ifdef __cplusplus
}
#endif

#endif /* PLAT_UART_STDIO_H_ */
