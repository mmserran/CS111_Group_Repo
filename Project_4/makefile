BINARIES = compatibility_test
CC       = clang


all: ${BINARIES}

${BINARIES}:
	${CC} -g tests/$@.c -o $@ -lm

clean:
	rm -rf ${BINARIES}