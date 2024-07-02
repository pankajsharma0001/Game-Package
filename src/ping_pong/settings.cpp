#include "pingpong/settings.h"
#include <cstring>
#include <iostream>
#pragma once

void Setting::DrawRadioButton(const RadioButton& button) {
    // Draw the button circle
    DrawCircleV({button.bounds.x + button.bounds.width / 2, button.bounds.y + button.bounds.height / 2}, button.bounds.width / 2, DARKGRAY);
    // Draw the inner circle if selected
    if (button.selected) {
        DrawCircleV({button.bounds.x + button.bounds.width / 2, button.bounds.y + button.bounds.height / 2}, button.bounds.width / 4, DARKBLUE);
    }
    // Draw the label
    DrawText(button.label, button.bounds.x + button.bounds.width + 10, button.bounds.y, button.bounds.height, DARKGRAY);
}

void Setting::UpdateRadioButtons(std::vector<RadioButton>& radioButtons, Vector2 mousePos) {
    for (auto& button : radioButtons) {
        if (CheckCollisionPointRec(mousePos, button.bounds)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (auto& btn : radioButtons) {
                    btn.selected = false;
                }
                button.selected = true;
            }
        }
    }
}

void Setting::settingMenu(GameMode& screen)
{
    // Initialize radio buttons
    std::vector<RadioButton> radioButtons = {
        {{200, 150, 20, 20}, "Horizontal Paddle", false},
        {{200, 200, 20, 20}, "Vertical Paddle", false}
    };
    // Select the first button initially
    radioButtons[0].selected = true;

    bool exitGame = false;
    while (!WindowShouldClose() && !exitGame) {
        Vector2 mousePos = GetMousePosition();

        // Update radio buttons based on input
        UpdateRadioButtons(radioButtons, mousePos);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mousePos, {(float)displayWidth / 2 - 100, 500, 200, 50})) {
                exitGame = true;
                screen = WINDOW;
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("SETTINGS", displayWidth / 2 - MeasureText("SETTINGS", 40) / 2, 100, 40, DARKGRAY);

        // Draw each radio button
        for (const auto& button : radioButtons) {
            DrawRadioButton(button);
        }

        DrawText("Points to Win:", displayWidth / 2 - 100, 400, 20, DARKGRAY);

        // Draw input box
        DrawRectangle(displayWidth / 2, 400, 100, 30, inputActive ? LIGHTGRAY : GRAY);
        DrawText(pointsToWin, displayWidth / 2 + 10, 405, 20, BLACK);

         // Instruction text
        if (inputActive) {
            DrawText("Press Enter to confirm", displayWidth / 2 - MeasureText("Press Enter to confirm", 20) / 2, 450, 20, DARKGRAY);
        } else {
            DrawText("Click to edit", displayWidth / 2 - MeasureText("Click to edit", 20) / 2, 450, 20, DARKGRAY);
        }

        // Debug: Check if mouse click is within bounds
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePoint = GetMousePosition();
        if (CheckCollisionPointRec(mousePoint, { (float)displayWidth / 2, 400, 100, 30 })) {
            inputActive = true;
            std::cout << "Input field activated!" << std::endl;  // Debug print
            //HandleInput(pointsToWin, inputIndex, inputActive);
        } else {
            std::cout << "Mouse click detected, but outside input field!" << std::endl;  // Debug print
        }
        }

        if (inputActive) {
            HandleInput(pointsToWin, inputIndex, inputActive);
        }

        DrawRectangleRec({(float)displayWidth / 2 - 100, 500, 200, 50}, WHITE);
        DrawText("Back", displayWidth/2-100 + (200 - MeasureText("Back", 20)) / 2, 500 + (50 - 20) / 2, 20, DARKGRAY);

        // Check for 'Back' button press or other exit condition
        if (IsKeyPressed(KEY_ESCAPE)) {
            exitGame = true;
            screen = WINDOW;
        }

        EndDrawing();
    }
}

void Setting::HandleInput(char* inputBuffer, int& index, bool& isActive) {
    if (isActive) {
        int key = GetKeyPressed();
        while (key > 0) {
            if (key >= 32 && key <= 126 && index < 3) {  // Printable characters
                inputBuffer[index] = (char)key;
                index++;
                inputBuffer[index] = '\0';  // Null-terminate the string
            }
            if (key == KEY_BACKSPACE && index > 0) {
                index--;
                inputBuffer[index] = '\0';
            }
            key = GetKeyPressed();
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
            isActive = false;  // Confirm input
            std::cout << "Input confirmed: " << inputBuffer << std::endl;  // Debug print
        }
    }
}

PlayOption currentPlayOption = PLAY_GAME;
MenuOption currentOption = RESUME;

void PauseMenu::drawPauseMenu(){
    const int menuX = 500;
    const int menuY = 300;
    const int menuSpacing = 50;

    DrawText("GAME PAUSED", menuX, menuY - 100, 40, DARKBROWN);
    DrawText("Resume", menuX, menuY, 30, currentOption == RESUME ? YELLOW : DARKBROWN);
    DrawText("Restart", menuX, menuY + menuSpacing, 30, currentOption == RESTART ? YELLOW : DARKBROWN);
    DrawText("Back", menuX, menuY + 2 * menuSpacing, 30, currentOption == BACK ? YELLOW : DARKBROWN);
}

void PauseMenu::pauseMenu(Ball& b){
        const int menuX = 500;
        const int menuY = 300;
        const int menuSpacing = 50;

        float text1 = MeasureText("Resume", 30);
        float text2 = MeasureText("Restart", 30);
        float text3 = MeasureText("Back", 30);

        Rectangle resumeButton = {menuX, menuY, text1, 30};
        Rectangle restartButton = {menuX, menuY + menuSpacing, text2, 30};
        Rectangle backButton = {menuX, menuY + 2 * menuSpacing, text3, 30};

        Vector2 mousePoint = GetMousePosition();

        if (CheckCollisionPointRec(mousePoint, resumeButton))
        {
            currentOption = RESUME;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentPlayOption = PLAY_GAME;
            }
        }
        if (CheckCollisionPointRec(mousePoint, restartButton))
        {
            currentOption = RESTART;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                b.player_score = 0;
                b.cpu_score = 0;
                b.Reset();
                currentPlayOption = PLAY_GAME;
            }
        }
        if (CheckCollisionPointRec(mousePoint, backButton))
        {
            currentOption = BACK;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentPlayOption = PLAY_GAME;
            }
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            if (currentOption == RESUME)
            {
                currentOption = RESTART;
            }
            else if (currentOption == RESTART)
            {
                currentOption = BACK;
            }
            else if (currentOption == BACK)
            {
                currentOption = RESUME;
            }
        }

        if (IsKeyPressed(KEY_UP))
        {
            if (currentOption == RESUME)
            {
                currentOption = BACK;
            }
            else if (currentOption == RESTART)
            {
                currentOption = RESUME;
            }
            else if (currentOption == BACK)
            {
                currentOption = RESTART;
            }
        }

        if (IsKeyPressed(KEY_ENTER))
        {
            if (currentOption == RESUME)
            {
                currentPlayOption = PLAY_GAME;
            }
            else if (currentOption == RESTART)
            {
                b.player_score = 0;
                b.cpu_score = 0;
                b.Reset();
                currentPlayOption = PLAY_GAME;
            }
            else if (currentOption == BACK)
            {
                currentPlayOption = PLAY_GAME;
            }
        }
}
