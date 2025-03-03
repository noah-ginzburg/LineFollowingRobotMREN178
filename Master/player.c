// Player.c

#include "Player.h"  // Include the header file to link the declarations

#include <stdio.h>
#include <stdlib.h>

void Initialize()
{
  cards = malloc(sizeof(Card) * 8);
  number = 0;
}
