#include <Arduino.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <RC5.h>
#include <rotary.h>
#include <mas6116.h>
#include "custom.h"

/******* MACHINE STATES *******/
#define STATE_RUN 0 // normal run state
#define STATE_IO 1	// when user selects input/output
#define STATE_OFF 4 //when power down
#define ON LOW
#define OFF HIGH
#define STANDBY 0 //Standby
#define ACTIVE 1  //Active

#define EEPROM_FIRST_USE 0 //EEPROM location: First use
#define EEPROM_VOLUME 1	   //EEPROM location: volume
#define EEPROM_SOURCE 2	   //EEPROM location: source
#define EEPROM_BALANCE 3   //EEPROM location: balance

#define TIME_EXITSELECT 5 //** Time in seconds to exit I/O select mode when no activity

#define printByte(args) write(args);

/******* TIMING *******/
unsigned long milOnButton;	// Stores last time for switch press
unsigned long milOnAction;	// Stores last time of user input
unsigned long milOnFadeIn;	// LCD fade timing
unsigned long milOnFadeOut; // LCD fade timing

/********* Global Variables *******************/
unsigned char volume;	 // current volume, between 0 and VOL_STEPS
unsigned char leftVol;	 // current left volume
unsigned char rightVol;	 // current right volume
unsigned char balance;	 // current balance (0<>20)
unsigned char backlight; // current backlight state
int counter = 0;
unsigned char source = 1;	 // current input channel
unsigned char oldsource = 1; // previous input channel
unsigned char oldtoggle;
unsigned char isMuted;	 // current mute status
unsigned char state = 0; // current machine state
unsigned char buttonState;
bool btnstate = 0;
unsigned char oldbtnstate = 0;
unsigned char rotarystate; // current rotary encoder status
unsigned char result = 0;  //current rotary status

int analogPin = A1;

const char *inputName[] = {
	"Phono ",
	"Media ",
	"CD    ",
	"Tuner "}; // Elektor i/p board

char buffer1[20] = "";
char balLeft[11] = {21, 20, 19, 18, 16, 15, 13, 11, 8, 5, 0};
char balRight[11] = {0, 5, 8, 11, 13, 15, 16, 18, 19, 20, 21};

// LCD construct
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 20 chars and 4 line display

// define encoder pins
const char encoderPinA = 6;
const char encoderPinB = 5;
const char encoderbtn = 7;
// Rotary construct
Rotary rotary = Rotary(encoderPinA, encoderPinB, encoderbtn);

// define IR input
unsigned int IR_PIN = 8;
unsigned long t0;
//RC5 construct
RC5 rc5(IR_PIN);

// define preAmp control pins
const int mutebPin = A3;
const int csbPin = 10;
// preAmp construct
mas6116 preamp(mutebPin, csbPin);

// Function prototypes
void RC5Update(void);
void setIO();
void volumeUpdate();
void buttonPressed();
void setVolume();
void sourceUpdate();
void mute();
void unMute();
void toggleMute();
void balanceUpdate();
void defineCustomChars();
void lcdPrintThreeNumber(unsigned char column, unsigned char number);
void saveIOValues();
void lcdPrintBal();
void lcdPrintSpaces();

ISR(ANALOG_COMP_vect)
{
	saveIOValues();
	backlight = STANDBY;
	lcd.noDisplay();
	lcd.noBacklight(); //Turn off backlight
	mute();			   //mute output
	state = STATE_OFF;
}

void saveIOValues()
{
	EEPROM.update(EEPROM_VOLUME, volume);
	EEPROM.update(EEPROM_SOURCE, source);
	EEPROM.update(EEPROM_BALANCE, balance);
}

void lcdPrintBal()
{
	sprintf(buffer1, "      ");
	lcd.setCursor(0, 3);
	lcd.printstr(buffer1);
	sprintf(buffer1, "Bal %d", balance - 5);
	lcd.setCursor(0, 3);
	lcd.printstr(buffer1);
}

