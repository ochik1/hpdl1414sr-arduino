#include "hpdl1414sr.h"

const uint8_t PIN_SCK = 5; 
const uint8_t PIN_LCK = 6;
const uint8_t PIN_SI = 7;

HPDL1414SR display(PIN_SI, PIN_SCK, PIN_LCK);

void setup() {
  display.disp_str("HELLO HP");
  delay(2000);
  display.clear();
  delay(1000);
}

void loop() {
  static byte pos = 0;
  for(byte c = 0x20; c<0x60; c++) {
    display.set_char(pos, c);
    delay(100);
    pos++;
    if(pos>=8){
      pos = 0;
      delay(500);
    }
  }
}
