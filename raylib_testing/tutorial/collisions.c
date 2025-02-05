#include "raylib.h"
#include <stdio.h>



int main()
{
    // Initialize window
    const int screenWidth = 1920;
    const int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Collisions");

    // Define player and obstacle properties
    Vector2 playerPos = { 0, 0 };
    Vector2 obstaclePos = { 500, 400 };
    float playerRadius = 50;
    float obstacleRadius = 30;

    Rectangle r1 = {0, 0, 100, 200};
    Rectangle r2 = {800, 500, 200, 300};

    SetTargetFPS(60);

    bool collisionHappens = false;
    Rectangle collisionRec;
    
    while (!WindowShouldClose())
    {
        // Input
        playerPos = GetMousePosition();
        r1.x = GetMouseX();
        r1.y = GetMouseY();

        // printf("%s", CheckCollisionCircles(playerPos, playerRadius, obstaclePos, obstacleRadius) ? "true" : "false");
        collisionHappens = CheckCollisionCircleRec(playerPos, playerRadius, r1);
        printf("%d", collisionHappens);
        
        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        


        DrawCircleV(playerPos, playerRadius, WHITE);
        DrawCircleV(obstaclePos, obstacleRadius, RED);
        DrawRectangleRec(r1, BLUE);
        DrawRectangleRec(r2, GREEN);

        if (collisionHappens){
            collisionRec = GetCollisionRec(r1, r2);
            //(Color){,,,1}
            DrawRectangleRec(collisionRec, GOLD);            
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
