/******************************************************************************
FILENAME:     deck.c
Author:       Visakan Makilrajah
DATE:         March/23/2025
NOTES:        Queue implementation of the deck of cards
ATTRIBUTIONS: 
******************************************************************************/
#include "deck.h"
#include <time.h>
#include <Arduino.h>

//initialize the deck of cards
void initDeck(Deck* deck) {
    deck->front = 0;
    deck->rear = -1;
    deck->count = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            Card* newC = newCard(j, i); //create a new card
            enqueue(newC, deck); //add to the deck
        }
    }
}

//adds card to the queue with wrap around implemented
void enqueue(Card* card, Deck* deck){
    if (deck->count >= MAX_CARDS) return;
    deck->rear = (deck->rear + 1) % MAX_CARDS;
    deck->count++;
    deck->queue[deck->rear] = card;    
}

//removes card from the queue with wrap around implemented
Card* dequeue(Deck* deck){
    if (deck->count <= 0) return NULL;
    Card* pick = deck->queue[deck->front];
    deck->front = (deck->front + 1) % MAX_CARDS;
    deck->count--;
    return pick;
}

//displays the deck of cards
void displayDeck(Deck* deck){
    for (int i = 0; i < deck->count; i++){
        int index = (deck->front + i) % MAX_CARDS;
        displaySuit(deck->queue[index]);
        displayRank(deck->queue[index]);
        Serial.println();
    }
}

//checks if queue is empty
bool isEmpty(Deck* deck){
    if (deck->count == 0) return true;
    return false;
}

//checks if queue is full
bool isFull(Deck* deck){
    if (deck->count == MAX_CARDS) return true;
    return false;
}


//shuffle deck of cards using Fisher-Yates Shuffle
//chatgpt was used to implement Fisher-Yates Shuffle
void shuffle(Deck* deck) {
    randomSeed(analogRead(0));  // Use Arduino's random
    int size = deck->count;
    Card* temp[size]; //an array to store the cards
    
    int index = 0;
    //dequeue all the cards
    while(!isEmpty(deck)){
        temp[index++] = dequeue(deck);
    }

    //sort the cards in the temp array
    for (int i = size-1; i > 0; i--) {
        // Generate a random index j such that 0 <= j <= i
        int j = rand() % (i + 1);
        // Swap temp[i] with temp[j]
        swap(temp[i], temp[j]);
    }

    //enqueue all the cards back in the deck
    int added = 0;
    while(added < size){
        enqueue(temp[added++], deck);
    }
}