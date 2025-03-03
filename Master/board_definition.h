#ifndef BOARD_DEFINITION_H_
#define BOARD_DEFINITION_H_

#define LCD_WIDTH   16
#define LCD_HEIGHT  2
#define LCD_SIZE    (LCD_WIDTH * LCD_HEIGHT)

#define PINRS       8
#define PINEN       9
#define PIND4       4 
#define PIND5       5 
#define PIND6       6 
#define PIND7       7  
#define BUTTONPIN   A0

// include the library code:
#include <Wire.h>
#include <Arduino.h>
#include <stdio.h>
#include <LiquidCrystal.h>
#include "linked_list.h"

#define LCD_WIDTH      16
#define LCD_HEIGHT     2
#define LCD_SIZE       (LCD_WIDTH * LCD_HEIGHT)

#define EXIT_OK         0
#define EXIT_ERROR     -1

////////////////////////////////////////////////////////////////////////////////

enum Button       {none, right, left, up, down, select};
enum InsertOption {ins_tail, ins_head, ins_middle};
enum DeleteOption {del_one, del_all};
/**
 * @brief LCDBoard class defined containing all methods, to use this, declare a 
 * global instance in .ino file
 */
LiquidCrystal* InitLCD( int pin_rs, int pin_en, int pin_d4, int pin_d5, 
                        int pin_d6, int pin_d7, int lcd_width, int lcd_height);

int Menu(LiquidCrystal *lcd);
void ReturnToMenu(LiquidCrystal *lcd);

/**
 * @brief Method to control the 4-digit number input mechanism. Press UP and 
 * DOWN to change the number, LEFT and RIGHT to shift the cursor.
 * @return int the number inputted by the user
 */
int NumberHandling(LiquidCrystal *lcd);
//This is popped after choosing ADD in MENU; to options: "Add node to front", and "Add node to back"
InsertOption InsertMenu(LiquidCrystal *lcd);         
DeleteOption DeleteMenu(LiquidCrystal *lcd); 
void  Check(LiquidCrystal *lcd);
       
    
/**
 * @brief Prints the linked list numbers onto the display
 * 
 * @param ptemp pass reference of which node to be printed
 * @return int EXIT_OK if success, otherwise EXIT_ERR
 */
int ShowListPrinting(Node *ptemp,LiquidCrystal *lcd);


//Option Handling Functions
int InsertHandle(LiquidCrystal *lcd);
int ShowHandle(LiquidCrystal *lcd);
int DeleteHandle(LiquidCrystal *lcd);


#endif
