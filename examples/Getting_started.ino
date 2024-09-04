#include "EERAM_47XXX.hpp"

/*
* You need to define what are going to be the addresses your IC will be using to communicate.
* These addresses are defined on 7 bits. Starting from the right, the 2nd and 3rd are linked
* to the A1 (for 2nd bit) and A2 (for 3rd bit) pins on your IC. Any of these pin tied to VCC
* will have a value of 1, so you need to change the addresses accordingly. If not connected,
* they will be pulled down to GND, and the value will be 0.
*/
int config_address = 0b0011000; // So 0b0011 A2 A1 0
int memory_address = 0b1010000; // And 0b1010 A2 A1 0

void setup() {
  Serial.begin(9600);
  // Create your 47XXX object. You need to assign its memory address part, as
  // configuration address is derived from it.
  // Valid addresses are 0b1010000, 0b1010100, 0b1010110, 0b1010010.
  //
  // You should also provide a "size"
  // Valid values are C47X04 (for 47L04 and 47C04), which are 4kbit,
  // and C47X16 (for 47L16 and 47C16), which are 16kb
  // This value is stored internally, and represents the number of available bytes on your chip.
  // It's used in some functions to ensure you don't ask the chip to store data at a
  // non-existant place.
  EERAM_47XXX ram(memory_address, C47X16);
  // Once created, you should initialize your object with begin()
  ram.begin();
  Serial.println("Communications with 47C16");
  
  // TO-DO: add some code to write a value, and then retrieve it.
  // Or read configuration register, change ASE value, read back.
}
void loop() {
  // put your main code here, to run repeatedly:
}
