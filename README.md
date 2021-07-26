Audio preamplifier control software
===================================

Arduino Nano is used as the primary circuit and connects via a controller motherboard to an on-board rotary encoder, on-board IR receiver and seperate source select relay board and balanced digital volume controller board.

The MAS6116 / WM8816 stereo digital volume control provides independently programmable gain of each channel from -111.5dB to +15.5dB together with mute.

The PEC11R Quadrature rotary encoder/switch provides control of the volume as default mode. Pressing the encoder shaft button selects SOURCE SELECT mode and this remains as the active mode till rotary encoder turning is inactive for longer than the TIME_EXITSELECT value (in seconds).

An Infra-red receiver module (TSOP4838) provides remote control of volume level, balance, mute, source select and display on/off.