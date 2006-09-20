/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include "xalloc.h"
#include "utils.h"
#include "config.h"

RCSID( "$Id: xalloc.c,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );

char* xmalloc( size_t size )
{
    char* res = malloc( size );
    
    if( ! res )
        die( "Memory allocation error.", 6 );

    return res;
}


char* xrealloc( void* base, size_t new_size )
{
    void* res = realloc( base, new_size );
    
    if( ! res )
        die( "Realloc error", 7 );

    return res;
}

