#define DEBOUNCE_TIME 30 // Debounce time in milliseconds

volatile bool buttonPressed = false;

void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Disable interrupts to ensure atomic access for register manipulation
  cli();

  // Set PD2 and PD3 as input pins by clearing the DDRD bits for PD2 and PD3
  DDRD &= ~(1 << DDD2) & ~(1 << DDD3);

  // Activate internal pull-up resistors for PD2 and PD3 by setting the PORTD bits for PD2 and PD3
  PORTD |= (1 << PORTD2) | (1 << PORTD3);

  // Enable pin change interrupt on PCINT18 and PCINT19 (Arduino Uno pins 2 and 3)
  PCICR |= (1 << PCIE2); // Enable pin change interrupt for the group containing pin 2 and 3
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19); // Enable pin change interrupt for pin 2 and 3

  sei(); // Enable global interrupts
  // Re-enable interrupts after register configuration is done
}

ISR(PCINT2_vect) {
  // Interrupt service routine for pin change interrupt
  buttonPressed = true;
  // Additional debouncing might be necessary here
}

// checkButtons is implemented with debouncing
void checkButtons(bool &buttonState2, bool &buttonState3) {
  static unsigned long lastDebounceTime2 = 0;
  static unsigned long lastDebounceTime3 = 0;
  static bool lastButtonState2 = true;
  static bool lastButtonState3 = true;

  bool currentButtonState2 = PIND & (1 << PIND2);
  bool currentButtonState3 = PIND & (1 << PIND3);

  if (currentButtonState2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if (currentButtonState3 != lastButtonState3) {
    lastDebounceTime3 = millis();
  }

  if ((millis() - lastDebounceTime2) > DEBOUNCE_TIME) {
    buttonState2 = !currentButtonState2;
  }

  if ((millis() - lastDebounceTime3) > DEBOUNCE_TIME) {
    buttonState3 = !currentButtonState3;
  }

  lastButtonState2 = currentButtonState2;
  lastButtonState3 = currentButtonState3;
}

void startGame(){
  Serial.print("game started!!!");
  // Disable pin change interrupt for pin 2 and 3 specifically
  PCMSK2 &= ~((1 << PCINT18) | (1 << PCINT19));
  delay(1000);
  Serial.println("Let's start the game in Three, Two, One!");
  delay(2000);
  Serial.println("Three!");
  delay(1000);
  Serial.println("Two!");
  delay(1000);
  Serial.println("One!");
  bool buttonState2 = false, buttonState3 = false;

  unsigned long start_time = millis();
  while (!(buttonState2 || buttonState3)) {
    checkButtons(buttonState2, buttonState3); // Non-blocking check of both buttons

    if (millis() - start_time > 5000) {
      Serial.println("What are you waiting for??");
      break;
    }
  }
  
  if (buttonState2) {
    Serial.println("Button on pin 2 pressed first!");
  }else if (buttonState3){
    Serial.println("Button on pin 3 pressed first!");
  }else{
    Serial.println("No button is pressed \n");
  }
  delay(500);
  Serial.println("Going to next game, Bye~");
  
  // Small delay to avoid flooding the serial output
  delay(2000);
  // Re-enable pin change interrupt for pin 2 and 3
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
}


void loop() {
  if (buttonPressed) {
    Serial.print("Button pressed");
    startGame();
    buttonPressed = false;
  }else{
    Serial.print("Wanna Play?");
  }
}
