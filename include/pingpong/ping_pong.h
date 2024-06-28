#pragma once
#include <unistd.h>
#include <raylib.h>
#include "screen.h"

const Color Green = {38, 185, 154, 255};
const Color Dark_Green = {20, 160, 133, 255};
const Color Light_Green = {129, 204, 184, 255};
const Color Yellow = {243, 213, 91, 255};

int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
    float x, y;
    int speed_x, speed_y;
    int radius;

    void Draw();
    void Reset();
    void Update();
};

class Paddle
{
protected:
    void LimitMovement();

public:
    float x, y;
    float width, height;
    int speed;

    void Draw();
};

class Paddle1 : public Paddle
{
public:
    void Update();
};

class Paddle2 : public Paddle
{
public:
    void Update();
};

class cpuPaddle : public Paddle
{
public:
    void Update(int ball_height);
};

class Pong
{
public:
    Pong();
    void Play(GameScreen&);

private:
    int displayWidth;
    int displayHeight;
    Image icon;
};