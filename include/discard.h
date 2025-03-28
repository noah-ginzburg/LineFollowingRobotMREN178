#ifndef DISCARD_H
#define DISCARD_H

#include <stdbool.h>
#include "card.h"


typedef struct Discard {
    int tos;
    Card* stack[MAX_CARDS];
}Discard;

//Function prototypes
void insert(Card* card, Discard* deck);
void initDeck(Discard *deck); //with cards
void displayDeck(Discard* deck);
Card* pop(Discard *deck);
bool isEmpty(Discard* deck);
bool isFull(Discard* deck);


#endif