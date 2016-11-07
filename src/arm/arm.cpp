#include "arm.h"

using namespace std;

template <typename T>
T remap(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <typename T>
T pow( T a )
{
    return a*a;
}

template <typename T>
string to_string( T t )
{
    stringstream ss;
    ss << t;
    return ss.str();
}

Arm::Arm()
{
    x=5,y=5;
    _port = 0;
}

Arm::~Arm()
{
    Serial.ClosePort(_port);
}


int Arm::openPort(unsigned short port)
{

    _port = port;
    return Serial.OpenPort( _port, 9600 );//if( _port )

}

void Arm::turn(long double kat)
{
    if(!_port)
        return;

         if( kat > 180.L ) kat = 180.L;
    else if( kat <   0.L ) kat =   0.L;

    int int_kat = round(remap(kat, 0.L, 180.L, 600.L, 2400.L));

    std::string str = to_string(int_kat)+'\n';
    for(char& c : str)
        Serial.SendByte( _port, c );

}

int Arm::move()
{

    long double d1=13.5L,d2=13.5L;
    long double s1,s2;

    try
    {

        if( (x==0.L)&&(y==0.L) ) throw 1;

        if( !(abs( pow(x)+pow(y)-pow(d1)-pow(d2) ) <= 2.L*d1*d2 ) ) throw 1;
        s2 =  acos((pow(x)+pow(y)-pow(d1)-pow(d2))/(2.L*d1*d2) );

        if( !(abs( d2*sin(s2)<=abs( sqrt( pow(x)+pow(y) ) ) ) ) ) throw 2;
        s1 =  atan( y/x ) - asin( ( d2*sin(s2) ) / sqrt( pow(x)+pow(y) ));



        s1 = s1 * 180.L / M_PI;
        s2 = s2 * 180.L / M_PI;

        s1+=90.L; //rotate

        s1+=10.L; //calibration
        s2-=10.L; //calibration

        if( s1<0.L )   s1+=180.L;

        if( s1<90.L&&x<0.L ) throw 1;

        if( s1>90.L&&y<0.L ) throw 1;

    }
    catch( int exc )
    {
        x = old_x;
        y = old_y;
        Serial.SendByte( _port, '\a' );
        return exc;
    }

    turn( s1 );
    turn( s2 );

    old_x = x;
    old_y = y;

    return 0;

}
