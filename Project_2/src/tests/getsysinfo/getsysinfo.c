#include <stdio.h>
#include <lib.h>
#include <unistd.h>

#include <minix/type.h>
#include <machine/archtypes.h>
#include "pm/mproc.h"
#include "kernel/proc.h"

struct mproc mproc_table[NR_PROCS];
struct proc proc_table[NR_PROCS];

int main(int argc, char *argv[]) {

	int i, s;
	struct kinfo kinfo;

	printf("This utility shows process information.\n");
	fflush(stdout);

	if((s = getsysinfo(PM_PROC_NR, SI_KINFO, &kinfo)) != 0) {
		printf("ERROR calling getsysinfo SI_KINFO\n");
		return -1;
	}
	fflush(stdout);
	printf("nr_procs is %d\n", kinfo.nr_procs);

	if(getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc_table) < 0) {
		printf("ERROR calling getsysinfo SI_PROC_TAB\n");
		return -2;
	}

	if(getsysinfo(PM_PROC_NR, SI_KPROC_TAB, proc_table) < 0) {
		printf("ERROR calling getsysinfo SI_KPROC_TAB\n");
		return -3;
	}
	for(i = 0; i < 20; i++) {
		printf("%d - %x %x %x\n", i, mproc_table[i].mp_pid, proc_table[i].p_priority, proc_table[i].p_scheduler);
	}

	return 0;
}

