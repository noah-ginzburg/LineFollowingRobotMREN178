#include "interface.h"
#include <Arduino.h>
#include <LiquidCrystal.h>

// Define and initialize variables
unsigned long timeElapsed = 0;
unsigned long tInit = 0;
unsigned long tFinal = 0;

int isActive = 1;
int isMsg1On = 0;
int isFirstPass = 1;

void AutoPrint(LiquidCrystal *lcd, char *str, int fromStart = 1)
{
    if (fromStart)
    {
        lcd->clear();
        lcd->home();
    }

    int strLength = strlen(str);

    if (strLength > rows * cols)
    {
        ThrowError(lcd, stdErrMsg);
        return;
    }

    for (int i = 0; i < strLength; i++)
    {
        lcd->write(str[i]);
        if (i == cols - 1)
        {
            lcd->setCursor(0, 1);
        }
    }
}

void ManualPrint(LiquidCrystal *lcd, char *str, int row, int col, int clear)
{
    int len = strlen(str);

    if (col + len > cols)
    {
        ThrowError(lcd, stdErrMsg);
        return;
    }

    if (clear)
    {
        lcd->clear();
    }

    for (int i = 0; i < len; i++)
    {
        lcd->write(str[i]);
    }
}

void SwapMessages(LiquidCrystal *lcd, unsigned long totalTime, char *str1, char *str2, unsigned int delayTime)
{
    if (isFirstPass)
    {
        tInit = totalTime;
        isFirstPass = 0;
        isActive = 0;
    }

    if (!isActive)
    {
        if (totalTime - tInit >= delayTime)
        {
            tInit = totalTime;
            isMsg1On = !isMsg1On;
            isActive = 1;

            Serial.println("Switching Messages...");
            Serial.println(totalTime);
            Serial.println("");
        }
    }
    else
    {
        if (isMsg1On)
        {
            AutoPrint(lcd, str1);
        }
        else
        {
            AutoPrint(lcd, str2);
        }

        isActive = 0;
    }
}

void ThrowError(LiquidCrystal *lcd, char *errMsg)
{
    lcd->clear();
    lcd->home();

    int length = strlen(errMsg);

    for (int i = 0; i < length; i++)
    {
        lcd->write(errMsg[i]);
    }

    delay(5000);
}

int IsButtonPressed(int analogVal, int desiredButton)
{
    int analogMapping = FindBtnMapping(analogVal);
    return analogMapping == desiredButton;
}

int FindBtnMapping(int val)
{
    if (val < RIGHT)
    {
        return RIGHT;
    }
    else if (val < UP)
    {
        return UP;
    }
    else if (val <= DOWN)
    {
        return DOWN;
    }
    else if (val <= LEFT)
    {
        return LEFT;
    }
    else if (val <= SELECT)
    {
        return SELECT;
    }
    else
    {
        return NONE;
    }
}

//test