// ast.c - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

#include "ast.h"
  

AST *astCreate(int type, HASH_NODE *symbol, AST* s0, AST* s1, AST* s2, AST* s3, int lineNumber)
{
  AST *newnode;

  newnode = (AST*) calloc(1, sizeof(AST));
  newnode->type = type;
  newnode->symbol = symbol;
  newnode->son[0] = s0;
  newnode->son[1] = s1;
  newnode->son[2] = s2;
  newnode->son[3] = s3;
  newnode->lineNumber = lineNumber;

  return newnode;
}

void astDecompiler(FILE* file, AST* node)
{
  int i;
  if (node == 0)
    return;

  switch(node->type)
  {
    case AST_VAR:
      astDecompiler(file, node->son[0]); // VARIABLE DECLARATION
      astDecompiler(file, node->son[1]); // DECLARATION LIST
      break;
    case AST_FUNC:
      astDecompiler(file, node->son[0]); // FUNCTION DECLARATION
      astDecompiler(file, node->son[1]); // DECLARATION LIST
      break;
    case AST_VARDECL:
      astDecompiler(file, node->son[0]); // TYPE
      fprintf(file, "%s(", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[1]); // LITERAL
      fprintf(file, ");\n");
      break;
    case AST_VECDECL:
      astDecompiler(file, node->son[0]); // TYPE
      fprintf(file, "%s[", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[1]); // INDEX
      fprintf(file, "]");
      astDecompiler(file, node->son[2]); // VALUE LIST
      fprintf(file, ";\n");
      break;
    case AST_KWINT: 
      fprintf(file, "int "); // TYPE int
      break;
    case AST_KWFLOAT: 
      fprintf(file, "float "); // TYPE float
      break;
    case AST_KWCHAR: 
      fprintf(file, "char "); // TYPE char
      break;
    case AST_INDEX:
      fprintf(file, "%s", node->symbol->text); // INDEX INTEGER
      break;
    case AST_LITERAL:
      fprintf(file, "%s", node->symbol->text); // INT/FLOAT/CHAR LITERAL
      break;
    case AST_VECVALUE:
      astDecompiler(file, node->son[0]); // VALUE
      fprintf(file, " ");
      astDecompiler(file, node->son[1]); // VALUE LIST
      break;
    case AST_FUNCDECL:
      astDecompiler(file, node->son[0]); // TYPE
      fprintf(file, "%s(", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[1]); // PARAMETER LIST
      fprintf(file, ")\n");
      astDecompiler(file, node->son[2]); // COMMAND BLOCK
      fprintf(file, "\n");
      break;
    case AST_PARAM:
      astDecompiler(file, node->son[0]); // TYPE
      fprintf(file, "%s ", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[1]); // PARAMETER LIST
      break;
    case AST_CMDBLOCK:
      fprintf(file, "{\n");
      astDecompiler(file, node->son[0]); // COMMAND LIST
      fprintf(file, "}");
      break;
    case AST_CMDLIST:
      astDecompiler(file, node->son[0]); // COMMAND
      fprintf(file, ";\n");
      astDecompiler(file, node->son[1]); // COMMAND LIST
      break;
    case AST_CMD:
      astDecompiler(file, node->son[0]); // COMMAND
      break;
    case AST_ATTRIB:
      fprintf(file, "%s <- ", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[0]); // EXPRESSION
      break;
    case AST_ATTRIBVEC:
      fprintf(file, "%s[", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "] <- ");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_READ:
      fprintf(file, "read %s", node->symbol->text); // read IDENTIFIER
      break;
    case AST_READVEC:
      fprintf(file, "read %s[", node->symbol->text); // read IDENTIFIER
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "]");
      break;
    case AST_PRINT:
      fprintf(file, "print");
      astDecompiler(file, node->son[0]); // PRINT LIST
      break;
    case AST_PRINTSTRING:
      fprintf(file, " %s", node->symbol->text); // STRING
      astDecompiler(file, node->son[0]); // PRINT LIST
      break;
    case AST_PRINTEXPR:
      fprintf(file, " ");
      astDecompiler(file, node->son[0]); // EXPRESSION
      astDecompiler(file, node->son[1]); // PRINT LIST
      break;
    case AST_RETURN:
      fprintf(file, "return ");
      astDecompiler(file, node->son[0]); // EXPRESSION
      break;
    case AST_IDVAR:
      fprintf(file, "%s", node->symbol->text); // IDENTIFIER
      break;
    case AST_IDVEC:
      fprintf(file, "%s[", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "]");
      break;
    case AST_ADD:
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "+");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_SUB:
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "-");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_MUL:
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, ".");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_DIV:
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "/");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_LT: 
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "<");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_GT: 
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, ">");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_LE: 
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "<=");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_GE: 
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, ">=");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_EQ: 
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "==");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_DIF:
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "!=");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_AND:
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "&");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_OR: 
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, "|");
      astDecompiler(file, node->son[1]); // EXPRESSION
      break;
    case AST_NEG:
      fprintf(file, "~(");
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, ")");
      break;
    case AST_FUNCCALL:
      fprintf(file, "%s(", node->symbol->text); // IDENTIFIER
      astDecompiler(file, node->son[0]); // ARGUMENT LIST
      fprintf(file, ")");
      break;
    case AST_ARGLIST:
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, " ");
      astDecompiler(file, node->son[1]); // ARGUMENT LIST
      break;
    case AST_IF:
      fprintf(file, "if(");
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, ")");
      astDecompiler(file, node->son[1]); // COMMAND
      break;
    case AST_IFELSE:
      fprintf(file, "if(");
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, ")\n");
      astDecompiler(file, node->son[1]); // COMMAND
      fprintf(file, "else\n");
      astDecompiler(file, node->son[2]); // COMMAND
      break;
    case AST_WHILE:
      fprintf(file, "while(");
      astDecompiler(file, node->son[0]); // EXPRESSION
      fprintf(file, ")\n");
      astDecompiler(file, node->son[1]); // COMMAND
      break;
    default:
      fprintf(file, "DECOMPILE ERROR!"); 
      exit(DECOMPILE_ERROR);
      break;
  }
}

