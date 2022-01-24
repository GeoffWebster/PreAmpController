Audio preamplifier control software
===================================

This is an application for controlling a stereo audio preamplifier. Items covered include
*  Source selection
*  Volume control
*  Volume balance
*  Mute
*  Visual display of above settings
*  Power-off detection with storage of current settings

An Arduino Nano interfaces through a motherboard to the on-board rotary encoder and IR receiver. External interfaces connect to a source select relay board, a balanced digital volume controller / preamplifier board (based on Bruno Putzeys balanced pre-amp with an integrated MAS6116 digital volume control chip) and a 20 x 4 LCD I2C module.

The Quadrature rotary encoder/switch (PEC11R) provides control of the volume as the default mode. Pressing the encoder shaft button switches over to SOURCE SELECT mode and this remains as the active mode till rotary encoder turning is inactive for longer than the TIME_EXITSELECT value (in seconds).

An Infra-red receiver module (TSOP4838) provides remote control of volume level, balance, mute, source select and display on/off. RC5 protocol was chosen as I had a remote transmitter from my existing preamplifier using that code. It's also the protocol used by many freely available transmitters.

The MAS6116 / WM8816 stereo digital volume control provides independently programmable gain of each channel from -111.5dB to +15.5dB together with mute. Communication between the Arduino and the MAS6116 / WM8816 is via an SPI bus.

The 20 x 4 LCD display module with an I2C interface provides Visual data for source input selected, volume level (in large characters), balance setting and mute status.

## Code Libraries
Interfacing with the LCD display, IR decoder and rotary encoder/switch was achieved using freely avalable libraries. These are
* LiquidCrystal_I2C https://github.com/johnrickman/LiquidCrystal_I2C?utm_source=platformio&utm_medium=piohome
* RC5 https://github.com/guyc/RC5
* Rotary https://github.com/CarlosSiles67/Rotary

The library Code for the MAS6116 is present within the lib\mas6116 folder and provides a MAS6116 object constructer,  together with  mas6116write and mas6116read functions.