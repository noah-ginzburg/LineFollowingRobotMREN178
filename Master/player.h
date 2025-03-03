//Player.h

#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"

const int STARTING_SIZE = 8;

typedef struct player {
  Card* cards;
  int handSize;
  int number;
}Player;

#endif
