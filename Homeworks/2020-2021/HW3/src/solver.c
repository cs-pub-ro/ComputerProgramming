#include "solver.h"

int check_row(int **sudoku_board, int row)
{
	int i;
	unsigned int correct = 0b1111111110, mask = 0;
	
	for (i = 0; i < SUDOKU_SIZE; i++)
		if (sudoku_board[row][i] != -1)
			mask = mask | (1 << sudoku_board[row][i]);

	return mask == correct;
}

int check_col(int **sudoku_board, int col)
{
	int i;
	unsigned int correct = 0b1111111110, mask = 0;
	
	for (i = 0; i < SUDOKU_SIZE; i++)
		if (sudoku_board[i][col] != -1)
			mask = mask | (1 << sudoku_board[i][col]);

	return mask == correct;
}

int check_subsquare(int **sudoku_board, int row, int col)
{
	int i, j;
	unsigned int correct = 0b1111111110, mask = 0;
	
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if (sudoku_board[row + i][col + j] != -1)	
				mask = mask | (1 << sudoku_board[row + i][col + j]);

	return mask == correct;
}

int check_correctness(int **sudoku_board)
{
	int i, j;
	int result = 1;

	for (i = 0; i < SUDOKU_SIZE; i++)
		result &= check_row(sudoku_board, i) & check_col(sudoku_board, i);

	for (i = 0; i < SUDOKU_SIZE; i += 3)
		for (j = 0; j < SUDOKU_SIZE; j += 3)
			result &= check_subsquare(sudoku_board, i, j);
	
	return result;
}

int get_missing_value_on_row(int **sudoku_board, int row)
{
	int i, missing = 0, value = -1;
	unsigned int mask = 0;
	
	for (i = 0; i < SUDOKU_SIZE; i++)
		if (sudoku_board[row][i] == -1)
			missing++;
		else 
			mask = mask | (1 << sudoku_board[row][i]);
	
	if (missing != 1)
		return value;


	mask >>= 1;
	value = 1;
	while (mask & 1) {
		mask >>= 1;
		value++;
	}
	
	return value;
}

int get_missing_value_on_col(int **sudoku_board, int col)
{
	int i, missing = 0, value = -1;
	unsigned int mask = 0;
	
	for (i = 0; i < SUDOKU_SIZE; i++)
		if (sudoku_board[i][col] == -1) 
			missing++;
		else 
			mask = mask | (1 << sudoku_board[i][col]);
	
	if (missing != 1)
		return value;

	mask >>= 1;
	value = 1;
	while (mask & 1) {
		mask >>= 1;
		value++;
	}
	
	return value;
}

int get_missing_value_in_square(int **sudoku_board, int row, int col)
{
	int i, j;
	int missing = 0, value = -1;
	unsigned int mask = 0;
	int square_row = (row / 3) * 3;
	int square_col = (col / 3) * 3;
	
	for (i = square_row; i < square_row + 3; i++)
		for (j = square_col; j < square_col + 3; j++)
			if (sudoku_board[i][j] == -1)
				missing++;
			else 
				mask = mask | (1 << sudoku_board[i][j]);

	if (missing != 1)
		return value;

	mask >>= 1;
	value = 1;
	while (mask & 1) {
		mask >>= 1;
		value++;
	}

	return value;
}

void half_solve_board(int **sudoku_board, bmp_file_t bmp_info)
{
	int i, j;
	int row, col, square, solution;
	
	for (i = 0; i < SUDOKU_SIZE; i++)
		for (j = 0; j < SUDOKU_SIZE; j++)
			if (sudoku_board[i][j] == -1) {
				col = get_missing_value_on_row(sudoku_board, i);
				row = get_missing_value_on_col(sudoku_board, j);
				square = get_missing_value_in_square(sudoku_board, i, j);

				if (col != -1)
					solution = col;
				else if (row != -1)
					solution = row;
				else
					solution = square;

				sudoku_board[i][j] = solution;
				insert_digit(bmp_info, i * 8 + 2, j * 8 + 2, solution, (pixel_t) MAGENTA);
			}
}

int is_valid(int **sudoku_board, int row, int col, int number)
{
	int i;
	int square_row = (row / 3) * 3;
	int square_col = (col / 3) * 3;

	// Verific pe rowie
	for (i = 0; i < SUDOKU_SIZE; i++)
	{
		if (sudoku_board[row][i] == number) return 0;
        if (sudoku_board[i][col] == number) return 0;
        if (sudoku_board[square_row + (i % 3)][square_col + (i / 3)] == number) return 0;
	}

	return 1;
}

int solve_puzzle(int **sudoku_board, int row, int col)
{
	int next_row = row + (col + 1) / 9;
	int next_col = (col + 1) % 9;
	int number;

	if (sudoku_board[row][col] != -1)
		return (next_row >= 9) || solve_puzzle(sudoku_board, next_row, next_col);

	for (number = 1; number <= 9; number++)
		if (is_valid(sudoku_board, row, col, number))
		{
			sudoku_board[row][col] = number;

			if (next_row >= 9 || solve_puzzle(sudoku_board, next_row, next_col))
				return 1;

			sudoku_board[row][col] = -1;
		}
	
	return 0;
}