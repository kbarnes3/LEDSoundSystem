
#pragma once

#include "spectrum.h"

// constants
const unsigned int c_HistorySize = 20;

struct ILightDisplay;

class CBeatChannel
{
public:
    CBeatChannel();
    ~CBeatChannel(){}

    void SetSpectrumFlags(byte spectrumFlags) { _spectrumFlags = spectrumFlags; }

    void AddSample(unsigned long results[c_cBands], bool fEndBeat);
    void AddSample(unsigned long energy, bool fEndBeat);

    void SetDisplay(ILightDisplay * pDisplay) { _pLightDisplay = pDisplay; }
    void Reset();

private:

    unsigned long AvgBeatEnergy() { return (_beatEnergySum/_beatSampleCount); }

    // data
    unsigned long _history[c_HistorySize]; // history of previous beats
    unsigned long _historySum;

    unsigned long _beatEnergySum;
    unsigned int  _beatSampleCount;

    byte _oldestHistoryIndex;

    byte _spectrumFlags;

    ILightDisplay * _pLightDisplay;

};
