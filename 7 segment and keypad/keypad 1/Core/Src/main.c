#include "stm32f4xx_hal.h"
#include <stdio.h>

// Define row and column GPIO pins for PB series
#define ROW1 GPIO_PIN_0
#define ROW2 GPIO_PIN_1
#define ROW3 GPIO_PIN_2
#define ROW4 GPIO_PIN_3
#define COL1 GPIO_PIN_4
#define COL2 GPIO_PIN_5
#define COL3 GPIO_PIN_6

#define ROW_PORT GPIOB  // Using Port B for rows
#define COL_PORT GPIOB  // Using Port B for columns

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void scan_keypad(void);

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    // Configure the main internal regulator output voltage
    __HAL_RCC_PWR_CLK_ENABLE();

    // Initializes the CPU, AHB and APB busses clocks
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    // Initializes the CPU, AHB and APB busses clocks
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}


int main(void) {
    HAL_Init();                      // Initialize HAL Library
    SystemClock_Config();             // Configure the system clock
    MX_GPIO_Init();                   // Initialize GPIO

    while (1) {
        scan_keypad();                // Scan the keypad for keypresses
        HAL_Delay(200);               // Debounce delay
    }
}

// Function to scan the keypad and detect which row has a key press
void scan_keypad(void) {
    // Iterate through columns
    for (int col = 0; col < 3; col++) {
        // Set current column HIGH
        HAL_GPIO_WritePin(COL_PORT, (COL1 << col), GPIO_PIN_SET);

        // Check if any row detects a key press
        if (HAL_GPIO_ReadPin(ROW_PORT, ROW1) == GPIO_PIN_SET) {
            printf("Key pressed in Row 1\n");
        }
        if (HAL_GPIO_ReadPin(ROW_PORT, ROW2) == GPIO_PIN_SET) {
            printf("Key pressed in Row 2\n");
        }
        if (HAL_GPIO_ReadPin(ROW_PORT, ROW3) == GPIO_PIN_SET) {
            printf("Key pressed in Row 3\n");
        }
        if (HAL_GPIO_ReadPin(ROW_PORT, ROW4) == GPIO_PIN_SET) {
            printf("Key pressed in Row 4\n");
        }

        // Reset column back to LOW
        HAL_GPIO_WritePin(COL_PORT, (COL1 << col), GPIO_PIN_RESET);
    }
}

// GPIO Initialization function for PB pins
static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOB clock
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure ROW pins as input with pull-down resistors
    GPIO_InitStruct.Pin = ROW1 | ROW2 | ROW3 | ROW4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(ROW_PORT, &GPIO_InitStruct);

    // Configure COLUMN pins as output (Initially LOW)
    GPIO_InitStruct.Pin = COL1 | COL2 | COL3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(COL_PORT, &GPIO_InitStruct);

    // Set all column pins LOW initially
    HAL_GPIO_WritePin(COL_PORT, COL1 | COL2 | COL3, GPIO_PIN_RESET);
}
