#include "stm32f4xx_hal.h"
#include <stdio.h>

void SystemClock_Config(void);
void GPIO_Init(void);

volatile uint8_t press_count = 0;
volatile uint8_t led1_state = 0;
volatile uint8_t led2_state = 1;

int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();

    while (1) {
        // Check if switch is pressed (Onboard switch assumed on GPIOC_PIN_13)
        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET) {
            HAL_Delay(200); // Debounce delay
            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET); // Wait for release
            HAL_Delay(200); // Debounce delay

            press_count++;
            led1_state = 1;
            led2_state = 0;

            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, led1_state ? GPIO_PIN_SET : GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, led2_state ? GPIO_PIN_SET : GPIO_PIN_RESET);

            printf("Switch pressed %d times\n", press_count);
            printf("LED1: %s, LED2: %s\n", led1_state ? "ON" : "OFF", led2_state ? "ON" : "OFF");
        }
    }
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure onboard LED (PA5 assumed) and external LED (PB7)
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Configure onboard button (PC13 assumed)
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void SystemClock_Config(void) {
    // This function should be configured based on your clock setup
    // Example configurations can be generated using STM32CubeMX
}
