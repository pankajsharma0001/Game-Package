#pragma once
#include <raylib.h>
#include "game.h"
#include "colors.h"
#include <iostream>
#include "screen.h"
#include "../src/ping_pong/settings.cpp"

class Pause : public PauseMenu{

};

class Tertris
{
private:
    int windowWidth;
    int windowHeight;
    Font font;
    void SaveHighScore(int score);
    int LoadHighScore();
    int highScore;
    
public:
    Tertris();
    bool EventTriggered(double interval);
    void Play();
    ~Tertris()
    {
        UnloadFont(font);
    }
};