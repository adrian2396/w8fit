#ifndef EPROM_H 
#define EPROM_H

#include <arduino.h>
#include <EEPROM.h>

// EEPROM memory for  non volatile variables
// Each variable is defined by its first and last position
#define EEPROM_SIZE 96

// SSID as station. 32  character string + terminating ASCII null
const unsigned int EEPROM_SSID_STA_KEY[]  = {0, 32}; 
// Password as station. 32  character string + terminating ASCII null
const unsigned int EEPROM_PASWD_STA_KEY[] = {33, 65};

const unsigned int EEPROM_STEPPER[]  = {0, 32};
const unsigned int WIFI_CONECTED[]  = {0, 32};

bool eeprom_init();
bool eeprom_write(const unsigned int key[], String keyval);
String eeprom_read(const unsigned int key[]);
void eeprom_commit();

#endif