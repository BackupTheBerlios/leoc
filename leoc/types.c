/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include "config.h"
#include "utils.h"
#include "types.h"

RCSID( "$Id: types.c,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );







data_type* new_data_type( void )
{
    data_type* res= (data_type*) malloc( sizeof( data_type ) );

    assert( res );

    memset( (void*) res, 0, sizeof( res ) );

    return res;
}




