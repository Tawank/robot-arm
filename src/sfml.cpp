#include "sfml.h"

template < class T >
std::wstring to_string( T t )
{
    std::wstringstream ss;
    ss << t;
    return ss.str();
}

template < class T >
std::wstring to_string_fixed( T t, const int decDigits ) {
    std::wstringstream ss;
    ss << std::fixed;
    ss.precision( decDigits );
    ss << t;
    return ss.str();
}

template <typename T>
std::vector<T>& operator+=(std::vector<T>& a, const std::vector<T>& b)
{
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

void Trace::clear()
{
    window.clear();
    arm.clear();
}

bool Trace::empty()
{
    return window.empty()&&arm.empty();
}

Sfml::Sfml()
{

    window.create( sf::VideoMode(1440, 900), "RAMIE", sf::Style::Default );
    window.setFramerateLimit     (   60  );
    window.setVerticalSyncEnabled( false );
    window.setKeyRepeatEnabled   ( true  );
    window.resetGLStates();

}

Sfml::run()
{
    Trace trace;

    Keyboard keyboard;

    SetSfGui();

    sf::CircleShape line(4.f);
    line.setFillColor( sf::Color( colour.red->GetValue(), colour.green->GetValue(), colour.blue->GetValue(), colour.alpha->GetValue() ));

    sf::Vector2i offset;
    sf::Vector2i mouse;

    while (window.isOpen())
    {
        auto delta = clock.restart().asSeconds();

        while ( window.pollEvent(event) )
        {
            if (event.type == sf::Event::Closed)
                window.close();

            desktop.HandleEvent( event );

            if (event.type == sf::Event::Resized)
            {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                settingsWindow->SetPosition(sf::Vector2f(event.size.width-150,0));
                colourWindow->SetPosition(sf::Vector2f(0,event.size.height-120));
                coordWindow->SetPosition(sf::Vector2f(event.size.width-120,event.size.height-60));
            }

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }

                if ((event.key.code == sf::Keyboard::Space)&&(iterator==-1))
                {
                    iterator = 0;
                }

                if (event.key.code >= sf::Keyboard::Key(0) && event.key.code < sf::Keyboard::Key(26))
                {
                    if(keyboard.Key[event.key.code].empty())
                    {
                        keyboard.Key[event.key.code].arm = trace.arm;
                        keyboard.Key[event.key.code].window = trace.window;
                    }
                    else
                    {
                        std::vector < sf::Vector2f > tmpA(keyboard.Key[event.key.code].arm);
                        sf::Vector2f gammaA = trace.arm.back() - keyboard.Key[event.key.code].arm.front();
                        for( auto &el: tmpA)
                            el += gammaA;
                        trace.arm += tmpA;

                        std::vector < sf::Vector2i > tmpW = keyboard.Key[event.key.code].window;
                        sf::Vector2i gammaW = trace.window.back() - keyboard.Key[event.key.code].window.front();
                        for( auto &el: tmpW)
                            el += gammaW;
                        trace.window += tmpW;
                    }
                }
            }
        }

        line.setFillColor( sf::Color( colour.red->GetValue(), colour.green->GetValue(), colour.blue->GetValue(), colour.alpha->GetValue() ));

        if ( sf::Mouse::isButtonPressed(sf::Mouse::Right) )
        {
            trace.clear();
        }

        mouse = sf::Mouse::getPosition(window);

        if (( sf::Mouse::isButtonPressed(sf::Mouse::Left)) &&
              (!colourWindow->GetAllocation().contains( sf::Vector2f( mouse.x, mouse.y ))&&
               !coordWindow->GetAllocation().contains( sf::Vector2f( mouse.x, mouse.y ))&&
               !settingsWindow->GetAllocation().contains( sf::Vector2f( mouse.x, mouse.y ))))
        {

            if(mouse!=offset)
            {
                coordLabel->SetText( L"x: "+to_string_fixed(arm.x,2)+L" y: "+to_string_fixed(arm.y,2));
                arm.x+=(static_cast<long double>(mouse.x)-static_cast<long double>(offset.x))/((mouseSpeedScale->GetValue()-100)*-1);
                arm.y-=(static_cast<long double>(mouse.y)-static_cast<long double>(offset.y))/((mouseSpeedScale->GetValue()-100)*-1);
                arm.move();
                trace.arm.push_back( sf::Vector2f( arm.x, arm.y ) );
                trace.window.push_back( mouse );
                offset = mouse;
            }

        }
        else
            offset = mouse;

        window.clear(sf::Color::Black);

        for( const auto &el: trace.window )
        {
            line.setPosition( sf::Vector2f( el.x, el.y ) );
            window.draw(line);
        }

        if(iterator>=0)
        {
            if(trace.empty())
            {
                iterator=0;
                continue;
            }

            arm.x=trace.arm[iterator].x;
            arm.y=trace.arm[iterator].y;
            arm.move();

            line.setFillColor( sf::Color( 0, 255, 0, 255 ));
            line.setPosition( sf::Vector2f( trace.window[iterator].x, trace.window[iterator].y ) );
            window.draw( line );
            if( iterator >= static_cast<int>(trace.arm.size()-1) ) iterator=-1;
            else ++iterator;
        }

        desktop.Update( delta );

        sfgui.Display( window );

        window.display();
    }

    return 0;

}

