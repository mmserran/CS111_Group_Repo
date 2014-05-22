 
#include <stdlib.h>

#include "slugmem.h"

int main()
{
	int size = 5;
	int* mem[size];
	int i;
	for(i = 0; i < size; ++i)
	{
		mem[i] = malloc(sizeof(int));
		*mem[i] = i;
	}

	for(i = 0; i < size; ++i)
	{
		free(mem[i]);
	}

	void* big = malloc(129000000);
	return 0;

}