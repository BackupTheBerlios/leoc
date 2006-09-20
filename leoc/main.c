/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "config.h"
#include "lexer.h"
#include "tree.h"
#include "parser.h"
#include "utils.h"
#include "codegen2.h"
#include "symbol.h"



int yyparse( void );
tree* st;
int verboselev;

RCSID( "$Id: main.c,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );


int main( int argc, char** argv )
{
    if ( argc == 2  )
    {
        if( ! strcmp( argv[1], "-h" ) || ! strcmp( argv[1], "--help" )  )
        {
            printf( "Usage: %s [ filename ]\n"
                    "Reading stdin if no filename given.\n\n", argv[0] );

            return 0;
        }
    }

    if( argc > 1 )
    {
        yyin= fopen( argv[1], "rt" );
        
        if ( yyin == NULL )
            die( "in file", 1 );
    }

    codeout2= stdout;

    if( argc > 2 )
    {
        codeout2 = fopen( argv[2], "wb" );
        
        if( ! codeout2 )
            die( "out file", 2 );

    }

    if( yyparse() == 0 )
    {
        printtree( st, 1 );
        codeout2 = fopen( "out.s", "w" );   gen2( st );        fclose(codeout2);
        codeout2 = stdout;                  gen2( st );
    }

    return 0;
}

