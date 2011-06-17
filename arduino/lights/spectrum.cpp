// Utilities for interacting with a spectrum shield

#include "WProgram.h"
#include "spectrum.h"

const byte c_pinSpectrumStrobe = 4;
const byte c_pinSpectrumReset = 5;
const byte c_apinSpectrumLeft = 0;
const byte c_apinSpectrumRight = 1;

int rgSpectrum[c_cBands];

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

void SampleSpectrum()
{
    for(byte iBand = 0; iBand < c_cBands; iBand++)
    {
        int bandValue = 0;
        // Read both the left and right channel twice and take the average

        bandValue = analogRead(c_apinSpectrumLeft);
        bandValue += analogRead(c_apinSpectrumRight);
        bandValue += analogRead(c_apinSpectrumLeft);
        bandValue += analogRead(c_apinSpectrumRight);

        bandValue = bandValue >> 2;
        rgSpectrum[iBand] = bandValue;

        // Cycle to the next band
        digitalWrite(c_pinSpectrumStrobe, HIGH);
        digitalWrite(c_pinSpectrumStrobe, LOW);
    }
}

int ReadBand(int iBand)
{
    if (iBand < 0)
    {
        return -1;
    }
    if (iBand >= c_cBands)
    {
        return -1;
    }

    return rgSpectrum[iBand];
}

