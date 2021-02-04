#include "utils.h"

#define INPUT_LEN 1000
#define FILE_LEN 1000

// RotateMatrix
void CheckTask1(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task1.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	int inputTask1;
	char *aux = calloc(INPUT_LEN, sizeof(char));

	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);

	fscanf(inputFile, "%d\n", &inputTask1);
	char *outputTask1 = RotateMatrix((void *)&inputTask1);
	fprintf(taskOutputFile, "%s", outputTask1);

	free(taskOutputFileName);
	free(aux);
	free(outputTask1);
	fclose(taskOutputFile);
}

// DecodeString
void CheckTask2(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task2.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	char *inputTask2 = calloc(INPUT_LEN, sizeof(char));
	char *aux = calloc(INPUT_LEN, sizeof(char));

	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);

	fscanf(inputFile, "%s\n", inputTask2);
	char *outputTask2 = DecodeString((void *)inputTask2);
	fprintf(taskOutputFile, "%s", outputTask2);

	free(taskOutputFileName);
	free(inputTask2);
	free(outputTask2);
	free(aux);
	fclose(taskOutputFile);
}

// InvertArray
void CheckTask3(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task3.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	int n;
	char *aux = calloc(INPUT_LEN, sizeof(char));

	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);

	fscanf(inputFile, "%d", &n);
	int *inputTask3 = calloc(n + 1, sizeof(int));

	inputTask3[0] = n;
	for(int i = 1; i <= n; i++)
		fscanf(inputFile, "%d\n", &inputTask3[i]);

	char *outputTask3 = InvertArray((void *)inputTask3);
	fprintf(taskOutputFile, "%s", outputTask3);

	free(taskOutputFileName);
	free(inputTask3);
	free(outputTask3);
	free(aux);
	fclose(taskOutputFile);
}

// allocPlayer allocGame
void CheckTask4(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task4.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	char *aux = calloc(INPUT_LEN, sizeof(char));
	
	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);

	Game *game = allocGame();
	Player *player = allocPlayer();
	fscanf(inputFile, "%s\n", game->name);
	fscanf(inputFile, "%s\n", player->name);
	fscanf(inputFile, "%s\n", player->color);
	fscanf(inputFile, "%s\n", player->hat);

	fprintf(taskOutputFile, "%s\n", game->name);
	fprintf(taskOutputFile, "%s\n", player->name);
	fprintf(taskOutputFile, "%s\n", player->color);
	fprintf(taskOutputFile, "%s", player->hat);

	free(game->name);
	free(game);
	free(player->name);
	free(player->color);
	free(player->hat);
	free(player);

	free(aux);
	free(taskOutputFileName);
	fclose(taskOutputFile);
}

// ReadPlayer ReadGame
void CheckTask5(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task5.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	char *aux = calloc(INPUT_LEN, sizeof(char));
	
	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);

	Game *game = ReadGame(inputFile);

	fprintf(taskOutputFile, "%s\n", game->name);
	fprintf(taskOutputFile, "%d\n", game->killRange);
	fprintf(taskOutputFile, "%d\n", game->numberOfCrewmates);

	char *emunToStr;
	Player *player;
	for(int i = 0; i < game->numberOfCrewmates; i++)
	{
		player = game->crewmates[i];

		fprintf(taskOutputFile, "%s\n", player->name);
		fprintf(taskOutputFile, "%s\n", player->color);
		fprintf(taskOutputFile, "%s\n", player->hat);

		fprintf(taskOutputFile, "(%d,%d)\n", player->locations[player->numberOfLocations - 1].x,
		 player->locations[player->numberOfLocations - 1].y);

		emunToStr = fromEnumtoString(player->playerRole);
		fprintf(taskOutputFile, "%s\n", emunToStr);
		free(emunToStr);
	}
	
	FreeGame(game);
	free(aux);
	free(taskOutputFileName);
	fclose(taskOutputFile);
}

