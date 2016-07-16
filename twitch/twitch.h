#ifndef TWITCH_H
#define TWITCH_H
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "libircclient/libircclient.h"
#include "../arm/ramie.h"

//void dump_event(irc_session_t *, const char *, const char *, const char **, unsigned int);
//void event_join (irc_session_t *, const char *, const char *, const char **, unsigned int);
//void event_connect (irc_session_t *, const char *, const char *, const char **, unsigned int);
//void event_channel (irc_session_t *, const char *, const char *, const char **, unsigned int);
//void event_numeric (irc_session_t *, unsigned int, const char *, const char **, unsigned int);
int twitch(const char 	*, const char 	*);

#endif // TWITCH_H
