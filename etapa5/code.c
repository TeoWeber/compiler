// code.c - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715
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
TAC *makeOperation(int type, TAC *code0, TAC *code1);
TAC *makeIfThen(TAC *code0, TAC *code1);
TAC *makeIfElse(TAC *code0, TAC *code1, TAC *code2);
TAC *makeWhile(TAC *code0, TAC *code1);

// CODE PRINT
void tacPrint(TAC *tac) {
  if (!tac)
    return;

  // Comente para exibir os símbolos
  if (tac->type == TAC_SYMBOL) {
    return;
  }
  
  fprintf(stderr, "TAC(");
  switch(tac->type)
  {
    case TAC_SYMBOL:    fprintf(stderr, "TAC_SYMBOL"); break;
    case TAC_ADD:       fprintf(stderr, "TAC_ADD"); break;
    case TAC_SUB:       fprintf(stderr, "TAC_SUB"); break;
    case TAC_MUL:       fprintf(stderr, "TAC_MUL"); break;
    case TAC_DIV:       fprintf(stderr, "TAC_DIV"); break;
    case TAC_LT:        fprintf(stderr, "TAC_LT"); break;
    case TAC_GT:        fprintf(stderr, "TAC_GT"); break;
    case TAC_LE:        fprintf(stderr, "TAC_LE"); break;
    case TAC_GE:        fprintf(stderr, "TAC_GE"); break;
    case TAC_EQ:        fprintf(stderr, "TAC_EQ"); break;
    case TAC_DIF:       fprintf(stderr, "TAC_DIF"); break;
    case TAC_AND:       fprintf(stderr, "TAC_AND"); break;
    case TAC_OR:        fprintf(stderr, "TAC_OR"); break;
    case TAC_NEG:       fprintf(stderr, "TAC_NEG"); break;
    case TAC_COPY:      fprintf(stderr, "TAC_COPY"); break;
    case TAC_COPYVEC:   fprintf(stderr, "TAC_COPYVEC"); break;
    case TAC_JFALSE:    fprintf(stderr, "TAC_JFALSE"); break;
    case TAC_LABEL:     fprintf(stderr, "TAC_LABEL"); break;
    case TAC_VECDECL:   fprintf(stderr, "TAC_VECDECL"); break;
    case TAC_READ:      fprintf(stderr, "TAC_READ"); break;
    case TAC_READVEC:   fprintf(stderr, "TAC_READVEC"); break;
    case TAC_JUMP:      fprintf(stderr, "TAC_JUMP"); break;
    case TAC_PRINT:     fprintf(stderr, "TAC_PRINT"); break;
    case TAC_RETURN:    fprintf(stderr, "TAC_RETURN"); break;
    case TAC_BEGINFUN:  fprintf(stderr, "TAC_BEGINFUN"); break;
    case TAC_ENDFUN:    fprintf(stderr, "TAC_ENDFUN"); break;
    case TAC_CALL:      fprintf(stderr, "TAC_CALL"); break;
    case TAC_PARAM:     fprintf(stderr, "TAC_PARAM"); break;
    case TAC_ARG:       fprintf(stderr, "TAC_ARG"); break;
    case TAC_VECGET:    fprintf(stderr, "TAC_VECGET"); break;
    default:            fprintf(stderr, "TAC_UNKNOWN"); break;
  }

  fprintf(stderr, ", %s", (tac->res)?tac->res->text:"0");
  fprintf(stderr, ", %s", (tac->op1)?tac->op1->text:"0");
  fprintf(stderr, ", %s", (tac->op2)?tac->op2->text:"0");
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
  if (!node)
    return 0;

  TAC *result = 0;
  TAC *code[MAX_SONS];
  int i;

  // PROCESS CHILDREN
  for (i=0; i<MAX_SONS; ++i)
    code[i] = generateCode(node->son[i]);
  
  switch(node->type)
  {
    // VARIABLES
    case AST_VARDECL:
      result = tacJoin(
        code[1], // Executa os comandos que possam ter no operador a ser atribuido
        tacCreate(TAC_COPY, node->symbol, code[1] ? code[1]->res : 0, 0) // Atribui
      ); break;
    case AST_ATTRIB:
      result = tacJoin(
        code[0], // Executa os comandos que possam ter no operador a ser atribuido
        tacCreate(TAC_COPY, node->symbol, code[0] ? code[0]->res : 0, 0) // Atribui
      ); break;

    // SYMBOL
    case AST_IDVAR:
    case AST_INDEX:
    case AST_LITERAL: result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0); break;

    // VECTOR
    case AST_VECVALUE: result = tacJoin(code[0], code[1]); break; // Apenas concatena os literais
    case AST_VECDECL:
      result = tacJoin(
        code[2], // Prepara todos os literais a serem atribuidos
        tacCreate(TAC_VECDECL, node->symbol, code[1] ? code[1]->res : 0, 0) // Atribui
      ); break;
    case AST_IDVEC:
      result = tacJoin(
        code[0], // Executa os possíveis comandos do índice a ser acessado
        tacCreate(TAC_VECGET, makeTemp(), node->symbol, code[0] ? code[0]->res : 0) // Acessa e armazena em um temporário
        ); break;
    case AST_ATTRIBVEC:
      result = tacJoin(
        tacJoin(code[0], code[1]), // Executa os possíveis comandos do índice a ser acessado e do valor a ser atribuido
        tacCreate(TAC_COPYVEC, node->symbol, code[0] ? code[0]->res : 0, code[1] ? code[1]->res : 0) // Atribui
      ); break;

    // READ
    case AST_READ: result = tacCreate(TAC_READ, node->symbol, 0, 0); break;
    case AST_READVEC:
      result = tacJoin(
        code[0], // Executa os possíveis comandos do índice a ser acessado
        tacCreate(TAC_READVEC, node->symbol, code[0] ? code[0]->res : 0, 0)
        ); break;

    // BASIC OPERATIONS
    case AST_ADD: result = makeOperation(TAC_ADD, code[0], code[1]); break;
    case AST_SUB: result = makeOperation(TAC_SUB, code[0], code[1]); break;
    case AST_MUL: result = makeOperation(TAC_MUL, code[0], code[1]); break;
    case AST_DIV: result = makeOperation(TAC_DIV, code[0], code[1]); break;
    case AST_LT:  result = makeOperation(TAC_LT, code[0], code[1]); break;
    case AST_GT:  result = makeOperation(TAC_GT, code[0], code[1]); break;
    case AST_LE:  result = makeOperation(TAC_LE, code[0], code[1]); break;
    case AST_GE:  result = makeOperation(TAC_GE, code[0], code[1]); break;
    case AST_EQ:  result = makeOperation(TAC_EQ, code[0], code[1]); break;
    case AST_DIF: result = makeOperation(TAC_DIF, code[0], code[1]); break;
    case AST_AND: result = makeOperation(TAC_AND, code[0], code[1]); break;
    case AST_OR:  result = makeOperation(TAC_OR, code[0], code[1]); break;
    case AST_NEG: result = makeOperation(TAC_NEG, code[0], 0); break;

    // FLOW
    case AST_IF: result = makeIfThen(code[0], code[1]); break;
    case AST_IFELSE: result = makeIfElse(code[0], code[1], code[2]); break;
    case AST_WHILE: result = makeWhile(code[0], code[1]); break;

    // PRINT
    case AST_PRINT: result = code[0]; break;
    case AST_PRINTEXPR:
      result = tacJoin( // Executa o print do resultado do primeiro valor
        tacJoin(code[0], tacCreate(TAC_PRINT, code[0] ? code[0]->res : 0, 0, 0)),
        code[1] // Opera o print do seguinte
      ); break;
    case AST_PRINTSTRING:
      result = tacJoin(
        tacJoin( // Cria um simbolo do tipo string e executa o seu print
          tacCreate(TAC_SYMBOL, node->symbol, 0, 0), 
          tacCreate(TAC_PRINT, node->symbol, 0, 0)
        ), 
        code[0] // Opera o print do seguinte
      ); break;

    // FUNCTION
    case AST_FUNCDECL:
      result = tacJoin(
        tacJoin(
          tacJoin(
            tacCreate(TAC_BEGINFUN, node->symbol, 0, 0), // Inicializa o bloco da função com o nome
            code[1] // Registra os parâmetros
          ), 
          code[2] // Executa os comandos
        ), 
        tacCreate(TAC_ENDFUN, node->symbol, 0, 0) // Encerra o bloco da função
      ); break;
    case AST_PARAM:
      result = tacJoin(tacCreate(TAC_PARAM, node->symbol, 0, 0), code[1]); break;
    case AST_RETURN:
      result = tacJoin(code[0], tacCreate(TAC_RETURN, code[0] ? code[0]->res : 0, 0, 0)); break;
    case AST_FUNCCALL:
      result = tacJoin(
        code[0], // Registra os parametros da função
        tacCreate(TAC_CALL, makeTemp(), node->symbol, 0) // Chama a função declarando um temporário para seu retorno
      ); break;
    case AST_ARGLIST:
      result = tacJoin( // Executa os possíveis comandos e depois registra o parâmetro
        tacJoin(code[0], tacCreate(TAC_ARG, code[0] ? code[0]->res : 0, 0, 0)), 
        code[1] // Registra os demais parâmetros
      ); break;

    default:
      result = tacJoin(code[0], tacJoin(code[1], tacJoin(code[2], code[3])));
      break;
  }

  return result;
}