// WritePlayer WriteGame
void CheckTask6(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task6.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	char *aux = calloc(INPUT_LEN, sizeof(char));
	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);

	Game *game = ReadGame(inputFile);
	WriteGame(game, taskOutputFile);

	FreeGame(game);
	free(aux);
	free(taskOutputFileName);
	fclose(taskOutputFile);
}

// KillPlayer
void CheckTask7(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task7.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	char *aux = calloc(INPUT_LEN, sizeof(char));
	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);
	
	Game *game = ReadGame(inputFile);
	char *output = (char *)KillPlayer((void *)game);
	fprintf(taskOutputFile, "%s", output);
	free(output);

	FreeGame(game);
	free(aux);
	free(taskOutputFileName);
	fclose(taskOutputFile);
}

// CalcuateNextCycleOfGame
void CheckTask8(FILE *inputFile, char *outputFileName)
{
	char *taskOutputFileName = calloc(FILE_LEN, sizeof(char));
	sprintf(taskOutputFileName, "%s-task8.out", outputFileName);
	FILE *taskOutputFile = fopen(taskOutputFileName, "wt");

	char *aux = calloc(INPUT_LEN, sizeof(char));
	fscanf(inputFile, "%s\n", aux);
	fscanf(inputFile, "%s\n", aux);

	Game *game = ReadGame(inputFile);

	int numberOfCicles = 0;
	fscanf(inputFile, "%d\n", &numberOfCicles);

	void **inputMatrix;
	Player *player;

	inputMatrix = calloc(game->numberOfCrewmates + 1, sizeof(void *));

	for(int i = 0; i < numberOfCicles; i++)
	{
		fprintf(taskOutputFile, "Cycle %d of game!\n", i + 1);
		for(int j = 0; j < game->numberOfCrewmates; j++)
		{
			player = game->crewmates[j];

			if(player->playerRole == Rotator)
			{
				int *n = calloc(1, sizeof(int));
				fscanf(inputFile, "%d\n", n);
				inputMatrix[j] = (void*)n;
			}
			else if(player->playerRole == Decoder)
			{
				char *str = calloc(INPUT_LEN, sizeof(char));
				fscanf(inputFile, "%s\n", str);
				inputMatrix[j] = (void *)str;
			}
			else if(player->playerRole == Invertor)
			{
				int n;
				fscanf(inputFile, "%d", &n);
				int *vec = calloc(n + 1, sizeof(int));
				vec[0] = n;
				for(int k = 1; k <= n; k++)
					fscanf(inputFile, "%d", &vec[k]);
				fgets(aux, INPUT_LEN, inputFile);
				inputMatrix[j] = (void *)vec;
			}
		}
		inputMatrix[game->numberOfCrewmates] = (void *)game;
		CalcuateNextCycleOfGame(game, taskOutputFile, inputMatrix);
		for(int k = 0; k < game->numberOfCrewmates; k++)
			free(inputMatrix[k]);
		fprintf(taskOutputFile, "\n");
	}
	free(inputMatrix);

	FreeGame(game);
	free(aux);
	free(taskOutputFileName);
	fclose(taskOutputFile);
} 

int main(int argc, char *argv[])
{
	if(argc != 3)
		return -1;

	char *inputFileName = strdup(argv[1]);
	char *outputFileName = strdup(argv[2]);

	FILE *inputFile = fopen(inputFileName, "rt");

	CheckTask1(inputFile, outputFileName);
	CheckTask2(inputFile, outputFileName);
	CheckTask3(inputFile, outputFileName);
	CheckTask4(inputFile, outputFileName);
	CheckTask5(inputFile, outputFileName);
	CheckTask6(inputFile, outputFileName);
	CheckTask7(inputFile, outputFileName);
	CheckTask8(inputFile, outputFileName);

	free(inputFileName);
	free(outputFileName);
	fclose(inputFile);
	return 0;
}