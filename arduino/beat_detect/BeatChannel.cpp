#include "WProgram.h"
#include "BeatChannel.h"

const float c_FullBeatFactor = 1.3;
const float c_MinBeatFactor = 1.2;

const unsigned long c_minEnergy = 12*12;

CBeatChannel::CBeatChannel(byte pin, byte spectrumFlags)
{
    _pin = pin;
    _spectrumFlags = spectrumFlags;
    
    // Initialize history to 0;
    int bufferSize = sizeof(unsigned long) * c_HistorySize;
    memset(_history, 0, bufferSize);
    
    _beatEnergySum = 0;
    _beatSampleCount = 0;
    
    _oldestHistoryIndex = 0;
    
    _lightState = Light_Off;
    
    pinMode(_pin, OUTPUT);
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
        
        UpdateDisplay(state);
        
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

void CBeatChannel::UpdateDisplay(LightState lightStateNew)
{
    if (lightStateNew != _lightState)
    {        
        analogWrite(_pin, lightStateNew);
        _lightState = lightStateNew;
    }
}