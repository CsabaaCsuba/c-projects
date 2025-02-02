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
    uint16_t board_pos_x;
    uint16_t board_pos_y;
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
    uint16_t mouse_x;
    uint16_t mouse_y;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mouse_pos = GetMousePosition();
        mouse_x = (uint16_t)mouse_pos.x;
        mouse_y = (uint16_t)mouse_pos.y;
        // printf("mouse x: %d, mouse y: %d\n", mouse_x, mouse_y);
        for (int i=0; i<MaxIters; i++){
            Piece p = PieceArr[i];
            if (p.board_pos_x < mouse_x
                && p.board_pos_x+tile_size > mouse_x
                && p.board_pos_y < mouse_y
                && p.board_pos_y+tile_size > mouse_y){
                
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
        case PAWN: { //TODO: ensure that pawns cannot move to a character forward.
                int m = 1; //multiplier
                if (!IsUpper){
                    m = -1;
                }
                printf("pawn case\n");
                if (current_piece.board_pos_y == offset_y+tile_size || (current_piece.board_pos_y == offset_y+tile_size*6 && !IsUpper)){
                    PossiblePositions[0].x = (float)current_piece.board_pos_x;
                    PossiblePositions[0].y = (float)current_piece.board_pos_y+m*tile_size;
                    PossiblePositions[1].x = (float)current_piece.board_pos_x;
                    PossiblePositions[1].y = (float)current_piece.board_pos_y+m*2*tile_size;

                    *PositionsLenPtr = 2;
                }
                else {
                    PossiblePositions[0].x = (float)current_piece.board_pos_x;
                    PossiblePositions[0].y = (float)current_piece.board_pos_y+m*tile_size; //current_piece.board_pos_y is not tested yet

                    //todo: write a checker for the case of "hitting" an opponent character down
                    *PositionsLenPtr = 1;
                }
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

uint8_t GetClickedTileIdx(Vector2* PossiblePositions, int PositionsLen){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        Vector2 mouse_pos = GetMousePosition();
        // printf("Mouse clicked here: %f, %f\n", mouse_pos.x, mouse_pos.y);
        Vector2 pos;
        for (uint8_t i=0; i<PositionsLen; i++){
            pos = PossiblePositions[i];
            if (mouse_pos.x < pos.x+(float)tile_size
                && mouse_pos.x > pos.x
                && mouse_pos.y < pos.y+(float)tile_size
                && mouse_pos.y > pos.y){
                // printf("bruh\n\n\n");
                return i;
            }
        }
    }
    return UINT8_MAX;
}

int main(){
    InitWindow(window_width, window_height, "Chess Gameplay");

    Piece player1_alive_pieces[16] = {
        (Piece){ROOK, 160, offset_y+0}, 
        (Piece){KNIGHT, 220, offset_y+0},
        (Piece){BISHOP, 280, offset_y+0},
        (Piece){QUEEN, 340, offset_y+0},
        (Piece){KING, 400, offset_y+0},
        (Piece){BISHOP, 460, offset_y+0},
        (Piece){KNIGHT, 520, offset_y+0},
        (Piece){ROOK, 580, offset_y+0},

        (Piece){PAWN, offset_x+0*tile_size, offset_y+tile_size},
        (Piece){PAWN, offset_x+1*tile_size, offset_y+tile_size},
        (Piece){PAWN, offset_x+2*tile_size, offset_y+tile_size},
        (Piece){PAWN, offset_x+3*tile_size, offset_y+tile_size},
        (Piece){PAWN, offset_x+4*tile_size, offset_y+tile_size},
        (Piece){PAWN, offset_x+5*tile_size, offset_y+tile_size},
        (Piece){PAWN, offset_x+6*tile_size, offset_y+tile_size},
        (Piece){PAWN, offset_x+7*tile_size, offset_y+tile_size},
        
    };
    
    Piece player1_dead_pieces[16] = {0};

    Piece player2_alive_pieces[16] = {
        (Piece){ROOK, offset_x+0*tile_size, offset_y+tile_size*7},    //kék
        (Piece){KNIGHT, offset_x+1*tile_size, offset_y+tile_size*7},  //barna
        (Piece){BISHOP, offset_x+2*tile_size, offset_y+tile_size*7},  //sárga
        (Piece){QUEEN, offset_x+3*tile_size, offset_y+tile_size*7},   //pink
        (Piece){KING, offset_x+4*tile_size, offset_y+tile_size*7},    //arany
        (Piece){BISHOP, offset_x+5*tile_size, offset_y+tile_size*7},  //sárga
        (Piece){KNIGHT, offset_x+6*tile_size, offset_y+tile_size*7},  //barna
        (Piece){ROOK, offset_x+7*tile_size, offset_y+tile_size*7},    //kék

        (Piece){PAWN, offset_x+0*tile_size, offset_y+tile_size*6},
        (Piece){PAWN, offset_x+1*tile_size, offset_y+tile_size*6},
        (Piece){PAWN, offset_x+2*tile_size, offset_y+tile_size*6},
        (Piece){PAWN, offset_x+3*tile_size, offset_y+tile_size*6},
        (Piece){PAWN, offset_x+4*tile_size, offset_y+tile_size*6},
        (Piece){PAWN, offset_x+5*tile_size, offset_y+tile_size*6},
        (Piece){PAWN, offset_x+6*tile_size, offset_y+tile_size*6},
        (Piece){PAWN, offset_x+7*tile_size, offset_y+tile_size*6},
    };

    Piece player2_dead_pieces[16] = {0};

    uint8_t player1_alive_count = 16;
    uint8_t player2_alive_count = 16;

    GameState game_state = PLAYER1_TURN;
    Color current_color;
    
    uint8_t selected_piece_idx;
    uint8_t last_piece_idx;

    char state_message[256] = "player1's turn";

    //exact calculated values will be stored here
    Vector2 possible_step_positions[21] = {0};  //queen can go through 7 tiles in one direction, so 21
    //something is needed to keep track of the current number of tiles that can be traveled
    int step_positions_len = 0;

    Color marking_color = {240, 0, 0, 120};

    Vector2 tile_size_vec = {(float)tile_size, (float)tile_size};
    uint8_t clicked_tile_idx = UINT8_MAX;
    
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
            DrawRectangle(player1_alive_pieces[i].board_pos_x, player1_alive_pieces[i].board_pos_y, tile_size, tile_size, current_color);
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
            
            DrawRectangle(player2_alive_pieces[i].board_pos_x, player2_alive_pieces[i].board_pos_y, tile_size, tile_size, current_color);
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
                    last_piece_idx = selected_piece_idx; 
                }
                

                for (int i=0; i<step_positions_len; i++){
                    DrawRectangleV(possible_step_positions[i], tile_size_vec, marking_color);
                }

                clicked_tile_idx = GetClickedTileIdx(possible_step_positions, step_positions_len);
                

                if (clicked_tile_idx < step_positions_len){
                    printf("clicked tile idx: %d", clicked_tile_idx);
                    uint16_t new_x = (uint16_t)possible_step_positions[clicked_tile_idx].x;
                    uint16_t new_y = (uint16_t)possible_step_positions[clicked_tile_idx].y;
                    player1_alive_pieces[last_piece_idx].board_pos_x = new_x;
                    player1_alive_pieces[last_piece_idx].board_pos_y = new_y;
                    step_positions_len = 0;
                    memset(possible_step_positions, 0, sizeof(possible_step_positions));
                    game_state = PLAYER2_TURN;
                    strcpy(state_message, "player2's turn");
                    printf("position should have changed...\n");
                    continue;
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
                    last_piece_idx = selected_piece_idx; 
                }

                for (int i=0; i<step_positions_len; i++){
                    DrawRectangleV(possible_step_positions[i], tile_size_vec, marking_color);
                }

                clicked_tile_idx = GetClickedTileIdx(possible_step_positions, step_positions_len);
                if (clicked_tile_idx < step_positions_len){
                    printf("clicked tile idx: %d", clicked_tile_idx);
                    uint16_t new_x = (uint16_t)possible_step_positions[clicked_tile_idx].x;
                    uint16_t new_y = (uint16_t)possible_step_positions[clicked_tile_idx].y;
                    player2_alive_pieces[last_piece_idx].board_pos_x = new_x;
                    player2_alive_pieces[last_piece_idx].board_pos_y = new_y;
                    step_positions_len = 0;
                    memset(possible_step_positions, 0, sizeof(possible_step_positions));
                    game_state = PLAYER1_TURN;
                    strcpy(state_message, "player1's turn");
                    printf("position should have changed...\n");
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