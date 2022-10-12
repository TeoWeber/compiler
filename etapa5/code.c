#include "code.h"

// IMPLEMENTATION
TAC *tacCreate(int type, HASH_NODE *res, HASH_NODE *op1, HASH_NODE *op2) {
  TAC *newtac = 0;
  newtac = (TAC*) calloc(1, sizeof(TAC));
  
  newtac->type = type;
  newtac->res = res;
  newtac->op1 = op1;
  newtac->op2 = op2;
  newtac->prev = 0;
  newtac->next = 0;

  return newtac;
}


// CODE GENERATION
TAC *generateCode(AST *node);

// CODE PRINT
void tacPrint(TAC *tac) {
  if (!tac)
    return;
  
  fprintf(stderr, "TAC(");

  switch(tac->type)
  {
    case TAC_SYMBOL: 
      fprintf(stderr, "TAC_SYMBOL");
      break;
    case TAC_ADD: 
      fprintf(stderr, "TAC_ADD");
      break;
    case TAC_SUB: 
      fprintf(stderr, "TAC_SUB");
      break;
    default:
      fprintf(stderr, "TAC_UNKNOWN");
      break;
  }

  fprintf(stderr, ",%s", (tac->res)?tac->res->text:"0");
  fprintf(stderr, ",%s", (tac->op1)?tac->op1->text:"0");
  fprintf(stderr, ",%s", (tac->op2)?tac->op2->text:"0");
  fprintf(stderr, ");\n");
}

void tacPrintBackwards(TAC *tac) {
  if (!tac)
    return;
  
  tacPrintBackwards(tac->prev);
  tacPrint(tac);
}

TAC *tacJoin(TAC *l1, TAC *l2) {
  TAC *point;

  if (!l1)
    return l2;
  if (!l2)
    return l1;
  
  for (point = l2; point->prev != 0; point = point->prev);
  point->prev = l1;

  return l2;
}

TAC *generateCode(AST *node) {
  TAC *result = 0;
  TAC *code[MAX_SONS];
  int i;


  if (!node)
    return 0;

  // PROCESS CHILDREN
  for (i=0; i<MAX_SONS; ++i)
    code[i] = generateCode(node->son[i]);
  
  switch(node->type)
  {
    case AST_LITERAL:
      result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);
      break;
    default:
      result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
      break;
  }

  return result;
}
