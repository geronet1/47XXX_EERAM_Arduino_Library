# 47XXX EERAM Arduino Library

## Description
This Arduino Library is for the Microchip 47XXX-series Memory ICs (see [their datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/20005371C.pdf)).<br/>
These memories are SRAM Arrays with an EEPROM backup, which can be triggered by a power loss (if you provide the chip a dedicated capacitor), a single pin change from any source or an I2C instruction.

There are four variants available. 2 of them are 2.7 to 3.6VDC powered (labelled 47LXX), and 2 are 4.5 to 5.5VDC powered (47CXX).
For each of these power category, there are 2 memory sizes available, 4Kbits (47X04) and 16Kbits (47X16).

These devices use I2C protocol, 2 pins are needed to interface.

## Usage
TO-DO: add examples

## License
This library is licenced under BSD 3-clause licence.

## Project status
Under development
