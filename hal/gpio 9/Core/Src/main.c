#include "stm32f4xx_hal.h"
#include <stdio.h>

void SystemClock_Config(void);
void GPIO_Init(void);

volatile uint8_t press_count = 0;
volatile uint8_t led_state = 0;

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
            led_state = !led_state; // Toggle LED state
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, led_state ? GPIO_PIN_SET : GPIO_PIN_RESET);

            if (led_state) {
                printf("LED ON\n");
            } else {
                printf("LED OFF\n");
            }

            printf("Switch pressed %d times\n", press_count);
        }
    }
}

void GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure onboard LED (PA5 assumed)
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
