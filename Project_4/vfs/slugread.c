/* This file contains the heart of the mechanism used to read (and write)
 * files.  Read and write requests are split up into chunks that do not cross
 * block boundaries.  Each chunk is then processed in turn.  Reads on special
 * files are also detected and handled.
 *
 * The entry points into this file are
 *   do_read:	 perform the READ system call by calling read_write
 *   do_getdents: read entries from a directory (GETDENTS)
 *   read_write: actually do the work of READ and WRITE
 *
 */

#include "fs.h"
#include <fcntl.h>
#include <unistd.h>
#include <minix/com.h>
#include <minix/u64.h>
#include "file.h"
#include "fproc.h"
#include "scratchpad.h"
#include "param.h"
#include <dirent.h>
#include <assert.h>
#include <minix/vfsif.h>
#include "vnode.h"
#include "vmnt.h"


/*===========================================================================*
 *				do_read					     *
 *===========================================================================*/
int do_slugread()
{
  printf("please work, do_slugread");
  return(do_read_write(READING));
}

