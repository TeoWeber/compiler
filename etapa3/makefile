# makefile - Compiladores 2022/1
# Astélio José Weber Júnior - 283864
# Wellington Nascente Hirsch - 287715

run: etapa3
	./etapa3 source.txt eq1.txt
	./etapa3 eq1.txt eq2.txt
	diff eq1.txt eq2.txt

etapa3: y.tab.o lex.yy.o main.o tools.o hash.o ast.o
	gcc -o etapa3 obj/ast.o obj/hash.o obj/tools.o obj/y.tab.o obj/lex.yy.o obj/main.o -ll

main.o: main.c obj/
	gcc -c main.c -o obj/main.o

lex.yy.o: lex.yy.c obj/
	gcc -c lex.yy.c -o obj/lex.yy.o

lex.yy.c: scanner.l
	lex --header-file=lex.yy.h scanner.l

y.tab.o: y.tab.c obj/
	gcc -c y.tab.c -o obj/y.tab.o

y.tab.c: parser.y
	yacc parser.y -d

tools.o: tools.c obj/
	gcc -c tools.c -o obj/tools.o

ast.o: ast.c obj/
	gcc -c ast.c -o obj/ast.o

hash.o: hash.c obj/
	gcc -c hash.c -o obj/hash.o

obj/:
	mkdir obj


clean:
	rm -rf obj/ y.tab.h y.tab.c lex.yy.h lex.yy.c eq1.txt eq2.txt etapa3