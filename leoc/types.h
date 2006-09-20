/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#ifndef __TYPES_H_
#define __TYPES_H_

/* $Id: types.h,v 1.1 2006/09/20 09:10:24 tikkeri Exp $ */

#define FUNC_CCALL      0x10000000l
#define FUNC_STDCALL    0x20000000l

#define TYPE_SIGNED     0x00000001l
#define TYPE_UNSIGNED   0x00000002l

#define SIZE_1          0x00000004l
#define SIZE_2          0x00000008l
#define SIZE_4          0x00000010l
#define SIZE_8          0x00000020l
#define SIZE_10         0x00000040l
#define SIZE_16         0x00000080l

#define SIZE_OTHER      0x00000100l

#define TYPE_EXTERN     0x00000200l
#define TYPE_STATIC     0x00000400l
#define TYPE_EXPORT     0x00000800l 

#define TYPE_BUILTIN    0x00001000l
#define TYPE_POINTER    0x00002000l
#define TYPE_STRUCT     0x00004000l
#define TYPE_UNION      0x00008000l
#define TYPE_ARRAY      0x00010000l

typedef struct _data_type
{
    unsigned long attr;
    size_t size;
    size_t dim;

    struct _data_type* child;
    char id[48];
    
} data_type;


data_type* new_data_type( void );


#endif /* not __TYPES_H_ */

