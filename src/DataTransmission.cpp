/******************************************************************************
FILENAME:     DataTransmission.cpp
AUTHOR:       Lucas Dejmek
DATE:         20/03/2025

ATTRIBUTIONS: MREN178 Course Content, GitHub Copilot
******************************************************************************/

#define BUZZER_PIN 3 // Pin for the buzzer

#include "DataTransmission.h"

// LCD Keypad Shield Pins
// LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// HC-12 Wireless Communication
SoftwareSerial HC12(10, 11);

// Change this line for every node. NOTE: MASTER IS NODE ZERO (0). ALL PLAYERS ARE NODES 1-7.
// WARNING!!! THIS IS NOT PLAYER_ID! NODE_ID IS THE ID OF THE NODE THE CODE IS RUNNING ON, WHEREAS PLAYER_ID IS THE ID OF THE PLAYER GETTING SENT DATA

//// TEST VALUES TO SEE IF PROGRAM WORKS

// Card* testcard;
// Card testCard = {Five, hearts}; // Create a Card object
// Card* testCardPtr = &testCard;     // Assign its address to the pointer
// char string[50] = "Message Sent Sucessfully!";
// String teststring;

//// END OF TEST VALUES

Packet receivedPkt;

int sendPacket(int senderID, int receiverID, Cmd command, Player *player, char *string)
{
    Packet *pkt = (Packet *)malloc(sizeof(Packet)); // Initialize a packet
    if (pkt == NULL)
    {
        Serial.println("FATAL ERROR: PACKET MEM ALLOCATION FAILED");
    }

    pkt->senderID = senderID;     // Set the sender ID as that node's ID - CHANGE DEPENDING ON WHICH ARDUINO THE CODE IS RUNNING ON
    pkt->receiverID = receiverID; // Set the variables of the packet to what was passed into the function
    pkt->command = command;

    pkt->player = *player; // Set the player to the player passed in

    strncpy(pkt->string, string, sizeof(pkt->string) - 1);
    pkt->string[sizeof(pkt->string) - 1] = '\0'; // Null terminate

    // Serial.print("Sending Packet with String: ");
    // Serial.println(pkt->string);

    HC12.write((uint8_t *)pkt, sizeof(Packet)); // Create a pointer to the packet and translate it into bits of data. Then send bits over the HC12

    Serial.println("Packet Sent: ");
    free(pkt); // Free the memory used by malloc after sending message

    return 1; // Packet sent sucessfully

    ////

    // Attention Lucas and Visakan, please note after 10 total hours of working with this
    // Dumb wireless tranceiver, i have decided to send cards via a string since thats the way
    // I got it to work. I know, it's ass, but it works so we roll with it

    ///
}

Packet *receivePacket()
{
    while (HC12.available())
    {                                                              // Make sure to receive the whole packet >= sizeof(pkt)
        HC12.readBytes((char *)&receivedPkt, sizeof(receivedPkt)); // Read the whole packet back into the struct
        // Serial.println("SenderID: ");
        // Serial.println(receivedPkt.senderID);
        // Serial.println("ReceiverID: ");
        // Serial.println(receivedPkt.receiverID);
        Serial.println("Packet Received");
        return &receivedPkt; // Return 1: Data Received
    }
    return NULL; // Return 0: Nothing received
}

char *getStrFromCard(Card card)
{
    static char str[5]; // Large enough to hold both rank and suit

    char str1[3]; // Buffer for rank (max 2 digits + null terminator)
    char str2[2]; // Buffer for suit (max 1 digit + null terminator)

    sprintf(str1, "%d", card.rank); // Convert rank to string
    sprintf(str2, "%d", card.suit); // Convert suit to string

    strcpy(str, str1); // Copy the rank into str
    strcat(str, str2); // Append the suit to str

    return str; // Return the concatenated result
}

Card getCardFromStr(char *str)
{
    Card card;
    Rank rank;
    Suit suit;

    int r = atoi(&str[0]);
    int s = atoi(&str[1]);

    switch (r)
    {
    case 0:
        rank = A;
        break;
    case 1:
        rank = Two;
        break;
    case 2:
        rank = Three;
        break;
    case 3:
        rank = Four;
        break;
    case 4:
        rank = Five;
        break;
    case 5:
        rank = Six;
        break;
    case 6:
        rank = Seven;
        break;
    case 7:
        rank = Eight;
        break;
    case 8:
        rank = Nine;
        break;
    case 9:
        rank = Ten;
        break;
    case 10:
        rank = Jack;
        break;
    case 11:
        rank = Queen;
        break;
    case 12:
        rank = King;
        break;
    default:
        rank = A;
        break; // Default to Ace if invalid
    }

    switch (s)
    {
    case 0:
        suit = clubs;
        break;
    case 1:
        suit = diamonds;
        break;
    case 2:
        suit = hearts;
        break;
    case 3:
        suit = spades;
        break;
    default:
        suit = spades;
        break; // Default to Hearts if invalid
    }

    card.rank = rank;
    card.suit = suit;

    return card;
}

// void setup()
// {
//     Serial.begin(9600);
//     HC12.begin(9600);
//     pinMode(BUZZER_PIN, OUTPUT);
// }

// void loop()
// { // Don't use, just call the send and receive functions in your code and pass by reference what you want

//     // Transmitting Function

//     // char* string = "test string abc";
//     // sendPacket(0, 1, addPlayer, NULL, NULL, string);
//     // delay(5000); //5 second delay before next send

//     // Receiving Function
//     Packet *pkt = receivePacket();
//     if (pkt != NULL)
//     {
//         Serial.print("String: ");
//         Serial.println(pkt->string);
//     }
// }
