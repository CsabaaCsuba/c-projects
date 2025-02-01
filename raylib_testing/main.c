#include <raylib.h>
#include <raymath.h>

int main(){

    const int windowHeight = 800;
    const int windowWidth = 800;
    const int gameSpeed = 200;

    Color green = {20, 160, 133, 255};
    SetExitKey(KEY_ESCAPE);

    InitWindow(windowWidth, windowHeight, "Mein First Raylib Game");
    // SetTargetFPS(60);

    Image miau_cat_image = LoadImage("assets/sad_miau.png");
    ImageColorGrayscale(&miau_cat_image);
    ImageResize(&miau_cat_image, 100, 100);
    Texture2D second_miau_cat = LoadTextureFromImage(miau_cat_image);

    Texture2D dracula = LoadTexture("assets/mini_dracula.png");
    Vector2 dracula_direction = {0, 0};
    Vector2 dracula_pos = {0, 0};

    Vector2 mini_miau_placement = {0.0, 0.0};

    float dt = 0;

    //Game loop
    while(WindowShouldClose() == false){
        dt = GetFrameTime();
        

        dracula_direction.x = IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
        dracula_direction.y = IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
        dracula_direction = Vector2Normalize(dracula_direction);

        dracula_pos.x += dracula_direction.x * gameSpeed * dt;
        dracula_pos.y += dracula_direction.y * gameSpeed * dt;
        mini_miau_placement.x += 100 * dt;
        if (mini_miau_placement.x > windowWidth) {
            mini_miau_placement.x = 0-second_miau_cat.width;
        }
        

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircleV((Vector2){400, 400}, 50, YELLOW);

        // DrawLineEx((Vector2){0, 0}, (Vector2){500, 500}, 10, (Color){50, 0, 0, 255});


        
        
        DrawTextureV(second_miau_cat, mini_miau_placement, WHITE);

        DrawTextureV(dracula, dracula_pos, WHITE);


        DrawFPS(0, 0);
        EndDrawing();
    }


    CloseWindow();
    return 0;
}