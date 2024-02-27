#include "Game.h"
#include "Arduino.h"

volatile bool buttonPressed = false;

void setup() {
  // Initialize Serial
  Serial.begin(115200);

  // Disable interrupts to ensure atomic access for register manipulation
  cli();

  // Set PD2 and PD3 as input pins by clearing the DDRD bits for PD2 and PD3
  DDRD &= ~(1 << DDD2) & ~(1 << DDD3) & ~(1 << DDD4) & ~(1 << DDD5);

  // Activate internal pull-up resistors for PD2 and PD3 by setting the PORTD bits for PD2 and PD3
  PORTD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5);

  // Enable pin change interrupt on PCINT18 and PCINT19, etc (Arduino Uno pins 2 and 3, etc)
  PCICR |= (1 << PCIE2); // Enable pin change interrupt for the group containing pin 2 and 3
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Enable pin change interrupt for pin 2, 3, 4, and 5

  sei(); // Enable global interrupts
  // Re-enable interrupts after register configuration is done
}

ISR(PCINT2_vect) {
  // Interrupt service routine for pin change interrupt
  buttonPressed = true;
}

void loop() {
  if (buttonPressed) {
    Serial.println("Button pressed");
    Game* game = new Game();
    game->start_game();
    buttonPressed = false;
  }else{
    Serial.println("testing");
  }
}
