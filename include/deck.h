#ifndef DECK_H
#define DECK_H

#include <stdbool.h>

#include "card.h"

//struct for a deck
typedef struct Deck{
    int front;
    int rear;
    int count;
    Card queue[MAX_CARDS];
}Deck;

//Function prototypes
void enqueue(Card* card, Deck* deck);
Card dequeue(Deck* deck);
Card peek(Deck* deck);
void initDeck(Deck* deck);
void displayDeck(Deck* deck);
void shuffle(Deck* deck);
bool isEmpty(Deck* deck);
bool isFull(Deck* deck);

#endif