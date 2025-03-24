/******************************************************************************
FILENAME:     card.c
Author:       Visakan Makilrajah
DATE:         March/23/2025
NOTES:        Stack implementation of the discard pile
ATTRIBUTIONS: 
******************************************************************************/

#include "../include/discard.h"

int main(void){
}

//set top pointer to -1 and count to zero
void initDeck(Discard* deck){
    deck->tos = -1;
}


//displays the discard pile
void displayDeck(Discard* deck) {
    for (int i = 0; i < MAX_CARDS; i++){
        displaySuit(deck->stack[i]);
        displayRank(deck->stack[i]);
        printf("\n");
    }
}

//insert new card into the stack
void insert(Card* card, Discard* deck) {
    //check if stack is full
    if (deck->tos == MAX_CARDS-1) return;
    //add card to stack and increment count by 1
    deck->stack[(deck->tos)++] = card;
}

//removes card from the stack
Card* pop(Discard* deck){
    //remove card if stack is not empty
    if (deck->tos != -1) return deck->stack[(deck->tos)--];
    return NULL;
}

//check if stack is empty
bool isEmpty(Discard* deck){
    if (deck->tos == -1) return true;
    return false;
}

//checks if stack is full
bool isFull(Discard* deck){
    if (deck->tos == MAX_CARDS-1) return true;
    return false;
}