#include "raylib.h"

int main(){

    const int windowHeight = 800;
    const int windowWidth = 800;

    int ballX = 400;
    int ballY = 400;
    Color green = {20, 160, 133, 255};

    InitWindow(windowWidth, windowHeight, "Mein First Raylib Game");
    SetTargetFPS(60);

    //Game loop
    while(WindowShouldClose() == false){
        //1. event handling
        if (IsKeyDown(KEY_RIGHT)){
            ballX += 3;
        }
        if (IsKeyDown(KEY_LEFT)){
            ballX -= 3;
        }
        if (IsKeyDown(KEY_UP)){
            ballY -= 3;
        }
        if (IsKeyDown(KEY_DOWN)){
            ballY += 3;
        }

        if (ballY < 0){
            ballY = windowHeight;
        }
        if (ballY > windowHeight){
            ballY = 0;
        }
        if (ballX > windowWidth){
            ballX = 0;
        }
        if (ballX < 0){
            ballX = windowWidth;
        } 
        

        //2. updating positions

        //3. drawing
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(ballX, ballY, 20.0, WHITE);

        EndDrawing();
    }


    CloseWindow();
    return 0;
}