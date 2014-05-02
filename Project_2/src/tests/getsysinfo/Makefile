CC=cc
CFLAGS=-I/usr/src/servers -I/usr/src

getsysinfo: getsysinfo.c
	$(CC) $(CFLAGS) -o $@ $<

test:
	./getsysinfo

.PHONY: test
