#include "WProgram.h"
#include "LightDisplay.h"

void CStandardDisplay::SetPin(byte pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
    
    _lightState = Light_Off;
}

void CStandardDisplay::UpdateDisplay(LightState lightStateNew)
{
    if (lightStateNew != _lightState)
    {        
        analogWrite(_pin, lightStateNew);
        _lightState = lightStateNew;
    }
}

void CLightCycleDisplay::SetPins(const byte pinArray[3])
{
    
    memcpy(_pinArray, pinArray, 3 * sizeof(byte));
    temp = sizeof(pinArray);

    int len = 3;
    Serial.println(len);
    for (int i = 0; i < len; i++)
    {
        pinMode(pinArray[i], OUTPUT);
    }
    
    _curPinIndex = len - 1;
}

void CLightCycleDisplay::UpdateDisplay(LightState lightStateNew)
{
    if (lightStateNew == Light_Bright)
    {
        // Turn off old light
        byte pin = _pinArray[_curPinIndex];
        analogWrite(pin, Light_Off);
        
        // Find next light
        _curPinIndex++;
        _curPinIndex %= 3;
        
        // Turn on new light
        pin = _pinArray[_curPinIndex];
        analogWrite(pin, Light_Bright);
        
    }
    // move to next light
}