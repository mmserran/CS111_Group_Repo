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
 *				do_slugread					     *
 *===========================================================================*/
int do_slugread()
{
  printf("pls\n");
  return (do_slug_read_write(READING));
}

/*===========================================================================*
 *				do_slug_read_write				     *
 *===========================================================================*/
int do_slug_read_write(rw_flag)
int rw_flag;			/* READING or WRITING */
{
/* Perform read(fd, buffer, nbytes) or write(fd, buffer, nbytes) call. */
  struct filp *f;
  tll_access_t locktype;
  int r;

  scratch(fp).file.fd_nr = job_m_in.fd;
  scratch(fp).io.io_buffer = job_m_in.buffer;
  scratch(fp).io.io_nbytes = (size_t) job_m_in.nbytes;

  locktype = (rw_flag == READING) ? VNODE_READ : VNODE_WRITE;
  if ((f = get_filp(scratch(fp).file.fd_nr, locktype)) == NULL)
	return(err_code);
  if (((f->filp_mode) & (rw_flag == READING ? R_BIT : W_BIT)) == 0) {
	unlock_filp(f);
	return(f->filp_mode == FILP_CLOSED ? EIO : EBADF);
  }
  if (scratch(fp).io.io_nbytes == 0) {
	unlock_filp(f);
	return(0);	/* so char special files need not check for 0*/
  }

  r = slug_read_write(rw_flag, f, scratch(fp).io.io_buffer, scratch(fp).io.io_nbytes,
		 who_e);

  unlock_filp(f);
  return(r);
}

/*===========================================================================*
 *				slug_read_write				     *
 *===========================================================================*/
int slug_read_write(int rw_flag, struct filp *f, char *buf, size_t size,
		      endpoint_t for_e)
{
  register struct vnode *vp;
  u64_t position, res_pos, new_pos;
  unsigned int cum_io, cum_io_incr, res_cum_io;
  int op, oflags, r;

  position = f->filp_pos;
  oflags = f->filp_flags;
  vp = f->filp_vno;
  r = OK;
  cum_io = 0;

  if (size > SSIZE_MAX) return(EINVAL);

  if (S_ISFIFO(vp->v_mode)) {
	if (fp->fp_cum_io_partial != 0) {
		panic("VFS: slug_read_write: fp_cum_io_partial not clear");
	}
	r = rw_pipe(rw_flag, for_e, f, buf, size);
	return(r);
  }

  op = (rw_flag == READING ? VFS_DEV_READ : VFS_DEV_WRITE);

  if (S_ISCHR(vp->v_mode)) {	/* Character special files. */
	dev_t dev;
	int suspend_reopen;

	if (vp->v_sdev == NO_DEV)
		panic("VFS: slug_read_write tries to access char dev NO_DEV");

	suspend_reopen = (f->filp_state & FS_NEEDS_REOPEN);
	dev = (dev_t) vp->v_sdev;

	r = dev_io(op, dev, for_e, buf, position, size, oflags,
		   suspend_reopen);
	if (r >= 0) {
		cum_io = r;
		position = add64ul(position, r);
		r = OK;
	}
  } else if (S_ISBLK(vp->v_mode)) {	/* Block special files. */
	if (vp->v_sdev == NO_DEV)
		panic("VFS: slug_read_write tries to access block dev NO_DEV");

	lock_bsf();

	r = req_breadwrite(vp->v_bfs_e, for_e, vp->v_sdev, position, size,
			   buf, rw_flag, &res_pos, &res_cum_io);
	if (r == OK) {
		position = res_pos;
		cum_io += res_cum_io;
	}

	unlock_bsf();
  } else {				/* Regular files */
	if (rw_flag == WRITING) {
		/* Check for O_APPEND flag. */
		if (oflags & O_APPEND) position = cvul64(vp->v_size);
	}

	/* Issue request asdfasdf */
	r = req_slugreadwrite(vp->v_fs_e, vp->v_inode_nr, position, rw_flag, for_e,
			  buf, size, &new_pos, &cum_io_incr);

	if (r >= 0) {
		if (ex64hi(new_pos))
			panic("slug_read_write: bad new pos");

		position = new_pos;
		cum_io += cum_io_incr;
	}
  }

  /* On write, update file size and access time. */
  if (rw_flag == WRITING) {
	if (S_ISREG(vp->v_mode) || S_ISDIR(vp->v_mode)) {
		if (cmp64ul(position, vp->v_size) > 0) {
			if (ex64hi(position) != 0) {
				panic("slug_read_write: file size too big ");
			}
			vp->v_size = ex64lo(position);
		}
	}
  }

  f->filp_pos = position;

  if (r == OK) return(cum_io);
  return(r);
}