// evaluate.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "hash.h"
#include "ast.h"

extern int errors;

int getExprDataType(AST* node);
void evaluateDeclared(AST* node);
void evaluateFunctions(AST* node);

#endif
