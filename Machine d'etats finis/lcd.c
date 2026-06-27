
// lcd.c
#include "lcd.h"

// Offsets DDRAM pour les lignes
static const uint8_t ROW_OFFSETS[] = {0x00, 0x40};

// --- Enable : impulsion sur EN ---
static void LCD_Enable(void) {
    GPIOC->ODR |= EN_PIN;
    Delay_Us(10);
    GPIOC->ODR &= ~EN_PIN;
    Delay_Us(100);
}

// --- Envoi de 4 bits ---
static void LCD_Send4Bits(uint8_t data) {
    GPIOC->ODR &= ~(D4_PIN | D5_PIN | D6_PIN | D7_PIN);
    GPIOC->ODR |= ((data & 0x0F) << 8);
    LCD_Enable();
}

// --- Commande ---
static void LCD_SendCommand(uint8_t cmd) {
    GPIOC->ODR &= ~RS_PIN;
    LCD_Send4Bits(cmd >> 4);
    LCD_Send4Bits(cmd & 0x0F);
    if(cmd == LCD_CLEAR || cmd == LCD_HOME) Delay_Ms(2);
}

// --- Données ---
static void LCD_SendData(uint8_t data) {
    GPIOC->ODR |= RS_PIN;
    LCD_Send4Bits(data >> 4);
    LCD_Send4Bits(data & 0x0F);
    Delay_Us(100);
}

// --- Initialisation LCD ---
void LCD_Init(void) {
    RCC->AHB1ENR |= (1 << 2); // Horloge GPIOC

    // Configurer PC0, PC1, PC10-PC13 en sortie
    GPIOC->MODER &= ~(
        (3 << RS_PIN*2)  | (3 << EN_PIN*2)  | 
        (3 << D4_PIN*2)  | (3 << D5_PIN*2)  | 
        (3 << D6_PIN*2) | (3 << D7_PIN*2)
    );
    GPIOC->MODER |= (
        (3 << RS_PIN*2)  | (3 << EN_PIN*2)  | 
        (3 << D4_PIN*2)  | (3 << D5_PIN*2)  | 
        (3 << D6_PIN*2) | (3 << D7_PIN*2)
    );

    // Séquence d'init HD44780
    Delay_Ms(50);
    LCD_Send4Bits(0x03); Delay_Ms(5);
    LCD_Send4Bits(0x03); Delay_Us(150);
    LCD_Send4Bits(0x03); Delay_Us(150);
    LCD_Send4Bits(0x02); Delay_Us(150);

    LCD_SendCommand(0x28); // 4 bits, 2 lignes
    LCD_SendCommand(0x0C); // Display ON, Cursor OFF
    LCD_SendCommand(0x06); // Entry mode
    LCD_SendCommand(0x01); // Clear
    Delay_Ms(2);
}

// --- Fonctions publiques ---
void LCD_Clear(void) { LCD_SendCommand(LCD_CLEAR); Delay_Ms(2); }
void LCD_Home(void)  { LCD_SendCommand(LCD_HOME);  Delay_Ms(2); }
void LCD_Print(char *str) { while(*str) LCD_SendData(*str++); }
void LCD_PrintText(int col, int row, char *text) {
    row = (row > 1) ? 1 : row;
    col = (col > 15) ? 15 : col;
    LCD_SendCommand(LCD_SETDDRAMADDR | (col + ROW_OFFSETS[row]));
    while(*text && (col++ < 16)) LCD_SendData(*text++);
}

// --- Fonctions avancées ---
void LCD_CursorOn(void)  { LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON); Delay_Ms(1); }
void LCD_CursorOff(void) { LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF); Delay_Ms(1); }
void LCD_BlinkOn(void)   { LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON); Delay_Ms(1); }
void LCD_BlinkOff(void)  { LCD_SendCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF); Delay_Ms(1); }

void LCD_MoveCursor(int col, int row) {
    row = (row > 1) ? 1 : row;
    col = (col > 15) ? 15 : col;
    LCD_SendCommand(LCD_SETDDRAMADDR | (col + ROW_OFFSETS[row]));
    Delay_Ms(1);
}

void LCD_CursorShiftLeft(void)  { LCD_SendCommand(LCD_CURSORSHIFT | 0x00); Delay_Ms(1); }
void LCD_CursorShiftRight(void) { LCD_SendCommand(LCD_CURSORSHIFT | 0x04); Delay_Ms(1); }
void LCD_ShiftDisplayLeft(void) { LCD_SendCommand(LCD_CURSORSHIFT | 0x08); Delay_Ms(1); }
void LCD_ShiftDisplayRight(void){ LCD_SendCommand(LCD_CURSORSHIFT | 0x0C); Delay_Ms(1); }

void LCD_CreateChar(unsigned char location, unsigned char charmap[]) {
    location &= 0x07; // 0-7
    LCD_SendCommand(LCD_SETCGRAMADDR | (location << 3));
    for (int i = 0; i < 8; i++) LCD_SendData(charmap[i]);
    Delay_Ms(1);
}
void LCD_WriteCustomChar(unsigned char location) {
    location &= 0x07;
    LCD_SendData(location);
    Delay_Ms(1);
}