/*
O resultado esperado é:
- code0 (executar todos os comandos para obter o primeiro operador da expressão)
- code1 (executar todos os comandos para obter o segundo operador da expressão)
- tacCreate(type) (executa o comando usando os resultados e armazena em um temporário)
*/
TAC *makeOperation(int type, TAC *code0, TAC *code1) {
  return tacJoin(
    tacJoin(code0, code1),
    tacCreate(type, makeTemp(), code0 ? code0->res : 0, code1 ? code1->res : 0)
  );
}

/*
Em um comando if-then:
if (bool) {
  cmd
}
...

O gerador faz o seguinte:
- bool (calcula a expressão boleana)
- jumpTac -> labelTac (verifica se é falso e se for pula para labelTac)
- cmd (comandos a serem executados)
- labelTac (label onde o jump ocorre caso bool seja falso)
- ... (os comandos após o if-then)
*/
TAC *makeIfThen(TAC *code0, TAC *code1) {
  TAC *jumpTac = 0;
  TAC *labelTac = 0;
  HASH_NODE *newLabel = makeLabel();

  jumpTac = tacCreate(TAC_JFALSE, newLabel, code0 ? code0->res : 0, 0);
  jumpTac->prev = code0;
  labelTac = tacCreate(TAC_LABEL, newLabel, 0, 0);
  labelTac->prev = code1;
  return tacJoin(jumpTac, labelTac);
}

