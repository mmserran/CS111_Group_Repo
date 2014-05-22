
BINARIES = perfect notallocated doublefree missedfree largeinput
SLUGMEM  = slugmem.c
CC       = cc


all: ${BINARIES}

${BINARIES}:
	${CC} -g ${SLUGMEM} $@.c -o $@

clean:
	rm -rf ${BINARIES}