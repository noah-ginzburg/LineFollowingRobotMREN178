/******************************************************************************
FILENAME:     player.c
AUTHORS:      Noah Ginzburg, Visakan Makilrajah
DATE:         March/16/2025

ATTRIBUTIONS:
******************************************************************************/

#include "player.h"
#include "deck.h"

bool PlayCard(Player* player, int index, Card* lastPlayed)
{
    //Get the card to be played
    Card* toPlay = player->cards[index];

    //Check if the card to be play follows the game conditions
    if (toPlay->rank == Eight){
        //add prompt to select the suit
        deleteCard(player, index);
        return true;
    }
    else if (toPlay->suit == lastPlayed->suit || toPlay->rank == lastPlayed->rank){
        deleteCard(player, index);
        return true;
    }
    else{
        return false;
    }
}

Card* DrawCard(Deck* deck)
{
    //returns card from the top of deck
    return pop(deck);
}

void ShowCards(Player *player)
{
    //need to add functionally for this
}

void initPlayer(Player *player, int num){
    player->curNumCards = 0;
    int playerNum = num;
}

//deletes a card from the player's hand
//reorganizes the array, so it sorted
void deleteCard(Player *player, int index){
    for (;;index< MAX_HAND_SIZE-1){
        player->cards[index] = player->cards[index++];
    }
    player->cards[player->curNumCards-1] = NULL;
    player->curNumCards--;
}
