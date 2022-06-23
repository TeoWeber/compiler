%%


int         {fprintf(stderr,"KW INT\n");}
if          {fprintf(stderr,"KW IF\n");}
[a-zA-Z]+    {fprintf(stderr,"ID %s\n", yytext);}
[0-9]+      {fprintf(stderr,"NUMBER %s\n", yytext);}

[\n \t]     {}
.           {fprintf(stderr,"Drama...\n");}


%%


int main () {
    yylex();
}

int yywrap()
{
    return 1;
}
