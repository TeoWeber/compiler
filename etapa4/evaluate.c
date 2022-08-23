// evaluate.c - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#include "evaluate.h"

int errors = 0;

int getDataType(AST* node) {
  int dataType = 0;

  switch(node->type)
  {
    case AST_KWINT:
      dataType = DATA_TYPE_INT;
      break;
    case AST_KWFLOAT:
      dataType = DATA_TYPE_FLOAT;
      break;
    case AST_KWCHAR:
      dataType = DATA_TYPE_CHAR;
      break;
    default:
      break;
  }

  return dataType;
}

void validateInitVariable(AST* node) {
  AST *literal = node->son[1];
  int dataType = node->symbol->dataType;

  switch (dataType)
  {
    case DATA_TYPE_INT:
      if (literal->symbol->type != SYMBOL_LIT_INT) {
        fprintf(stderr, "Semantic ERROR: Variable \"%s\" has wrong initialization value!\n", node->symbol->text);
        ++errors;
      }
      break;
    case DATA_TYPE_FLOAT:
      if (literal->symbol->type != SYMBOL_LIT_FLOAT) {
        fprintf(stderr, "Semantic ERROR: Variable \"%s\" has wrong initialization value!\n", node->symbol->text);
        ++errors;
      }
      break;
    case DATA_TYPE_CHAR:
      if (literal->symbol->type != SYMBOL_LIT_CHAR) {
        fprintf(stderr, "Semantic ERROR: Variable \"%s\" has wrong initialization value!\n", node->symbol->text);
        ++errors;
      }
      break;
    default:
      break;
  }
}

int validateInitVectorValues(AST* node, int expectedType, int countValues) {
  AST *literal;
  AST *valueTail;

  if(!node)
    return countValues;

  ++countValues;

  literal = node->son[0];
  valueTail = node->son[1];

  if (literal->symbol->type != expectedType) {
    fprintf(stderr, "Semantic ERROR: Literal \"%s\" is of wrong type!\n", literal->symbol->text);
    ++errors;
  }

  return countValues = validateInitVectorValues(valueTail, expectedType, countValues);
}

void validateInitVector(AST* node) {
  AST *index = node->son[1];
  AST *vectorValues = node->son[2];
  int dataType = node->symbol->dataType;
  int indexValue = atoi(index->symbol->text);
  int countValues = 0;
  
  if(vectorValues) {
    switch(dataType)
    {
      case DATA_TYPE_INT:
        countValues = validateInitVectorValues(vectorValues, SYMBOL_LIT_INT, countValues);
        break;
      case DATA_TYPE_FLOAT:
        countValues = validateInitVectorValues(vectorValues, SYMBOL_LIT_FLOAT, countValues);
        break;
      case DATA_TYPE_CHAR:
        countValues = validateInitVectorValues(vectorValues, SYMBOL_LIT_CHAR, countValues);
        break;
      default:
        break;
    }

    if (countValues != indexValue) {
      fprintf(stderr, "Semantic ERROR: Vector \"%s\" has wrong amount of initialization values!\n", node->symbol->text);
      ++errors;
    }
  }
}

void evaluateVarDeclared(AST* node) {
  AST *firstSon = node->son[0];
  AST *secondSon = node->son[1];
  AST *thirdSon = node->son[2];
  int dataType;

  if(node->symbol->type != SYMBOL_IDENTIFIER) { // Already declared!
    fprintf(stderr, "Semantic ERROR: Variable \"%s\" already declared!\n", node->symbol->text);
    ++errors;
  }

  dataType = getDataType(firstSon);
  node->symbol->dataType = dataType;

  if(secondSon->type == AST_INDEX) { // Vector
    node->symbol->type = SYMBOL_VECTOR;
    validateInitVector(node);
  }
  else { // Value
    node->symbol->type = SYMBOL_VARIABLE;
    validateInitVariable(node);
  }
}

void evaluateParamDeclared(AST *node) {
  AST *typeNode;
  AST *paramTail;

  if (!node)
    return;

  typeNode = node->son[0];
  paramTail = node->son[1];
  
  if(node->symbol->type != SYMBOL_IDENTIFIER) { // Already declared!
    fprintf(stderr, "Semantic ERROR: Parameter \"%s\" already declared!\n", node->symbol->text);
    ++errors;
  }

  node->symbol->type = SYMBOL_PARAM;
  node->symbol->dataType = getDataType(typeNode);

  evaluateParamDeclared(paramTail);
}

void evaluateFuncDeclared(AST *node) {
  AST *typeNode = node->son[0];
  AST *paramList = node->son[1];
  int dataType;

  if(node->symbol->type != SYMBOL_IDENTIFIER) { // Already declared!
    fprintf(stderr, "Semantic ERROR: Function \"%s\" already declared!\n", node->symbol->text);
    ++errors;
  }

  node->symbol->type = SYMBOL_FUNCTION;
  node->symbol->dataType = getDataType(typeNode);
  node->symbol->paramList = paramList;

  evaluateParamDeclared(paramList);
}

void evaluateDeclared(AST* node) {
  if(!node)
    return;

  switch (node->type)
  {
    case AST_VAR: // VARIABLE DECLARATION
      evaluateVarDeclared(node->son[0]);
      break;
    case AST_FUNC: // FUNCTION DECLARATION
      evaluateFuncDeclared(node->son[0]);
      break;
    default:
      break;
  }

  evaluateDeclared(node->son[1]);
}