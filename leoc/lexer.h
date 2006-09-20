/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#ifndef __LEXER_H_
#define __LEXER_H_

/* $Id: lexer.h,v 1.1 2006/09/20 09:10:24 tikkeri Exp $ */

#include <stdio.h>


int yylex( void );

extern FILE* yyin;

#endif /* not  __LEXER_H_ */

