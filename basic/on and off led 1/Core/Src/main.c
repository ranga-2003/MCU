#include "stm32f4xx.h"  // Include header for STM32F4 series

void GPIO_Config(void);

int main(void) {
    GPIO_Config();  // Configure GPIOs

    while (1) {
        if (GPIOC->IDR & GPIO_IDR_ID9) {  // Check if switch (PC9) is pressed
            GPIOA->BSRR = GPIO_BSRR_BS8;  // Turn on LED (PA8)
        } else {
            GPIOA->BSRR = GPIO_BSRR_BR8;  // Turn off LED (PA8)
        }
    }
}

void GPIO_Config(void) {
    // Enable clocks for GPIOA and GPIOC
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN;

    // Configure PA8 as output
    GPIOA->MODER &= ~(GPIO_MODER_MODE8); // Clear mode bits for PA8
    GPIOA->MODER |= GPIO_MODER_MODE8_0;  // Set PA8 as output
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT8;   // Push-pull mode
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR8; // High speed
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD8;   // No pull-up, pull-down

    // Configure PC9 as input
    GPIOC->MODER &= ~(GPIO_MODER_MODE9); // Set PC9 as input
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD9_0;  // Enable pull-up resistor for PC9
}
