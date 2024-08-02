#pragma once
#include "raylib.h"
#include "ping_pong.h"
#include <vector>

enum GameMode;

class Setting{
private:
    int displayWidth = 1280;
    int displayHeight = 600;
public:
    char pointsToWin[4] = "5";  // Default value, maximum 3 digits + null terminator
    int inputIndex = 2;  // Length of initial value
    bool inputActive = false;

    void HandleInput(char* inputBuffer, int& index, bool& isActive);
    void settingMenu(GameMode&);
};

enum PlayOption {PLAY_GAME, PAUSE_GAME};
enum MenuOption {RESUME, RESTART, BACK};

class PauseMenu{
public:
    void drawPauseMenu();
    void pauseMenu(Ball&, GameMode&, bool&);
};
