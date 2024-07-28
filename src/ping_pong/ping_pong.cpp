#include "pingpong/ping_pong.h"
#include "computer.cpp"
#include "player.cpp"
#include "settings.cpp"
#define NUM_FRAMES  3       // Number of frames (rectangles) for the button sprite texture

const float buttonWidth = 224.0f;
const float buttonHeight = 318.0f;

float frame_Height = (float)buttonHeight/NUM_FRAMES;

Rectangle source_Rec = { 0, 0, buttonWidth, frame_Height };

Rectangle btn_Bounds[4] = { 
    {1280/2.0f - buttonWidth/2.0f, 110, buttonWidth, frame_Height},
    {1280/2.0f - buttonWidth/2.0f, 230, buttonWidth, frame_Height},
    {1280/2.0f - buttonWidth/2.0f, 350, buttonWidth, frame_Height},
    {1280/2.0f - buttonWidth/2.0f, 470, buttonWidth, frame_Height}};
    
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

void Paddle::Draw(Color color)
{
    DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, color);
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

    Texture2D background_image = LoadTexture("./assets/images/ping_pong_background.png");
    Texture2D player_button = LoadTexture("./assets/images/player_button.png");
    Texture2D computer_button = LoadTexture("./assets/images/computer_button.png");
    Texture2D setting_button = LoadTexture("./assets/images/setting_button.png");
    Texture2D back_button = LoadTexture("./assets/images/quit_button.png");

    SetWindowIcon(icon);
    UnloadImage(icon);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        Vector2 mousePoint = GetMousePosition();

        BeginDrawing();
        ClearBackground(Dark_Green);
        DrawTexture(background_image, 0, 0, WHITE);


        if (screen == WINDOW) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, btn_Bounds[0])) {
                    screen = MULTIPLAYER;
                } else if (CheckCollisionPointRec(mousePoint, btn_Bounds[1])) {
                    screen = COMPUTER;
                } else if (CheckCollisionPointRec(mousePoint, btn_Bounds[2])) {
                    screen = SETTING;
                } else if (CheckCollisionPointRec(mousePoint, btn_Bounds[3])) {
                    CloseWindow();
                    //Switch to main game screen
                    //currentScreen = MENU; 
                    break; // Assuming MENU is the main menu screen
                }
            }
            Menu(player_button, computer_button, setting_button, back_button);
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
    UnloadTexture(background_image);
    UnloadTexture(player_button);
    UnloadTexture(computer_button);
    UnloadTexture(setting_button);
    UnloadTexture(back_button);
}

void Pong::Menu(Texture2D& player_button, Texture2D& computer_button, Texture2D& setting_button, Texture2D& back_button)
{
    Texture2D buttons[4] = {player_button, computer_button, setting_button, back_button};
    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

    Vector2 mousePoint = GetMousePosition();

    DrawText("CHOOSE GAME MODE", displayWidth / 2 - MeasureText("CHOOSE GAME MODE", 40) / 2, 50, 40, DARKGRAY);

    for (int i = 0; i < 4; i++)
    {
        // Check button state
        if (CheckCollisionPointRec(mousePoint, btn_Bounds[i]))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;
        }

        else btnState = 0;
    // Calculate button frame rectangle to draw depending on button state
        source_Rec.y = btnState*frame_Height;
        DrawTextureRec(buttons[i], source_Rec, (Vector2){ btn_Bounds[i].x, btn_Bounds[i].y }, WHITE); // Draw button frame
    }
}
