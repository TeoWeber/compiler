// hash.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_SIZE 997

typedef struct hash_node
{
  int type;
  char *text;
  struct hash_node * next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(int type, char *text);
void hashPrint(void);

#endif
