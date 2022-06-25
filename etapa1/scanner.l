%{
    #include "tokens.h"
    #include "lib/include/tools.h"
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

[a-zA-Z_]+                  { return TK_IDENTIFIER; }   // hash_insert(TK_IDENTIFIER, yytext);

[0-9]+                      { return LIT_INTEGER; }     // hash_insert(LIT_INTEGER, yytext);
[0-9]+,[0-9]+               { return LIT_FLOAT; }       // hash_insert(LIT_FLOAT, yytext);
\'([^"\\]|\\.)?\'           { return LIT_CHAR; }        // hash_insert(LIT_CHAR, yytext);
\"([^"\\]|\\.)*\"           { return LIT_STRING; }      // hash_insert(LIT_STRING, yytext);

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
    fprintf(stderr, "Number of lines in the document: %d\n", lineNumber);
    running = 0;
    return 1;
}
