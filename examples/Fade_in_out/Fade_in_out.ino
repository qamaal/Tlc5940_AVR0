

#include <Tlc5940_AVR0.h>



void setup() {
  Tlc_init();
}

void loop() {

  for (int i=0; i<255; i++) {
    Tlc_setAll(i); 
     Tlc_update();  delay(5);
  }

  for (int i=255; i>=0; i--) {
    Tlc_setAll(i); 
    Tlc_update();  delay(5);
  }

}
