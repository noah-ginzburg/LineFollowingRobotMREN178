#ifndef DECK_H
#define DECK_H

#include "card.h"

typedef struct Deck {
    int tos;
    int count;
    Card* stack[MAX_CARDS];
}Deck;

//Function prototypes
void insert(int rank, int suit, Deck* deck);
Card* pop(Deck *deck);
void initDeckCards();
void initDeck();
void shuffle();
void displayDeck();


#endif