# Starter Makefile for prog1
CC	= cc -ansi
PROG	= shell

${PROG}:	shell.o lex.yy.o
	${CC} -o ${PROG} shell.o lex.yy.o

shell.o:	shell.c
	${CC} -c shell.c

lex.yy.o:	lex.yy.c
	${CC} -c lex.yy.c

lex.yy.c:	lex.c
	flex lex.c

clean:		${PROG}
	rm lex.yy.o shell.o lex.yy.c ${PROG}
