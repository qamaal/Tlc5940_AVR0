
/*  Copyright 2021 Willem E.J. Hoekert. All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


    /******************************** Scharre Soft™ *******************************

    This file is part of the Scharre Soft™ Tlc5940_AVR0 Library. This library was 
    developed to work with microcontrollers from the megaAVR® 0-series, in 
    particular the Arduino Nano Every (ATmega4809).

    Most of the code in this library comes from the e-book "Demystifying the 
    TLC5940" that was written by Matthew T. Pandina. I modified the code for the 
    port registers, the timers, the SPI and the ISR, so that it would work with 
    microcontrollers from the megaAVR® 0-series. In addition, I added a parameter 
    (GRAYSCALE_RESOLUTION) that allows you to adjust the PWM frequency, I renamed 
    some functions and parameters so that they resemble those from the Alex Leone 
    library, and I added a small configuration file (Tlc_AVR0_config.h). Most of 
    the modified code can be found in the Tlc_init() function in Tlc5940_AVR0.cpp. 

 
    (c) 2021 Willem Hoekert
    Scharre Soft™ 
    Amsterdam, The Netherlands
    ScharreSoft@outlook.com

    ******************************************************************************/



#ifndef TLC_AVR0_CONFIG_H
#define TLC_AVR0_CONFIG_H


#define NUM_TLCS 1

/** From the Alex Leone file:
    Number of TLCs daisy-chained.  To daisy-chain, attach the SOUT (TLC pin 17)
    of the first TLC to the SIN (TLC pin 26) of the next.  The rest of the pins
    are attached normally.
    \note Each TLC needs it's own IREF resistor 
*/


#define USE_DC_FUNCTIONS 0	

/** If you want to use Dot Correction functions, set USE_DC_FUNCTIONS to 1.
    In addition, connect pin D4 of the Arduino to pin 19 of the TLC5940, 
    and connect pin D7 of the Arduino to pin 27 of the TLC5940. Enabling and 
    using the Dot Correction functions will require around 120 bytes of Flash 
    memory and you will need Arduino pins D4 and D7. When not using the Dot
    Correction functions, connect pin 19 and 27 of the TLC5940 to GND.
*/


#define GRAYSCALE_RESOLUTION 8191

/** Higher PWM rates can be achieved by lowering the grayscale resolution. 
	
   GRAYSCALE_RESOLUTION	8191	12 bit resolution (0-4095)	1,96 kHz PWM
   GRAYSCALE_RESOLUTION 4095	11 bit resolution (0-2047)	3,92 kHz PWM
   GRAYSCALE_RESOLUTION 2047	10 bit resolution (0-1023)	7,85 kHz PWM
   GRAYSCALE_RESOLUTION 1023	9 bit resolution  (0-511)	15,7 kHz PWM
   GRAYSCALE_RESOLUTION 511	8  bit resolution (0-255)	31,4 kHz PWM
   GRAYSCALE_RESOLUTION 255	7  bit resolution (0-127)	62,8 kHz PWM
   GRAYSCALE_RESOLUTION 127	6  bit resolution (0-63)	125 kHz  PWM
*/


#endif



