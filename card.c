/******************************************************************************
FILENAME:     card.c
Author:       Visakan Makilrajah
DATE:         March/02/2025

ATTRIBUTIONS: [chatgpt]
******************************************************************************/

#include "card.h"


Card discardPile[MAX_CARDS]; //array stack for discard pile
Card deck[MAX_CARDS]; //array stack for the deck of cards

int tos = -1; //index of the top of the stack

//insert new card into the array
void push(int rank, int suit) {
    if (tos == MAX_CARDS-1) return;
    deck[tos++].rank = (Rank)rank;
    deck[tos].suit = (Suit)suit;
}

//initialize the deck of cards
void initDeck() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            push(i, j);
        }
    }
}

//shuffle deck of cards using Fisher-Yates Shuffle
//credit: chatGPT
void shuffle(Card arr[MAX_CARDS]) {
    //random number generator
    srand(time(NULL));

    for (int i = MAX_CARDS - 1; i > 0; i--) {
        // Generate a random index from 0 to i
        int j = rand() % (i + 1);
        // Swap the elements at index i and j
        swap(&arr[i], &deck[j]);
    }
}

void swap(Card* x, Card* y) {
    Card temp = *x;
    *x = *y;
    *y = temp;
}

void displayDeck() {
    //apply switch case to display the corresponding rank and suit
    //display in rows of thirteen
}
int main(void) {
    printf("Hello, World!\n");
    return 0;
}