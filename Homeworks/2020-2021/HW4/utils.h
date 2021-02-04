#ifndef utils
#define utils

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define MAX_LEN_STR_ATR 2000
#define MAX_LEN_STR_LOC 2000
#define MAX_LEN_STR_OUT 700000

typedef enum types{
	Rotator,
	Decoder,
	Invertor,
    Impostor
} PlayerRoles;

typedef struct _location{
    int x, y;
} Location;

typedef struct _player{
    char *name;
    char *color;
    char *hat;
    int alive;
    int indexOfLocation;
    int numberOfLocations;
    Location *locations;
    PlayerRoles playerRole;
    void * (* ability)(void* x);
} Player;

typedef struct _game
{
	char *name;
	int killRange;
	int numberOfCrewmates;
	Player **crewmates;
    Player *impostor;
} Game;

void *RotateMatrix(void *input);
void *DecodeString(void *input);
void *InvertArray(void *input);
void *KillPlayer(void *input);

char *fromEnumtoString(PlayerRoles playerRole);
Player *allocPlayer();
Game *allocGame();
Player *ReadPlayer(FILE *inputFile);
Game *ReadGame(FILE *inputFile);
void WritePlayer(Player *player, FILE *outputFile);
void WriteGame(Game *game, FILE *outputFile);
void CalcuateNextCycleOfGame(Game *game, FILE *outputFile, void **inputMatrix);
void FreePlayer(Player *player);
void FreeGame(Game *game);
#endif