/*
Em um comando if-else:
if (bool) {
  cmdif
} else {
  cmdelse
}
...

O gerador faz o seguinte:
- bool (calcula a expressão boleana)
- jumpElseTac -> labelElseTac (verifica se é falso e se for pula para labelElseTac para executar o cmdelse)
- cmdif (comandos a serem executados se bool for verdadeiro)
- jumpEndTac -> labelEndTac (pula para labelEndTac localizada após o bloco else para continuar em ...)
- labelElseTac (label onde o jump ocorre caso bool seja falso para executar cmdelse)
- cmdelse (comandos a serem executados se bool for falso)
- labelEndTac (label onde o jump ocorre após executar o cmdif caso o bool seja verdadeiro, para pular os comandos cmdelse)
- ... (os comandos após o if-else)
*/
TAC *makeIfElse(TAC *code0, TAC *code1, TAC *code2) {
  TAC *jumpElseTac = 0;
  TAC *jumpEndTac = 0;
  TAC *labelElseTac = 0;
  TAC *labelEndTac = 0;
  HASH_NODE *elseLabel = makeLabel();
  HASH_NODE *endLabel = makeLabel();

  jumpElseTac = tacCreate(TAC_JFALSE, elseLabel, code0 ? code0->res : 0, 0);
  jumpElseTac->prev = code0;
  jumpEndTac = tacCreate(TAC_JUMP, endLabel, 0, 0);
  jumpEndTac->prev = code1;
  labelElseTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);
  labelElseTac->prev = jumpEndTac;
  labelEndTac = tacCreate(TAC_LABEL, endLabel, 0, 0);
  labelEndTac->prev = code2;
  return tacJoin(tacJoin(jumpElseTac, labelElseTac), labelEndTac);
}

/*
Em um comando while:
while (bool) {
  cmd
}
...

O gerador faz o seguinte:
- labelStartTac (label inicial, antes mesmo da verificação bool para poder reiniciar o fluxo)
- bool (calcula a expressão boleana)
- jumpEndTac -> labelEndTac (verifica se é falso e se for termina o fluxo pulando para labelEndTac)
- cmd (comandos a serem executados se bool for verdadeiro)
- jumpStartTac -> labelStartTac (após todos os comandos, ele pula para a label inicial reiniciando o fluxo)
- labelEndTac (label onde o jump ocorre caso a restrição do fluxo não seja mais verdadeira)
- ... (os comandos após o while)
*/
TAC *makeWhile(TAC *code0, TAC *code1) {
  TAC *jumpStartTac = 0;
  TAC *jumpEndTac = 0;
  TAC *labelStartTac = 0;
  TAC *labelEndTac = 0;
  HASH_NODE *startLabel = makeLabel();
  HASH_NODE *endLabel = makeLabel();

  labelStartTac = tacCreate(TAC_LABEL, startLabel, 0, 0);
  jumpEndTac = tacCreate(TAC_JFALSE, endLabel, code0 ? code0->res : 0, 0);
  jumpEndTac->prev = code0;
  jumpStartTac = tacCreate(TAC_JUMP, startLabel, 0, 0);
  jumpStartTac->prev = code1;
  labelEndTac = tacCreate(TAC_LABEL, endLabel, 0, 0);
  labelEndTac->prev = jumpStartTac;
  return tacJoin(tacJoin(labelStartTac, jumpEndTac), labelEndTac);
}
