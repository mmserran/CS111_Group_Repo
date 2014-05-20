
SOURCES = slugmem.c fucked.c perfect.c

build: ${SOURCES}
	gcc -g -c slugmem.c
	gcc -g -o perfect perfect.c
	gcc -g -o fucked fucked.c

debug: slugmem.c
	gcc -g -o slugmem slugmem.c

tests:	perfect.c fucked.c
	gcc -g -o perfect perfect.c
	gcc -g -o fucked fucked.c

clean:
	rm slugmem perfect fucked
