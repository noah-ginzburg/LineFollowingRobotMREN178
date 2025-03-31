// Player GUI setup below:

// Start by intializing buttons (left, right) as well as LCD pins
// Different menu options
// Switch cases -> if button 'left/right' is pressed, switch to menu 2
// If select is pressed for each menu (pass/draw/show), present different screen

// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>

// Includes all other libraries
#include "DataTransmission.h"
#include "interface.h"
#include "player.h"
#include "card.h"

const int NODE_ID = 2; //Player Node ID (sender ID)

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int analogInPin = A0;
int buttonVal = 0;
int prevButtonVal = 0;

extern SoftwareSerial HC12;

// initialize menu
int menu = 1;


////// TEST VALUES //////

// Next two lines create a pointer to a card that is passed on 
// into the sendPacket function to simulate a player playing or receiving a card
// Card testCard = {Five, hearts};
// Card* testCardPtr = &testCard;

// Card* testHand[10] = {&testCard}; //This creates an array of pointers to cards and sets them all to the 5 of Hearts.
// This simulates the master sending the player their hand at the beginning of the game

///// END OF TEST VALUES //////

typedef enum playerstate
{
  NOT_CONNECTED,
  CONNECTED_AND_WAITING,
  GET_HAND,
  IN_GAME,
  GAME_OVER
} PlayerState;


PlayerState ps = NOT_CONNECTED;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  HC12.begin(9600);
  // initialize the serial communications:
  Serial.begin(9600);
}

// initializes loop
void loop(){
  buttonVal = analogRead(analogInPin);
  Packet *incomingPacket = receivePacket();
  if (incomingPacket != NULL) {
    if (incomingPacket->receiverID != NODE_ID && incomingPacket->receiverID != -1) {
      incomingPacket = NULL;
    }
  }

  switch (ps)
  {
    case NOT_CONNECTED:
      if (abs(prevButtonVal - buttonVal) > 20){
        if (IsButtonPressed(buttonVal, SELECT)){
          AutoPrint(&lcd, "Requesting to Connect...");
          sendPacket(NODE_ID, 0, addPlayer, NULL, NULL);
        }
      }

      if (incomingPacket != NULL){
        if (incomingPacket->command == addPlayer){
          ps = CONNECTED_AND_WAITING;
        }
      }
      break;


    case CONNECTED_AND_WAITING:
      SwapMessages(&lcd, millis(), "You are Connected!", "Waiting for Game to Start!", 1500);
      if (incomingPacket != NULL && incomingPacket->command == startGame)
      {
        AutoPrint(&lcd, "Game Starting!");
        delay(2000);
        ps = GET_HAND;
        tone(3, 1000, 200);
        lcd.clear();
      }
      break;
    

    case GET_HAND:
      if (incomingPacket != NULL){
        AutoPrint(&lcd, "Hello");
        if (incomingPacket->command == addHand){
          Card cardReceived = incomingPacket->cardSent;
          //add to the player's hand array
          char rankPtr[2];
          char suitPtr[2];
          sprintf(rankPtr, "%d", cardReceived.rank);
          sprintf(suitPtr, "%d", cardReceived.suit);
          char card[30] = "New Card: Rank: ";
          strcat(card, rankPtr);
          strcat(card, " Suit: ");
          strcat(card, suitPtr);
          AutoPrint(&lcd, card, 1);
        }
      }
      break;
    
    case IN_GAME:














































































































































































































































      
    default:
      AutoPrint(&lcd, "Default", 1);
      break;
  }
  prevButtonVal = buttonVal;
  delay(100);
}



















/*
  //Packet *incomingPacket = receivePacket();

  switch (incomingPacket->command)
  {
  case addPlayer:
    isConnected = true;
    pNum = incomingPacket->receiverID;

    AutoPrint(&lcd, incomingPacket->string, 1);
    tone(3, 500, 200);
    noTone(3);
    delay(5000);

    break;

  default:
    break;
  }

  int x = analogRead(0);
  lcd.setCursor(0,1);

//loops between menus depending on when LEFT and RIGHT is pressed

  if (x < RIGHT) {
    menu++;
    delay(200);
        while (x < 1000) {
      x = analogRead (0);
    }
  }
  if (x <= LEFT) {
    menu--;
    delay(200);
        while (x < 1000) {
      x = analogRead (0);
    }
  }

  //loops between more menus when select is pressed for each main switch case (see switch(menu))

  if (menu == 1 && x <= SELECT) {
    menu = 5;
    delay(200);
       while (x < 1000) {
      x = analogRead (0);
    }
  }

  //initialize switch cases
    switch(menu) {
    //MAIN SWITCH CASES (shows the user three options: pass, draw, or show)

    //Switch case 1:
    case 1:
    lcd.print("Pass ");
    break;
    //Switch case 2:
    case 2:
    lcd.print("Draw ");
    break;
    //Switch case 3:
    case 3:
    lcd.print("Show ");
    break;

    //INVALID SWITCH CASES (cases that, if encountered, return the user to one of the main switch cases)

    //Switch case 0:
    case 0:
    menu = 3;
    break;
    //Switch case 4:
    case 4:
    menu = 1;
    break;

    //SELECT SWITCH CASES (cases that, when select is pressed while one of the main switch cases is displayed, returns one of
    //three functions: pass function, draw function, or show function

    //Switch case 5:
    case 5:
        //run pass function
    break;
    }
*/
//}