#include "sfml.h"

extern Arm Ramie;

using namespace std;

template < class T >
string do_string( T t )
{
    stringstream ss;
    ss << t;
    return ss.str();
}

Sfml::Sfml()
{

    render_window.create( sf::VideoMode(800, 600), "RAMIE", sf::Style::Default );
    render_window.setFramerateLimit     (   0   );
    render_window.setVerticalSyncEnabled( false );
    render_window.setKeyRepeatEnabled   ( true  );

    iterator = 0;

    arm.clear();
    win.clear();

}

Sfml::run()
{

    // okno dla ustawien
    auto mouse_speed_scale = sfg::Scale::Create  ( 1.f, 500.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );

    mouse_speed_scale->SetValue  ( 50.f );

    auto ustaw_okno = sfg::Window::Create();
    ustaw_okno->SetTitle( "Ustawienia" );

    auto ustaw_table = sfg::Table::Create();
    ustaw_table->SetRowSpacings( 5.f );
    ustaw_table->SetColumnSpacings( 5.f );

    ustaw_table->Attach( sfg::Label::Create( "Speed:"   ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    ustaw_table->Attach( mouse_speed_scale,   sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );

    ustaw_okno->Add( ustaw_table );

    desktop.Add( ustaw_okno );
    // okno dla dla ustawien

    // okno dla wyboru koloru
    auto red_scale   = sfg::Scale::Create  ( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );
    auto green_scale = sfg::Scale::Create( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );
    auto blue_scale  = sfg::Scale::Create ( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );
    auto alpha_scale = sfg::Scale::Create( 0.f, 255.f, 1.f, sfg::Scale::Orientation::HORIZONTAL );

    red_scale->SetValue  ( 255.f );
    green_scale->SetValue(   0.f );
    blue_scale->SetValue (   0.f );
    alpha_scale->SetValue( 255.f );

    auto kolor_table = sfg::Table::Create();
    kolor_table->SetRowSpacings( 5.f );
    kolor_table->SetColumnSpacings( 5.f );

    kolor_table->Attach( sfg::Label::Create( "Red:"   ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    kolor_table->Attach( red_scale,   sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
    kolor_table->Attach( sfg::Label::Create( "Green:" ), sf::Rect<sf::Uint32>( 0, 2, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    kolor_table->Attach( green_scale, sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
    kolor_table->Attach( sfg::Label::Create( "Blue:"  ), sf::Rect<sf::Uint32>( 0, 3, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    kolor_table->Attach( blue_scale,  sf::Rect<sf::Uint32>( 1, 3, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );
    kolor_table->Attach( sfg::Label::Create( "Alpha:" ), sf::Rect<sf::Uint32>( 0, 4, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
    kolor_table->Attach( alpha_scale, sf::Rect<sf::Uint32>( 1, 4, 1, 1 ), sfg::Table::FILL | sfg::Table::EXPAND, sfg::Table::FILL | sfg::Table::EXPAND );

    auto kolor_okno = sfg::Window::Create();
    kolor_okno->SetTitle( "Kolor" );
    kolor_okno->Add( kolor_table );

    desktop.Add( kolor_okno );
    // okno dla wyboru koloru


    // okno dla x i y
    auto m_label = sfg::Label::Create( "x: "+do_string(Ramie.x)+" y: "+do_string(Ramie.y) );
	auto box = sfg::Box::Create( sfg::Box::Orientation::VERTICAL, 5.0f );
	box->Pack( m_label );

	auto koord_okno = sfg::Window::Create();
	koord_okno->SetTitle( "Koordynaty" );
	koord_okno->Add( box );

	desktop.Add( koord_okno );
    // okno dla x i y

    auto okno_vis = true;


    sf::CircleShape shape(4.f);
    shape.setFillColor( sf::Color( red_scale->GetValue(), green_scale->GetValue(), blue_scale->GetValue(), alpha_scale->GetValue() ));

    sf::Vector2i offset;
    sf::Vector2i mouse;

    arm.push_back( sf::Vector2f( Ramie.x, Ramie.y ) );
    win.push_back( mouse );

    while (render_window.isOpen())
    {
        auto delta = clock.restart().asSeconds();

        while ( render_window.pollEvent(event) )
        {
            if (event.type == sf::Event::Closed)
                render_window.close();
            else
                desktop.HandleEvent( event );

            if (event.type == sf::Event::Resized)
                render_window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    render_window.close();
                }

                if ((event.key.code == sf::Keyboard::R)&&(iterator==-1))
                {
                    iterator = 0;
                }

                if ((event.key.code == sf::Keyboard::C))
                {
                    okno_vis = !okno_vis;
                    kolor_okno->Show( okno_vis );
                    koord_okno->Show( okno_vis );
                }
            }
        }

        shape.setFillColor( sf::Color( red_scale->GetValue(), green_scale->GetValue(), blue_scale->GetValue(), alpha_scale->GetValue() ));

        if ( sf::Mouse::isButtonPressed(sf::Mouse::Right) )
        {
            arm.clear();
            win.clear();
        }

        mouse = sf::Mouse::getPosition(render_window);

        if ( sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
             ((!kolor_okno->GetAllocation().contains( sf::Vector2f( mouse.x, mouse.y ))&&
               !koord_okno->GetAllocation().contains( sf::Vector2f( mouse.x, mouse.y ))&&
               !ustaw_okno->GetAllocation().contains( sf::Vector2f( mouse.x, mouse.y ))  ) || (!okno_vis) ) )
        {

            if(mouse!=offset)
            {
                Ramie.x+=(static_cast<long double>(mouse.x)-static_cast<long double>(offset.x))/mouse_speed_scale->GetValue();
                Ramie.y-=(static_cast<long double>(mouse.y)-static_cast<long double>(offset.y))/mouse_speed_scale->GetValue();
                Ramie.movexy();
                arm.push_back( sf::Vector2f( Ramie.x, Ramie.y ) );
                win.push_back( mouse );
                offset = mouse;
            }

        }
        else
            offset = mouse;

        render_window.clear(sf::Color::Black);

        for( const auto &el: win )
        {
            shape.setPosition( sf::Vector2f( el.x, el.y ) );
            render_window.draw(shape);
        }

        if(iterator>=0)
        {
            if(win.empty()||arm.empty())
            {
                iterator=0;
                continue;
            }

            Ramie.x=arm[iterator].x;
            Ramie.y=arm[iterator].y;
            Ramie.movexy();

            shape.setFillColor( sf::Color( 0, 255, 0, 255 ));
            shape.setPosition( sf::Vector2f( win[iterator].x, win[iterator].y ) );
            render_window.draw( shape );
            if( iterator >= static_cast<int>(arm.size()-1) ) iterator=-1;
            else ++iterator;
        }

        m_label->SetText( "x: "+do_string(Ramie.x)+" y: "+do_string(Ramie.y) );

        desktop.Update( delta );

        sfgui.Display( render_window );

        render_window.display();
    }

    return 0;

}
