#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <string.h>

/* UART Handle */
UART_HandleTypeDef huart2;

/* LCD GPIO Definitions */
#define RS GPIO_PIN_0
#define EN GPIO_PIN_1
#define D4 GPIO_PIN_4
#define D5 GPIO_PIN_5
#define D6 GPIO_PIN_6
#define D7 GPIO_PIN_7
#define LCD_PORT GPIOA

/* Function Prototypes */
void UART2_Init(void);
char UART_Receive(void);
void LCD_Init(void);
void LCD_Command(char cmd);
void LCD_Char(char data);
void LCD_String(char *str);
void LCD_Clear(void);

/* UART GPIO Initialization */
void UART2_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART2_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* UART2 Initialization */
void UART2_Init(void) {
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart2) != HAL_OK) {
        while (1);
    }
}

/* UART Receive Function */
char UART_Receive(void) {
    char receivedChar;
    HAL_UART_Receive(&huart2, (uint8_t *)&receivedChar, 1, HAL_MAX_DELAY);
    return receivedChar;
}

/* LCD GPIO Initialization */
void LCD_GPIO_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = RS | EN | D4 | D5 | D6 | D7;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(LCD_PORT, &GPIO_InitStruct);
}

/* LCD Send Command */
void LCD_Command(char cmd) {
    HAL_GPIO_WritePin(LCD_PORT, RS, GPIO_PIN_RESET); // RS = 0 for command

    HAL_GPIO_WritePin(LCD_PORT, D4, (cmd >> 4) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5, (cmd >> 5) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6, (cmd >> 6) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7, (cmd >> 7) & 1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_PORT, D4, cmd & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5, (cmd >> 1) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6, (cmd >> 2) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7, (cmd >> 3) & 1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_RESET);
}

/* LCD Initialization */
void LCD_Init(void) {
    HAL_Delay(20);
    LCD_Command(0x02); // 4-bit mode
    LCD_Command(0x28); // 2 lines, 5x8 font
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x06); // Auto Increment cursor
    LCD_Command(0x01); // Clear screen
    HAL_Delay(2);
}

/* LCD Send Character */
void LCD_Char(char data) {
    HAL_GPIO_WritePin(LCD_PORT, RS, GPIO_PIN_SET); // RS = 1 for data

    HAL_GPIO_WritePin(LCD_PORT, D4, (data >> 4) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5, (data >> 5) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6, (data >> 6) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7, (data >> 7) & 1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_RESET);

    HAL_GPIO_WritePin(LCD_PORT, D4, data & 1);
    HAL_GPIO_WritePin(LCD_PORT, D5, (data >> 1) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D6, (data >> 2) & 1);
    HAL_GPIO_WritePin(LCD_PORT, D7, (data >> 3) & 1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_PORT, EN, GPIO_PIN_RESET);
}

/* LCD Send String */
void LCD_String(char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

/* LCD Clear */
void LCD_Clear(void) {
    LCD_Command(0x01);
    HAL_Delay(2);
}

/* Main Function */
int main(void) {
    HAL_Init();
    UART2_GPIO_Init();
    UART2_Init();
    LCD_GPIO_Init();
    LCD_Init();

    char receivedChar;
    LCD_String("Waiting for UART...");

    while (1) {
        receivedChar = UART_Receive(); // Get character from UART

        if (receivedChar == 'C' || receivedChar == 'c') {
            LCD_Clear(); // Clear LCD
        } else {
            LCD_Char(receivedChar); // Print received character
        }
    }
}
