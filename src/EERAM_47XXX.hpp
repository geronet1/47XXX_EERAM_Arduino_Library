#include <stdint.h>
/**
 * @file        EERAM_47XXX.hpp
 * @brief       Header file for EERAM_47XXX Arduino Library
 * @author      Jerry Magnin
 * @version     1
 * @date        2024
 * @copyright   BSD 3-Clause
 */


#include "Arduino.h"
#include <Wire.h>

/**
* @enum REG_CONFIG_ADDRESS
* @brief Enumerates the existing registers for configuring the 47XXX Memory Chip
*/
typedef enum {
  STATUS_REG = 0x00, //!< Address of the \c STATUS register
  COMMAND_REG = 0x55 //!< \c COMMAND register, which can be used to either force store to or recall from EEPROM.
} REG_CONFIG_ADDRESS;

/**
* @enum STATUS_WRITE_PROTECT_BITS
* @brief Pre-defines values for \c STATUS register to allow software protection on memory ranges
*/
typedef enum {
  UNPROTECTED = 0b000,
  WP_UPPER_1_64 = 0b001,
  WP_UPPER_1_32 = 0b010,
  WP_UPPER_1_16 = 0b011,
  WP_UPPER_1_8 = 0b100,
  WP_UPPER_1_4 = 0b101,
  WP_UPPER_1_2 = 0b110,
  WP_ALL = 0b111
} STATUS_WRITE_PROTECT_BITS;

typedef enum {
  STORE = 0b00110011,
  RECALL = 0b11011101
} COMMAND_EXEC;

typedef enum {
  C47X04 = 512,
  C47X16 = 2048
} IC_MEM_SIZE;

typedef enum {
  WRITE_THEORY_OK,
  WRITE_OK,
  READ_THEORY_OK,
  ADDRESS_OVER,
  ADDRESS_DNE,
  ADDRESS_ROLLOVER,
} MEMORY_OP_STATUS;

class EERAM_47XXX {
  public:
    // Constructors & destructors
    EERAM_47XXX(int address, IC_MEM_SIZE chipSize);
    EERAM_47XXX(bool A1, bool A2, IC_MEM_SIZE chipSize);

    // Initialisation & basic functions
    void begin();
    int getConfigAddress();

    // Configuration-related functions
    void writeConfigByte(int register_address, int payload);
    void writeStatus();
    void clearConfig();
    int getConfigStatus(void);
    bool setWriteProtect(STATUS_WRITE_PROTECT_BITS wp_mode);
    void removeWriteProtect(void);
    void enableAutoStore();
    void disableAutoStore();
    void resetEventDetector(void);
    bool eventDetected(void);
    bool sramAltered(void);
    void memoryStore(void);
    void memoryRecall(void);

    // Memory storage related functions
    MEMORY_OP_STATUS write(uint16_t addressPointer, uint8_t singleByte);
    MEMORY_OP_STATUS write(uint16_t addressPointer, uint8_t* dataArray, size_t arraySize, bool overwrite);
    uint8_t read();
    uint8_t read(uint16_t addressPointer);
    MEMORY_OP_STATUS read(uint16_t addressPointer, uint8_t* buffer, size_t byteCount);
  private:
    IC_MEM_SIZE m_chipSize;
    int m_config_address;
    int m_address;
    bool m_wire_active;
    int m_config_status_register;
};
