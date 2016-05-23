#include <iostream>
#include <cstdlib>
#include <conio.h>
#include "RS232/RS232.h"
#include "libircclient/libircclient.h"
#include "ramie.h"

using namespace std;

ramie Ramie;

typedef struct _irc_ctx_t
{
	const char 	* channel;
	const char 	* nick;

} irc_ctx_t;

void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char buf[512];
	int cnt;

	buf[0] = '\0';

	for ( cnt = 0; cnt < count; cnt++ )
	{
		if ( cnt )
			strcat (buf, "|");

		strcat (buf, params[cnt]);
	}
}

void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);
	irc_cmd_user_mode (session, "+i");
	irc_cmd_msg (session, params[0], "No siema");
}


void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
	dump_event (session, event, origin, params, count);

	irc_cmd_join (session, ctx->channel, 0);
}

void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    char nickbuf[128];

	if ( count != 2 )
		return;

	if ( !origin )
		return;

	irc_target_get_nick (origin, nickbuf, sizeof(nickbuf));
    if ( !strncmp(params[1], "ster", 4 ) )
	{

		char str[10];

		int kat1;
		int kat2;

		sscanf( params[1], "%s %d %d", str, &kat1, &kat2 );

        //tu
        Ramie.obrot(kat1);
        Ramie.obrot(kat2);

	}

}

void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
	char buf[24];
	sprintf (buf, "%d", event);

	dump_event (session, buf, origin, params, count);
}


int main()
{
    unsigned short port;
    unsigned char znak;
    int w;
    int kat1=90,kat2=90;
    float rozd=1;

    cout << "Podaj port: ";
    cin >> port;
    Ramie.openPort( port );

    cout << "Metoda kontroli(cout, getcho, getchxy, twitch): ";
    int kontrol;
    cin >> kontrol;
    if ( kontrol==0 )
        while( 1 )
        {
            cin >> kat1;
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
            cout << kat1 << " " << kat2 << " " << rozd;
        }
    if ( kontrol==2 )
    {
        Ramie._x=1;
        Ramie._y=1;
        while( 1 )
        {
            znak = getch();
            if(znak=='a')      Ramie._x-=rozd;
            else if(znak=='d') Ramie._x+=rozd;
            else if(znak=='s') Ramie._y-=rozd;
            else if(znak=='w') Ramie._y+=rozd;
            else if(znak=='q') rozd-=0.1;
            else if(znak=='e') rozd+=0.1;
            system("cls");
            w=Ramie.ruchxy(Ramie._x, Ramie._y);
            if ( w == 1) cout << "Nic1" << endl;
            if ( w == 2) cout << "Nic2" << endl;
            cout << Ramie._x << " " << Ramie._y << " " << rozd;
        }
    }
    if ( kontrol==3 )
    {
        irc_callbacks_t	callbacks;
        irc_ctx_t ctx;
        irc_session_t * s;
        memset (&callbacks, 0, sizeof(callbacks));

        callbacks.event_connect = event_connect;
        callbacks.event_join = event_join;
        callbacks.event_nick = dump_event;
        callbacks.event_channel = event_channel;
        callbacks.event_numeric = event_numeric;

        s = irc_create_session (&callbacks);

        if ( !s )
        {
            printf ("Could not create session\n");
            return 1;
        }

        ctx.nick = "lokiete";
        ctx.channel = "#lokiete";
        irc_set_ctx (s, &ctx);

        if ( irc_connect (s, "irc.chat.twitch.tv", 6667, "oauth:7xlpn8in3tw74ej8ha2wiorzw38rq5", "lokiete", 0, 0) )
        {
            cout << "Could not connect: %s\n" << irc_strerror (irc_errno(s));
            return 1;
        }

        if ( irc_run (s) )
        {
            cout << "Could not connect or I/O error: %s\n" << irc_strerror (irc_errno(s));
            return 1;
        }
    }

    return 0;

}
