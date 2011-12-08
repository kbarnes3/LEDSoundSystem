// Utilities for interacting with a spectrum shield

#include "Arduino.h"
#include "Pins.h"
#include "spectrum.h"

const int   c_maxAnalogRead = 1023;
const int   c_maxByte = 255; 

const float c_energyScale = ((float)c_maxByte/(float)c_maxAnalogRead);

void SetupSpectrum()
{
    // Set the pin modes
    pinMode(c_pinSpectrumStrobe, OUTPUT);
    pinMode(c_pinSpectrumStrobe, OUTPUT);

    // Initialize the spectrum analyzer
    digitalWrite(c_pinSpectrumStrobe, LOW);
    delay(1);
    digitalWrite(c_pinSpectrumReset, HIGH);
    delay(1);
    digitalWrite(c_pinSpectrumStrobe, HIGH);
    delay(1);
    digitalWrite(c_pinSpectrumStrobe, LOW);
    delay(1);
    digitalWrite(c_pinSpectrumReset, LOW);
    
    delay(5);
}

unsigned long ReadBandEnergy()
{
    unsigned long left  = analogRead(c_apinSpectrumLeft);
    left *= c_energyScale;
    
    unsigned long right = analogRead(c_apinSpectrumRight);
    right *= c_energyScale;
    
    unsigned long energyOut = (left*left) + (right*right);
    energyOut /= 2;
    
    return energyOut;
}

void GoToNextBand()
{
    // Cycle to the next band
    digitalWrite(c_pinSpectrumStrobe, HIGH);
    digitalWrite(c_pinSpectrumStrobe, LOW);
}

void SampleSpectrum(byte spectrumFlags, unsigned long results[c_cBands])
{
    
    for (int i = 0; i < c_cBands; i++)
    {
        byte flag = (1 << i);
        
        unsigned long energy = 0;
        
        if (flag & spectrumFlags)
        {
            energy = ReadBandEnergy();
        }
        
        results[i] = energy;
        
        GoToNextBand();
    }
}



