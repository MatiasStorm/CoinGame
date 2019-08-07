#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "ListHashTable.c"

typedef struct {
  int *coins;
  int size;
  int computerScore;
  int userScore;
} coinGame;

/*Create test game for bugfixing*/
coinGame* createTestCoinGame(){
  coinGame *g;
  g = (coinGame*) malloc(sizeof(coinGame));
  g->size = 6;
  g->computerScore = 0;
  g->userScore = 0;
  g->coins = (int*) malloc(sizeof(int) * g->size);
  int coins[] = {9,3,8,5,7,13};
  for(int i = 0; i < g->size; i++){
    g->coins[i] = coins[i];
  }
  return g;
}

/*Prints the score of the computer and the user and the array of coins still in the game*/
void printGame(coinGame *g){
  printf("Your Score: %d  Computer Score: %d\n", g->userScore, g->computerScore);
  for (int i = 0; i < g->size; i++){
    printf("%d ", g->coins[i]);
  }
  printf("\n\n");
}

/*Initialize a new game, asks the user to define the coins to be used in the game*/
coinGame* new(){
  printf("Welcome to the coin game!\n");
  printf("Enter n numbers separated by spaces\n");
  coinGame *game = (coinGame*) malloc(sizeof(coinGame));
  int size = 20;
  game->userScore = 0;
  game->computerScore = 0;
  game->size = 1;
  game->coins = (int*) malloc(sizeof(int) * size);
  scanf("%d", &game->coins[0]);
  while(game->size <= 20 && getchar() != '\n'){
    scanf("%d", &game->coins[game->size]);
    game->size++;    
  }
  if (game->size % 2 != 0){
    game->size--;
  } 
  
  return game;
}

/*Return the max of a and b*/
int max(int a, int b){
  return a > b ? a : b;
}

/*Returns the hash table of all subgames the computer can recieve, with the maximum score uptainable of that subgame as the value*/
arrayHashTable* createScoreTable(coinGame *game){
  int size = game->size * 2;
  arrayHashTable *scoreTable = createArrayHashTable(size);
  for(int L = 2; L <= game->size; L = L + 2){ // L = frame length
    for(int startIndex = 0; startIndex < game->size - L + 1; startIndex++){ // Starting index of the frame
      int maxScore = 0;
      int *framePtr = &game->coins[startIndex];
      int left = framePtr[0];
      int right = game->coins[startIndex + L - 1]; // End index of frame
      if (L == 2){
        maxScore = max(left, right);
      }
      else{ // Look up the max score of the two sub arrays which doesnt contain left, And do the same for right.
        int leftScore = left + max(hashGet(scoreTable, &framePtr[1], L - 2), // Middle sub Array
                                   hashGet(scoreTable, &framePtr[2], L - 2)); // Last sub array
        int rightScore = right + max(hashGet(scoreTable, framePtr, L - 2), // First sub array
                                     hashGet(scoreTable, &framePtr[1], L - 2)); // Middle sub array
        maxScore = max(rightScore, leftScore);
      }
      hashInsert(scoreTable, framePtr, L, maxScore); // Insert the sub array into the hashtable with maxScore as the value.
    }
  }
  return scoreTable;
}

/*Finds the best move (right or left), given a coinGame and the scoreTable
Returns either 'L or 'R'*/
char getMove(coinGame *game, arrayHashTable *scoreTable){
  if(game->size == 2){
    if(game->coins[0] > game->coins[1]) return 'L';
    else return 'R';
  }
  else{
    int L = game->size - 2; // Frame length
    int maxIndex;
    int maxScore = 0;
    for (int i = 0; i < 3; i++){ // Find the sub array with the highest score (Only 3 subarrays).
      int score = hashGet(scoreTable, &game->coins[i], L);
      if (score > maxScore){
        maxScore = score;
        maxIndex = i;
      }
    }
    if(maxIndex = 0) return 'R'; // Sub array [0:L] have the highest score, thus we take the right index so we can recieve that sub array.
    if(maxIndex == 2) return 'L';
    else return game->coins[0] > game->coins[game->size-1] ? 'L' : 'R'; // Mid subarray has highest score, take the coin with the highest value.
  }
}

/*Calls getMove, and modifies the game correspoinding the the returned char from getMove()*/
void computerTurn(coinGame *game, arrayHashTable *scoreTable){
  char move = getMove(game, scoreTable);
  printf("Computer Picked '%c'\n", move);
  if(move == 'L'){
    game->computerScore += game->coins[0];
    game->coins = &game->coins[1];
    game->size--;
  }
  else if(move == 'R'){
    game->computerScore += game->coins[game->size - 1];
    game->size--;
  }
}

/*Asks the user to type L or R, and updates the game depending on the coise*/
void userTurn(coinGame *game){
  printf("User Turn: \nEnter 'L' for left or 'R' right to pick a coin: ");
  char move;
  scanf("%c", &move);
  getchar(); // Remove \n from the terminal
  if(move == 'L' || move == 'l'){
    game->userScore += game->coins[0];
    game->coins = &game->coins[1];
    game->size--;
  }
  else if(move == 'R' || move == 'r'){
    game->userScore += game->coins[game->size - 1];
    game->size--;
  }
}

/*Loops through computers turn and users turn until the array of coins is empty*/
void run(coinGame *game){
  printGame(game);
  arrayHashTable *scoreTable = createScoreTable(game);
  while(game->size > 0){
    computerTurn(game, scoreTable);
    printGame(game);
    userTurn(game);
    printGame(game);
  }
  free(scoreTable);
}

void endGame(coinGame *game){
  char *winnerText = game->userScore > game->computerScore ? "Congratulations You Won!": "The computer won!";
  if(game->userScore == game->computerScore) winnerText = "It was a tie!";
  printf("%s\n\n", winnerText);
  free(game);
}



int main(){
  bool playing = true;
  while(playing){
      coinGame *game = new();
      run(game);
      endGame(game);
  }
  return 0;
}
