#include <unistd.h> 
#include <stdio.h> 
 
int main(int argc, char** args)
{ 
	pid_t id = fork(); 
	printf("Process ID: %d\n", id); 
 
 return 0; 
} 