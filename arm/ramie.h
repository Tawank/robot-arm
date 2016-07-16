#ifndef RAMIE_H
#define RAMIE_H
#include <string>
#include <sstream>
#include <cmath>
#include "RS232/RS232.h"

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

//template <typename T>
//T remap(T, T, T, T, T);

//template <typename T>
//T p (T);

class Arm
{
    private:
        RS232::SerialConnection Serial;
        unsigned short _port;
        long double old_x,old_y;
    public:
        ~Arm();
        int openPort(unsigned short);
        void turn(long double);
        int movexy();
        long double x,y;
};

#endif // RAMIE_H
