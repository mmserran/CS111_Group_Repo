#include <stdlib.h>

#include "slugmem.h"

int main()
{
	int* p = malloc(sizeof(int));
	free(p);
	int i = 4;
	free(&i);
}