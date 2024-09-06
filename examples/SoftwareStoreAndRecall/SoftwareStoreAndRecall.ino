# include "EERAM_47XXX.hpp"

int config_address = 0b0011000;
int memory_address = 0b1010000;

// You may use this function to flag a specific step in your program if you are using a logic analyzer.
// It will trigger a short pulse LOW>HIGH>LOW.
void pulsePin(int pin) {
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);

  EERAM_47XXX ram(memory_address, C47X16);
  ram.begin();
  Serial.println("Communications with 47C16");
  
  // We're note using opstat in this code because error checking is not totally operational.
  // It's previsional for a future version of the lib.
  MEMORY_OP_STATUS opstat;

  uint8_t array_out[4] = {10, 20, 40, 80};
  uint8_t array_in[4] = {0};
  uint8_t array_blank[4] = {0};

  pulsePin(2);
  opstat = ram.write(0x0000, array_out, 4, false);

  pulsePin(2);
  ram.memoryStore();
  Serial.println("We stored some data in SRAM and backed up in EEPROM");
  Serial.println("Now wiping the data we just wrote, so we only have the copy in the EEPROM");

  ram.write(0x0000, array_blank, 4, false);
  Serial.println("Cleared, let's check its first 4 bytes");
  ram.read(0x0000, array_in, 4);
  for(int i=0; i<4; i++) {
    Serial.print("Array i = ");
    Serial.print(array_in[i]);
    Serial.println(" ");
  }

  Serial.println("Now recalling data from EEPROM");
  pulsePin(2);
  ram.memoryRecall();
  Serial.println("We should be done now. Checking");
  ram.read(0x0000, array_in, 4);
  bool identical = true;
  for(int i=0; i<4; i++) {
    if(array_in[i]!=array_out[i]) {
      identical = false;
    }
    Serial.print("Array i = ");
    Serial.print(array_in[i]);
    Serial.println(" ");
  }
  if(identical) {
    // Hint: It should be good…
    Serial.println("GOOD! Stored array and retrieved array are identical");
  } else {
    Serial.println("ERROR! Stored array and retrieved array are different");
  }

  
}
void loop() {
  // Nothing to do here
}
