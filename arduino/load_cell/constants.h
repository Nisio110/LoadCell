#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Arduino.h>

#define PLOT false

constexpr int adcPin = A1;
constexpr int zeroPin = 2;
constexpr int resetPin = 3;

constexpr float gPerDU = 1.063849; // g/DU
constexpr float mVPerDU = 4.888; // mV/DU
constexpr int weightLimitGrams = 1008;
constexpr int debounceMs = 100; // button debounce time

#endif
