#ifndef HPDL1414SR_H
#define HPDL1414SR_H
#include <Arduino.h>

#define MAX_CHAR    8

class HPDL1414SR
{
public:
    HPDL1414SR(uint8_t si, uint8_t sck, uint8_t lck);
    int set_char(byte pos, byte c);
    int disp_str(const char str[]);
    void clear(void);
private:
    uint8_t si_pin;
    uint8_t sck_pin;
    uint8_t lck_pin;
};

#endif