Audio preamplifier control software
===================================

An Arduino Nano is used as the primary circuit and connects via a controller motherboard to an on-board rotary encoder, on-board IR receiver and seperate source select relay board and balanced digital volume controller / preamplifier board (based on Bruno Putzeys balanced pre-amp with an integrated MAS6116 digital volume control chip).

The MAS6116 / WM8816 stereo digital volume control provides independently programmable gain of each channel from -111.5dB to +15.5dB together with mute. Communication between the Arduino and the MAS6116 / WM8816 is via an SPI bus.

The PEC11R Quadrature rotary encoder/switch provides control of the volume as default mode. Pressing the encoder shaft button selects SOURCE SELECT mode and this remains as the active mode till rotary encoder turning is inactive for longer than the TIME_EXITSELECT value (in seconds).

An Infra-red receiver module (TSOP4838) provides remote control of volume level, balance, mute, source select and display on/off.

Visual data showing source input selected, volume level (in large characters), balance setting and mute status is provided using a 20 x 4 LCD dispay with an I2C interface.