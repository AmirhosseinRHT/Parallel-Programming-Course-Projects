#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define TOKEN_SIZE 10
#define PAWN   "pawn"
#define KNIGHT "knight"
#define ROOK   "rook"
#define QUEEN  "queen"
#define KING   "king"
#define MOVE_SIZE 6
const char CHESS_TOKEN[] = "CHESS";


char* createBlackToken()
{
    char* tokenHolder = (char*)(malloc(sizeof(char) * TOKEN_SIZE + 1));
    if (tokenHolder == NULL)
        return NULL;

    srand(time(0));
    for (int i = 0; i < TOKEN_SIZE; i++)
        tokenHolder[i] = rand() % 255;
    tokenHolder[TOKEN_SIZE] = '\0';
    return tokenHolder;
}

char* createWhiteToken(char* previousToken)
{
    if (previousToken == NULL)
        return NULL;
    char* currentToken = (char*)(malloc(sizeof(char) * TOKEN_SIZE + 1));
    if (currentToken == NULL)
        return NULL;
    int i = 0;
    while (i < TOKEN_SIZE)
    {
        if (i < strlen(CHESS_TOKEN))
            currentToken[i] = CHESS_TOKEN[i];
        else
            currentToken[i] = previousToken[i] + 1;
        i++;
    }
    currentToken[TOKEN_SIZE] = '\0';
    free(previousToken);
    return currentToken;
}

char* initFirstMove(char* whiteToken)
{
    if (whiteToken == NULL || strncmp(whiteToken, CHESS_TOKEN, strlen(CHESS_TOKEN)) != 0)
        return whiteToken;
    
    int choice;
    printf(
        "0: A King move\n"
        "1: A Queen move\n"
        "2: A Rook move\n"
        "3: A Knight move\n"
        "4: A Pawn move\n"
        "White's turn, enter the first move: ");
    scanf("%d", &choice);
    char* newToken = (char*)(malloc(sizeof(char) * MOVE_SIZE+1));
    if (newToken == NULL){
        free(whiteToken);
        return NULL;
    }
    switch (choice){
    case 0:
        strncpy(newToken, KING, MOVE_SIZE);
        break;
    case 1:
        strncpy(newToken, QUEEN, MOVE_SIZE);
        break;
    case 2:
        strncpy(newToken, ROOK, MOVE_SIZE);
        break;
    case 3:
        strncpy(newToken, KNIGHT, MOVE_SIZE);
        break;
    case 4:
        strncpy(newToken, PAWN, MOVE_SIZE);
        break;
    default:
        free(newToken);
        free(whiteToken);
        return NULL;
    }
    newToken[MOVE_SIZE] = '\0';
    free(whiteToken);
    return newToken;
}

int main(int argc, char* argv[])
{
    char* token = createBlackToken();
    printf("Token: %s\n", token);
    token = createWhiteToken(token);
    token = initFirstMove(token);
    printf("White's move: %s\n", token);
    free(token);
    return EXIT_SUCCESS;
}