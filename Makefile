
BINARIES = perfect notallocated doublefree missedfree largeinput notfreed
SLUGMEM  = slugmem.c
CC       = cc


all: ${BINARIES}

${BINARIES}:
	${CC} -g ${SLUGMEM} $@.c -o $@ -lm

clean:
	rm -rf ${BINARIES}