// Utilities for interacting with a spectrum shield

#pragma once

const byte c_cBands = 7;

#define SPECTRUM_BAND_1     1
#define SPECTRUM_BAND_2     2
#define SPECTRUM_BAND_3     4
#define SPECTRUM_BAND_4     8
#define SPECTRUM_BAND_5     16
#define SPECTRUM_BAND_6     32
#define SPECTRUM_BAND_7     64

void SetupSpectrum();
void SampleSpectrum(unsigned long * piLow, unsigned long * piHigh);
void SampleSpectrum(byte spectrumFlags, unsigned long results[c_cBands]);

