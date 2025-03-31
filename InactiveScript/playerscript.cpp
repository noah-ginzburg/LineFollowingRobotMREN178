
////////////////////////////////
// FOR EACH PLAYER, DEFINE A RANDOM CONNECT ID
// THIS IS SO THAT THE MASTER CAN IDENTIFY THE PLAYER WHO CONNECTS
// THIS VALUES CANNOT BE BETWEEN (0-8) AS THOSE ARE PLAYER IDS. MUST BE POSITIVE
#define CONNECT_ID 1573
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
const int PL_AY = 0;
const int DR_AW = 1;
const int SHOW_CARDS = 2;
int mmo = PL_AY;
int numMMO = 3;
bool isPlayerTurn = true;
bool isInSubMenu = false;
//If card is being chosen for PLAY:
int choose = 1;
//custom characters:
byte dimon[] = { 
  B00000, 
  B00100, 
  B01110, 
  B11111, 
  B11111, 
  B01110, 
  B00100, 
  B00000 
};
byte hart[] = { 
  B00000, 
  B01010, 
  B11111, 
  B11111, 
  B01110, 
  B00100, 
  B00000, 
  B00000 
};
byte speds[] = { 
  B00000, 
  B00100, 
  B01110, 
  B11111, 
  B10101, 
  B00100, 
  B01110, 
  B00000 
};
byte klubz[] = { 
  B00000, 
  B01110, 
  B01110, 
  B11111, 
  B11111, 
  B00100, 
  B01110, 
  B0000 
};
Player P_NULL;
Card C_NULL;
PlayerState ps = IN_GAME;
int pNum = -1;
extern SoftwareSerial HC12;

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
  lcd.createChar(0, dimon);
  lcd.createChar(1, hart);
  lcd.createChar(2, speds);
  lcd.createChar(3, klubz);
  C_NULL.rank = A;
  C_NULL.suit = clubs;
  for (int i = 0; i < MAX_CARDS; i++)
  {
    P_NULL.hand[i] = C_NULL;
  }
  P_NULL.playerNum = -1;
  P_NULL.handSize = -1;
}
// initializes loop
void loop() {
  buttonVal = analogRead(analogInPin);
  Packet *incomingPacket = receivePacket();
  switch (ps) {
    case NOT_CONNECTED:
      if (incomingPacket != NULL) {
        Serial.println("Packet is not null");
        Serial.println(incomingPacket->senderID);
        Serial.println(incomingPacket->receiverID);
        Serial.println(incomingPacket->player.playerNum);
      }
      if (incomingPacket != NULL && incomingPacket->command == addPlayer && incomingPacket->senderID == 0 && incomingPacket->receiverID == CONNECT_ID) {
        ps = CONNECTED_AND_WAITING;
        if (incomingPacket->player.playerNum < 1 && incomingPacket->player.playerNum > 8) {
          Serial.println("Player is NULL");
        } else {
          Serial.println("Player is NOT NULL");
        }
        pNum = incomingPacket->player.playerNum;
        if (incomingPacket->string != NULL) {
          strcpy(connectedMsg, incomingPacket->string);
        }
        Serial.println("pnum: ");
        Serial.println(pNum);
        tone(3, 800, 200);
        delay(2000);
      } else if (isSomeBtnClicked(prevButtonVal, buttonVal)) {
        if (IsButtonPressed(buttonVal, SELECT)) {
          AutoPrint(&lcd, "Requesting to Connect...");
          sendPacket(CONNECT_ID, 0, addPlayer, &P_NULL, NULL);
        }
      }
      break;
    case CONNECTED_AND_WAITING:
      SwapMessages(&lcd, millis(), connectedMsg, waitGameStartMsg, 3000);
      if (incomingPacket != NULL && incomingPacket->command == startGame) {
        AutoPrint(&lcd, "Game Starting!");
        tone(3, 1000, 200);
        delay(5000);
        ps = IN_GAME;
      }
      break;
    case IN_GAME:
    
    Packet *pkt = receivePacket();
    if (pkt != NULL)
    {
      //if (pkt->command = StartPlayerTurn)
      //{ 
      isPlayerTurn = true;
      //}
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
          //play main menu
          AutoPrint(&lcd, "PLAY");
        }
        else
        {
          AutoPrint(&lcd, "CHOOSE CARD");
          lcd.setCursor(0,1);
          if (choose == 1) lcd.write(byte(0));
          if (choose == 2) lcd.write(byte(1));
          if (choose == 3) lcd.write(byte(2));
          if (choose == 4) lcd.write(byte(3));
          if (choose == 5) choose = 1;
          if (choose == 0) choose = 4;
          //fix button logic in play case
          if (isSomeBtnClicked(prevButtonVal, buttonVal))
      {
        if (IsButtonPressed(buttonVal, RIGHT)) {
          choose++;
        }
      else if (IsButtonPressed(buttonVal, LEFT)) {
        choose--;
      }
      else if (IsButtonPressed(buttonVal, SELECT) && (choose > 0 && choose < 5)) {
        lcd.clear();
        AutoPrint(&lcd, "PLAYED!");
        delay(2000);
        lcd.clear();
        isInSubMenu = false;
        //return to main menu
         }
      }
            // choose a card
            //play it
        }
        break;
      case DR_AW:
      if (!isInSubMenu)
      {
        //print the draw cards text
        AutoPrint(&lcd, "DRAW");
      }
      else
      {
        //Send a packet - wait for this
        /*AutoPrint(&lcd, "DRAWN!");
        delay(2000);
        isInSubMenu = false;*/
      }
      break;
      case SHOW_CARDS:
      if (!isInSubMenu)
      {
        //print the show cards text
        AutoPrint(&lcd, "SHOW CARDS");
      }
      else
      {
        //print all available player cards
        //use pkt->player.values
        //hard time string concatenation
        //do this last
      }
      break;
      
      default:
        break;
      }
      if (isSomeBtnClicked(prevButtonVal, buttonVal))
      {
        Serial.println("btn clicked");
        if (IsButtonPressed(buttonVal, RIGHT)) mmo++;
      else if (IsButtonPressed(buttonVal, LEFT)) mmo--;
      else if (IsButtonPressed(buttonVal, SELECT)) isInSubMenu = true;
      }
      
      mmo = mmo % numMMO;

      prevButtonVal = buttonVal;
    } 
      /*int menu = 1;
      int tracker = 1;
      int buttonVal = analogRead(A0);
      lcd.setCursor(0,0);
      lcd.print("Action:");
      lcd.setCursor(0,1);
      if (buttonVal < RIGHT) {
        menu++;
        delay(300);
        lcd.print(menu);
      }
      if (buttonVal <= LEFT) {
        menu--;
        delay(300);
        lcd.print(menu);
      }*/
      break;
      
    case GAME_OVER:
      // Add handling for game over if necessary
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