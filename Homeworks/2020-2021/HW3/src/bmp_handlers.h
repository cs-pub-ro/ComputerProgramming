#include "bmp_header.h"

/**
 * Mirrors each column.
 * @bmp_info: information regarding the bmp file header, info header and bitmap
 * 
 * Iterate over each column (on the middle of the cell) individually and swap
 * bitmap[i][j-1] with bitmap[i][j+1], and bitmap[i][j-2] with bitmap[i][j+2]
 * (i index for line and j index for column).
 * 
 * Return:
 */
void correct_mirror(bmp_file_t bmp_info);

/**
 * Identify a digit from the bitmap on [ROW][COL]
 * @bmp_info: information regarding the bmp file header, info header and bitmap
 * @row: row where the digit is located
 * @col: col where the digit is located
 * 
 * Using a bit mask iterate over a 5x5 matrix and for each set pixel set a bit
 * on said mask. Identify the digit by matching the mask with "digit_masks"
 * 
 * Return: the identified digit or -1 otherwise
 */
unsigned int identify_digit(bmp_file_t bmp_info, int row, int col);

/**
 * Generate an integer matrix from the bitmap
 * @bmp_info: information regarding the bmp file header, info header and bitmap
 * 
 * Iterate over the bitmap and using "identify_digit" save every digit in an
 * integer matrix.
 * 
 * Return: an integer matrix with every identified digit.
 */
int **generate_integer_matrix(bmp_file_t bmp_info);

/**
 * Modifies BMP_INFO bitmap to match the SUDOKU_BOARD
 * @bmp_info: board saved in bmp format
 * @sudoku_board: board saved as integer
 * @status: 1 if the boards need to be rewritten or 0 if the board needs to be
 * completed with "X"
 * 
 * If status is 1 then the whole board is completed with Xs, otherwise iterate
 * over SUDOKU_BOARD: checks if the equivalent place in the bitmap has a digit
 * and skips it or writes the digit to match SUDOKU_BOARD.
 * The newly written digits are written with magenta.
 * 
 * Return:
 */
void rewrite_board(bmp_file_t bmp_info, int **sudoku_board, int status);

/**
 * Checks if a pixel is white or not
 * @p: pixle to check
 * 
 * Return: 1 if the pixel is set or 0 otherwise
 */
int is_set(pixel_t p);

/**
 * Inserts a DIGIT in the bitmap at a given position (ROW,COL) with COLOR
 * @bmp_info: board saved in bmp format
 * @row: row where to insert the digit
 * @col: col where to insert the digit
 * @digit: digit to be inserted
 * @color: color of the digit
 * 
 * Return:
 */
void insert_digit(bmp_file_t bmp_info, int row, int col, int digit, pixel_t color);

/**
 * Reads a bmp image from a file and saves it into BMP_INFO
 * @input_file: input file name
 * @bmp_info: board saved in bmp format
 * 
 * Return:
 */
void read_bmp(const char *input_file, bmp_file_t *bmp_info);

/**
 * Writes a bmp image in the FILE_NAME.
 * @file_name: output file name
 * @bmp_info: board saved in bmp format
 * 
 * Return:
 */
void print_bmp(char *file_name, bmp_file_t bmp_info);