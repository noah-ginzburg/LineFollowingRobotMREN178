/******************************************************************************

FILENAME:     player.h
Author:       Noah Ginzburg
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#ifndef PLAYER.H
#define PLAYER.H

#include "card.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_HAND_SIZE 10

// player has three options, they can play a card, draw, view their cards or pass their turn
typedef enum PlayerOption
{
    PLAY,
    DRAW,
    SHOW,
    PASS
} PO;

typedef struct player
{
    Card *cards[MAX_HAND_SIZE];
    int curNumCards;
    int playerNum;
} Player;

bool PlayCard(Player *player, int index, Card *lastPlayed);
void ShowCards();
void initPlayer(Player *player);
void deleteCard(Player *player, int index);
// Not Done Yet,

#endif