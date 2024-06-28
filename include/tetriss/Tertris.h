#pragma once
#include <raylib.h>
#include "game.h"
#include "colors.h"
#include<iostream>
#include "screen.h"

class Tertris
{
private:
    int windowWidth;
    int windowHeight;
    Font font;
    void SaveHighScore(int score);
    int LoadHighScore();
    
public:
    Tertris();
    bool EventTriggered(double interval);
    void Play(GameScreen&);
};