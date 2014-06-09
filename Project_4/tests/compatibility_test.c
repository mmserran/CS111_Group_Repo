
#include <slug_library.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h> 

 #define BUFFER_LEN 400

int main(int argc, char** args)
{ 
	FILE* read_file; 
	int fd;
	read_file = fopen("tests/testread", "r");
	if(read_file == NULL)
	{
		printf("Failed to open file 'testread'.  Exiting\n");
		return 0;
	} else {
		fd = fileno(read_file);
 		char buffer[BUFFER_LEN + 1];
		slugread(fd, buffer, 120); 
		printf("%s\n", buffer);
	}
 
 return 0; 
} 