/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#ifndef __TREE_H_
#define __TREE_H_

/* $Id: tree.h,v 1.1 2006/09/20 09:10:24 tikkeri Exp $ */

#include "config.h"
#include "types.h"

enum treetype
{
    nop_node = 0, operator_node, operand_node, assign_node, label_node,
    list_node, if_node, ifelse_node, while_node, dowhile_node, fcall_node
};



typedef struct _tree tree;

struct _tree
{
    enum treetype nodetype;
    
    union
    {
        
        struct 
        {
            int type;
            tree *left, *right;
            char operator[32];
        } an_operator;
        
        struct
        {
            tree* exp1;
            unsigned long int attr;
            char id[32];
        } fcall;
        
        struct
        {
            tree* exp1;
            char id[32];
        } an_assign;
        
        struct
        {
            tree* exp1;
            char label[32];
        } an_label;
        
        struct
        {
            int type;
            char val[32];
        } an_operand;

        struct 
        {
            tree* exp1;
            tree* exp2;
        } list;

        struct
        {
            tree* cond;
            tree* exp1;
            char id[32];
        } _if;

        struct
        {
            tree* cond;
            tree* exp1;
            tree* exp2;
            char id[32];
        } ifelse;

        struct
        {
            tree* cond;
            tree* exp1;
            char id[32];
        } _while;

        struct
        {
            tree* cond;
            tree* exp1;
            char id[32];
        } dowhile;

        
    } body;

};


tree* new_tree_node( void );
tree* make_operator (tree *l, char o[], tree *r);
tree* make_label( char l[], tree* e );
tree* make_assign (char i[], tree *e);
tree* make_operand (char v[], int type);
tree* make_nop();
tree* make_if( tree* cond, tree* exp, char id[] );
tree* make_ifelse( tree* cond, tree* exp1, tree* exp2, char id[] );
tree* make_list( tree* exp1, tree* exp2 );
tree* make_while( tree* cond, tree* exp, char id[] );
tree* make_dowhile( tree* cond, tree* exp, char id[] );
tree* make_fcall( char id[], tree* exp1, unsigned long attribute );

void printtree (tree *t, int level);

#endif /* not __TREE_H_ */

