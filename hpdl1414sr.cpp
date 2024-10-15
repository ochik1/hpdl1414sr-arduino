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
int HPDL1414SR::set_char(byte pos, byte c)
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