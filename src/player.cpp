/******************************************************************************
FILENAME:     player.c
AUTHORS:      Noah Ginzburg, Visakan Makilrajah
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#include "player.h"

bool AddCardToHand(Player *player, Card card)
{
    if (player->handSize < MAX_CARDS)
    {
        player->hand[(player->handSize)++] = card;
        return true;
    }
    else
        return false;
}

// Deletes a card from the player's hand
// Shuffles all elements after the deleted element down one
bool deleteCard(Player *player, Card card)
{
    if (player->handSize < 1)
        return false;

    int idx;
    for (idx = 0; idx < player->handSize; idx++) // Iterate only through the handSize
    {
        if (player->hand[idx].rank == card.rank && player->hand[idx].suit == card.suit)
        {
            // Shift cards after the deleted card
            for (int i = idx; i < player->handSize - 1; i++)
            {
                player->hand[i] = player->hand[i + 1];
            }

            player->handSize--; // Decrease the hand size
            return true;        // Return true when the card is successfully deleted
        }
    }

    return false; // Return false if the card is not found
}

// Init player not working in master.cpp?
void initPlayer(Player *player)
{
    player->handSize = 0;
    player->playerNum = -1;

    return;
}