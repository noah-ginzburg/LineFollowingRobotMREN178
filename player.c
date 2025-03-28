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

Player* initPlayer()
{
    Player* player = (Player *)malloc(sizeof(Player)); // allocate memory for the player struct
    
    if (player == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    player->hand = (CardLL *)malloc(sizeof(CardLL)); // allocate memory for the player's hand
    player->hand->head = NULL;                       // initialize the head of the linked list to NULL

    player->handSize = 0;
    player->playerNum = -1;

    return player;
}

// deletes a card from the player's hand
// reorganizes the array, so it sorted
void deleteCard(Player *player, Card card)
{
    CardNode *node = searchNode(player->hand->head, card); // search for the node to delete

    node = deleteNode(node);

    free(node);

    player->handSize--;
}
