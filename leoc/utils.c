/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include "utils.h"


RCSID( "$Id: utils.c,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );



void die( const char* msg, int exitcode )
{
    perror( msg );
    exit( exitcode );
}

