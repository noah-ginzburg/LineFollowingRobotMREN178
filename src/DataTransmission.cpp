/******************************************************************************
FILENAME:     DataTransmission.cpp
AUTHOR:       Lucas Dejmek
DATE:         20/03/2025

ATTRIBUTIONS: MREN178 Course Content, GitHub Copilot
******************************************************************************/

//Arduino Standard Libraries
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

//Custom Libraries
#include <card.h>
#include <player.h>

//C Standard Libraries
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// LCD Keypad Shield Pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// HC-12 Wireless Communication
SoftwareSerial HC12(10, 11);  

#define NODE_ID 1 //Change this line for every node. NOTE: MASTER IS NODE ZERO (0). ALL PLAYERS ARE NODES 1-7. 
             // WARNING!!! THIS IS NOT PLAYER_ID! NODE_ID IS THE ID OF THE NODE THE CODE IS RUNNING ON, WHEREAS PLAYER_ID IS THE ID OF THE PLAYER GETTING SENT DATA

#define NUM_NODES 4    
#define INF 9999       
#define BUZZER_PIN 3   

#define BTN_RIGHT  0    // Analog Output Value for Right button
#define BTN_LEFT   410  // Analog Output Value for Left button
#define BTN_UP     99   // Analog Output Value for Up button
#define BTN_DOWN   255  // Analog Output Value for Down button
#define BTN_SELECT 640  // Analog Output Value for Select button
#define BTN_NONE   1023 // No button pressed

typedef enum commands {playCard = 0, drawCard, showCards} Cmd; //Commands

typedef struct packet{
    int senderID; // ID of who the packet is getting sent from
    int receiverID; // ID of who the packet is getting sent to
    Cmd command;
    //Card cardsDealt[10]; //Use this when the master sends the client their initial or updated hand every round
    Card cardPlayed; //Use this when the player sends the master which card to play
    char string[50]; //String the master to send to the players - CHANGE LENGTH DEPENDING ON WHAT YOU WANT
} Packet;



//// TEST VALUES TO SEE IF PROGRAM WORKS

// Card* testcard;
Card testcard = {Five, hearts}; // Example
char string[50] = "Message Sent Sucessfully!";
//String teststring;

//// END OF TEST VALUES

int sendPacket(int receiverID, Cmd command, Card cardPlayed, char string[]){ //Add "Card cardsDealt" later

    Packet* pkt = (Packet*) malloc(sizeof(Packet)); //Initialize a packet
    pkt->senderID = NODE_ID; // Set the sender ID as that node's ID - CHANGE DEPENDING ON WHICH ARDUINO THE CODE IS RUNNING ON
    pkt->receiverID = receiverID;  // Set the variables of the packet to what was passed into the function
    pkt->command = command;
    //pkt->cardsDealt[10] = cardsDealt;
    pkt->cardPlayed = cardPlayed;
    //pkt->cardPlayed.suit = cardPlayed.suit;
    strncpy(pkt->string, string, sizeof(pkt->string) - 1); // Copy string into pkt->string, leave space for null terminator
    pkt->string[sizeof(pkt->string) - 1] = '\0'; // Ensure null termination

    HC12.write((uint8_t*)pkt, sizeof(Packet)); //Create a pointer to the packet and translate it into bits of data. Then send bits over the HC12
    //Serial.println("Packet Sent!");

    free(pkt); //Free the memory used by malloc after sending message

    return 1; //Packet sent sucessfully
}


int receivePacket(){
    Packet pkt;

    //Serial.println("Receive Packet Function Opened");

    // Serial.print("HC12 available bytes: ");
    // Serial.println(HC12.available());
    // delay(2000);

    while (HC12.available()) {  // Make sure to receive the whole packet >= sizeof(pkt)
        
        HC12.readBytes((char*)&pkt, sizeof(pkt));  // Read the whole packet back into the struct
        
        if (pkt.receiverID == NODE_ID) { //Check if the message was meant for this node. If so, process the data
            Serial.println("\n--- BEGINNING OF TRANSMISSION ---");
            Serial.print("Received from Player #");
            Serial.println(pkt.senderID);
            Serial.print("Command: ");
            Serial.println(pkt.command);
            Serial.print("Card Played: ");
            Serial.print(pkt.cardPlayed.rank);
            Serial.print(" of ");
            Serial.println(pkt.cardPlayed.suit);
            Serial.println(pkt.string);
            Serial.println("--- END OF TRANSMISSION ---\n");
            return 1; //Return 1: Data Received
        }
    }
    return 0; //Return 0: Nothing received
}



