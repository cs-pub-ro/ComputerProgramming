#include "util/scrabble.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util/print_board.h"

#define MAX_WORD_SIZE 30
#define MAX_LINE_LENGTH 100
#define MAX_INT_LENGTH 10

int lettersPoints[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

/**
 * Adauga un cuvant nou pe board
 */
void addWordToBoard(char board[BOARD_SIZE][BOARD_SIZE], int y, int x, int direction, char word[]) {
    int wordLength = strlen(word);
    for (int i = 0; i < wordLength; i++) {
        if (direction == 0) {
            board[y][x + i] = word[i];
        } else {
            board[y + i][x] = word[i];
        }
    }
}

/**
 * Calculeaza punctajul unui cuvant fara bonus
 */
int getWordPointsWithoutBonus(char word[]) {
    int points = 0;
    int wordLength = strlen(word);

    for (int i = 0; i < wordLength; i++) {
        points += lettersPoints[word[i] - 'A'];
    }
    return points;
}

/**
 * Calculeaza punctajul unui cuvant tinand cont de bonus
 */
int getWordPointsWithBonus(char word[], int y, int x, int direction, char bonusString1[], char bonusString2[]) {
    int wordLength = strlen(word);
    int bonus1 = 0, bonus2 = 0;  // Retin 1 daca word e pe vreun bonus

    int points = getWordPointsWithoutBonus(word);  // Calculez punctajul fara bonus
    for (int i = 0; i < wordLength; i++) {         // Verific daca literele sunt pe bonus
        if (direction == 0) {                      // Verifica bonusul pe orizontala
            if (bonus_board[y][x + i] == 1) {
                bonus1++;
            }
            if (bonus_board[y][x + i] == 2) {
                bonus2++;
            }
        } else {  // Verifica bonusul pe verticala
            if (bonus_board[y + i][x] == 1) {
                bonus1++;
            }
            if (bonus_board[y + i][x] == 2) {
                bonus2++;
            }
        }
    }
    if (bonus1 >= 1 && strstr(word, bonusString1) != NULL) {  // Primul bonus
        points *= 1 << bonus1;                                // Punctele se inmultesc cu 2^(cate casute de 1 acopera cuvantul in tabla de bonus)
    }
    if (bonus2 >= 1 && strcmp(word + strlen(word) - strlen(bonusString2), bonusString2) == 0) {  // Al doilea bonus
        for (int i = 0; i < bonus2; i++) {                                                       // Punctele se inmultesc cu 3^(cate casute de 1 acopera cuvantul in tabla de bonus)
            points *= 3;
        }
    }

    return points;
}

/**
 * Verifica daca un cuvant a fost deja jucat
 */
int isWordPlayed(char word[], char wordsPlayed[][MAX_WORD_SIZE], int noWordsPlayed) {
    for (int i = 0; i < noWordsPlayed; i++) {
        if (strcmp(word, wordsPlayed[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * Verifica daca un cuvant poate fi pus pe orizontala la coordonatele date
 */
int canPlaceHorizontally(char board[BOARD_SIZE][BOARD_SIZE], char word[], int y, int x) {
    int wordLength = strlen(word);
    if (board[y][x] == word[0]) {
        for (int k = 1; k < wordLength; k++) {
            if (x + k >= BOARD_SIZE || board[y][x + k] != '.') {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

/**
 * Verifica daca un cuvant poate fi pus pe verticala la coordonatele date
 */
int canPlaceVertically(char board[BOARD_SIZE][BOARD_SIZE], char word[], int y, int x) {
    int wordLength = strlen(word);
    if (board[y][x] == word[0]) {
        for (int k = 1; k < wordLength; k++) {
            if (y + k >= BOARD_SIZE || board[y + k][x] != '.') {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

/**
 * Gaseste primul cuvant care poate fi jucat
 * @return 1 daca gaseste vreun cuvant, 0 daca nu
 */
int findAndPlaceFirstWord(char board[BOARD_SIZE][BOARD_SIZE],
                          char wordsPlayed[][MAX_WORD_SIZE], int noWordsPlayed) {
    for (int wordIndex = 0; wordIndex < NUM_WORDS; wordIndex++) {
        if (isWordPlayed(words[wordIndex], wordsPlayed, noWordsPlayed) == 0) {
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    if (canPlaceHorizontally(board, words[wordIndex], i, j)) {
                        addWordToBoard(board, i, j, 0, words[wordIndex]);
                        return 1;
                    }
                    if (canPlaceVertically(board, words[wordIndex], i, j)) {
                        addWordToBoard(board, i, j, 1, words[wordIndex]);
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/**
 * Cauta cel mai bun cuvant care poate fi jucat
 * @return scorul cuvantului jucat / 0 daca nu s-a gasit cuvant
 */
int findAndPlaceBestWord(char board[BOARD_SIZE][BOARD_SIZE],
                         char wordsPlayed[][MAX_WORD_SIZE], int *noWordsPlayed,
                         char bonusString1[], char bonusString2[]) {
    int bestScore = 0, bestWordY, bestWordX, bestWordDirection;
    char bestWord[MAX_WORD_SIZE];

    for (int wordIndex = 0; wordIndex < NUM_WORDS; wordIndex++) {
        if (isWordPlayed(words[wordIndex], wordsPlayed, *noWordsPlayed) == 0) {
            // Caut pozitia optima a cuvantului curent
            int currentWordScore = -1, currentWordY, currentWordX, currentWordDirection;
            for (int i = 0; i < BOARD_SIZE; i++) {
                for (int j = 0; j < BOARD_SIZE; j++) {
                    int horizontalScore = -1, verticalScore = -1;
                    if (canPlaceHorizontally(board, words[wordIndex], i, j)) {
                        horizontalScore = getWordPointsWithBonus(words[wordIndex], i, j, 0, bonusString1, bonusString2);
                    }
                    if (canPlaceVertically(board, words[wordIndex], i, j)) {
                        verticalScore = getWordPointsWithBonus(words[wordIndex], i, j, 1, bonusString1, bonusString2);
                    }

                    if (horizontalScore > currentWordScore) {
                        currentWordScore = horizontalScore;
                        currentWordY = i;
                        currentWordX = j;
                        currentWordDirection = 0;
                    }
                    if (verticalScore > currentWordScore) {
                        currentWordScore = verticalScore;
                        currentWordY = i;
                        currentWordX = j;
                        currentWordDirection = 1;
                    }
                }
            }
            // Verific daca s-a gasit un cuvant cu scor la fel de bun ca maximul curent
            if (currentWordScore == bestScore) {
                if (currentWordY < bestWordY) {
                    // Aleg cuvantul cu indicele de linie cel mai mic
                    bestScore = currentWordScore;
                    bestWordY = currentWordY;
                    bestWordX = currentWordX;
                    bestWordDirection = currentWordDirection;
                    strcpy(bestWord, words[wordIndex]);
                } else if (currentWordY == bestWordY) {
                    if (currentWordX < bestWordX) {
                        // Daca ambele cuvinte sunt pe aceeasi linie, il iau pe cel cu indecele de coloana cel mai mic
                        bestScore = currentWordScore;
                        bestWordY = currentWordY;
                        bestWordX = currentWordX;
                        bestWordDirection = currentWordDirection;
                        strcpy(bestWord, words[wordIndex]);
                    }
                }
            } else if (currentWordScore > bestScore) {
                // Daca noul cuvant are scor mai bun, atunci il stochez ca fiind cel mai bun
                bestScore = currentWordScore;
                bestWordY = currentWordY;
                bestWordX = currentWordX;
                bestWordDirection = currentWordDirection;
                strcpy(bestWord, words[wordIndex]);
            }
        }
    }
    if (bestScore > 0) {
        // Daca s-a gasit un cuvant, il pun pe tabla
        addWordToBoard(board, bestWordY, bestWordX, bestWordDirection, bestWord);
        strcpy(wordsPlayed[*noWordsPlayed], bestWord);
        (*noWordsPlayed)++;
    }
    return bestScore;
}

/**
 * Verifica daca string-ul are un '\n' la final si il elimina
 */
void removeNewLineFromString(char string[]){
    if(string[strlen(string) - 1] == '\n'){
        string[strlen(string) - 1] = 0;
    }
}

/**
 * Citeste un intreg de la tastatura
 */
int readInteger(){
    char aux[MAX_INT_LENGTH];
    fgets(aux, sizeof(aux), stdin);
    return atoi(aux);
}

/**
 * Citeste un cuvant, alaturi de coordonatele si directia acestuia.
 */
void readWordInformation(int *y, int *x, int *direction, char word[]){
    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), stdin);
    
    char *p = strtok(line, " ");
    *y = atoi(p);
    p = strtok(NULL, " ");
    *x = atoi(p);
    p = strtok(NULL, " ");
    *direction = atoi(p);
    p = strtok(NULL, " ");
    strcpy(word, p);
    removeNewLineFromString(word);
}

void solveTask1(char board[BOARD_SIZE][BOARD_SIZE]) {
    int n, y, x, direction;
    char word[MAX_WORD_SIZE];

    n = readInteger();
    for (int i = 0; i < n; i++) {
        readWordInformation(&y, &x, &direction, word);
        addWordToBoard(board, y, x, direction, word);
    }
    print_board(board);
}

void solveTask2() {
    int n, y, x, direction;
    int playerScore1 = 0, playerScore2 = 0;
    char word[MAX_WORD_SIZE];
    n = readInteger();
    for (int i = 0; i < n; i++) {
        readWordInformation(&y, &x, &direction, word);
        if (i % 2 == 0) {
            playerScore1 += getWordPointsWithoutBonus(word);
        } else {
            playerScore2 += getWordPointsWithoutBonus(word);
        }
    }
    printf("Player 1: %d Points\n", playerScore1);
    printf("Player 2: %d Points\n", playerScore2);
}

void solveTask3() {
    int n, y, x, direction;
    int playerScore1 = 0, playerScore2 = 0;
    char word[MAX_WORD_SIZE], bonusString1[MAX_WORD_SIZE], bonusString2[MAX_WORD_SIZE];
    fgets(bonusString1, sizeof(bonusString1), stdin);
    fgets(bonusString2, sizeof(bonusString2), stdin);
    removeNewLineFromString(bonusString1);
    removeNewLineFromString(bonusString2);
    n = readInteger();
    for (int i = 0; i < n; i++) {
        readWordInformation(&y, &x, &direction, word);
        if (i % 2 == 0) {
            playerScore1 += getWordPointsWithBonus(word, y, x, direction, bonusString1, bonusString2);
        } else {
            playerScore2 += getWordPointsWithBonus(word, y, x, direction, bonusString1, bonusString2);
        }
    }
    printf("Player 1: %d Points\n", playerScore1);
    printf("Player 2: %d Points\n", playerScore2);
}

void solveTask4(char board[BOARD_SIZE][BOARD_SIZE]) {
    int n, y, x, direction;
    char word[MAX_WORD_SIZE], bonusString1[MAX_WORD_SIZE], bonusString2[MAX_WORD_SIZE];
    char wordsPlayed[NUM_WORDS][MAX_WORD_SIZE];  // Retin cuvintele jucate pana acum
    int noWordsPlayed = 0;                       // Numarul de cuvinte jucate pana acum
    fgets(bonusString1, sizeof(bonusString1), stdin);
    fgets(bonusString2, sizeof(bonusString2), stdin);
    removeNewLineFromString(bonusString1);
    removeNewLineFromString(bonusString2);
    n = readInteger();
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d%s", &y, &x, &direction, word);
        addWordToBoard(board, y, x, direction, word);
        strcpy(wordsPlayed[noWordsPlayed++], word);
    }
    findAndPlaceFirstWord(board, wordsPlayed, noWordsPlayed);
    print_board(board);
}

void solveTask5(char board[BOARD_SIZE][BOARD_SIZE]) {
    int n, y, x, direction;
    int playerScore1 = 0;
    int playerScore2 = 0;
    char word[MAX_WORD_SIZE], bonusString1[MAX_WORD_SIZE], bonusString2[MAX_WORD_SIZE];
    char wordsPlayed[NUM_WORDS][MAX_WORD_SIZE];  // Retin cuvintele jucate pana acum
    int noWordsPlayed = 0;                       // Numarul de cuvinte jucate pana acum
    fgets(bonusString1, sizeof(bonusString1), stdin);
    fgets(bonusString2, sizeof(bonusString2), stdin);
    removeNewLineFromString(bonusString1);
    removeNewLineFromString(bonusString2);
    n = readInteger();
    for (int i = 0; i < n; i++) {
        readWordInformation(&y, &x, &direction, word);
        // scanf("%d%d%d%s", &y, &x, &direction, word);
        if (i % 2 == 0) {
            playerScore1 += getWordPointsWithBonus(word, y, x, direction, bonusString1, bonusString2);
        } else {
            playerScore2 += getWordPointsWithBonus(word, y, x, direction, bonusString1, bonusString2);
        }
        addWordToBoard(board, y, x, direction, word);
        strcpy(wordsPlayed[noWordsPlayed++], word);
    }
    playerScore2 += findAndPlaceBestWord(board, wordsPlayed, &noWordsPlayed, bonusString1, bonusString2);

    if (playerScore2 < playerScore1) {
        printf("Fail!\n");
    } else {
        print_board(board);
    }
}

void solveTask6(char board[BOARD_SIZE][BOARD_SIZE]) {
    int n, y, x, direction;
    int playerScore1 = 0;
    int playerScore2 = 0;
    char word[MAX_WORD_SIZE], bonusString1[MAX_WORD_SIZE], bonusString2[MAX_WORD_SIZE];
    char wordsPlayed[NUM_WORDS][MAX_WORD_SIZE];
    int noWordsPlayed = 0;
    fgets(bonusString1, sizeof(bonusString1), stdin);
    fgets(bonusString2, sizeof(bonusString2), stdin);
    removeNewLineFromString(bonusString1);
    removeNewLineFromString(bonusString2);
    n = readInteger();
    for (int i = 0; i < n; i++) {
        scanf("%d%d%d%s", &y, &x, &direction, word);
        strcpy(wordsPlayed[noWordsPlayed++], word);
        playerScore1 += getWordPointsWithBonus(word, y, x, direction, bonusString1, bonusString2);
        addWordToBoard(board, y, x, direction, word);

        playerScore2 += findAndPlaceBestWord(board, wordsPlayed, &noWordsPlayed, bonusString1, bonusString2);
    }

    print_board(board);
    printf("Player %d Won!\n", playerScore1 > playerScore2 ? 1 : 2);
}

int main() {
    // Initializez matricea
    char board[BOARD_SIZE][BOARD_SIZE];
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = '.';
        }
    }

    int task = readInteger();

    if (task == 0) {
        print_board(board);
    } else if (task == 1) {  // Cerinta 1
        solveTask1(board);
    } else if (task == 2) {  // Cerinta 2
        solveTask2();
    } else if (task == 3) {  // Cerinta 3
        solveTask3();
    } else if (task == 4) {  // Cerinta 4
        solveTask4(board);
    } else if (task == 5) {  // Cerinta 5
        solveTask5(board);
    } else if (task == 6) {  // Cerinta 6
        solveTask6(board);
    }

    return 0;
}