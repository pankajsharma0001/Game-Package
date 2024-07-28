#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <./2048/2048.h>

using namespace std;

Game2048::Game2048()
{
    board.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    currentScore = 0;
    highScore = LoadHighScore();
    move = LoadSound("./assets/sounds/move_block.mp3");
    win = LoadSound("./assets/sounds/winning_sound.mp3");
    gameOver = false; // Initialize game as not over
    InitGame();
}

void Game2048::InitGame()
{
    // srand(static_cast<unsigned int>(time(nullptr)));
    AddRandomTile();
    AddRandomTile();
}

void Game2048::AddRandomTile()
{
    int emptyCells = 0;
    for (const auto &row : board)
    {
        for (int value : row)
        {
            if (value == 0)
                emptyCells++;
        }
    }

    if (emptyCells == 0)
        return;

    int target = rand() % emptyCells;
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (board[i][j] == 0)
            {
                if (target == 0)
                {
                    board[i][j] = (rand() % 5 == 0) ? 2 : 4;
                    return;
                }
                target--;
            }
        }
    }
}

void Game2048::SlideTiles(int direction)
{

    if (gameOver)
        return; // Prevent moves if game is over

    moved = false;
    if (direction == 0)
        MoveUp();
    else if (direction == 1)
        MoveRight();
    else if (direction == 2)
        MoveDown();
    else if (direction == 3)
        MoveLeft();

    if (moved)
    {
        AddRandomTile();
        UpdateHighScore();
        CheckWinCondition(); // Check for win after a move
    }
}

void Game2048::MoveUp()
{
    for (int col = 0; col < GRID_SIZE; col++)
    {
        for (int row = 1; row < GRID_SIZE; row++)
        {
            if (board[row][col] != 0)
            {
                int r = row;
                while (r > 0 && (board[r - 1][col] == 0 || board[r - 1][col] == board[r][col]))
                {
                    if (board[r - 1][col] == board[r][col])
                    {
                        board[r - 1][col] *= 2;
                        currentScore += board[r - 1][col];
                        board[r][col] = 0;
                        moved = true;
                        break;
                    }
                    else if (board[r - 1][col] == 0)
                    {
                        board[r - 1][col] = board[r][col];
                        board[r][col] = 0;
                        moved = true;
                    }
                    r--;
                }
                if (moved == true)
                {
                    PlaySound(move);
                }
            }
        }
    }
}

void Game2048::MoveDown()
{
    for (int col = 0; col < GRID_SIZE; col++)
    {
        for (int row = GRID_SIZE - 2; row >= 0; row--)
        {
            if (board[row][col] != 0)
            {
                int r = row;
                while (r < GRID_SIZE - 1 && (board[r + 1][col] == 0 || board[r + 1][col] == board[r][col]))
                {
                    if (board[r + 1][col] == board[r][col])
                    {
                        board[r + 1][col] *= 2;
                        currentScore += board[r + 1][col];
                        board[r][col] = 0;
                        moved = true;
                        break;
                    }
                    else if (board[r + 1][col] == 0)
                    {
                        board[r + 1][col] = board[r][col];
                        board[r][col] = 0;
                        moved = true;
                    }
                    r++;
                }
                if (moved == true)
                {
                    PlaySound(move);
                }
            }
        }
    }
}

void Game2048::MoveLeft()
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = 1; col < GRID_SIZE; col++)
        {
            if (board[row][col] != 0)
            {
                int c = col;
                while (c > 0 && (board[row][c - 1] == 0 || board[row][c - 1] == board[row][c]))
                {
                    if (board[row][c - 1] == board[row][c])
                    {
                        board[row][c - 1] *= 2;
                        currentScore += board[row][c - 1];
                        board[row][c] = 0;
                        moved = true;
                        break;
                    }
                    else if (board[row][c - 1] == 0)
                    {
                        board[row][c - 1] = board[row][c];
                        board[row][c] = 0;
                        moved = true;
                    }
                    c--;
                }
                if (moved == true)
                {
                    PlaySound(move);
                }
            }
        }
    }
}

