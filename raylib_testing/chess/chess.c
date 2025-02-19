#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
// #include <stdlib.h>

typedef enum PieceType {
    REMOVED=0,
    PAWN,   //paraszt
    KNIGHT, //ló
    ROOK,   //bástya
    BISHOP, //futó
    QUEEN,
    KING
} PieceType;

typedef enum GameState {
    PLAYER1_TURN,
    PLAYER2_TURN,
    GAME_OVER
} GameState;

typedef enum TileState {
    PLAYER1,
    PLAYER2,
    EMPTY
} TileState;

typedef struct Tile {
    PieceType CurrentPiece;
    uint16_t pos_x;
    uint16_t pos_y;
    TileState CurrentOwner;
    bool hasPieceMoved;
} Tile;

const int window_height = 600;
const int window_width = 800;
const int tile_size = 60;
const int offset_x = window_width / 2 - tile_size*8 / 2;
const int offset_y = window_height / 2 - tile_size*8 / 2;

const int restart_button_x = window_width/2-60;
const int restart_button_y = window_height/2-60;

#define MAP_SIZE 64

Tile map[MAP_SIZE];

Color BackgroundColors[MAP_SIZE];

Color CurrentColor;

Texture2D PieceTextures[12];

char GameStateMsg[256];

// GameState CURRENT_GAME_STATE;
TileState CURRENT_PLAYER = PLAYER1;

uint8_t LatestClickIdx = 255;
uint8_t SavedClickIdx = 255;

#define TRAVERSABLETILES_MAX 27

uint8_t TraversableTileIndices[TRAVERSABLETILES_MAX]; //because the queen can travel to up to 27 tiles in one step.
uint8_t TraversablesLen = 0;

uint8_t SavedPlayerIdx = 255;

