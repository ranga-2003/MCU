#include "main.h"
#include <stdio.h>
#include "stm32f4xx_hal.h"   // STM32F4 HAL functions



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

// Function Prototypes
void MX_GPIO_Init(void);
void scanKeypad(void);
void printMessage(const char* message); // UART print function

int main(void) {
    HAL_Init();

    MX_GPIO_Init();

    while (1) {
        scanKeypad();  // Continuously scan the keypad
    }
}

// Function to scan the keypad
void scanKeypad(void) {
    // Set all rows to HIGH initially (inactive)
    HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_SET);

    // Check each row one by one
    for (int row = 0; row < 4; row++) {
        // Reset all rows to HIGH before setting one row to LOW
        HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_SET);

        // Set one row to LOW
        if (row == 0) HAL_GPIO_WritePin(ROW_PORT, ROW1_PIN, GPIO_PIN_RESET);
        else if (row == 1) HAL_GPIO_WritePin(ROW_PORT, ROW2_PIN, GPIO_PIN_RESET);
        else if (row == 2) HAL_GPIO_WritePin(ROW_PORT, ROW3_PIN, GPIO_PIN_RESET);
        else HAL_GPIO_WritePin(ROW_PORT, ROW4_PIN, GPIO_PIN_RESET);

        // Check each column for a LOW state (indicating a key press)
        if (HAL_GPIO_ReadPin(COL_PORT, COL1_PIN) == GPIO_PIN_RESET) {
            printMessage("Key Pressed\n");
            HAL_Delay(300); // Debounce delay
        }
        if (HAL_GPIO_ReadPin(COL_PORT, COL2_PIN) == GPIO_PIN_RESET) {
            printMessage("Key Pressed\n");
            HAL_Delay(300); // Debounce delay
        }
        if (HAL_GPIO_ReadPin(COL_PORT, COL3_PIN) == GPIO_PIN_RESET) {
            printMessage("Key Pressed\n");
            HAL_Delay(300); // Debounce delay
        }
    }
}

// Function to initialize GPIO for Keypad
void MX_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable GPIOA Clock

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

// Function to print messages (via UART)
void printMessage(const char* message) {
    printf("%s", message);  // Print message to UART
}

// Required for printf to work with UART (Retargeting)
