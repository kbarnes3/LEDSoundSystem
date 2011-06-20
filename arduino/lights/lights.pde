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
    char szBuffer[c_cBands + 1] = {};
    int rgValues[c_cBands] = {};
    SampleSpectrum();

    for(byte iBand = 0; iBand < c_cBands; iBand++)
    {
        rgValues[iBand] = ReadBand(iBand);
    }

    sprintf(szBuffer, "%d%d%d%d%d%d%d",
            rgValues[0],
            rgValues[1],
            rgValues[2],
            rgValues[3],
            rgValues[4],
            rgValues[5],
            rgValues[6]);

    Serial.println(szBuffer);
}

