#include "Arduino.h"
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

void CStandardDisplay::Reset()
{
    analogWrite(_pin, Light_Off);
    _lightState = Light_Off;
}

void CLightCycleDisplay::SetPins(const byte pinArray[c_cCycleLights])
{

    memcpy(_pinArray, pinArray, sizeof(_pinArray));

    for (int i = 0; i < c_cCycleLights; i++)
    {
        pinMode(pinArray[i], OUTPUT);
    }

    _curPinIndex = c_cCycleLights - 1;
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
        _curPinIndex %= c_cCycleLights;

        // Turn on new light
        pin = _pinArray[_curPinIndex];
        analogWrite(pin, Light_Bright);

    }
}

void CLightCycleDisplay::Reset()
{
    byte pin = 0;
    // Turn off all the lights
    for (int i = 0; i < c_cCycleLights; i++)
    {
        pin = _pinArray[i];
        analogWrite(pin, Light_Off);
    }

    // Set the first light to be the light that is on
    _curPinIndex = 0;

    // Turn on first light
    pin = _pinArray[_curPinIndex];
    analogWrite(pin, Light_Bright);
}

