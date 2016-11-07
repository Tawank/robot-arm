#ifndef SFML_H
#define SFML_H

#include <string>
#include <sstream>

#include <SFML/Graphics.hpp>
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>

#include "arm/arm.h"
#include "keyboard.h"

class Sfml
{
public:
    Sfml();
    run();
    void SetSfGui();
private:
    Arm arm;

    sf::RenderWindow window;

    sf::Clock clock;
    sf::Event event;


    sfg::Window::Ptr settingsWindow;
    sfg::Scale::Ptr mouseSpeedScale;
    sfg::ComboBox ::Ptr portComboBox;
    void portChange();

    sfg::Window::Ptr colourWindow;
    struct Colour
    {
        sfg::Scale::Ptr red;
        sfg::Scale::Ptr green;
        sfg::Scale::Ptr blue;
        sfg::Scale::Ptr alpha;
    };
    Colour colour;

    sfg::Window::Ptr coordWindow;
    sfg::Label::Ptr coordLabel;

    sfg::Desktop desktop;
    sfg::SFGUI sfgui;

    int iterator;
};


#endif // SFML_H
