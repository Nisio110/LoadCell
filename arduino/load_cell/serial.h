#ifndef SERIAL_H
#define SERIAL_H
#include <Arduino.h>

const String ANSI_RED {"\033[31m"};
const String ANSI_RESET {"\033[0m"};
const String ANSI_BLUE {"\033[34m"};
const String ANSI_PURPLE {"\033[35m"};

void printReading(String name, int value, String unit);
void printButtonPressed(String name);
void printError(String description);

#endif
