#include "LCD.h"

MCUFRIEND_kbv tft(LCD_CS, LCD_RS, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

const int shapeSideLength = 120; // Side length for the rectangle
int iterationCounter = 0;

// uint16_t originalColor = 0x001F; // Example original color (pure blue)
// uint16_t blueComponent = originalColor & 0x001F; // Masking to extract blue component
// uint16_t adjustedBlue = blueComponent >> 1;// Example: Increase brightness by shifting blue bits to the left


void display(bool side, bool shape) {
  int16_t centerX = tft.width() / 2;
  int16_t centerY = tft.height() / 2;

  // Green background color for the entire screen
  tft.fillScreen(0x07E0); // Green background

  int shapeX, shapeY;
  if (side == 0) { // Left side
    shapeX = (centerX - shapeSideLength) / 2;
    shapeY = (centerY - shapeSideLength) / 2;
  } else { // Right side
    shapeX = centerX + (centerX - shapeSideLength) / 2;
    shapeY = (centerY - shapeSideLength) / 2;
  }

  if (shape == 0) {
    drawCircle(shapeX, shapeY);
  } else {
    drawRectangle(shapeX, shapeY);
  }
}

void drawRectangle(int x, int y) {
  tft.fillRect(x, y, shapeSideLength, shapeSideLength, 0x001F);
}

void drawCircle(int x, int y) {
  tft.fillCircle(x + shapeSideLength / 2, y + shapeSideLength / 2, shapeSideLength / 2, 0x001F);
}

void displayTextFlipped(const char* text, uint16_t bgColor, uint8_t textSize) {
  int textWidth = strlen(text) * 6 * textSize;  // Estimate width based on character count and size
  int textHeight = 8 * textSize;  // Assuming a standard font height of 8 pixels
  int screenWidth = tft.width();
  int screenHeight = tft.height();

  // Calculate the center coordinates
  int x = (screenWidth - textWidth) / 2;
  int y = (screenHeight - textHeight) / 2;

  tft.setRotation(1); // Rotate 90 degrees clockwise (adjust the value as needed)

  tft.fillRect(x, y, textWidth, textHeight, bgColor);
  tft.setCursor(x, y);
  tft.setTextColor(0x0000);  // Black color
  tft.setTextSize(textSize);
  tft.print(text);

  tft.setRotation(0); // Restore the original rotation
}

void displayText(const char* text, uint16_t bgColor, uint8_t textSize) {
  int textWidth = strlen(text) * 6 * textSize;  // Estimate width based on character count and size
  int textHeight = 8 * textSize;  // Assuming a standard font height of 8 pixels
  int screenWidth = tft.width();
  int screenHeight = tft.height();

  // Calculate the center coordinates
  int x = (screenWidth - textWidth) / 2;
  int y = (screenHeight - textHeight) / 2;

  tft.fillRect(x, y, textWidth, textHeight, bgColor);
  tft.setCursor(x, y);
  tft.setTextColor(0x0000);  // Black color
  tft.setTextSize(textSize);
  tft.print(text);
}