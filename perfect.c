
#include <stdlib.h>

#include "slugmem.h"

int main()
{
	int size = 10;
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
	return 0;

}