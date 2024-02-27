#ifndef GAME_H
#define GAME_H

class Game {
public:
    // Constructor
    Game(int rounds = 5);

    // Destructor
    ~Game();

    // Player class
    class Player {
    public:
        int score;
        int rank;
        Player();
    };

    // Methods
    void start_game();
    void assign_ranks();

private:
    int rounds;
    Player* player1;
    Player* player2;
};

#endif
