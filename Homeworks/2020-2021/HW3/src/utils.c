#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

pixel_t **initialize_data(const char *input_file, bmp_fileheader *fhdr, bmp_infoheader *ihdr) {
	int i, l, c;
	cJSON *json = NULL, *f_head = NULL, *i_head = NULL, *byte = NULL, *bm = NULL;
	pixel_t **bitmap = NULL;

	// Citesc jsonul
	json = read_json(input_file);

	f_head = cJSON_GetObjectItemCaseSensitive(json, "file_header");
	i_head = cJSON_GetObjectItemCaseSensitive(json, "info_header");

	// Populez headerele cu informatiile primite in json
	fhdr->fileMarker1 = 	cJSON_GetObjectItemCaseSensitive(f_head, "signature")->valuestring[0];
	fhdr->fileMarker2 = 	cJSON_GetObjectItemCaseSensitive(f_head, "signature")->valuestring[1];
	fhdr->imageDataOffset = cJSON_GetObjectItemCaseSensitive(f_head, "offset")->valueint;
	fhdr->unused1 =			cJSON_GetObjectItemCaseSensitive(f_head, "reserved")->valueint;
	fhdr->unused2 = 		cJSON_GetObjectItemCaseSensitive(f_head, "reserved")->valueint;
	fhdr->bfSize = 			cJSON_GetObjectItemCaseSensitive(f_head, "file_size")->valueint;

	ihdr->biClrUsed = 		cJSON_GetObjectItemCaseSensitive(i_head, "colors_used")->valueint;
	ihdr->biSize = 			cJSON_GetObjectItemCaseSensitive(i_head, "size")->valueint;
	ihdr->biClrImportant = 	cJSON_GetObjectItemCaseSensitive(i_head, "colors_important")->valueint;
	ihdr->biSizeImage = 	cJSON_GetObjectItemCaseSensitive(i_head, "image_size")->valueint;
	ihdr->biYPelsPerMeter = cJSON_GetObjectItemCaseSensitive(i_head, "y_pixels_per_meter")->valueint;
	ihdr->biXPelsPerMeter = cJSON_GetObjectItemCaseSensitive(i_head, "x_pixels_per_meter")->valueint;
	ihdr->width = 			cJSON_GetObjectItemCaseSensitive(i_head, "width")->valueint;
	ihdr->planes = 			cJSON_GetObjectItemCaseSensitive(i_head, "planes")->valueint;
	ihdr->bitPix = 			cJSON_GetObjectItemCaseSensitive(i_head, "bit_count")->valueint;
	ihdr->biCompression = 	cJSON_GetObjectItemCaseSensitive(i_head, "compression")->valueint;
	ihdr->height = 			cJSON_GetObjectItemCaseSensitive(i_head, "height")->valueint;

	// Aloc memorie pentru matricea de pixeli
	bitmap = malloc(sizeof(*bitmap) * ihdr->height);
	for (i = 0; i < ihdr->height; i++)
		bitmap[i] = calloc(ihdr->width, sizeof(*(bitmap[i])));

	// Populez bitmapul
	l = 0; c = 0; i = 0;
	bm = cJSON_GetObjectItemCaseSensitive(json, "bitmap");
	cJSON_ArrayForEach(byte, bm)
	{
		if (i % 3 == 0)
			bitmap[l][c].blue = byte->valueint;
		else if (i % 3 == 1)
			bitmap[l][c].green = byte->valueint;
		else if (i % 3 == 2) {
			bitmap[l][c].red = byte->valueint;
			c++;
		}

		i++;
		if (c > 0 && c % ihdr->width == 0) {
			l++;
			c = 0;
		}
	}
	
	// Sterg jsonul
	cJSON_Delete(json);

	return bitmap;
}

cJSON *read_json(const char *input_file)
{
	long json_size;
	FILE *json_in = NULL;
	char *buffer = NULL;
	cJSON *json = NULL;

	// Deschidem fisierul de intrare
	json_in = fopen(input_file, "rb");

	// Setam cursorul la final de fisier
	fseek(json_in, 0, SEEK_END);

	// Salvam dimensiunea fisierului
	json_size = ftell(json_in);

	// Setam cursorul la inceput de fisier pentru citire
	fseek(json_in, 0, SEEK_SET);

	// Citirea efectiva
	buffer = (char *)calloc(sizeof(*buffer), json_size + 1);
	fread(buffer, 1, json_size, json_in);

	// Inchidem fisierul
	fclose(json_in);

	// Parsam jsonul
	json = cJSON_Parse(buffer);

	// Eliberam memoria alocata pentru buffer
	free(buffer);

	return json;
}

void swap(pixel_t *a, pixel_t *b)
{
	pixel_t aux = *a;
	*a = *b;
	*b = aux;
}

void print_json(char *input_file, int game_state)
{
	FILE *output = NULL;
	cJSON *monitor = NULL, *name = NULL, *status = NULL;
	char *file_name = NULL;
	char *buffer = NULL;

	monitor = cJSON_CreateObject();
	name = cJSON_CreateString(input_file + 14);

	if (game_state)
		status = cJSON_CreateString("Win!");
	else
		status = cJSON_CreateString("Loss :(");
	
	cJSON_AddItemToObject(monitor, "input_file", name);
	cJSON_AddItemToObject(monitor, "game_state", status);

	file_name = change_file_name(input_file, "json", 3);
	output = fopen(file_name, "wt");

	buffer = cJSON_Print(monitor);
	fprintf(output, "%s\n", buffer);

	free(buffer);
	free(file_name);
	cJSON_Delete(monitor);
	fclose(output);
}

void cleanup(bmp_file_t bmp_info, int **sudoku_board)
{
	int i;

	for (i = 0; i < bmp_info.ihdr.height; i++)
		free(bmp_info.bitmap[i]);
	free(bmp_info.bitmap);

	for (i = 0; i < SUDOKU_SIZE; i++)
		free(sudoku_board[i]);
	free(sudoku_board);
}

char *change_file_name(char *file, char suffix[5], int task)
{
	char *name = calloc(100, 1);
	char *aux = strrchr(file, '.');

	*aux = '\0';
	
	if (task != 5)
		sprintf(name, "output_task%d_%s.%s", task, strrchr(file, '/') + 1, suffix);
	else
		sprintf(name, "output_bonus_%s.%s", strrchr(file, '/') + 1, suffix);

	*aux = '.';

    return name;
}

void print_sudoku_readable(int **sudoku_board)
{
	int i, j;
	for(i = 0; i < SUDOKU_SIZE; i++)
	{
		for (j = 0; j < SUDOKU_SIZE; j++)
		{
			if (sudoku_board[i][j] == -1)
				printf("0 ");
			else 
				printf("%d ", sudoku_board[i][j]);
		}
		printf("\n");
	}
}
