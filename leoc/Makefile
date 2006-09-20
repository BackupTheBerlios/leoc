# $Id: Makefile,v 1.1 2006/09/20 09:10:24 tikkeri Exp $

include Makefile.inc

CC=gcc
LEX=flex
FLEXFLAGS=
LIBS=-lgetopt
CFLAGS=-g3 -O2 -DLINT -pedantic -Wall
#CFLAGS=-O3 -DNLINT -DNDEBUG
e=.exe
o=.o

ifndef BYACC
 YACC=bison --yacc
 YFLAGS=--verbose --defines #--no-lines
else
 YACC=byacc
 YFLAGS=-v -d -t
endif


OBJS=parser$o lexer$o tree$o main$o codegen2$o types$o xalloc$o symbol$o utils$o

all: law$e
parser.c: parser.y
lexer.c: lexer.l


law$e: $(OBJS)
	gcc $(LDFLAGS) $(OBJS)  $(LIBS) -o $@


clean:
	-rm -vf $(OBJS) *.output parser.[hc] *.tab.[hc] *~ lexer.c


klean: clean
	-rm law$e

Makedep: 
	gcc -MM *.c > Makedep

rebuild: clean all

include Makedep



