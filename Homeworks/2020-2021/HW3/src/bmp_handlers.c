#ifndef BMP_HANDLERS_H
#define BMP_HANDLERS_H

#include "bmp_handlers.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void correct_mirror(bmp_file_t bmp_info)
{
	int i, j;

	for (i = 2; i < bmp_info.ihdr.height - 2; i++)
		for (j = 4; j < bmp_info.ihdr.width - 3; j += 8) {
			swap(&bmp_info.bitmap[i][j - 1], &bmp_info.bitmap[i][j + 1]);
			swap(&bmp_info.bitmap[i][j - 2], &bmp_info.bitmap[i][j + 2]);
		}
} 

unsigned int identify_digit(bmp_file_t bmp_info, int row, int col)
{
	int i, j;
	unsigned int digit = 0;
	pixel_t p;

	for (i = 0; i < 5; i++) {
		for (j = 0; j < 5; j++) {
			p = bmp_info.bitmap[row + i][col + j];
			digit = digit | (is_set(p) << (i * 5 + j));
		}
	}

	switch (digit)
	{
	case one:
		return 1;
		break;

	case two:
		return 2;
		break;

	case three:
		return 3;
		break;

	case four:
		return 4;
		break;

	case five:
		return 5;
		break;

	case six:
		return 6;
		break;

	case seven:
		return 7;
		break;

	case eight:
		return 8;
		break;

	case nine:
		return 9;
		break;

	default:
		return -1;
		break;
	}
}

int **generate_integer_matrix(bmp_file_t bmp_info)
{
	int i, j;
	int **sudoku_board = NULL;

	// Aloc memorie pentru matricea de cifre
	sudoku_board = malloc(sizeof(*sudoku_board) * SUDOKU_SIZE);
	for (i = 0; i < SUDOKU_SIZE; i++)
		sudoku_board[i] = calloc(SUDOKU_SIZE, sizeof(*(sudoku_board[i])));

	for (i = 2; i < bmp_info.ihdr.height - 6; i += 8)
		for (j = 2; j < bmp_info.ihdr.width - 6; j += 8)
			sudoku_board[i / 8][j / 8] = identify_digit(bmp_info, i, j);

	return sudoku_board;
}

void rewrite_board(bmp_file_t bmp_info, int **sudoku_board, int status)
{
	int i, j;

	if (status) {
		for(i = 0; i < SUDOKU_SIZE; i++)
			for (j = 0; j < SUDOKU_SIZE; j++)
				if (!is_set(bmp_info.bitmap[i * 8 + 2][j * 8 + 6]))
					insert_digit(bmp_info, i * 8 + 2, j * 8 + 2, sudoku_board[i][j], (pixel_t) MAGENTA);
					
	} else {
		for(i = 0; i < SUDOKU_SIZE; i++)
			for (j = 0; j < SUDOKU_SIZE; j++)
				insert_digit(bmp_info, i * 8 + 2, j * 8 + 2, -1, (pixel_t) RED);
	}
}

int is_set(pixel_t p)
{
	return (p.blue != 255) || (p.green != 255) || (p.red != 255);
}

void insert_digit(bmp_file_t bmp_info, int row, int col, int digit, pixel_t color)
{
	int i, j;
	unsigned int mask = 0;

	switch (digit)
	{
	case 1:
		mask = one;
		break;

	case 2:
		mask = two;
		break;

	case 3:
		mask = three;
		break;

	case 4:
		mask = four;
		break;

	case 5:
		mask = five;
		break;

	case 6:
		mask = six;
		break;

	case 7:
		mask = seven;
		break;

	case 8:
		mask = eight;
		break;

	case 9:
		mask = nine;
		break;

	default:
		mask = wrong;
		break;
	}

	for (i = 0; i <= 4; i++) {
		for (j = 0; j <= 4; j++) {
			if (mask & 1)
				bmp_info.bitmap[row + i][col + j] = color;
			else
				bmp_info.bitmap[row + i][col + j] = (pixel_t) WHITE;
			
			mask >>= 1;
		}
	}
}

void read_bmp(const char *input_file, bmp_file_t *bmp_info)
{
	FILE *bmp_in = NULL;
	int i, padding;
	bmp_in = fopen(input_file, "rb");

	if (bmp_in == NULL)
	{
		fprintf(stderr, "ERROR: Can't open file \n");
		exit(-1);
	}

	fread(&bmp_info->fhdr, sizeof(bmp_fileheader), 1, bmp_in);
	fread(&bmp_info->ihdr, sizeof(bmp_infoheader), 1, bmp_in);

	// Aloc memorie pentru matricea de pixeli
	bmp_info->bitmap = malloc(sizeof(*(bmp_info->bitmap)) * bmp_info->ihdr.height);
	for (i = 0; i < bmp_info->ihdr.height; i++)
		bmp_info->bitmap[i] = calloc(bmp_info->ihdr.width, sizeof(*(bmp_info->bitmap[i])));

	padding = 0;
	while((bmp_info->ihdr.width * 3 + padding) % 4 != 0)
		padding++;

	for(i = bmp_info->ihdr.height - 1; i >= 0; i--)
	{
		fread(bmp_info->bitmap[i], sizeof(pixel_t), bmp_info->ihdr.width, bmp_in);
		fseek(bmp_in, padding, SEEK_CUR);
	}

	fclose(bmp_in);
}

void print_bmp(char *file_name, bmp_file_t bmp_info)
{
	FILE *output = NULL;
	int i;
	BYTE padding_value = 0;

	// Deschidem fisierul pentru scriere in mod ibnar (bmp)
	output = fopen(file_name,"wb");

	// Scriem headerele
	fwrite(&bmp_info.fhdr, sizeof(bmp_info.fhdr), 1, output);
	fwrite(&bmp_info.ihdr, sizeof(bmp_info.ihdr), 1, output);

	// Calculam dimensiunea paddingul
	int padding = 0;
	while((bmp_info.ihdr.width * 3 + padding) % 4 != 0)
		padding++;

	// Afisez imaginea invers (datorita formatului bmp)
	for(i = bmp_info.ihdr.height - 1; i >= 0; i--)
	{
		// Scriu linia de pixeli
		fwrite(bmp_info.bitmap[i], sizeof(pixel_t), bmp_info.ihdr.width, output);

		// Scriu paddingul
		fwrite(&padding_value, sizeof(BYTE), padding, output);
	}

	// Inchid fisierul
	fflush(output);
	fclose(output);
}

#endif