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

#define MAX_STR_SIZE 33

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int analogInPin = A0;
int buttonVal = 0;
int prevButtonVal = 0;

typedef enum playerstate
{
  NOT_CONNECTED,
  CONNECTED_AND_WAITING,
  IN_GAME,
  GAME_OVER
} PlayerState;

PlayerState ps = NOT_CONNECTED;

int pNum = -1;

extern SoftwareSerial HC12;

// initialize menu
int menu = 1;

////// TEST VALUES //////

// Next two lines create a pointer to a card that is passed on
// into the sendPacket function to simulate a player playing or receiving a card
Card testCard = {Five, hearts};
Card *testCardPtr = &testCard;

Card *testHand[10] = {&testCard}; // This creates an array of pointers to cards and sets them all to the 5 of Hearts.
// This simulates the master sending the player their hand at the beginning of the game

char string[MAX_STR_SIZE] = "Message";

char connectedMsg[MAX_STR_SIZE] = "";
char waitGameStartMsg[MAX_STR_SIZE] = "Waiting on Game Start";

///// END OF TEST VALUES //////

int isSomeBtnClicked(int prevButtonVal, int buttonVal)
{
  return abs(prevButtonVal - buttonVal) > 20;
}

// prints 'Action:' on LCD
void setup()
{
  lcd.begin(16, 2);
  HC12.begin(9600);
  Serial.begin(9600);
  lcd.setCursor(0, 0);
  lcd.print("Action:");
}

// initializes loop
void loop()
{
  // Serial.println("Inside Loop");
  buttonVal = analogRead(analogInPin);
  Packet *incomingPacket = receivePacket();

  switch (ps)
  {
  case NOT_CONNECTED:
    if (incomingPacket != NULL && incomingPacket->command == addPlayer && incomingPacket->senderID == 0)
    {
      ps = CONNECTED_AND_WAITING;

      pNum = incomingPacket->receiverID;

      if (incomingPacket->string != NULL)
      {
        strcpy(connectedMsg, incomingPacket->string);
      }

      tone(3, 800, 200);

      delay(2000);
    }
    else if (isSomeBtnClicked(prevButtonVal, buttonVal))
    {
      if (IsButtonPressed(buttonVal, SELECT))
      {
        AutoPrint(&lcd, "Requesting to Connect...");
        sendPacket(pNum, 0, addPlayer, NULL, NULL, NULL);
      }
    }
    break;
  case CONNECTED_AND_WAITING:
    SwapMessages(&lcd, millis(), connectedMsg, waitGameStartMsg, 3000);

    if (receivePacket()->command == startGame)
    {
      AutoPrint(&lcd, "Game Starting!");
      tone(3, 1000, 200);

      delay(2000);
      ps = IN_GAME;
    }
    break;
  case IN_GAME:

    break;

  case GAME_OVER:

    break;
  default:
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