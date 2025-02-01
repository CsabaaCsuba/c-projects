#include "raylib.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
    PieceType type;
    uint8_t board_pos_x;
    uint8_t board_pos_y;
} Piece;

typedef enum {
    PLAYER1_TURN,
    PLAYER1_SELECTING,
    PLAYER2_TURN,
    PLAYER2_SELECTING,

    PLAYER1_WON,
    PLAYER2_WON,
} GameState;

const int window_height = 600;
const int window_width = 800;
const int tile_size = 60;

uint8_t GetSelectedPieceIdx(Piece* PieceArr, uint8_t MaxIters){
    
}

int main(){
    InitWindow(window_width, window_height, "Chess Gameplay");

    Piece player1_alive_pieces[16] = {
        (Piece){ROOK, 0, 0},
        (Piece){KNIGHT, 1, 0},
        (Piece){BISHOP, 2, 0},
        (Piece){QUEEN, 3, 0},
        (Piece){KING, 4, 0},
        (Piece){BISHOP, 5, 0},
        (Piece){KNIGHT, 6, 0},
        (Piece){ROOK, 7, 0},

        (Piece){PAWN, 0, 1},
        (Piece){PAWN, 1, 1},
        (Piece){PAWN, 2, 1},
        (Piece){PAWN, 3, 1},
        (Piece){PAWN, 4, 1},
        (Piece){PAWN, 5, 1},
        (Piece){PAWN, 6, 1},
        (Piece){PAWN, 7, 1},
        
    };
    
    Piece player1_dead_pieces[16] = {0};

    Piece player2_alive_pieces[16] = {
        (Piece){ROOK, 0, 7},
        (Piece){KNIGHT, 1, 7},
        (Piece){BISHOP, 2, 7},
        (Piece){QUEEN, 3, 7},
        (Piece){KING, 4, 7},
        (Piece){BISHOP, 5, 7},
        (Piece){KNIGHT, 6, 7},
        (Piece){ROOK, 7, 7},

        (Piece){PAWN, 0, 6},
        (Piece){PAWN, 1, 6},
        (Piece){PAWN, 2, 6},
        (Piece){PAWN, 3, 6},
        (Piece){PAWN, 4, 6},
        (Piece){PAWN, 5, 6},
        (Piece){PAWN, 6, 6},
        (Piece){PAWN, 7, 6},
    };

    Piece player2_dead_pieces[16] = {0};

    uint8_t player1_alive_count = 16;
    uint8_t player2_alive_count = 16;

    GameState game_state = PLAYER1_SELECTING;
    Color current_color;
    const int offset_x = window_width / 2 - tile_size*8 / 2;
    const int offset_y = window_height / 2 - tile_size*8 / 2;

    char state_message[256] = {0};
    
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);
        
        for (int i=0; i<8; i++){ //rows (y coordinate)
            for (int j=0; j<8; j++){ //columns (x coordinate)
                if (i % 2 == 1){
                    if (j % 2 == 1)
                        current_color = WHITE;
                    else
                        current_color = BLACK;
                }
                else { //if i % 2 == 0
                    if (j % 2 == 1)
                        current_color = BLACK;
                    else
                        current_color = WHITE;
                }
                DrawRectangle(offset_x+j*tile_size, offset_y+i*tile_size, tile_size, tile_size, current_color);
            }
        }

        for (int i=0; i<player1_alive_count; i++){
            switch (player1_alive_pieces[i].type){
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
                    printf("unknown piece type: %d\n", player1_alive_pieces[i].type);
                    return -1;
            }

            DrawRectangle(offset_x+player1_alive_pieces[i].board_pos_x*tile_size, offset_y+player1_alive_pieces[i].board_pos_y*tile_size, tile_size, tile_size, current_color);
        }
        for (int i=0; i<player2_alive_count; i++){
            switch (player2_alive_pieces[i].type){
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
                    printf("unknown piece type: %d\n", player2_alive_pieces[i].type);
                    return -1;
            }
            

            DrawRectangle(offset_x+player2_alive_pieces[i].board_pos_x*tile_size, offset_y+player2_alive_pieces[i].board_pos_y*tile_size, tile_size, tile_size, current_color);

        }

        switch (game_state){
            case PLAYER1_TURN:
                break;
            case PLAYER2_TURN:
                break;
            case PLAYER1_SELECTING:
                break;
            case PLAYER2_SELECTING:
                break;
            case PLAYER1_WON:
                break;
            case PLAYER2_WON:
                break;
            
        }

        
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}