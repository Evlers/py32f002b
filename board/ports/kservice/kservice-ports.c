/*
 * Copyright (c) 2025-2030 Evlers Developers
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-11     Evlers       first implementation
 */

#include <stdint.h>
#include "board.h"

static USART_HandleTypeDef uart_handle;

/**
 * @brief This function will put string to the console.
 *
 * @param str is the string output to the console.
 */
void _kputs(const char *str, long len)
{
    HAL_USART_Transmit(&uart_handle, (uint8_t *)str, len, 10);
}

void ports_kservice_init(void)
{
    HAL_USART_DeInit(&uart_handle);

    uart_handle.Instance = USART1;
    uart_handle.Init.BaudRate = 115200;
    uart_handle.Init.WordLength = USART_WORDLENGTH_8B;
    uart_handle.Init.StopBits = USART_STOPBITS_1;
    uart_handle.Init.Parity = USART_PARITY_NONE;
    uart_handle.Init.Mode = USART_MODE_TX_RX;

    HAL_USART_Init(&uart_handle);
}
