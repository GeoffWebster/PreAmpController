#include "mas6116.h"
#include <Arduino.h>
#include <SPI.h>

mas6116::mas6116(int mutebPin, int csbPin)
{
    SPI.begin();
	pinMode(mutebPin, OUTPUT);
	digitalWrite(mutebPin, HIGH);
	pinMode(csbPin, OUTPUT);
  	digitalWrite(csbPin, HIGH);
	_csbPin = csbPin;	  
}

void mas6116::mas6116Write(unsigned char mas6116Reg, unsigned char value) {
	// mas6116 datasheet recommends setting "don't care" bits to HIGH
	unsigned char address = (mas6116Reg & mas6116RegMask) | ~(mas6116RegMask | mas6116ReadBit);
	// take the CS pin low to select the vol chip
	digitalWrite(_csbPin, LOW);
	//  send in the address and value via SPI
	SPI.transfer(address);
	SPI.transfer(value);
	// take the CS pin high to deselect the vol chip
	digitalWrite(_csbPin, HIGH);	//PA1;
}

unsigned char mas6116::mas6116Read(unsigned char mas6116Reg) {
	// mas6116 datasheet recommends setting "don't care" bits to HIGH
	unsigned char address = (mas6116Reg & mas6116RegMask) | ~mas6116RegMask | mas6116ReadBit;
	unsigned char result = 0;
	// take the SS pin low to select the chip:
	digitalWrite(_csbPin, LOW);
	//  send in the address via SPI
	SPI.transfer(address);
	// Read value via SPI (any value can be written when reading)
	result = SPI.transfer(0);
	// take the SS pin high to deselect the chip
	digitalWrite(_csbPin, HIGH);//PA1;
	return result;
}
