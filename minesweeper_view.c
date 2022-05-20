#include <stdio.h>
#include <windows.h>

#include "minesweeper_view.h"
#include "minesweeper_control.h"

static const char Type_List[TYPE_NUM] = { ' ','1', '2','3', '4', '5', '6', '7', '8', 'X' };

// Set started position for print
void Set_Position(int x, int y) {
    HANDLE Screen;
    Screen = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position = { x, y };
    SetConsoleCursorPosition(Screen, Position);
}

// Print function for char
void Char_Print(int x, int y, char c) {
    Set_Position(x, y);
    printf("%c", c);
}

// Get the symbol of GUI by cell type
char Get_Cell_Symbol(TYPE_E  cell_type) {
    if (cell_type < TYPE_NUM && cell_type >= 0)
        return (Type_List[cell_type]);
    else
        return ' ';
}

// Print the symbol of GUI by cell type
char Print_Cell(STATE_E cell_state, TYPE_E  cell_type, bool show) {
    char c = '-';
    if (show) {
        c = Get_Cell_Symbol(cell_type);
    }
    else {
        switch (cell_state) {
        case STATE_BOMB_HIT: c = 'X'; break;
        case STATE_FLAGGED:  c = 'F'; break;
        case STATE_OPEN:     c = Get_Cell_Symbol(cell_type); break;
        default: break;
        }
    }
    return c;
}

// End function for GUI
void View_End(int x, int y) {
    Sleep(2500);
    Set_Position(x, y);
}

