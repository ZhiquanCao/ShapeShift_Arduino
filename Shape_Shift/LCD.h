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

void displayTextFlipped(const char* text, uint16_t bgColor, uint8_t textSize);
void displayText(const char* text, uint16_t bgColor, uint8_t textSize);
void display(bool side, bool shape);
void drawRectangle(int x, int y);
void drawCircle(int x, int y);

#endif
