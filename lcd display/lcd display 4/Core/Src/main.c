#include "stm32f4xx_hal.h"
#include <string.h>

// LCD Pins
#define LCD_DATA_PORT GPIOC
#define RS_PIN GPIO_PIN_5
#define RW_PIN GPIO_PIN_6
#define EN_PIN GPIO_PIN_7
#define LCD_CTRL_PORT GPIOB

// Keypad Pins
#define ROW_PORT GPIOA
#define COL_PORT GPIOA
#define ROW1 GPIO_PIN_0
#define ROW2 GPIO_PIN_1
#define ROW3 GPIO_PIN_2
#define ROW4 GPIO_PIN_3
#define COL1 GPIO_PIN_4
#define COL2 GPIO_PIN_5
#define COL3 GPIO_PIN_6

// Function prototypes
void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_Char(char data);
void LCD_String(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_EnablePulse(void);
void LCD_Scroll_Left(char *text);
void LCD_Scroll_Right(char *text);
void Keypad_Init(void);
char Keypad_Read(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

// Message to scroll
char message[] = " STM32F4 LCD SCROLL DEMO ";

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    LCD_Init();
    Keypad_Init();

    LCD_SetCursor(0, 0);
    LCD_String("Press 5 /0 ");

    while (1)
    {
        char key = Keypad_Read();
        if (key == '5') LCD_Scroll_Left(message);
        if (key == '0') LCD_Scroll_Right(message);
    }
}

// Send command to LCD
void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(LCD_CTRL_PORT, RS_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_CTRL_PORT, RW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_DATA_PORT, 0xFF, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_DATA_PORT, cmd, GPIO_PIN_SET);
    LCD_EnablePulse();
}

// Send character to LCD
void LCD_Char(char data)
{
    HAL_GPIO_WritePin(LCD_CTRL_PORT, RS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_CTRL_PORT, RW_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_DATA_PORT, 0xFF, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_DATA_PORT, data, GPIO_PIN_SET);
    LCD_EnablePulse();
}

// Send string to LCD
void LCD_String(char *str)
{
    while (*str) LCD_Char(*str++);
}

// Set cursor position
void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Command(address);
}

// Enable Pulse
void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(LCD_CTRL_PORT, EN_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(LCD_CTRL_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_Delay(1);
}

// LCD Initialization (8-bit mode)
void LCD_Init(void)
{
    HAL_Delay(50);
    LCD_Command(0x38); // 8-bit mode, 2-line, 5x7 font
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x06); // Auto increment cursor
    LCD_Command(0x01); // Clear display
    HAL_Delay(5);
}

// Scroll text left
void LCD_Scroll_Left(char *text)
{
    int len = strlen(text);
    char buffer[17] = {0};

    for (int i = 0; i <= len - 16; i++)
    {
        strncpy(buffer, &text[i], 16);
        LCD_SetCursor(0, 0);
        LCD_String(buffer);
        HAL_Delay(300);
    }
}

// Scroll text right
void LCD_Scroll_Right(char *text)
{
    int len = strlen(text);
    char buffer[17] = {0};

    for (int i = len - 16; i >= 0; i--)
    {
        strncpy(buffer, &text[i], 16);
        LCD_SetCursor(0, 0);
        LCD_String(buffer);
        HAL_Delay(300);
    }
}

// Initialize Keypad
void Keypad_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure ROWS as OUTPUT
    GPIO_InitStruct.Pin = ROW1 | ROW2 | ROW3 | ROW4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(ROW_PORT, &GPIO_InitStruct);

    // Configure COLUMNS as INPUT
    GPIO_InitStruct.Pin = COL1 | COL2 | COL3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(COL_PORT, &GPIO_InitStruct);
}

// Read Keypad
char Keypad_Read(void)
{
    char keys[4][3] = {{'1', '2', '3'},
                       {'4', '5', '6'},
                       {'7', '8', '9'},
                       {'*', '0', '#'}};

    for (int row = 0; row < 4; row++)
    {
        HAL_GPIO_WritePin(ROW_PORT, ROW1 | ROW2 | ROW3 | ROW4, GPIO_PIN_SET);
        HAL_GPIO_WritePin(ROW_PORT, (ROW1 << row), GPIO_PIN_RESET);

        for (int col = 0; col < 3; col++)
        {
            if (HAL_GPIO_ReadPin(COL_PORT, (COL1 << col)) == GPIO_PIN_RESET)
            {
                HAL_Delay(200);
                return keys[row][col];
            }
        }
    }
    return 0;
}

// GPIO Initialization
static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = RS_PIN | RW_PIN | EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_CTRL_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_All;
    HAL_GPIO_Init(LCD_DATA_PORT, &GPIO_InitStruct);
}

// System Clock Configuration
void SystemClock_Config(void)
{
    // System clock function (Generated by CubeMX)
}
