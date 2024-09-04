#include "EERAM_47XXX.hpp"

int config_address = 0b0011000;
int memory_address = 0b1010000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  EERAM_47XXX ram(memory_address, C47X16);
  ram.begin();
  Serial.println("Communications with 47C16");
  MEMORY_OP_STATUS opstat;
  
  uint8_t arr_out[5] = {10, 20, 30, 40, 50};
  uint8_t arr_in[5] = {0};

  // Writing the whole array arr_out into SRAM.
  opstat = ram.write(0, arr_out, 5, false);
  Serial.print("Operation result: ");
  Serial.println(opstat);

  // Retrieving 1 byte (the third). Result should be 30.
  int reading(0);
  reading = ram.read(2);
  Serial.print("Read memory location 3 (address 2), value = ");
  Serial.println(reading);

  opstat = ram.read(0, arr_in, 5);
  Serial.print("Operation result: ");
  Serial.println(opstat);
  bool identical = true;
  for(int i=0; i<5; i++) {
    if(arr_out[i] != arr_in[i]){
      identical = false;
    }
  }
  if(identical) {
    Serial.println("Table written & table read back are identical");
  } else {
    Serial.println("Tables are not identical");
  }
}
void loop() {
  // put your main code here, to run repeatedly:
}
