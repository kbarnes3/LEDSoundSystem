// Main Sustainable Lights Arduino entry point

#include "WProgram.h"

const byte c_cBands = 7;
byte rgBands[c_cBands];
byte currentValue = 0;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    char szOutput[c_cBands + 1] = {};

    for (int iBand = 0; iBand < c_cBands; iBand++)
    {
        rgBands[iBand] = currentValue;
        IncrementValue();
    }

    sprintf(szOutput, "%d%d%d%d%d%d%d",
            rgBands[0],
            rgBands[1],
            rgBands[2],
            rgBands[3],
            rgBands[4],
            rgBands[5],
            rgBands[6]);

    Serial.println(szOutput);
    delay(1000);
}

void IncrementValue()
{
    currentValue++;
    if (currentValue > 9)
    {
        currentValue = 0;
    }
}
