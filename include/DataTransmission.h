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

// HC-12 Wireless Communication
extern SoftwareSerial HC12;

#define BTN_RIGHT 0    // Analog Output Value for Right button
#define BTN_LEFT 410   // Analog Output Value for Left button
#define BTN_UP 99      // Analog Output Value for Up button
#define BTN_DOWN 255   // Analog Output Value for Down button
#define BTN_SELECT 640 // Analog Output Value for Select button
#define BTN_NONE 1023  // No button pressed

typedef enum commands
{
    playCard = 0,
    drawCard,
    addPlayer,
    addHand,
    showCards,
    startGame,
    startPlayerTurn,
    stopPlayerTurn,
    tryAgain,
    changeSuit,
    endTheGame
} Cmd; // Commands

typedef struct packet
{
    int senderID;   // ID of who the packet is getting sent from
    int receiverID; // ID of who the packet is getting sent to
    Cmd command;
    Card cardSent;   // Use this when the player sends the master which card to play
    char string[50]; // String the master to send to the players - CHANGE LENGTH DEPENDING ON WHAT YOU WANT
} Packet;            //

int sendPacket(int senderID, int receiverID, Cmd command, Card *cardSent, char string[]);
Packet *receivePacket();

#endif