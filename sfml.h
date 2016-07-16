#ifndef SFML_H
#define SFML_H

#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "arm/ramie.h"

//template < class T >
//string do_string( T )

class Sfml
{
    private:
        sf::RenderWindow render_window;
        sfg::SFGUI sfgui;
        sfg::Desktop desktop;

        sf::Clock clock;
        sf::Event event;

        std::vector < sf::Vector2i > win;
        std::vector < sf::Vector2f > arm;


        int iterator;

    public:
        Sfml();
        run();
};

#endif // SFML_H
