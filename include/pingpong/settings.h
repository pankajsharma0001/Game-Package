#pragma once
#include "raylib.h"
#include "ping_pong.h"
#include <vector>

struct RadioButton {
    Rectangle bounds; // Button bounds
    const char* label; // Button label
    bool selected; // Is the button selected?
};

enum GameMode;

class Setting{
private:
    int displayWidth = 1280;
    int displayHeight = 600;
public:
    char pointsToWin[4] = "10";  // Default value, maximum 3 digits + null terminator
    int inputIndex = 2;  // Length of initial value
    bool inputActive = false;

    void HandleInput(char* inputBuffer, int& index, bool& isActive);
    void DrawRadioButton(const RadioButton& button);
    void UpdateRadioButtons(std::vector<RadioButton>& buttons, Vector2 mousePos);
    void settingMenu(GameMode&);
};

enum PlayOption {PLAY_GAME, PAUSE_GAME};
enum MenuOption {RESUME, RESTART, BACK};

class PauseMenu{
public:
    void drawPauseMenu();
    void pauseMenu(Ball&);
};