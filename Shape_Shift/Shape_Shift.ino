#include "game.h"
#include "Arduino.h"

volatile bool buttonPressed = false;

void setup() {
  // Initialize Serial
  Serial.begin(9600);
  // Disable interrupts to ensure atomic access for register manipulation
  cli();

  // // Set PD2 and PD3 as input pins by clearing the DDRD bits for PD2 and PD3
  // DDRD &= ~(1 << DDD2) & ~(1 << DDD3) & ~(1 << DDD4) & ~(1 << DDD5);

  // // Activate internal pull-up resistors for PD2 and PD3 by setting the PORTD bits for PD2 and PD3
  // PORTD |= (1 << PORTD2) | (1 << PORTD3) | (1 << PORTD4) | (1 << PORTD5);
  
  // Set pins 53, 52, 51, and 50 as input pins by clearing the DDRB bits for PB0, PB1, PB2, and PB3
  DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3));

  // Activate internal pull-up resistors for pins 53, 52, 51, and 50 by setting the PORTB bits for PB0, PB1, PB2, and PB3
  PORTB |= (1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2) | (1 << PORTB3);

  // // Enable pin change interrupt on PCINT18 and PCINT19, etc (Arduino Uno pins 2 and 3, etc)
  // PCICR |= (1 << PCIE2); // Enable pin change interrupt for the group containing pin 2 and 3
  // PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21); // Enable pin change interrupt for pin 2, 3, 4, and 5

  // Enable pin change interrupt for PORTB (PCINT0 to PCINT7)
  PCICR |= (1 << PCIE0);

  // Enable pin change interrupt for pins 53 (PCINT0), 52 (PCINT1), 51 (PCINT2), and 50 (PCINT3)
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);

  sei(); // Enable global interrupts
  // Re-enable interrupts after register configuration is done
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
    Serial.println("testing");
  }
}
