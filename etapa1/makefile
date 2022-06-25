# Makefile - Compiladores 2022/1
# Astélio José Weber Júnior - 283864
# Wellington Nascente Hirsch - 287715

etapa1: lex.yy.o main.o tools.o hash.o
	gcc -o etapa1 obj/hash.o obj/tools.o obj/lex.yy.o obj/main.o -ll

main.o: main.c obj/
	gcc -c main.c -o obj/main.o

lex.yy.o: lex.yy.c obj/
	gcc -c lex.yy.c -o obj/lex.yy.o

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

tools.o: lib/tools.c obj/
	gcc -c lib/tools.c -o obj/tools.o

hash.o: lib/hash.c obj/
	gcc -c lib/hash.c -o obj/hash.o

obj/:
	mkdir obj
	
clean:
	rm -rf obj/ lex.yy.h lex.yy.c etapa1