#include "pingpong/ping_pong.h"
#include "computer.cpp"
#include "player.cpp"
#include "settings.cpp"

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
        sleep(1);
        Reset();
    }
    if (x - radius <= 0)
    { // cpu wins
        cpu_score++;
        lastScoreTime = GetTime();
        sleep(1);
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

void Ball::GameOver(GameMode& screen)
{   
    // Display "GAME OVER" message
    bool gameOver = true;
    while (gameOver && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Display the "GAME OVER" message
        DrawText("GAME OVER", 1280 / 2 - MeasureText("GAME OVER", 40) / 2, 200, 40, RED);
        DrawText("Click to Continue", 1280 / 2 - MeasureText("Click to Continue", 20) / 2, 300, 20, DARKGRAY);

        EndDrawing();

        // Check for mouse click to break the loop
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            gameOver = false;
        }
    }
}

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
    Setting option;
    
    GameMode screen = WINDOW;

    InitWindow(displayWidth, displayHeight, "Ping Pong");
    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePoint = GetMousePosition();

        BeginDrawing();
        ClearBackground(Dark_Green);

        if (screen == WINDOW) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, {(float)displayWidth / 2 - 100, 200, 200, 50})) {
                    screen = MULTIPLAYER;
                } else if (CheckCollisionPointRec(mousePoint, {(float)displayWidth / 2 - 100, 300, 200, 50})) {
                    screen = COMPUTER;
                } else if (CheckCollisionPointRec(mousePoint, {(float)displayWidth / 2 - 100, 400, 200, 50})) {
                    screen = SETTING;
                } else if (CheckCollisionPointRec(mousePoint, {(float)displayWidth / 2 - 100, 500, 200, 50})) {
                    // Switch to main game screen
                    currentScreen = MENU;  // Assuming MENU is the main menu screen
                    break;
                }
            }
            Menu();
        } else if (screen == MULTIPLAYER) {
            multiplayer.Multiplayer(screen);
            screen = WINDOW;  // Return to menu after exiting the game mode
        } else if (screen == COMPUTER) {
            comp.Bot(screen);
            screen = WINDOW;  // Return to menu after exiting the game mode
        } else if (screen == SETTING) {
            option.settingMenu(screen);

            // Update game setting after input
            if (!option.inputActive) {
                int points = atoi(option.pointsToWin);  // Convert input to integer
                if (points > 0) {
                    pointToWin = points;
                }
            }
            screen = WINDOW;
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
    DrawText("Setting", 1280/2-100 + (200 - MeasureText("Setting", 20)) / 2, 400 + (50 - 20) / 2, 20, DARKGRAY);

    DrawRectangleRec({(float)displayWidth / 2 - 100, 500, 200, 50}, WHITE);
    DrawText("Back", 1280/2-100 + (200 - MeasureText("Back", 20)) / 2, 500 + (50 - 20) / 2, 20, DARKGRAY);
}
