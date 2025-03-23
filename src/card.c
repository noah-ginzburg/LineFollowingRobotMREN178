/******************************************************************************
FILENAME:     card.c
Author:       Visakan Makilrajah
DATE:         March/02/2025

ATTRIBUTIONS: [chatgpt]
******************************************************************************/
#include "../include/card.h"

int main(void){
    printf("Hello");
}

void swap(Card* x, Card* y) {
    Card temp = *x;
    *x = *y;
    *y = temp;
}

void displaySuit(Card* x){
    switch (x->suit){
        case clubs:
            printf("Clubs ");
            //display the card to the LED screen on reciever's arduino
            //repeat the same for the subsequent printf statements
            break;
        case diamonds:
            printf("Diamonds ");
            break;
        case hearts:
            printf("Hearts ");
            break;
        case spades:
            printf("Spades ");
            break;
        default:
    }
}

void displayRank(Card* x){
    switch (x->rank){
        case A:
            printf("A ");
            //Display the rank to the LCD screen
            //functionality needs to be added to
            //replace the printf statements
            break;
        case Two:
            printf("2 ");
            break;
        case Three:
            printf("3 ");
            break;
        case Four:
            printf("4 ");
            break;
        case Five:
            printf("5 ");
            break;
        case Six:
            printf("6 ");
            break;
        case Seven:
            printf("7 ");
            break;
        case Eight:
            printf("8 ");
            break;
        case Nine:
            printf("9 ");
            break;
        case Ten:
            printf("10 ");
            break;
        case Jack:
            printf("Jack ");
            break;
        case Queen:
            printf("Queen ");
            break;
        case King:
            printf("King ");
            break;
    }
}
