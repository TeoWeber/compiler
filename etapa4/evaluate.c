// evaluate.c - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#include "evaluate.h"

int errors = 0;

int isTypeCompatible (int varType, int dataType) {
  if (!varType || !dataType)
    return 0;

  switch(varType)
  {
    case DATA_TYPE_INT: // Compatible with integer
      if(dataType == DATA_TYPE_FLOAT || dataType == DATA_TYPE_BOOL)
        return 0;
      break;
    case DATA_TYPE_FLOAT: // Incompatible with anything
      if(dataType == DATA_TYPE_INT || dataType == DATA_TYPE_CHAR || dataType == DATA_TYPE_BOOL)
        return 0;
      break;
    case DATA_TYPE_CHAR: // Compatible with char
      if(dataType == DATA_TYPE_FLOAT || dataType == DATA_TYPE_BOOL)
        return 0;
      break;
    case DATA_TYPE_BOOL: // Incompatible with anything
      if(dataType == DATA_TYPE_INT || dataType == DATA_TYPE_CHAR || dataType == DATA_TYPE_FLOAT)
        return 0;
      break;
  }

  return 1;
}

int convertSTtoDT(int symbolType) {
  switch(symbolType)
  {
    case SYMBOL_LIT_INT:
      return DATA_TYPE_INT;
    case SYMBOL_LIT_FLOAT:
      return DATA_TYPE_FLOAT;
    case SYMBOL_LIT_CHAR:
      return DATA_TYPE_CHAR;
    default:
      return 0;
  }
}

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

  literal->symbol->dataType = convertSTtoDT(literal->symbol->type); // Ex: SYMBOL_LIT_INT -> DATA_TYPE_INT
  
  if (!isTypeCompatible(dataType, literal->symbol->dataType)) {
    fprintf(stderr, "Semantic ERROR: Variable \"%s\" has wrong initialization value!\n", node->symbol->text);
    ++errors;
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

  literal->symbol->dataType = convertSTtoDT(literal->symbol->type);

  if (!isTypeCompatible(expectedType, literal->symbol->dataType)) {
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
    countValues = validateInitVectorValues(vectorValues, dataType, countValues);

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
