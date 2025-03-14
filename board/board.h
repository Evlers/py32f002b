/*
 * Copyright (c) 2025-2025 Evlers Developers
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-11     Evlers       first implementation
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "py32f002bx5.h"
#include "py32f0xx_hal.h"

#define BOARD_LED2_PORT         GPIOA
#define BOARD_LED2_PIN          GPIO_PIN_1
#define BOARD_LED2_ON()         BOARD_LED2_PORT->BRR = BOARD_LED2_PIN
#define BOARD_LED2_OFF()        BOARD_LED2_PORT->BSRR = BOARD_LED2_PIN
#define BOARD_LED2_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BOARD_LED3_PORT         GPIOA
#define BOARD_LED3_PIN          GPIO_PIN_5
#define BOARD_LED3_ON()         BOARD_LED3_PORT->BRR = BOARD_LED3_PIN
#define BOARD_LED3_OFF()        BOARD_LED3_PORT->BSRR = BOARD_LED3_PIN
#define BOARD_LED3_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BOARD_LED4_PORT         GPIOA
#define BOARD_LED4_PIN          GPIO_PIN_4
#define BOARD_LED4_ON()         BOARD_LED4_PORT->BRR = BOARD_LED4_PIN
#define BOARD_LED4_OFF()        BOARD_LED4_PORT->BSRR = BOARD_LED4_PIN
#define BOARD_LED4_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BOARD_BTN1_PORT         GPIOA
#define BOARD_BTN1_PIN          GPIO_PIN_3
#define BOARD_BTN1_GET()        (!(BOARD_BTN1_PORT->IDR & BOARD_BTN1_PIN))
#define BOARD_BTN1_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

#define BOARD_BTN2_PORT         GPIOA
#define BOARD_BTN2_PIN          GPIO_PIN_0
#define BOARD_BTN2_GET()        (!(BOARD_BTN2_PORT->IDR & BOARD_BTN2_PIN))
#define BOARD_BTN2_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()

void board_init (void);

#endif /* _BOARD_H_ */
