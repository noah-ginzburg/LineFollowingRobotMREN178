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
#include "discard.h"

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

const int NUM_STARTING_CARDS = 5;

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
// Player P_NULL;
// Card C_NULL;

Player players[MAX_PLAYERS]; // Array of players
int numPlayers = 0;
int currentPlayer = 1;
extern SoftwareSerial HC12; // HC-12 Wireless Communication

Deck deck; // This is the deck of cards that will be used in the game
Discard discard;

//////////////////////////////////////////////////

void ResetGame()
{
  // Initialize everything here again, should reset every single variable
}

bool CheckIsCardValid(Card playCard, Card topCard)
{
  // If the play card is an Eight, it's always valid
  if (playCard.rank == Eight)
    return true;

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

void setup()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
  HC12.begin(9600);

  //////////////////////////////////////////////////////
  gs = PLAYERS_WAIT;
  // C_NULL.rank = A;
  // C_NULL.suit = clubs;

  // for (int i = 0; i < MAX_CARDS; i++)
  // {
  //   P_NULL.hand[i] = C_NULL;
  // }

  // P_NULL.playerNum = -1;
  // P_NULL.handSize = -1;

  // for (int i = 0; i < MAX_PLAYERS; i++)
  // {
  //   players[i] = P_NULL;
  // }

  initDeck(&deck);
  initDeck(&discard); // Visakan made an overloaded funciton, dont worry. the compiler knows which function to choose based on the input passed
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
    bool shouldStartGame = false;

    // Function to wait on player connect (use a while loop, detect when select is presesd to start the game)
    do
    {
      buttonVal = analogRead(analogInPin);
      // If Received new player from anothre arduino (serial.available)
      // Detect incoming connection from player, check if command packet says connect
      AutoPrint(&lcd, "Waiting on Player Connect", 1);

      Packet *pkt = receivePacket();

      if (pkt != NULL && numPlayers <= MAX_PLAYERS) // if master received a packet and the command is to add a new player
      {
        if (pkt->command == addPlayer)
        {
          //
          int curPlayer = numPlayers; // IF we cant reference the second player, adjust this line
          // Serial.print("Current Player Index: ");
          // Serial.println(curPlayer);

          numPlayers++;
          // Serial.print("Num players: ");
          // Serial.println(numPlayers);

          // Initialize the player
          players[curPlayer].handSize = 0;
          players[curPlayer].playerNum = numPlayers; // Autoincrements the player number

          // Print msg to screen
          AutoPrint(&lcd, "Player ", 1);
          AutoPrint(&lcd, players[curPlayer].playerNum, 0);
          ManualPrint(&lcd, "is Connected!", 1, 0, 0);

          // Serial.println(players[curPlayer].playerNum); // this works

          // Format string to display
          char playerMsg[MAX_STR_SIZE] = "You are Player #";
          char pNumStr[2];
          sprintf(pNumStr, "%d", players[curPlayer].playerNum);
          strcat(playerMsg, pNumStr);

          int connectID = pkt->senderID; // Get the ID of the player that connected

          // // Send back player number to the player to connect them
          sendPacket(0, connectID, addPlayer, &players[curPlayer], playerMsg);

          // Serial.println("Master sent packet back to player");

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

    break;

  case START_GAME:
    AutoPrint(&lcd, "Game Starting...", 1);
    sendPacket(0, -1, startGame, NULL, NULL);
    delay(5000);

    currentPlayer = 0;

    for (int i = 0; i < NUM_STARTING_CARDS; i++)
    {
      for (int j = 0; j < numPlayers; j++)
      {
        AddCardToHand(&players[j], *dequeue(&deck)); // distribute cards to all players
      }
    }

    push(dequeue(&deck), &discard); // lay down the first card

    gs = PLAYER_TURN;
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
      Packet *pkt = receivePacket();

      // formats the turn player messagee
      char msg1[MAX_STR_SIZE] = "Turn: Player #";
      char pNumStr[2];
      sprintf(pNumStr, "%d", players[currentPlayer].playerNum);
      strcat(msg1, pNumStr);

      // Check top discard pile
      Card *topCard = top(&discard);

      if (topCard == NULL) // check to make sure top card isnt null
      {
        AutoPrint(&lcd, "ERR: top of discard NULL");
        delay(5000);
        break;
      }

      // char tempSuitChar = 'a'; //the custonm caharater for the suits

      // Next whole code block is to format second message on master
      char msg2[MAX_STR_SIZE] = "Top Card:"; // Message formatting
      char pNumStr[2];

      if (topCard->rank < Ten && topCard->rank != A)
      {
        sprintf(pNumStr, "%d", topCard->rank);
        strcat(msg2, pNumStr);
      }
      else
      {
        switch (topCard->rank)
        {
        case A:
          strcat(msg2, "A");
          break;
        case Jack:
          strcat(msg2, "J");
          break;
        case Queen:
          strcat(msg2, "Q");
          break;
        case King:
          strcat(msg2, "K");
          break;
        }
      } // all to do with the message formatting

      // Displays the messages
      AutoPrint(&lcd, msg1);
      ManualPrint(&lcd, msg2, 1, 0, 0);
      // Manual.print &lcs, custom character, figure out spacing

      // Wait on player option
      if (pkt != NULL)
      {
        // Determine player's requirested action
        switch (pkt->command)
        {
        case playCard:

          // deciher the card that was requested to be played
          Card playCard = getCardFromStr(pkt->string);

          // Validate the card
          if (&playCard == NULL)
          {
            AutoPrint(&lcd, "ERR: play card NULL");
            delay(5000);
          }
          else
          {
            if (CheckIsCardValid(playCard, *topCard)) //, determine validity of the card played
            {
              if (deleteCard(&players[currentPlayer], playCard)) // Delete card from user inventory and check if its valkid
              {
                // Adds the card to the discard pile
                push(&playCard, &discard);

                // Checks if it is an 8
                if (playCard.rank == Eight)
                {
                  // While loop
                  // Stores an extra packet to swtich between suits
                  Packet *extraPkt = (Packet *)malloc(sizeof(Packet));

                  // Detects the incoming packet
                  if (extraPkt != NULL)
                  {
                    // Prompt user

                    // whilke packet
                    // Change suit
                  }
                  else
                  {
                    // Print error
                    AutoPrint(&lcd, "ERR: Mem pkt 8s alloc failed");
                    delay(5000);
                  }
                }
              }
              else
              {
                // Prit error
                AutoPrint(&lcd, "Card Delete Failed, Skipping Turn");
                delay(5000);
              }
            }
            else
            {
              // Send invalid packet
            }
          }

          break;

        case drawCard:

          // Check to make sure the player hand does not exceed the max size
          if (players[currentPlayer].handSize < MAX_HAND_SIZE)
          {
            // Peek at the top card to make sure one exists
            Card *drawnCard = peek(&deck);

            // checks if the card is not null
            if (drawnCard == NULL)
            {
              // Reshuffle the deck to ensure a card exists
              ReshuffleDeckWithDiscard(deck, discard);
            }

            // Now actually pop (remove from deck) that card and store it
            drawnCard = dequeue(&deck);

            // Add the card to the player hand
            AddCardToHand(&players[currentPlayer], *drawnCard);

            // Send feedback string
          }
          else
          {
            // Skip the turn and output messages
            AutoPrint(&lcd, "Player #");
            AutoPrint(&lcd, currentPlayer + 1, 0);
            ManualPrint(&lcd, "at max hand size", 1, 0, 0);
            delay(5000);
            AutoPrint(&lcd, "Skipping Turn...");
            delay(5000);

            // Send feedback to skip player turn
            // tell player to stop selecting stuff
          }

          break;

        default:
          // Just in case a bad command was sent
          AutoPrint(&lcd, "Player CMD OPTN INVALID");
          delay(5000);
          break; // or print invalid player option
        }
      }

      // If deck is empty, reshuffle discard, enqueue everything to deck
      if (peek(&deck) == NULL)
        ReshuffleDeckWithDiscard(deck, discard);

      // Move to the next player
      currentPlayer = (currentPlayer + 1) % numPlayers;
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
