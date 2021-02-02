#include "bmp_header.h"
#include "bmp_handlers.h"

/**
 * Checks if ROW respects sudoku rules
 * @sudoku_board: board saved as integer
 * @row: row number to be checked
 * 
 * Check if every digit from 0 to 9 is used on the row ONLY once. Iterate over
 * the row and for every digit found set the coresponding bit in a mask. After
 * testing every, digit if the mask isn't equal to 1022 (0b1111111110) then the
 * row isn't correct.
 * 
 * Return: 1 if the rules are respected and 0 otherwise
 */
int check_row(int **sudoku_board, int row);

/**
 * Checks if COL respects sudoku rules
 * @sudoku_board: board saved as integer
 * @col: row number to be checked
 * 
 * Check if every digit from 0 to 9 is used on the col ONLY once. 
 * Same idea as check_row.
 * 
 * Return: 1 if the rules are respected and 0 otherwise
 */
int check_col(int **sudoku_board, int col);

/**
 * Checks if the subsquare respects sudoku rules
 * @sudoku_board: board saved as integer
 * @row: row from the subsquare to be checked
 * @col: col from the subsquare to be checked
 * 
 * Determines the subsquare to be checked and then verifies that it is
 * correctly completed.
 * Same idea as check_row.
 * 
 * Return: 1 if the rules are respected and 0 otherwise
 */
int check_subsquare(int **sudoku_board, int row, int col);

/**
 * Checks if the entire board is corectly completed.
 * @sudoku_board: board saved as integer
 * 
 * Checks every row, column and subsquare for corectness.
 * Same idea as check_row.
 * 
 * Return: 1 if the rules are respected and 0 otherwise
 */
int check_correctness(int **sudoku_board);

/**
 * Determines which digit is missing on ROW
 * @sudoku_board: board saved as integer
 * @row: row number to be checked
 * 
 * Same idea as check_row.
 * 
 * Return: missing digit (0-9) or -1 if it cannot be determined
 */
int get_missing_value_on_row(int **sudoku_board, int row);

/**
 * Determines which digit is missing on COL
 * @sudoku_board: board saved as integer
 * @col: row number to be checked
 * 
 * Same idea as check_row.
 * 
 * Return: missing digit (0-9) or -1 if it cannot be determined
 */
int get_missing_value_on_col(int **sudoku_board, int col);

/**
 * Determines which digit is missing the subsquare.
 * @sudoku_board: board saved as integer
 * @row: row from the subsquare to be checked
 * @col: col from the subsquare to be checked
 * 
 * Determines the subsquare to be checked and then determines the missing digit.
 * Same idea as check_row.
 * 
 * Return: missing digit (0-9) or -1 if it cannot be determined
 */
int get_missing_value_in_square(int **sudoku_board, int row, int col);

/**
 * Completes the missing digits in the 
 * @sudoku_board: board saved as integer
 * @bmp_info: information regarding bmp format for the image
 * 
 * Naive solution to simple sudoku boards by checking each row, column and
 * for missing digits and completing the empty positions with the digits
 * found. If no digit can be determined from the "get_missing" functions the
 * position is skipped.
 * 
 * Return: 
 */
void half_solve_board(int **sudoku_board, bmp_file_t bmp_info);


/**
 * Checks if NUMBER can be place on ROW and COL and not violate sudoku rules.
 * @sudoku_board: board saved as integer
 * @row: row number
 * @col: col number
 * @number: number to place
 * 
 * Using "check_" functions determines if the position cand be filled with
 * said number.
 * 
 * Return: 1 if the number can be placed on that row and column or 0 otherwise
 */
int is_valid(int **sudoku_board, int row, int col, int number);

/**
 * Solve a sudoku board recursively
 * @sudoku_board: board saved as integer
 * @row: starting row
 * @col: starting column
 * 
 * Classic backtracking algorithm that fills a suoku board.
 * 
 * Return: 1 if the board is solved and 0 if it is UNSOLVABLE
 */
int solve_puzzle(int **sudoku_board, int row, int col);