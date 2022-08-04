// ast.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef AST_HEADER
#define AST_HEADER

#include "hash.h"

#define MAX_SONS 4

#define AST_VAR         0
#define AST_FUNC        1
#define AST_VARDECL     2
#define AST_VECDECL     3
#define AST_KWINT       4
#define AST_KWFLOAT     5
#define AST_KWCHAR      6
#define AST_INDEX       7
#define AST_LITERAL     8
#define AST_VECVALUE    9
#define AST_FUNCDECL    10
#define AST_PARAM       11
#define AST_CMDBLOCK    12
#define AST_CMDLIST     13
#define AST_CMD         14
#define AST_ATTRIB      15
#define AST_ATTRIBVEC   16
#define AST_READ        17
#define AST_READVEC     18
#define AST_PRINT       19
#define AST_PRINTSTRING 20
#define AST_PRINTEXPR   21
#define AST_RETURN      22
#define AST_IDVAR       23
#define AST_IDVEC       24
#define AST_ADD         25
#define AST_SUB         26
#define AST_MUL         27
#define AST_DIV         28
#define AST_LT          29
#define AST_GT          30
#define AST_LE          31
#define AST_GE          32
#define AST_EQ          33
#define AST_DIF         34
#define AST_AND         35
#define AST_OR          36
#define AST_NEG         37
#define AST_FUNCCALL    38
#define AST_ARGLIST     39
#define AST_IF          40
#define AST_IFELSE      41
#define AST_WHILE       42

#define PRINT_ERROR     98
#define DECOMPILE_ERROR 99

typedef struct astnode
{
  int type;
  HASH_NODE *symbol;
  struct astnode *son[MAX_SONS];
} AST;

AST *astCompiled;

AST *astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3);
void astDecompiler(FILE* file, AST* node);
void astPrint(AST *node, int level);


#endif

// END OF FILE