void defineCustomChars()
{
	lcd.createChar(0, cc0);
	lcd.createChar(1, cc1);
	lcd.createChar(2, cc2);
	lcd.createChar(3, cc3);
	lcd.createChar(4, cc4);
	lcd.createChar(5, cc5);
	lcd.createChar(6, cc6);
	lcd.createChar(7, cc7);
}

void lcdPrintSpaces()
{
	lcd.printByte(A); // Blank
	lcd.printByte(A); // Blank
	lcd.printByte(A); // Blank
	lcd.printByte(A); // Blank
}

void lcdPrintThreeNumber(unsigned char column, unsigned char number)
{
	unsigned char highnumber;
	unsigned char midnumber;
	unsigned char lownumber;
	highnumber = number / 100;
	midnumber = (number - (highnumber * 100)) / 10;
	lownumber = (number - (highnumber * 100) - (midnumber * 10));
	lcd.setCursor(column, 0);
	if (highnumber == 0)
	{
		lcdPrintSpaces();
	}
	else
	{
		lcd.printByte(bn1[highnumber * 4]);
		lcd.printByte(bn1[highnumber * 4 + 1]);
		lcd.printByte(bn1[highnumber * 4 + 2]);
		lcd.printByte(bn1[highnumber * 4 + 3]);
	}
	lcd.printByte(A); // Blank
	lcd.printByte(bn1[midnumber * 4]);
	lcd.printByte(bn1[midnumber * 4 + 1]);
	lcd.printByte(bn1[midnumber * 4 + 2]);
	lcd.printByte(bn1[midnumber * 4 + 3]);
	lcd.printByte(A); // Blank
	lcd.printByte(bn1[lownumber * 4]);
	lcd.printByte(bn1[lownumber * 4 + 1]);
	lcd.printByte(bn1[lownumber * 4 + 2]);
	lcd.printByte(bn1[lownumber * 4 + 3]);
	lcd.setCursor(column, 1);
	if (highnumber == 0)
	{
		lcdPrintSpaces();
	}
	else
	{
		lcd.printByte(bn2[highnumber * 4]);
		lcd.printByte(bn2[highnumber * 4 + 1]);
		lcd.printByte(bn2[highnumber * 4 + 2]);
		lcd.printByte(bn2[highnumber * 4 + 3]);
	}
	lcd.printByte(A); // Blank
	lcd.printByte(bn2[midnumber * 4]);
	lcd.printByte(bn2[midnumber * 4 + 1]);
	lcd.printByte(bn2[midnumber * 4 + 2]);
	lcd.printByte(bn2[midnumber * 4 + 3]);
	lcd.printByte(A); // Blank
	lcd.printByte(bn2[lownumber * 4]);
	lcd.printByte(bn2[lownumber * 4 + 1]);
	lcd.printByte(bn2[lownumber * 4 + 2]);
	lcd.printByte(bn2[lownumber * 4 + 3]);
	lcd.setCursor(column, 2);
	if (highnumber == 0)
	{
		lcdPrintSpaces();
	}
	else
	{
		lcd.printByte(bn3[highnumber * 4]);
		lcd.printByte(bn3[highnumber * 4 + 1]);
		lcd.printByte(bn3[highnumber * 4 + 2]);
		lcd.printByte(bn3[highnumber * 4 + 3]);
	}
	lcd.printByte(A); // Blank
	lcd.printByte(bn3[midnumber * 4]);
	lcd.printByte(bn3[midnumber * 4 + 1]);
	lcd.printByte(bn3[midnumber * 4 + 2]);
	lcd.printByte(bn3[midnumber * 4 + 3]);
	lcd.printByte(A); // Blank
	lcd.printByte(bn3[lownumber * 4]);
	lcd.printByte(bn3[lownumber * 4 + 1]);
	lcd.printByte(bn3[lownumber * 4 + 2]);
	lcd.printByte(bn3[lownumber * 4 + 3]);
	lcd.setCursor(column, 3);
	if (highnumber == 0)
	{
		lcdPrintSpaces();
	}
	else
	{
		lcd.printByte(bn4[highnumber * 4]);
		lcd.printByte(bn4[highnumber * 4 + 1]);
		lcd.printByte(bn4[highnumber * 4 + 2]);
		lcd.printByte(bn4[highnumber * 4 + 3]);
	}
	lcd.printByte(A); // Blank
	lcd.printByte(bn4[midnumber * 4]);
	lcd.printByte(bn4[midnumber * 4 + 1]);
	lcd.printByte(bn4[midnumber * 4 + 2]);
	lcd.printByte(bn4[midnumber * 4 + 3]);
	lcd.printByte(A); // Blank
	lcd.printByte(bn4[lownumber * 4]);
	lcd.printByte(bn4[lownumber * 4 + 1]);
	lcd.printByte(bn4[lownumber * 4 + 2]);
	lcd.printByte(bn4[lownumber * 4 + 3]);
};

