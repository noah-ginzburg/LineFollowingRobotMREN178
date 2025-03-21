// include the library code:
#include <LiquidCrystal.h>

// Includes all other libraries
#include "player.h"
#include "card.h"
#include "interface.h"

// Defines all game macros
#define MAX_PLAYERS 8

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int analogInPin = A0;
int sensorValue = 0;
int prevSensorValue = 0;

/////////////////////////////////////////
typedef enum gamestate
{
  INITIALIZE,
  PLAYERS_WAIT,
  START_GAME,
  PLAYER_TURN,
  END_GAME
} GameState;

GameState gs;

Player *players;
int numPlayers;
int currentPlayer;

//////////////////////////////////////////////////

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
  //////////////////////////////////////////////////////

  gs = INITIALIZE;
  players = malloc(sizeof(Player) * MAX_PLAYERS);
  numPlayers = 0;

  gs = PLAYERS_WAIT
}

void loop()
{
  switch (gs)
  {

  case INITIALIZE:
    ResetGame();
    break;
  case PLAYERS_WAIT:

    // Function to wait on player connect (use a while loop, detect when select is presesd to start the game)

    // If players is not at max size

    // If Received new player from anothre arduino
    // Detect incoming connection from player

    Player newPlayer;
    newPlayer.playerNum = numPlayers + 1;

    players[numPlayers++] = newPlayer;

    // DisplayMsg("Player [num] connected");
    // delay(3000);

    realloc(players, numPlayers * sizeof(Player)); // Check if this line actually works

    // else
    // start game by setting gs to PlayerTurn
    currentPlayer = 0;
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
      PO playerOption = 0; // for now
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

    winningPlayer++;

    // Display Message "PLAYER" winner "WINS! PRess select to restart the game"

    // Detect detect select button press
    // gs = INITIALIZE;

    break;
    default break;
  }

  /*
  // when characters arrive over the serial port...
  if (Serial.available())
  {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0)
    {
      // display each character to the LCD
      char c = Serial.read();
      if (c >= 32 && c <= 126)
      {
        lcd.write(c);
      }
    }
  }

  sensorValue = analogRead(analogInPin);
  // Serial.print("sensor = ");
  Serial.println(sensorValue);
  delay(10);

  if (prevSensorValue != sensorValue)
  {
    if (sensorValue >= 0 && sensorValue <= 60)
    {
      lcd.scrollDisplayLeft();
    }
    else if (sensorValue >= 400 && sensorValue <= 600)
    {
      lcd.scrollDisplayRight();
    }
  }

  prevSensorValue = sensorValue;
  */
}

void ResetGame()
{
  // Initialize everything here again, should reset every single variable
}

void DisplayMsg(char *str)
{
  // Function to easily display a message on the LCD
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

    default break; // or print invalid player option
  }
}
