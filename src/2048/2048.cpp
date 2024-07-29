#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include "./2048/2048.h"

using namespace std;

Game2048::Game2048()
{
    board.resize(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    currentScore = 0;
    highScore = LoadHighScore();
    move = LoadSound("./assets/sounds/move_block.mp3");
    win = LoadSound("./assets/sounds/winning_sound.mp3");
    gameOver = false; // Initialize game as not over
    gameWon = false;
    InitGame();
}

void Game2048::InitGame()
{
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
    if (gameOver || gameWon)
        return; // Prevent moves if game is over or won

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
        if (!IsMovePossible())
        {
            gameOver = true;
        }
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
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (board[i][j] == 2048)
            {
                PlaySound(win);
                gameWon = true; // Set game won if tile reaches 2048
            }
        }
    }
}

bool Game2048::IsMovePossible()
{
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (board[i][j] == 0)
                return true; // Move is possible if there is an empty tile

            // Check adjacent tiles for possible merges
            if ((i > 0 && board[i][j] == board[i - 1][j]) ||
                (i < GRID_SIZE - 1 && board[i][j] == board[i + 1][j]) ||
                (j > 0 && board[i][j] == board[i][j - 1]) ||
                (j < GRID_SIZE - 1 && board[i][j] == board[i][j + 1]))
            {
                return true; // Move is possible if there are adjacent tiles with the same number
            }
        }
    }
    return false; // No moves possible
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
            Rectangle tile = {static_cast<float>(j * TILE_SIZE), static_cast<float>(i * TILE_SIZE + 80), (float)TILE_SIZE, (float)TILE_SIZE};
            Color color = (board[i][j] == 0)     ? BEIGE
                          : (board[i][j] == 2)    ? LIGHTGRAY
                          : (board[i][j] == 4)    ? GRAY
                          : (board[i][j] == 8)    ? SKYBLUE
                          : (board[i][j] == 16)   ? YELLOW
                          : (board[i][j] == 32)   ? DARKGRAY
                          : (board[i][j] == 64)   ? RED
                          : (board[i][j] == 128)  ? VIOLET
                          : (board[i][j] == 256)  ? DARKPURPLE
                          : (board[i][j] == 512)  ? PURPLE
                                                   : PINK;

            DrawRectangleRec(tile, color);        // Draw tile
            DrawRectangleLinesEx(tile, 2, BLACK); // Draw border

            if (board[i][j] != 0)
            {
                if (board[i][j] < 10)
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 80, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
                else if (board[i][j] < 100)
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 65, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
                else if (board[i][j] < 1000)
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 50, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
                else
                {
                    DrawText(std::to_string(board[i][j]).c_str(), j * TILE_SIZE + 30, i * TILE_SIZE + 80 + 70, 70, WHITE);
                }
            }
        }
    }

    // Draw win message
    if (gameWon)
    {
        DrawText("Congratulations! You Win!", 180, GAME_AREA_HEIGHT / 2 - 20, 40, endtext);
        DrawText("Click to Restart", 800 / 2 - MeasureText("Click to Restart", 20) / 2, 300, 40, DARKGRAY);

        // Check for mouse click to restart the game
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = 0; j < GRID_SIZE; j++)
                {
                    board[i][j] = 0;
                }
            }
            AddRandomTile();
            AddRandomTile();
            currentScore = 0;
            gameWon = false;
        }
    }
    // Draw game over message
    if (gameOver)
    {
        DrawText("GAME OVER", 800 / 2 - MeasureText("GAME OVER", 40) / 2, 200, 60, RED);
        DrawText("Click to Continue", 800 / 2 - MeasureText("Click to Continue", 20) / 2, 300, 40, DARKGRAY);
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

void Game2048::GameOver()
{
    bool continueGame = false;
    while (!continueGame && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display the "GAME OVER" message
        DrawText("GAME OVER", 800 / 2 - MeasureText("GAME OVER", 40) / 2, 200, 60, RED);
        DrawText("Click to Continue", 800 / 2 - MeasureText("Click to Continue", 20) / 2, 300, 40, DARKGRAY);

        EndDrawing();

        // Check for mouse click to break the loop
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            continueGame = true;
            for (int i = 0; i < GRID_SIZE; i++)
            {
                for (int j = 0; j < GRID_SIZE; j++)
                {
                    board[i][j] = 0;
                }
            }
            AddRandomTile();
            AddRandomTile();
            currentScore = 0;
            gameOver = false;
            gameWon = false; // Reset gameWon status
        }
    }
}

void Game2048::Start()
{
    // Set the window size to 800x880
    InitWindow(800, 880, "2048 Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Game2048 game;

    while (!WindowShouldClose())
    {
        if (!game.gameOver && !game.gameWon)
        {
            if (IsKeyPressed(KEY_UP))
                game.SlideTiles(0);
            if (IsKeyPressed(KEY_RIGHT))
                game.SlideTiles(1);
            if (IsKeyPressed(KEY_DOWN))
                game.SlideTiles(2);
            if (IsKeyPressed(KEY_LEFT))
                game.SlideTiles(3);
        }
        else if (game.gameOver)
        {
            game.GameOver();
        }
        if(IsKeyPressed(KEY_ESCAPE)){
            CloseWindow();
        }
        BeginDrawing();
        ClearBackground(ORANGE);
        game.Draw();
        EndDrawing();
    }
    CloseAudioDevice();
    CloseWindow();
}