void setIO()
{
	digitalWrite(oldsource, LOW);
	digitalWrite(source, HIGH);
	lcd.setCursor(0, 0);
	lcd.print(inputName[source - 1]);
}

void RotaryUpdate()
{
	switch (state)
	{
	case STATE_RUN:
		volumeUpdate();
		break;
	case STATE_IO:
		sourceUpdate();
		break;
	default:
		break;
	}
}

void volumeUpdate()
{
	//0 = no rotation, 10 = clockwise,  20 = counter clockwise
	//result = rotary.process();
	switch (rotary.process())
	{
	case 0:
		// check button
		buttonPressed();
		break;
	case DIR_CW:
		if (volume < 234)
		{
			if (isMuted)
			{
				unMute();
			}
			volume = volume + 1;
			setVolume();
		}
		break;
	case DIR_CCW:
		if (volume != 0)
		{
			if (isMuted)
			{
				unMute();
			}
			volume = volume - 1;
			setVolume();
		}
	default:
		break;
	}
}

void setVolume()
{
	balanceUpdate();
	preamp.mas6116Write(mas6116RegLeft, leftVol);
	preamp.mas6116Write(mas6116RegRight, rightVol);
	//display volume setting
	lcdPrintThreeNumber(6, volume);
}

void balanceUpdate(void)
{
	leftVol = volume + balLeft[balance];
	rightVol = volume + balRight[balance];
}

// button pressed routine
void buttonPressed()
{
	if (rotary.buttonPressedReleased(20))
	{
		switch (state)
		{
		case STATE_RUN:
			state = STATE_IO;
			milOnButton = millis();
			break;
		default:
			break;
		}
	}
}

void sourceUpdate()
{
	//0 = do nothing, 10 = clockwise,  20 = counter clockwise
	//result = rotary.process();
	switch (rotary.process())
	{
	case DIR_CW:
		oldsource = source;
		milOnButton = millis();
		if (oldsource < 4)
		{
			source++;
		}
		else
		{
			source = 1;
		}
		setIO();
		break;
	case DIR_CCW:
		oldsource = source;
		milOnButton = millis();
		if (source > 1)
		{
			source--;
		}
		else
		{
			source = 4;
		}
		setIO();
		break;
	default:
		break;
	}
}

