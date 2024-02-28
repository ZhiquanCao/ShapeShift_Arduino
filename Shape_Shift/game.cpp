#include "Arduino.h"
#include "Game.h"
#include "LCD.h"
#define DEBOUNCE_TIME 30 // Debounce time in milliseconds

Game::Game(int rounds) : rounds(rounds) {
  player1 = new Player();
  player2 = new Player();
}

Game::~Game() {
    delete player1;
    delete player2;
}

Game::Player::Player() : score(0){};

void Game::start_game(){
  Serial.print("STARTING GAME !!!");
  // // Disable pin change interrupt
  // PCMSK2 &= ~((1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21));
  PCMSK0 &= ~ ((1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3));

  while (rounds > 0){
    each_round();
  }
  display_result();
  // Re-enable pin change interrupt
  // PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21);
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
}

void Game::each_round(){
  delay(1000);
  displayTextFlipped("Three!");
  delay(1000);
  // Serial.println("Two!");
  displayTextFlipped("Two!");
  delay(1000);
  // Serial.println("One!");
  displayTextFlipped("One!");

  // Player1 uses pin2 (left) and pin3 (right), player2 uses pin4(left) and pin5(right)
  bool buttonState2 = false, buttonState3 = false, buttonState4 = false, buttonState5 = false;
  bool left_0_right_1 = random(2);
  bool sphere_0_square_1 = random(2);
  // char* left_or_right = left_0_right_1 ? "right" : "left";

  // if (left_0_right_1){
  //   displayTextFlipped("right");
  // } 
  // else {
  //   displayTextFlipped("left");
  // }

  display(left_0_right_1, sphere_0_square_1);
  unsigned long start_time = millis();
  while (!(buttonState2 || buttonState3 || buttonState4 || buttonState5)) {
    checkButtons(buttonState2, buttonState3, buttonState4, buttonState5); // Non-blocking check of both buttons

    if (millis() - start_time > 5000) {
      displayTextFlipped("??");
      return;
    }
  }
  
  if (buttonState2 + buttonState3 + buttonState4 + buttonState5 > 1) {
    // Serial.println("Please don't press buttons before count down!");
    displayTextFlipped("Too Early!");
    return;
  }
  // displayTextFlipped("result!");

  if (buttonState2) {
    // if this left button for player1 is pressed first
    if(!sphere_0_square_1){
      displayTextFlipped("Nice!");
      player1->score++;
    }else{
      displayTextFlipped("Wrong side!");
      player1->score--;
    }
  }else if (buttonState3){
    // if this right button for player1 is pressed first
    if(sphere_0_square_1){
      displayTextFlipped("Nice!");
      player1->score++;
    }else{
      displayTextFlipped("Wrong side!");
      player1->score--;
    }
  }else if (buttonState4) {
    // if this left button for player2 is pressed first
    if(!sphere_0_square_1){
      // Serial.println("Player2 is quicker!");
      displayText("Nice!");
      player2->score++;
    }else{
      // Serial.println("Player2 is wrong!");
      displayText("Wrong side!");
      player2->score--;
    }
  }else if (buttonState5){
    // if this right button for player2 is pressed first
    if(sphere_0_square_1){
      displayText("Nice!");
      player2->score++;
    }else{
      displayText("Wrong side!");
      player2->score--;
    }
  }
  delay(1000);
  // Serial.println("Going to next game, Bye~");
  
  rounds --;
  // Small delay to avoid flooding the serial output
  delay(2000);
}

void Game::display_result(){
  if (player1->score > player2->score){
    displayTextFlipped("You WIN!!!");
  }else{
    displayText("You WIN!!!");
  }
}

// checkButtons is implemented with debouncing
void Game::checkButtons(bool &buttonState2, bool &buttonState3, bool &buttonState4, bool &buttonState5) {
  static unsigned long lastDebounceTime2 = 0;
  static unsigned long lastDebounceTime3 = 0;
  static unsigned long lastDebounceTime4 = 0;
  static unsigned long lastDebounceTime5 = 0;
  static bool lastButtonState2 = true;
  static bool lastButtonState3 = true;
  static bool lastButtonState4 = true;
  static bool lastButtonState5 = true;

  bool currentButtonState2 = PINB & (1 << PINB0); // For Pin 53
  bool currentButtonState3 = PINB & (1 << PINB1); // For Pin 52
  bool currentButtonState4 = PINB & (1 << PINB2); // For Pin 51
  bool currentButtonState5 = PINB & (1 << PINB3); // For Pin 50

  if (currentButtonState2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if (currentButtonState3 != lastButtonState3) {
    lastDebounceTime3 = millis();
  }

  if (currentButtonState4 != lastButtonState4) {
    lastDebounceTime4 = millis();
  }

  if (currentButtonState5 != lastButtonState5) {
    lastDebounceTime5 = millis();
  }

  if ((millis() - lastDebounceTime2) > DEBOUNCE_TIME) {
    buttonState2 = !currentButtonState2;
  }

  if ((millis() - lastDebounceTime3) > DEBOUNCE_TIME) {
    buttonState3 = !currentButtonState3;
  }

  if ((millis() - lastDebounceTime4) > DEBOUNCE_TIME) {
    buttonState4 = !currentButtonState4;
  }

  if ((millis() - lastDebounceTime5) > DEBOUNCE_TIME) {
    buttonState5 = !currentButtonState5;
  }

  lastButtonState2 = currentButtonState2;
  lastButtonState3 = currentButtonState3;
  lastButtonState4 = currentButtonState4;
  lastButtonState5 = currentButtonState5;
}
