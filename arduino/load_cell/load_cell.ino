#include "serial.h"
#include "constants.h"


int offsetMassGrams{0};
int actualMassGrams{0};
int voltsmV{0};
int offsetDU{0};
int adcVal{0};
int freeMem{0};
int calibrationFactor = 0;

bool zeroButtonPressed{0};
bool calibrationButtonPressed{0};

void serialMonitor(int period);
void serialPlotter(int period);
void zeroWeightISR();
void calibrationISR();

void serialMonitor(int period){
  Serial.print(ANSI_PURPLE);
  Serial.print("[ READING ]  ");
  Serial.print(ANSI_RESET);

  printReading("Voltage", voltsmV, "mV");
  Serial.print(", ");

  printReading("ADC", adcVal, "");
  Serial.print(", ");

  printReading("Weight", offsetMassGrams, "g");
  Serial.println();

  delay(period);
}

void serialPlotter(int period){
  Serial.println(offsetMassGrams);
  delay(period);
}


void zeroWeightISR(){
  offsetDU = -adcVal;
  zeroButtonPressed = true;
  delay(debounceMs);
}

void calibrationISR(){
  gPerDU = (float)CALIBRATION_WEIGHT/(adcVal+offsetDU);
  calibrationButtonPressed = true;
  delay(debounceMs);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(zeroPin, INPUT_PULLUP);
  pinMode(calibrationPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(zeroPin), zeroWeightISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(calibrationPin), calibrationISR, FALLING);
}



void loop() {
  // put your main code here, to run repeatedly:
  if (zeroButtonPressed){ 
    zeroButtonPressed = false;
    printButtonPressed("Zero"); 
  }
  else if (calibrationButtonPressed){ 
    printButtonPressed("Calibration");
    Serial.print("[ INFO ] ");
    Serial.print("Calibrating - I hope you put ");
    Serial.print(CALIBRATION_WEIGHT);
    Serial.print(" on the scale........");
    Serial.println();
    calibrationButtonPressed = false; 
  }
  
  adcVal    = analogRead(adcPin);
  offsetMassGrams = round(gPerDU  * (adcVal + offsetDU));
  actualMassGrams = round(gPerDU * adcVal);
  voltsmV   = round(mVPerDU *  adcVal);

  if (actualMassGrams > weightLimitGrams)
    printError("Weight exceeds the limit!");
  else{
    int monitorPeriodMs = 2000;
    int plotPeriodMs = 100;
    PLOT ? serialPlotter(plotPeriodMs) : serialMonitor(monitorPeriodMs);
  }
}
