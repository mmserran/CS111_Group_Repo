#include <sys/cdefs.h>
#include <lib.h>
#include "namespace.h"



#include <unistd.h>

ssize_t slugread(int fd, void *buffer, size_t nbytes)
{
  message m;

  m.m1_i1 = fd;
  m.m1_i2 = nbytes;
  m.m1_p1 = (char *) buffer;
  //return(_syscall(VFS_PROC_NR, 69, &m));
  return(_syscall(VFS_PROC_NR, SLUGREAD, &m));
}
