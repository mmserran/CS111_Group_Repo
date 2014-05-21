#include <stdlib.h>

#include "slugmem.h"

int main()
{
	int* p = malloc(sizeof(int));
	free(p);
	int* i = malloc(sizeof(int) * 2);
	free((i+1));
}