/******************************************************************************

FILENAME:     player.h
Author:       Noah Ginzburg
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#ifndef PLAYER.H
#define PLAYER .H

#include "card.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_HAND_SIZE 10

typedef enum PlayerOption
{
    PLAY,
    DRAW,
    SHOW
} PO;

typedef struct player
{
    Card cards[MAX_HAND_SIZE];
    int curNumCards;
    int playerNum;
} Player;

void PlayCard(Card card);
Card DrawCard();
void ShowCards();
// Not Done Yet,

#endif