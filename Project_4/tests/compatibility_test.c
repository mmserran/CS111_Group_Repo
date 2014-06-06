#include <sys/cdefs.h>
#include <lib.h>

#include <unistd.h>
#include <stdio.h> 
 
 #define BUFFER_LEN 400

int main(int argc, char** args)
{ 

  message m;
  
  _syscall(VFS_PROC_NR, SLUGREAD, &m);
 return 0; 
} 

