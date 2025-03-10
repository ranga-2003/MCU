#include "stm32f4xx_hal.h"
#include <stdio.h>

void SystemClock_Config(void);
void GPIO_Init(void);
void delay_ms(uint32_t ms);

volatile uint8_t press_count = 0;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    while (1) {
        // Check if switch PB8 is pressed
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET) {
            HAL_Delay(200); // Debounce delay
            while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET); // Wait for release
            HAL_Delay(200); // Debounce delay

            press_count++;

            if (press_count == 1) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); // Turn on LED1 (PA7)
            } else if (press_count == 2) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); // Turn on LED2 (PA8)
            } else if (press_count == 3) {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET); // Turn on LED3 (PA9)
            }
        }
    }
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PA7, PA8, PA9 as output
    GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PB8 as input with pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SystemClock_Config(void) {
    // This function should be configured based on your clock setup
    // Example configurations can be generated using STM32CubeMX
}

void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}
