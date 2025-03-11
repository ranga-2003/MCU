//#include "stm32f446re.h"  // Change according to your STM32 series

void delay(void);  // Delay function prototype

int main(void) {
    int num1 = 20;
    int num2 = 8;
    int result;

    result = num1 - num2;  // Perform subtraction

    // GPIO Configuration for LED (if you want to visualize the result)
    RCC->AHB1ENR |= (1 << 0);  // Enable GPIOA clock
    GPIOA->MODER |= (1 << 10); // Set PA5 as output

    while (1) {
        if (result > 0) {
            GPIOA->ODR |= (1 << 5);  // Turn ON LED if result is positive
        } else {
            GPIOA->ODR &= ~(1 << 5); // Turn OFF LED if result is negative
        }
        delay();
    }
}

void delay(void) {
    for (volatile int i = 0; i < 1000000; i++);  // Basic delay
}
