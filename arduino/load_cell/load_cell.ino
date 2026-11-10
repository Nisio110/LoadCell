#include "serial.h"
#include "constants.h"


int massGrams{0};
int voltsmV{0};
int offsetDU{0};
int adcVal{0};
int freeMem{0};
int calibrationFactor = 0;

bool zeroButtonPressed{0};
bool resetButtonPressed{0};

void serialMonitor(int period);
void serialPlotter(int period);
void zeroWeightISR();
void resetWeightISR();

void serialMonitor(int period){
  Serial.print(ANSI_PURPLE);
  Serial.print("[ READING ]  ");
  Serial.print(ANSI_RESET);

  printReading("Voltage", voltsmV, "mV");
  Serial.print(", ");

  printReading("ADC", adcVal, "");
  Serial.print(", ");

  printReading("Weight", massGrams, "g");
  Serial.println();

  delay(period); // polling rate
}

void serialPlotter(int period){
  Serial.println(massGrams);
  delay(period); // polling rate
}


void zeroWeightISR(){
  offsetDU = -adcVal;
  zeroButtonPressed = true;
  delay(debounceMs);
}

void resetWeightISR(){
  calibrationFactor = round((adcVal - offsetDU)/upperCalibrationWeight);
  resetButtonPressed = true;
  delay(debounceMs);
  
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(zeroPin, INPUT_PULLUP);
  pinMode(resetPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(zeroPin), zeroWeightISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(resetPin), resetWeightISR, FALLING);
}



void loop() {
  // put your main code here, to run repeatedly:
  if (zeroButtonPressed){ 
    zeroButtonPressed = false;
    printButtonPressed("Zero"); 
  }
  else if (resetButtonPressed){ 
    printButtonPressed("Reset");
    resetButtonPressed = false; 
  }

  adcVal    = analogRead(adcPin);
  massGrams = round(gPerDU  * (adcVal + offsetDU));
  voltsmV   = round(mVPerDU *  adcVal);

  if (!calibrationFactor) {
    massGrams = 0;
    printError("Calibration not performed");
  }
  else{
    massGrams = round((gPerDU  * (adcVal + offsetDU))/calibrationFactor);
  
    if (massGrams > weightLimitGrams)
      printError("Weight exceeds the limit!");
    else{
      int monitorPeriodMs = 2000;
      int plotPeriodMs = 100;
      PLOT ? serialPlotter(plotPeriodMs) : serialMonitor(monitorPeriodMs);
    }
  }
}
