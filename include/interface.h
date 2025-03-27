#ifndef INTERFACE_H
#define INTERFACE_H
#include <LiquidCrystal.h>

// Include libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define rowI 1
#define colI 15
#define rows 2
#define cols 16

#define stdErrMsg "Err: str display"

#define RIGHT 60
#define UP 200
#define DOWN 400
#define LEFT 600
#define SELECT 800
#define NONE 1023

// Function prototypes
void AutoPrint(LiquidCrystal *lcd, char *str, int fromStart = 1);
void ManualPrint(LiquidCrystal *lcd, char *str, int row, int col, int clear);
void SwapMessages(LiquidCrystal *lcd, unsigned long totalTime, char *str1, char *str2, unsigned int delayTime);
void ThrowError(LiquidCrystal *lcd, char *errMsg);
int IsButtonPressed(int analogVal, int desiredButton);
int FindBtnMapping(int val);

#endif