void astPrint(AST *node, int level)
{
  int i;
  
  if (node == 0)
    return;
  
  for (i=0; i<level; ++i)
    fprintf(stderr, " | ");

  switch(node->type)
  {
    case AST_VAR:
      fprintf(stderr, "VAR");
      break;
    case AST_FUNC:
      fprintf(stderr, "FUNC");
      break;
    case AST_VARDECL:
      fprintf(stderr, "VARDECL");
      break;
    case AST_VECDECL:
      fprintf(stderr, "VECDECL");
      break;
    case AST_KWINT:
      fprintf(stderr, "KWINT");
      break;
    case AST_KWFLOAT:
      fprintf(stderr, "KWFLOAT");
      break;
    case AST_KWCHAR:
      fprintf(stderr, "KWCHAR");
      break;
    case AST_INDEX:
      fprintf(stderr, "INDEX");
      break;
    case AST_LITERAL:
      fprintf(stderr, "LITERAL");
      break;
    case AST_VECVALUE:
      fprintf(stderr, "VECVALUE");
      break;
    case AST_FUNCDECL:
      fprintf(stderr, "FUNCDECL");
      break;
    case AST_PARAM:
      fprintf(stderr, "PARAM");
      break;
    case AST_CMDBLOCK:
      fprintf(stderr, "CMDBLOCK");
      break;
    case AST_CMDLIST:
      fprintf(stderr, "CMDLIST");
      break;
    case AST_CMD:
      fprintf(stderr, "CMD");
      break;
    case AST_ATTRIB:
      fprintf(stderr, "ATTRIB");
      break;
    case AST_ATTRIBVEC:
      fprintf(stderr, "ATTRIBVEC");
      break;
    case AST_READ:
      fprintf(stderr, "READ");
      break;
    case AST_READVEC:
      fprintf(stderr, "READVEC");
      break;
    case AST_PRINT:
      fprintf(stderr, "PRINT");
      break;
    case AST_PRINTSTRING:
      fprintf(stderr, "PRINTSTRING");
      break;
    case AST_PRINTEXPR:
      fprintf(stderr, "PRINTEXPR");
      break;
    case AST_RETURN:
      fprintf(stderr, "RETURN");
      break;
    case AST_IDVAR:
      fprintf(stderr, "IDVAR");
      break;
    case AST_IDVEC:
      fprintf(stderr, "IDVEC");
      break;
    case AST_ADD:
      fprintf(stderr, "ADD");
      break;
    case AST_SUB:
      fprintf(stderr, "SUB");
      break;
    case AST_MUL:
      fprintf(stderr, "MUL");
      break;
    case AST_DIV:
      fprintf(stderr, "DIV");
      break;
    case AST_LT:
      fprintf(stderr, "LT");
      break;
    case AST_GT:
      fprintf(stderr, "GT");
      break;
    case AST_LE:
      fprintf(stderr, "LE");
      break;
    case AST_GE:
      fprintf(stderr, "GE");
      break;
    case AST_EQ:
      fprintf(stderr, "EQ");
      break;
    case AST_DIF:
      fprintf(stderr, "DIF");
      break;
    case AST_AND:
      fprintf(stderr, "AND");
      break;
    case AST_OR:
      fprintf(stderr, "OR");
      break;
    case AST_NEG:
      fprintf(stderr, "NEG");
      break;
    case AST_FUNCCALL:
      fprintf(stderr, "FUNCCALL");
      break;
    case AST_ARGLIST:
      fprintf(stderr, "ARGLIST");
      break;
    case AST_IF:
      fprintf(stderr, "IF");
      break;
    case AST_IFELSE:
      fprintf(stderr, "IFELSE");
      break;
    case AST_WHILE:
      fprintf(stderr, "WHILE");
      break;
    default:
      fprintf(stderr, "PRINT ERROR!"); 
      exit(PRINT_ERROR);
      break;
  }

  if (node->symbol != 0)
    fprintf(stderr, " %s\n", node->symbol->text);
  else
    fprintf(stderr, "\n");
  
  for (i=0; i< MAX_SONS; ++i)
    astPrint(node->son[i], level+1);
}

// END OF FILE
