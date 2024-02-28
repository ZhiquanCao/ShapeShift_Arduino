#include "Arduino.h"
#include "Game.h"
#include "LCD.h"


volatile bool buttonPressed = false;

void setup() {
  Serial.begin(9600);
  cli();

  // Set pins 53, 52, 51, and 50 as input pins by clearing the DDRB bits for PB0, PB1, PB2, and PB3
  DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3));

  // Activate internal pull-up resistors for pins 53, 52, 51, and 50 by setting the PORTB bits for PB0, PB1, PB2, and PB3
  PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);

  // Enable pin change interrupt for PORTB (PCINT0 to PCINT7)
  PCICR |= (1 << PCIE0);

  // Enable pin change interrupt for pins 53 (PCINT0), 52 (PCINT1), 51 (PCINT2), and 50 (PCINT3)
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);

  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(3);
  // Green background color for the entire screen
  tft.fillScreen(0x07E0);

  sei(); 
}

ISR(PCINT0_vect) {
  // Interrupt service routine for pin change interrupt
  buttonPressed = true;
}

void loop() {
  if (buttonPressed) {
    Serial.println("Button pressed");
    Game* game = new Game();
    game->start_game();
    buttonPressed = false;
    delete game;
  }else{
    displayText("Wanna Play?");
    // displayTextFlipped("Player 1 wins!", 0x07E0, 2);
    
  }
}
