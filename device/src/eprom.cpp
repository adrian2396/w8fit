#include "eprom.h"

bool eeprom_init(){
  return (EEPROM.begin(EEPROM_SIZE));
}
void eeprom_commit()
{
  EEPROM.commit();
}

//-----------------------------------------------------------------------------
// Write a key value in EEPROM. Key values are strings
//  key[0] => First byte position of the key in EEPROM 
//  key[1] => Last byte position of the key in EEPROM 
// The value is a null terminated string. All the characters just after 
// the null terminator until the size of the key must be filled with 0xAA 
// to identify a correct key.
// Returns true if the key value fits into the key reserved space and false 
// otherwise.
//-----------------------------------------------------------------------------
bool eeprom_write(const unsigned int key[], String keyval){
  int i; // Index in the keyval string

  // Return false if the keyval does not fit 
  if (keyval.length() > key[1] - key[0])
    return false;
  
  // Write each character in the string including the ASCII null
  for (i = 0; i < keyval.length(); i++)
    EEPROM.write(key[0] + i, keyval.charAt(i));
  EEPROM.write(key[0] + i, '\0');

  // Fill the remaining bytes with the value 0xAA
  i++;
  while (key[0] + i <= key[1]){
    EEPROM.write(key[0] + i, 0xAA);
    i++;
  }
  return true;
}

//-----------------------------------------------------------------------------
// Read the value stored in EEPROM for a key. Each key has two components:
//  key[0] => First byte position of the key in EEPROM 
//  key[1] => Last byte position of the key in EEPROM 
// The value is a null terminated string. All the characters just after the null 
// terminator until the size of the key must be filled with 0xAA to identify a 
// correct key.
// Returns the key value, or an empty string if the key is not valid .
//-----------------------------------------------------------------------------
String eeprom_read(const unsigned int key[]){
  String keyval; 
  bool keyend = false; // The key value has reached its end?
  char string[2] ; 

  // For each byte stored in the EEPROM reserved space for the key
  for (int i = key[0]; i <= key[1]; i++){
    sprintf(string, "%c", EEPROM.read(i)); // Read the byte from EEPROM
    if (keyend && string[0] != 0xAA) // The remainder bytes of the key must be filled up with 0xAA
      return "";
    if (!keyend)  // Add character to key value wntil the end of the string
      keyval += string[0];
    if (string[0] == '\0') // Detect the end of the string
      keyend = true;
  }
  if (!keyend)  return "";
  
  return keyval;
}
