MAS6116 Library
===============

This library provides read register, write register and mute functions for the MAS6116 / WM8816 digital volume control.

Using the Library
-----------------

```C++

#include <mas6116.h>

// define preAmp control pins
const int mutePin = A3;
const int csPin = 10;

// preAmp construct
mas6116 preamp(mutePin, csPin);

void setVolume()
{
	preamp.mas6116Write(mas6116RegLeft, leftVol);
	preamp.mas6116Write(mas6116RegRight, rightVol);
	//display volume setting
	lcdPrintThreeNumber(6, volume);
}

void unMute()
{
	if (!backlight)
	{
		backlight = ACTIVE;
		lcd.backlight(); //Turn on backlight
	}
	isMuted = 0;
	preamp.mas6116Mute(HIGH);
	lcd.setCursor(0, 1);
	lcd.print("      ");
}

void mute()
{
	isMuted = 1;
	preamp.mas6116Mute(LOW);
	lcd.setCursor(0, 1);
	lcd.print("Muted ");
}

```
