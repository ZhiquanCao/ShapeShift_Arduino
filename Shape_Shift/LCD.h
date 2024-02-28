#ifndef LCD_H
#define LCD_H

#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_RS A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define YP A2
#define XM A3
#define YM 8
#define XP 9

extern MCUFRIEND_kbv tft;
extern TouchScreen ts;

extern const int shapeSideLength;
extern int iterationCounter;
// int lastX, lastY;
// int lastWidth, lastHeight;

void displayTextFlipped(char* text, uint16_t bgColor = 0x0000, uint8_t textSize = 5);
void displayText(char* text, uint16_t bgColor = 0x0000, uint8_t textSize = 5);
void display(bool side, bool shape);
void drawRectangle(int x, int y);
void drawCircle(int x, int y);

#endif
