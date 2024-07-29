#include <raylib.h>
#include <iostream>
#include <vector>

class Game2048 {
private:
    std::vector<std::vector<int>> board;
    int currentScore;
    int highScore;
    bool moved;
    const int GRID_SIZE = 4;
    const int TILE_SIZE = 200; // Size of each tile
    const int GAME_AREA_WIDTH = TILE_SIZE * GRID_SIZE; // Game area width
    const int GAME_AREA_HEIGHT = TILE_SIZE * GRID_SIZE; // Game area height
    const float ANIMATION_SPEED = 0.08f;
    bool gameOver; // Variable to track game state
    bool gameWon;
    Sound move;
    Sound win;

public:
    Game2048();
    ~Game2048(){
        UnloadSound(move);
        UnloadSound(win);
    }
    void InitGame();
    void AddRandomTile();
    void SlideTiles(int direction);
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void CheckWinCondition();
    bool IsMovePossible();
    void Draw();
    void UpdateHighScore();
    int LoadHighScore();
    void SaveHighScore();
    void Start();
    void GameOver();
};