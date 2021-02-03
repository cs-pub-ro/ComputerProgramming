#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "solver.h"
#include "bmp_handlers.h"

int main(int argc, char *argv[])
{
	int **sudoku_board = NULL;
	char *output_file_name = NULL;
	int status = 0;
	bmp_file_t bmp_info;

	// Default case	
	if (argc < 3) {
		fprintf(stderr, "Wrong number of arguments!\n");
		exit(-1);
	}
	
	// Parse arguments
	if (!strcmp(argv[2], "123")) {
		// Task 1
		bmp_info.bitmap = initialize_data(argv[1], &(bmp_info.fhdr), &(bmp_info.ihdr));
		output_file_name = change_file_name(argv[1], "bmp", 1);
		print_bmp(output_file_name, bmp_info);
		free(output_file_name);

		// Task 2
		correct_mirror(bmp_info);
		output_file_name = change_file_name(argv[1], "bmp", 2);
		print_bmp(output_file_name, bmp_info);
		free(output_file_name);

		// Task 3
		// Generez matricea de inturi din bmp
		sudoku_board = generate_integer_matrix(bmp_info);

		// Afisez datele in format json
		print_json(argv[1], check_correctness(sudoku_board));

		// Cleanup between tasks
		cleanup(bmp_info, sudoku_board);
	} else if (!strcmp(argv[2], "4")) {
		// Task 4
		// Citesc datele
		read_bmp(argv[1], &bmp_info);
		sudoku_board = generate_integer_matrix(bmp_info);

		// Rezolv golurile
		half_solve_board(sudoku_board, bmp_info);

		// Afisez plansa de joc
		output_file_name = change_file_name(argv[1], "bmp", 4);
		print_bmp(output_file_name, bmp_info);
		free(output_file_name);

		// Cleanup between tasks
		cleanup(bmp_info, sudoku_board);
	} else if (!strcmp(argv[2], "bonus")) {
		// Bonus
		read_bmp(argv[1], &bmp_info);
		sudoku_board = generate_integer_matrix(bmp_info);

		status = solve_puzzle(sudoku_board, 0, 0);
		rewrite_board(bmp_info, sudoku_board, status);
		
		output_file_name = change_file_name(argv[1], "bmp", 5);
		print_bmp(output_file_name, bmp_info);
		free(output_file_name);
		cleanup(bmp_info, sudoku_board);
	}

	return 0;
}