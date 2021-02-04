#include "utils.h"

void* (* Abilities[4])(void* x) = {RotateMatrix, DecodeString, InvertArray, KillPlayer};

char *fromEnumtoString(PlayerRoles playerRole)
{
	char *str = calloc(MAX_LEN_STR_ATR, sizeof(char));
	switch(playerRole)
	{
		case Rotator:
			str = strcpy(str, "Rotator");
			break;
		case Decoder:
			str = strcpy(str, "Decoder");
			break;
		case Invertor:
			str = strcpy(str, "Invertor");
			break;
		case Impostor:
			str = strcpy(str, "Impostor");
			break;
	}
	return str;
}

// Task 1
void *RotateMatrix(void *input)
{
	int x = *((int *)input);

	int **matrix = calloc(x, sizeof(int *));
	int **Rmatrix = calloc(x, sizeof(int *));

	for(int i = 0; i < x; i++)
	{
		matrix[i] = calloc(x, sizeof(int));
		Rmatrix[i] = calloc(x, sizeof(int));
	}

	int y, k;

	for(int i = 0; i < x; i++)
		for(int j = 0; j < x; j++)
		{
			y = (i + 1);
			k = j + 1;

			while(k != 0)
			{
				y *= 10;
				k /= 10;
			}
			y += j + 1;
			matrix[i][j] = y;
		}

	for(int i = 0; i < x; i++)
		for(int j = 0; j < x; j++)
			Rmatrix[j][x - i - 1] = matrix[i][j];

	char *outputStr = calloc(MAX_LEN_STR_OUT, sizeof(char));

	int index = 0;
	for(int i = 0; i < x; i++)
	{
		for(int j = 0; j < x; j++)
			index += sprintf(&outputStr[index], "%d ", Rmatrix[i][j]);
		index--;
		index += sprintf(&outputStr[index], "\n");
	}
	outputStr[strlen(outputStr) - 1] = '\0';
	for(int i = 0; i < x; i++)
	{
		free(matrix[i]);
		free(Rmatrix[i]);
	}
	free(matrix);
	free(Rmatrix);

	return (void *)outputStr;
}

// Task 2
void *DecodeString(void *input)
{
	char *str = (char *)input;
	char *p;
	int sum = 0;

	p = strtok(str, "_");

	while(p != NULL)
	{
		sum += atoi(p);
		p = strtok(NULL, "_");
	}

	char *outputStr = calloc(MAX_LEN_STR_OUT, sizeof(char));
	sprintf(outputStr, "%d", sum);

	return (void *)outputStr;
}

//Task 3
void *InvertArray(void *input)
{
	int *array = (int *)input;

	int n = array[0];
	int temp;

	if(n % 2 == 0)
	{
		for(int i = 1; i < n; i += 2)
		{
			temp = array[i];
			array[i] = array[i + 1];
			array[i + 1] = temp;
		}
	}
	else
	{
		for(int i = 1; i <= n / 2; i++)
		{
			temp = array[i];
			array[i] = array[n - i + 1];
			array[n - i + 1] = temp;
		}
	}
	char *outputStr = calloc(MAX_LEN_STR_OUT, sizeof(char));
	int index = 0;
	for(int i = 1; i <= n; i++)
		index += sprintf(&outputStr[index], "%d ", array[i]);

	outputStr[strlen(outputStr) - 1] = '\0';

	return (void *)outputStr;
}

//Task 4
Player *allocPlayer()
{
	Player *player = calloc(1, sizeof(Player));

	player->name = calloc(MAX_LEN_STR_ATR, sizeof(char));
	player->color = calloc(MAX_LEN_STR_ATR, sizeof(char));
	player->hat = calloc(MAX_LEN_STR_ATR, sizeof(char));
	player->indexOfLocation = 0;
	player->alive = 1;

	return player;
}

//Task 4
Game *allocGame()
{
	Game *game = calloc(1, sizeof(Game));
	game->name = calloc(MAX_LEN_STR_ATR, sizeof(char));

	return game;
}

//Task 5
Player *ReadPlayer(FILE *inputFile)
{
	Player *player = allocPlayer();

	fscanf(inputFile, "%s", player->name);
	fscanf(inputFile, "%s", player->color);
	fscanf(inputFile, "%s", player->hat);

	fscanf(inputFile, "%d", &player->numberOfLocations);
	player->locations = calloc(player->numberOfLocations, sizeof(Location));

	char *strLocation = calloc(MAX_LEN_STR_LOC, sizeof(char));
	char *p;

	for(int i = 0; i < player->numberOfLocations; i++)
	{
		fscanf(inputFile, "%s", strLocation);

		p = strtok(strLocation, ",");
		player->locations[i].x = atoi(p + 1);
		p = strtok(NULL, ",");
		p[strlen(p) - 1] = '\0';
		player->locations[i].y = atoi(p);
	}

	char *strPlayerRole = calloc(MAX_LEN_STR_ATR, sizeof(char));
	fscanf(inputFile, "%s", strPlayerRole);

	if(strcmp(strPlayerRole, "Rotator") == 0)
	{
		player->playerRole = Rotator;
		player->ability = Abilities[Rotator];
	}

	if(strcmp(strPlayerRole, "Decoder") == 0)
	{
		player->playerRole = Decoder;
		player->ability = Abilities[Decoder];
	}

	if(strcmp(strPlayerRole, "Invertor") == 0)
	{
		player->playerRole = Invertor;
		player->ability = Abilities[Invertor];
	}

	if(strcmp(strPlayerRole, "Impostor") == 0)
	{
		player->playerRole = Impostor;
		player->ability = Abilities[Impostor];
	}

	free(strPlayerRole);
	free(strLocation);
	return player;
}