void InitTextures(Texture2D* textureArr, int texturesLen){
    Image CurrentImage;
    Texture2D CurrentTexture;
    CurrentImage = LoadImage("chess_assets/bB.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[0] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/bK.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[1] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/bN.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[2] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/bP.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[3] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/bQ.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[4] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/bR.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[5] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/wB.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[6] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/wK.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[7] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/wN.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[8] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/wP.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[9] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/wQ.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[10] = CurrentTexture;

    CurrentImage = LoadImage("chess_assets/wR.png");
    ImageResize(&CurrentImage, tile_size, tile_size);
    CurrentTexture = LoadTextureFromImage(CurrentImage);
    textureArr[11] = CurrentTexture;

}

void InitTile(Tile* tile, PieceType piece, TileState owner, uint8_t pos_x, uint8_t pos_y){
    if (tile != NULL){
        tile->CurrentPiece = piece;
        tile->CurrentOwner = owner;
        tile->pos_x = offset_x+pos_x*tile_size;
        tile->pos_y = offset_y+pos_y*tile_size;
        if (piece == REMOVED){
            tile->hasPieceMoved = true; //so pawn start, en passant and castling won't be available on non-starter tiles
        }
        else {
            tile->hasPieceMoved = false;
        }
        
    }
}

void LoadStartingMap(Tile* map, int mapLen){
    //THESE ARE FOR TESTING ONLY:
    for (int i=0; i<MAP_SIZE; i++){
        InitTile(map+i, REMOVED, EMPTY, i%8, i/8);
    }
    // InitTile(map+3*8+0, QUEEN, PLAYER1, 0, 3);
    // InitTile(map+(8*6)+0, KING, PLAYER1, 0, 6);
    // InitTile(map+4*8+0, BISHOP, PLAYER1, 0, 4);

    // InitTile(map+7*8+0, QUEEN, PLAYER2, 0, 7);

    //ACTUAL TABLE HERE:

    InitTile(map, ROOK, PLAYER1, 0, 0);
    InitTile(map+1, KNIGHT, PLAYER1, 1, 0);
    InitTile(map+2, BISHOP, PLAYER1, 2, 0);
    InitTile(map+3, QUEEN, PLAYER1, 3, 0);
    InitTile(map+4, KING, PLAYER1, 4, 0);
    InitTile(map+5, BISHOP, PLAYER1, 5, 0);
    InitTile(map+6, KNIGHT, PLAYER1, 6, 0);
    InitTile(map+7, ROOK, PLAYER1, 7, 0);
    
    for (int i=0; i<8; i++){
        InitTile(map+8+i, PAWN, PLAYER1, i, 1);
        InitTile(map+(8*6)+i, PAWN, PLAYER2, i, 6); //lower player's pawns
    }
    // for (int i=2*8; i<6*8; i++){
    //     InitTile(map+i, REMOVED, EMPTY, i%8, i/8);
    // }

    InitTile(map+(8*7), ROOK, PLAYER2, 0, 7);
    InitTile(map+(8*7)+1, KNIGHT, PLAYER2, 1, 7);
    InitTile(map+(8*7)+2, BISHOP, PLAYER2, 2, 7);
    InitTile(map+(8*7)+4, KING, PLAYER2, 4, 7);
    InitTile(map+(8*7)+3, QUEEN, PLAYER2, 3, 7);
    InitTile(map+(8*7)+5, BISHOP, PLAYER2, 5, 7);
    InitTile(map+(8*7)+6, KNIGHT, PLAYER2, 6, 7);
    InitTile(map+(8*7)+7, ROOK, PLAYER2, 7, 7);
}

void DrawCharacterOnCurrentTile(Tile tile, int tileIdx){
    int textureIdx = 0;
    if (tile.CurrentOwner == EMPTY){
        return;
    }
    
    switch (tile.CurrentPiece){
        case PAWN:
            textureIdx = 3;
            break;
        case ROOK:
            textureIdx = 5;
            break;
        case KNIGHT:
            textureIdx = 2;
            break;
        case BISHOP:
            textureIdx = 0;
            break;
        case QUEEN:
            textureIdx = 4;
            break;
        case KING:
            textureIdx = 1;
            break;
        default:
            printf("unknown piece type: %d\n", tile.CurrentPiece);
            return;
    }

    if (tile.CurrentOwner == PLAYER1){
        textureIdx += 6;
    }
    
    DrawTexture(PieceTextures[textureIdx], offset_x+tileIdx%8*tile_size, offset_y+tileIdx/8*tile_size, WHITE);
}

void InitBackground(Color* colorsArr){
    for (int i=0; i<8; i++){ //rows (y coordinate)
        for (int j=0; j<8; j++){ //columns (x coordinate)
            if (i % 2 == 1){
                colorsArr[i*8+j] = (j % 2 == 1 ) ? WHITE : BLACK;
            }
            else { //if i % 2 == 0
                colorsArr[i*8+j] = (j % 2 == 1) ? BLACK : WHITE;
            }
        }
    }   
}

void DrawTileDebugInfo(){
    char buf[256];
    int usedUpLen = 0;
    for (int i=0; i<MAP_SIZE; i++){
        usedUpLen = sprintf(buf, "%d", i);
        buf[usedUpLen] = '\0';
        DrawText(buf, offset_x+i%8*tile_size, offset_y+i/8*tile_size, 20, RED);
    }
}

uint8_t GetClickedTileIdx(Tile* map){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        uint16_t mouse_x = (uint16_t)mouse_pos.x;
        uint16_t mouse_y = (uint16_t)mouse_pos.y;

        for (int i=0; i<MAP_SIZE; i++){
            // printf("pos checked at: x=%d, y=%d\n", map[i].pos_x, map[i].pos_y);
            
            if (mouse_x >= map[i].pos_x && mouse_x <= map[i].pos_x+tile_size
                && mouse_y >= map[i].pos_y && mouse_y <= map[i].pos_y+tile_size){
                return i;
            }
        }
    }
    return 255;
}

void DrawTraversableTiles(uint8_t* TraversableTileIndices, uint8_t TraversablesLen){
    if (TraversablesLen > TRAVERSABLETILES_MAX){
        printf("\ntraversables len is %d, and Max traversable things is %d. Big error happened.\n", TraversablesLen, TRAVERSABLETILES_MAX);
        return;
    }
    for (int i=0; i<TraversablesLen; i++){
        DrawRectangle(offset_x+TraversableTileIndices[i]%8*tile_size, offset_y+TraversableTileIndices[i]/8*tile_size, tile_size, tile_size, (Color){240, 0, 0, 120});
        // printf("Traversable index: %d\n", TraversableTileIndices[i]);
    }
}

