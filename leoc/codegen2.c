/*
 * Copyright (C) 2005, 2006 Artem V. Leonenko aka tikkeri
 * All rights reserverd.
 *
 * This file is part of the leoc project.
 */
#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "codegen2.h"
#include "utils.h"

RCSID( "$Id: codegen2.c,v 1.1 2006/09/20 09:10:24 tikkeri Exp $" );

#ifdef __GNUC__
# define EMIT(...)  fprintf( codeout2, __VA_ARGS__ )
# define ERR(...)   fprintf( codeout2, "%s:\n", __PRETTY_FUNCTION__ ), fprintf( codeout2, __VA_ARGS__ )
#else /* not __GNUC__ */
# error Ooops... It seems you are to rewrite some macros...
#endif /* __GNUC__ */

FILE* codeout2;

static int eax_used;
static int ebx_used;
static int ecx_used;
static int edx_used;
static int pushed_args_size;
static unsigned ilabel;



static int getilabel( void )
{
    return ilabel++;
}


int init_codegen2( void )
{
    eax_used = 0;
    ebx_used = 0;
    ecx_used = 0;
    edx_used = 0;
    pushed_args_size = 0;
    ilabel= 0;

    return 0;
}

static void get_cond_flags( char cond[], char flags[], unsigned long type)
{
    if( ( type & TYPE_SIGNED ) && ( type & TYPE_UNSIGNED ) )
        ERR( "Conficting types speciefied!" );

    flags[0] = '\0';

    if( cond[0] == '\0' )
    {
        ERR( "Empty condition speciefied!" );
        cond[0] = cond[1] = '=';
    }

    if( ! strcmp( cond, "==" ) )        {   strcpy( flags, "e"  );  return;     }
    else if( ! strcmp( cond, "!=" ) )   {   strcpy( flags, "ne" );  return;     }

    if( type & TYPE_UNSIGNED )
    {
        if( ! strcmp( cond, "<=" ) )        strcpy( flags, "be" );
        else if( ! strcmp( cond, ">=" ) )   strcpy( flags, "ae" );
        else if( ! strcmp( cond, "<"  ) )   strcpy( flags, "b"  );
        else if( ! strcmp( cond, ">"  ) )   strcpy( flags, "a"  );

    }
    else
    {
        if( ! strcmp( cond, "<=" ) )        strcpy( flags, "le" );
        else if( ! strcmp( cond, ">=" ) )   strcpy( flags, "ge" );
        else if( ! strcmp( cond, "<"  ) )   strcpy( flags, "l"  );
        else if( ! strcmp( cond, ">"  ) )   strcpy( flags, "g"  );
    }

    if( flags[0] == '\0' )
    {
        ERR( "Codegen panic: unknown condition %s\n", cond );
        flags[0] = 'e';
    }

}


/*
 * 
static void get_not_cond_flags( char cond[], char flags[], unsigned long type)
{
    if( ( type & TYPE_SIGNED ) && ( type & TYPE_UNSIGNED ) )
        ERR( "Conficting types speciefied!" );

    flags[0] = '\0';

    if( cond[0] == '\0' )
    {
        ERR( "Empty condition speciefied!" );
        cond[0] = cond[1] = '=';
    }

    if( ! strcmp( cond, "==" ) )        {   strcpy( flags, "ne" );  return;     }
    else if( ! strcmp( cond, "!=" ) )   {   strcpy( flags, "e"  );  return;     }

    if( type & TYPE_UNSIGNED )
    {
        if( ! strcmp( cond, "<=" ) )        strcpy( flags, "a"  );
        else if( ! strcmp( cond, ">=" ) )   strcpy( flags, "b"  );
        else if( ! strcmp( cond, "<"  ) )   strcpy( flags, "nb" );
        else if( ! strcmp( cond, ">"  ) )   strcpy( flags, "na" );

    }
    else
    {
        if( ! strcmp( cond, "<=" ) )        strcpy( flags, "g"  );
        else if( ! strcmp( cond, ">=" ) )   strcpy( flags, "l"  );
        else if( ! strcmp( cond, "<"  ) )   strcpy( flags, "nl" );
        else if( ! strcmp( cond, ">"  ) )   strcpy( flags, "ng" );
    }

    if( flags[0] == '\0' )
    {
        ERR( "Codegen panic: unknown condition %s\n", cond );
        flags[0] = 'e';
    }
}
 *
 */



