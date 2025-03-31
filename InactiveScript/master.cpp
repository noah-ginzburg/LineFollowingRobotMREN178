// include the library code:
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>
#include <stdlib.h>

// Includes all other libraries
#include "interface.h"
#include "player.h"
#include "card.h"
#include "DataTransmission.h"
#include "deck.h"

// Defines all game macros
#define MAX_PLAYERS 3
#define MAX_STR_SIZE 33

// Hello Here is an Update

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int analogInPin = A0;
int buttonVal = 0;
int prevButtonVal = 0;

/////////////////////////////////////////
typedef enum gamestate
{
  INITIALIZE,
  PLAYERS_WAIT,
  START_GAME,
  PLAYER_TURN,
  END_GAME,
  test
} GameState;

GameState gs = PLAYERS_WAIT;

Player players[MAX_PLAYERS];
int numPlayers = 0;
int currentPlayer = 1;
extern SoftwareSerial HC12; // HC-12 Wireless Communication

bool shouldStartGame = false;

Deck *deck; // This is the deck of cards that will be used in the game

//////////////////////////////////////////////////

void ResetGame()
{
  // Initialize everything here again, should reset every single variable
}

/*void ReadPlayerOption(PO po)
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
}*/

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
  HC12.begin(9600);

  //////////////////////////////////////////////////////
  gs = PLAYERS_WAIT;

  deck = (Deck *)malloc(sizeof(Deck));
}

void loop()
{

  buttonVal = analogRead(analogInPin);

  // SwapMessages(&lcd, millis(), "Message1", "Message2", 3000);

  switch (gs)
  {
  case INITIALIZE:
    ResetGame();
    break;
  case PLAYERS_WAIT:

    // Function to wait on player connect (use a while loop, detect when select is presesd to start the game)
    do
    {
      // If Received new player from anothre arduino (serial.available)
      // Detect incoming connection from player, check if command packet says connect
      AutoPrint(&lcd, "Waiting on Player Connect", 1);

      Packet *pkt = receivePacket();

      if (pkt != NULL) // if master received a packet and the command is to add a new player
      {
        if (pkt->command == addPlayer)
        {
          int curPlayer = numPlayers;

          numPlayers++;

          players[curPlayer].playerNum = numPlayers; // Autoincrements the player number
          players[curPlayer].handSize = 0;           // initialize player
                                                     // Stores the new player

          AutoPrint(&lcd, "Player ", 1);
          AutoPrint(&lcd, players[curPlayer].playerNum, 0);
          ManualPrint(&lcd, "is Connected!", 1, 0, 0);

          // Serial.println(numPlayers);
          // Serial.println(players[numPlayers].playerNum); //this works

          char playerMsg[MAX_STR_SIZE] = "You are Player #";
          char pNumStr[2];
          sprintf(pNumStr, "%d", players[curPlayer].playerNum);
          pNumStr[1] = '\0'; // Null-terminate the string
          strcat(playerMsg, pNumStr);

          int connectID = pkt->senderID;
          // Send back player number to the player
          sendPacket(0, connectID, addPlayer, &players[curPlayer], playerMsg);

          tone(3, 1000, 200);
          delay(5000);
          noTone(3);
        }
      }

      if (numPlayers > 1 && IsButtonPressed(buttonVal, SELECT)) // adjuts to not instantly start game
      {
        shouldStartGame = true;
      }
      delay(100);
    } while (!shouldStartGame); // may have to implement button debouncing (so it only activates once), this line is just if prevbuttonval != buttonval

    AutoPrint(&lcd, "Game Starting...", 1);
    sendPacket(0, -1, startGame, NULL, NULL);
    delay(2000);

    // realloc(players, numPlayers * sizeof(Player)); // Check if this line actually works

    // else
    // start game by setting gs to PlayerTurn
    currentPlayer = 0;
    gs = START_GAME;
    break;

  case START_GAME:

    break;

  case PLAYER_TURN:

    // Check end game condition
    int isGameOver = 0;

    for (int i = 0; i < numPlayers; i++)
    {
      if (players[i].handSize == 0)
        isGameOver = 1;
    }

    if (isGameOver == 1)
      gs = END_GAME;
    else
    {

      // Wait on player option
      /*PO playerOption = PLAY; // for now, use commandpacket later
      ReadPlayerOption(playerOption);

      // If deck is empty, reshuffle discard, enqueue everything to deck*/

      currentPlayer++;
      if (currentPlayer > numPlayers)
        currentPlayer = 0; // do better wraparound
    }

    break;

  case END_GAME:

    int winner = 0;

    for (int i = 0; i < numPlayers; i++)
    {
      if (players[i].handSize == 0)
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
}
