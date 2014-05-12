# Mark, James C., Tyler, James K., Richard
# Makefile for CS111 - Project 1

TGZNAME  = Project2.tgz
USER     = root
REMOTE   = /asg2
LOCAL    = /Users/Mark/Dev/C++/minix318/Project_2

all:
	grep 'VER:' schedule.c

#
# Utility functions.
#
# ``make rdy''.. packs project into a .tgz file.
# 	we use ``tar -zxvf filename.tgz'' to unpack.
#   For my system (OSX), .DS_Store is created by finder
rdy:
	rm -f .DS_Store documentation/.DS_Store
	tar -cvzf ${TGZNAME} *

# ``make send''.. sends project code base to MINIX
send:
	scp -r -P 2222 * ${USER}@localhost:~${REMOTE}
	
# ``make receive''.. gets the .tgz file produced by ``make rdy'' into LOCAL
receive:
	scp -r -P 2222 ${USER}@localhost:~${REMOTE}/${TGZNAME} ${LOCAL}


