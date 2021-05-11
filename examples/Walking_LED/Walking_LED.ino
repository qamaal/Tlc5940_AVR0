
#include <Tlc5940_AVR0.h>

int i=0;

void setup() {
  Tlc_init();
}

void loop() {

  while(1) {
    Tlc_setAll(0);   Tlc_update();
    Tlc_set(i,255);  Tlc_update();
    delay(100);
    if (i<16) i++; else i=0;
  }
}