//returns true if tiles in this path cannot be further traversed.
bool CheckTile(TileState CurrentTileOwner, TileState TileOwner, uint8_t* TraversableTileIndices, uint8_t* TraversablesLen, uint8_t TileIdx){
    TileState Enemy;
    if (CurrentTileOwner == TileOwner || TileIdx >= MAP_SIZE || TileIdx < 0){
        return true;
        //do nothing
    }
    else {
        if (TileOwner == PLAYER1){
            Enemy = PLAYER2;
        }
        else {
            Enemy = PLAYER1;
        }
        if (CurrentTileOwner == Enemy){
            TraversableTileIndices[*TraversablesLen] = TileIdx;
            *TraversablesLen += 1;
            return true;
        }
        else {
            TraversableTileIndices[*TraversablesLen] = TileIdx;
            *TraversablesLen += 1;
            
        }
    }
    return false;
}

void CalculateCrossMovement(Tile* map, uint8_t currentTileIdx, uint8_t* TraversableTileIndices, uint8_t* TraversablesLen){
    TileState TileOwner = map[currentTileIdx].CurrentOwner;

    for (int i=currentTileIdx-1; i>=(currentTileIdx/8)*8; i--){
        if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)){
            break;
        }
        
    }
    for (int i=currentTileIdx+1; i<(currentTileIdx/8)*8+8; i++){
        if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)){
            break;
        }
        
    }

    for (int i=currentTileIdx-8; i>=0; i-=8){
        if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)){
            break;
        }
        
    }
    for (int i=(currentTileIdx/8)*8+(currentTileIdx%8)+8; i<MAP_SIZE; i+=8){
        if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)){
            break;
        }
        
    }
}



void CalculateDiagonalMovement(Tile* map, uint8_t currentTileIdx, uint8_t* TraversableTileIndices, uint8_t* TraversablesLen) {
    TileState TileOwner = map[currentTileIdx].CurrentOwner;

    if (currentTileIdx % 8 != 7){
        for (int i=currentTileIdx-7; i>=0 && i%8 != 0; i-=7) {
            if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)) {
                break;
            }
        }
    }
    
    if (currentTileIdx % 8 != 0){
        for (int i=currentTileIdx+7; i<MAP_SIZE && i%8!=7; i+=7) { 
            if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)) {
                break;
            }
        }
    }

    if (currentTileIdx % 8 != 0){
        for (int i=currentTileIdx-9; i>=0; i-=9) {
            if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)) {
                break;
            }
            if (i%8 == 0){
                break;
            }
        }
    }
    
    if (currentTileIdx % 8 != 7){
        for (int i=currentTileIdx+9; i<MAP_SIZE; i+=9) {
            if (CheckTile(map[i].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, i)) {
                break;
            }
            if (i%8 == 7){
                break;
            }
        }
    }
    
}



void CalculatePawnMovement(Tile* map, uint8_t currentTileIdx, uint8_t* TraversableTileIndices, uint8_t* TraversablesLen){
    TileState TileOwner = map[currentTileIdx].CurrentOwner;
    uint8_t TileX = currentTileIdx%8;
    uint8_t TileY = currentTileIdx/8;
    // uint8_t multiplier = 1;
    TileState Enemy = PLAYER2;

    if (TileOwner == PLAYER2){
        // multiplier = -1;
        Enemy = PLAYER1;
        //Due to the TileX checking conditions, the "capturing" moves have to be coded out separately
        //Plus, the multiplier didn't even work for the vertical movement check for player 2...
        if (map[currentTileIdx-7].CurrentOwner == Enemy && TileX != 7){
            CheckTile(map[currentTileIdx-7].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-7);
        }
        if (map[currentTileIdx-9].CurrentOwner == Enemy && TileX != 0){
            CheckTile(map[currentTileIdx-9].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-9);
        }
        if (map[currentTileIdx-8].CurrentPiece == REMOVED){
            CheckTile(map[currentTileIdx-8].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-8);
        }
        if (!map[currentTileIdx].hasPieceMoved){
            if (map[currentTileIdx-16].CurrentPiece == REMOVED){
                CheckTile(map[currentTileIdx-16].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-16);
            }
        }
    }
    else if (TileOwner == PLAYER1){
        if (map[currentTileIdx+7].CurrentOwner == Enemy && TileX != 0){
            CheckTile(map[currentTileIdx+7].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+7);
        }
        if (map[currentTileIdx+9].CurrentOwner == Enemy && TileX != 7){
            CheckTile(map[currentTileIdx+9].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+9);
        }
        if (map[currentTileIdx+8].CurrentPiece == REMOVED){
            CheckTile(map[currentTileIdx+8].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+8);
        }
        if (!map[currentTileIdx].hasPieceMoved){
            if (map[currentTileIdx+16].CurrentPiece == REMOVED){
                CheckTile(map[currentTileIdx+16].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+16);
            }
        }
    }
    else {
        printf("TileOwner is %d. I don't even know how the game got there...\n", TileOwner);
        return;
    }
}

