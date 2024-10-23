#include "EERAM_47XXX.hpp"

EERAM_47XXX::EERAM_47XXX(int address, IC_MEM_SIZE chipSize): m_address(address), m_chipSize(chipSize) {
  // Config address can be derived from memory address. This makes process simpler for end user
  m_config_address = (m_address & ~(0b1111<<3)) | (0b11<<3);
}

EERAM_47XXX::EERAM_47XXX(bool A1, bool A2, IC_MEM_SIZE chipSize): m_chipSize(chipSize) {
  uint8_t addr_mask = 0;
  if(A1) {
    addr_mask |= 0b1 << 1;
  }
  if(A2) {
    addr_mask |= 0b1 << 2;
  }
  m_config_address = 0b0011000 | addr_mask;
  m_address = 0b1010000 | addr_mask;
}

void EERAM_47XXX::begin() {
  Wire.begin();
  m_wire_active = true;
  getConfigStatus();
}

int EERAM_47XXX::getConfigAddress() {
  return(m_config_address);
}

void EERAM_47XXX::writeConfigByte(int register_address, int payload) {
  Wire.beginTransmission(m_config_address);
  Wire.write(register_address);
  Wire.write(payload);
  Wire.endTransmission();
}

void EERAM_47XXX::writeStatus() {
  writeConfigByte(REG_CONFIG_ADDRESS::STATUS_REG, m_config_status_register);
}

void EERAM_47XXX::clearConfig() {
  m_config_status_register = 0;
  writeStatus();
}

int EERAM_47XXX::getConfigStatus() {
  Wire.beginTransmission(m_config_address);
  Wire.write(REG_CONFIG_ADDRESS::STATUS_REG);
  Wire.endTransmission();
  Wire.requestFrom(m_config_address, 1);
  m_config_status_register = Wire.read();
  return(m_config_status_register);
}

void EERAM_47XXX::setWriteProtect(STATUS_WRITE_PROTECT_BITS wp_mode) {
  uint8_t wp_bits = wp_mode << 2;

  if(!m_wire_active) {
    begin();
  }
  
  // Le ~ (NOT) met tous les autres bits à 1: on part de 0b00011100 à 0b11100011, en AND-ant on clear les bits.
  m_config_status_register = (m_config_status_register & ~(0b111<<2)) | wp_bits;
  writeStatus();
}

void EERAM_47XXX::removeWriteProtect() {
  m_config_status_register = m_config_status_register & ~(0b111<<2);
  writeStatus();
}

void EERAM_47XXX::enableAutoStore() {
  m_config_status_register = (m_config_status_register & ~(0b1<<1)) | 2; // 2 = 0b10
  writeStatus();
}

void EERAM_47XXX::disableAutoStore() {
  m_config_status_register &= ~(0b1<<1);
  writeStatus();
}

void EERAM_47XXX::resetEventDetector() {
  m_config_status_register &= ~(0b1);
  writeStatus();
}

bool EERAM_47XXX::eventDetected() {
  getConfigStatus();
  return(m_config_status_register & 1);
}

bool EERAM_47XXX::sramAltered(void) {
  getConfigStatus();
  return(m_config_status_register & 128); // 128 is 0b10000000
}

void EERAM_47XXX::memoryStore() {
  writeConfigByte(REG_CONFIG_ADDRESS::COMMAND_REG, COMMAND_EXEC::STORE);
}

void EERAM_47XXX::memoryRecall() {
  writeConfigByte(REG_CONFIG_ADDRESS::COMMAND_REG, COMMAND_EXEC::RECALL);
}

MEMORY_OP_STATUS EERAM_47XXX::write(uint16_t addressPointer, uint8_t singleByte) {
  if(addressPointer < 0) {return(MEMORY_OP_STATUS::ADDRESS_DNE);}
  if(addressPointer >= m_chipSize) {return(MEMORY_OP_STATUS::ADDRESS_OVER);}
  Wire.beginTransmission(m_address);
  Wire.write(highByte(addressPointer));
  Wire.write(lowByte(addressPointer));
  Wire.write(singleByte);
  Wire.endTransmission();
  return(MEMORY_OP_STATUS::WRITE_THEORY_OK);
}

MEMORY_OP_STATUS EERAM_47XXX::write(uint16_t addressPointer, uint8_t* dataArray, size_t arraySize, bool overwrite) {
  if(addressPointer < 0) {return(MEMORY_OP_STATUS::ADDRESS_DNE);}
  if(addressPointer >= m_chipSize) {return(MEMORY_OP_STATUS::ADDRESS_OVER);}
  if(!overwrite && (addressPointer + arraySize) >= m_chipSize) {return(MEMORY_OP_STATUS::ADDRESS_ROLLOVER);}
  Wire.beginTransmission(m_address);
  Wire.write(highByte(addressPointer));
  Wire.write(lowByte(addressPointer));
  for(int i=0; i<arraySize; i++) {Wire.write(dataArray[i]);}
  Wire.endTransmission();
  return(MEMORY_OP_STATUS::WRITE_THEORY_OK);
}

uint8_t EERAM_47XXX::read() {
  Wire.beginTransmission(m_address);
  Wire.endTransmission();
  Wire.requestFrom(m_address, 1);
  return(Wire.read());
}

uint8_t EERAM_47XXX::read(uint16_t addressPointer){
  if(addressPointer < 0) {return(MEMORY_OP_STATUS::ADDRESS_DNE);}
  if(addressPointer >= m_chipSize) {return(MEMORY_OP_STATUS::ADDRESS_OVER);}

  Wire.beginTransmission(m_address);
  Wire.write(highByte(addressPointer));
  Wire.write(lowByte(addressPointer));
  Wire.endTransmission();
  Wire.requestFrom(m_address, 1);
  return(Wire.read());
}

/**
* @brief Read an arbitrary number of bytes from memory
* @warning Buffer should be at least the same size than byteCount, otherwise the program will crash
*/
MEMORY_OP_STATUS EERAM_47XXX::read(uint16_t addressPointer, uint8_t* buffer, size_t byteCount=1) {
  if(addressPointer < 0) {return(MEMORY_OP_STATUS::ADDRESS_DNE);}
  if(addressPointer >= m_chipSize) {return(MEMORY_OP_STATUS::ADDRESS_OVER);}

  size_t i(0);
  Wire.beginTransmission(m_address);
  Wire.write(highByte(addressPointer));
  Wire.write(lowByte(addressPointer));
  Wire.endTransmission();
  Wire.requestFrom(m_address, byteCount);
  while(Wire.available()) {
    buffer[i] = Wire.read();
    i++;
  }
  return(MEMORY_OP_STATUS::READ_THEORY_OK);
}