#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
// #include <stdlib.h>

typedef enum {
    REMOVED=0,
    PAWN,   //paraszt
    KNIGHT, //ló
    ROOK,   //bástya
    BISHOP, //futó
    QUEEN,
    KING
} PieceType;

typedef struct {
    PieceType CurrentPiece;
    bool IsUpperPlayer;
    bool IsEmpty;
} Tile;

const int window_height = 600;
const int window_width = 800;
const int tile_size = 60;
const int offset_x = window_width / 2 - tile_size*8 / 2;
const int offset_y = window_height / 2 - tile_size*8 / 2;
#define MAP_SIZE 64

Tile map[MAP_SIZE];

Color current_color;

void InitTile(Tile* tile, PieceType piece, bool isUpper, bool isEmpty){
    if (tile != NULL){
        tile->CurrentPiece = piece;
        tile->IsUpperPlayer = isUpper;
        tile->IsEmpty = isEmpty;
    }
}
void InitTileOnMap(Tile* map, int x, int y, PieceType piece, bool isUpper, bool isEmpty){

}
    

void LoadStartingMap(Tile* map, int mapLen){
    InitTile(map, ROOK, true, false);
    InitTile(map+1, KNIGHT, true, false);
    InitTile(map+2, BISHOP, true, false);
    InitTile(map+3, QUEEN, true, false);
    InitTile(map+4, KING, true, false);
    InitTile(map+5, BISHOP, true, false);
    InitTile(map+6, KNIGHT, true, false);
    InitTile(map+7, ROOK, true, false);
    
    for (int i=0; i<8; i++){
        InitTile(map+8+i, PAWN, true, false);
        InitTile(map+(8*6)+i, PAWN, false, false); //lower player's characters
    }
    for (int i=2*8; i<6*8; i++){
        InitTile(map+i, REMOVED, false, true);
    }

    InitTile(map+(8*7), ROOK, false, false);
    InitTile(map+(8*7)+1, KNIGHT, false, false);
    InitTile(map+(8*7)+2, BISHOP, false, false);
    InitTile(map+(8*7)+3, KING, false, false);
    InitTile(map+(8*7)+4, QUEEN, false, false);
    InitTile(map+(8*7)+5, BISHOP, false, false);
    InitTile(map+(8*7)+6, KNIGHT, false, false);
    InitTile(map+(8*7)+7, ROOK, false, false);
}

void DrawCharacterOnCurrentTile(Tile tile, int tileIdx){
    if (tile.IsEmpty){
        return;
    }
    switch (tile.CurrentPiece){
        //TODO: create an image array and a current_image variable later...
        case PAWN:
            current_color = PURPLE;
            break;
        case ROOK:
            current_color = BLUE;
            break;
        case KNIGHT:
            current_color = BROWN;
            break;
        case BISHOP:
            current_color = YELLOW;
            break;
        case QUEEN:
            current_color = PINK;
            break;
        case KING:
            current_color = GOLD;
            break;
        default:
            printf("unknown piece type: %d\n", tile.CurrentPiece);
            return;
    }

    Color used_color;
    if (tile.IsUpperPlayer){
        used_color.r = current_color.r - (current_color.r)/3;
        used_color.g = current_color.g - (current_color.g)/3;
        used_color.b = current_color.b - (current_color.b)/3;
        used_color.a = current_color.a;
    }
    else {
        used_color = current_color;
    }

    DrawRectangle(offset_x+ tileIdx % 8 * tile_size, offset_y + tileIdx / 8 * tile_size, tile_size, tile_size, used_color);
    
}

int main(){
    LoadStartingMap(map, MAP_SIZE);
    InitWindow(window_width, window_height, "Chess Gameplay");

    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);
        for (int i=0; i<8; i++){ //rows (y coordinate)
            for (int j=0; j<8; j++){ //columns (x coordinate)
                if (i % 2 == 1){
                    current_color = (j % 2 == 1 ) ? WHITE : BLACK;
                }
                else { //if i % 2 == 0
                    current_color = (j % 2 == 1) ? BLACK : WHITE;
                }
                DrawRectangle(offset_x+j*tile_size, offset_y+i*tile_size, tile_size, tile_size, current_color);
            }
        }
        for (int i=0; i<MAP_SIZE; i++){
            DrawCharacterOnCurrentTile(map[i], i);
        }


        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}