void setup() {
    Serial.begin(9600);
    HC12.begin(9600);
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop() { //Don't use, just call the send and receive functions in your code and pass by reference what you want

    // Transmitting Function
    //sendPacket(1, playCard, testcard, string);
    //delay(5000); //5 second delay before next send

    //Receiving Function
    receivePacket();
}






// int buttonPress() {
//     int x = analogRead(0);
//     int pressed = 0;
//     if (pressed==0){
//       if (x < 100) return BTN_RIGHT; //RIGHT BUTTON
//       else if (x < 250) return BTN_UP; //UP BUTTON
//       else if (x < 400) return BTN_DOWN; //DOWN BUTTON
//       else if (x < 600) return BTN_LEFT; //LEFT BUTTON
//       else if (x < 800) return BTN_SELECT; //SELECT BUTTON
//       pressed = 1;
//     }
//     else{
//       pressed=0;
//       return BTN_NONE;
//     }
// }


/////////////////////////









/*


// Each node in your network needs to have a unique id.  Change NODE_ID
// to be a value between 0 and NUM_NODES-1
const int NODE_ID = 0;  

// This is an arbitrary adjacency matrix denoting the topology of the network 
int graph[NUM_NODES][NUM_NODES] = {
    {0, 1, INF, 2},  
    {1, 0, 3, INF},  
    {INF, 3, 0, 1},  
    {2, INF, 1, 0}   
};

int destinationNode = 0;
char message[MAX_MSG_LEN + 1];  
int cursorPos = 0;   
char selectedChar = 'A';  

void setup() {
    Serial.begin(9600);
    hc12.begin(9600);
    lcd.begin(16, 2);
    pinMode(BUZZER_PIN, OUTPUT);

    memset(message, ' ', MAX_MSG_LEN);
    message[MAX_MSG_LEN] = '\0'; 

    lcd.print("Enter Msg:");
}

void loop() {
    receiveMessage();  // Always check for incoming messages
    inputMessage();    // Allows for user input message

    if (readButton() == BTN_SELECT) {
        lcd.clear();
        lcd.print("Select Dest:");
        delay(500);
        selectDestination();
        int prev[NUM_NODES];
        findShortestPath(NODE_ID, destinationNode, prev);
        int nextNode = getNextNodeInPath(destinationNode);
        if (nextNode != -1) {
            sendMessage(NODE_ID, nextNode, destinationNode, String(message));
        }
    }
}

void receiveMessage() {
  if (HC12.available()) {
      String msg = HC12.readStringUntil('\n');  // FIX: Read until newline
      Serial.println("Received: " + msg);

      if (msg.startsWith("MSG,")) {
          int firstComma = msg.indexOf(',');
          int secondComma = msg.indexOf(',', firstComma + 1);
          int thirdComma = msg.indexOf(',', secondComma + 1);
          int messageStart = thirdComma + 1;

          int sender = msg.substring(4, firstComma).toInt();
          int nextNode = msg.substring(firstComma + 1, secondComma).toInt();
          int destination = msg.substring(secondComma + 1, thirdComma).toInt();
          String receivedText = msg.substring(messageStart);

          // if the message isn't relayed through us, ignore
          if (nextNode != NODE_ID) return;



          if (destination == NODE_ID) {
              lcd.clear();
              lcd.print("Msg from ");
              lcd.print(sender);
              lcd.setCursor(0, 1);
              lcd.print(receivedText);
              playReceiveTone();
              delay(3000);
              lcd.clear();
              lcd.print("Enter Msg:");
          } else {
              int newNextNode = getNextNodeInPath(destination);
              if (newNextNode != -1) {
                  sendMessage(NODE_ID, newNextNode, destination, receivedText);
              }
          }
      }
  }
}

// Function to get the next node in the shortest path
int getNextNodeInPath(int target) {
    int prev[NUM_NODES];  // Stores previous nodes in the shortest path

    // Call findShortestPath() to populate prev[] array
    findShortestPath(NODE_ID, target, prev);

    // Reconstruct the path to determine the next hop by backtracking through 
    // the prev array
    int node = target;
    while (prev[node] != -1 && prev[node] != NODE_ID) {
        node = prev[node];
    }

    return (prev[node] == NODE_ID) ? node : -1;  // Return next node in path
}

// Function to enter a message using the potentiometer
void inputMessage() {
    receiveMessage();  
    selectedChar = getLetterFromPot();
    int button = readButton();

    if (button == BTN_LEFT) {
        moveCursor(-1);
    } else if (button == BTN_RIGHT) {
        moveCursor(1);
    } else {
        message[cursorPos] = selectedChar;
    }
 
    lcd.setCursor(0, 1);
    lcd.print(message);
    lcd.setCursor(cursorPos, 1);
    lcd.blink();
}

// Function to move cursor left or right
void moveCursor(int direction) {
    int newCursorPos = cursorPos + direction;
    if (newCursorPos >= 0 && newCursorPos < MAX_MSG_LEN) {
        cursorPos = newCursorPos;
    }

    lcd.setCursor(0, 1);
    lcd.print(message);
    lcd.setCursor(cursorPos, 1);
}

// Get the selected letter from the potentiometer
char getLetterFromPot() {
    int potValue = analogRead(A1);
    int index = map(potValue, 0, 1023, 0, 27); // 26 letters + Space 
    return (index < 26) ? ('A' + index) : ' '; // Map 0-25 to A-Z, 26 to space
}

// Function to use up/left/down/right buttons on LCD keypad shield to select 
// destination node
void selectDestination() {
    while (true) {
        receiveMessage();  // Always listen for incoming messages

        lcd.setCursor(0, 1);
        lcd.print("Node ");
        lcd.print(destinationNode);
        lcd.print("    ");

        int button = readButton();

        if (button == BTN_RIGHT || button == BTN_UP) {
            destinationNode = (destinationNode + 1) % NUM_NODES;  // Increment node, wrap around
            delay(200); // Debounce delay
        } else if (button == BTN_LEFT || button == BTN_DOWN) {
            destinationNode = (destinationNode - 1 + NUM_NODES) % NUM_NODES;  // Decrement node, wrap around
            delay(200); // Debounce delay
        } else if (button == BTN_SELECT) {
            lcd.clear();
            lcd.print("Dest Selected:");
            lcd.setCursor(0, 1);
            lcd.print("Node ");
            lcd.print(destinationNode);
            delay(1000);
            return;
        }
    }
}

int readButton() {
    int adc = analogRead(A0);
    delay(100);  // Debounce delay

    if (adc < 50) return BTN_RIGHT;  // Right button pressed
    if (adc < 150) return BTN_UP;     // Up button pressed
    if (adc < 300) return BTN_DOWN;   // Down button pressed
    if (adc < 500) return BTN_LEFT;   // Left button pressed
    if (adc < 800) return BTN_SELECT; // Select button pressed

    return BTN_NONE;  // No button pressed
}

void sendMessage(int source, int nextNode, int destination, String msg) {
    String messagePacket = "MSG," + String(source) + "," + String(nextNode) + "," + String(destination) + "," + msg;
    hc12.println(messagePacket);
    lcd.clear();
    lcd.print("Sent to Node ");
    lcd.setCursor(0, 1);
    lcd.print(nextNode);
    Serial.println("Sent: " + messagePacket);
    playSendTone();
}

void playSendTone() {
    tone(BUZZER_PIN, 1000, 200);
    delay(250);
    tone(BUZZER_PIN, 1200, 200);
    
    tone(BUZZER_PIN, 660, 200);
    delay(250);
    tone(BUZZER_PIN, 831, 200);

    tone(BUZZER_PIN, 523, 200);
    delay(250);
    tone(BUZZER_PIN, 660, 200);
}

void playReceiveTone() {
    int melody[] = { 262, 330, 392, 523 };
    for (int i = 0; i < 4; i++) {
        tone(BUZZER_PIN, melody[i], 200);
        delay(250);
    }
}

void findShortestPath(int start, int target, int prev[]) {
    int dist[NUM_NODES];
    bool visited[NUM_NODES] = {false};

    // Initialize distances and previous nodes
    for (int i = 0; i < NUM_NODES; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    // Compute shortest paths using Dijkstra’s Algorithm
    // -----------------------YOUR CODE GODES HERE-----------------------------

    bool finished = false;

    do
    {
      int shortestDist = INF;
      int nodeToCheck = 0;

      for (int i = 0; i < NUM_NODES; i++)
      {
        if (visited[i] == false)
        {
          if (dist[i] < shortestDist) 
          {
            shortestDist = dist[i];
            nodeToCheck = i;
          }
        }
      }

      visited[nodeToCheck] = true;

      for (int i = 0; i < NUM_NODES; i++)
      {
        if (visited[i] == false && graph[nodeToCheck][i] != INF)
        {
          if (dist[i] > dist[nodeToCheck] + graph[nodeToCheck][i])
          {
            dist[i] = dist[nodeToCheck] + graph[nodeToCheck][i];
            prev[i] = nodeToCheck;
          }
        }
      }
      
      finished = true;
      
      for (int i = 0; i < NUM_NODES; i++)
      {
        if (visited[i] == false || shortestDist == INF) finished = false;
      }
    }
    while (finished == false);
     
    // ------------------------------------------------------------------------
    // Constructs the path from the prev array
    int path[NUM_NODES];
    int count = 0, node = target;
    while (node != -1) {
        path[count++] = node;
        node = prev[node];
    }

    // Prints the path to screen 
    lcd.clear();
    lcd.print("Path: ");
    for (int i = count - 1; i >= 0; i--) {
        lcd.print(path[i]);
        if (i > 0) lcd.print("->");
    }
    delay(2000);
}

*/