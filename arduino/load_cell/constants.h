#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <Arduino.h>

#define PLOT false
constexpr int CALIBRATION_WEIGHT = 496;


float gPerDU = 1.063849; 
// default value, adjustable via calibration

constexpr int adcPin = A1;
constexpr int zeroPin = 2;
constexpr int calibrationPin = 3;

constexpr float mVPerDU = 4.888; // mV/DU
constexpr int weightLimitGrams = 974;
constexpr int debounceMs = 50; // button debounce time

#endif