void Sfml::portChange()
{
    if(arm.openPort(portComboBox->GetSelectedItem()+1))
        coordLabel->SetText( L"COM"+to_string(portComboBox->GetSelectedItem()+1)+L" Blad");
    else
        coordLabel->SetText( L"COM"+to_string(portComboBox->GetSelectedItem()+1)+L" OK");
}

void Sfml::SetSfGui()
{
    sfgui.GetRenderer().AddCharacterSet( 32, 300 );

    // settings window
    mouseSpeedScale = sfg::Scale::Create ( 0.f, 99.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );

    mouseSpeedScale->SetValue  ( 50.f );

    portComboBox = sfg::ComboBox::Create();
    for(int i=1;i<10;++i)
        portComboBox->AppendItem(L"COM"+to_string(i));

    //portComboBox->OnSelect.Connect(std::bind( &Sfml::portChange, this ));
    portComboBox->GetSignal(sfg::ComboBox::OnSelect).Connect(std::bind( &Sfml::portChange, this ));


    settingsWindow = sfg::Window::Create();
    settingsWindow->SetTitle( "Ustawienia" );
    auto settingsTable = sfg::Table::Create();
    settingsTable->SetRowSpacings( 2.f );
    settingsTable->SetColumnSpacings( 1.f );

    settingsTable->Attach( sfg::Label::Create( "Speed:"   ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    settingsTable->Attach( mouseSpeedScale, sf::Rect<sf::Uint32>( 1, 1, 30, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
    settingsTable->Attach( sfg::Label::Create( "Port:"   ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    settingsTable->Attach( portComboBox, sf::Rect<sf::Uint32>( 1, 2, 30, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );

    settingsWindow->Add( settingsTable );

    desktop.Add( settingsWindow );
    settingsWindow->SetPosition( sf::Vector2f (1440-150,0) );
    // settings window

    // colour window
    colour.red   = sfg::Scale::Create( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );
    colour.green = sfg::Scale::Create( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );
    colour.blue  = sfg::Scale::Create( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );
    colour.alpha = sfg::Scale::Create( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );

    colour.red ->SetValue ( 255.f );
    colour.green->SetValue(   0.f );
    colour.blue->SetValue (   0.f );
    colour.alpha->SetValue( 255.f );

    colourWindow = sfg::Window::Create();
    desktop.Add( colourWindow );
    auto colourTable = sfg::Table::Create();
    colourTable->SetRowSpacings( 5.f );
    colourTable->SetColumnSpacings( 5.f );

    colourTable->Attach( sfg::Label::Create( "Red: "    ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    colourTable->Attach( colour.red,   sf::Rect<sf::Uint32>( 1, 1, 10, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
    colourTable->Attach( sfg::Label::Create( "Green: "  ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    colourTable->Attach( colour.green, sf::Rect<sf::Uint32>( 1, 2, 10, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
    colourTable->Attach( sfg::Label::Create( "Blue: "   ), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    colourTable->Attach( colour.blue,  sf::Rect<sf::Uint32>( 1, 3, 10, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
    colourTable->Attach( sfg::Label::Create( "Alpha: "  ), sf::Rect<sf::Uint32>( 0, 4, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    colourTable->Attach( colour.alpha, sf::Rect<sf::Uint32>( 1, 4, 10, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );

    colourWindow->SetTitle( "Kolor" );
    colourWindow->Add( colourTable );
    colourWindow->SetPosition( sf::Vector2f (0,900-120) );

    // colour window

    // x i y window
    coordLabel = sfg::Label::Create( "x: 00.00 y: 00.00" );
	auto coordBox = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	coordBox->Pack( coordLabel );

	coordWindow = sfg::Window::Create();
	coordWindow->SetTitle( "Koordynaty" );
	coordWindow->Add( coordBox );

	desktop.Add( coordWindow );

	coordWindow->SetPosition(sf::Vector2f(1440-120,900-60));

    // x i y window

}
