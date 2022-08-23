// evaluate.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "hash.h"
#include "ast.h"

extern int errors;

int getDataType(AST* node);
void validateInitVariable(AST* node);
int validateInitVectorValues(AST* node, int expectedType, int countValues);
void validateInitVector(AST* node);
void evaluateVarDeclared(AST* node);
void evaluateDeclared(AST* node);

#endif
