#include "stm32f4xx_hal.h"

// Define 7-Segment GPIO Pins (Common Cathode)
#define SEG_A GPIO_PIN_0
#define SEG_B GPIO_PIN_1
#define SEG_C GPIO_PIN_2
#define SEG_D GPIO_PIN_3
#define SEG_E GPIO_PIN_4
#define SEG_F GPIO_PIN_5
#define SEG_G GPIO_PIN_6
#define SEG_GPIO_PORT GPIOA

// Define LED and Button GPIO
#define LED_PIN GPIO_PIN_7
#define BUTTON_PIN GPIO_PIN_8
#define LED_GPIO_PORT GPIOA
#define BUTTON_GPIO_PORT GPIOA

// Number patterns for 7-segment (Common Cathode)
uint8_t segment_map[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

// Function to initialize GPIO
void GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIOA Clock
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure 7-segment pins as output
    GPIO_InitStruct.Pin = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SEG_GPIO_PORT, &GPIO_InitStruct);

    // Configure LED pin as output
    GPIO_InitStruct.Pin = LED_PIN;
    HAL_GPIO_Init(LED_GPIO_PORT, &GPIO_InitStruct);

    // Configure Button pin as input with pull-up
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_GPIO_PORT, &GPIO_InitStruct);
}

// Function to display a number on 7-segment
void Display_Number(uint8_t num) {
    uint8_t pattern = segment_map[num];

    // Set segments according to pattern
    HAL_GPIO_WritePin(SEG_GPIO_PORT, SEG_A, (pattern & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_GPIO_PORT, SEG_B, (pattern & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_GPIO_PORT, SEG_C, (pattern & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_GPIO_PORT, SEG_D, (pattern & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_GPIO_PORT, SEG_E, (pattern & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_GPIO_PORT, SEG_F, (pattern & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(SEG_GPIO_PORT, SEG_G, (pattern & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

// Function to toggle LED "num" times
void Toggle_LED(uint8_t num) {
    for (uint8_t i = 0; i < num; i++) {
        HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
        HAL_Delay(500); // 500ms delay
    }
}

// Main Function
int main(void) {
    HAL_Init();  // Initialize HAL Library
    GPIO_Init(); // Initialize GPIO

    uint8_t count = 0; // Start count at 0

    while (1) {
        // Display count on 7-segment
        Display_Number(count);

        // Toggle LED "count" times
        Toggle_LED(count);

        // Wait for button press to increase count
        while (HAL_GPIO_ReadPin(BUTTON_GPIO_PORT, BUTTON_PIN) == GPIO_PIN_RESET) {
            HAL_Delay(100); // Debounce delay
        }

        // Increase count (reset to 0 after 9)
        count = (count + 1) % 10;

        HAL_Delay(1000); // Delay before next loop
    }
}
