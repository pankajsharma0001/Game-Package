#include "./include/ping_pong/ping_pong.h"

using namespace std;

void Ball::Draw()
{
    DrawCircle(x, y, radius, Yellow);
}

void Ball::Reset()
{
    x = GetScreenWidth() / 2;
    y = GetRandomValue(20, GetScreenHeight() - 20);
    int choices[2] = {-1, 1};
    speed_x *= choices[GetRandomValue(0, 1)];
    speed_y *= choices[GetRandomValue(0, 1)];
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
        sleep(0.8);
        Reset();
    }
    if (x - radius <= 0)
    { // cpu wins
        cpu_score++;
        sleep(0.8);
        Reset();
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

void Pong::Play()
{
    Ball ball;
    cpuPaddle paddle2;
    Paddle1 paddle1;

    InitWindow(displayWidth, displayHeight, "Ping Pong");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);

    ball.x = displayWidth / 2;
    ball.y = displayHeight / 2;
    ball.radius = 15;
    ball.speed_x = 7;
    ball.speed_y = 7;

    paddle1.width = 25;
    paddle1.height = 120;
    paddle1.x = 10;
    paddle1.y = displayHeight / 2 - paddle1.height / 2;
    paddle1.speed = 6;

    paddle2.width = 25;
    paddle2.height = 120;
    paddle2.x = displayWidth - paddle2.width - 10;
    paddle2.y = displayHeight / 2 - paddle1.height / 2;
    paddle2.speed = 6;

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ball.Update();
        paddle1.Update();
        paddle2.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle1.x, paddle1.y, paddle1.width, paddle1.height}))
        {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle2.x, paddle2.y, paddle2.width, paddle2.height}))
        {
            ball.speed_x *= -1;
        }

        ClearBackground(Dark_Green);
        DrawRectangle(displayWidth / 2, 0, displayWidth / 2, displayHeight, Green);
        DrawCircle(displayWidth / 2, displayHeight / 2, 150, Light_Green);
        DrawLine(displayWidth / 2, 0, displayWidth / 2, displayHeight, WHITE);
        ball.Draw();
        paddle1.Draw();
        paddle2.Draw();
        DrawText(TextFormat("%i", player_score), displayWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu_score), 3 * displayWidth / 4 - 20, 20, 80, WHITE);

        EndDrawing();
    }
    CloseWindow();
}
