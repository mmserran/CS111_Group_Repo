#include "fs.h"
#include <assert.h>
#include <minix/vfsif.h>
#include <minix/bdev.h>
#include "inode.h"

#include "clean.h"
/*===========================================================================*
 *        fs_slugread             *
 *===========================================================================*/
int fs_slugread()
{
/* Perform the sync() system call.  Flush all the tables. 
 * The order in which the various tables are flushed is critical.  The
 * blocks must be flushed last, since rw_inode() leaves its results in
 * the block cache.
 */
  printf("fs_slugread called\n");
  return(OK);   /* sync() can't fail */
}
