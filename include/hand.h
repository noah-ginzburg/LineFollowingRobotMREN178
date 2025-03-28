#include <stdlib.h>
#include <stdio.h>
#include <card.h>

typedef struct Node{
    Card* card; //pointer to card itself
    CardNode* left; //pointer to the left card
    CardNode* right; //pointer to the right card

} CardNode;

CardNode* head;

//functions for circular doubly linked list
void insertNode(CardNode* insert);
CardNode* createNode(Card* newCard);
CardNode* deleteNode(CardNode* node);
void displayCard(CardNode* node);
void emptyHand(); //empties the hand

