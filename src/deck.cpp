/******************************************************************************
FILENAME:     deck.c
Author:       Visakan Makilrajah
DATE:         March/23/2025
NOTES:        Queue implementation of the deck of cards
ATTRIBUTIONS:
******************************************************************************/
#include "deck.h"
#include <Arduino.h>

void initDeck(Deck *deck)
{
    deck->front = 0;
    deck->rear = -1;
    deck->count = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = A; j <= King; j++)
        {
            // Store values directly in the queue array
            deck->rear = (deck->rear + 1) % MAX_CARDS;
            deck->queue[deck->rear].suit = (Suit)i;
            deck->queue[deck->rear].rank = (Rank)j;
            deck->count++;
        }
    }
}

// adds card to the queue with wrap around implemented
void enqueue(Card *card, Deck *deck)
{
    if (deck->count >= MAX_CARDS)
        return;
    deck->rear = (deck->rear + 1) % MAX_CARDS;
    deck->count++;
    deck->queue[deck->rear] = *card;
}

Card dequeue(Deck *deck)
{
    if (deck->count <= 0)
    {
        Card empty = {A, clubs}; // Define these in your enum
        return empty;
    }
    Card result = deck->queue[deck->front];
    deck->front = (deck->front + 1) % MAX_CARDS;
    deck->count--;
    return result;
}

// returns the top card
Card peek(Deck *deck)
{
    if (deck->count <= 0)
    {
        Card empty = {BAD, clubs};
        return empty;
    }

    return deck->queue[deck->front];
}

// displays the deck of cards
void displayDeck(Deck *deck)
{
    for (int i = 0; i < deck->count; i++)
    {
        int index = (deck->front + i) % MAX_CARDS;
        displaySuit(&(deck->queue[index]));
        displayRank(&(deck->queue[index]));
        Serial.println();
    }
}

// checks if queue is empty
bool isEmpty(Deck *deck)
{
    if (deck->count == 0)
        return true;
    return false;
}

// checks if queue is full
bool isFull(Deck *deck)
{
    if (deck->count == MAX_CARDS)
        return true;
    return false;
}

void shuffle(Deck *deck)
{
    if (deck == NULL || deck->count == 0)
        return;

    randomSeed(analogRead(0)); // Better randomness
    int size = deck->count;

    // Create temporary array of Cards (not pointers)
    Card temp[MAX_CARDS]; // Use max size to avoid VLA

    // Dequeue all cards into temp array
    for (int i = 0; i < size; i++)
    {
        temp[i] = dequeue(deck);
    }

    // Fisher-Yates shuffle
    for (int i = size - 1; i > 0; i--)
    {
        int j = random(i + 1); // Arduino's better random function
        // Swap cards
        Card tmp = temp[i];
        temp[i] = temp[j];
        temp[j] = tmp;
    }

    // Enqueue back
    for (int i = 0; i < size; i++)
    {
        enqueue(&temp[i], deck);
    }
}