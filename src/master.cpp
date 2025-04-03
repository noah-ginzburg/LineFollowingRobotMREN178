// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>

// Includes all other libraries
#include "interface.h"
#include "player.h"
#include "card.h"
#include "deck.h"
#include "discard.h"
#include "DataTransmission.h"

// Defines all game macros
#define MAX_PLAYERS 3

// Hello Here is an Update

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int analogInPin = A0;
int buttonVal = 0;
int prevButtonVal = 0;

const int NUM_STARTING_CARDS = 5;

/////////////////////////////////////////

// Initialize the deck of cards and the discard pile
Deck deck;
Discard discard;

typedef enum gamestate
{
  PLAYERS_WAIT,
  START_GAME,
  PLAYER_TURN,
  END_GAME
  // test
} GameState;

GameState gs = PLAYERS_WAIT;

int players[MAX_PLAYERS];
int numPlayers = 0;
int currentPlayerTurn = 0;
int prevPlayer;
Packet *pkt = NULL;

Suit topEightSuit;

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

//////////////////////////////////////////////////

void ResetGame()
{
  // Initialize everything here again, should reset every single variable
}

bool CheckIsCardValid(Card playCard, Card topCard, Suit topSuit)
{
  // If the play card is an Eight, it's always valid
  if (playCard.rank == Eight)
    return true;

  if (topCard.rank == Eight)
  {
    if (playCard.suit == topCard.suit)
      return true;
  }
  // If the rank or the suit matches, the card is valid
  if (playCard.rank == topCard.rank || playCard.suit == topCard.suit)
    return true;

  // If neither rank nor suit matches, the card is invalid
  return false;
}

void ReshuffleDeckWithDiscard(Deck deck, Discard discard)
{
  while (discard.tos != 0)
  {
    enqueue(pop(&discard), &deck);
  }

  shuffle(&deck);
}

void StopAllPlayerTurn()
{
  sendPacket(0, -1, stopPlayerTurn, NULL, NULL);
}

