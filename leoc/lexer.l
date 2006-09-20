%{
/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include "config.h"
#include "tree.h"
#include "parser.h"
#include "utils.h"

void yyerror( const char* );

RCSID( "$Id: lexer.l,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );

#define UPDATETOKENPOS() {}


%}

%%

"if"        {   UPDATETOKENPOS();   return IF;          }
"else"      {   UPDATETOKENPOS();   return ELSE;        }
"do"        {   UPDATETOKENPOS();   return DO;          }
"while"     {   UPDATETOKENPOS();   return WHILE;       }
"break"     {   UPDATETOKENPOS();   return BREAK;       }
"continue"  {   UPDATETOKENPOS();   return CONTINUE;    }
"switch"    {   UPDATETOKENPOS();   return SWITCH;      }
"case"      {   UPDATETOKENPOS();   return CASE;        }
"int"       {   UPDATETOKENPOS();   return INT;         }
"long"      {   UPDATETOKENPOS();   return LONG;        }
"float"     {   UPDATETOKENPOS();   return FLOAT;       }
"double"    {   UPDATETOKENPOS();   return DOUBLE;      }
"goto"      {   UPDATETOKENPOS();   return GOTO;        }
"char"      {   UPDATETOKENPOS();   return CHAR;        }
"void"      {   UPDATETOKENPOS();   return VOID;        }
"size"      {   UPDATETOKENPOS();   return SIZE;        }



[0-9]+		{   UPDATETOKENPOS();   strcpy (yylval.string, yytext); return NUM;}
[_A-Za-z][_A-Za-z0-9]*	{   UPDATETOKENPOS();   strcpy (yylval.string, yytext); return IDENT;}
[ \t\n]		{	UPDATETOKENPOS();                                                           }
","         {   UPDATETOKENPOS();   return ',';                                             }
">"			{   UPDATETOKENPOS();   strcpy( yylval.string, ">");    return CONDOPERATOR;    }
"<"			{   UPDATETOKENPOS();   strcpy( yylval.string, "<");    return CONDOPERATOR;    }
">="		{   UPDATETOKENPOS();   strcpy( yylval.string, ">=");   return CONDOPERATOR;    }
"!="		{   UPDATETOKENPOS();   strcpy( yylval.string, "!=");   return CONDOPERATOR;    }
"<="		{   UPDATETOKENPOS();   strcpy( yylval.string, "<=");   return CONDOPERATOR;    }
"=="		{   UPDATETOKENPOS();   strcpy( yylval.string, "==");   return CONDOPERATOR;    }
"-"			{   UPDATETOKENPOS();   strcpy( yylval.string, "sub");  return LOWOPERATOR;     }
"+"			{   UPDATETOKENPOS();   strcpy( yylval.string, "add");  return LOWOPERATOR;     }
"/"			{   UPDATETOKENPOS();   strcpy( yylval.string, "div");  return HIGHOPERATOR;    }
"="			{   UPDATETOKENPOS();   return ASSIGN;                                          }
"*"			{   UPDATETOKENPOS();   strcpy( yylval.string, "mul"); return HIGHOPERATOR;     }
[();:{}]	{   UPDATETOKENPOS();   return yytext[0];                                       }
.			{   UPDATETOKENPOS();   ECHO; yyerror ("unexpected character");                 }

%%

int yywrap( void )
{
    return 1;
}

