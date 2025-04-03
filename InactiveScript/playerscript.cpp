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

const int NODE_ID = 2; // Player Node ID (sender ID)

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int analogInPin = A0;
int buttonVal = 0;
int prevButtonVal = 0;

extern SoftwareSerial HC12;

// initialize menu
int menu = 1;

const int PL_AY = 0;
const int DR_AW = 1;
const int SHOW_CARDS = 2;

int mmo = PL_AY;
int numMMO = 3;

bool isPlayerTurn = false;
bool isInSubMenu = false;

// If card is being chosen for PLAY:
int cardIdx = 0;

// custom characters:
byte dimon[] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000};
byte hart[] = {
    B00000,
    B01010,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
    B00000};
byte speds[] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B10101,
    B00100,
    B01110,
    B00000};
byte klubz[] = {
    B00000,
    B01110,
    B01110,
    B11111,
    B11111,
    B00100,
    B01110,
    B0000};

typedef enum playerstate
{
  NOT_CONNECTED,
  CONNECTED_AND_WAITING,
  GET_HAND,
  IN_GAME,
  GAME_OVER
} PlayerState;

PlayerState ps = NOT_CONNECTED;
Player player;

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  HC12.begin(9600);
  // initialize the serial communications:
  Serial.begin(9600);

  lcd.createChar(0, dimon);
  lcd.createChar(1, hart);
  lcd.createChar(2, speds);
  lcd.createChar(3, klubz);

  initPlayer(&player);
  player.playerNum = NODE_ID;

  // Testing
  /*player.handSize = 4;
  /// test values
  player.hand[0].rank = Two;
  player.hand[0].suit = clubs;
  player.hand[1].rank = Four;
  player.hand[1].suit = hearts;
  player.hand[2].rank = A;
  player.hand[2].suit = spades;
  player.hand[3].rank = Queen;
  player.hand[3].suit = diamonds;*/

  AutoPrint(&lcd, "PRESS SELECT to Connect");
}

void displayCardLCD(Card chosenCard)
{
  if (chosenCard.rank > 1 && chosenCard.rank < 10)
  {
    lcd.print((int)chosenCard.rank);
  }
  else
  {
    switch (chosenCard.rank)
    {
    case A:
      lcd.write("A");
      break;

    case Ten:
      lcd.write("T");
      break;
    case Jack:
      lcd.write("J");
      break;
    case Queen:
      lcd.write("Q");
      break;
    case King:
      lcd.write("K");
      break;
    }
  }

  switch (chosenCard.suit)
  {
  case diamonds:
    lcd.write(byte(0));
    break;
  case hearts:
    lcd.write(byte(1));
    break;
  case spades:
    lcd.write(byte(2));
    break;
  case clubs:
    lcd.write(byte(3));
    break;
  }
}

