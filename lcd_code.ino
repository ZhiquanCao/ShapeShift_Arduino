#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#define LCD_CS A3
#define LCD_RS A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

MCUFRIEND_kbv tft;
#define YP A2
#define XM A3
#define YM 8
#define XP 9

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

const int shapeSideLength = 120; // Side length for the rectangle
int iterationCounter = 0;

void setup() {
  Serial.begin(9600);

  tft.reset();
  tft.begin(0x9486);
  tft.setRotation(3);

  // Green background color for the entire screen
  tft.fillScreen(0x07E0);
}

void loop() {
  if (iterationCounter == 0) {
    // Randomly choose the side: 0 for left, 1 for right
    //int side = random(2);
    drawRandomShape(side);
    iterationCounter++;
  } else {
    // Display the next 4 iterations after the user inputs 'x'
    if (Serial.available() > 0) {
      char input = Serial.read();
      if (input == 'x' || input == 'X') {
        if (iterationCounter < 5) {
          // Randomly choose the side: 0 for left, 1 for right
          //int side = random(2);
          drawRandomShape(side);
          iterationCounter++;
        }
      }
    }
  }
}

void drawRandomShape(int side) {
  int16_t centerX = tft.width() / 2;
  int16_t centerY = tft.height() / 2;

  // Green background color for the entire screen
  tft.fillScreen(0x07E0);

  if (side == 0) { // Left side
    int shapeX = (centerX - shapeSideLength) / 2;
    int shapeY = (tft.height() - shapeSideLength) / 2;
    // Randomly choose the shape: 0 for rectangle, 1 for circle
    int shapeType = random(2);
    if (shapeType == 0) {
      drawRectangle(shapeX, shapeY);
    } else {
      drawCircle(shapeX, shapeY);
    }
  } else { // Right side
    int shapeX = centerX + (centerX - shapeSideLength) / 2;
    int shapeY = (tft.height() - shapeSideLength) / 2;
    // Randomly choose the shape: 0 for rectangle, 1 for circle
    int shapeType = random(2);
    if (shapeType == 0) {
      drawRectangle(shapeX, shapeY);
    } else {
      drawCircle(shapeX, shapeY);
    }
  }
}

void drawRectangle(int x, int y) {
  tft.fillRect(x, y, shapeSideLength, shapeSideLength, 0x001F);
}

void drawCircle(int x, int y) {
  tft.fillCircle(x + shapeSideLength / 2, y + shapeSideLength / 2, shapeSideLength / 2, 0x001F);
}
