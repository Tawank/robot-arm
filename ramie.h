#ifndef RAMIE_H
#define RAMIE_H
#include <iostream>
#include <cstdio>
#include <cmath>
#include "RS232/RS232.h"

long double map(long double, long double, long double, long double, long double);

long double p  (long double);

class ramie
{
    private:
        RS232::SerialConnection Serial;
        unsigned short _port;
        long double old_x, old_y;
        void blad();
    public:
        openPort(unsigned short);
        void obrot(long double);
        int ruchxy();
        long double x, y;
};

#endif // RAMIE_H
