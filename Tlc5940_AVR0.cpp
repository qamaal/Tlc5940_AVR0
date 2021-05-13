    
/*  Copyright 2010 Matthew T. Pandina. 
    Copyright 2021 Willem E.J. Hoekert. All rights reserved.
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY MATTHEW T. PANDINA "AS IS" AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
    EVENT SHALL MATTHEW T. PANDINA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
    INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



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

    The Arduino TLC5940_AVR0 Library is distributed WITHOUT ANY WARRANTY. In no 
    event shall Scharre Soft™ be liable for any direct or indirect damages 
    arising out of the use of this software. Use at your own risk. The original 
    source code was made available by Matthew Pandina under a BSD-style license.
 
    (C) 2021 Willem Hoekert
    Scharre Soft™ 
    Amsterdam, The Netherlands
    ScharreSoft@outlook.com

    ******************************************************************************/


 
#include <avr/interrupt.h>
#include "Tlc5940_AVR0.h"

#if (3 * 16 * NUM_TLCS > 255)
#define channel3_t uint16_t
#else
#define channel3_t uint8_t
#endif

uint8_t dcData[dcDataSize];
uint8_t gsData[gsDataSize];
volatile uint8_t gsUpdateFlag;



#if (USE_DC_FUNCTIONS)

  void Tlc_set_DC(channel_t channel, uint8_t value) {
    channel = numChannels - 1 - channel;
    uint16_t i = (uint16_t)channel * 3 / 4;
  
    switch (channel % 4) {
      case 0:
        dcData[i] = (dcData[i] & 0x03) | (uint8_t)(value << 2);
        break;
      case 1:
        dcData[i] = (dcData[i] & 0xFC) | (value >> 4);
        i++;
        dcData[i] = (dcData[i] & 0x0F) | (uint8_t)(value << 4);
        break;
      case 2:
        dcData[i] = (dcData[i] & 0xF0) | (value >> 2);
        i++;
        dcData[i] = (dcData[i] & 0x3F) | (uint8_t)(value << 6);
        break;
      default: // case 3:
        dcData[i] = (dcData[i] & 0xC0) | (value);
        break;
    }
  }

  void Tlc_setAll_DC(uint8_t value) {
    uint8_t tmp1 = (uint8_t)(value << 2);
    uint8_t tmp2 = (uint8_t)(tmp1 << 2);
    uint8_t tmp3 = (uint8_t)(tmp2 << 2);
    tmp1 |= (value >> 4);
    tmp2 |= (value >> 2);
    tmp3 |= value;
    dcData_t i = 0;
    do {
      dcData[i++] = tmp1; // bits: 05 04 03 02 01 00 05 04
      dcData[i++] = tmp2; // bits: 03 02 01 00 05 04 03 02
      dcData[i++] = tmp3; // bits: 01 00 05 04 03 02 01 00
    } while (i < dcDataSize);
  }

  void Tlc_update_DC(void) {
    cli();
    setHigh(DCPRG_PORTOUT, DCPRG_PIN);
    setHigh(VPRG_PORTOUT, VPRG_PIN);

    for (dcData_t i = 0; i < dcDataSize; i++) {
      // Start transmission
      SPI0.DATA = dcData[i];
      // Wait for transmission complete
      while (!(SPI0.INTFLAGS & SPI_IF_bm))    // waits until data is exchanged
      { ; }
    }
    pulse(XLAT_PORTOUT, XLAT_PIN);
    sei();
  }

#endif



void Tlc_set(channel_t channel, uint16_t value) {
  channel = numChannels - 1 - channel;
  uint16_t i = (uint16_t)channel * 3 / 2;
  
  switch (channel % 2) {
    case 0:
      gsData[i] = (value >> 4);
      i++;
      gsData[i] = (gsData[i] & 0x0F) | (uint8_t)(value << 4);
      break;
    default: // case 1:
      gsData[i] = (gsData[i] & 0xF0) | (value >> 8);
      i++;
      gsData[i] = (uint8_t)value;
      break;
    }
}


void Tlc_setAll(uint16_t value) {
  uint8_t tmp1 = (value >> 4);
  uint8_t tmp2 = (uint8_t)(value << 4) | (tmp1 >> 4);
  gsData_t i = 0;
  do {
    gsData[i++] = tmp1; // bits: 11 10 09 08 07 06 05 04
    gsData[i++] = tmp2; // bits: 03 02 01 00 11 10 09 08
    gsData[i++] = (uint8_t)value; // bits: 07 06 05 04 03 02 01 00
  } while (i < gsDataSize);
}


