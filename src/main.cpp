#include "raylib.h"
#include "ping_pong/ping_pong.cpp"
#include "tetris/tetris.cpp"
#include "tetris/game.cpp"
#include "tetris/block.cpp"
#include "tetris/colors.cpp"
#include "tetris/grid.cpp"
#include "tetris/position.cpp"
#include "screen.h"
#include "2048/2048.cpp"

#define NUM_FRAMES  3       // Number of frames (rectangles) for the button sprite texture

const int screenWidth = 1000;
const int screenHeight = 800;

const float button_width = 224.0f;
const float button_height = 318.0f;

float frameHeight = (float)button_height/NUM_FRAMES;

Rectangle sourceRec = { 0, 0, button_width, frameHeight };
Rectangle sourceMusicRec = { 0, 0, 112, frameHeight };

Rectangle btnBounds[4] = { 
    {screenWidth/2.0f - button_width/2.0f, 200, button_width, frameHeight},
    {screenWidth/2.0f - button_width/2.0f, 350, button_width, frameHeight},
    {screenWidth/2.0f - button_width/2.0f, 500, button_width, frameHeight},
    {screenWidth/2.0f - button_width/2.0f, 650, button_width, frameHeight}};

Rectangle optionButtons[2] = {
    {screenWidth / 2 + 120, 350, 112, frameHeight},  // Plus button
    {screenWidth / 2 + 260, 350, 112, frameHeight}}; // Minus button

int volume;

class Main{
public:
    Image menu_icon;
    Music background;
    Main()
    {
        InitAudioDevice();
        background = LoadMusicStream("./assets/sounds/background.mp3");
        menu_icon = LoadImage("./assets/images/menu.png");

        PlayMusicStream(background);
        background.looping = true;
        volume = 30;
        SetMusicVolume(background, volume/100.0f);
    }
    ~Main(){
        StopMusicStream(background);
        UnloadMusicStream(background);
        CloseAudioDevice();
    }
};

void DrawMenu(Texture2D&, Texture2D&, Texture2D&, Texture2D&);
void DrawPlayOptions(Texture2D&, Texture2D&, Texture2D&, Texture2D&);
void DrawOptionsMenu(Texture2D&, Texture2D&, Texture2D&);
void DrawHelpMenu(Texture2D&, Texture2D&, Texture2D&, Texture2D&);
void DrawPINGPONGHELP();
void Drawtertishelp();
void help();

GameScreen currentScreen = MENU;

int main(){
    Main gam;
    Pong pong;
    Tertris tertris;
    Game2048 game_2048;
    // Initialization
    InitWindow(screenWidth, screenHeight, "ClassicFun: Timeless Games Collection");

    SetWindowIcon(gam.menu_icon);
    UnloadImage(gam.menu_icon);

    Texture2D backgroundImage = LoadTexture("./assets/images/background_image.png");

    Texture2D play_button = LoadTexture("./assets/images/play_button.png");
    Texture2D music_button = LoadTexture("./assets/images/music_button.png");
    Texture2D help_button = LoadTexture("./assets/images/help_button.png");
    Texture2D quit_button = LoadTexture("./assets/images/quit_button.png");

    Texture2D ping_pong_button = LoadTexture("./assets/images/ping_pong_button.png");
    Texture2D tetris_button = LoadTexture("./assets/images/tetris_button.png");
    Texture2D T048_button = LoadTexture("./assets/images/2048_button.png");

    Texture2D plus_button = LoadTexture("./assets/images/plus_button.png");
    Texture2D minus_button = LoadTexture("./assets/images/minus_button.png");

    Texture2D back_button = LoadTexture("./assets/images/back_button.png");

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
                if (CheckCollisionPointRec(mousePoint, btnBounds[0]))
                {
                    currentScreen = PLAY;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[1]))
                {
                    currentScreen = OPTIONS;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[2]))
                {
                    currentScreen = HELP_MENU;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[3]))
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
                if (CheckCollisionPointRec(mousePoint, btnBounds[0]))
                {
                    currentScreen = PING_PONG;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[1]))
                {
                    currentScreen = TERTRIS;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[2]))
                {
                    currentScreen = T048;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[3]))
                {
                    currentScreen = MENU;
                }
            }
        }

        else if (currentScreen == OPTIONS)
        {
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mousePoint, optionButtons[0])) // Plus button
                {
                        if (volume < 100)
                            volume += 10;
                        SetMusicVolume(gam.background, volume/100.0f);
                }

                else if (CheckCollisionPointRec(mousePoint, optionButtons[1])) // Minus button
                {
                        if (volume > 0)
                            volume -= 10;
                        SetMusicVolume(gam.background, volume/100.0f);
                }

                else if (CheckCollisionPointRec(mousePoint, btnBounds[1]))
                {
                        currentScreen = MENU; // Back to main menu
                }
            }
        }
        else if (currentScreen == HELP_MENU)
        {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                if (CheckCollisionPointRec(mousePoint, btnBounds[0]))
                {
                    currentScreen = PINGPONG_INSTRUCTIONS;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[1]))
                {
                    currentScreen = TERTIS_INSTRUCTIONS;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[2]))
                {
                    currentScreen = INSTUCTIONS;
                }
                else if (CheckCollisionPointRec(mousePoint, btnBounds[3]))
                {
                    currentScreen = MENU;
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
        DrawTexture(backgroundImage, 0, 0, RAYWHITE);

        if (currentScreen == MENU)
        {
            DrawMenu(play_button, music_button, help_button, quit_button);
        }
        else if (currentScreen == PLAY)
        {
            DrawPlayOptions(ping_pong_button, tetris_button, T048_button, back_button);
        }
        else if (currentScreen == OPTIONS)
        {
            DrawOptionsMenu(back_button, plus_button, minus_button);
        }
        else if (currentScreen == HELP_MENU)
        {
            DrawHelpMenu(ping_pong_button, tetris_button, T048_button, back_button);
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
            pong.Play(currentScreen);
        }
        else if (currentScreen == TERTRIS)
        {
            gam.~Main();
            CloseWindow();
            tertris.Play();
        }
        else if (currentScreen == T048){
            gam.~Main();
            CloseWindow();
            game_2048.Start();
        }
        EndDrawing();
    }

    UnloadTexture(backgroundImage);
    UnloadTexture(quit_button);
    UnloadTexture(help_button);
    UnloadTexture(music_button);
    UnloadTexture(play_button);
    UnloadTexture(ping_pong_button);
    UnloadTexture(tetris_button);
    UnloadTexture(T048_button);
    UnloadTexture(plus_button);
    UnloadTexture(minus_button);
    UnloadTexture(back_button);
    CloseWindow();
    return 0;
}

