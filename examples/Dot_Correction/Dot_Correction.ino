

#include <Tlc5940_AVR0.h>

/*
 * To use the Dot Correction functions (6 bit resolution): 
 * 1: Edit Tlc_AVR0_config.h and set USE_DC_FUNCTIONS to 1.
 * 2: Connect Arduino pin D4 to TLC pin 19
 * 3: Connect Arduino pin D7 to TLC pin 27
 */

void setup() {
  Tlc_init();
  
  Tlc_setAll_DC(5);
  Tlc_set_DC(3,1);    // LED in channel 3 will be dimmer than the other LEDS
  Tlc_set_DC(8,63);   // LED in channel 8 will be brighter than the other LEDS
  Tlc_update_DC();
}


void loop() {
  Tlc_setAll(4095);   Tlc_update();  delay(3000);
  Tlc_setAll(2047);   Tlc_update();  delay(3000);
  Tlc_setAll(50);     Tlc_update();  delay(3000);
}
