/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#ifndef __UTILS_H
#define __UTILS_H

/* $Id: utils.h,v 1.1 2006/09/20 09:10:24 tikkeri Exp $ */

#ifdef LINT
# ifdef __RCSID
#  define RCSID(id)	__RCSID(id)
# elif defined(__GNUC__) && defined(__ELF__)
#  define RCSID(id)	__asm__(".ident\t\"" id "\"")
# else
#  define RCSID(id)	static const char cvsid[] = id
# endif
#else
# define RCSID(id)
#endif /* LINT */


#include "config.h"

#include <assert.h>

#ifdef __GNUC__
void __attribute__((noreturn)) die( const char* msg, int exit_code );
#else /* not __GNUC__ */
void __attribute__((noreturn)) die( const char* msg, int exit_code );
#endif /* __GNUC__ */


#endif /* not __UTILS_H */

