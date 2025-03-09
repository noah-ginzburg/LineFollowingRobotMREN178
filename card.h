#ifndef CARD_H
#define CARD_H

//Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//define enums and macros
typedef enum {clubs = 0, diamonds, hearts, spades} Suit;
typedef enum {A = 0, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King} Rank;
#define MAX_CARDS 52

//define structures
typedef struct Card {
    Rank rank;
    Suit suit;
}Card;


//Function prototypes
void push(int rank, int suit);
void initDeck();
void shuffle();
void swap(int *x, int *y);
void displayDeck();

#endif