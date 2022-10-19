// hash.c - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#include "hash.h"

HASH_NODE*Hash[HASH_SIZE];

void hashInit(void) {
  int i;

  for (i = 0; i < HASH_SIZE; i++)
    Hash[i] = 0;
}

int hashAddress(char *text) {
  int i;
  int addr = 1;

  for (i = 0; i < strlen(text); i++)
    addr = (addr * text[i]) % HASH_SIZE + 1;
  
  return addr - 1;
}

HASH_NODE *hashFind(char *text) {
  HASH_NODE *node;
  int addr = hashAddress(text);

  for (node = Hash[addr]; node; node = node->next)
    if (strcmp(text, node->text) == 0)
      return node;
  
  return 0;
}

HASH_NODE *hashInsert(int type, char *text) {
  HASH_NODE *newnode;
  int addr = hashAddress(text);

  if ((newnode = hashFind(text)) != 0)
    return newnode;

  newnode = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
  newnode->text = (char*) calloc(strlen(text)+1, sizeof(char));

  newnode->type = type;
  newnode->dataType = 0;
  newnode->paramList = 0;
  strcpy(newnode->text, text);
  newnode->next = Hash[addr];
  Hash[addr] = newnode;

  return newnode;
}

void hashPrint(void) {
  int i;
  HASH_NODE *node;

  for (i = 0; i < HASH_SIZE; i++)
    for (node=Hash[i]; node; node = node->next)
      printf("Hash[%d] has %s from type id %d\n", i, node->text, node->type);
}
