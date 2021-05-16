Tlc5940_AVR0

A Tlc5940 library for the Arduino Nano Every (Atmega4809)

I was looking for a TLC5940 library that would work on the Arduino Nano Every, but I could not find one. Using the e-book "Demystifying the TLC5940" that was written by Matthew T. Pandina, I was able to make my own library. I tested the library with a maximum of two daisy chained TLC5940's and it worked fine. The library uses timers TCB0 and TCB1.

I haven't tried it but I think it could be adapted to work on other microcontrollers from the megaAVR® 0-series, the tinyAVR® 0-series and the tinyAVR® 1-series.


Features:
- Dot Correction
- PWM frequency can be increased (grayscale resolution will decrease accordingly)
- Pin compatible with the Arduino Nano v3


Drawback:
- Arduino pin D12 becomes unusable as a GPIO


To install this library in the Arduino IDE follow these steps:

- Download ZIP file and extract the ZIP file that is in the ZIP file.
- Rename the extracted ZIP file to Tlc5940_AVR0.zip.
- In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library.
- Navigate to the .zip file's location and open it.
 
