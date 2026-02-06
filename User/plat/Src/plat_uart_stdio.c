/***************************************************************
 * FILENAME    : plat_uart_stdio.c
 * DESCRIPTION : Platform UART stdio implementation (printf redirection)
 * AUTHOR      : jin
 * DATE        : 2026/02/06
 ****************************************************************/


#include "plat_uart_stdio.h"

#include "usart.h" // huartX がここに extern 宣言される（CubeMX生成）
#include "stm32h7xx_hal.h"

#include <stdint.h>
#include <sys/unistd.h> // STDOUT_FILENO など（環境により不要）

void plat_uart_stdio_init(void)
{
// NOP（将来、リングバッファ/DMA化する場合にここで初期化）
}

/**

@brief 1文字送信（CubeIDEで printf がここに来る構成が多い）
「Project Properties > C/C++ Build > Settings」や
newlib-nano / small printf の設定により、printf は __io_putchar を使うことがあります。
\n が来たら \r\n に変換して、TeraTerm等で改行が崩れないようにします。
*/
int __io_putchar(int ch)
{
	uint8_t c = (uint8_t)ch;

	// 使用するUARTに合わせる（CubeMXで生成されたハンドル名）
	// 例：USART3を使っているなら huart3
	if (c == '\n') {
		uint8_t cr = '\r';
		(void)HAL_UART_Transmit(&huart3, &cr, 1, HAL_MAX_DELAY);
	}

	(void)HAL_UART_Transmit(&huart3, &c, 1, HAL_MAX_DELAY);
	return ch;
}

/**
@brief write() syscall（printfがこちらを使う構成もあるので保険で実装）
newlib系では printf → _write() 経由になることがあります。
__io_putchar() と両方実装しておくと、プロジェクト設定差に強くなります。
*/
int _write(int file, char *ptr, int len)
{
	// stdout/stderr 以外は無視してOK（必要なら対応拡張）
	if ((file != STDOUT_FILENO) && (file != STDERR_FILENO)) {
		return -1;
	}

	for (int i = 0; i < len; i++) {
		__io_putchar((unsigned char)ptr[i]);
	}
	return len;
}
