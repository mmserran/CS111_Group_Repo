#include <stdlib.h>

#include "slugmem.h"

int main()
{
	int i = 4;
	free(&i);
}