void DrawMenu(Texture2D& play_button, Texture2D& music_button, Texture2D& help_button, Texture2D& quit_button)
{
    Texture2D buttons[4] = {play_button, music_button, help_button, quit_button};
    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

    Vector2 mousePoint = GetMousePosition();

    DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 40) / 2, 100, 40, DARKGRAY);
    
    for(int i=0; i<4; i++){
    // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds[i]))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;
        }

        else btnState = 0;
    // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState*frameHeight;
        DrawTextureRec(buttons[i], sourceRec, (Vector2){ btnBounds[i].x, btnBounds[i].y }, WHITE); // Draw button frame
    }
}

void DrawPlayOptions(Texture2D& ping_pong_button, Texture2D& tetris_button, Texture2D& T048_button, Texture2D& back_button)
{
    Texture2D buttons[4] = {ping_pong_button, tetris_button, T048_button, back_button};
    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

    Vector2 mousePoint = GetMousePosition();

    DrawText("PLAY OPTIONS", screenWidth / 2 - MeasureText("PLAY OPTIONS", 40) / 2, 100, 40, DARKGRAY);

    for (int i = 0; i < 4; i++)
    {
        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds[i]))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;
        }

        else btnState = 0;
    // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState*frameHeight;
        DrawTextureRec(buttons[i], sourceRec, (Vector2){ btnBounds[i].x, btnBounds[i].y }, WHITE); // Draw button frame
    }
}

void DrawOptionsMenu(Texture2D& back_button, Texture2D& plus_button, Texture2D& minus_button)
{
    DrawText("OPTIONS MENU", screenWidth / 2 - MeasureText("OPTIONS MENU", 40) / 2, 25, 40, DARKGRAY);
    DrawText(TextFormat("VOLUME: %d", volume), screenWidth / 2 - MeasureText(TextFormat("VOLUME: %d", volume), 20) / 2, 300, 40, GRAY);

    Texture2D buttons[3] = {plus_button, minus_button, back_button};

    int btnState = 0; 
    Vector2 mousePoint = GetMousePosition();

    // Draw + and - buttons
    for (int i = 0; i < 2; i++)
    {
       // Check button state
        if (CheckCollisionPointRec(mousePoint, optionButtons[i]))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;
        }

        else btnState = 0;
    // Calculate button frame rectangle to draw depending on button state
        sourceMusicRec.y = btnState*frameHeight;
        DrawTextureRec(buttons[i], sourceMusicRec, (Vector2){ optionButtons[i].x, optionButtons[i].y }, WHITE); // Draw button frame
    }

    // Check button state
    if (CheckCollisionPointRec(mousePoint, btnBounds[1]))
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
        else btnState = 1;
    }

    else btnState = 0;

    // Calculate button frame rectangle to draw depending on button state
    sourceRec.y = btnState*frameHeight;
    DrawTextureRec(buttons[2], sourceRec, (Vector2){ btnBounds[1].x, btnBounds[1].y }, WHITE); // Draw button frame
}

void DrawHelpMenu(Texture2D& ping_pong_button, Texture2D& tetris_button, Texture2D& T048_button, Texture2D& back_button)
{
    Texture2D buttons[4] = {ping_pong_button, tetris_button, T048_button, back_button};
    int btnState = 0;               // Button state: 0-NORMAL, 1-MOUSE_HOVER, 2-PRESSED

    Vector2 mousePoint = GetMousePosition();

    DrawText("HELP MENU", screenWidth / 2 - MeasureText("HELP MENU", 40) / 2, 100, 40, DARKGRAY);

    for (int i = 0; i < 4; i++)
    {
        // Check button state
        if (CheckCollisionPointRec(mousePoint, btnBounds[i]))
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
            else btnState = 1;
        }

        else btnState = 0;
    // Calculate button frame rectangle to draw depending on button state
        sourceRec.y = btnState*frameHeight;
        DrawTextureRec(buttons[i], sourceRec, (Vector2){ btnBounds[i].x, btnBounds[i].y }, WHITE); // Draw button frame
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
