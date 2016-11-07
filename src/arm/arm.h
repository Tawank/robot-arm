#ifndef RAMIE_H
#define RAMIE_H
#include <string>
#include <sstream>
#include <cmath>
#include "RS232/RS232.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

class Arm
{
    private:
        RS232::SerialConnection Serial;
        long double old_x,old_y;
        unsigned short _port;
    public:
        Arm();
        ~Arm();
        int openPort(unsigned short);
        void turn(long double);
        int move();
        long double x,y;
};

#endif // RAMIE_H
