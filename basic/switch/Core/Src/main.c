#include "stm32f4xx.h"

void GPIO_Init(void);
void Delay(uint32_t time);

int main(void)
{
    GPIO_Init();
    uint8_t buttonState = 0, prevButtonState = 0;
    uint8_t ledState = 0;

    while (1)
    {
        buttonState = (GPIOC->IDR & GPIO_IDR_ID9) ? 1 : 0;

        if (buttonState && !prevButtonState) // Detect rising edge
        {
            ledState = !ledState;
            if (ledState)
                GPIOA->BSRR = GPIO_BSRR_BS8; // Turn on LED
            else
                GPIOA->BSRR = GPIO_BSRR_BR8; // Turn off LED
        }

        prevButtonState = buttonState;
        Delay(100000); // Debounce delay
    }
}

void GPIO_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN; // Enable GPIOA & GPIOC clock

    // Configure PA8 as output
    GPIOA->MODER &= ~(GPIO_MODER_MODER8);
    GPIOA->MODER |= GPIO_MODER_MODER8_0;

    // Configure PC9 as input
    GPIOC->MODER &= ~(GPIO_MODER_MODER9);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR9_0; // Enable pull-up resistor
}

void Delay(uint32_t time)
{
    while (time--) __NOP(); // Simple delay loop
}
