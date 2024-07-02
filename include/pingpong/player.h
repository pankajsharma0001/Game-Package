#include <raylib.h>
#include "ping_pong.h"

enum GameMode;

class Player{
private:
    int displayWidth = 1280;
    int displayHeight = 600;
public:
    Ball ball;
    Paddle1 paddle1;
    Paddle2 paddle2;
    Player();
    void Multiplayer(GameMode& screen);
};