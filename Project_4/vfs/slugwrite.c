/* This file is the counterpart of "read.c".  It contains the code for writing
 * insofar as this is not contained in read_write().
 *
 * The entry points into this file are
 *   do_write:     call read_write to perform the WRITE system call
 */

#include "fs.h"
#include "file.h"


/*===========================================================================*
 *				do_slugwrite				     *
 *===========================================================================*/
int do_slugwrite()
{
/* Perform the write(fd, buffer, nbytes) system call. */
  printf("do_slugwrite\n");
  return(do_slug_read_write(WRITING));
}
