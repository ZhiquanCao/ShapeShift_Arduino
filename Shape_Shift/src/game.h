#ifndef GAME_H
#define GAME_H

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

#endif
