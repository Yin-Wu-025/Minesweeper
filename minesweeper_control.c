#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "minesweeper_view.h"
#include "minesweeper_control.h"
#include "minesweeper_model.h"
#include "minesweeper_unit_test.h"


static bool Is_Bomb_Random = true; // select bomb generation by randon or input, default by random

// Game message center
static void print_game_message(GAME_MESSAGE_E message_type, MESSAGE_U message_input) {

    Set_Position(0, Y_LENGTH + message_type);

    switch (message_type) {
    case MESSAGE_BOMB_LEFT: printf("Bombs Left: %3d", message_input.bomb_num); break;
    case MESSAGE_GAME_RESULT:
        if (message_input.is_win) {
            printf("Congratulation!! You Win!");
        }
        else {
            printf("Game Over! :(");
        }
        break;
    case MESSAGE_REPLAY: printf("Do you want to replay ? [Y / N]"); break;
    case MESSAGE_THANKS: printf("Thank you :)");
    default: break;
    }
}

// set the method of bomb generation
static void select_game_mode() {

    printf("Play by generating bomb randomly? [Y/N] \n");
    switch (getch()) {
    case 'y': case 'Y': Set_Is_Bomb_Random_Status(true); break;
    case 'n': case 'N': Set_Is_Bomb_Random_Status(false); break;
    default: Set_Is_Bomb_Random_Status(true); break;
    }
    system("cls");
}

// print map for GUI (map has been generated)
static void print_map(bool show) {
    for (int i = 0; i < CELL_NUM; i++) {
        int x = i % X_LENGTH;
        int y = i / X_LENGTH;
        CELL_T* cell = Get_Cell(x, y);
        Char_Print(x, y, Print_Cell(cell->cell_state, cell->cell_type, show));
    }
}

// play again check when gameover
static bool play_again_check() {
    while (true) {
        switch (getch()) {
        case 'y': case 'Y': system("cls"); return true;
        case 'n': case 'N': return false;
        default: break;
        }
    }
}

// play operation for GUI
static void play_operation() {
    int previous_y = 0, previous_x = 0;
    int current_y = 0, current_x = 0;
    int flagged = 0, bombsFlagged = 0;
    bool gameover = false;
    MESSAGE_U message;
    while (!gameover) {
        switch (getch()) {
        case OPERATION_LEFT: if (!Margin_Check(current_x - 1, current_y)) current_x--; break;
        case OPERATION_RIGHT: if (!Margin_Check(current_x + 1, current_y)) current_x++; break;
        case OPERATION_DOWN: if (!Margin_Check(current_x, current_y - 1)) current_y--; break;
        case OPERATION_UP: if (!Margin_Check(current_x, current_y + 1)) current_y++; break;
        case OPERATION_OPEN: gameover = Open_Cell(current_x, current_y); break;
        case OPERATION_FLAG_1: case OPERATION_FLAG_2:
            if (Flag_Cell(current_x, current_y, &flagged, &bombsFlagged, BOMB_NUM)) {
                message.bomb_num = BOMB_NUM - flagged;
                print_game_message(MESSAGE_BOMB_LEFT, message);
            }
            break;
        case OPERATION_LEAVE_1: case OPERATION_LEAVE_2: print_map(true); return; //leave
        }

        // print current cell and recover previous cell
        CELL_T* cell = Get_Cell(previous_x, previous_y);
        Char_Print(previous_x, previous_y, Print_Cell(cell->cell_state, cell->cell_type, false));
        Char_Print(current_x, current_y, 'P');

        // win decision (if all bombs flagged-> win!)
        if (bombsFlagged == BOMB_NUM && flagged == BOMB_NUM) {
            message.is_win = true;
            gameover = true;
            print_game_message(MESSAGE_GAME_RESULT, message);
        }

        previous_y = current_y; previous_x = current_x;
    }
    print_map(true);
}

// Set flag for bomb-position generation 
void Set_Is_Bomb_Random_Status(bool is_true) {
    Is_Bomb_Random = is_true;
}

// Get flag for bomb-position generation
bool Get_Is_Bomb_Random_Status() {
    return Is_Bomb_Random;
}

// Operation "Open" for player
bool Open_Cell(int x, int y) {
    
    if (!Margin_Check(x, y)) {
        CELL_T* cell = Get_Cell(x, y);

        if (cell->cell_state != STATE_OPEN) {
            cell->cell_state = STATE_OPEN;
            Char_Print(x, y, Print_Cell(cell->cell_state, cell->cell_type, true));

            if (cell->cell_type == TYPE_EMPTY) {
                Open_Neighbor_Cells(x, y);
            }
            else if (cell->cell_type == TYPE_BOOM) {
                MESSAGE_U message; 
                message.is_win = false;
                cell->cell_state = STATE_BOMB_HIT;
                print_game_message(MESSAGE_GAME_RESULT, message);
                return true;
            }
        }
    }
    return false;
}

// Set bomb position by input
void Bomb_Position_By_Input(int* x, int* y, int bomb_idx) {

    MESSAGE_U message_input;
    message_input.bomb_idx = bomb_idx;
    char dummy;

    printf("%d bomb position: \n", (message_input.bomb_idx) + 1);
    while (1) {
        printf("Type in X-position  (x from 0 to %d): ", X_LENGTH - 1);
        while (scanf_s("%d", x) != 1) {
            scanf_s("%c", &dummy); // use a character off the buffer
            printf("Type in X-position  (x from 0 to %d): ", X_LENGTH - 1);
        }

        printf("Type in Y-position  (y from 0 to %d): ", Y_LENGTH - 1);
        while (scanf_s("%d", y) != 1) {
            scanf_s("%c", &dummy);// use a character off the buffer
            printf("Type in Y-position  (y from 0 to %d): ", Y_LENGTH - 1);
        }

        if (Margin_Check(*x, *y)) {
            printf("The value is out of range, please type it again.\n");
        }
        else {
            break;
        }
    };
    system("cls");
}


int main() {
    MESSAGE_U *message_input = calloc(sizeof(MESSAGE_U), 1);
    message_input->bomb_num = BOMB_NUM;

    // UT report generation
    Unit_Test_Case();
    system("cls");
    // Game start - select game mode
    select_game_mode();
    do {

        // Generate map by game mode
        Construct_Map();
        print_game_message(MESSAGE_BOMB_LEFT, *message_input);
        
        // Cell will be shown if it is open
        print_map(false);
        play_operation();

        print_game_message(MESSAGE_REPLAY, *message_input);
        Reset_Map();
    } while (play_again_check());

    print_game_message(MESSAGE_THANKS, *message_input);
    View_End(0, Y_LENGTH + MESSAGE_NUM);
    return 0;
}

