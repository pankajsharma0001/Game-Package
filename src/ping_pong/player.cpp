#include "pingpong/player.h"
#include "settings.cpp"

Player::Player()
{
    ball.x = displayWidth / 2;
    ball.y = displayHeight / 2;
    ball.radius = 15;
    ball.speed_x = 7;
    ball.speed_y = 7;

    paddle1.width = 25;
    paddle1.height = 120;
    paddle1.x = 10;
    paddle1.y = displayHeight / 2 - paddle1.height / 2;
    paddle1.speed = 6;

    paddle2.width = 25;
    paddle2.height = 120;
    paddle2.x = displayWidth - paddle2.width - 10;
    paddle2.y = displayHeight / 2 - paddle1.height / 2;
    paddle2.speed = 6;

}

void Player::Multiplayer(GameMode& screen)
{
    currentPlayOption = PLAY_GAME;
    PauseMenu pause;

    bool exitGame = false;
    while (!WindowShouldClose() && !exitGame){
        if (IsKeyPressed(KEY_SPACE))
        {
            currentPlayOption = (currentPlayOption == PLAY_GAME) ? PAUSE_GAME : PLAY_GAME;
        }

        if(currentPlayOption == PLAY_GAME){
            ball.Update();
            paddle1.Update();
            paddle2.Update();

            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle1.x, paddle1.y, paddle1.width, paddle1.height}))
            {
                ball.speed_x *= -1;
            }
            if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{paddle2.x, paddle2.y, paddle2.width, paddle2.height}))
            {
                ball.speed_x *= -1;
            }
        }
        else{
            pause.pauseMenu(ball);
        }

        BeginDrawing();
        ClearBackground(Dark_Green);

        DrawRectangle(displayWidth / 2, 0, displayWidth / 2, displayHeight, Green);
        DrawCircle(displayWidth / 2, displayHeight / 2, 150, Light_Green);
        DrawLine(displayWidth / 2, 0, displayWidth / 2, displayHeight, WHITE);
        
        ball.Draw();
        paddle1.Draw();
        paddle2.Draw();

        DrawText(TextFormat("%i", ball.player_score), displayWidth / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", ball.cpu_score), 3 * displayWidth / 4 - 20, 20, 80, WHITE);
        // Check for 'Back' button press or other exit condition
        if (IsKeyPressed(KEY_BACKSPACE)) {
            exitGame = true;
            screen = WINDOW;
        }

        if(currentPlayOption == PAUSE_GAME){
            pause.drawPauseMenu();
        }

        if(ball.player_score >= pointToWin || ball.cpu_score >= pointToWin){
            ball.Reset();
            ball.cpu_score = 0;
            ball.player_score = 0;
            ball.GameOver(screen);
            exitGame = true;
            screen = WINDOW;
        }
        EndDrawing();
    }
}
