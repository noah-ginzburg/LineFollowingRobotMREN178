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
    Serial.print("Encoded String: ");
    Serial.println(pkt->string);
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
    static char str[4]; // Increased buffer size
    snprintf(str, sizeof(str), "%02d%d", card.rank, card.suit);
    return str;
}

Card getCardFromStr(char *str)
{
    static Card card;
    int r, s;

    Serial.print("Raw input string: ");
    Serial.println(str); // Debugging

    // Extract up to 2-digit rank and 1-digit suit
    if (sscanf(str, "%2d%1d", &r, &s) != 2)
    {
        Serial.println("Parsing failed!");
        card.rank = A;
        card.suit = spades;
        return card;
    }

    Serial.print("Parsed values -> Rank: ");
    Serial.print(r);
    Serial.print(", Suit: ");
    Serial.println(s);

    // **Ensure valid range**
    if (r < 1 || r > 13)
        r = A; // **King is 13, not out of range**
    if (s < 0 || s > 3)
        s = spades;

    card.rank = (Rank)r; // **Direct cast instead of switch**
    card.suit = (Suit)s;

    return card;
}

/*void setup()
{
    Serial.begin(9600);
    HC12.begin(9600);
    pinMode(BUZZER_PIN, OUTPUT);
}*/

/*void loop()
{ // Don't use, just call the send and receive functions in your code and pass by reference what you want

    // Transmitting Function

    // Card testCard;
    // testCard.rank = King;
    // testCard.suit = hearts;
    // sendPacket(0, 1, addPlayer, NULL, getStrFromCard(testCard));
    // delay(5000); // 5 second delay before next send

    // Receiving Function
    Packet *pkt = receivePacket();
    if (pkt != NULL)
    {
        Card receiveCard = getCardFromStr(pkt->string);
        Serial.print("Rank: ");
        Serial.println(receiveCard.rank);
        Serial.print("Suit: ");
        Serial.println(receiveCard.suit);
    }
<<<<<<< HEAD
}*/
=======
}*/
>>>>>>> BackWork
