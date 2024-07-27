#pragma once
#include <unistd.h>
#include <raylib.h>
#include "screen.h"

enum GameMode { WINDOW = 0, COMPUTER, MULTIPLAYER, SETTING};

const Color Green = {38, 185, 154, 255};
const Color Dark_Green = {20, 160, 133, 255};
const Color Light_Green = {129, 204, 184, 255};
const Color Yellow = {243, 213, 91, 255};

int pointToWin = 5;

class Ball
{
public:
    float x, y;
    float speed_x, speed_y;
    int radius;
    int player_score;
    int cpu_score;
    double lastScoreTime;  // Time since the last score change
    double speedIncrementInterval;  // Interval to increase speed if no score
    float speedIncrementFactor;  // Factor by which to increase speed
    
    Ball() : x(0), y(0), speed_x(7), speed_y(7), radius(15), player_score(0), cpu_score(0),
             lastScoreTime(GetTime()), speedIncrementInterval(5.0), speedIncrementFactor(1.1f) {}

    void Draw();
    void Reset();
    void Update();
    void GameOver(GameMode&);
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
    void Menu();
    int displayWidth;
    int displayHeight;

private:
    Image icon;
};

class PlaySoundClass{
public:
    Sound hittingBall;
    PlaySoundClass(){
        InitAudioDevice();
        hittingBall = LoadSound("./assets/sounds/ping_pong_hitting_pad.wav");
    }
    ~PlaySoundClass(){
        UnloadSound(hittingBall);
        CloseAudioDevice();
    }
};
//PlaySoundClass play;