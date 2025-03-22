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

#define MAX_HAND_SIZE 10

typedef enum PlayerOption
{
    PLAY,
    DRAW,
    SHOW,
    PASS
} PO;

typedef enum {false, true} bool;

typedef struct player
{
    Card* cards[MAX_HAND_SIZE];
    int curNumCards;
    int playerNum;
} Player;

bool PlayCard(Player* player, int index, Card* lastPlayed);
Card* DrawCard(Deck* deck);
void ShowCards();
void initPlayer(Player* player, int num);
void deleteCard(Player* player, int index);
// Not Done Yet,

#endif