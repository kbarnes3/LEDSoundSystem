
#pragma once

enum LightState
{
    Light_Off = 0,
    Light_Dim = 50,
    Light_Bright = 255,
};

class CLightDisplay
{
public:
    virtual void UpdateDisplay(LightState lightStateNew);
};

class CStandardDisplay : public CLightDisplay
{
public:
    CStandardDisplay(){}
    
    void SetPin(byte pin);
    virtual void UpdateDisplay(LightState lightStateNew);
    
private:
    byte _pin;
    LightState _lightState;
};

class CLightCycleDisplay : public CLightDisplay
{
public:
    CLightCycleDisplay(){};
    
    void SetPins(const byte pinArray[3]);
    virtual void UpdateDisplay(LightState lightStateNew);
    
private:
    byte _pinArray[3];
    byte _curPinIndex;
    int temp;
};