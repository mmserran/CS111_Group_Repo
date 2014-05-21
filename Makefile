
SOURCES = perfect.c notallocated.c doublefree.c missedfree.c
BINARIES = perfect notallocated doublefree missedfree
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


clean:
	rm ${BINARIES}
