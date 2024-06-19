#include "raylib.h"
#include "./ping_pong/ping_pong.cpp"

enum GameScreen { MENU = 0, PLAY, OPTIONS, HELP, PING_PONG, TERTRIS, T048};
const int screenWidth = 800;
const int screenHeight = 600;
const char *buttonTexts[4] = {"PLAY", "OPTIONS", "HELP", "QUIT"};
const char *button2Text[4] = {"PING PONG", "TETRIS", "2048", "BACK"};
Color buttonColors[4] = {WHITE, WHITE, WHITE, WHITE};

Rectangle buttons[4] = {
        {screenWidth / 2 - 100, 200, 200, 50},
        {screenWidth / 2 - 100, 300, 200, 50},
        {screenWidth / 2 - 100, 400, 200, 50},
        {screenWidth / 2 - 100, 500, 200, 50}
        };

void DrawMenu();
void DrawPlayOptions();

int main() {
    Pong pong;
    // Initialization
    InitWindow(screenWidth, screenHeight, "Game Menu Example");

    GameScreen currentScreen = MENU;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        Vector2 mousePoint = GetMousePosition();    
        if (currentScreen == MENU) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, buttons[0])) {
                    currentScreen = PLAY;
                } else if (CheckCollisionPointRec(mousePoint, buttons[1])) {
                    currentScreen = OPTIONS;
                } else if (CheckCollisionPointRec(mousePoint, buttons[2])) {
                    currentScreen = HELP;
                } else if (CheckCollisionPointRec(mousePoint, buttons[3])) {
                    CloseWindow();
                    return 0;
                }
            }
        } else if (currentScreen == PLAY) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, buttons[0])) {
                    currentScreen = PING_PONG;
                } else if (CheckCollisionPointRec(mousePoint, buttons[1])) {
                    currentScreen = TERTRIS;
                } else if (CheckCollisionPointRec(mousePoint, buttons[2])) {
                    currentScreen = T048;
                } else if (CheckCollisionPointRec(mousePoint, buttons[3])) {
                    currentScreen = MENU;
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (currentScreen == MENU) {
            DrawMenu();
        } else if (currentScreen == PLAY) {
            DrawPlayOptions();
        } else if (currentScreen == OPTIONS) {
            DrawText("OPTIONS SCREEN", screenWidth / 2 - MeasureText("OPTIONS SCREEN", 20) / 2, screenHeight / 2 - 10, 20, GRAY);
        } else if (currentScreen == HELP) {
            DrawText("HELP SCREEN", screenWidth / 2 - MeasureText("HELP SCREEN", 20) / 2, screenHeight / 2 - 10, 20, GRAY);
        } else if (currentScreen == PING_PONG) {
            pong.Play();
        } else if (currentScreen == TERTRIS) {
            DrawText("GAME MODE 2", screenWidth / 2 - MeasureText("GAME MODE 2", 20) / 2, screenHeight / 2 - 10, 20, GRAY);
        }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

void DrawMenu() {
    Vector2 mousePoint = GetMousePosition();

    DrawText("MAIN MENU", screenWidth / 2 - MeasureText("MAIN MENU", 40) / 2, 100, 40, DARKGRAY);
    for (int i = 0; i < 4; i++)
    {
        buttonColors[i] = LIGHTGRAY;
        if (CheckCollisionPointRec(mousePoint, buttons[i])) {
            buttonColors[i] = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? MAROON : RED;
        }
        DrawRectangleRec(buttons[i], buttonColors[i]);
        DrawText(buttonTexts[i], buttons[i].x + (200 - MeasureText(buttonTexts[i], 20)) / 2, buttons[i].y + (50 - 20) / 2, 20, BLACK);
    }
}

void DrawPlayOptions() {
    Vector2 mousePoint = GetMousePosition();

    DrawText("PLAY OPTIONS", screenWidth / 2 - MeasureText("PLAY OPTIONS", 40) / 2, 100, 40, DARKGRAY);
    for (int i = 0; i < 4; i++)
    {
        buttonColors[i] = LIGHTGRAY;
        if (CheckCollisionPointRec(mousePoint, buttons[i])) {
            buttonColors[i] = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? MAROON : RED;
        }
        DrawRectangleRec(buttons[i], buttonColors[i]);
        DrawText(button2Text[i], buttons[i].x + (200 - MeasureText(button2Text[i], 20)) / 2, buttons[i].y + (50 - 20) / 2, 20, DARKGRAY);
    }
}
