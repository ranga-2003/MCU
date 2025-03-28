#include "main.h"
#include <stdio.h>

// Define GPIO Pins for Rows and Columns
#define ROW1_PIN GPIO_PIN_0
#define ROW2_PIN GPIO_PIN_1
#define ROW3_PIN GPIO_PIN_2
#define ROW4_PIN GPIO_PIN_3
#define COL1_PIN GPIO_PIN_4
#define COL2_PIN GPIO_PIN_5
#define COL3_PIN GPIO_PIN_6

#define ROW_PORT GPIOA
#define COL_PORT GPIOA

// Keypad Layout (4x3)
char keyMap[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

// Function Prototypes
void MX_GPIO_Init(void);
char scanKeypad(void);
int _write(int file, char *ptr, int len);  // For printf via UART



int main(void) {
    HAL_Init();

    MX_GPIO_Init();

    while (1) {
        char key = scanKeypad();  // Get the pressed key
        if (key != 0) {
            printf("Key Pressed: %c\n", key); // Print the key value
            HAL_Delay(300);  // Debounce delay
        }
    }
}

// Function to scan the keypad
char scanKeypad(void) {
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_SET);

    for (int row = 0; row < 4; row++) {
        HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_SET);

        if (row == 0) HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_RESET);
        else if (row == 1) HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_RESET);
        else if (row == 2) HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_RESET);
        else HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_RESET);

        for (int col = 0; col < 3; col++) {
            uint16_t colPin = (col == 0) ? COL1_PIN : (col == 1) ? COL2_PIN : COL3_PIN;
            if (HAL_GPIO_ReadPin(COL_PORT, colPin) == GPIO_PIN_RESET) {
                return keyMap[row][col];  // Return the key pressed
            }
        }
    }
    return 0; // No key pressed
}

// Function to initialize GPIO for Keypad
void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Initialize Rows (PA0 to PA3) as Output
    GPIO_InitStruct.Pin = ROW1_PIN | ROW2_PIN | ROW3_PIN | ROW4_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ROW_PORT, &GPIO_InitStruct);

    // Initialize Columns (PA4 to PA6) as Input with Pull-up Resistor
    GPIO_InitStruct.Pin = COL1_PIN | COL2_PIN | COL3_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(COL_PORT, &GPIO_InitStruct);
}
