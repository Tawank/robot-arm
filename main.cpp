#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include "RS232/RS232.h"
#include "ramie.h"
#include "twitch.h"

ramie Ramie;

int main()
{
    unsigned short port;
    unsigned char znak;
    int w;
    int kat1=90,kat2=90;
    long double rozd=1.0;

    std::cout << "Podaj port: ";
    std::cin >> port;
    Ramie.openPort( port );

    std::cout << "Metoda kontroli(0-cout, 1-getcho, 2-getchxy, 3-mousesfml, 4-twitch): ";
    int kontrol;
    std::cin >> kontrol;
    if ( kontrol==0 )
        while( 1 )
        {
            std::cin >> kat1;
            Ramie.obrot(kat1);
        }
    if ( kontrol==1 )
        while( 1 )
        {
            znak = getch();
            if(znak=='a')      kat1-=rozd;
            else if(znak=='d') kat1+=rozd;
            else if(znak=='s') kat2-=rozd;
            else if(znak=='w') kat2+=rozd;
            else if(znak=='q') rozd--;
            else if(znak=='e') rozd++;
            Ramie.obrot(kat1);
            Ramie.obrot(kat2);
            system("cls");
            std::cout << kat1 << " " << kat2 << " " << rozd;
        }
    if ( kontrol==2 )
    {
        Ramie.x=5;
        Ramie.y=5;
        while( 1 )
        {
            znak = getch();
                 if(znak=='4') Ramie.x-=rozd;
            else if(znak=='6') Ramie.x+=rozd;

            else if(znak=='2') Ramie.y-=rozd;
            else if(znak=='8') Ramie.y+=rozd;

            else if(znak=='1') {Ramie.x-=rozd;Ramie.y-=rozd;}
            else if(znak=='3') {Ramie.x+=rozd;Ramie.y-=rozd;}
            else if(znak=='7') {Ramie.x-=rozd;Ramie.y+=rozd;}
            else if(znak=='9') {Ramie.x+=rozd;Ramie.y+=rozd;}

            else if(znak=='-') rozd-=0.125;
            else if(znak=='+') rozd+=0.125;
            system("cls");
            w=Ramie.ruchxy();
            if ( w == 1) std::cout << "Blad1" << std::endl;
            if ( w == 2) std::cout << "Blad2" << std::endl;
            std::cout << Ramie.x << " " << Ramie.y << " " << rozd;
        }
    }
    if ( kontrol==3 )
    {
        Ramie.x=5;
        Ramie.y=5;
        sf::Uint8 red   = 255;
        sf::Uint8 green = 0;
        sf::Uint8 blue  = 0;
        sf::Uint8 alpha = 255;
        sf::RenderWindow window(sf::VideoMode(1000, 1000), "RAMIE", sf::Style::Close);
        window.setFramerateLimit(0);
        window.setVerticalSyncEnabled(false);
        window.setKeyRepeatEnabled( true );
        sf::CircleShape shape(4.f);
        shape.setFillColor( sf::Color( red, green, blue, alpha ));
        int r=0;
        unsigned int i;

        std::vector < sf::Vector2i > linia;

        std::vector < sf::Vector2f > nagranie;

        sf::Vector2i offset(0,0);// = sf::Mouse::getPosition(window);
        sf::Vector2i mouse(0,0);

        while (window.isOpen())
        {
            sf::Event event;
            while ( window.pollEvent(event) )
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                    }

                    if ((event.key.code == sf::Keyboard::R)&&!r)
                    {
                        r = 1;
                        i = 0;
                    }
                }
            }

            shape.setFillColor( sf::Color( red, green, blue, alpha ));

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                red+=1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                red-=1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                green+=1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
                green-=1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                blue+=1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                blue-=1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
                alpha+=1;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
                alpha-=1;


            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                linia.clear();
                nagranie.clear();
            }

            mouse = sf::Mouse::getPosition(window);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                //window.draw(shape);
                linia.push_back( mouse );

                if(mouse!=offset)
                {
                    Ramie.x+=(double(mouse.x)-double(offset.x))/50;
                    Ramie.y-=(double(mouse.y)-double(offset.y))/50;
                    Ramie.ruchxy();
                    nagranie.push_back( sf::Vector2f( Ramie.x, Ramie.y ) );
                    offset = mouse;
                }
            }
            else
                offset = mouse;

            window.clear();

            for( unsigned int j = 0; j < linia.size(); j++ )
            {
                window.draw(shape);
                if(r) shape.setFillColor( sf::Color( 255, 0, 0, 255 ));
                shape.setPosition( sf::Vector2f( linia[j].x, linia[j].y ) );
            }

            if(r)
            {
                Ramie.x=nagranie[i].x;
                Ramie.y=nagranie[i].y;
                Ramie.ruchxy();

                shape.setFillColor( sf::Color( 0, 255, 0, 255 ));
                shape.setPosition( sf::Vector2f( linia[i].x, linia[i].y ) );
                window.draw(shape);
                if( i >= nagranie.size()-1 ) r=0;
                else i++;
            }

            window.display();
        }
    }
    if ( kontrol==4 )
    {
        Ramie.x=5;
        Ramie.y=5;
        twitch( "lokiete", "#lokiete" );
    }

    return 0;

}
