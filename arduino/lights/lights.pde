// Main Sustainable Lights Arduino entry point

#include "WProgram.h"
#include "spectrum.h"

void setup()
{
    SetupSpectrum();
    Serial.begin(9600);
}

void loop()
{
    SampleSpectrum();

    for(byte iBand = 0; iBand < c_cBands; iBand++)
    {
        char szBuffer[50] = {};
        int bandValue = 0;

        bandValue = ReadBand(iBand);
        sprintf(szBuffer, "Band: %d, Value: %d", iBand, bandValue);
        Serial.println(szBuffer);
    }

    Serial.println("--------------------");

    delay(1000);
}

