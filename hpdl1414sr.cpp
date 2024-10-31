/*
**  HPDL1414SR
**      X2(8-digits) HPDL1414 with 74HC595 Shift-Register/Latch
*/
/*
    74HC595     U1                              U2
                QA  QB  QC  QD  QE  QF  QG  QH  QA  QB  QC  QD  QE  QF  QG  QH
    HPDL1414    A0  A1  W1  W2  --  --  --  --  D0  D1  D2  D3  D4  D5  D6  --
                U1  U2
*/

#include "hpdl1414sr.h"

int HPDL1414SR::dpos = 0;

HPDL1414SR::HPDL1414SR(uint8_t si, uint8_t sck, uint8_t lck)
{
    si_pin = si;
    sck_pin = sck;
    lck_pin = lck;
    pinMode(si_pin, OUTPUT);
    pinMode(sck_pin, OUTPUT);
    pinMode(lck_pin, OUTPUT);
    clear();
}

// pos: digit position, left to right
int HPDL1414SR::set_char(uint8_t pos, uint8_t c)
{
    byte shift1, shift2;
    if(c<0x20 || c>0x5f || pos>7 || pos<0) {
        return -1;
    } else {
        shift1 = c;
        shift2 = 0x0C | (~pos & 0x3);
        // WRB1 & WRB2 = HIGH
        digitalWrite(lck_pin, LOW);
        shiftOut(si_pin, sck_pin, MSBFIRST, shift1);
        shiftOut(si_pin, sck_pin, MSBFIRST, shift2);
        digitalWrite(lck_pin, HIGH);
        // which WRB1 or WRB2
        if(pos>3) {     // 2nd device
            shift2 &= 0xF7;
        } else {
            shift2 &= 0xFB;
        }
        digitalWrite(lck_pin, LOW);
        shiftOut(si_pin, sck_pin, MSBFIRST, shift1);
        shiftOut(si_pin, sck_pin, MSBFIRST, shift2);
        digitalWrite(lck_pin, HIGH);
        // WRB1 & WRB2 = HIGH
        shift2 |= 0x0C;
        digitalWrite(lck_pin, LOW);
        shiftOut(si_pin, sck_pin, MSBFIRST, shift1);
        shiftOut(si_pin, sck_pin, MSBFIRST, shift2);
        digitalWrite(lck_pin, HIGH);
    }
    return 0;
}

int HPDL1414SR::disp_str(const char str[])
{
    for(int i=0; i<8; i++) {
        if(str[i]=='\0') {
            return 0;
        } else {
            int r = set_char(i, str[i]);
            if(r<0) {
                return r;
            }
        }
    }
    return 0;
}

void HPDL1414SR::clear(void)
{
    for(byte i = 0; i<8; i++){
        set_char(i, 0x20);
    }
}

void HPDL1414SR::scr_init(void)
{
  HPDL1414SR::dpos = -8;
}

int HPDL1414SR::scr_disp(const char* str)
{
  char tmp_str[MAX_CHAR + 1];
  char tmp_c;
  int idx;
  uint8_t set_null = 0;
  uint8_t turn_over = 0;

  for(int8_t i = 0; i<MAX_CHAR; i++) {
    idx = HPDL1414SR::dpos + i;
    if(idx<0) {
      tmp_str[i] = 0x20;
    } else if(set_null) {
      tmp_str[i] = 0x20;
    } else {

      tmp_c = *(str + idx);

      // Last position of the string
      if(tmp_c=='\0') {
        tmp_str[i] = 0x20;
        set_null = 1;
        // Scroll out timing
        if(i==1) {
          turn_over = 1;
        }
      } else {
        tmp_str[i] = tmp_c;
      }
    }
  }
  tmp_str[MAX_CHAR] = '\0';

  // Last position of the string or not
  if(turn_over) {
    HPDL1414SR::dpos = -8;
  } else {
    HPDL1414SR::dpos++;
  }
  
  disp_str(tmp_str);

  return (int)turn_over;
}