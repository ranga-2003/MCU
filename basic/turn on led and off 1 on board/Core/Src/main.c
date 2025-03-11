#include "stm32f4xx.h"  // Include header for STM32F4 series

void GPIO_Config(void);

int main(void) {
    GPIO_Config();  // Configure GPIOs

    while (1) {
        if (GPIOC->IDR & GPIO_IDR_ID9) {  // Check if switch (PC9) is pressed
            GPIOA->BSRR = GPIO_BSRR_BS5;  // Turn on LED (PA5)
        } else {
            GPIOA->BSRR = GPIO_BSRR_BR5;  // Turn off LED (PA5)
        }
    }
}

void GPIO_Config(void) {
    // Enable clocks for GPIOA and GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    // Configure PA5 as output
    GPIOA->MODER &= ~(GPIO_MODER_MODE5); // Clear mode bits for PA5
    GPIOA->MODER |= GPIO_MODER_MODE5_0;  // Set PA5 as output
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;   // Push-pull mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5; // High speed
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;   // No pull-up, pull-down

    // Configure PC9 as input
    GPIOC->MODER &= ~(GPIO_MODER_MODE9); // Set PC9 as input
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD9_0;  // Enable pull-up resistor for PC9
}
