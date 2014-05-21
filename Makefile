
SOURCES = perfect.c notallocated.c doublefree.c missedfree.c largeinput.c
BINARIES = perfect notallocated doublefree missedfree largeinput
SLUGMEM = slugmem.c


all: ${BINARIES}

perfect: ${SLUGMEM} perfect.c
	gcc -g -o $@ $^

notallocated: ${SLUGMEM} notallocated.c
	gcc -g -o $@ $^

doublefree: ${SLUGMEM} doublefree.c
	gcc -g -o $@ $^

missedfree: ${SLUGMEM} missedfree.c
	gcc -g -o $@ $^

largeinput: ${SLUGMEM} largeinput.c
	gcc -g -o $@ $^

clean:
	rm ${BINARIES}
