#include "Arduino.h"
#include "spectrum.h"
#include "LightDisplay.h"
#include "BeatChannel.h"

const float c_FullBeatFactor = 1.3;
const float c_MinBeatFactor = 1.2;

const unsigned long c_minEnergy = 12*12;

CBeatChannel::CBeatChannel() :
    _spectrumFlags(0),
    _pLightDisplay(NULL)
{
    Reset();
}

void CBeatChannel::AddSample(unsigned long results[c_cBands], bool fEndBeat)
{
    unsigned long energy = 0;
    int sampleCount = 0;
    
    for (int i = 0; i < c_cBands; i++)
    {
        byte flag = (1 << i);
        
        if (flag & _spectrumFlags)
        {
            energy += results[i];
            sampleCount++;
        }
    }
    
    energy /= sampleCount; // take average value
    
    AddSample(energy, fEndBeat);
}

void CBeatChannel::AddSample(unsigned long energy, bool fEndBeat)
{
    _beatEnergySum += energy;
    _beatSampleCount++;
    
    if (fEndBeat)
    {
        float historyAvg = (float)_historySum / (float)c_HistorySize;
    
        float beatFactor = (float)_beatEnergySum / historyAvg;
        LightState state = Light_Off;
        
        if ( (beatFactor > c_MinBeatFactor) &&
            (AvgBeatEnergy() >= c_minEnergy) )
        {
            state = (beatFactor > c_FullBeatFactor) ? Light_Bright : Light_Dim;
        }
        
        _pLightDisplay->UpdateDisplay(state);
        
        _historySum += _beatEnergySum;
        _historySum -= _history[_oldestHistoryIndex];
        
        // Replace oldest value with the newest beat sum
        _history[_oldestHistoryIndex] = _beatEnergySum;
        
        // Move oldest index.
        _oldestHistoryIndex++;
        if (_oldestHistoryIndex == c_HistorySize)
        {
            _oldestHistoryIndex = 0;
        }
        
        _beatEnergySum = 0;
        _beatSampleCount = 0;
    }
}

void CBeatChannel::Reset()
{
    // Initialize history to 0;
    memset(_history, 0, sizeof(_history));
    _historySum = 0;

    _beatEnergySum = 0;
    _beatSampleCount = 0;

    _oldestHistoryIndex = 0;
}