void Tlc_init(void) {

  setOutput(SCLK_PORTDIR, SCLK_PIN);
  setOutput(XLAT_PORTDIR, XLAT_PIN);
  setOutput(BLANK_PORTDIR, BLANK_PIN);
  setOutput(SIN_PORTDIR, SIN_PIN);

  setLow(SCLK_PORTOUT, SCLK_PIN);
  setLow(XLAT_PORTOUT, XLAT_PIN);
  setHigh(BLANK_PORTOUT, BLANK_PIN);

  #if (USE_DC_FUNCTIONS)
    setOutput(DCPRG_PORTDIR, DCPRG_PIN);
    setOutput(VPRG_PORTDIR, VPRG_PIN);
    setLow(DCPRG_PORTOUT, DCPRG_PIN);
    setHigh(VPRG_PORTOUT, VPRG_PIN);
  #endif
  

  /*----------------------------------------------------------------------  
    Set up hardware SPI, change default location to PE[3:0] using PORTMUX, 
    and set Slave Select Disable bit (SSD) in Control Register B to 1
    (SPI0.CTRLB) to release the SS pin.
  ----------------------------------------------------------------------*/ 
  
  PORTMUX.TWISPIROUTEA |= PORTMUX_SPI01_bm;  /* Change default location */
  SPI0.CTRLB |= SPI_SSD_bm;                  /* set SSD bit */
                     
  SPI0.CTRLA = SPI_CLK2X_bm                  /* Enable double-speed */
             | SPI_ENABLE_bm                 /* Enable module */
             | SPI_MASTER_bm                 /* SPI module in Master mode */
             | SPI_PRESC_DIV4_gc;            /* System Clock divided by 4 */
 
  
  /*---------------------------------------------------------------------------
    Set up 8 MHz PWM signal at pin D3, this will be used as the clock signal 
    for GSCLK.
  ---------------------------------------------------------------------------*/

  PORTMUX.TCBROUTEA |= PORTMUX_TCB1_bm;
  TCB1.CCMPL = 1;   /* PWM Period*/
  TCB1.CCMPH = 1;   /* PWM Compare*/

  TCB1.CTRLB = 0 << TCB_ASYNC_bp        /* Asynchronous Enable: disabled */
               | 1 << TCB_CCMPEN_bp     /* Pin Output Enable: enabled */
               | TCB_CNTMODE_PWM8_gc;   /* 8-bit PWM */
 
  TCB1.CTRLA = TCB_CLKSEL_CLKDIV1_gc;   /* CLK_PER (No Prescaling)      */ 
  TCB1.CTRLA |= TCB_ENABLE_bm;

  
  /*--------------------------------------------------------------  
    Setup Timer interrupt. 
  --------------------------------------------------------------*/

  TCB0.CTRLB = TCB_CNTMODE_INT_gc;         // Periodic Interrupt
  TCB0.CCMP = GRAYSCALE_RESOLUTION;        // Value to compare with
  TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc;      // div1 prescale
  TCB0.CTRLA |= TCB_ENABLE_bm;             // enable timer
  TCB0.INTCTRL = TCB_CAPT_bm;              // Enable the interrupt
}


ISR(TCB0_INT_vect)
{
  static uint8_t xlatNeedsPulse = 0;
  setHigh(BLANK_PORTOUT, BLANK_PIN);

  #if (USE_DC_FUNCTIONS)
    if (outputState(VPRG_PORTOUT, VPRG_PIN)) {
      setLow(VPRG_PORTOUT, VPRG_PIN);
      if (xlatNeedsPulse) {
        pulse(XLAT_PORTOUT, XLAT_PIN);
        xlatNeedsPulse = 0;
      }
      pulse(SCLK_PORTOUT, SCLK_PIN);
    } 
    else  if (xlatNeedsPulse) {
      pulse(XLAT_PORTOUT, XLAT_PIN);
      xlatNeedsPulse = 0;
    }
  #endif 

  #if !(USE_DC_FUNCTIONS)
    if (xlatNeedsPulse) {
      pulse(XLAT_PORTOUT, XLAT_PIN);
      xlatNeedsPulse = 0;
    }
  #endif  
 
  setLow(BLANK_PORTOUT, BLANK_PIN);

  // Below this we have 4096 cycles to shift in the data for the next cycle

  if (gsUpdateFlag) {
    for (gsData_t i = 0; i < gsDataSize; i++) {
      SPI0.DATA = gsData[i];
      while (!(SPI0.INTFLAGS & SPI_IF_bm)) /* waits until data is exchanged*/
      { ; }
    }
    xlatNeedsPulse = 1;
    gsUpdateFlag = 0;
  }  

  TCB0.INTFLAGS = TCB_CAPT_bm;      // Clear interrupt flag
}
