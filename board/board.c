/*
 * Copyright (c) 2025-2030 Evlers Developers
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-03-11     Evlers       first implementation
 */

#include "board.h"
#include "kservice-ports.h"

static void system_clock_config (void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* oscillator configuration */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE; /* Select oscillator HSE, HSI, LSI, LSE */
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;                           /* Enable HSI */
    RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                           /* HSI 1 frequency division */
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_24MHz;  /* Configure HSI clock 24MHz */
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS_DISABLE;               /* Close HSE bypass */
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                          /* Close LSI */
    // RCC_OscInitStruct.LSICalibrationValue = RCC_LSICALIBRATION_32768Hz;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                          /* Close LSE */
    // RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;
    /* Configure oscillator */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        while (1);
    }

    /* Clock source configuration */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Choose to configure clock HCLK, SYSCLK, PCLK1 */
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSISYS; /* Select HSISYS as the system clock */
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;        /* AHB clock 1 division */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;         /* APB clock 1 division */
    /* Configure clock source */
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        while (1);
    }
}

static void btn_init (void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    BOARD_BTN1_CLK_ENABLE();
    GPIO_InitStruct.Pin = BOARD_BTN1_PIN;
    HAL_GPIO_Init(BOARD_BTN1_PORT, &GPIO_InitStruct);

    BOARD_BTN2_CLK_ENABLE();
    GPIO_InitStruct.Pin = BOARD_BTN2_PIN;
    HAL_GPIO_Init(BOARD_BTN2_PORT, &GPIO_InitStruct);
}

static void led_init (void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    BOARD_LED2_CLK_ENABLE();
    GPIO_InitStruct.Pin = BOARD_LED2_PIN;
    HAL_GPIO_WritePin(BOARD_LED2_PORT, BOARD_LED2_PIN, GPIO_PIN_SET);
    HAL_GPIO_Init(BOARD_LED2_PORT, &GPIO_InitStruct);

    BOARD_LED3_CLK_ENABLE();
    GPIO_InitStruct.Pin = BOARD_LED3_PIN;
    HAL_GPIO_WritePin(BOARD_LED3_PORT, BOARD_LED3_PIN, GPIO_PIN_SET);
    HAL_GPIO_Init(BOARD_LED3_PORT, &GPIO_InitStruct);

    BOARD_LED4_CLK_ENABLE();
    GPIO_InitStruct.Pin = BOARD_LED4_PIN;
    HAL_GPIO_WritePin(BOARD_LED4_PORT, BOARD_LED4_PIN, GPIO_PIN_SET);
    HAL_GPIO_Init(BOARD_LED4_PORT, &GPIO_InitStruct);
}

void board_init (void)
{
    /* reset of all peripherals, initializes the systick. */
    HAL_Init();

    /* system clock configuration */
    system_clock_config();

    /* initialize the porting layer */
    ports_kservice_init();

    /* button and led init */
    btn_init();
    led_init();
}
