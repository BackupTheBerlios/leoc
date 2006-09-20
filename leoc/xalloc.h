/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#ifndef __XALLOC_H_
#define __XALLOC_H_

#include "config.h"

/* $Id: xalloc.h,v 1.1 2006/09/20 09:10:24 tikkeri Exp $ */

char* xmalloc( size_t size );
char* xrealloc( void* base, size_t new_size );

#endif /* not __XALLOC_H */


