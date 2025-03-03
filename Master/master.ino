#include "board_definition.h"

LiquidCrystal *lcd;

S

void setup() {
  Serial.begin(9600);
  // Initialize the LCD board
  lcd = InitLCD(PINRS, PINEN, PIND4, PIND5, PIND6, PIND7,
                LCD_WIDTH, LCD_HEIGHT);
}

void loop() {
  //checklist();

  // Shows the menu on the LCD
  // Option choice 1--"Add" 2--"show" 3--"remove"
  switch (Menu(lcd)) {
    case 1:
      if (InsertHandle(lcd) == EXIT_OK) {
        ReturnToMenu(lcd);
      }
      break;
    case 2:
      if (ShowHandle(lcd) == EXIT_OK) {
        ReturnToMenu(lcd);
      }
      break;
    case 3:
      if (DeleteHandle(lcd) == EXIT_OK) {
        ReturnToMenu(lcd);
      }
      break;
  }
}

int checklist() {
  if (p_head) {
    Serial.println("list exists");
  } else {
    Serial.println("no list");
  }
}
