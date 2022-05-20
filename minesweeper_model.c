#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "minesweeper_model.h"
#include "minesweeper_control.h"

static const int Neighbor_Position[NEIGHBOR_NUM][DIMENSION_NUM] = { {1, 0}, {0, 1},  {-1, 0}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
static CELL_T* map;

// bomb-position generation (by random variable or input)
static void generate_bomb_postion() {
    for (int i = 0; i < BOMB_NUM;) {
        int y, x;

        // Bomb position by random variable
        if (Get_Is_Bomb_Random_Status()) {
            y = rand() % Y_LENGTH;
            x = rand() % X_LENGTH;
        }
        else { // Bomb position by input
            Bomb_Position_By_Input(&x, &y, i);
        }

        CELL_T* cell = Get_Cell(x, y);
        if (cell->cell_type != TYPE_BOOM) {
            cell->cell_type = TYPE_BOOM;
            i++;
        }
    }
}

// After bomb-position generation, set the coresponding cell type
static void cell_type_assign(int x, int y) {
    int n = 0;
    CELL_T* cell = Get_Cell(x, y);

    // use Neighbor_Position to get (x,y) of neighbors
    for (int i = 0; i < NEIGHBOR_NUM; i++) {
        int neighbor_x = x + Neighbor_Position[i][DIMENSION_X];
        int neighbor_y = y + Neighbor_Position[i][DIMENSION_Y];

        if (!Margin_Check(neighbor_x, neighbor_y)) {
            if (Get_Cell(neighbor_x, neighbor_y)->cell_type == TYPE_BOOM) n++;
        }
    }

    cell->cell_type = (TYPE_E)n;
}

// Get the pointer of Neighbor_Position (position diff from observed cell)
const int *Get_Neighbor_Position(int neighbor_idx) {
    if (neighbor_idx > NEIGHBOR_NUM || neighbor_idx < 0) 
        return (Neighbor_Position[0]); // always return 1st element for error handle
    
    return (Neighbor_Position[neighbor_idx]);
}

// Get the pointer of the observed cell by (x,y)
CELL_T* Get_Cell(int x, int y) {
    return &map[x + y * X_LENGTH];
}

// x,y should be valid in the vaild range of game frame
bool Margin_Check(int x, int y) {
    return (y < 0 || y >= Y_LENGTH || x < 0 || x >= X_LENGTH);
}

// Open Neighbor cells for empty cell
void Open_Neighbor_Cells(int x, int y) {
    if (Get_Cell(x, y)->cell_type != TYPE_EMPTY) return;

    for (int i = 0; i < NEIGHBOR_NUM; i++) {
        const int* neighbor_cell = Neighbor_Position[i];
        Open_Cell(x + neighbor_cell[DIMENSION_X], y + neighbor_cell[DIMENSION_Y]);
    }

}

/* Play Operation - Set cell as "FLAG":
*       1. State change:
*           a. close -> flag
*           b. flag -> closed (disbled flag)
*           c. open -> do nothing
*       2. Calculate number of bomb flag 
*/ 
bool Flag_Cell(int x, int y, int* flag_num, int* bombs_flag_num, int bombs) {

    bool is_vaild_flag = false;

    if (!Margin_Check(x, y)) {
        CELL_T* cell = Get_Cell(x, y);
        is_vaild_flag = true;
        if (cell->cell_state == STATE_CLOSE) {
            cell->cell_state = STATE_FLAGGED;
            (*flag_num)++;
            if (cell->cell_type == TYPE_BOOM) (*bombs_flag_num)++;
        }
        else if (cell->cell_state == STATE_FLAGGED) {
            cell->cell_state = STATE_CLOSE;
            (*flag_num)--;
            if (cell->cell_type == TYPE_BOOM) (*bombs_flag_num)--;
        }
    }
    return is_vaild_flag;
}

/* Init flow - Contruct map :
*       1. Init all cells as empty
*       2. Set bomb position
*       3. Set cell state/ type for all cells
*/ 
void Construct_Map() {

    map = (CELL_T*)malloc(sizeof(CELL_T) * (CELL_NUM));

    // 1. Init all cells as empty
    for (int i = 0; i < CELL_NUM; i++) {
        map[i].cell_type = TYPE_EMPTY;
    }

    // 2. Set bomb position
    generate_bomb_postion();

    // 3. Set cell state/ type for all cells
    for (int y = 0; y < Y_LENGTH; y++) {
        for (int x = 0; x < X_LENGTH; x++) {
            Get_Cell(x, y)->cell_state = STATE_CLOSE;
            if (Get_Cell(x, y)->cell_type != TYPE_BOOM) {
                cell_type_assign(x, y);
            }
        }
    }
}

// Release map after finishing game
void Reset_Map() {
    free(map);
    map = NULL;
}


