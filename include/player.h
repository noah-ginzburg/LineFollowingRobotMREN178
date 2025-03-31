/******************************************************************************

FILENAME:     player.h
Author:       Noah Ginzburg
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "deck.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_HAND_SIZE 10

typedef struct player
{
    Card hand[MAX_HAND_SIZE]; // pointer to the player's hand
    int handSize;
    int playerNum;
} Player;

bool AddCardToHand(Player *player, Card card);
void initPlayer(Player *player);
bool deleteCard(Player *player, Card card);

#endif