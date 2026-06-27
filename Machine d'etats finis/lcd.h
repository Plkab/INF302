// lcd.h
#ifndef LCD_H
#define LCD_H

#include "stm32f4xx.h"
#include "delay.h"   // utilisation de ta lib delay

// Broches LCD sur GPIOC
#define RS_PIN  (1 << 0)   // PC0
#define EN_PIN  (1 << 1)   // PC1
#define D4_PIN  (1 << 10)  // PC10
#define D5_PIN  (1 << 11)  // PC11
#define D6_PIN  (1 << 12)  // PC12
#define D7_PIN  (1 << 13)  // PC13

// Commandes LCD
#define LCD_CLEAR           0x01
#define LCD_HOME            0x02
#define LCD_ENTRYMODE       0x04
#define LCD_DISPLAYCONTROL  0x08
#define LCD_CURSORSHIFT     0x10
#define LCD_FUNCTIONSET     0x20
#define LCD_SETCGRAMADDR    0x40
#define LCD_SETDDRAMADDR    0x80

// Flags DisplayControl
#define LCD_DISPLAYON  0x04
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

// Fonctions publiques
void LCD_Init(void);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_Print(char *str);
void LCD_PrintText(int col, int row, char *text);

// Fonctions avancées
void LCD_CursorOn(void);
void LCD_CursorOff(void);
void LCD_BlinkOn(void);
void LCD_BlinkOff(void);
void LCD_MoveCursor(int col, int row);
void LCD_CursorShiftLeft(void);
void LCD_CursorShiftRight(void);
void LCD_ShiftDisplayLeft(void);
void LCD_ShiftDisplayRight(void);
void LCD_CreateChar(unsigned char location, unsigned char charmap[]);
void LCD_WriteCustomChar(unsigned char location);

#endif



