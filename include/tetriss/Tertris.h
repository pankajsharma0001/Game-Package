#pragma once
#include<raylib.h>
#include "game.h"
#include "colors.h"
#include<iostream>

class Tertris{
private:
    int windowWidth;
    int windowHeight;
    Font font;
    
public:
    Tertris();
    bool EventTriggered(double interval);
    void Play();
};