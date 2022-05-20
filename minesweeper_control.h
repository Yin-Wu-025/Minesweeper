#ifndef CONTROL_HEADER_INCLUDED
#define CONTROL_HEADER_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {

	MESSAGE_BOMB_LEFT = 1,
	MESSAGE_GAME_RESULT,
	MESSAGE_REPLAY,
	MESSAGE_THANKS,
	MESSAGE_NUM,
}GAME_MESSAGE_E;

typedef enum {
	STATE_CLOSE,
	STATE_OPEN,
	STATE_FLAGGED,
	STATE_BOMB_HIT,
}STATE_E;

typedef enum {
	TYPE_EMPTY,
	TYPE_ONE,
	TYPE_TWO,
	TYPE_THREE,
	TYPE_FOUR,
	TYPE_FIVE,
	TYPE_SIX,
	TYPE_SEVEN,
	TYPE_EIGHT,
	TYPE_BOOM,
	TYPE_NUM,
}TYPE_E;

typedef struct {
	STATE_E cell_state;
	TYPE_E  cell_type;
}CELL_T;

typedef union {
	bool is_win;
	int  bomb_num;
	int  bomb_idx;
}MESSAGE_U;


bool Get_Is_Bomb_Random_Status(void);
void Set_Is_Bomb_Random_Status(bool is_true);
void Bomb_Position_By_Input(int* x, int* y, int bomb_idx);
bool Open_Cell(int x, int y);

#endif
