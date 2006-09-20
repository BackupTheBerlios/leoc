/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#ifndef __SYMBOL_H_
#define __SYMBOL_H_

/* $Id: symbol.h,v 1.1 2006/09/20 09:10:24 tikkeri Exp $ */

#include "xalloc.h"


typedef struct _symbol
{
    size_t size;
    char name[28];   
} symbol;


typedef struct _symbols
{
    size_t max;
    size_t n;
    struct _symbol** table;
    struct _symbols* up;
} symbols;

symbols* new_symbols( void );
symbol* new_symbol( char* name, size_t size );
void add_name( symbols* s, char* name, size_t size );
symbols* child_stable( symbols* parent );

#endif /* not __SYMBOL_H_ */

