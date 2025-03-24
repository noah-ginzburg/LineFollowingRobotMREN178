#include "deck.h"

//set top pointer to -1
void initDeck(Deck* deck){
    deck->count = 0;
    deck->tos = -1;
}

//initialize the deck of cards
void initDeckCards(Deck* deck) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            insert(j, i, deck);
        }
    }
}

//shuffle deck of cards using Fisher-Yates Shuffle
//credit: chatGPT
void shuffle(Deck* deck) {
    //random number generator
    srand(time(NULL));

    for (int i = MAX_CARDS - 1; i > 0; i--) {
        // Generate a random index from 0 to i
        int j = rand() % (i + 1);
        // Swap the elements at index i and j
        swap(&(deck->stack[i]), &(deck->stack[j]));
    }
}

void displayDeck(Deck* deck) {
    for (int i = 0; i < 4; i++){
        displaySuit(deck->stack[i*13]);
        for (int j = 0; j < 13; j++){
            displayRank(deck->stack[i*13 + j]);
        }
        printf("\n");
    }
    //apply switch case to display the corresponding rank and suit
    //display in rows of thirteen
}

//insert new card into the array
void insert(int rank, int suit, Deck* deck) {
    if (deck->tos == MAX_CARDS-1) return;
    deck->stack[(deck->tos)++]->suit = (Suit)suit;
    deck->stack[(deck->tos)]->rank = (Rank)rank;
    (deck->count)++;
}

//removes card from the deck
Card* pop(Deck* deck){
    if (deck->tos != -1) return deck->stack[(deck->tos)--];
}