#include "raylib.h"
#include <string.h>

int main(void)
{

    // INIT
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Pong Kol");
    InitAudioDevice();

    Sound start = LoadSound("resources/start.wav");
    Sound win = LoadSound("resources/win.wav");
    Sound pongHit = LoadSound("resources/pong_hit.wav");
    Sound hit = LoadSound("resources/wall_hit.wav");
    Sound score = LoadSound("resources/plink.wav");
    // Player x y width height
    const int playerWidth = 20;
    const int playerHeight = 100;
    Rectangle Player = {20, 10, playerWidth, playerHeight};
    const int playerSpeed = 5;

    // Opponent
    Rectangle Opponent = {screenWidth - 40, 10, playerWidth, playerHeight};
    const int opponentSpeed = 4;

    // Ball
    const int ballSize = 16;
    Rectangle Ball = {screenWidth / 2, screenHeight / 2, ballSize, ballSize};
    int ballSpeed = 5;
    int ballDx = 1;
    int ballDy = 1;

    // Net
    Rectangle Net = {screenWidth / 2 - 2, 16, 4, screenHeight - 32};
    Rectangle debugNet = {screenWidth - 40, 16, 4, screenHeight - 32};

    // Scores
    int playerScore = 0;
    int opponentScore = 0;

    bool weHaveAWinner = false;
    int winner;

    SetTargetFPS(60);

    PlaySound(start);

    while (!WindowShouldClose())
    {
        // UPDATE

        // player movement
        if (IsKeyDown(KEY_UP) && Player.y >= 0)
            Player.y -= playerSpeed;
        if (IsKeyDown(KEY_DOWN) && Player.y + playerHeight < screenHeight)
            Player.y += playerSpeed;

        // ball movement
        Ball.x += ballSpeed * ballDx;
        Ball.y += ballSpeed * ballDy;

        // Ball bounces when hits the walls
        if (Ball.y + ballSize >= screenHeight || Ball.y <= 0)
        {

            ballDy *= -1;
            PlaySound(hit);
        }

        // Score when hits Boundaries
        // Reset ball position
        if (Ball.x + ballSize >= screenWidth)
        {
            playerScore += 1;
            PlaySound(score);
            Ball.x = screenWidth / 2;
            Ball.y = screenHeight / 2;
        }

        if (Ball.x <= 0)
        {
            opponentScore += 1;
            PlaySound(score);
            Ball.x = screenWidth / 2;
            Ball.y = screenHeight / 2;
        }

        // Opponent AI
        if (Ball.y > Opponent.y && Opponent.y + playerHeight < screenHeight)
            Opponent.y += opponentSpeed;
        if (Ball.y < Opponent.y && Opponent.y >= 0)
            Opponent.y -= opponentSpeed;

        // check Collisions
        // Player collision
        if (Ball.x <= 40 && Ball.y >= Player.y && Ball.y + ballSize <= Player.y + playerHeight)
        {
            Ball.x = 40;
            ballDx *= -1;
            PlaySound(hit);
        }

        // Opponent collision
        if (Ball.x >= screenWidth - 40 && Ball.y >= Opponent.y && Ball.y + ballSize <= Opponent.y + playerHeight)
        {
            Ball.x = screenWidth - 40;
            ballDx *= -1;
            PlaySound(hit);
        }

        // win state
        if (playerScore == 5 || opponentScore == 5)
        {
            // end game
            // reset game

            weHaveAWinner = true;
            PlaySound(win);

            if (playerScore == 5)
            {
                winner = 1;
            }
            else if (opponentScore == 5)
            {
                winner = 2;
            }

            playerScore = 0;
            opponentScore = 0;
            Ball.x = screenWidth / 2;
            Ball.y = screenHeight / 2;
            ballSpeed = 0;
        }

        // DRAW
        BeginDrawing();

        ClearBackground(DARKGRAY);
        // DrawRectangleRec(debugNet, LIGHTGRAY);
        DrawRectangleRec(Net, GOLD);
        DrawRectangleRec(Player, GOLD);
        DrawRectangleRec(Opponent, GOLD);
        DrawRectangleRec(Ball, GOLD);
        DrawText(TextFormat("%i", playerScore), screenWidth / 2 - 60, 20, 40, GOLD);
        DrawText(TextFormat("%i", opponentScore), screenWidth / 2 + 40, 20, 40, GOLD);

        if (weHaveAWinner)
        {
            if (winner == 1)
            {

                DrawText("You won!", 100, 100, 20, GOLD);
            }
            else
            {

                DrawText("The computer wins!", 100, 100, 20, GOLD);
            }

            DrawText("Press [SPACE] to play again", 80, 160, 16, GOLD);

            if (IsKeyDown(KEY_SPACE))
            {
                weHaveAWinner = false;
                ClearBackground(DARKGRAY);
                ballSpeed = 5;
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}