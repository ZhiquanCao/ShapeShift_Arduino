#define DEBOUNCE_TIME 30 // Debounce time in milliseconds
// #include "Game.h"


volatile bool buttonPressed = false;

void setup() {
  // Initialize Serial
  Serial.begin(9600);

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

class Game{
  /*
  Game rules: when the screen display the sphere, press left button; when the square is displayed, press the right button

  Configuration: Player1 uses pin2 (left) and pin3 (right), player2 uses pin4(left) and pin5(right)
  */
  public:
    Game(int rounds = 5);
    ~Game();

    class Player{
      public:
        int score;
        Player();
    };
    
    int rounds;
    Player* player1;
    Player* player2;
    void start_game();
    void each_round();
    void checkButtons(bool &buttonState2, bool &buttonState3, bool &buttonState4, bool &buttonState5);
    void display_result();
      
};

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
  Serial.print("game started!!!");
  // Disable pin change interrupt for pin 2 and 3 specifically
  PCMSK2 &= ~((1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21));

  while (rounds > 0){
    each_round();
  }
  display_result();
  // Re-enable pin change interrupt for pin 2 and 3
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21);
}

void Game::each_round(){
  delay(1000);
  Serial.println("Let's start the game in Three, Two, One!");
  delay(2000);
  Serial.println("Three!");
  delay(1000);
  Serial.println("Two!");
  delay(1000);
  Serial.println("One!");

  // Player1 uses pin2 (left) and pin3 (right), player2 uses pin4(left) and pin5(right)
  bool buttonState2 = false, buttonState3 = false, buttonState4 = false, buttonState5 = false;
  bool left_0_right_1 = random(2);
  bool sphere_0_square_1 = random(2);

  unsigned long start_time = millis();
  while (!(buttonState2 || buttonState3 || buttonState4 || buttonState5)) {
    checkButtons(buttonState2, buttonState3, buttonState4, buttonState5); // Non-blocking check of both buttons

    if (millis() - start_time > 5000) {
      Serial.println("What are you waiting for??");
      break;
    }
  }
  
  if (buttonState2 + buttonState3 + buttonState4 + buttonState5 > 1) {
    Serial.println("Please don't press buttons before count down!");
    return;
  }

  if (buttonState2) {
    // if this left button for player1 is pressed first
    if(!sphere_0_square_1){
      Serial.println("Player1 is quicker!");
      player1->score++;
    }else{
      Serial.println("Player1 is wrong!");
      player1->score--;
    }
  }else if (buttonState3){
    // if this right button for player1 is pressed first
    if(sphere_0_square_1){
      Serial.println("Player1 is quicker!");
      player1->score++;
    }else{
      Serial.println("Player1 is wrong!");
      player1->score--;
    }
  }else if (buttonState4) {
    // if this left button for player2 is pressed first
    if(!sphere_0_square_1){
      Serial.println("Player2 is quicker!");
      player2->score++;
    }else{
      Serial.println("Player2 is wrong!");
      player2->score--;
    }
  }else if (buttonState5){
    // if this right button for player2 is pressed first
    if(sphere_0_square_1){
      Serial.println("Player2 is quicker!");
      player2->score++;
    }else{
      Serial.println("Player2 is wrong!");
      player2->score--;
    }
  }
  delay(500);
  Serial.println("Going to next game, Bye~");
  
  rounds --;
  // Small delay to avoid flooding the serial output
  delay(2000);
}

void Game::display_result(){
  if (player1->score > player2->score){
    Serial.println("Player1 Wins!!!");
  }else{
    Serial.println("Player2 Wins!!!");
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

  bool currentButtonState2 = PIND & (1 << PIND2);
  bool currentButtonState3 = PIND & (1 << PIND3);
  bool currentButtonState4 = PIND & (1 << PIND4);
  bool currentButtonState5 = PIND & (1 << PIND5);

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


void loop() {
  if (buttonPressed) {
    Game* game = new Game();
    game->start_game();
    buttonPressed = false;
  }else{
  }
}
