/******************************************************************************
FILENAME:     card.c
Author:       Visakan Makilrajah
DATE:         March/02/2025

ATTRIBUTIONS: [chatgpt]
******************************************************************************/
#include "card.h"

#include <Arduino.h>

// int main(void){
//     printf("Hello");
// }

//returns a pointer to a new card given the suit and rank
Card* newCard(int rank, int suit){
    Card* newC = (Card*)malloc(sizeof(Card)); //initalize a new card
    newC->rank = (Rank)(rank);
    newC->suit = (Suit)(suit);
    return newC;
}

//Given two pointers to cards, the function
//swaps the card struct they point to
void swap(Card* x, Card* y) {
    Card temp = *x;
    *x = *y;
    *y = temp;
}


//displays the card suit
void displaySuit(Card* x){
    switch (x->suit){
        case clubs:
            Serial.println("Clubs ");
            //display the card to the LED screen on reciever's arduino
            //repeat the same for the subsequent printf statements
            break;
        case diamonds:
            Serial.println("Diamonds ");
            break;
        case hearts:
            Serial.println("Hearts ");
            break;
        case spades:
            Serial.println("Spades ");
            break;
        default:
            break;
    }
}

//displays the card rank
void displayRank(Card* x){
    switch (x->rank){
        case A:
            Serial.println("A ");
            //Display the rank to the LCD screen
            //functionality needs to be added to
            //replace the printf statements
            break;
        case Two:
            Serial.println("2 ");
            break;
        case Three:
            Serial.println("3 ");
            break;
        case Four:
            Serial.println("4 ");
            break;
        case Five:
            Serial.println("5 ");
            break;
        case Six:
            Serial.println("6 ");
            break;
        case Seven:
            Serial.println("7 ");
            break;
        case Eight:
            Serial.println("8 ");
            break;
        case Nine:
            Serial.println("9 ");
            break;
        case Ten:
            Serial.println("10 ");
            break;
        case Jack:
            Serial.println("Jack ");
            break;
        case Queen:
            Serial.println("Queen ");
            break;
        case King:
            Serial.println("King ");
            break;
    }
}