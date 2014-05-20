
#include <stdlib.h>

#include "slugmem.h"

int main()
{
	int* mem[5];
	int i;
	for(i = 0; i < 5; ++i)
	{
		mem[i] = malloc(sizeof(int));
		*mem[i] = i;
	}

	for(i = 0; i < 5; ++i)
	{
		free(mem[i]);
	}
	return 0;

}