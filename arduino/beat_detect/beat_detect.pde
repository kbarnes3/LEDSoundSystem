// Main Sustainable Lights Arduino entry point

#include "WProgram.h"
#include "spectrum.h"
#include "BeatChannel.h"

// Constants
const unsigned int c_ReadsPerSecond = 512;
const unsigned int c_SampleInstance = 50000; // micro-seconds (5/100 of a second)
const float c_SampleInterval = (1000000/c_ReadsPerSecond);

const byte c_SpectrumFlagsAll = 127;

const bool fDebugMode = false;

unsigned long lastReadTime = 0;
unsigned long lastIntanceTime = 0;

CBeatChannel beatChanArray[] = 
{
    CBeatChannel(3, (SPECTRUM_BAND_1 | SPECTRUM_BAND_2)),
    CBeatChannel(6, (SPECTRUM_BAND_4 | SPECTRUM_BAND_5)),
};


void setup()
{
    SetupSpectrum();
    
    if (fDebugMode)
    {
        Serial.begin(9600);
    }
}

void loop()
{
    unsigned long currentTime = micros();
   
    unsigned long spectrumEnergy[c_cBands];
    SampleSpectrum(c_SpectrumFlagsAll, spectrumEnergy);
    
    // Record the time of the last sample
    lastReadTime = currentTime;
    
    unsigned long elapsedTime = (currentTime - lastIntanceTime);
    
    bool fEndBeat = (elapsedTime >= c_SampleInstance);
    
    int arraySize = sizeof(beatChanArray) / sizeof(CBeatChannel);
    
    for (int i = 0; i < arraySize; i++)
    {
        beatChanArray[i].AddSample(spectrumEnergy, fEndBeat);
    }
    
    if (fEndBeat)
    {      
        lastIntanceTime = currentTime;
    }
}


