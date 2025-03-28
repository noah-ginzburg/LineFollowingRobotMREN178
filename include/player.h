/******************************************************************************

FILENAME:     player.h
Author:       Noah Ginzburg
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "hand.h"

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
    CardLL *hand; // pointer to the player's hand
    int handSize;
    int playerNum;
} Player;

bool PlayCard(Player *player, int index, Card *lastPlayed);
void ShowCards();
void initPlayer(Player *player);
void deleteCard(Player *player, Card card);
// Not Done Yet,

#endif