#include <iostream>
#include <cstdlib>
#include <conio.h>

#include "arm/ramie.h"
#include "twitch/twitch.h"
#include "sfml.h"

Arm Ramie;

int main()
{
    unsigned short port;
    unsigned char znak;

    std::cout << "Podaj port: ";
    std::cin >> port;
    Ramie.openPort( port );

    std::cout << "Metoda kontroli(0-cout, 1-getcho, 2-getchxy, 3-mousesfml, 4-twitch): ";
    int kontrol;
    std::cin >> kontrol;

    if ( kontrol==0 )
    {
        int kat=90;
        while( 1 )
        {
            std::cin >> kat;
            Ramie.turn(kat);
        }
    }

    if ( kontrol==1 )
    {
        int kat1=90,kat2=90;
        long double rozd=1.0L;
        while( 1 )
        {
            znak = getch();
            if(znak=='a')      kat1-=rozd;
            else if(znak=='d') kat1+=rozd;
            else if(znak=='s') kat2-=rozd;
            else if(znak=='w') kat2+=rozd;
            else if(znak=='q') rozd--;
            else if(znak=='e') rozd++;
            Ramie.turn(kat1);
            Ramie.turn(kat2);
            system("cls");
            std::cout << kat1 << " " << kat2 << " " << rozd;
        }
    }

    if ( kontrol==2 )
    {
        long double rozd=1.0L;
        Ramie.x=5.0L;
        Ramie.y=5.0L;
        while( 1 )
        {
            znak = getch();

                 if(znak=='4') Ramie.x-=rozd;
            else if(znak=='6') Ramie.x+=rozd;

            else if(znak=='2') Ramie.y-=rozd;
            else if(znak=='8') Ramie.y+=rozd;

            else if(znak=='1') {Ramie.x-=rozd;Ramie.y-=rozd;
            }
            else if(znak=='3') {Ramie.x+=rozd;Ramie.y-=rozd;}
            else if(znak=='7') {Ramie.x-=rozd;Ramie.y+=rozd;}
            else if(znak=='9') {Ramie.x+=rozd;Ramie.y+=rozd;}

            else if(znak=='-') rozd-=0.125;
            else if(znak=='+') rozd+=0.125;

            system("cls");
            std::cout << Ramie.movexy() << std::endl;
//            std::cout << Ramie.s1<< " " << Ramie.s2 << std::endl;
            std::cout << Ramie.x << " " << Ramie.y  << " " << rozd;
        }
    }

    if ( kontrol==3 )
    {
        Ramie.x=5.0L;
        Ramie.y=5.0L;

        Sfml sfml;
        sfml.run();
    }
    if ( kontrol==4 )
    {
        Ramie.x=5.0L;
        Ramie.y=5.0L;
        twitch( "lokiete", "#lokiete" );
    }

    return 0;

}
