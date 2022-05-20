#ifndef MODEL_HEADER_INCLUDED
#define MODEL_HEADER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "minesweeper_control.h"

#define SQUARE_LENGTH (8)
#define X_LENGTH      (SQUARE_LENGTH)
#define Y_LENGTH      (SQUARE_LENGTH)
#define BOMB_NUM      (10)
#define CELL_NUM      (X_LENGTH * Y_LENGTH)
#define NEIGHBOR_NUM  (8)


typedef enum {
	DIMENSION_X,
	DIMENSION_Y,
	DIMENSION_NUM,
}POSITION_DIMENSION_E;


CELL_T* Get_Cell(int x, int y);
bool Margin_Check(int x, int y);
void Construct_Map();
void Reset_Map();
void Open_Neighbor_Cells(int x, int y);
bool Flag_Cell(int x, int y, int* flag_num, int* bombs_flag_num, int bombs);
const int* Get_Neighbor_Position(int neighbor_idx);

#endif

