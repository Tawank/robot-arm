#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include <string>
#include <sstream>
#include <vector>

class Trace
{
public:
    //Trace();
    void clear();
    bool empty();
    std::vector < sf::Vector2f > arm;
    std::vector < sf::Vector2i > window;
};

class Keyboard
{
    public:
        Keyboard();
        ~Keyboard();
        Trace Key[26];
};

#endif // KEYBOARD_H
