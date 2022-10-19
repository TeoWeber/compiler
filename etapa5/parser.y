// parser.y - Compiladores 2022/1
// Astélio José Weber Júnior - 283864
// Wellington Nascente Hirsch - 287715

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "ast.h"
    #include "evaluate.h"
    #include "code.h"

    int yylex();
    int yyerror();
    extern int getLineNumber(void);

    void root(AST* node){
        astPrint(node, 0);
        hashPrint();
        evaluateDeclared(node);
        evaluateFunctions(node);
        tacPrintBackwards(generateCode(node));
    }
%}

%union
{
    HASH_NODE *symbol;
    AST *ast;
}

%token KW_CHAR      
%token KW_INT       
%token KW_FLOAT     

%token KW_IF        
%token KW_ELSE      
%token KW_WHILE     
%token KW_READ      
%token KW_PRINT     
%token KW_RETURN    

%token ASSIGNMENT
%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_DIF

%token<symbol> TK_IDENTIFIER

%token<symbol> LIT_INTEGER  
%token<symbol> LIT_FLOAT    
%token<symbol> LIT_CHAR     
%token<symbol> LIT_STRING   

%token TOKEN_ERROR  

%type<ast> program
%type<ast> decl
%type<ast> varDecl
%type<ast> type
%type<ast> index
%type<ast> literal
%type<ast> vecInitValue
%type<ast> funcDecl
%type<ast> paramList
%type<ast> cmdBlock
%type<ast> cmdList
%type<ast> cmd
%type<ast> attrib
%type<ast> read
%type<ast> print
%type<ast> printList
%type<ast> return
%type<ast> expr
%type<ast> funcCall
%type<ast> argList
%type<ast> flow

%left '|' '&'
%left '<' '>' OPERATOR_EQ OPERATOR_LE OPERATOR_GE OPERATOR_DIF
%left '+' '-'
%left '.' '/'
%right '~'

%right "flow" KW_ELSE
%right "identifier" '('


%%


program: decl                                                   { root($1);  astCompiled = $1; }
    ;

decl: varDecl decl                                              { $$ = astCreate(AST_VAR, 0, $1, $2, 0, 0, getLineNumber()); }
    | funcDecl decl                                             { $$ = astCreate(AST_FUNC, 0, $1, $2, 0, 0, getLineNumber()); }
    |                                                           { $$ = 0; }
    ;

varDecl: type TK_IDENTIFIER '(' literal ')' ';'                 { $$ = astCreate(AST_VARDECL, $2, $1, $4, 0, 0, getLineNumber()); }
    | type TK_IDENTIFIER '[' index ']' vecInitValue ';'         { $$ = astCreate(AST_VECDECL, $2, $1, $4, $6, 0, getLineNumber()); }
    ;

type: KW_INT                                                    { $$ = astCreate(AST_KWINT, 0, 0, 0, 0, 0, getLineNumber()); }
    | KW_FLOAT                                                  { $$ = astCreate(AST_KWFLOAT, 0, 0, 0, 0, 0, getLineNumber()); }
    | KW_CHAR                                                   { $$ = astCreate(AST_KWCHAR, 0, 0, 0, 0, 0, getLineNumber()); }
    ;

index: LIT_INTEGER                                              { $$ = astCreate(AST_INDEX, $1, 0, 0, 0, 0, getLineNumber()); }
    ;

literal: LIT_INTEGER                                            { $$ = astCreate(AST_LITERAL, $1, 0, 0, 0, 0, getLineNumber()); }
    | LIT_FLOAT                                                 { $$ = astCreate(AST_LITERAL, $1, 0, 0, 0, 0, getLineNumber()); }
    | LIT_CHAR                                                  { $$ = astCreate(AST_LITERAL, $1, 0, 0, 0, 0, getLineNumber()); }
    ;

vecInitValue: literal vecInitValue                              { $$ = astCreate(AST_VECVALUE, 0, $1, $2, 0, 0, getLineNumber()); }
    |                                                           { $$ = 0; }
    ;

funcDecl: type TK_IDENTIFIER '(' paramList ')' cmdBlock         { $$ = astCreate(AST_FUNCDECL, $2, $1, $4, $6, 0, getLineNumber()); }
    ;

paramList: type TK_IDENTIFIER paramList                         { $$ = astCreate(AST_PARAM, $2, $1, $3, 0, 0, getLineNumber()); }
    |                                                           { $$ = 0; }
    ;

cmdBlock: '{' cmdList '}'                                       { $$ = astCreate(AST_CMDBLOCK, 0, $2, 0, 0, 0, getLineNumber()); }
    ;

