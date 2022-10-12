// ast.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef CODE_H
#define CODE_H

#include "hash.h"
#include "ast.h"
#include <stdio.h>

#define TAC_SYMBOL 1
#define TAC_ADD 2
#define TAC_SUB 3

typedef struct tac_node
{
    int type;

    HASH_NODE *res;
    HASH_NODE *op1;
    HASH_NODE *op2;

    struct tac_node *prev;
    struct tac_node *next;
} TAC;

// IMPLEMENTATION
TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2);
TAC *tacJoin(TAC *l1, TAC *l2);

// CODE GENERATION
TAC *generateCode(AST *node);

// CODE PRINT
void tacPrint(TAC *tac);
void tacPrintBackwards(TAC *tac);

#endif
