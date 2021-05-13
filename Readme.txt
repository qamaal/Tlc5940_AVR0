
/*  Copyright 2021 Willem E.J. Hoekert. All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    ******************************** Scharre Soft™ *******************************

    This file is part of the Scharre Soft™ Tlc5940_AVR0 Library. This library was 
    developed to work with microcontrollers from the megaAVR® 0-series, in 
    particular the Arduino Nano Every (ATmega4809).

    Most of the code in this library comes from the e-book "Demystifying the 
    TLC5940" that was written by Matthew T. Pandina. I modified the code for the 
    port registers, the timers, the SPI and the ISR, so that it would work with 
    microcontrollers from the megaAVR® 0-series. In addition, I added a parameter 
    (GRAYSCALE_RESOLUTION) that allows one to adjust the PWM frequency, I renamed 
    some functions and parameters so that they resemble those from the Alex Leone 
    library, and I added a small configuration file (Tlc_AVR0_config.h). Most of 
    the modified code can be found in the Tlc_init() function in Tlc5940_AVR0.cpp. 
 
 
    (c) 2021 Willem Hoekert
    Scharre Soft™ 
    Amsterdam, The Netherlands
    ScharreSoft@outlook.com

    ******************************************************************************/
  


    Basic Pin setup: (adapted from the Alex Leone Tlc5940 library)
 
  ----------                                       ---u----
          13|-> SCLK (pin 25)                OUT1 |1     28| OUT channel 0
          12|   Cannot use this pin (NB4)    OUT2 |2     27|-> VPRG (GND or pin 7, see NB2)
          11|-> SIN (pin 26)                 OUT3 |3     26|-> SIN (pin 11)
          10|-> BLANK (pin 23)               OUT4 |4     25|-> SCLK (pin 13)
           9|-> XLAT (pin 24)                  .  |5     24|-> XLAT (pin 9)
           8|                                  .  |6     23|-> BLANK (pin 10)
           7|-> VPRG (NC or pin 27 (NB2))      .  |7     22|-> GND
           6|                                  .  |8     21|-> VCC (+5V)
           5|                                  .  |9     20|-> 2K Resistor -> GND
           4|-> DCPRG (NC or pin 19 (NB2))     .  |10    19|-> DCPRG (GND or pin 4, see NB2)
           3|-> GSCLK (pin 18)                 .  |11    18|-> GSCLK (pin 3)
           2|                                  .  |12    17|-> SOUT
           1|                                  .  |13    16|-> XERR
           0|                               OUT14 |14    15| OUT channel 15
  ----------                                       --------
   ARDUINO 
   Nano Every                                      TLC5940
   ATmega4809 




NB1: The number of TLC5940's (NUM_TLCS) can be changed by editing the 
Tlc_AVR0_config.h file: set NUM_TLCS to the desired number of TLC5940's. 


NB2: If you want to use the manual Dot Correction functions, edit the Tlc_AVR0_config.h 
file: set USE_DC_FUNCTIONS to 1, connect pin D4 of the Arduino to pin 19 of the TLC5940, 
and connect pin D7 of the Arduino to pin 27 of the TLC5940. Enabling the Dot Correction 
functions will require 126 bytes of Flash memory and you will need Arduino pins D4 and D7. 
If you are not going to use Dot Correction, USE_DC_FUNCTIONS should be set to 0, so that 
pin D4 and D7 are available as a GPIO. 


NB3: This library gives you 12 bit greyscale resolution (0 - 4095) at a PWM frequency of 
1,96 KHz. If needed, the PWM frequency can be increased, but the resolution will decrease 
accordingly. Edit the value of GRAYSCALE_RESOLUTION in Tlc_AVR0_config.h

   GRAYSCALE_RESOLUTION	8191	12 bit resolution (0-4095)	1,96 kHz PWM
   GRAYSCALE_RESOLUTION 4095	11 bit resolution (0-2047)	3,92 kHz PWM
   GRAYSCALE_RESOLUTION 2047	10 bit resolution (0-1023)	7,85 kHz PWM
   GRAYSCALE_RESOLUTION 1023	9 bit resolution  (0-511)	15,7 kHz PWM
   GRAYSCALE_RESOLUTION 511	8  bit resolution (0-255)	31,4 kHz PWM
   GRAYSCALE_RESOLUTION 255	7  bit resolution (0-127)	62,8 kHz PWM
   GRAYSCALE_RESOLUTION 127	6  bit resolution (0-63)	125 kHz  PWM


NB4: pin D12 becomes unusable when using this library because of the PORTMUX command that 
is needed to change the hardware SPI pins from the default location. Pin D12 is reserved 
as the MISO pin for the SPI, and can not be used as a GPIO. This problem could be overcome 
by using Bit Banging / softSPI in stead of the hardware SPI, but this is not supported by 
this library.


(C) 2021 Willem Hoekert
Scharre Soft (TM) 
Amsterdam, The Netherlands
ScharreSoft@outlook.com

**********************************************************************************







