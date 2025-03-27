/******************************************************************************
FILENAME:     player.c
AUTHORS:      Noah Ginzburg, Visakan Makilrajah
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#include "player.h"
#include "deck.h"

bool PlayCard(Player *player, int index, Card *lastPlayed)
{
    /*
    1. display a "choose card" menu menu to the user, let them choose from their current cards
    2. send that card to the master
    -> the master will determine whether the card is valid
    3. wait for hc12 to be available for feedback
    -> the master will send feedback, if it is good feedback, end the function. if not, keep looping until a valid option is chosen
    */

    // Get the card to be played
    Card *toPlay = player->cards[index];

    // Check if the card to be play follows the game conditions
    if (toPlay->rank == Eight)
    {
        // add prompt to select the suit
        deleteCard(player, index);
        return true;
    }
    else if (toPlay->suit == lastPlayed->suit || toPlay->rank == lastPlayed->rank)
    {
        deleteCard(player, index);
        return true;
    }
    else
    {
        return false;
    }
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

void initPlayer(Player *player)
{
    for (int i = 0; i < MAX_HAND_SIZE; i++)
    {
        player->cards[i] = NULL;
    }

    player->curNumCards = 0;
    player->playerNum = -1;
}

// deletes a card from the player's hand
// reorganizes the array, so it sorted
void deleteCard(Player *player, int index)
{
    for (;; index < MAX_HAND_SIZE - 1)
    {
        player->cards[index] = player->cards[index++];
    }
    player->cards[player->curNumCards - 1] = NULL;
    player->curNumCards--;
}
