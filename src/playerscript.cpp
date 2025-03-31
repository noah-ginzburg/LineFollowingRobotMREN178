////////////////////////////////

// FOR EACH PLAYER, DEFINE A RANDOM CONNECT ID
// THIS IS SO THAT THE MASTER CAN IDENTIFY THE PLAYER WHO CONNECTS
// THIS VALUES CANNOT BE BETWEEN (0-8) AS THOSE ARE PLAYER IDS. MUST BE POSITIVE
#define CONNECT_ID 1222

/////////////////////////////

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

Player P_NULL; // DONT NEED
Card C_NULL;   // DONT NEED, CAN DELETE THESE TWO

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

  C_NULL.rank = A;
  C_NULL.suit = clubs;

  for (int i = 0; i < MAX_CARDS; i++)
  {
    // P_NULL.hand[i] = C_NULL;
  }

  P_NULL.playerNum = -1;
  P_NULL.handSize = -1;
}

// initializes loop
void loop()
{
  buttonVal = analogRead(analogInPin);
  Packet *incomingPacket = receivePacket();

  switch (ps)
  {
  case NOT_CONNECTED:
    if (incomingPacket != NULL)
    {
      Serial.println("Packet is not null");
      Serial.println(incomingPacket->senderID);
      Serial.println(incomingPacket->receiverID);
      Serial.println(incomingPacket->player.playerNum);
    }

    if (incomingPacket != NULL && incomingPacket->command == addPlayer && incomingPacket->senderID == 0 && incomingPacket->receiverID == CONNECT_ID)
    {
      ps = CONNECTED_AND_WAITING;

      if (incomingPacket->player.playerNum < 1 && incomingPacket->player.playerNum > 8)
      {
        Serial.println("Player is NULL");
      }
      else
      {
        Serial.println("Player is NOT NULL");
      }

      pNum = incomingPacket->player.playerNum;

      if (incomingPacket->string != NULL)
      {
        strcpy(connectedMsg, incomingPacket->string);
      }

      Serial.println("pnum: ");
      Serial.println(pNum);

      tone(3, 800, 200);
      delay(2000);
    }
    else if (isSomeBtnClicked(prevButtonVal, buttonVal))
    {
      if (IsButtonPressed(buttonVal, SELECT))
      {
        AutoPrint(&lcd, "Requesting to Connect...");
        sendPacket(CONNECT_ID, 0, addPlayer, &P_NULL, NULL);
      }
    }
    break;

  case CONNECTED_AND_WAITING:
    SwapMessages(&lcd, millis(), connectedMsg, waitGameStartMsg, 3000);

    if (incomingPacket != NULL && incomingPacket->command == startGame)
    {
      AutoPrint(&lcd, "Game Starting!");
      tone(3, 1000, 200);
      delay(5000);
      ps = IN_GAME;
    }
    break;

  case IN_GAME:
    // Move menu & tracker outside switch-case to maintain their values
    int menu = 1;
    int tracker = 1;

    int x = analogRead(0);
    lcd.setCursor(0, 0);
    lcd.print("Action:");
    lcd.setCursor(0, 1);

    // Main menu navigation
    if (menu != 5)
    {
      if (x < RIGHT || x < UP)
      {
        menu++;
        delay(200);
        while (analogRead(0) < 1000)
          ;
      }
      if (x <= LEFT || x <= DOWN)
      {
        menu--;
        delay(200);
        while (analogRead(0) < 1000)
          ;
      }
    }

    // Handle submenu selections
    if (menu == 1 && x <= SELECT)
    {
      menu = 5;
      delay(200);
      while (analogRead(0) < 1000)
        ;
    }
    if (menu == 2 && x <= SELECT)
    {
      menu = 6;
      delay(200);
      while (analogRead(0) < 1000)
        ;
    }
    if (menu == 3 && x <= SELECT)
    {
      menu = 7;
      delay(200);
      while (analogRead(0) < 1000)
        ;
    }

    switch (menu)
    {
    case 1:
      lcd.print("Play ");
      break;
    case 2:
      lcd.print("Draw ");
      break;
    case 3:
      lcd.print("Show ");
      break;
    case 0:
      menu = 3;
      break;
    case 4:
      menu = 1;
      break;
    case 5: // Play card selection
      lcd.setCursor(0, 0);
      lcd.print("Play Card");
      lcd.setCursor(0, 1);
      lcd.print(tracker);
      lcd.print("         ");

      if (x < RIGHT)
      {
        tracker++;
        delay(200);
        while (analogRead(0) < 1000)
          ;
      }
      if (x <= LEFT)
      {
        tracker--;
        delay(200);
        while (analogRead(0) < 1000)
          ;
      }

      if (x <= SELECT)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Played!");
        delay(2000);
        lcd.clear();
        menu = 1;
      }
      break;
    case 6:
      lcd.setCursor(0, 0);
      lcd.print("Card Drawn!");
      delay(2000);
      lcd.clear();
      menu = 2;
      break;
    case 7:
      lcd.setCursor(0, 0);
      lcd.print("Shown Cards:");
      delay(2000);
      lcd.clear();
      menu = 3;
      break;
    }
    break;

  case GAME_OVER:
    // Add handling for game over if necessary
    break;
  }

  prevButtonVal = buttonVal;
  delay(100);
}