void CalculateKnightMovement(Tile* map, uint8_t currentTileIdx, uint8_t* TraversableTileIndices, uint8_t* TraversablesLen){
    TileState TileOwner = map[currentTileIdx].CurrentOwner;
    uint8_t TileX = currentTileIdx%8;
    uint8_t TileY = currentTileIdx/8;

    if (TileX < 6){
        CheckTile(map[currentTileIdx-6].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-6);
        CheckTile(map[currentTileIdx+10].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+10);
    }
    if (TileX < 7){
        CheckTile(map[currentTileIdx-15].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-15);
        CheckTile(map[currentTileIdx+17].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+17);
    }
    if (TileX > 1){
        CheckTile(map[currentTileIdx+6].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+6);
        CheckTile(map[currentTileIdx-10].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-10);
    }
    if (TileX > 0){
        CheckTile(map[currentTileIdx+15].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+15);
        CheckTile(map[currentTileIdx-17].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-17);
    }
    
}

void CalculateKingMovement(Tile* map, uint8_t currentTileIdx, uint8_t* TraversableTileIndices, uint8_t* TraversablesLen){
    TileState TileOwner = map[currentTileIdx].CurrentOwner;
    
    uint8_t TileX = currentTileIdx%8;
    uint8_t TileY = currentTileIdx/8;
    
    CheckTile(map[currentTileIdx-8].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-8);
    CheckTile(map[currentTileIdx+8].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+8);
    if (TileX != 7){
        CheckTile(map[currentTileIdx-7].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-7);
        CheckTile(map[currentTileIdx+1].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+1);
        CheckTile(map[currentTileIdx+9].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+9);
    }
    
    if (TileX != 0){
        CheckTile(map[currentTileIdx-1].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-1);
        CheckTile(map[currentTileIdx+7].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx+7);
        CheckTile(map[currentTileIdx-9].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, currentTileIdx-9);
    }
    

    //if king has moved, end the function
    if (map[currentTileIdx].hasPieceMoved){
        return;
    }
    //detect rooks in the row of the king, if they have been moved, this is invalid
    bool LeftCastling = true;
    if (!map[(TileY)*8].hasPieceMoved) {
        for (int i=currentTileIdx-1; i>(TileY)*8;i--){
            if (map[i].CurrentOwner != EMPTY){
                LeftCastling = false;
                break;
            }
        }
    }
    bool RightCastling = true;
    if (!map[(TileY)*8+7].hasPieceMoved){
        for (int i=currentTileIdx+1; i<(TileY)*8+7;i++){
            if (map[i].CurrentOwner != EMPTY){
                RightCastling = false;
                // printf("right castling blocked on idx %d\n", i);
                break;
            }
        }
    }
    
    if (LeftCastling){
        CheckTile(map[(TileY)*8+2].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, (TileY)*8+2);
    }
    if (RightCastling){
        CheckTile(map[(TileY)*8+6].CurrentOwner, TileOwner, TraversableTileIndices, TraversablesLen, (TileY)*8+6);
    }

}

