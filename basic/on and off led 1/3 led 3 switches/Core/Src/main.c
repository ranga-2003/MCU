#include "stm32f4xx.h"

void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 4000; i++) {
        __NOP(); // No Operation (CPU idle)
    }
}

void GPIO_Init(void) {
    // Enable GPIOA, GPIOB, and GPIOC clocks
    RCC->AHB1ENR |= (1 << 0) | (1 << 1) | (1 << 2);

    // Set PA7, PA8, and PA9 as output (MODER: 01)
    GPIOA->MODER |= (1 << (7 * 2)) | (1 << (8 * 2)) | (1 << (9 * 2));
    GPIOA->MODER &= ~((1 << (7 * 2 + 1)) | (1 << (8 * 2 + 1)) | (1 << (9 * 2 + 1)));

    // Set PB8, PB9, and PC9 as input (MODER: 00)
    GPIOB->MODER &= ~((1 << (8 * 2)) | (1 << (9 * 2)));
    GPIOC->MODER &= ~(1 << (9 * 2));

    // Enable pull-up resistors for PB8, PB9, and PC9
    GPIOB->PUPDR |= (1 << (8 * 2)) | (1 << (9 * 2));
    GPIOB->PUPDR &= ~((1 << (8 * 2 + 1)) | (1 << (9 * 2 + 1)));
    GPIOC->PUPDR |= (1 << (9 * 2));
    GPIOC->PUPDR &= ~(1 << (9 * 2 + 1));
}

int main(void) {
    GPIO_Init();

    while (1) {
        // Check PB8 (Switch 1)
        if (!(GPIOB->IDR & (1 << 8))) {
            GPIOA->BSRR = (1 << 7); // Turn on LED1 (PA7)
            delay_ms(500);
            GPIOA->BSRR = (1 << (7 + 16)); // Turn off LED1 (PA7)
        }

        // Check PB9 (Switch 2)
        if (!(GPIOB->IDR & (1 << 9))) {
            GPIOA->BSRR = (1 << 8); // Turn on LED2 (PA8)
            delay_ms(500);
            GPIOA->BSRR = (1 << (8 + 16)); // Turn off LED2 (PA8)
        }

        // Check PC9 (Switch 3)
        if (!(GPIOC->IDR & (1 << 9))) {
            GPIOA->BSRR = (1 << 9); // Turn on LED3 (PA9)
            delay_ms(500);
            GPIOA->BSRR = (1 << (9 + 16)); // Turn off LED3 (PA9)
        }
    }
}
