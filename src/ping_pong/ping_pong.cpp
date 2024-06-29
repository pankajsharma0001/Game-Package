#include "pingpong/ping_pong.h"
#include "computer.cpp"
#include "player.cpp"
#include <iostream>

enum GameMode { WINDOW = 0, COMPUTER, MULTIPLAYER};
GameMode Screen = WINDOW;

void Ball::Draw()
{
    DrawCircle(x, y, radius, Yellow);
}

void Ball::Reset()
{
    x = GetScreenWidth() / 2;
    y = GetRandomValue(20, GetScreenHeight() - 20);
    int choices[2] = {-1, 1};
    speed_x = 7 * choices[GetRandomValue(0, 1)];
    speed_y = 7 * choices[GetRandomValue(0, 1)];
}

void Ball::Update()
{
    x += speed_x;
    y += speed_y;

    if (y + radius >= GetScreenHeight() || y - radius <= 0)
    {
        speed_y *= -1;
    }
    if (x + radius >= GetScreenWidth())
    { // player wins
        player_score++;
        lastScoreTime = GetTime();
        sleep(0.8);
        Reset();
    }
    if (x - radius <= 0)
    { // cpu wins
        cpu_score++;
        lastScoreTime = GetTime();
        sleep(0.8);
        Reset();
    }
    // Check if we need to increase the speed
    if (GetTime() - lastScoreTime > speedIncrementInterval) {
        speed_x *= speedIncrementFactor;  // Increase speed by 10%
        speed_y *= speedIncrementFactor;  // Increase speed by 10%
        lastScoreTime = GetTime();  // Reset the timer
        //printf("Speed increased: speed_x = %f, speed_y = %f\n", speed_x, speed_y);  // Debug print
    }
};

void Paddle::LimitMovement()
{
    if (y <= 0)
    {
        y = 0;
    }
    if (y + height >= GetScreenHeight())
    {
        y = GetScreenHeight() - height;
    }
}

void Paddle::Draw()
{
    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
}

void Paddle1::Update()
{
    if (IsKeyDown(KEY_W))
    {
        y -= speed;
    }
    else if (IsKeyDown(KEY_S))
    {
        y += speed;
    }
    LimitMovement();
}

void Paddle2::Update()
{
    if (IsKeyDown(KEY_UP))
    {
        y -= speed;
    }
    else if (IsKeyDown(KEY_DOWN))
    {
        y += speed;
    }
    LimitMovement();
}

void cpuPaddle::Update(int ball_height)
{
    if (y + height / 2 > ball_height)
    {
        y -= speed;
    }
    if (y + height / 2 <= ball_height)
    {
        y += speed;
    }
    LimitMovement();
}

Ball ball;
cpuPaddle paddle2;
Paddle1 paddle1;

Pong::Pong()
{
    displayWidth = 1280;
    displayHeight = 600;
    icon = LoadImage("./assets/images/ping_pong.png");
}

void Pong::Play(GameScreen& currentScreen)
{
    Computer comp;
    Player multiplayer;

    InitWindow(displayWidth, displayHeight, "Ping Pong");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePoint = GetMousePosition();

        if (Screen == WINDOW)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mousePoint, {(float)displayWidth / 2 - 100, 200, 200, 50}))
                {
                    Screen = MULTIPLAYER;
                }
                else if (CheckCollisionPointRec(mousePoint, {(float)displayWidth / 2 - 100, 300, 200, 50}))
                {
                    Screen = COMPUTER;
                }
                else if (CheckCollisionPointRec(mousePoint, {(float)displayWidth / 2 - 100, 400, 200, 50}))
                {
                    Screen = WINDOW;
                }
            }
        }

        BeginDrawing();
        ClearBackground(Dark_Green);

        if (Screen == WINDOW)
        {
            Menu();
        }
        else if (Screen == MULTIPLAYER)
        {
            multiplayer.Multiplayer();
        }
        else if (Screen == COMPUTER)
        {
            comp.Bot();
        }
        EndDrawing();
    }
    CloseWindow();
}

void Pong::Menu()
{
    DrawText("CHOOSE GAME MODE", displayWidth / 2 - MeasureText("CHOOSE GAME MODE", 40) / 2, 100, 40, DARKGRAY);
    DrawRectangleRec({(float)displayWidth / 2 - 100, 200, 200, 50}, WHITE);
    DrawText("Player", 1280/2-100 + (200 - MeasureText("Player", 20)) / 2, 200 + (50 - 20) / 2, 20, DARKGRAY);

    DrawRectangleRec({(float)displayWidth / 2 - 100, 300, 200, 50}, WHITE);
    DrawText("Computer", 1280/2-100 + (200 - MeasureText("Computer", 20)) / 2, 300 + (50 - 20) / 2, 20, DARKGRAY);

    DrawRectangleRec({(float)displayWidth / 2 - 100, 400, 200, 50}, WHITE);
    DrawText("Back", 1280/2-100 + (200 - MeasureText("Back", 20)) / 2, 400 + (50 - 20) / 2, 20, DARKGRAY);
}
