/******************************************************************************
FILENAME:     card.c
Author:       Visakan Makilrajah
DATE:         March/23/2025
NOTES:        Stack implementation of the discard pile
ATTRIBUTIONS:
******************************************************************************/

#include "discard.h"

// set top pointer to -1 and count to zero
void initDeck(Discard *deck)
{
    deck->tos = -1;
}

// displays the discard pile
void displayDeck(Discard *deck)
{
    // Loop until and including the top of the stack (tos)
    for (int i = 0; i <= deck->tos; i++) // Change from i < deck->tos to i <= deck->tos
    {
        displaySuit(deck->stack[i].card);
        displayRank(deck->stack[i].card);
        printf("\n");
    }
}

// insert new card into the stack
void insert(CardNode *cardNode, Discard *deck)
{
    // check if stack is full
    if (deck->tos == MAX_CARDS - 1)
        return;
    // add card to stack and increment count by 1
    deck->stack[(deck->tos)++] = *cardNode;
}

// removes card from the stack
CardNode *pop(Discard *deck)
{
    // remove card if stack is not empty
    if (deck->tos != -1)
        return &deck->stack[(deck->tos)--];
    return NULL;
}

// check if stack is empty
bool isEmpty(Discard *deck)
{
    if (deck->tos == -1)
        return true;
    return false;
}

// checks if stack is full
bool isFull(Discard *deck)
{
    if (deck->tos == MAX_CARDS - 1)
        return true;
    return false;
}