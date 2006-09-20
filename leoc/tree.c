/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "utils.h"

RCSID( "$Id: tree.c,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );




tree* new_tree_node( void )
{
    tree* t= (tree*) malloc( sizeof(tree) );
    
    if( ! t )
    {
        perror( "tree malloc" );
        exit(1);
    }
    
    memset( (void*)t, 0, sizeof(tree));
    
    return t;
}


tree* make_operator (tree* l, char o[], tree* r)
{
    tree* res= new_tree_node();
    
    res->nodetype= operator_node;
    res->body.an_operator.left= l;
    strncpy( res->body.an_operator.operator, o, 31);
    res->body.an_operator.right= r;
    
    return res;
}


tree* make_label( char l[], tree* e )
{
    tree* res= new_tree_node();
    
    res->nodetype= label_node;
    strncpy( res->body.an_label.label, l, 31);    
    res->body.an_label.exp1= e;
    
    return res;
}


tree* make_assign (char i[], tree* e)
{
    tree* res= new_tree_node();
    
    res->nodetype= assign_node;
    strncpy( res->body.an_assign.id, i, 31);
    res->body.an_assign.exp1= e;
    
    return res;
}


tree* make_operand (char v[], int type)
{
    tree* res= new_tree_node();
    
    res->nodetype= operand_node;
    res->body.an_operand.type= type;
    strncpy( res->body.an_operand.val, v, 31);
    
    return res;
}


tree* make_nop()
{
    tree* res= new_tree_node();
    res->nodetype= nop_node;

    return res;
}


tree* make_list( tree* exp1, tree* exp2 )
{
    tree* res= new_tree_node();

    res->nodetype= list_node;
    res->body.list.exp1= exp1;
    res->body.list.exp2= exp2;

    return res;
}


tree* make_if( tree* cond, tree* exp, char id[] )
{
    tree* res= new_tree_node();

    res->nodetype= if_node;
    res->body._if.cond= cond;
    res->body._if.exp1= exp;
    strncpy( res->body._if.id, id, 31 );

    return res;
}

tree* make_while( tree* cond, tree* exp, char id[] )
{
    tree* res= new_tree_node();

    res->nodetype= while_node;
    res->body._while.cond= cond;
    res->body._while.exp1= exp;
    strncpy( res->body._while.id, id, 31 );

    return res;
}

tree* make_dowhile( tree* cond, tree* exp, char id[] )
{
    tree* res= new_tree_node();

    res->nodetype= dowhile_node;
    res->body.dowhile.cond= cond;
    res->body.dowhile.exp1= exp;
    strncpy( res->body.dowhile.id, id, 31 );

    return res;
}


tree* make_ifelse( tree* cond, tree* exp1, tree* exp2, char id[] )
{
    tree* res= new_tree_node();

    res->nodetype= ifelse_node;
    res->body.ifelse.cond= cond;
    res->body.ifelse.exp1= exp1;
    res->body.ifelse.exp2= exp2;    
    strncpy( res->body.ifelse.id, id, 31 );

    return res;
}

tree* make_fcall( char id[], tree* exp1, unsigned long attribute )
{
    tree* res= new_tree_node();

    res->nodetype= fcall_node;
    res->body.fcall.exp1= exp1;
    res->body.fcall.attr=  attribute;
    strncpy( res->body.fcall.id, id, 31 );

    return res;
}


void printtree (tree *t, int level)
{
#define STEP 4
    if (t)
        switch (t->nodetype)
        {
            case operator_node:
                
                printtree (t->body.an_operator.right, level+STEP);
                printf ("%*c%s\n", level, ' ', t->body.an_operator.operator);
                printtree (t->body.an_operator.left, level+STEP);
                break;
                
            case assign_node:
                
                printtree (t->body.an_assign.exp1, level+STEP);
                printf ("%*c:=\n", level, ' ');
                printf ("%*c%s\n", level+STEP, ' ', t->body.an_assign.id);
                break;
                
            case operand_node:
                
                printf ("%*c%s\n", level, ' ', t->body.an_operand.val);
                break;

            case label_node:
                
                printf( "%*c%s:\n", level, ' ', t->body.an_label.label );
                printtree( t->body.an_label.exp1, level );
                break;

            case list_node:
                
                printtree( t->body.list.exp2, level+STEP );
                printf( "%*c%s:\n", level, ' ', "+list" );
                printtree( t->body.list.exp1, level+STEP );                
                break;

            case if_node:
                
                printtree( t->body._if.exp1, level+STEP );
                printf( "%*c%s:\n", level, ' ', "IF" );
                printtree( t->body._if.cond, level+STEP );
                break;

            case while_node:
                
                printtree( t->body._while.exp1, level+STEP );
                printf( "%*c%s:\n", level, ' ', "WHILE" );
                printtree( t->body._while.cond, level+STEP );
                break;
                
            case dowhile_node:
                
                printtree( t->body._while.exp1, level+STEP );
                printf( "%*c%s:\n", level, ' ', "DOWHILE" );
                printtree( t->body._while.cond, level+STEP );
                break;

            case ifelse_node:
                
                printtree( t->body.ifelse.exp2, level+STEP*2 );
                printf( "%*c%s:\n", level+STEP, ' ', "+EXPS" );
                printtree( t->body.ifelse.exp1, level+STEP*2 );
                printf( "%*c%s:\n", level, ' ', "IFELSE" );
                printtree( t->body.ifelse.cond, level+STEP*2 );
                break;

            case fcall_node:
                
                printtree( t->body.fcall.exp1, level+STEP );
                printf( "%*cFCALL:\n", level, ' ' );
                printf( "%*c%s attr %#lx\n", level+STEP, ' ', t->body.fcall.id, t->body.fcall.attr );
                break;

            case nop_node:
                
                printf( "%*cnop\n", level, ' ' );
                break;
            
            default:
                printf( "%*c #%i [ERROR]\n", level, '*', t->nodetype );
        }
#undef STEP
}


