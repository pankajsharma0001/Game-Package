#include "tetriss/Tertris.h"
#include <fstream>
//#include <cstdio>

const char* highScoreFilePath = "assets/highscore.txt";
double lastUpdateTime = 0;

Tertris::Tertris()
{
    windowWidth = 500;
    windowHeight = 620;
    font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    highScore = LoadHighScore();
}

void Tertris::SaveHighScore(int score) {
    std::ofstream file(highScoreFilePath, std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        file << score;
        file.close();
    } else {
        std::cerr << "Unable to open file for writing high score!" << std::endl;
    }
}

int Tertris::LoadHighScore() {
    int highScore = 0;
    std::ifstream file(highScoreFilePath);
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        std::cerr << "Unable to open file for reading high score!" << std::endl;
    }
    return highScore;
}

bool Tertris::EventTriggered(double interval)
{
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

void Tertris::Play()
{
    InitWindow(windowWidth, windowHeight, "Tertris");
    SetTargetFPS(60);

    Game game = Game();

    while (!WindowShouldClose())
    {
        UpdateMusicStream(game.music);
        game.HandleInput();
        if (EventTriggered(0.3))
        {
            game.MoveBlockDown();
        }
        // if(IsKeyDown(KEY_ENTER)){
        //     currentScreen = MENU;
        //     break;
        // }
        BeginDrawing();
        ClearBackground(darkBlue);
        DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
        DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
        if(game.gameOver){
            if(game.score > highScore){
                highScore = game.score;
                SaveHighScore(highScore);
            }
            char highscor[10];
            sprintf(highscor, "%d", highScore);
            DrawTextEx(font, "Game Over", {320, 430}, 38, 2, WHITE);
            DrawTextEx(font, "High Score", {320, 490}, 35, 2, WHITE);
            DrawTextEx(font, highscor, {370, 550}, 38, 2, WHITE);
        }
        DrawRectangleRounded({320, 55, 170, 60}, 0.3, 6, lightBlue);

        char scoreText[10];
        sprintf(scoreText, "%d", game.score);
        Vector2 textSize = MeasureTextEx(font, scoreText, 38, 2);
        DrawTextEx(font, scoreText, {320 + (170 - textSize.x) / 2, 65}, 38, 2, WHITE);
        DrawRectangleRounded({320, 215, 170, 180}, 0.3, 6, lightBlue);
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
}