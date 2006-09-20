/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#ifndef __CODEGEN2_H_
#define __CODEGEN2_H_

/* $Id: codegen2.h,v 1.1 2006/09/20 09:10:24 tikkeri Exp $ */


#include "config.h"
#include "tree.h"


extern FILE* codeout2;

int gen2( tree* t );

#endif /* not __CODEGEN2_H_ */

