#define DEBOUNCE_TIME 30 // Debounce time in milliseconds

void setup() {
  // Initialize Serial
  Serial.begin(9600);

  // Disable interrupts to ensure atomic access for register manipulation
  noInterrupts();

  // Set PD2 and PD3 as input pins by clearing the DDRD bits for PD2 and PD3
  DDRD &= ~(1 << DDD2) & ~(1 << DDD3);

  // Activate internal pull-up resistors for PD2 and PD3 by setting the PORTD bits for PD2 and PD3
  PORTD |= (1 << PORTD2) | (1 << PORTD3);
  Serial.print(PD2);
  Serial.print(PD3);
  // Re-enable interrupts after register configuration is done
  interrupts();
}

bool debounce(int pin) {
  // Initial state is considered as high due to internal pull-up
  bool state = true;
  bool previousState = true;

  unsigned long currentTime = millis();
  unsigned long startTime = currentTime;
  
  // Check the state of the button at least two times
  while (currentTime - startTime < DEBOUNCE_TIME) {
    state = PIND & (1 << pin);

    if (state != previousState) {
      // Reset the timer because the state changed
      startTime = currentTime;
    }

    previousState = state;
    currentTime = millis();
  }

  // Return the stabilized state, inverted because the setup is active low
  return !state;
}

void loop() {
  delay(1000);
  Serial.println("Let's start the game in Three, Two, One!");
  delay(2000);
  Serial.println("Three!");
  delay(1000);
  Serial.println("Two!");
  delay(1000);
  Serial.println("One!");

  bool buttonState2 = false;
  bool buttonState3 = false;
  // Here you can use buttonState2 and buttonState3 as needed
  // For demonstration, we'll just do a simple print (not recommended for actual loop use due to speed)
  unsigned long start_time = millis();
  while (!(buttonState2 || buttonState3)) {
    if (millis() - start_time > 5000) {
      Serial.println("What are you waiting for??");
      break;
    }
    buttonState2 = debounce(PIND2);
    buttonState3 = debounce(PIND3);
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
}