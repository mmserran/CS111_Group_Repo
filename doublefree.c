
#include <stdlib.h>

#include "slugmem.h"

int main()
{
	int* p = malloc(sizeof(int));
	free(p);

	int* p2 = malloc(sizeof(int));
	free(p2);
	//int i = 4;
	//free(&i);
	free(p);
}