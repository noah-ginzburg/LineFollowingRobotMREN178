/******************************************************************************
FILENAME:     player.c
AUTHORS:      Noah Ginzburg, Visakan Makilrajah
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#include "player.h"

bool PlayCard(Player *player, int index, Card *lastPlayed)
{
    /*
    1. display a "choose card" menu menu to the user, let them choose from their current cards
    2. send that card to the master
    -> the master will determine whether the card is valid
    3. wait for hc12 to be available for feedback
    -> the master will send feedback, if it is good feedback, end the function. if not, keep looping until a valid option is chosen
    */
}

Card *DrawCard(Deck *deck)
{
    /*
    1. request to draw a card from the msater
    2. wait for serial feedback
    3. display a "Card received: [new card]" message to the user

    */

    // returns card from the top of deck
    return pop(deck);
}

void ShowCards(Player *player)
{
    // need to add functionally for this
    // Simply display the cards in the player's hand
}

// Init player not working in master.cpp?
void initPlayer(Player *player)
{
    player->handSize = 0;
    player->playerNum = -1;

    return;
}

// deletes a card from the player's hand
// reorganizes the array, so it sorted
void deleteCard(Player *player, Card card)
{
}
