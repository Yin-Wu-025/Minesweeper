#ifndef VIEW_HEADER_INCLUDED
#define VIEW_HEADER_INCLUDED

#include "minesweeper_control.h"

// ASCII code mapping for operation
typedef enum {
    OPERATION_UP      = 80,  // up arrow
    OPERATION_DOWN    = 72,  // down arrow
    OPERATION_LEFT    = 75,  // left arrow
    OPERATION_RIGHT   = 77,  // right arrow
    OPERATION_OPEN    = 32,  // space
    OPERATION_FLAG_1  = 'f', // f
    OPERATION_FLAG_2  = 'F', // F
    OPERATION_LEAVE_1 = 'l', // l
    OPERATION_LEAVE_2 = 'L', // L
}PLAY_OPERATION_E;

void Set_Position(int x, int y);
void Char_Print(int x, int y, char c);
char Get_Cell_Symbol(TYPE_E  cell_type);
char Print_Cell(STATE_E cell_state, TYPE_E  cell_type, bool show);
void View_End(int x, int y);

#endif