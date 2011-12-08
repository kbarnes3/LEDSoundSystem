// Main LED Sound System Arduino entry point

#include "Arduino.h"
#include "Pins.h"
#include "spectrum.h"
#include "LightDisplay.h"
#include "BeatChannel.h"

// Constants
const unsigned int c_SampleInstance = 50000; // micro-seconds (5/100 of a second)

const byte c_SpectrumFlagsAll = SPECTRUM_BAND_1 | SPECTRUM_BAND_2 | SPECTRUM_BAND_3 |
    SPECTRUM_BAND_4 | SPECTRUM_BAND_5 | SPECTRUM_BAND_6 | SPECTRUM_BAND_7;

const bool fDebugMode = false;

// Forward declares
void ToggleMode();

DisplayMode::Enum g_eDisplayMode = DisplayMode::Initial;
unsigned long lastIntanceTime = 0;
bool g_fButtonDown = false;

CBeatChannel beatChanArray[3];

// Standard light display that controls one light per beat channel
CStandardDisplay standardDisplayArray[3];

CLightCycleDisplay cycleLightDisplay;

byte cBeatChannelsUsed;

void setup()
{
    byte mode = 0;

    pinMode(c_pinPushButton, INPUT);

    ToggleMode();

    if (mode == 0)
    {
        // Standard mode.  One light for part of the sound spectrum.

        standardDisplayArray[0].SetPin(c_pinLight1);
        beatChanArray[0].SetDisplay(&standardDisplayArray[0]);
        beatChanArray[0].SetSpectrumFlags(SPECTRUM_BAND_1 | SPECTRUM_BAND_2 | SPECTRUM_BAND_3);

        standardDisplayArray[1].SetPin(c_pinLight2);
        beatChanArray[1].SetDisplay(&standardDisplayArray[1]);
        beatChanArray[1].SetSpectrumFlags(SPECTRUM_BAND_4 | SPECTRUM_BAND_5);

        standardDisplayArray[2].SetPin(c_pinLight3);
        beatChanArray[2].SetDisplay(&standardDisplayArray[2]);
        beatChanArray[2].SetSpectrumFlags(SPECTRUM_BAND_6 | SPECTRUM_BAND_7);

        cBeatChannelsUsed = 3;
    }
    else
    {
        // Lights cycle over one part of the spectrum

        byte pins[3] = { c_pinLight1, c_pinLight2, c_pinLight3 };
        cycleLightDisplay.SetPins(pins);
        beatChanArray[0].SetDisplay(&cycleLightDisplay);
        beatChanArray[0].SetSpectrumFlags(SPECTRUM_BAND_1 | SPECTRUM_BAND_2);

        cBeatChannelsUsed = 1;
    }

    SetupSpectrum();

    if (fDebugMode)
    {
        Serial.begin(9600);
    }
}

void loop()
{
    unsigned long currentTime = micros();

    int pushButton = digitalRead(c_pinPushButton);
    if (g_fButtonDown && (pushButton == LOW))
    {
        // The button was released, switch modes
        g_fButtonDown = false;
    }
    else if (pushButton == HIGH)
    {
        g_fButtonDown = true;
    }

    unsigned long spectrumEnergy[c_cBands];
    SampleSpectrum(c_SpectrumFlagsAll, spectrumEnergy);

    unsigned long elapsedTime = (currentTime - lastIntanceTime);

    bool fEndBeat = (elapsedTime >= c_SampleInstance);

    for (byte i = 0; i < cBeatChannelsUsed; i++)
    {
        beatChanArray[i].AddSample(spectrumEnergy, fEndBeat);
    }

    if (fEndBeat)
    {
        lastIntanceTime = currentTime;
    }
}

void ToggleMode()
{
    switch (g_eDisplayMode)
    {
        case DisplayMode::Initial:
        {
            g_eDisplayMode = DisplayMode::Standard;
            break;
        }
        case DisplayMode::Standard:
        {
            g_eDisplayMode = DisplayMode::Cycle;
            break;
        }
        case DisplayMode::Cycle:
        {
            g_eDisplayMode = DisplayMode::Standard;
            break;
        }
    }
}

