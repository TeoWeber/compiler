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

int inferDataType(int type1, int type2) {
  if(type1 && type2) {
    if(type1 == type2) // type1 == type2 = type
      return type1;
    
    if(type1 == DATA_TYPE_INT && type2 == DATA_TYPE_CHAR) // int x char = int
      return DATA_TYPE_INT;
    if(type1 == DATA_TYPE_CHAR && type2 == DATA_TYPE_INT) // char x int = int
      return DATA_TYPE_INT;

    if(type1 == DATA_TYPE_FLOAT && type2 != DATA_TYPE_FLOAT) // float x !float = incompatible!
      return 0;
    if(type1 != DATA_TYPE_FLOAT && type2 == DATA_TYPE_FLOAT) // !float x float = incompatible!
      return 0;

    if(type1 == DATA_TYPE_BOOL && type2 != DATA_TYPE_BOOL) // bool x !bool = incompatible!
      return 0;
    if(type1 != DATA_TYPE_BOOL && type2 == DATA_TYPE_BOOL) // !bool x bool = incompatible!
      return 0;
  }

  return 0;
}

int getSymbolDataType(AST* node) {
  switch(node->symbol->type)
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

int getKWDataType(AST* node) {
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

int getVarDataType(AST *node) {
  return node->symbol->dataType;
}

void validateFuncCall(AST *paramList, AST *argList) {
  AST *argument;
  int argumentDataType;

  if (!paramList && argList) {
    fprintf(stderr, "Semantic ERROR: Too many arguments on function call!\n");
    ++errors;
    return;
  }
  
  if (paramList && !argList) {
    fprintf(stderr, "Semantic ERROR: Too few arguments on function call!\n");
    ++errors;
    return;
  }

  if (!paramList && !argList)
    return;

  argument = argList->son[0];
  argumentDataType = getExprDataType(argument);

  if (!isTypeCompatible(paramList->symbol->dataType, argumentDataType)) {
    fprintf(stderr, "Semantic ERROR: Argument \"%s\" has wrong type.\n", argument->symbol->text);
    ++errors;
  }

  validateFuncCall(paramList->son[1], argList->son[1]);
}

int getExprDataType(AST *node) {
  AST *index;
  HASH_NODE *symbol;
  int indexDataType = 0;
  int dataType = 0;
  int leftElemDT = 0;
  int rightElemDT = 0;

  if (!node)
    return 0;

  switch(node->type)
  {
    case AST_LITERAL:
      dataType = getSymbolDataType(node);
      break;
    case AST_IDVAR:
      if (node->symbol->type == SYMBOL_VARIABLE)
        dataType = getVarDataType(node);
      else
        dataType = 0;
      break;
    case AST_IDVEC:
      index = node->son[0];
      indexDataType = getExprDataType(index);

      if(isTypeCompatible(DATA_TYPE_INT, indexDataType) && node->symbol->type == SYMBOL_VECTOR)
        dataType = getVarDataType(node);
      else
        dataType = 0;
      break;
    case AST_ADD:
    case AST_SUB:
    case AST_MUL:
      leftElemDT = getExprDataType(node->son[0]);
      rightElemDT = getExprDataType(node->son[1]);
      dataType = inferDataType(leftElemDT, rightElemDT);
      break;
    case AST_DIV:
      dataType = DATA_TYPE_FLOAT;
      break;
    case AST_LT:
    case AST_GT:
    case AST_LE:
    case AST_GE:
    case AST_EQ:
    case AST_DIF:
    case AST_AND:
    case AST_OR:
    case AST_NEG:
      dataType = DATA_TYPE_BOOL;
      break;
    case AST_FUNCCALL:
      symbol = hashFind(node->symbol->text);
      dataType = symbol->dataType;
      validateFuncCall(symbol->paramList, node->son[0]);
      break;
  }

  return dataType;
}

void validateInitVariable(AST* node) {
  AST *literal = node->son[1];
  int dataType = node->symbol->dataType;

  literal->symbol->dataType = getSymbolDataType(literal); // Ex: SYMBOL_LIT_INT -> DATA_TYPE_INT
  
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

  literal->symbol->dataType = getSymbolDataType(literal);

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
  else {
    dataType = getKWDataType(firstSon);
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

  node->symbol->type = SYMBOL_VARIABLE;
  node->symbol->dataType = getKWDataType(typeNode);

  evaluateParamDeclared(paramTail);
}

int isCorrectSymbolType(AST *node, int expectedSymbolType) {
  if (node->symbol->type == SYMBOL_IDENTIFIER) { // Undeclared!
    fprintf(stderr, "Semantic ERROR: Identifier \"%s\" not declared!\n", node->symbol->text);
    ++errors;
    return 1;
  }

  return node->symbol->type == expectedSymbolType;
}

void evaluatePrint(AST *node) {
  int dataType;
  AST *expression;

  if (!node) 
    return;
  
  if (node->type == AST_PRINTEXPR) {
    expression = node->son[0];
    dataType = getExprDataType(expression);

    if(!dataType) {
      fprintf(stderr, "Semantic ERROR: Print command has incorrect expression.\n");
      ++errors;
    }
    
    evaluatePrint(node->son[1]);
  }
  else
    evaluatePrint(node->son[0]);
}

void evaluateCommands(AST *node, int returnValue) {
  AST *command;
  AST *commandList;
  AST *commandBlock;
  AST *index;
  AST *expression;
  int dataType;
  int indexDataType;

  if(!node)
    return;

  command = node->son[0];
  commandList = node->son[1];

  if(!command)
    return;

  switch (command->type)
  {
    case AST_ATTRIB:
      if(!isCorrectSymbolType(command, SYMBOL_VARIABLE)) {
        fprintf(stderr, "Semantic ERROR: \"%s\" has incorrect use! Should be used as a variable.\n", command->symbol->text);
        ++errors;
      }

      expression = command->son[0];
      dataType = getExprDataType(expression);

      if(!isTypeCompatible(command->symbol->dataType, dataType)) {
        if(dataType && command->symbol->dataType) {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being attributed with incompatible type.\n", command->symbol->text);
          ++errors;
        }
        else if (command->symbol->dataType) {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being attributed with incorrect expression.\n", command->symbol->text);
          ++errors;
        }
      }
      break;
    case AST_ATTRIBVEC:
      if(!isCorrectSymbolType(command, SYMBOL_VECTOR)) {
        fprintf(stderr, "Semantic ERROR: \"%s\" has incorrect use! Should be used as a vector.\n", command->symbol->text);
        ++errors;
      }

      index = command->son[0];
      indexDataType = getExprDataType(index);

      if(!isTypeCompatible(DATA_TYPE_INT, indexDataType)) {
        if (indexDataType) {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being attributed with incompatible index type.\n", command->symbol->text);
          ++errors;
        }
        else {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being attributed with incorrect index expression.\n", command->symbol->text);
          ++errors;
        }
      }

      expression = command->son[1];
      dataType = getExprDataType(expression);

      if(!isTypeCompatible(command->symbol->dataType, dataType)) {
        if (dataType && command->symbol->dataType) {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being attributed with incompatible type.\n", command->symbol->text);
          ++errors;
        }
        else if (command->symbol->dataType) {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being attributed with incorrect expression.\n", command->symbol->text);
          ++errors;
        }
      }
      break;
    case AST_READVEC:
      index = command->son[0];
      indexDataType = getExprDataType(index);

      if(!isTypeCompatible(DATA_TYPE_INT, indexDataType)) {
        if (indexDataType) {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being used with incompatible index type.\n", command->symbol->text);
          ++errors;
        }
        else {
          fprintf(stderr, "Semantic ERROR: \"%s\" is being used with incorrect index expression.\n", command->symbol->text);
          ++errors;
        }
      }
      break;
    case AST_PRINT:
      evaluatePrint(node->son[0]);
      break;
    case AST_RETURN:
      expression = command->son[0];
      dataType = getExprDataType(expression);

      if (!isTypeCompatible(returnValue, dataType)) {
        if (dataType) {
          fprintf(stderr, "Semantic ERROR: Return value is incompatible with function declaration.\n");
          ++errors;
        }
        else {
          fprintf(stderr, "Semantic ERROR: Return value has incorrect expression.\n");
          ++errors;
        }
      }
      break;
    case AST_IF:
      expression = command->son[0];
      dataType = getExprDataType(expression);

      if (!isTypeCompatible(dataType, DATA_TYPE_BOOL)) {
        if (dataType) {
          fprintf(stderr, "Semantic ERROR: Conditional expression should result in a boolean.\n");
          ++errors;
        }
        else {
          fprintf(stderr, "Semantic ERROR: Incorrect conditional expression.\n");
          ++errors;
        }
      }

      commandBlock = command->son[1];
      evaluateCommands(commandBlock->son[0], 0);
      break;
    case AST_IFELSE:
      expression = command->son[0];
      dataType = getExprDataType(expression);

      if (!isTypeCompatible(dataType, DATA_TYPE_BOOL)) {
        if (dataType) {
          fprintf(stderr, "Semantic ERROR: Conditional expression should result in a boolean.\n");
          ++errors;
        }
        else {
          fprintf(stderr, "Semantic ERROR: Incorrect conditional expression.\n");
          ++errors;
        }
      }

      commandBlock = command->son[1];
      evaluateCommands(commandBlock->son[0], 0);

      commandBlock = command->son[2];
      evaluateCommands(commandBlock->son[0], 0);
      break;
    case AST_WHILE:
      expression = command->son[0];
      dataType = getExprDataType(expression);

      if (!isTypeCompatible(dataType, DATA_TYPE_BOOL)) {
        if (dataType) {
          fprintf(stderr, "Semantic ERROR: Conditional expression should result in a boolean.\n");
          ++errors;
        }
        else {
          fprintf(stderr, "Semantic ERROR: Incorrect conditional expression.\n");
          ++errors;
        }
      }

      commandBlock = command->son[1];
      evaluateCommands(commandBlock->son[0], 0);
      break;
    case AST_CMDBLOCK:
      evaluateCommands(command->son[0], 0);
      break;
    default:
      break;
  }

  evaluateCommands(commandList, returnValue);
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
  node->symbol->dataType = getKWDataType(typeNode);
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

void evaluateFunctions(AST* node) {
  AST *functionHeader;
  AST *commandBlock;

  if(!node)
    return;

  if (node->type == AST_FUNC) {
    
    functionHeader = node->son[0];
    commandBlock = functionHeader->son[2];
    evaluateCommands(commandBlock->son[0], functionHeader->symbol->dataType);
  }

  evaluateFunctions(node->son[1]);
}
