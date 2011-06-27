// Main Sustainable Lights Arduino entry point

#include "WProgram.h"
#include "spectrum.h"
#include "BeatChannel.h"

// Constants
const unsigned int c_ReadsPerSecond = 512;
const unsigned int c_SampleInstance = 50000; // micro-seconds (5/100 of a second)
const float c_SampleInterval = (1000000/c_ReadsPerSecond);

const byte c_SpectrumFlags1 = (SPECTRUM_BAND_1 | SPECTRUM_BAND_2);
const byte c_SpectrumFlags2 = (SPECTRUM_BAND_4 | SPECTRUM_BAND_5);
//const byte c_SpectrumFlagsCombined = (c_SpectrumFlags1 | c_SpectrumFlags2);
const byte c_SpectrumFlagsCombined = c_SpectrumFlags1;

const byte c_SpectrumFlagsAll = 127;

unsigned long lastReadTime = 0;
unsigned long lastIntanceTime = 0;


CBeatChannel beatChan1(5, c_SpectrumFlags1);
CBeatChannel beatChan2(6, c_SpectrumFlags2);

void setup()
{
    SetupSpectrum();
    Serial.begin(9600);
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
    
    beatChan1.AddSample(spectrumEnergy, fEndBeat);
    beatChan2.AddSample(spectrumEnergy, fEndBeat);
    
    if (fEndBeat)
    {      
        lastIntanceTime = currentTime;
    }
}