void Game2048::MoveRight()
{
    for (int row = 0; row < GRID_SIZE; row++)
    {
        for (int col = GRID_SIZE - 2; col >= 0; col--)
        {
            if (board[row][col] != 0)
            {
                int c = col;
                while (c < GRID_SIZE - 1 && (board[row][c + 1] == 0 || board[row][c + 1] == board[row][c]))
                {
                    if (board[row][c + 1] == board[row][c])
                    {
                        board[row][c + 1] *= 2;
                        currentScore += board[row][c + 1];
                        board[row][c] = 0;
                        moved = true;
                        break;
                    }
                    else if (board[row][c + 1] == 0)
                    {
                        board[row][c + 1] = board[row][c];
                        board[row][c] = 0;
                        moved = true;
                    }
                    c++;
                }
                if (moved == true)
                {
                    PlaySound(move);
                }
            }
        }
    }
}

void Game2048::CheckWinCondition()
{
    int count = 0;
    for (const auto &row : board)
    {
        for (int value : row)
        {
            if (value == 2048)
            {
                PlaySound(win);
                gameOver = true; // Set game over if tile reaches 2048
            }
            if(value == 0){
                count++;
            }
        }
    }
    if(count == 0){
        GameOver();
    }
}

void Game2048::Draw()
{
    Color endtext = (Color){143, 36, 102, 255};
    DrawText(("Score: " + to_string(currentScore)).c_str(), 10, 30, 25, BLACK);
    DrawText(("High Score: " + to_string(highScore)).c_str(), 570, 30, 25, BLACK);
    DrawText("2048", 300, 5, 80, BLACK);

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            Rectangle tile = {static_cast<float>(j * TILE_SIZE), static_cast<float>(i * TILE_SIZE + 80), TILE_SIZE, TILE_SIZE};
            Color color = (board[i][j] == 0) ? LIGHTGRAY : (board[i][j] == 2) ? GREEN
                                                       : (board[i][j] == 4)   ? BLUE
                                                       : (board[i][j] == 8)   ? SKYBLUE
                                                       : (board[i][j] == 16)  ? YELLOW
                                                       : (board[i][j] == 32)  ? DARKGRAY
                                                       : (board[i][j] == 64)  ? RED
                                                       : (board[i][j] == 128) ? VIOLET
                                                       : (board[i][j] == 256) ? DARKPURPLE
                                                       : (board[i][j] == 512) ? PURPLE
                                                                              : PINK;

            DrawRectangleRec(tile, color);        // Draw tile
            DrawRectangleLinesEx(tile, 2, BLACK); // raw border
            // DrawRectangleGradientH(tile, 5, BLACK);

            if (board[i][j] != 0)
            {
                if (board[i][j] < 10)
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 80, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
                if (board[i][j] > 10 && board[i][j] < 100)
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 65, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
                if (board[i][j] > 100 && board[i][j] < 1000)
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 50, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
                if (board[i][j] > 1000)
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 30, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
            }
        }
    }

    // Draw win message
    if (gameOver)
    {
        DrawText("Congratulations! You Win!", 180, GAME_AREA_HEIGHT / 2 - 20, 40, endtext);
    }
}

void Game2048::UpdateHighScore()
{
    if (currentScore > highScore)
    {
        highScore = currentScore;
        SaveHighScore();
    }
}

int Game2048::LoadHighScore()
{
    int score = 0;
    std::ifstream infile("highscore.txt");
    if (infile.is_open())
    {
        infile >> score;
        infile.close();
    }
    return score;
}

void Game2048::SaveHighScore()
{
    std::ofstream outfile("highscore.txt");
    if (outfile.is_open())
    {
        outfile << highScore;
        outfile.close();
    }
}

void Game2048::GameOver(){
    bool gameOver = true;
    while (gameOver && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display the "GAME OVER" message
        DrawText("GAME OVER", 800 / 2 - MeasureText("GAME OVER", 40) / 2, 200, 40, RED);
        DrawText("Click to Continue", 800 / 2 - MeasureText("Click to Continue", 20) / 2, 300, 20, DARKGRAY);

        EndDrawing();

        // Check for mouse click to break the loop
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i=0; i<4; i++)
            {
                for (int j=0; j<4; j++)
                {
                    board[i][j] = 0;
                }
            }
            gameOver = false;
        }
    }

}
void Game2048::Start()
{
    // Set the window size to 800x890
    InitWindow(800, 880, "2048 Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Game2048 game;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_UP))
            game.SlideTiles(0);
        if (IsKeyPressed(KEY_RIGHT))
            game.SlideTiles(1);
        if (IsKeyPressed(KEY_DOWN))
            game.SlideTiles(2);
        if (IsKeyPressed(KEY_LEFT))
            game.SlideTiles(3);

        
        BeginDrawing();
        ClearBackground(ORANGE);
        game.Draw();
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
}
