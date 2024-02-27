#include "Game.h"

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

// Constructor implementation
Game::Game(int rounds) : rounds(rounds) {
    player1 = new Player();
    player2 = new Player();
}

// Destructor implementation
Game::~Game() {
    delete player1;
    delete player2;
}

// Player constructor implementation
Game::Player::Player() : score(0), rank(0) {}

// Method to assign ranks to players based on their scores
void Game::assign_ranks() {
    // Create an array of pointers to players
    Player* players[] = {player1, player2};

    // Sort players based on their scores in descending order
    for (int i = 0; i < 2; ++i) {
        for (int j = i + 1; j < 2; ++j) {
            if (players[i]->score < players[j]->score) {
                Player* temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    // Assign ranks to players
    for (int i = 0; i < 2; ++i) {
        players[i]->rank = i + 1;
    }
}

// Method to start the game
void Game::start_game() {
    // Simulated game logic for demonstration
    player1->score = 10;
    player2->score = 5;

    // Assign ranks
    assign_ranks();

    // Display ranks
    Serial.println("Game Over!");
    Serial.println("Rank\tPlayer\tScore");
    Serial.print(player1->rank);
    Serial.print("\tPlayer 1\t");
    Serial.println(player1->score);
    Serial.print(player2->rank);
    Serial.print("\tPlayer 2\t");
    Serial.println(player2->score);
}

void setup() {
    // Initialize Serial communication
    Serial.begin(9600);

    // Create a game instance
    Game game;

    // Start the game
    game.start_game();
}

void loop() {
    // Empty loop
}
