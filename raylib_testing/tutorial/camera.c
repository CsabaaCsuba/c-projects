#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define NUM_CIRCLES 100
#define PLAYER_SPEED 400.0f

typedef struct {
    Vector2 position;
    int radius;
    Color color;
} CircleObject;

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera");
    
    Vector2 pos = {0};
    float radius = 50.0f;
    Vector2 direction = {0};
    
    //random generating circles
    CircleObject circles[NUM_CIRCLES];
    srand(time(NULL));
    for (int i = 0; i < NUM_CIRCLES; i++) {
        circles[i].position = (Vector2){ (float)(rand() % 4001 - 2000), (float)(rand() % 2001 - 1000) };
        circles[i].radius = rand() % 151 + 50;
        circles[i].color = (Color){ rand() % 256, rand() % 256, rand() % 256, 255 };
    }
    
    // SetTargetFPS(60);

    Camera2D camera = {};
    camera.zoom = 1;
    camera.target = pos;
    camera.offset = (Vector2){SCREEN_WIDTH/2, SCREEN_HEIGHT/2};
    camera.rotation = 0;

    while (!WindowShouldClose()) {
        // Input handling
        direction.x = (IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT));
        direction.y = (IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP));
        float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction.x /= length;
            direction.y /= length;
        }
        
        // Movement
        float dt = GetFrameTime();
        pos.x += direction.x * PLAYER_SPEED * dt;
        pos.y += direction.y * PLAYER_SPEED * dt;


        float rotate_direction = (IsKeyDown(KEY_A) - IsKeyDown(KEY_D));
        float zoom_direction = (IsKeyDown(KEY_Q) - IsKeyDown(KEY_E));
        camera.rotation += rotate_direction * 10 * dt;
        camera.zoom += zoom_direction * dt * 2;
        camera.zoom = fmax(0.2, fmin(5, camera.zoom));
        camera.target = pos;
        
        // Drawing
        BeginDrawing();
        BeginMode2D(camera);
        ClearBackground(WHITE);
        for (int i = 0; i < NUM_CIRCLES; i++) {
            DrawCircleV(circles[i].position, circles[i].radius, circles[i].color);
        }
        DrawCircleV(pos, radius, BLACK);
        EndMode2D();
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
