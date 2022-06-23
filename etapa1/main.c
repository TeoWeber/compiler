int main (int argc, char **argv) {
    int tok;

    if(argc < 2){
        fprintf(stderr, "Call: etapa input_file_name\n");
        exit(1);
    }

    if((yyin = fopen(argv[1], "r")) == 0 ){
        fprintf(stderr, "File %s not found!\n", argv[1]);
        exit(2);
    }

    while(running){
        tok = yylex();
        if(running == 0)
            break;
        
        switch(tok) {
            case 1:
                fprintf(stderr, "case 1\n");
                break;
            case 2:
                fprintf(stderr, "case 2\n");
                break;
            case 3:
                fprintf(stderr, "case 3\n");
                break;
            case 4:
                fprintf(stderr, "case 4\n");
                break;
            case 5:
                fprintf(stderr, "case 5\n");
                break;
            case 6:
                fprintf(stderr, "case 6\n");
                break;
            default:
                fprintf(stderr, "ERROR: invalid token!\n");
        }
    }
    exit(0);
}