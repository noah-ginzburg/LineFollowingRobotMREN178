#ifndef DATATRANSMISSION_H
#define DATATRANSMISSION_H

// Arduino Standard Libraries
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Custom Libraries
#include <card.h>
#include <player.h>

// C Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// LCD Keypad Shield Pins
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// #define NODE_ID 0 //Change this line for every node. NOTE: MASTER IS NODE ZERO (0). ALL PLAYERS ARE NODES 1-7.
//  WARNING!!! THIS IS NOT PLAYER_ID! NODE_ID IS THE ID OF THE NODE THE CODE IS RUNNING ON, WHEREAS PLAYER_ID IS THE ID OF THE PLAYER GETTING SENT DATA

// #define NUM_NODES 4
// #define INF 9999
// #define BUZZER_PIN 3

#define BTN_RIGHT 0    // Analog Output Value for Right button
#define BTN_LEFT 410   // Analog Output Value for Left button
#define BTN_UP 99      // Analog Output Value for Up button
#define BTN_DOWN 255   // Analog Output Value for Down button
#define BTN_SELECT 640 // Analog Output Value for Select button
#define BTN_NONE 1023  // No button pressed

typedef enum commands
{
    addPlayer = 0,
    startGame,
    startPlayerTurn,
    stopPlayerTurn,
    retryTurn,
    displayText,
    changeSuit,
    endTheGame,
    playCard,
    drawCard,
} Cmd; // Commands

typedef struct packet
{
    int senderID;   // ID of who the packet is getting sent from
    int receiverID; // ID of who the packet is getting sent to
    Cmd command;
    // Card* cardsDealt[10]; //Use this when the master sends the client their initial or updated hand every round
    Player player;   // Use this when the master sends the client their initial or updated hand every round
    char string[33]; // String the master to send to the players - CHANGE LENGTH DEPENDING ON WHAT YOU WANT
} Packet;

int sendPacket(int senderID, int receiverID, Cmd command, Player *player, char *string);
Packet *receivePacket();
char *getStrFromCard(Card card);
Card getCardFromStr(char *str);

#endif