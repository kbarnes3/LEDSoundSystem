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

DisplayMode::Enum g_eDisplayMode = DisplayMode::Initial;
unsigned long lastIntanceTime = 0;
bool g_fButtonDown = false;

CBeatChannel beatChanArray[3];

// Standard light display that controls one light per beat channel
CStandardDisplay standardDisplayArray[3];

CLightCycleDisplay cycleLightDisplay;

byte g_cBeatChannelsUsed;

void setup()
{
    byte mode = 0;
    byte pins[3] = { c_pinLight1, c_pinLight2, c_pinLight3 };

    pinMode(c_pinPushButton, INPUT);

    // Standard mode.  One light for part of the sound spectrum.
    standardDisplayArray[0].SetPin(c_pinLight1);
    standardDisplayArray[1].SetPin(c_pinLight2);
    standardDisplayArray[2].SetPin(c_pinLight3);

    // Lights cycle over one part of the spectrum
    cycleLightDisplay.SetPins(pins);

    ToggleMode();

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
        ToggleMode();
    }
    else if (pushButton == HIGH)
    {
        g_fButtonDown = true;
    }

    unsigned long spectrumEnergy[c_cBands];
    SampleSpectrum(c_SpectrumFlagsAll, spectrumEnergy);

    unsigned long elapsedTime = (currentTime - lastIntanceTime);

    bool fEndBeat = (elapsedTime >= c_SampleInstance);

    for (byte i = 0; i < g_cBeatChannelsUsed; i++)
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

    ResetBeatChannels();
}

void ResetBeatChannels()
{
    switch (g_eDisplayMode)
    {
        case DisplayMode::Standard:
        {
            // Standard mode.  One light for part of the sound spectrum.
            standardDisplayArray[0].Reset();
            beatChanArray[0].SetDisplay(&standardDisplayArray[0]);
            beatChanArray[0].SetSpectrumFlags(SPECTRUM_BAND_1 | SPECTRUM_BAND_2 | SPECTRUM_BAND_3);

            standardDisplayArray[1].Reset();
            beatChanArray[1].SetDisplay(&standardDisplayArray[1]);
            beatChanArray[1].SetSpectrumFlags(SPECTRUM_BAND_4 | SPECTRUM_BAND_5);

            standardDisplayArray[2].Reset();
            beatChanArray[2].SetDisplay(&standardDisplayArray[2]);
            beatChanArray[2].SetSpectrumFlags(SPECTRUM_BAND_6 | SPECTRUM_BAND_7);

            g_cBeatChannelsUsed = 3;
        }
        case DisplayMode::Cycle:
        {
            cycleLightDisplay.Reset();
            beatChanArray[0].SetDisplay(&cycleLightDisplay);
            beatChanArray[0].SetSpectrumFlags(SPECTRUM_BAND_1 | SPECTRUM_BAND_2);

            g_cBeatChannelsUsed = 1;
        }
    }
}

