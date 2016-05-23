#include "ramie.h"

using namespace std;

long double map(long double x, long double in_min, long double in_max, long double out_min, long double out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long double p( long double a )
{
    return a*a;
}

ramie::openPort(unsigned short port)
{

    _port = port;

    return Serial.OpenPort( port, 9600 );

}

void ramie::obrot(long double kat)
{

    if( kat > 180 ) kat = 180;
    else if( kat < 0 ) kat = 0;
    unsigned char bufor[5];
    char ckat[5];

    sprintf( ckat, "%d", (int)round(map(kat, 0, 180, 600, 2400)) );

    strcpy( (char*)bufor, ckat );

    bufor[strlen((char*)bufor)+1] = '\0';
    bufor[strlen((char*)bufor)] = '\n';

    for( unsigned int i=0; i<strlen((char*)bufor); i++ )
        Serial.SendByte( _port, bufor[i] );

}

void ramie::blad()
{
    x = old_x;
    y = old_y;
}

int ramie::ruchxy()
{

    if( (x==0)&&(y==0) ) { blad(); return 1; }

    long double s1,s2;
    long double d1=13.50,d2=13.50;

    if( !(abs( p(x)+p(y)-p(d1)-p(d2) ) <= 2*d1*d2 ) ) { blad(); return 1; }
    s2 =  acos((p(x)+p(y)-p(d1)-p(d2))/(2*d1*d2) );

    if( !(abs( d2*sin(s2)<=abs( sqrt( p(x)+p(y) ) ) ) ) ) { blad(); return 2; }
    s1 =  atan( y/x ) - asin( ( d2*sin(s2) ) / sqrt( p(x)+p(y) ));

    s1 = s1 * 180.0 / M_PI;
    s2 = s2 * 180.0 / M_PI;

    s1+=90; //obrocenie

    s1+=10; //kalibracja
    s2-=10; //kalibracja

    if( s1<0.0 )   s1+=180.0;

    if( s1<90.0&&x<0 ) { blad(); return 1; }

    if( s1>90.0&&y<0 ) { blad(); return 1; }

    std::cout << s1 << " " << s2 << std::endl;

    obrot( s1 );
    obrot( s2 );

    old_x = x;
    old_y = y;

    return 0;

}


