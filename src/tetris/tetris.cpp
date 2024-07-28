#include "tetriss/Tertris.h"
#include <fstream>
#include <iostream>

//#include <cstdio>

const char* highScoreFilePath = "assets/highscore.txt";
double lastUpdateTime = 0;

enum State { PLAYING, PAUSED, menu };

State currentGameState = PLAYING;

Tertris::Tertris()
{
    windowWidth = 500;
    windowHeight = 620;
    font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);
    Icon = LoadImage("./assets/images/tetris.png");

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

void Tertris::DrawPauseMenu() {

    const int menuX = 150;
    const int menuY = 300;
    const int menuSpacing = 50;
   
    DrawText("GAME PAUSED", menuX, menuY - 100, 40, yellow);
    DrawText("Resume", menuX, menuY-40, 30, currentGameState== PLAYING ? YELLOW : Green);

    DrawText("Back",menuX,menuY+menuSpacing-30,30,yellow);

}

void Tertris::HandlePauseMenu(Game& game) {
    Vector2 mousePoint = GetMousePosition();
  

    float text1Width=MeasureText("Resume", 30);
    float text2Width=MeasureText("Back", 30);

    Rectangle resumeButton = {150, 260, text1Width, 35};
    Rectangle backButton = {150, 320, text2Width, 35};

    if (CheckCollisionPointRec(mousePoint, resumeButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentGameState = PLAYING;
    } else if (CheckCollisionPointRec(mousePoint, backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        currentGameState = menu;
    }

    if (IsKeyPressed(KEY_ONE)) {
        currentGameState = PLAYING;
    } else if (IsKeyPressed(KEY_TWO)) {
        currentGameState = menu;
    }
}
void Tertris::Play()
{
    InitWindow(windowWidth, windowHeight, "Tertris");

    SetWindowIcon(Icon);
    UnloadImage(Icon);

    SetTargetFPS(60);

    Game game = Game();
    
    while (!WindowShouldClose())
    {
        UpdateMusicStream(game.music);

        if (IsKeyPressed(KEY_SPACE)) {
            if (currentGameState == PLAYING) {
                currentGameState = PAUSED;
            } else if (currentGameState == PAUSED) {
                currentGameState = PLAYING;
            }
        }
        
        if (currentGameState == PLAYING) {
            game.HandleInput();
            if (EventTriggered(0.5)) {
                game.MoveBlockDown();
            }
        }        

        BeginDrawing();
        ClearBackground(darkBlue);
        if (currentGameState == PLAYING) {
            DrawTextEx(font, "Score", {365, 15}, 38, 2, WHITE);
            DrawTextEx(font, "Next", {370, 175}, 38, 2, WHITE);
            if (game.gameOver) {
                if (game.score > highScore) {
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
            
        } else if (currentGameState == PAUSED) {
            DrawPauseMenu();
            HandlePauseMenu(game);
        }
        if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)){
            CloseWindow();
        }
        EndDrawing();
    }
    CloseWindow();
}