// Task 5
Game *ReadGame(FILE *inputFile)
{
	Game *game = allocGame();

	fscanf(inputFile, "%s", game->name);
	fscanf(inputFile, "%d", &game->killRange);
	fscanf(inputFile, "%d", &game->numberOfCrewmates);

	game->crewmates = calloc(game->numberOfCrewmates, sizeof(Player *));

	for(int i = 0; i < game->numberOfCrewmates; i++)
		game->crewmates[i] = ReadPlayer(inputFile);
	
	game->impostor = ReadPlayer(inputFile);
	return game;
}

// Task 6
void WritePlayer(Player *player, FILE *outputFile)
{
	char *emunStr = fromEnumtoString(player->playerRole);

	fprintf(outputFile, "Player %s with color %s, hat %s and role %s has entered the game.\n",
	 player->name, player->color, player->hat, emunStr);
	fprintf(outputFile, "Player's locations: ");
	for(int i = 0; i < player->numberOfLocations; i++)
	{
		fprintf(outputFile, "(%d,%d) ", player->locations[i].x, player->locations[i].y);
	}
	fprintf(outputFile, "\n");

	free(emunStr);
}

// Task 6
void WriteGame(Game *game, FILE *outputFile)
{
	fprintf(outputFile, "Game %s has just started!\n", game->name);
	fprintf(outputFile, "\tGame options:\n");
	fprintf(outputFile, "Kill Range: %d\n", game->killRange);
	fprintf(outputFile, "Number of crewmates: %d\n\n", game->numberOfCrewmates);
	fprintf(outputFile, "\tCrewmates:\n");

	for(int i = 0; i < game->numberOfCrewmates; i++)
		WritePlayer(game->crewmates[i], outputFile);

	fprintf(outputFile, "\n\tImpostor:\n");
	WritePlayer(game->impostor, outputFile);
}

//Task 7
void *KillPlayer(void *input)
{
	Game *game = (Game *)input;
	Player *player;
	Player *impostor = game->impostor;
	int distance, minDistance = 1000;
	int indexOfTarget = -1;
	int playerX, playerY, impostorX, impostorY;
	char *outputStr = calloc(MAX_LEN_STR_OUT, sizeof(char));

	for(int i = 0; i < game->numberOfCrewmates; i++)
	{
		player = game->crewmates[i];
		if(player->alive == 0)
			continue;
		
		playerX = player->locations[player->indexOfLocation].x;
		playerY = player->locations[player->indexOfLocation].y;
		impostorX = impostor->locations[impostor->indexOfLocation].x;
		impostorY = impostor->locations[impostor->indexOfLocation].y;

		distance = abs(playerX - impostorX) + abs(playerY - impostorY);
		if(distance <= minDistance)
		{
			minDistance = distance;
			indexOfTarget = i;
		}
	}

	if(minDistance <= game->killRange)
	{
		game->crewmates[indexOfTarget]->alive = 0;
		sprintf(outputStr, "Impostor %s has just killed crewmate %s from distance %d.",
		 impostor->name, game->crewmates[indexOfTarget]->name, minDistance);
	}
	else
		sprintf(outputStr, "Impostor %s couldn't kill anybody.", impostor->name);

	return outputStr;
}

// Task 8
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix)
{
	Player *player, *impostor = game->impostor;
	char *outputPlayer;

	for(int i = 0; i < game->numberOfCrewmates; i++)
	{
		player = game->crewmates[i];

		if(player->alive == 0)
		{
			fprintf(outputFile, "Crewmate %s is dead.\n", player->name);
			continue;
		}

		player->indexOfLocation = (player->indexOfLocation + 1)
		 % player->numberOfLocations;
		fprintf(outputFile, "Crewmate %s went to location (%d,%d).\n", player->name,
		 player->locations[player->indexOfLocation].x, player->locations[player->indexOfLocation].y);

		outputPlayer = (char *)player->ability(inputMatrix[i]);
		fprintf(outputFile, "Crewmate %s's output:\n%s\n", player->name, outputPlayer);
		free(outputPlayer);
	}
	impostor->indexOfLocation = (impostor->indexOfLocation + 1)
	 % impostor->numberOfLocations;
	fprintf(outputFile, "Impostor %s went to location (%d,%d).\n", impostor->name,
	 impostor->locations[impostor->indexOfLocation].x, impostor->locations[impostor->indexOfLocation].y);

	outputPlayer = (char *)impostor->ability(inputMatrix[game->numberOfCrewmates]);
	fprintf(outputFile, "Impostor %s's output:\n%s\n", impostor->name, outputPlayer);
	free(outputPlayer);
}

// Task 9
void FreePlayer(Player *player)
{
	free(player->name);
	free(player->color);
	free(player->hat);
	free(player->locations);
	free(player);
}

// Task 9
void FreeGame(Game *game)
{
	free(game->name);
	FreePlayer(game->impostor);
	for(int i = 0; i < game->numberOfCrewmates; i++)
		FreePlayer(game->crewmates[i]);
	free(game->crewmates);
	free(game);
}