void displayCardLCD(Card chosenCard)
{
  if (chosenCard.rank > 1 && chosenCard.rank < 10)
  {
    lcd.write((int)(chosenCard.rank));
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
}

void loop()
{

  /*
  pkt = receivePacket();

  if (pkt != NULL){
    //Serial.println("Received Packet");
    //AutoPrint(&lcd, "Player #", 1);
    Serial.println(pkt->senderID);
    //int playernum = (char) pkt->senderID;
    char num[2];
    sprintf(num, "%d", pkt->senderID);
    char input[50] = "Player #";
    strcat(input, num);
    strcat(input, "sent a message");
    //AutoPrint(&lcd, num, 0);
    AutoPrint(&lcd, input, 0);
  }
    */

  if (gs == PLAYERS_WAIT)
  {
    bool shouldStartGame = false;

    // Function to wait on player connect (use a while loop, detect when select is presesd to start the game)
    do
    {
      buttonVal = analogRead(analogInPin);
      // If Received new player from anothre arduino (serial.available)
      // Detect incoming connection from player, check if command packet says connect
      AutoPrint(&lcd, "Waiting on Player Connect", 1);

      Packet *pkt = receivePacket();

      if (pkt != NULL) // if master received a packet and the command is to add a new player
      {
        if (pkt->command == addPlayer)
        {
          players[numPlayers] = pkt->senderID;
          numPlayers++;

          // Print msg to screen
          char lcdMsg[20] = "Player #";
          char senderIDStr[5];
          sprintf(senderIDStr, "%d", pkt->senderID); // Make the senderID a character
          strcat(lcdMsg, senderIDStr);               // add the ID to the string
          strcat(lcdMsg, " is connected!");
          AutoPrint(&lcd, lcdMsg, 1); // print the string

          // Send back player number to the player to connect them
          sendPacket(0, pkt->senderID, addPlayer, NULL, NULL);

          tone(3, 1000, 200);
          delay(3000);
          noTone(3);
        }
      }

      if (IsButtonPressed(buttonVal, SELECT))
      {
        Serial.println("Button is pressed!");
        if (numPlayers > 1)
        {
          Serial.println("Should start game is true");
          shouldStartGame = true;
        }
      }
      delay(50);
    } while (!shouldStartGame); // may have to implement button debouncing (so it only activates once), this line is just if prevbuttonval != buttonval

    gs = START_GAME;
    lcd.clear();
  }
  else if (gs == START_GAME)
  {
    AutoPrint(&lcd, "Game Starting...", 1);
    sendPacket(0, -1, startGame, NULL, NULL);
    Serial.println("PACKET SENT TO ALL");
    delay(3000);
    initDeck(&deck);
    Serial.println("deck initialized");
    initDiscard(&discard);
    Serial.println("Discard initialized");

    shuffle(&deck);
    Serial.println("Deck Shuffled");

    for (int i = 1; i <= numPlayers; i++)
    {
      Serial.print("\nDealing cards to player ");
      Serial.println(i);
      for (int j = 0; j < NUM_STARTING_CARDS; j++)
      {
        Card retrieved = dequeue(&deck);
        Serial.print(j + 1);
        Serial.print(". ");
        Serial.print((int)retrieved.rank);
        Serial.print(" of ");
        Serial.println((int)retrieved.suit);
        sendPacket(0, i, addHand, &retrieved, NULL);
        delay(200);
      }
    }

    Card firstCard = dequeue(&deck);

    push(&firstCard, &discard); // lay down the first card

    Serial.print("discard Added: Rank: ");
    Serial.print(firstCard.rank);
    Serial.print(", Suit: ");
    Serial.println(firstCard.suit);

    gs = PLAYER_TURN;
    lcd.clear();

    currentPlayerTurn = 1;

    sendPacket(0, -1, startGame, NULL, NULL);
    delay(2000);
  }
  else if (gs == PLAYER_TURN)
  {
    Packet *pkt = receivePacket();

    // Check for end game
    if (pkt != NULL && pkt->command == endTheGame)
    {
      gs = END_GAME;

      sendPacket(0, -1, endTheGame, NULL, "GAME OVER"); // ends the game
    }
    else
    {
      bool isNextPTurn = false;

      lcd.clear();
      lcd.home();
      lcd.print("Player ");
      lcd.print((int)currentPlayerTurn);
      lcd.print("'s Turn");

      Card tempTopCard = *top(&discard);

      lcd.setCursor(0, 1);
      lcd.print("Top Card: ");
      displayCardLCD(tempTopCard);

      /*Serial.print("Top card: Rank: ");
      Serial.print((*top(&discard)).rank);
      Serial.print(", Suit: ");
      Serial.println((*top(&discard)).suit);*/

      if (prevPlayer != currentPlayerTurn)
      {
        Serial.println(currentPlayerTurn);
        // Packet Sent to start player turn
        Serial.println("Sent packet to start player turn");
        sendPacket(0, currentPlayerTurn, startPlayerTurn, NULL, NULL); // Starts the player's turn
      }

      if (pkt != NULL && pkt->senderID == currentPlayerTurn)
      {
        Serial.println("Received packet from player");
        Serial.print("Command: ");
        Serial.println(pkt->command);

        switch (pkt->command)
        {
        case playCard:
          // Check top discard pile
          Card *topCard = top(&discard);
          Serial.print("Top card: ");
          Serial.print(topCard->rank);
          Serial.print(", of ");
          Serial.println(topCard->suit);

          if (topCard->rank != Eight)
            topEightSuit = topCard->suit; // Basically, this prevents the suit from changing again after the user changed the suit by playing an 8

          if (topCard == NULL) // check to make sure top card isnt null
          {
            AutoPrint(&lcd, "ERR: top of discard NULL");
            delay(5000);
            isNextPTurn = false;
          }

          Card playCard = pkt->cardSent;
          Serial.print("play card: ");
          Serial.print(playCard.rank);
          Serial.print(", of ");
          Serial.println(playCard.suit);

          // deciher the card that was requested to be played

          if (CheckIsCardValid(playCard, *topCard, topEightSuit)) //, determine validity of the card played
          {
            Serial.println("playCard was valid");
            // Adds the card to the discard pile
            push(&playCard, &discard);
            Serial.println("pushed card to discard");

            Serial.println("test");

            // Checks if it is an 8
            if (playCard.rank == Eight)
            {
              Packet *extraPacket = NULL;
              sendPacket(0, currentPlayerTurn, changeSuit, NULL, NULL);
              Serial.println("test");

              do
              {
                extraPacket = receivePacket();
                AutoPrint(&lcd, "Waiting on Suit Change");
                delay(50);
              } while (extraPacket == NULL);

              Serial.println("test");
              Card receivedCard = extraPacket->cardSent;
              topEightSuit = receivedCard.suit;

              AutoPrint(&lcd, "Switched suit to: ");
              switch (receivedCard.suit)
              {
              case clubs:
                ManualPrint(&lcd, "CLUBS", 1, 0, 0);
                break;
              case diamonds:
                ManualPrint(&lcd, "DIAMONDS", 1, 0, 0);
                break;
              case hearts:
                ManualPrint(&lcd, "HEARTS", 1, 0, 0);
                break;
              case spades:
                ManualPrint(&lcd, "SPADES", 1, 0, 0);
                break;
              }

              delay(5000);
              Serial.println("Setting next player turn to true");
              isNextPTurn = true;
            }
          }
          else
          {
            // Prit error
            AutoPrint(&lcd, "Card was invalid, try again");
            delay(5000);
            isNextPTurn = false;
            sendPacket(0, currentPlayerTurn, tryAgain, &playCard, NULL);
          }

          break;

        case drawCard:
        {
          // Peek at the top card to make sure one exists
          Card drawnCard = peek(&deck);

          // checks if the card is not null
          if (drawnCard.rank == BAD)
          {
            // Reshuffle the deck to ensure a card exists
            ReshuffleDeckWithDiscard(deck, discard);
          }

          drawnCard = dequeue(&deck);

          AutoPrint(&lcd, "You Drew: ");
          displayCardLCD(drawnCard);
          delay(5000);

          isNextPTurn = true;
        }
        break;
        }

        Card peekCard = peek(&deck);

        // If deck is empty, reshuffle discard, enqueue everything to deck
        if (peekCard.rank == BAD)
          ReshuffleDeckWithDiscard(deck, discard); // Might result in an infinite loop

        StopAllPlayerTurn();

        if (isNextPTurn)
        {
          Serial.println("isNext Player turn");
          // Move to the next player
          currentPlayerTurn++;
          if (currentPlayerTurn > numPlayers)
            currentPlayerTurn = 1;
        }
      }

      // Serial.println(prevPlayer);
      // Serial.println(currentPlayerTurn);
      prevPlayer = currentPlayerTurn;
    }
  }

  delay(100);
}

// Master goes "waiting for players"

// Players click select to join, sends a command to add them as players

// Master receives commands, needs to know who the players are (not assigning them a number), how many are connected
// Master initializes deck, discard pile and shuffles deck.
// Master send ten cards to each player

// players store cards (check how much memory it takes)

// THE GAME:

// Master filters through players one at a time

// Player 1 begins:
// Master displays top card on its LCD
// Player has 3 options: play, draw, show hand
// Players 2, 3 have message told to wait
// Player chooses an option:
// Show cards: shows hand, double blinking cursor with known index, select knows which index was clicked
// Play: clear LCD, send card to master, player does not delete yet
// Master checks if the card can be played: (A)
// Yes: nothing on player side, Master updates top card on discard pile
// No: Master sends "bad card" command and player LCD says to pick a valid card
// If the player just played its last card, game ends and player wins - player sends "I win!"
// Draw: adds card to LCD unless hand is 10 cards - then player sends a "my hand is full and cant do anything" command - master skips turn
// Master updates top card on its LCD

// Next player plays

// Players keep playing until:
// A player has zero cards () Display "I win"

// Game Logic (A)
// Compares the card recieved to card at the top of the discard pile
// If suit or rank matches, card is played and player is told to discard the card
// If the card played is an Eight, card is played and player is told to change the suit
// Players selects the suit, needs to display
// and delete the card
// We need a global variable to keep track of the current suit, keep track of rank

// buttonVal = analogRead(analogInPin);

// SwapMessages(&lcd, millis(), "Message1", "Message2", 3000);

// switch (gs)
// {
// case INITIALIZE:
//   ResetGame();
//   break;
// case PLAYERS_WAIT:
// }

// Function to wait on player connect (use a while loop, detect when select is presesd to start the game)
// do
//{
// If Received new player from anothre arduino (serial.available)
// Detect incoming connection from player, check if command packet says connect

/*
if (pkt != NULL) // if master received a packet and the command is to add a new player
{
if (pkt->command == addPlayer && pkt->senderID == -1)
{
  if (pkt->command == addPlayer && pkt->senderID == -1)
  {
    Player *newPlayer = (player *)malloc(sizeof(Player));
    // initPlayer(newPlayer);

  newPlayer->playerNum = ++numPlayers; // Autoincrements the player number

  players[numPlayers] = *newPlayer; // Stores the new player

  AutoPrint(&lcd, "Player ", 1);
  AutoPrint(&lcd, newPlayer->playerNum, 0);
  AutoPrint(&lcd, "is Connected!", 0);

  char *playerMsg = "You are Player #";
  char *pNumStr;
  sprintf(pNumStr, "%d", newPlayer->playerNum);
  strcpy(playerMsg, pNumStr);

    // Send back player number to the player
    sendPacket(0, newPlayer->playerNum, addPlayer, NULL, playerMsg);

    tone(3, 1000, 200);
    noTone(3);
    delay(5000);
  }
}
}

//} while (numPlayers < MAX_PLAYERS || (numPlayers > 1 && !IsButtonPressed(buttonVal, SELECT))); // may have to implement button debouncing (so it only activates once), this line is just if prevbuttonval != buttonval

realloc(players, numPlayers * sizeof(Player)); // Check if this line actually works

// else
// start game by setting gs to PlayerTurn
currentPlayerTurn = 0;
break;

case START_GAME:

break;

case PLAYER_TURN:

// Check end game condition
int isGameOver = 0;

for (int i = 0; i < numPlayers; i++)
{
if (players[i].curNumCards == 0)
  isGameOver = 1;
}

if (isGameOver == 1)
gs = END_GAME;
else
{

// Wait on player option
PO playerOption = PLAY; // for now, use commandpacket later
ReadPlayerOption(playerOption);

// If deck is empty, reshuffle discard, enqueue everything to deck

currentPlayerTurn++;
if (currentPlayerTurn > numPlayers)
  currentPlayerTurn = 0;
}

break;

case END_GAME:

int winner = 0;

for (int i = 0; i < numPlayers; i++)
{
if (players[i].curNumCards == 0)
  winner = i;
}

winner++;

// Display Message "PLAYER" winner "WINS! PRess select to restart the game"

// Detect detect select button press
// gs = INITIALIZE;

break;
default:
break;
}

prevButtonVal = buttonVal;

delay(50);


*/
