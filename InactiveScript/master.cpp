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
#define MAX_PLAYERS 8

// Hello Here is an Update

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int analogInPin = A0;
int buttonVal = 0;
int prevButtonVal = 0;

const int NUM_STARTING_CARDS = 5;

/////////////////////////////////////////

//Initialize the deck of cards and the discard pile
Deck deck;
Discard discard;


typedef enum gamestate
{
  PLAYERS_WAIT,
  START_GAME,
  PLAYER_TURN,
  END_GAME
  //test
} GameState;

GameState gs = PLAYERS_WAIT;

int players[MAX_PLAYERS];
int numPlayers = 0;
int currentPlayerTurn = 0;
Packet *pkt = NULL;

//////////////////////////////////////////////////

void ResetGame()
{
  // Initialize everything here again, should reset every single variable
}

void ReadPlayerOption(PO po)
{
  switch (po)
  {
  case PLAY:

    // Check top discard pile, determine validity of the card played
    // If the played card is valid, add the card to the discard pile
    // HARD PART: also send an instruction to the player script to play their card
    // This is hard because of potential card mismatching
    // If the card chosen is invalid, throw an error

    // Handle Special Case for eights (change the suit)

    break;

  case DRAW:

    // Send the top card from the draw pile to the player
    // HARD PART: also send an instruction to the player script to receive (draw) a card
    // This is hard because of potential card mismatching

    break;

  default:
    break; // or print invalid player option
  }
}

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  HC12.begin(9600);
  // initialize the serial communications:
  Serial.begin(9600);
}

void loop() {
  
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

  if (gs == PLAYERS_WAIT){
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
          sprintf(senderIDStr, "%d", pkt->senderID); //Make the senderID a character
          strcat(lcdMsg, senderIDStr); //add the ID to the string
          strcat(lcdMsg, " is connected!");
          AutoPrint(&lcd, lcdMsg, 1); //print the string

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
          shouldStartGame = true;
        }
      }
      delay(50);
    } while (!shouldStartGame); // may have to implement button debouncing (so it only activates once), this line is just if prevbuttonval != buttonval

    gs = START_GAME;
    lcd.clear();

  }

  else if (gs == START_GAME){
    AutoPrint(&lcd, "Game Starting...", 1);
    sendPacket(0, -1, startGame, NULL, NULL);
    delay(3000);
    initDeck(&deck);
    initDiscard(&discard);
    shuffle(&deck);

    for (int i= 0; i < numPlayers; i++){
      Serial.print("\nDealing cards to player ");
      Serial.println(i + 1);
      for (int j = 0; j < NUM_STARTING_CARDS; j++){
        Card* retrieved = dequeue(&deck);
        if (retrieved != NULL){
          Serial.print(j + 1);
          Serial.print(". ");
          Serial.print((int)retrieved->rank);
          Serial.print(" of ");
          Serial.println((int)retrieved->suit);
        }
        sendPacket(0, players[i], addHand, retrieved, NULL);
        delay(1000);
      }
    }
    push(dequeue(&deck), &discard); // lay down the first card
    gs = PLAYER_TURN;
    lcd.clear();
    AutoPrint(&lcd, "Player 1's Turn", 1);
  }

  else if (gs == PLAYER_TURN){
    return;

  }
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


  //Game Logic (A)
  //Compares the card recieved to card at the top of the discard pile
  //If suit or rank matches, card is played and player is told to discard the card
  //If the card played is an Eight, card is played and player is told to change the suit
  //Players selects the suit, needs to display
  //and delete the card
  //We need a global variable to keep track of the current suit, keep track of rank







  









  //buttonVal = analogRead(analogInPin);

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
    currentPlayer = 0;
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

      currentPlayer++;
      if (currentPlayer > numPlayers)
        currentPlayer = 0;
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
