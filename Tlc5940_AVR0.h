
/*  Copyright 2010 Matthew T. Pandina. 
    Copyright 2021 Willem E.J. Hoekert. All rights reserved.
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
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR 
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



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


#pragma once

#include "Tlc_AVR0_config.h"
#include <stdint.h>
#include <avr/io.h>

#ifndef USE_DC_FUNCTIONS	    	    // See Tlc_AVR0_config.h 
#define USE_DC_FUNCTIONS 0
#endif

#ifndef NUM_TLCS	        	    	// See Tlc_AVR0_config.h 
#define NUM_TLCS 1
#endif

#ifndef GRAYSCALE_RESOLUTION		    // See Tlc_AVR0_config.h 
#define GRAYSCALE_RESOLUTION 255
#endif

#define SIN_PORTDIR VPORTE.DIR
#define SIN_PORTOUT VPORTE.OUT		    //  PE0
#define SIN_PIN PIN0	    		    //  D11     TLC pin 26   SIN

#define SCLK_PORTDIR VPORTE.DIR
#define SCLK_PORTOUT VPORTE.OUT		    //  PE2
#define SCLK_PIN PIN2		    	    //  D13     TLC pin 25   SCLK

#define BLANK_PORTDIR VPORTB.DIR
#define BLANK_PORTOUT VPORTB.OUT    	//  PB1
#define BLANK_PIN PIN1		        	//  D10     TLC pin 23   BLANK

#define XLAT_PORTDIR VPORTB.DIR
#define XLAT_PORTOUT VPORTB.OUT	    	//  PB0
#define XLAT_PIN PIN0	    	    	//  D9      TLC pin 24   XLAT


#if (USE_DC_FUNCTIONS)

  #define DCPRG_PORTDIR VPORTC.DIR
  #define DCPRG_PORTOUT VPORTC.OUT  	//  PC6
  #define DCPRG_PIN PIN6	    	    //  D4      TLC pin 19   DCPRG

  #define VPRG_PORTDIR VPORTA.DIR
  #define VPRG_PORTOUT VPORTA.OUT	    //  PA1
  #define VPRG_PIN PIN1		        	//  D7      TLC pin 27   VPRG 

#endif


#define setOutput(ddr, pin) ((ddr) |= (1 << (pin)))
#define setLow(port, pin) ((port) &= ~(1 << (pin)))
#define setHigh(port, pin) ((port) |= (1 << (pin)))
#define pulse(port, pin) do { \
    setHigh((port), (pin)); \
    setLow((port), (pin)); \
  } while (0)
  
#define outputState(port, pin) ((port) & (1 << (pin)))

#if (12 * NUM_TLCS > 255)
#define dcData_t uint16_t
#else
#define dcData_t uint8_t
#endif

#if (24 * NUM_TLCS > 255)
#define gsData_t uint16_t
#else
#define gsData_t uint8_t
#endif

#if (16 * NUM_TLCS > 255)
#define channel_t uint16_t
#else
#define channel_t uint8_t
#endif

#define dcDataSize ((dcData_t)12 * NUM_TLCS)
#define gsDataSize ((gsData_t)24 * NUM_TLCS)
#define numChannels ((channel_t)16 * NUM_TLCS)

extern uint8_t dcData[dcDataSize];
extern uint8_t gsData[gsDataSize];
extern volatile uint8_t gsUpdateFlag;

static inline void Tlc_update(void) {
  __asm__ volatile ("" ::: "memory");
  gsUpdateFlag = 1;
}

#if (USE_DC_FUNCTIONS)
  void Tlc_set_DC(channel_t channel, uint8_t value);
  void Tlc_setAll_DC(uint8_t value);
  void Tlc_update_DC(void);
#endif

void Tlc_set(channel_t channel, uint16_t value);
void Tlc_setAll(uint16_t value);
void Tlc_init(void);
