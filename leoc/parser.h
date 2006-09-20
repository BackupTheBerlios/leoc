/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
typedef union {
    tree* a_tree;
    char string[32];
} YYSTYPE;
#define	NUM	257
#define	IDENT	258
#define	HIGHOPERATOR	259
#define	LOWOPERATOR	260
#define	CONDOPERATOR	261
#define	IF	262
#define	ELSE	263
#define	DO	264
#define	WHILE	265
#define	BREAK	266
#define	CONTINUE	267
#define	SWITCH	268
#define	CASE	269
#define	INT	270
#define	LONG	271
#define	DOUBLE	272
#define	CHAR	273
#define	VOID	274
#define	GOTO	275
#define	FLOAT	276
#define	ASSIGN	277
#define	SIZE	278


extern YYSTYPE yylval;
