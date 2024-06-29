#include "raylib.h"
#include "ping_pong/ping_pong.cpp"
#include "tetris/tetris.cpp"
#include "tetris/game.cpp"
#include "tetris/block.cpp"
#include "tetris/colors.cpp"
#include "tetris/grid.cpp"
#include "tetris/position.cpp"
#include "screen.h"

const int screenWidth = 800;
const int screenHeight = 600;

const char *buttonTexts[4] = {"PLAY", "MUSIC SETTING", "HELP", "QUIT"};
const char *button2Text[4] = {"PING PONG", "TETRIS", "2048", "BACK"};
const char *helpButtonTexts[4] = {"PING PONG", "TETRIS", "2048", "BACK"};
const char *musicButtonsText[4] = {"VOLUME", "INDIVIDUAL VOLUME", "MUTE", "BACK"};

Color buttonColors[4] = {WHITE, WHITE, WHITE, WHITE};
Color helpButtonColors[4] = {WHITE, WHITE, WHITE, WHITE};
Color musicButtonColors[4] = {WHITE, WHITE, WHITE, WHITE};
Color optionButtonColors[2] = {WHITE, WHITE};

Rectangle buttons[4] = {
    {screenWidth / 2 - 100, 200, 200, 50},
    {screenWidth / 2 - 100, 300, 200, 50},
    {screenWidth / 2 - 100, 400, 200, 50},
    {screenWidth / 2 - 100, 500, 200, 50}};

Rectangle helpButtons[4] = {
    {screenWidth / 2 - 100, 100, 200, 50},
    {screenWidth / 2 - 100, 200, 200, 50},
    {screenWidth / 2 - 100, 300, 200, 50},
    {screenWidth / 2 - 100, 400, 200, 50}};

Rectangle musicButtons[4] = {
    {screenWidth / 2 - 100, 100, 200, 50},
    {screenWidth / 2 - 100, 200, 200, 50},
    {screenWidth / 2 - 100, 300, 200, 50},
    {screenWidth / 2 - 100, 400, 200, 50}};

Rectangle optionButtons[2] = {
    {screenWidth / 2 + 120, 200, 50, 50},  // Plus button
    {screenWidth / 2 + 180, 200, 50, 50}}; // Minus button

Rectangle backButton = {screenWidth / 2 - 100, 300, 200, 50}; // Back button in options menu

int volume;

class Main{
public:
    Music background;
    Main()
    {
        InitAudioDevice();
        background = LoadMusicStream("./assets/sounds/background.mp3");
        PlayMusicStream(background);
        background.looping = true;
        volume = 50;
        SetMusicVolume(background, volume/100.0f);
    }
    ~Main(){
        StopMusicStream(background);
        UnloadMusicStream(background);
        CloseAudioDevice();
    }
};

void DrawMenu();
void DrawPlayOptions();
void DrawOptionsMenu();
void DrawHelpMenu();
void DrawPINGPONGHELP();
void Drawtertishelp();
void help();

 GameScreen currentScreen = MENU;