void CalculateTraversableTiles(Tile* map, uint8_t currentTileIdx, uint8_t* TraversableTileIndices, uint8_t* TraversablesLen){
    uint8_t SavedEnemies[8] = {255, 255, 255, 255, 255, 255, 255, 255};
    uint8_t EnemiesIdx = 0;

    uint8_t SavedPosX = map[currentTileIdx].pos_x;
    uint8_t SavedPosY = map[currentTileIdx].pos_y;
    switch (map[currentTileIdx].CurrentPiece){
        case PAWN:
            *TraversablesLen = 0;
            CalculatePawnMovement(map, currentTileIdx, TraversableTileIndices, TraversablesLen);
            break;
        case ROOK:
            *TraversablesLen = 0;
            CalculateCrossMovement(map, currentTileIdx, TraversableTileIndices, TraversablesLen);
            break;

        case KNIGHT:
            *TraversablesLen = 0;
            CalculateKnightMovement(map, currentTileIdx, TraversableTileIndices, TraversablesLen);
            break;
        
        case BISHOP:
            *TraversablesLen = 0;
            CalculateDiagonalMovement(map, currentTileIdx, TraversableTileIndices, TraversablesLen);
            break;

        case QUEEN:
            *TraversablesLen = 0;
            CalculateDiagonalMovement(map, currentTileIdx, TraversableTileIndices, TraversablesLen);
            CalculateCrossMovement(map, currentTileIdx, TraversableTileIndices, TraversablesLen);

            
            break;
        case KING:
            *TraversablesLen = 0;
            CalculateKingMovement(map, currentTileIdx, TraversableTileIndices, TraversablesLen);
            break;
        
        default:
            printf("not implemented piece on the tile: %d\n", map[currentTileIdx].CurrentPiece);
            break;
    }
}

void DrawResartButton(int button_x, int button_y, int size_x, int size_y){
    DrawRectangle(button_x, button_y, size_x, size_y, GREEN);
    DrawText("Restart", button_x+5, button_y+5, 24, BLACK);
}

bool IsRestartButtonClicked(int button_x, int button_y, int size_x, int size_y){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mouse_pos = GetMousePosition();
        int mouse_x = (int)mouse_pos.x;
        int mouse_y = (int)mouse_pos.y;

        if (mouse_x > button_x && mouse_x < button_x+size_x && mouse_y > button_y && mouse_y < button_y+size_y){
            return true;
        }
    }
    return false;
}

void Move(Tile* map, uint8_t fromIdx, uint8_t toIdx, TileState* currentPlayer, char* gameStateMsgBuf){
    if (fromIdx >= MAP_SIZE || toIdx >= MAP_SIZE){
        printf("the idx of moved player's tile (%d) or the traversed tile(%d) is too large. \n", fromIdx, toIdx);
        return;
    }
    if (map[toIdx].CurrentOwner == map[fromIdx].CurrentOwner){
        printf("some bug happened. target tile's owner is the same as current tile's owner\n");   
    }
    uint8_t TileX = fromIdx%8;
    uint8_t TileY = fromIdx/8;
    if (map[fromIdx].CurrentPiece == KING){
        if (fromIdx > toIdx){
            if (fromIdx-toIdx == 2){
                // printf("Left Castle movement detected...\n");
                map[TileY*8].CurrentOwner = EMPTY;
                map[TileY*8].hasPieceMoved = true;
                map[TileY*8].CurrentPiece = REMOVED;
                map[TileY*8+3].CurrentOwner = *currentPlayer;
                // map[TileY*8+3].hasPieceMoved = true; //this should be already set
                map[TileY*8+3].CurrentPiece = ROOK;

            }
        }
        if (fromIdx < toIdx){
            if (toIdx-fromIdx == 2){
                // printf("Right Castle movement detected...\n");
                map[TileY*8+7].CurrentOwner = EMPTY;
                map[TileY*8+7].hasPieceMoved = true;
                map[TileY*8+7].CurrentPiece = REMOVED;
                map[TileY*8+5].CurrentOwner = *currentPlayer;
                // map[TileY*8+5].hasPieceMoved = true; //this should be already set
                map[TileY*8+5].CurrentPiece = ROOK;
            }
        }

    }

    if (map[toIdx].CurrentPiece == KING){
        *currentPlayer = EMPTY;
        if (map[fromIdx].CurrentOwner == PLAYER1){
            strcpy(gameStateMsgBuf, "player1 won!");
        }
        else if (map[fromIdx].CurrentOwner == PLAYER2){
            strcpy(gameStateMsgBuf, "player2 won!");
        }
        
    }
    // printf("%d hit %d", map[]);

    map[toIdx].CurrentOwner = map[fromIdx].CurrentOwner;
    map[toIdx].CurrentPiece = map[fromIdx].CurrentPiece;
    map[toIdx].hasPieceMoved = true;

    map[fromIdx].CurrentOwner = EMPTY;
    map[fromIdx].CurrentPiece = REMOVED;
    map[fromIdx].hasPieceMoved = true;



}



