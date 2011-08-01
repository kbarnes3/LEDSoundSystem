// Main LED Sound System Arduino entry point

#include "WProgram.h"
#include "spectrum.h"
#include "BeatChannel.h"

// Constants
const unsigned int c_SampleInstance = 50000; // micro-seconds (5/100 of a second)

const byte c_SpectrumFlagsAll = SPECTRUM_BAND_1 | SPECTRUM_BAND_2 | SPECTRUM_BAND_3 |
    SPECTRUM_BAND_4 | SPECTRUM_BAND_5 | SPECTRUM_BAND_6 | SPECTRUM_BAND_7;

const bool fDebugMode = false;

unsigned long lastReadTime = 0;
unsigned long lastIntanceTime = 0;

CBeatChannel beatChanArray[] =
{
    CBeatChannel(9, (SPECTRUM_BAND_1 | SPECTRUM_BAND_2 | SPECTRUM_BAND_3)),
    CBeatChannel(10, (SPECTRUM_BAND_4 | SPECTRUM_BAND_5)),
    CBeatChannel(11, (SPECTRUM_BAND_6 | SPECTRUM_BAND_7)),
};

const byte c_cBeatChanArray = sizeof(beatChanArray) / sizeof(CBeatChannel);

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
    
    for (byte i = 0; i < c_cBeatChanArray; i++)
    {
        beatChanArray[i].AddSample(spectrumEnergy, fEndBeat);
    }
    
    if (fEndBeat)
    {
        lastIntanceTime = currentTime;
    }
}