int main()
{
    Main gam;
    Pong pong;
    Tertris tertris;
    // Initialization
    InitWindow(screenWidth, screenHeight, "ClassicFun: Timeless Games Collection");

   
    SetTargetFPS(60);
    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        UpdateMusicStream(gam.background);
        Vector2 mousePoint = GetMousePosition();
        if (currentScreen == MENU)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mousePoint, buttons[0]))
                {
                    currentScreen = PLAY;
                }
                else if (CheckCollisionPointRec(mousePoint, buttons[1]))
                {
                    currentScreen = OPTIONS;
                }
                else if (CheckCollisionPointRec(mousePoint, buttons[2]))
                {
                    currentScreen = HELP_MENU;
                }
                else if (CheckCollisionPointRec(mousePoint, buttons[3]))
                {
                    CloseWindow();
                    return 0;
                }
            }
        }
        else if (currentScreen == PLAY)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mousePoint, buttons[0]))
                {
                    currentScreen = PING_PONG;
                }
                else if (CheckCollisionPointRec(mousePoint, buttons[1]))
                {
                    currentScreen = TERTRIS;
                }
                else if (CheckCollisionPointRec(mousePoint, buttons[2]))
                {
                    currentScreen = T048;
                }
                else if (CheckCollisionPointRec(mousePoint, buttons[3]))
                {
                    currentScreen = MENU;
                }
            }
        }

        else if (currentScreen == OPTIONS)
        {
            if (CheckCollisionPointRec(mousePoint, optionButtons[0])) // Plus button
            {
                optionButtonColors[0] = BROWN;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (volume < 100)
                        volume += 10;
                    SetMusicVolume(gam.background, volume);
                }
            }
            else
            {
                optionButtonColors[0] = WHITE;
            }

            if (CheckCollisionPointRec(mousePoint, optionButtons[1])) // Minus button
            {
                optionButtonColors[1] = BROWN;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    if (volume > 0)
                        volume -= 10;
                    SetMusicVolume(gam.background, volume);
                }
            }
            else
            {
                optionButtonColors[1] = WHITE;
            }

            if (CheckCollisionPointRec(mousePoint, backButton))
            {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    currentScreen = MENU; // Back to main menu
                }
            }
        }
        else if (currentScreen == HELP_MENU)
        {
            for (int i = 0; i < 4; i++)
            {
                if (CheckCollisionPointRec(mousePoint, helpButtons[i]))
                {
                    helpButtonColors[i] = BROWN;
                    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        if (i == 0)
                            currentScreen = PINGPONG_INSTRUCTIONS; // Switch to PING PONG instructions
                        else if (i == 1)
                            currentScreen = TERTIS_INSTRUCTIONS;
                        else if (i == 2)
                            currentScreen = INSTUCTIONS;
                        else if (i == 3)
                            currentScreen = MENU; // Back button
                    }
                }
                else
                {
                    helpButtonColors[i] = WHITE;
                }
            }
        }
        else if (currentScreen == PINGPONG_INSTRUCTIONS ||
                 currentScreen == TERTIS_INSTRUCTIONS ||
                 currentScreen == INSTUCTIONS)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentScreen = HELP_MENU; // Back to HELP menu on click
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU)
        {
            DrawMenu();
        }
        else if (currentScreen == PLAY)
        {
            DrawPlayOptions();
        }
        else if (currentScreen == OPTIONS)
        {
            DrawOptionsMenu();
        }
        else if (currentScreen == HELP_MENU)
        {
            DrawHelpMenu();
        }
        else if (currentScreen == PINGPONG_INSTRUCTIONS)
        {
            DrawPINGPONGHELP();
        }
        else if (currentScreen == TERTIS_INSTRUCTIONS)
        {
            Drawtertishelp();
        }
        else if (currentScreen == INSTUCTIONS)
        {
            help();
        }
        else if (currentScreen == PING_PONG)
        {
            gam.~Main();
            CloseWindow();
            // InitWindow(1280, 600, "Ping Pong");
            pong.Play(currentScreen);
            // if (currentScreen == MENU)
            // {
            //     InitWindow(screenWidth, screenHeight, "ClassicFun: Timeless Games Collection");
            //     PlayMusicStream(gam.background); // Restart background music if required
            // }
        }
        else if (currentScreen == TERTRIS)
        {
            gam.~Main();
            CloseWindow();
            // InitWindow(500, 620, "Tertris");
            tertris.Play();
            // if (currentScreen == MENU)
            // {
            //     InitWindow(screenWidth, screenHeight, "ClassicFun: Timeless Games Collection");
            //     PlayMusicStream(gam.background); // Restart background music if required
            // }
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

void DrawMenu()
{
    Vector2 mousePoint = GetMousePosition();

    DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 40) / 2, 100, 40, DARKGRAY);
    for (int i = 0; i < 4; i++)
    {
        buttonColors[i] = LIGHTGRAY;
        if (CheckCollisionPointRec(mousePoint, buttons[i]))
        {
            buttonColors[i] = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? MAROON : RED;
        }
        DrawRectangleRec(buttons[i], buttonColors[i]);
        DrawText(buttonTexts[i], buttons[i].x + (200 - MeasureText(buttonTexts[i], 20)) / 2, buttons[i].y + (50 - 20) / 2, 20, BLACK);
    }
}

void DrawPlayOptions()
{
    Vector2 mousePoint = GetMousePosition();

    DrawText("PLAY OPTIONS", screenWidth / 2 - MeasureText("PLAY OPTIONS", 40) / 2, 100, 40, DARKGRAY);
    for (int i = 0; i < 4; i++)
    {
        buttonColors[i] = LIGHTGRAY;
        if (CheckCollisionPointRec(mousePoint, buttons[i]))
        {
            buttonColors[i] = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? MAROON : RED;
        }
        DrawRectangleRec(buttons[i], buttonColors[i]);
        DrawText(button2Text[i], buttons[i].x + (200 - MeasureText(button2Text[i], 20)) / 2, buttons[i].y + (50 - 20) / 2, 20, DARKGRAY);
    }
}

