#include "raylib.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sound");
    InitAudioDevice();

    //Sound is in the RAM
    Sound laser_sound = LoadSound("assets/laser.wav");
    PlaySound(laser_sound);

    //Music is loaded from the drive and is done on an other thread?
    Music background_music = LoadMusicStream("assets/music.wav");
    PlayMusicStream(background_music);
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose()) {
        UpdateMusicStream(background_music);
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }
    UnloadMusicStream(background_music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
