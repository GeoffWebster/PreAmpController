#ifndef MAS6116_H
#define MAS6116_H

#include <Arduino.h>
#include <SPI.h>

// mas6116 register addresses
// Bits 6..3 contain the actual register address
// Bits 7, 1, and 0 are "don't care", set to 0 here
// Bit 2 is 1 for read and 0 for write
const int mas6116RegRight = 0x0E << 3;  // R1 - right channel gain (read/write)
const int mas6116RegLeft = 0x0D << 3;   // R2 - left channel gain (read/write)
const int mas6116RegRef = 0x0C << 3;    // R3 - peak detector reference (read/write)
const int mas6116RegStatus = 0x0B << 3; // R4 - peak detector status (read/write)
const int mas6116RegBoth = 0x09 << 3;   // R5 - both channels gain (write only)
const int mas6116RegMask = 0x0F << 3;   // mask for register address
const int mas6116ReadBit = 0x01 << 2;   // set bit 2 in the address to 1 for read, to 0 for write


class mas6116
{
private:
    /* data */
public:
    int mutebPin;
    int csbPin;
    mas6116(int mutebPin, int csbPin);
    void mas6116Write(unsigned char mas6116Reg, unsigned char value);
    unsigned char mas6116Read(unsigned char mas6116Reg);
};

#endif