void DrawOptionsMenu()
{
    DrawText("OPTIONS MENU", screenWidth / 2 - MeasureText("OPTIONS MENU", 40) / 2, 25, 40, DARKGRAY);
    DrawText(TextFormat("VOLUME: %d", volume), screenWidth / 2 - MeasureText(TextFormat("VOLUME: %d", volume), 20) / 2, 150, 20, GRAY);

    // Draw + and - buttons
    for (int i = 0; i < 2; i++)
    {
        DrawRectangleRec(optionButtons[i], optionButtonColors[i]);
    }
    DrawText("+", optionButtons[0].x + (50 - MeasureText("+", 30)) / 2, optionButtons[0].y + (50 - 30) / 2, 30, BLACK);
    DrawText("-", optionButtons[1].x + (50 - MeasureText("-", 30)) / 2, optionButtons[1].y + (50 - 30) / 2, 30, BLACK);

    // Draw back button
    DrawRectangleRec(backButton, WHITE);
    DrawText("BACK", backButton.x + (200 - MeasureText("BACK", 20)) / 2, backButton.y + (50 - 20) / 2, 20, BLACK);
}

void DrawHelpMenu()
{
    DrawText("HELP MENU", screenWidth / 2 - MeasureText("HELP MENU", 40) / 2, 25, 40, DARKGRAY);
    for (int i = 0; i < 4; i++)
    {
        DrawRectangleRec(helpButtons[i], helpButtonColors[i]);
        DrawText(helpButtonTexts[i], helpButtons[i].x + (200 - MeasureText(helpButtonTexts[i], 20)) / 2, helpButtons[i].y + (50 - 20) / 2, 20, BLACK);
    }
}

void DrawPINGPONGHELP()
{
    DrawText("PING PONG INSTRUCTIONS", screenWidth / 2 - MeasureText("PING PONG INSTRUCTIONS", 30) / 2, 25, 30, DARKGRAY);

    // Computer vs Player Instructions
    DrawText("FOR COMPUTER VS PLAYER", screenWidth / 2 - MeasureText("FOR COMPUTER VS PLAYER", 20) / 2, 80, 25, RED);

    DrawText("1. Computer controls the left paddle, you control the right.", 50, 110, 20, BLACK);
    DrawText("2. Use arrow keys to move the paddle up and down.", 50, 140, 20, BLACK);
    DrawText("3. Hit the ball back and forth with the paddle.", 50, 170, 20, BLACK);
    DrawText("4. Don't let the ball pass your paddle.", 50, 200, 20, BLACK);
    DrawText("5. First player to 20 points wins.", 50, 230, 20, BLACK);

    // Multiplayer Instructions
    DrawText("FOR MULTIPLAYER", screenWidth / 2 - MeasureText("FOR MULTIPLAYER", 20) / 2, 280, 25, RED);
    DrawText("1. One player controls the left paddle, another the right.", 50, 310, 20, BLACK);
    DrawText("2. Use arrow keys to move the right paddle up and down.", 50, 340, 20, BLACK);
    DrawText("3. Use 'W' and 'S' keys to move the left paddle.", 50, 370, 20, BLACK);
    DrawText("4. Hit the ball back and forth with the paddles.", 50, 400, 20, BLACK);
    DrawText("5. Don't let the ball pass your paddle.", 50, 430, 20, BLACK);
    DrawText("6. First player to 20 points wins.", 50, 460, 20, BLACK);

    DrawText("CLICK ANYWHERE TO GO BACK", screenWidth / 2 - MeasureText("CLICK ANYWHERE TO GO BACK", 20) / 2, 510, 20, RED);
}

void Drawtertishelp()
{
    DrawText("TETRIS INSTRUCTIONS", screenWidth / 2 - MeasureText("TETRIS INSTRUCTIONS", 30) / 2, 25, 30, DARKGRAY);

    DrawText("1. Use arrow keys to move the blocks.", 50, 110, 20, BLACK);
    DrawText("2. Use 'Up arrow key' to rotate the blocks.", 50, 140, 20, BLACK);
    DrawText("3. Use 'Space' to directly place the block.", 50, 170, 20, BLACK);
    DrawText("4. Complete a line to clear it.", 50, 200, 20, BLACK);
    DrawText("5. Don't let the blocks reach the top.", 50, 230, 20, BLACK);

    DrawText("CLICK ANYWHERE TO GO BACK", screenWidth / 2 - MeasureText("CLICK ANYWHERE TO GO BACK", 20) / 2, 510, 20, RED);
}

void help()
{
    DrawText("2048 INSTRUCTIONS", screenWidth / 2 - MeasureText("2048 INSTRUCTIONS", 30) / 2, 25, 30, DARKGRAY);

    DrawText("1. Use arrow keys to move the tiles.", 50, 110, 20, BLACK);
    DrawText("2. Combine tiles with the same number.", 50, 140, 20, BLACK);
    DrawText("3. Reach the 2048 tile to win.", 50, 170, 20, BLACK);

    DrawText("CLICK ANYWHERE TO GO BACK", screenWidth / 2 - MeasureText("CLICK ANYWHERE TO GO BACK", 20) / 2, 510, 20, RED);
}
