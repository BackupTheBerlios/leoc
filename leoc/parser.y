// vim:ts=4
%{
/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "tree.h"
#include "codegen2.h"
#include "utils.h"

unsigned yylex( void );
int yyerror( const char* );

RCSID( "$Id: parser.y,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );

extern tree* st;

%}

%union {
    tree* a_tree;
    char string[32];
}

%start op_list
%type <a_tree> stat op_list exp op assign arglist fcall statlist
%token <string> NUM IDENT HIGHOPERATOR LOWOPERATOR CONDOPERATOR
%token IF ELSE DO WHILE BREAK CONTINUE SWITCH CASE INT LONG DOUBLE CHAR VOID  GOTO FLOAT
%token ASSIGN SIZE
%left CONDOPERATOR	
%left LOWOPERATOR	
%left HIGHOPERATOR	

%%





op_list:    op_list op                          { st = make_list( st, $2 );                 }    
            | op                                { st = make_list( st, $1 );                 }
            ;

op:         IDENT ';'                           { $$ = make_nop();                          }
            |statlist ';'                       { $$ = $1;                                  }
            | IF '(' stat ')' op                { $$ = make_if( $3, $5 , "NULL" );          }
            | IF '(' stat ')' op ELSE op        { $$ = make_ifelse( $3, $5, $7, "NULL" );   }
            | WHILE '(' stat ')' op             { $$ = make_while( $3, $5, "NULL" );        }
            | DO op WHILE '(' stat ')'          { $$ = make_dowhile( $5, $2, "NULL" );      }
            | GOTO IDENT ';'                    { $$ = make_operator (NULL, "GOTO",  make_operand( $2, 'i' ) ); }
            | IDENT ':' op                      { $$ = make_label( $1, $3 );                }
            | '{' op '}'                        { $$ = $2;                                  }
            | '{' '}'                           { $$ = make_nop();                          }
            | ';'                               { $$ = make_nop();                          }
            ;

arglist:    arglist ',' exp                     { $$ = make_list( $$, $3 );                 }
            |    exp                            { $$ = $1;                                  }
            ;

fcall:      IDENT '(' arglist ')'               { $$ = make_fcall( $1, $3, FUNC_CCALL );    }
            | IDENT '(' ')'                     { $$ = make_fcall( $1, NULL, FUNC_CCALL );  }


stat:       assign 
            | exp 
	        ;

statlist:   statlist ';' stat                   { $$ = make_list( $$, $3 );                 }
            | stat                              { $$ = $1;                                  }
//            | '{' '}'                           { $$ = make_nop();                          }
//            | ';'                               { $$ = make_nop();                          }
            ;

assign:     IDENT ASSIGN exp                    { $$ = make_assign( $1, $3 );               } 
            | IDENT ASSIGN assign               { $$ = make_assign( $1, $3 );               }

exp:        exp CONDOPERATOR exp		        { $$ = make_operator( $1, $2, $3);          }
            | exp HIGHOPERATOR exp		        { $$ = make_operator( $1, $2, $3);          }
	        | exp LOWOPERATOR exp		        { $$ = make_operator( $1, $2, $3);          }
	        | NUM			                    { $$ = make_operand( $1, 'c');              }
            | IDENT       		                { $$ = make_operand( $1, 'i');              }
            | fcall                             { $$ = $1;                                  }
	        | '(' exp ')'			            { $$ = $2;                                  }
            ;


%%

int yyerror( const char *s )
{
    fprintf( stderr, "%s\n", s);

    return 0;
}