int main(){
    SetTargetFPS(144);
    LoadStartingMap(map, MAP_SIZE);
    InitWindow(window_width, window_height, "Chess Gameplay");
    SetTraceLogLevel(LOG_ERROR); 
    InitTextures(PieceTextures, 12);
    InitBackground(BackgroundColors);

    strcpy(GameStateMsg, "player1's turn");
    
    // CURRENT_GAME_STATE = PLAYER1_TURN; //player 1 starts for now
    CURRENT_PLAYER = PLAYER1;


    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);
        
        
        
        for (int i=0; i<MAP_SIZE; i++){
            DrawRectangle(offset_x+i%8*tile_size, offset_y+i/8*tile_size, tile_size, tile_size, BackgroundColors[i]);
            DrawCharacterOnCurrentTile(map[i], i);
        }

    
        if (LatestClickIdx < MAP_SIZE){
            SavedClickIdx = LatestClickIdx;
            // printf("click registered on: %d\n", SavedClickIdx);
        }
        if (CURRENT_PLAYER == EMPTY){
            DrawRectangle(offset_x, offset_y, 8*tile_size, 8*tile_size, (Color){0, 0, 0, 120});
            DrawResartButton(restart_button_x, restart_button_y, 120, 50);
            if (IsRestartButtonClicked(restart_button_x, restart_button_y, 120, 50)){
                LoadStartingMap(map, MAP_SIZE);
                strcpy(GameStateMsg, "player1's turn");
                CURRENT_PLAYER = PLAYER1;
            }
            goto LoopEnd;
        }

        DrawTileDebugInfo();
        
        LatestClickIdx = GetClickedTileIdx(map);

        DrawTraversableTiles(TraversableTileIndices, TraversablesLen);
        if (SavedClickIdx != 255){
            Tile current_tile = map[SavedClickIdx];
            if (current_tile.CurrentOwner == CURRENT_PLAYER){
                if (SavedClickIdx != SavedPlayerIdx){
                    printf("Player clicked his own tile.\n");
                }
                
                CalculateTraversableTiles(map, SavedClickIdx, TraversableTileIndices, &TraversablesLen);
                SavedPlayerIdx = SavedClickIdx;
            }
            //if a tile that isn't player's got clicked, check if that tile is in the TraversableIndices array
            else {
                for (int i=0; i<TraversablesLen; i++){
                    if (SavedClickIdx == TraversableTileIndices[i]){
                        // printf("%d has moved.\n", current_tile.CurrentPiece);
                        Move(map, SavedPlayerIdx, SavedClickIdx, &CURRENT_PLAYER, GameStateMsg);
                        if (CURRENT_PLAYER == PLAYER1){
                            CURRENT_PLAYER = PLAYER2;
                            strcpy(GameStateMsg, "player2's turn");
                        }
                        else if (CURRENT_PLAYER == PLAYER2){
                            CURRENT_PLAYER = PLAYER1;
                            strcpy(GameStateMsg, "player1's turn");
                        }
                        TraversablesLen = 0;
                        SavedClickIdx = 255;
                        SavedPlayerIdx = 255;
                    }
                }
            }
            
        }
        

        LoopEnd:
        DrawText(GameStateMsg, offset_x+tile_size*2, 20, 20, BLACK);
        DrawFPS(0, 0);
        EndDrawing();

        
        
    }
    CloseWindow();
    return 0;
}