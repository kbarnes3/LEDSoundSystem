
#pragma once

enum LightState
{
    Light_Off = 0,
    Light_Dim = 50,
    Light_Bright = 255,
};

struct ILightDisplay
{
    virtual void UpdateDisplay(LightState lightStateNew) = 0;
};

class CStandardDisplay : public ILightDisplay
{
public:
    CStandardDisplay(){}
    
    void SetPin(byte pin);
    virtual void UpdateDisplay(LightState lightStateNew);
    
private:
    byte _pin;
    LightState _lightState;
};

class CLightCycleDisplay : public ILightDisplay
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
