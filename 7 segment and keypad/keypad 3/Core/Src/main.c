#include "stm32f4xx_hal.h"
#include "stdio.h"

// Define keypad connections (adjust GPIO pin numbers accordingly)
#define ROW1_PIN GPIO_PIN_0
#define ROW2_PIN GPIO_PIN_1
#define ROW3_PIN GPIO_PIN_2
#define ROW4_PIN GPIO_PIN_3
#define COL1_PIN GPIO_PIN_4
#define COL2_PIN GPIO_PIN_5
#define COL3_PIN GPIO_PIN_6

// GPIO Port Definitions
#define ROW_GPIO_PORT GPIOB
#define COL_GPIO_PORT GPIOA

// Keypad Layout
char keys[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Function to initialize GPIO for Keypad
void Keypad_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clock
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // Configure row pins as output (Push-pull)
    GPIO_InitStruct.Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ROW_GPIO_PORT, &GPIO_InitStruct);

    // Configure column pins as input with pull-up
    GPIO_InitStruct.Pin = COL1_PIN | COL2_PIN | COL3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(COL_GPIO_PORT, &GPIO_InitStruct);
}

// Function to scan keypad and check if a key is pressed
char Keypad_Scan(void) {
    for (int row = 0; row < 4; row++) {
        // Drive the row LOW
        HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW2_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW3_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_GPIO_PORT, ROW4_PIN, GPIO_PIN_SET);

        // Set the current row LOW
        HAL_GPIO_WritePin(ROW_GPIO_PORT, (1 << row), GPIO_PIN_RESET);

        // Check columns for LOW input
        for (int col = 0; col < 3; col++) {
            if (HAL_GPIO_ReadPin(COL_GPIO_PORT, (1 << col)) == GPIO_PIN_RESET) {
                // Key is pressed, return corresponding key
                return keys[row][col];
            }
        }
    }
    return 0;  // No key pressed
}

int main(void) {
    // HAL Initialization
    HAL_Init();

    // Initialize Keypad
    Keypad_Init();

    char pressedKey;

    while (1) {
        // Scan for key press
        pressedKey = Keypad_Scan();

        if (pressedKey != 0) {
            // Print "Key pressed" when a key is detected
            printf("Key pressed: %c\n", pressedKey);
        }
    }
}
