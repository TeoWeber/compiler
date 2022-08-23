// evaluate.h - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "hash.h"
#include "ast.h"

extern int errors;

void evaluateDeclared(AST* node);

#endif
