#include "unistd.h"
#include <stdio.h> 
 
 #define BUFFER_LEN 400

int main(int argc, char** args)
{ 
	FILE* read_file; 
	read_file = fopen("tests/testread", "r");
	if(read_file == NULL)
	{
		printf("Failed to open file 'testread'.  Exiting\n");
		return 0;
	} else {
		int fd = fileno(read_file);
 		char buffer[BUFFER_LEN + 1];
		slugread(fd, buffer, 400); 
		printf("slug_libread worked\n"); 
	}
 
 return 0; 
} 