cmdList: cmd ';' cmdList                                        { $$ = astCreate(AST_CMDLIST, 0, $1, $3, 0, 0, getLineNumber()); }
    | cmd                                                       { $$ = astCreate(AST_CMD, 0, $1, 0, 0, 0, getLineNumber()); }
    ;

cmd: read                                                       { $$ = $1; }
    | print                                                     { $$ = $1; }
    | return                                                    { $$ = $1; }
    | attrib                                                    { $$ = $1; }
    | flow                                                      { $$ = $1; }
    | cmdBlock                                                  { $$ = $1; }
    |                                                           { $$ = 0; }
    ;

attrib: TK_IDENTIFIER ASSIGNMENT expr                           { $$ = astCreate(AST_ATTRIB, $1, $3, 0, 0, 0, getLineNumber()); }
    | TK_IDENTIFIER '[' expr ']' ASSIGNMENT expr                { $$ = astCreate(AST_ATTRIBVEC, $1, $3, $6, 0, 0, getLineNumber()); }
    ;

read: KW_READ TK_IDENTIFIER                                     { $$ = astCreate(AST_READ, $2, 0, 0, 0, 0, getLineNumber()); }
    | KW_READ TK_IDENTIFIER '[' expr ']'                        { $$ = astCreate(AST_READVEC, $2, $4, 0, 0, 0, getLineNumber()); }
    ;

print: KW_PRINT printList                                       { $$ = astCreate(AST_PRINT, 0, $2, 0, 0, 0, getLineNumber()); }
    ;

printList: LIT_STRING printList                                 { $$ = astCreate(AST_PRINTSTRING, $1, $2, 0, 0, 0, getLineNumber()); }
    | expr printList                                            { $$ = astCreate(AST_PRINTEXPR, 0, $1, $2, 0, 0, getLineNumber()); }
    |                                                           { $$ = 0; }
    ;

return: KW_RETURN expr                                          { $$ = astCreate(AST_RETURN, 0, $2, 0, 0, 0, getLineNumber()); }
    ;

expr: TK_IDENTIFIER                         %prec "identifier"  { $$ = astCreate(AST_IDVAR, $1, 0, 0, 0, 0, getLineNumber()); }
    | TK_IDENTIFIER '[' expr ']'                                { $$ = astCreate(AST_IDVEC, $1, $3, 0, 0, 0, getLineNumber()); }
    | literal                                                   { $$ = $1; }
    | '(' expr ')'                                              { $$ = $2; }
    | expr '+' expr                                             { $$ = astCreate(AST_ADD, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '-' expr                                             { $$ = astCreate(AST_SUB, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '.' expr                                             { $$ = astCreate(AST_MUL, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '/' expr                                             { $$ = astCreate(AST_DIV, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '<' expr                                             { $$ = astCreate(AST_LT, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '>' expr                                             { $$ = astCreate(AST_GT, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr OPERATOR_LE expr                                     { $$ = astCreate(AST_LE, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr OPERATOR_GE expr                                     { $$ = astCreate(AST_GE, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr OPERATOR_EQ expr                                     { $$ = astCreate(AST_EQ, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr OPERATOR_DIF expr                                    { $$ = astCreate(AST_DIF, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '&' expr                                             { $$ = astCreate(AST_AND, 0, $1, $3, 0, 0, getLineNumber()); }
    | expr '|' expr                                             { $$ = astCreate(AST_OR, 0, $1, $3, 0, 0, getLineNumber()); }
    | '~' expr                                                  { $$ = astCreate(AST_NEG, 0, $2, 0, 0, 0, getLineNumber()); }
    | funcCall                                                  { $$ = $1; }
    ;

funcCall: TK_IDENTIFIER '(' argList ')'                         { $$ = astCreate(AST_FUNCCALL, $1, $3, 0, 0, 0, getLineNumber()); }
    ;

argList: expr argList                                           { $$ = astCreate(AST_ARGLIST, 0, $1, $2, 0, 0, getLineNumber()); }
    |                                                           { $$ = 0; }
    ;

flow: KW_IF '(' expr ')' cmd                %prec "flow"        { $$ = astCreate(AST_IF, 0, $3, $5, 0, 0, getLineNumber()); }
    | KW_IF '(' expr ')' cmd KW_ELSE cmd                        { $$ = astCreate(AST_IFELSE, 0, $3, $5, $7, 0, getLineNumber()); }
    | KW_WHILE '(' expr ')' cmd                                 { $$ = astCreate(AST_WHILE, 0, $3, $5, 0, 0, getLineNumber()); }
    ;


%%

int yyerror ()
{
  fprintf(stderr,"Syntax error at line %d.\n", getLineNumber());
  exit(3);
}
