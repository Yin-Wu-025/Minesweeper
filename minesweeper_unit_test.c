#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "minesweeper_view.h"
#include "minesweeper_control.h"
#include "minesweeper_model.h"

#define CASE_NUM (10000)
FILE* UT_Report;
FILE* UT_Reference;

/* Check cell status - 
*		1. neighbors of BOMB cell can not be empty
*		2. neighbors of empty cell can not be BOMB
*		3. number of bomb neighbors should be aglined by the type of cell
*/
static bool check_cell_status(int x, int y) {
	int bomb_num = 0;
	bool is_neighbor_empty = false;
	bool is_pass = true;
	CELL_T* current_cell = Get_Cell(x,y);

	for (int j = 0; j < NEIGHBOR_NUM; j++)
	{
		const int *neighbor_position = Get_Neighbor_Position(j);
		int neighbor_x = x + neighbor_position[DIMENSION_X];
		int neighbor_y = y + neighbor_position[DIMENSION_Y];

		if (!Margin_Check(neighbor_x, neighbor_y)) {
			CELL_T* neighbor_cell = Get_Cell(neighbor_x, neighbor_y);

			if (neighbor_cell->cell_type == TYPE_EMPTY) is_neighbor_empty = true;
			if (neighbor_cell->cell_type == TYPE_BOOM) bomb_num++;
		}
	}

	if (current_cell->cell_type == TYPE_BOOM) {
		if (is_neighbor_empty == true) {
			fprintf(UT_Report, "Bomb Cell(%d,%d) is with a empty neighbor cell\n", x, y);
			is_pass = false;
		}
	}
	else if (current_cell->cell_type == TYPE_EMPTY) {
		if (bomb_num != 0) {
			fprintf(UT_Report, "EMPTY Cell(%d,%d) is with a Bomb neighbor cell\n", x, y);
			is_pass = false;
		}
	}
	else {
		if (bomb_num != (int)current_cell->cell_type) {
			fprintf(UT_Report, "A [%d] Cell(%d,%d) is with wrong bomb number (%d)\n", (int)current_cell->cell_type, x, y, bomb_num);
			is_pass = false;
		}
	}
	return is_pass;

}

void Unit_Test_Case() {

	errno_t err;
	int fail_case = 0;
	float successful_rate = 0;
	err = fopen_s(&UT_Report   , "unit_test_result.txt", "w+");
	err = fopen_s(&UT_Reference, "unit_test_reference.txt", "w+");

	fprintf(UT_Report, "---- Test Case Result ----\n");
	fprintf(UT_Reference, "---- Map Reference ----\n");
	for (int i = 0; i < CASE_NUM;i++) {
		int failed_cells = 0;
		
		fprintf(UT_Report, "UT case-%d\t", i);
		fprintf(UT_Reference, "UT case-%d\n", i);
		Construct_Map();

		for (int y = 0; y < Y_LENGTH; y++) {
			for (int x = 0; x < X_LENGTH; x++) {
				CELL_T* cell = Get_Cell(x, y);

				if (!check_cell_status(y, x)) failed_cells++;

				if (cell->cell_type == TYPE_BOOM) {
					fprintf(UT_Reference, "X ");
				}
				else {
					fprintf(UT_Reference, "%d ", cell->cell_type);
				}
			}
			fprintf(UT_Reference, "\n");
		}

		if (failed_cells != 0) {
			fail_case++;
			fprintf(UT_Report, "[Fail] failed cells num = %d\n", failed_cells);
		}
		else {
			fprintf(UT_Report, "[PASS] \n");
		}

		fprintf(UT_Reference, "\n");
	}

	successful_rate = (float)(CASE_NUM - fail_case) * 100 / CASE_NUM;
	fprintf(UT_Report, "=============================================================== \n");
	fprintf(UT_Report, "Total case: %d, Failed Case: %d, Successful Rate = %f (%%)\n", CASE_NUM, fail_case, successful_rate);


	fclose(UT_Report);
	fclose(UT_Reference);
	
}