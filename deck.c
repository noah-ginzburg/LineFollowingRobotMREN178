/******************************************************************************
FILENAME:     deck.c
Author:       Visakan Makilrajah
DATE:         March/23/2025
NOTES:        Queue implementation of the deck of cards
ATTRIBUTIONS:
******************************************************************************/
#include "deck.h"
#include <time.h>

// adds card to the queue with wrap around implemented
void enqueue(Card *card, Deck *deck)
{
    if (deck->count >= MAX_CARDS)
        return;
    deck->rear = ++(deck->rear) % MAX_CARDS;
    (deck->count)++;

    deck->queue[deck->rear] = *card;
};

// removes card from the queue with wrap around implemented
Card *dequeue(Deck *deck)
{
    if (deck->count <= 0)
        return NULL;
    Card pick = deck->queue[deck->front];
    deck->front = ++(deck->front) % MAX_CARDS;
    return &pick;
}

// returns the top card
Card *peek(Deck *deck)
{
    if (deck->count <= 0)
        return NULL;
    return &deck->queue[deck->front];
}

// displays the deck of cards
void displayDeck(Deck *deck)
{
    for (int i = 0; i < deck->count; i++)
    {
        int index = (deck->front + i) % MAX_CARDS;
        displaySuit(deck->queue[index].suit);
        displayRank(deck->queue[index].rank);
        printf("\n");
    }
}

// checks if queue is empty
bool isEmpty(Deck *deck)
{
    if (deck->count == 0)
        return true;
    return false;
};

// checks if queue is full
bool isFull(Deck *deck)
{
    if (deck->count == MAX_CARDS)
        return true;
    return false;
};

// shuffle deck of cards using Fisher-Yates Shuffle
// chatgpt was used to implement Fisher-Yates Shuffle
void shuffle(Deck *deck)
{
    if (isEmpty(deck))
        return; // Nothing to shuffle

    // Seed RNG
    srand((unsigned int)time(NULL));
    int size = deck->count;
    CardNode *temp[size]; // Temporary array to store cards

    // Dequeue all cards into temp array
    int index = 0;
    while (!isEmpty(deck))
    {
        temp[index++] = dequeue(deck);
    }

    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);     // Random index
        CardNode *tempCard = temp[i]; // Swap manually
        temp[i] = temp[j];
        temp[j] = tempCard;
    }

    // Re-enqueue shuffled cards
    for (int i = 0; i < size; i++)
    {
        enqueue(temp[i], deck);
    }
}

// initialize the deck of cards
void initDeck(Deck *deck)
{
    deck->front = 0;
    deck->rear = -1;
    deck->count = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            Card *new = newCard(j, i); // create a new card
            enqueue(new, deck);        // add to the deck
        }
    }
}