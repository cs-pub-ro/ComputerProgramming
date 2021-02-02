#ifndef UTIL_H
#define UTIL_H

#include "cJSON.h"
#include "bmp_header.h"

/**
 * Stores in a predefined format the data parsed from the json
 * @input_file: the name of the input file
 * @fhdr: structure in which the file header of a bmp file is stored
 * @ihdr: structure in which the info header of a bmp file is stored
 *
 * Return: Matrix of type pixel_t, that stores the bitmap
 */
pixel_t **initialize_data(const char *input_file, bmp_fileheader *fhdr, bmp_infoheader *ihdr);

/**
 * Reads the json data from INPUT_FILE 
 * @input_file: the name of the input file
 *
 * Return: (cJSON *) which is the entire json read from INPUT_FILE
 */
cJSON *read_json(const char *input_file);

/**
 * Swaps the value of two pixels
 * @a: first pixel to be swapped
 * @b: second pixel to be swapped
 * 
 * Return:
 */
void swap(pixel_t *a, pixel_t *b);

/**
 * Writes the GAME_STATE in a file with the name parsed from INPUT_FILE
 * @input_file: the name of the input file
 * @game_state: state of the game "1" for "win" and "0" for loss
 * 
 * Changes the file name using "change_file_name", and generates a json with
 * the state of the game and the file_name. The json is written to a file with
 * the new name.
 * 
 * Return:
 */
void print_json(char *input_file, int game_state);

/**
 * Deallocates dynamically allocate memory from the bitmap
 * @bmp_info: information regarding the bmp file header, info header and bitmap
 * @sudoku_board: integer matrix with the sudoku board
 * 
 * Deallocate the bitmap saved both as a pixel matrix and as an integer matrix
 * 
 * Return:
 */
void cleanup(bmp_file_t bmp_info, int **sudoku_board);

/**
 * Change the name of the FILE, by adding the SUFFIX and TASK number 
 * @file: initial file name
 * @suffix: file extension
 * @task: task number 
 * 
 * Return: file name following this name scheme "<file_name>_task<task>.<suffix>"
 */
char *change_file_name(char *file, char suffix[5], int task);

/**
 * Prints the sudoku board to the console
 * @sudoku_board: board saved as integer
 * 
 * Return:
 */
void print_sudoku_readable(int **sudoku_board);

#endif