// initializes loop
void loop()
{
  // Serial.print("Player State: ");
  // Serial.println(ps);

  buttonVal = analogRead(analogInPin);
  Packet *incomingPacket = receivePacket();
  if (incomingPacket != NULL)
  {
    if (incomingPacket->receiverID != NODE_ID && incomingPacket->receiverID != -1)
    {
      incomingPacket = NULL;
    }
  }

  switch (ps)
  {
  case NOT_CONNECTED:
    if (abs(prevButtonVal - buttonVal) > 20)
    {
      if (IsButtonPressed(buttonVal, SELECT))
      {
        AutoPrint(&lcd, "Requesting to Connect...");
        sendPacket(NODE_ID, 0, addPlayer, NULL, NULL);
      }
    }

    if (incomingPacket != NULL)
    {
      if (incomingPacket->command == addPlayer)
      {
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
    if (incomingPacket != NULL && incomingPacket->receiverID == NODE_ID)
    {
      AutoPrint(&lcd, "Hello");
      if (incomingPacket->command == addHand)
      {
        Card cardReceived = incomingPacket->cardSent;
        // add to the player's hand array
        char rankPtr[2];
        char suitPtr[2];
        sprintf(rankPtr, "%d", cardReceived.rank);
        sprintf(suitPtr, "%d", cardReceived.suit);
        char card[30] = "New Card: Rank: ";
        strcat(card, rankPtr);
        strcat(card, " Suit: ");
        strcat(card, suitPtr);
        AutoPrint(&lcd, card, 1);

        //
        AddCardToHand(&player, cardReceived);
        Serial.print("Card Added: Rank: ");
        Serial.print(cardReceived.rank);
        Serial.print(", Suit: ");
        Serial.println(cardReceived.suit);
      }
    }

    if (incomingPacket != NULL && incomingPacket->command == startGame)
    {
      Serial.println("switching into game");
      ps = IN_GAME;
    }
    break;

  case IN_GAME:

    // Serial.println("test in game");

    if (incomingPacket != NULL && incomingPacket->receiverID == NODE_ID && incomingPacket->senderID == 0)
    {
      Serial.print("Received personalized packet for player ");
      Serial.println(NODE_ID);

      Cmd cmd = incomingPacket->command;

      if (cmd == endTheGame)
      {
        // ps = GAME_OVER;
      }
      else if (cmd == stopPlayerTurn)
      {
        isPlayerTurn = false;
        Serial.println("Should stop player trurn");
      }
      else if (cmd == startPlayerTurn)
      {
        isPlayerTurn = true;
        Serial.println("Should start player turn");
      }
    }

    if (isPlayerTurn)
    {
      buttonVal = analogRead(analogInPin);

      switch (mmo)
      {
      case PL_AY:
        /* code to print text*/
        if (!isInSubMenu)
        {
          // play main menu
          AutoPrint(&lcd, "PLAY");
        }
        else
        {
          AutoPrint(&lcd, "CHOOSE CARD");

          Card chosenCard = player.hand[cardIdx];
          /*Serial.print("Card Index: ");
          Serial.println(cardIdx);
          Serial.print("Chosen card number: ");
          Serial.println(chosenCard.rank);
          Serial.print("Chosen card suit: ");
          Serial.println(chosenCard.suit);*/

          lcd.setCursor(0, 1);
          displayCardLCD(chosenCard);
          lcd.home();

          // fix button logic in play case
          if (abs(prevButtonVal - buttonVal) > 20)
          {
            if (IsButtonPressed(buttonVal, RIGHT))
            {
              cardIdx++;

              cardIdx = cardIdx % player.handSize;
            }
            else if (IsButtonPressed(buttonVal, LEFT))
            {
              cardIdx--;
              if (cardIdx < 0)
                cardIdx = player.handSize - 1;
            }
            else if (IsButtonPressed(buttonVal, SELECT))
            {
              lcd.clear();
              AutoPrint(&lcd, "PLAYED!");

              /*Serial.println("\nPLAYED");
              Serial.print("Chosen card number: ");
              Serial.println(chosenCard.rank);
              Serial.print("Chosen card suit: ");
              Serial.println(chosenCard.suit);*/

              delay(2000);
              lcd.clear();
              isInSubMenu = false;
              buttonVal = analogRead(analogInPin);

              sendPacket(NODE_ID, 0, playCard, &chosenCard, NULL);
              deleteCard(&player, chosenCard);
              isPlayerTurn = false;
            }
          }
        }
        break;
      case DR_AW:
        if (!isInSubMenu)
        {
          // print the draw cards text
          AutoPrint(&lcd, "DRAW");
        }
        else
        {
          // Send a packet - wait for this
          sendPacket(NODE_ID, 0, drawCard, NULL, NULL);
          Serial.println("Sent packet");
          AutoPrint(&lcd, "Waiting to receive card...");

          Packet *extraPacket = NULL;

          do
          {
            extraPacket = receivePacket();
            Serial.println("waiting on receive card");
            delay(200);
          } while (extraPacket == NULL && extraPacket->senderID != 0);

          /////////////////////////////////////////TEST
          AddCardToHand(&player, extraPacket->cardSent);

          AutoPrint(&lcd, "Card Received!");

          lcd.setCursor(0, 1);
          displayCardLCD(extraPacket->cardSent);
          lcd.home();
          delay(5000);
          isInSubMenu = false;
          buttonVal = analogRead(analogInPin);

          isPlayerTurn = false;

          ////////////////////////////////////////////////
        }
        break;
      case SHOW_CARDS:
        if (!isInSubMenu)
        {
          // print the show cards text
          AutoPrint(&lcd, "SHOW CARDS");
        }
        else
        {
          lcd.home();

          for (int i = 0; i < player.handSize; i++)
          {
            displayCardLCD(player.hand[i]);
            lcd.write(" ");

            if (i == 4)
              lcd.setCursor(0, 1);
          }

          lcd.home();

          if (abs(prevButtonVal - buttonVal) > 20 && IsButtonPressed(buttonVal, SELECT))
          {
            lcd.clear();
            AutoPrint(&lcd, "Returning to menu...");
            delay(2000);
            lcd.clear();
            isInSubMenu = false;
            buttonVal = analogRead(analogInPin);
          }
        }
        break;

      default:
        break;
      }

      if (abs(prevButtonVal - buttonVal) > 20 && !isInSubMenu)
      {
        if (IsButtonPressed(buttonVal, RIGHT))
        {
          mmo++;
          mmo = mmo % numMMO;
        }

        else if (IsButtonPressed(buttonVal, LEFT))
        {
          mmo--;
          if (mmo < 0)
          {
            mmo = numMMO - 1;
          }
        }
        else if (IsButtonPressed(buttonVal, SELECT))
        {
          cardIdx = 0;
          isInSubMenu = true;
        }
      }

      prevButtonVal = buttonVal;

      break;
    }
    else
    {
      AutoPrint(&lcd, "Waiting For Turn");
      delay(1000);
    }
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