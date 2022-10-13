// ast.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef CODE_H
#define CODE_H

#include "hash.h"
#include "ast.h"
#include <stdio.h>

#define TAC_SYMBOL      1
#define TAC_ADD         2
#define TAC_SUB         3
#define TAC_MUL         4
#define TAC_DIV         5
#define TAC_LT          6
#define TAC_GT          7
#define TAC_LE          8
#define TAC_GE          9
#define TAC_EQ          10
#define TAC_DIF         11
#define TAC_AND         12
#define TAC_OR          13
#define TAC_NEG         14
#define TAC_COPY        15
#define TAC_COPYVEC     16
#define TAC_JFALSE      17
#define TAC_LABEL       18
#define TAC_VECDECL     19
#define TAC_READ        20
#define TAC_READVEC     21
#define TAC_JUMP        22
#define TAC_PRINT       23
#define TAC_RETURN      24
#define TAC_BEGINFUN    25
#define TAC_ENDFUN      26
#define TAC_CALL        27
#define TAC_PARAM       28
#define TAC_ARG         29
#define TAC_VECGET      30

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
