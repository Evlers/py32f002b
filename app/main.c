/*
 * Copyright (c) 2025-2025 Evlers Developers
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-11     Evlers       first implementation
 */

#include <stdint.h>
#include <stdbool.h>
#include "compiler.h"
#include "kservice.h"
#include "board.h"

struct led_ctrl
{
    GPIO_TypeDef *gpio;
    uint16_t pin;
    bool is_on;
    uint32_t delay;
};

struct led_ctrl leds[] =
{
    { BOARD_LED2_PORT, BOARD_LED2_PIN, true, 50 },
    { BOARD_LED3_PORT, BOARD_LED3_PIN, true, 50 },
    { BOARD_LED4_PORT, BOARD_LED4_PIN, true, 50 },
    { BOARD_LED2_PORT, BOARD_LED2_PIN, false, 50 },
    { BOARD_LED3_PORT, BOARD_LED3_PIN, false, 50 },
    { BOARD_LED4_PORT, BOARD_LED4_PIN, false, 1000 },
};

int main (void)
{
    board_init();
    kprintf("PY32F002B\r\n");

    uint8_t index = 0;
    uint32_t delay = 0, tick = 0;
    while (1)
    {
        if (HAL_GetTick() - tick > delay)
        {
            tick = HAL_GetTick();
            HAL_GPIO_WritePin(leds[index].gpio, leds[index].pin, leds[index].is_on ? GPIO_PIN_RESET : GPIO_PIN_SET);
            delay = leds[index].delay;
            index ++;
            if (index >= (sizeof(leds) / sizeof(leds[0])))
                index = 0;
        }

        if (BOARD_BTN1_GET())
        {
            kprintf("Button 1 pressed\r");
            index = 0;
            delay = 0;
        }

        if (BOARD_BTN2_GET())
        {
            kprintf("Button 2 pressed\r");
            index = 1;
            delay = 0;
        }
    }
}
