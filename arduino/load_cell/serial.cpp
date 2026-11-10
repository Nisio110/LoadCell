#include "serial.h"

void printReading(String name, int value, String unit = ""){
  Serial.print(name);
  Serial.print(": ");
  Serial.print(value);
  Serial.print(unit);
}

void printButtonPressed(String name){
  Serial.print(ANSI_BLUE);
  Serial.print("[ INTERRUPT ] ");
  Serial.print(ANSI_RESET);
  Serial.print(name);
  Serial.print(" button pressed");
  Serial.println();
}

void printError(String description){
  Serial.print(ANSI_RED);
  Serial.print("[ ERROR ] ");
  Serial.print(ANSI_RESET);
  Serial.print(description);
  Serial.println();
}

