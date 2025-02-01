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
    // PLAYER1_SELECTING,
    PLAYER2_TURN,
    // PLAYER2_SELECTING,

    PLAYER1_WON,
    PLAYER2_WON,
} GameState;

const int window_height = 600;
const int window_width = 800;
const int tile_size = 60;
const int offset_x = window_width / 2 - tile_size*8 / 2;
const int offset_y = window_height / 2 - tile_size*8 / 2;

//if user clicked, return the index of the chess piece he/she clicked on. Else, return an out-of-bounds value
uint8_t GetSelectedPieceIdxOnClick(Piece* PieceArr, uint8_t MaxIters){
    uint32_t mouse_x;
    uint32_t mouse_y;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mouse_pos = GetMousePosition();
        mouse_x = (uint32_t)mouse_pos.x;
        mouse_y = (uint32_t)mouse_pos.y;
        // printf("mouse x: %d, mouse y: %d\n", mouse_x, mouse_y);
        for (int i=0; i<MaxIters; i++){
            Piece p = PieceArr[i];
            if (offset_x+p.board_pos_x*tile_size < mouse_x
                && offset_x+p.board_pos_x*tile_size+tile_size > mouse_x
                && offset_y+p.board_pos_y*tile_size < mouse_y
                && offset_y+p.board_pos_y*tile_size+tile_size > mouse_y){
                
                return i;
            }
        }
    }
    return UINT8_MAX;
}

void DeterminePossibleMoves(Piece* PieceArr, Piece* EnemyArr, int PieceArrLen, int EnemyArrLen, int SelectedIdx, Vector2* PossiblePositions, int* PositionsLenPtr, bool IsUpper){
    int r = tile_size/2;
    Piece current_piece = PieceArr[SelectedIdx];
    switch (current_piece.type){
        case PAWN:
            int m = 1; //multiplier
            if (!IsUpper){
                m = -1;
            }
            if (current_piece.board_pos_y == 1 || (current_piece.board_pos_y == 6 && !IsUpper)){
                PossiblePositions[0].x = (float)offset_x+current_piece.board_pos_x*tile_size;
                PossiblePositions[0].y = (float)offset_y+(current_piece.board_pos_y+m)*tile_size;
                PossiblePositions[1].x = (float)offset_x+current_piece.board_pos_x*tile_size;
                PossiblePositions[1].y = (float)offset_y+(current_piece.board_pos_y+m*2)*tile_size;

                *PositionsLenPtr = 2;
                printf("positions: (%f, %f) | (%f, %f)\n", PossiblePositions[0].x, PossiblePositions[0].y, PossiblePositions[1].x, PossiblePositions[1].y);
            }
            else {
                PossiblePositions[0].x = (float)offset_x+current_piece.board_pos_x*tile_size;
                PossiblePositions[0].y = (float)offset_y+m*(current_piece.board_pos_y+m)*tile_size; //current_piece.board_pos_y is not tested yet

                //todo: write a checker for the case of "hitting" an opponent character down
                *PositionsLenPtr = 1;
            }
        
            break;
        case ROOK:
            
            
            break;
        case KNIGHT:
            break;
        case BISHOP:
            break;
        case QUEEN:
            break;
        case KING:
            break;
        default:
            printf("Unexpected type for current piece: %d", current_piece.type);
            break;
        
    }
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

    GameState game_state = PLAYER1_TURN;
    Color current_color;
    
    uint8_t selected_piece_idx;

    char state_message[256] = "player1's turn";

    //exact calculated values will be stored here
    Vector2 possible_step_positions[21] = {0};  //queen can go through 7 tiles in one direction, so 21
    //something is needed to keep track of the current number of tiles that can be traveled
    int step_positions_len = {0};

    Color marking_color = {240, 0, 0, 120};

    Vector2 tile_size_vec = {(float)tile_size, (float)tile_size};
    
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
                selected_piece_idx = GetSelectedPieceIdxOnClick(player1_alive_pieces, player1_alive_count);
                if (selected_piece_idx < player1_alive_count){
                    DeterminePossibleMoves(
                        player1_alive_pieces, player2_alive_pieces, 
                        player1_alive_count, player2_alive_count, 
                        selected_piece_idx, 
                        possible_step_positions, &step_positions_len, 
                        true
                    );
                }
                

                for (int i=0; i<step_positions_len; i++){
                    DrawRectangleV(possible_step_positions[i], tile_size_vec, marking_color);
                }


                for (int i=0; i<step_positions_len; i++){

                }

                break;
            case PLAYER2_TURN:
                selected_piece_idx = GetSelectedPieceIdxOnClick(player2_alive_pieces, player2_alive_count);
                if (selected_piece_idx < player2_alive_count){
                    DeterminePossibleMoves(
                        player2_alive_pieces, player1_alive_pieces, 
                        player2_alive_count, player1_alive_count, 
                        selected_piece_idx, 
                        possible_step_positions, &step_positions_len, 
                        false
                    );
                }

                for (int i=0; i<step_positions_len; i++){
                    DrawRectangleV(possible_step_positions[i], tile_size_vec, marking_color);
                }

                break;
            case PLAYER1_WON:
                break;
            case PLAYER2_WON:
                break;
            
        }

        DrawText(state_message, 300, 20, 20, BLACK);
        
        DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}