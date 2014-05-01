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


#
# Test functions.
#
# ``make test4''.. performs a test to verify Req. 4
test4:
	clear
	time sleep 11 &
	@echo "  Process 1 of 3 started: ''time sleep 11 &''"
	ps
	sleep 3
	clear
	time sleep 12 &
	@echo "  Process 2 of 3 started: ''time sleep 12 &''"
	ps
	sleep 3
	clear
	time sleep 13 &
	@echo "  Process 3 of 3 started: ''time sleep 13 &''"
	ps
	sleep 3
	
	clear
	@echo "Now waiting"
	@echo "  Observe below.."
	ps
	sleep 4
	clear
	@echo "Now waiting"
	@echo "  Observe below.."
	ps
	sleep 4
	clear
	@echo "Now waiting"
	@echo "  Observe below.."
	ps
	sleep 4
	clear
	@echo "Test Complete"
	@echo "  Verify no <defunct> processes exist via ''ps''"
	ps
