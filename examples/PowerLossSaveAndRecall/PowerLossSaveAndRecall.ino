# include "EERAM_47XXX.hpp"

int config_address = 0b0011000;
int memory_address = 0b1010000;

// You may use this function to flag a specific step in your program if you are using a logic analyzer
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
  
  MEMORY_OP_STATUS opstat;

  uint8_t array_out[4] = {10, 20, 40, 80};
  uint8_t array_in[4] = {0};
  uint8_t array_blank[4] = {0};

  pulsePin(2);
  opstat = ram.write(0x0000, array_out, 4, false);

  Serial.println("You have now 10 seconds to switch off and on your EERAM module if you wish to test its save capabilities");

  for(int timeleft=10; timeleft>0; --timeleft) {
    Serial.print(timeleft);
    Serial.println(" remaining");
    delay(1000);
  }

  Serial.println("Checking our SRAM array is backed up");
  pulsePin(2);
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
    Serial.println("GOOD! Stored array and retrieved array are identical");
  } else {
    Serial.println("ERROR! Stored array and retrieved array are different");
  }
  
}
void loop() {
  // Nothing to do
}
