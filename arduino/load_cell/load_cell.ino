#include "serial.h"
#include "constants.h"


int offsetMassGrams{0};
int actualMassGrams{0};
int voltsmV{0};
int offsetDU{0};
int adcVal{0};
int freeMem{0};

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

void resetWeightISR(){
  offsetDU = 0;
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
  offsetMassGrams = round(gPerDU  * (adcVal + offsetDU));
  actualMassGrams = round(gPerDU * adcVal);
  voltsmV   = round(mVPerDU *  adcVal);

  if (actualMassGrams > weightLimitGrams)
    printError("Actual weight exceeds the limit!");
  else{
    int monitorPeriodMs = 1500;
    int plotPeriodMs = 100;
    PLOT ? serialPlotter(plotPeriodMs) : serialMonitor(monitorPeriodMs);
  }
}
