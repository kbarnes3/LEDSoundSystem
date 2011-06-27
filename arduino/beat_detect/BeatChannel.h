
#include "spectrum.h"

// constants
const unsigned int c_HistorySize = 20;

class CBeatChannel
{
public:
    CBeatChannel(byte pin, byte spectrumFlags);
    ~CBeatChannel(){}
    
    void AddSample(unsigned long results[c_cBands], bool fEndBeat);
    void AddSample(unsigned long energy, bool fEndBeat);
    
private:

    enum LightState
    {
        Light_Off = 0,
        Light_Dim = 50,
        Light_Bright = 255,
    };

    void UpdateDisplay(LightState lightState);

    // data
    unsigned long _history[c_HistorySize]; // history of previous beats
    unsigned long _historySum;
    
    unsigned long _beatEnergySum;
    unsigned int  _beatSampleCount;
    
    byte _oldestHistoryIndex;
    
    LightState _lightState;
    
    byte _pin; // pin to write beat signals to
    byte _spectrumFlags;
    
    
};