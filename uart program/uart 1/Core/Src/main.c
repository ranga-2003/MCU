#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <string.h>

UART_HandleTypeDef huart2;  // UART handle for USART2

// Function to configure GPIO for UART
void UART2_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();   // Enable GPIOA Clock
    __HAL_RCC_USART2_CLK_ENABLE();  // Enable USART2 Clock

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PA2 (TX) and PA3 (RX) as Alternate Function
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      // Alternate Function, Push-Pull
    GPIO_InitStruct.Pull = GPIO_PULLUP;          // Enable Pull-up Resistors
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2; // Set AF7 for USART2

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

// Function to initialize UART2
void UART2_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;             // Set Baud Rate
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;      // Enable TX & RX
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        // Initialization Error
        while (1);
    }
}

// Function to transmit data via UART
void UART_Send(char *message) {
    HAL_UART_Transmit(&huart2, (uint8_t *)message, strlen(message), 1000);
}

// Function to receive data via UART
void UART_Receive(char *buffer, uint16_t size) {
    HAL_UART_Receive(&huart2, (uint8_t *)buffer, size, 1000);
}

// Main function
int main(void) {
    HAL_Init();          // Initialize HAL Library
    UART2_GPIO_Init();   // Configure UART2 GPIO
    UART2_Init();        // Initialize UART2

    char rx_buffer[20];  // Buffer for received data
    char *msg = "Hello, STM32!\n";

    while (1) {
        UART_Send(msg);       // Send message
        HAL_Delay(1000);      // Wait 1 second

        // Receive data (blocking mode)
        UART_Receive(rx_buffer, sizeof(rx_buffer));

        // Send back received data
        UART_Send("Received: ");
        UART_Send(rx_buffer);
        UART_Send("\n");
    }
}
