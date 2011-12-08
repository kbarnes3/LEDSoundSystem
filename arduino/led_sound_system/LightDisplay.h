#pragma once

enum LightState
{
    Light_Off = 0,
    Light_Dim = 50,
    Light_Bright = 255,
};

namespace DisplayMode
{
    enum Enum
    {
        Initial,
        Standard,
        Cycle,
    };
}

const size_t c_cCycleLights = 3;

struct ILightDisplay
{
    virtual void UpdateDisplay(LightState lightStateNew) = 0;
    virtual void Reset() = 0;
};

class CStandardDisplay : public ILightDisplay
{
public:
    CStandardDisplay(){}

    void SetPin(byte pin);
    virtual void UpdateDisplay(LightState lightStateNew);
    virtual void Reset();

private:
    byte _pin;
    LightState _lightState;
};

class CLightCycleDisplay : public ILightDisplay
{
public:
    CLightCycleDisplay(){};

    void SetPins(const byte pinArray[c_cCycleLights]);
    virtual void UpdateDisplay(LightState lightStateNew);
    virtual void Reset();

private:
    byte _pinArray[c_cCycleLights];
    byte _curPinIndex;
};

