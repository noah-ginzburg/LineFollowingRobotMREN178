// Player GUI setup below:

// Start by intializing buttons (left, right) as well as LCD pins
// Different menu options
// Switch cases -> if button 'left/right' is pressed, switch to menu 2
// If select is pressed for each menu (pass/draw/show), present different screen

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

// initialize the library by associating any needed LCD interface pin with the arduino pin number it is connected to
const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int analogInPin = A0;
int buttonVal = 0;
int prevButtonVal = 0;

int pNum = -1;

bool isConnected = false;

// initialize menu
int menu = 1;

// prints 'Action:' on LCD
void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Action:");
}

// initializes loop
void loop()
{

  buttonVal = analogRead(analogInPin);

  if (abs(prevButtonVal - buttonVal) > 20)
  {
    if (IsButtonPressed(buttonVal, SELECT) && isConnected == false)
    {
      AutoPrint(&lcd, "Button Pressed!");

      sendPacket(pNum, 0, addPlayer, NULL, "\0");
    }
  }

  Packet *incomingPacket = receivePacket();

  switch (incomingPacket->command)
  {
  case addPlayer:
    isConnected = true;
    pNum = incomingPacket->receiverID;

    AutoPrint(&lcd, incomingPacket->string, 1);
    tone(3, 500, 200);
    noTone(3);
    delay(5000);

    break;

  default:
    break;
  }

  prevButtonVal = buttonVal;

  /*
  int x = analogRead(0);
  lcd.setCursor(0,1);

//loops between menus depending on when LEFT and RIGHT is pressed

  if (x < RIGHT) {
    menu++;
    delay(200);
        while (x < 1000) {
      x = analogRead (0);
    }
  }
  if (x <= LEFT) {
    menu--;
    delay(200);
        while (x < 1000) {
      x = analogRead (0);
    }
  }

  //loops between more menus when select is pressed for each main switch case (see switch(menu))

  if (menu == 1 && x <= SELECT) {
    menu = 5;
    delay(200);
       while (x < 1000) {
      x = analogRead (0);
    }
  }

  //initialize switch cases
    switch(menu) {
    //MAIN SWITCH CASES (shows the user three options: pass, draw, or show)

    //Switch case 1:
    case 1:
    lcd.print("Pass ");
    break;
    //Switch case 2:
    case 2:
    lcd.print("Draw ");
    break;
    //Switch case 3:
    case 3:
    lcd.print("Show ");
    break;

    //INVALID SWITCH CASES (cases that, if encountered, return the user to one of the main switch cases)

    //Switch case 0:
    case 0:
    menu = 3;
    break;
    //Switch case 4:
    case 4:
    menu = 1;
    break;

    //SELECT SWITCH CASES (cases that, when select is pressed while one of the main switch cases is displayed, returns one of
    //three functions: pass function, draw function, or show function

    //Switch case 5:
    case 5:
        //run pass function
    break;
    }
*/
}