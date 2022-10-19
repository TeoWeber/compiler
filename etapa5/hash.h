// hash.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 997

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_FLOAT 2
#define SYMBOL_LIT_CHAR 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_IDENTIFIER 5

#define SYMBOL_VARIABLE 6
#define SYMBOL_VECTOR 7
#define SYMBOL_FUNCTION 8
#define SYMBOL_LABEL 9

#define DATA_TYPE_INT 101
#define DATA_TYPE_FLOAT 102
#define DATA_TYPE_CHAR 103
#define DATA_TYPE_BOOL 104

typedef struct astnode AST;

typedef struct hash_node
{
  int type;
  char *text;
  struct hash_node * next;
  int dataType;
  AST *paramList;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(int type, char *text);
void hashPrint(void);
HASH_NODE *makeTemp(void);
HASH_NODE *makeLabel(void);

#endif
