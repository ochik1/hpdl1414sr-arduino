#include "hpdl1414sr.h"
#include <string.h>

const uint8_t PIN_SCK = 5; 
const uint8_t PIN_LCK = 6;
const uint8_t PIN_SI = 7;

char str[64];

#define INTERVAL 20000
unsigned long now, then;

uint8_t state = 0, state_f = 0 ;
uint8_t ds = 0;

HPDL1414SR display(PIN_SI, PIN_SCK, PIN_LCK);

void setup() {
  display.clear();
  delay(1000);
  strcpy(str, "HELLO WORLD! [HPDL1414]");
  display.scr_init();
  now = millis();
  then = now;
}

void loop() {
  now = millis();

  // 20sec interval
  if((now - then)>INTERVAL) {
    then = then + INTERVAL;
    state_f = 1;
    state++;
    if(state>3) {
      state = 0;
    }
  } else {
    state_f = 0;
  }

  if(state_f) {
    switch (state) {
      case 1:
        strcpy(str, "LOOK @ ME");
        break;
      case 2:
        strcpy(str, "MY NAME IS RETRO-DISPLAY #8");
        break;
      case 3:
        strcpy(str, "FREE PALESTINE!");
        break;
      default :
        strcpy(str, "HELLO WORLD! [HPDL1414]");
        break;
    }
    display.scr_init();
  } else {
    display.scr_disp(str);
  }

  delay(500);
}
