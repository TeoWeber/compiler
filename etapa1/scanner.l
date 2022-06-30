%{
    // scanner.l - Compiladores 2022/1
    // Astélio José Weber Júnior - 283864
    // Wellington Nascente Hirsch - 287715

    #include "tools.h"
    #include "hash.h"
    #include "tokens.h"
%}

%x COMMENT


%%


char                        { return KW_CHAR; }
int                         { return KW_INT; }
float                       { return KW_FLOAT; }
if                          { return KW_IF; }
else                        { return KW_ELSE; }
while                       { return KW_WHILE; }
read                        { return KW_READ; }
print                       { return KW_PRINT; }
return                      { return KW_RETURN; }

"<-"                        { return ASSIGNMENT; }
"<="                        { return OPERATOR_LE; }
">="                        { return OPERATOR_GE; }
"=="                        { return OPERATOR_EQ; }
"!="                        { return OPERATOR_DIF; }

[a-zA-Z_]+                  { hashInsert(TK_IDENTIFIER, yytext); return TK_IDENTIFIER; }

[0-9]+                      { hashInsert(LIT_INTEGER, yytext); return LIT_INTEGER; }
[0-9]+,[0-9]+               { hashInsert(LIT_FLOAT, yytext); return LIT_FLOAT; }
\'([^"\\]|\\.)?\'           { hashInsert(LIT_CHAR, yytext); return LIT_CHAR; }
\"([^"\\]|\\.)*\"           { hashInsert(LIT_STRING, yytext); return LIT_STRING; }

[;\(\)\[\]{}\+\-\*./<>&|~]  { return yytext[0]; }

[ \t]
[\n]                        { ++ lineNumber; }
"##".*
"\\#"                       { BEGIN(COMMENT); }
.                           { return TOKEN_ERROR; }

<COMMENT>"#\\"              { BEGIN(INITIAL); }
<COMMENT>[\n]               { ++ lineNumber; }
<COMMENT>.


%%


int yywrap()
{
    fprintf(stderr, "Number of lines in the document: %d\n\n\n", lineNumber);
    running = 0;
    return 1;
}