static void push_args( tree* exp1 )
{
    if( ! exp1 )
        return;

    switch( exp1->nodetype )
    {
        case list_node:

            push_args( exp1->body.list.exp2 );
            push_args( exp1->body.list.exp1 );
            break;

        case operand_node:

            if( exp1->body.an_operand.type=='i')
                EMIT("\tpushl\t%s\n", exp1->body.an_operand.val);
            else
                EMIT("\tpushl\t$%s\n", exp1->body.an_operand.val);

            pushed_args_size += 4;
            break;

        case operator_node:

            {
                if( eax_used )
                {
                    ERR( "Codegen panic: %%eax used!" );
                    EMIT( " ud2\n" );
                }


                eax_used = 0;

                gen2( exp1 );

                eax_used = 0;

                EMIT( "\tpushl\t%%eax\n" );

                pushed_args_size += 4;

            }
            break;

        default:

            ERR( "Error node_type number: %i\n", exp1->nodetype );
            break;
    }

}


int gen2 (tree *t)
{
    if (!t)
        return -1;

    switch (t->nodetype)
    {
        case list_node:

            gen2( t->body.list.exp1 );
            gen2( t->body.list.exp2 );
            break;

        case fcall_node:

            pushed_args_size = 0;
            push_args( t->body.fcall.exp1 );

            switch ( t->body.fcall.attr )
            {
                case FUNC_CCALL:

                    EMIT( "\tcall\t%s\n", t->body.fcall.id );

                    if( pushed_args_size != 0  )
                        EMIT( "\taddl\t$%i, %%esp\n", pushed_args_size );
                    break;

                case FUNC_STDCALL:

                    EMIT( "\tcall\t%s@%i\n", t->body.fcall.id, pushed_args_size );
                    break;

                default:

                    ERR( "Codegen panic: don't know how to handle attr %lu\n", t->body.fcall.attr );
                    EMIT( " addl\t$%i, %%esp\n", pushed_args_size );
            }

            break;


        case label_node:

            EMIT( "\n_%s:\n", t->body.an_label.label );
            gen2( t->body.an_label.exp1);
            break;


        case assign_node:

            gen2 (t->body.an_assign.exp1);
            EMIT( "\tmovl\t%%eax, %s\n", t->body.an_assign.id);
            eax_used= 0;
            break;


        case operator_node:

            gen2( t->body.an_operator.left );

            if( t->body.an_operator.right->nodetype == operand_node)
            {
                char* val = t->body.an_operator.right->body.an_operand.val;

                if( ! strcmp( "GOTO", t->body.an_operator.operator ) )
                {
                    EMIT( "\tjmp\t_%s\n", val );
                }

                if( ! strcmp( "mul", t->body.an_operator.operator ) )
                {
                    if( t->body.an_operator.right->body.an_operand.type == 'i' )
                        EMIT( "\tmull\t%s\n", val );
                    else
                        EMIT( "\tmull\t$%s\n", val );
                }

                if( ! strcmp( "div", t->body.an_operator.operator ) )
                {
                    if( t->body.an_operator.right->body.an_operand.type == 'i' )
                        EMIT( "\tdivl\t%s\n", val );
                    else
                    {
                        if( ecx_used )
                            EMIT( "\tpushl\t%%ecx\n" );

                        EMIT( "\tmovl\t$%s, %%ecx\n", val );
                        EMIT( "\tdivl\t%%ecx\n" );

                        if( ecx_used )
                            EMIT( "\tpopl\t%%ecx\n" );
                    }
                }

                if( ! strcmp( "add", t->body.an_operator.operator ) )
                {
                    if( t->body.an_operator.right->body.an_operand.type == 'i' )
                        EMIT( "\taddl\t%s, %%eax\n", val );
                    else
                        EMIT( "\taddl\t$%s, %%eax\n", val );
                }

                if( ! strcmp( "sub", t->body.an_operator.operator ) )
                {
                    if( t->body.an_operator.right->body.an_operand.type == 'i' )
                        EMIT( "\tsubbl\t%s, %%eax\n", val );
                    else
                        EMIT( "\tsubbl\t$%s, %%eax\n", val );
                }

                if( ! strcmp( "==", t->body.an_operator.operator ) 
                        || ! strcmp( "!=", t->body.an_operator.operator ) 
                        || ! strcmp( ">=", t->body.an_operator.operator ) 
                        || ! strcmp( "<=", t->body.an_operator.operator ) 
                        || ! strcmp( "<",  t->body.an_operator.operator ) 
                        || ! strcmp( ">",  t->body.an_operator.operator ) )
                {
                    char flags[4] = { '\0' };

                    get_cond_flags( t->body.an_operator.operator, flags, TYPE_SIGNED );

                    /* XXX: cmpl %eax, %ebx? or cmpl %ebx, %eax? I dont remember exactly... */
                    if( t->body.an_operator.right->body.an_operand.type == 'i' )
                        EMIT( "\tcmpl\t%s, %%eax\n", val );
                    else
                        EMIT( "\tcmpl\t$%s, %%eax\n", val );

                    EMIT( "\tset%s\t%%al\n", flags );
                    EMIT( "\tandl\t$0x1, %%eax\n" );

                }

            }
            else
            {

                /*  TODO:   Check for free register and use it. Otherwise save one. */
                /*  XXX:    What about changing %ebx to the %edx? */
                gen2 (t->body.an_operator.right);

                if( ebx_used )
                    EMIT( "\tpushl\t%%ebx\n" );

                EMIT( "\tmovl\t%%eax, %%ebx\n" );
                EMIT( "\tpopl\t%%eax\n" );

                if( ! strcmp( "mul", t->body.an_operator.operator ) )
                    EMIT( "\tmull\t%%ebx\n");

                if( ! strcmp( "add", t->body.an_operator.operator ) )
                    EMIT( "\taddl\t%%ebx, %%eax\n");

                if( ! strcmp( "div", t->body.an_operator.operator ) )
                {
                    if( edx_used )
                        EMIT( "\tpushl\t%%edx\n" );
                    
                    EMIT( "\tdivl\t%%ebx\n");
                    
                    if( edx_used )
                        EMIT( "\tpopl\t%%edx\n" );
                }

                if( ! strcmp( "sub", t->body.an_operator.operator ) )
                    EMIT( "\tsubl\t%%ebx, %%eax\n"  );

                if( ! strcmp( "==", t->body.an_operator.operator ) 
                        || ! strcmp( "!=", t->body.an_operator.operator ) 
                        || ! strcmp( ">=", t->body.an_operator.operator ) 
                        || ! strcmp( "<=", t->body.an_operator.operator ) 
                        || ! strcmp( "<",  t->body.an_operator.operator ) 
                        || ! strcmp( ">",  t->body.an_operator.operator ) )
                {
                    char flags[4] = { '\0' };

                    get_cond_flags( t->body.an_operator.operator, flags, TYPE_SIGNED );

                    /* XXX: cmpl %eax, %ebx? or cmpl %ebx, %eax? I dont remember exactly... */
                    EMIT( "\tcmpl\t%%eax, %%ebx\n" );
                    EMIT( "\tset%s\t%%al\n", flags );
                    EMIT( "\tandl\t$0x1, %%eax\n" );
                }

                if( ebx_used )
                    EMIT( "\tpopl\t%%ebx\n" );

            }
            break; /* operator_node: */


        case operand_node:

            if (eax_used)
                EMIT( "\tpushl\t%%eax\n");

            if( t->body.an_operand.type=='i' )
                EMIT( "\tmovl\t%s, %%eax\n", t->body.an_operand.val);
            else
                EMIT( "\tmovl\t$%s, %%eax\n", t->body.an_operand.val);

            eax_used= 1;
            break;


        case nop_node:

            EMIT( "\tnop\n" );
            break;


        case if_node:
            {
                int eax;
                char label[16];

                label[0]= 0;
                sprintf( label, "L%i", getilabel() );

                if( eax_used )
                    EMIT( "\tpushl\t%%eax\n" );

                eax= eax_used;
                eax_used = 0;

                gen2( t->body._if.cond );
         
                EMIT( "\ttestl\t%%eax, %%eax\n" );
                EMIT( "\tjne\t%s\n", label );

                eax_used = 0;
                       
                gen2( t->body._if.exp1 );

                EMIT( "%s:\n", label );

                if( eax )
                    EMIT( "\tpopl\t%%eax\n" );

                eax_used = eax;
            }
            break;


        case ifelse_node:
            {
                int eax = eax_used;
                char label[16];
                char label2[16];

                label[0]= 0;
                label2[0]=0;
                sprintf( label,  "L%i", getilabel() );
                sprintf( label2, "L%i", getilabel() );

                if( eax )
                    EMIT( "\tpushl\t%%eax\n" );

                eax_used= 0;

                gen2( t->body.ifelse.cond );

                EMIT( "\ttestl\t%%eax, %%eax\n" );
                EMIT( "\tje\t%s\n", label );

                gen2( t->body.ifelse.exp1 );
                eax_used= 0;

                EMIT( "\tjmp\t%s\n", label2 );
                EMIT( "%s:\n", label );

                gen2( t->body.ifelse.exp2 );
                eax_used= 0;

                EMIT( "%s:\n", label2 );

                if( eax )
                    EMIT( "\tpopl\t%%eax\n" );

                eax_used = eax;
            }
            break;


        case while_node:
            {
                char label[16];
                char label2[16];
                int eax = eax_used;

                label[0] = '\0';
                label2[0] = '\0';
                sprintf( label, "L%i", getilabel() );
                sprintf( label2, "L%i", getilabel() );
                
                if( eax )
                    EMIT( "\tpushl\t%%eax\n" );

                eax_used = 0;

                EMIT( "%s:\n", label2 );

                gen2( t->body._while.cond );

                EMIT( "\ttestl\t%%eax, %%eax\n" );
                EMIT( "\tje\t%s\n", label );

                eax_used = 0;

                gen2( t->body._while.exp1 );

                EMIT( "\tjmp\t%s\n", label2 );
                EMIT( "%s:\n", label );

                if( eax )
                    EMIT( "\tpopl\t%%eax\n" );

                eax_used= eax;

            }
            break;

        case dowhile_node:
            {

                char label[16];
                int eax = eax_used;

                label[0] = '\0';
                sprintf( label, "L%i", getilabel() );
                

                if( eax )
                    EMIT( "\tpushl\t%%eax\n" );

                eax_used = 0;

                EMIT( "%s:\n", label );
                gen2( t->body.dowhile.exp1 );

                eax_used = 0;

                gen2( t->body.dowhile.cond );

                EMIT( "\ttestl\t%%eax, %%eax\n" );
                EMIT( "\tje\t%s\n", label );

                eax_used = 0;

                if( eax )
                    EMIT( "\tpopl\t%%eax\n" );

                eax_used= eax;
            }
            break;


        default:

            ERR( "\nCodegen: unknown node type(deflt)\n" );
            EMIT( " ud2\n" );
    }

    return 0;
}


