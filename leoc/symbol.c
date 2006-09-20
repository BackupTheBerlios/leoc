/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include "utils.h"
#include "symbol.h"

RCSID( "$Id: symbol.c,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );


symbols* new_symbols( void )
{
    symbols* res = (symbols*) xmalloc( sizeof( symbols ) );
    
    memset( (void*) res, 0, sizeof( symbols ) );

    return res;
}


symbol* new_symbol( char* name, size_t size )
{
    symbol* res = (symbol*)xmalloc( sizeof( symbol ) );
    
    memset( (void*) res, 0, sizeof( symbol ) );

    res->size = size;
    strcpy( res->name, name );
    
    return res;
}


void add_name( symbols* s, char* name, size_t size )
{
    register int i;
    
    if( s->max == 0 )
    {
        s->table = (symbol**)xmalloc( 16 * sizeof( symbol ) );
        s->n = 16;
    }
    
    if ( ( s->n + 1 ) == s->max )
    {
        s->table = (symbol**)xrealloc( s->table, s->max + 32 );
        s->n += 32;
    }

    for( i = 0; i < s->n; i++ )
    {
        if( ! strcmp( name, s->table[i]->name ) )
        {
                fprintf( stderr, "Symbol %s already defined!", name );
                return;
        }
    }

    s->table[ s->n++ ] = new_symbol( name, size );
}


symbols* child_stable( symbols* parent )
{
    symbols* res = new_symbols();

    res->up = parent;

    return res;
}

