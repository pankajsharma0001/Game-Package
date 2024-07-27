#pragma once
#include <raylib.h>
#include "ping_pong.h"

enum GameMode;

class Computer{
private:
    int displayWidth = 1280;
    int displayHeight = 600;

public:
    Ball ball;
    Paddle1 paddle1;
    cpuPaddle paddle2;
    Computer();
    void Bot(GameMode& screen);
};