void RC5Update()
{
	unsigned char toggle;
	unsigned char address;
	unsigned char command;
	// Poll for new RC5 command
	if (rc5.read(&toggle, &address, &command))
	{
		if (address == 0x10)
		{
			switch (command)
			{
			case 1:
				// Phono
				if ((oldtoggle != toggle))
				{
					oldsource = source;
					source = 1;
					setIO();
				}
				break;
			case 3:
				// Tuner
				if ((oldtoggle != toggle))
				{
					oldsource = source;
					source = 4;
					setIO();
				}
				break;
			case 7:
				// CD
				if ((oldtoggle != toggle))
				{
					oldsource = source;
					source = 3;
					setIO();
				}
				break;
			case 8:
				// Media
				if ((oldtoggle != toggle))
				{
					oldsource = source;
					source = 2;
					setIO();
				}
				break;
			case 13:
				//Mute
				if ((oldtoggle != toggle))
				{
					toggleMute();
				}
				break;
			case 16:
				//Increase Vol
				if (volume < 234)
				{
					if (isMuted)
					{
						unMute();
					}
					volume = volume + 1;
					setVolume();
				}
				break;
			case 17:
				//Reduce Vol
				if (volume != 0)
				{
					if (isMuted)
					{
						unMute();
					}
					volume = volume - 1;
					setVolume();
				}
				break;
			case 32:
				//up button pressed - balance adjust right
				if ((oldtoggle != toggle))
				{
					if (balance < 10) // not yet fully right
					{
						balance = balance + 1;
						lcdPrintBal();
						setVolume();
					}
				}
				break;
			case 33:
				//down button pressed - balance adjust left
				if ((oldtoggle != toggle))
				{
					if (balance != 0) //not yet fully left
					{
						balance = balance - 1;
						lcdPrintBal();
						setVolume();
					}
				}
				break;
			case 59:
				//Display Toggle
				if ((oldtoggle != toggle))
				{
					lcd.setCursor(0, 2);
					if (backlight)
					{
						backlight = STANDBY;
						lcd.noBacklight(); //Turn off backlight
						mute();			   //mute output
					}
					else
					{
						backlight = ACTIVE;
						lcd.backlight(); //Turn on backlight
						unMute();		 //unmute output
					}
				}
				break;
			}
		}
		else if (address == 0x14)
		{
			if ((oldtoggle != toggle))
			{
				if (command == 53)
				{
					oldsource = source;
					source = 3;
					setIO();
				}
			}
		}
		oldtoggle = toggle;
	}
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

void toggleMute()
{
	if (isMuted)
	{
		unMute();
	}
	else
	{
		mute();
	}
}

void setup()
{
	for (size_t pinOut = 1; pinOut < 5; pinOut++)
	{
		pinMode(pinOut, OUTPUT);
		digitalWrite(pinOut, LOW);
	}
	lcd.init();		 // initialize the lcd
	lcd.backlight(); // turn on LCD backlight
	backlight = 1;
	defineCustomChars();
	lcd.home(); // LCD cursor to home position

	//test for first use settings completed. If not, carry out
	if (EEPROM.read(EEPROM_FIRST_USE))
	{
		// Set vol, source, balance for first time
		EEPROM.write(EEPROM_SOURCE, 1);
		EEPROM.write(EEPROM_VOLUME, 0);
		EEPROM.write(EEPROM_BALANCE, 5);
		EEPROM.write(EEPROM_FIRST_USE, 0x00);
	}

	// Load source, volume, balance values
	volume = EEPROM.read(EEPROM_VOLUME);
	source = EEPROM.read(EEPROM_SOURCE);
	balance = EEPROM.read(EEPROM_BALANCE);

	// Setup Analog Compare Interrupt
	ADCSRB = 0x40;									   //Analog Comparator Multiplexer Enable
	ADCSRA = 0x00;									   //ADC Disabled
	ADMUX = 0x01;									   // Arduino pin A1
	ACSR |= (1 << ACBG) | (1 << ACIS1) | (1 << ACIS0); //Analog Comparator Bandgap Select, Interrupt on rising edge
	ACSR |= (1 << ACIE);							   //Analog Comparator Interrupt enable

	//set startup volume
	setVolume();
	//set source
	setIO();
	//display balance setting
	lcdPrintBal();
	//unmute
	isMuted = 0;
	preamp.mas6116Mute(HIGH);
}

void loop()
{
	RC5Update();
	RotaryUpdate();
	if ((millis() - milOnButton) > TIME_EXITSELECT * 1000)
	{
		state = STATE_RUN;
	}
}