#ifndef HAND_H
#define HAND_H

#include <stdlib.h>
#include <stdio.h>

#include <card.h>

typedef struct cardnode
{
    Card *card;            // pointer to card itself
    struct cardnode *next; // pointer to the left card
    struct cardnode *prev; // pointer to the right card

} CardNode;

typedef struct CardLinkedList
{
    CardNode *head; // pointer to the head of the list
} CardLL;

// functions for circular doubly linked list
CardNode *insertNode(CardNode *insert);
CardNode *createNode(Card *newCard);
CardNode *deleteNode(CardNode *node);
void displayCard(CardNode *node);
CardNode *searchNode(CardNode *head, Card card);
void emptyHand(); // empties the hand

#endif
