#include "deck.h"
#include "deck.c"
#include "card.c"

int main(void){
    Deck deck;
    Deck discard;
    initDeck(&deck);
    initDeck(&discard);
    initDeckCards(&deck);
    displayDeck(&deck);
}