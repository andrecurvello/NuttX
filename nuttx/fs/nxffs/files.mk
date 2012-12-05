#
# file: nuttx/fs/nxffs/files.mk
#
# List of files with full path (relative to nuttx/fs) for build of libfs.a. It is meant to be included by both make and
# tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_FS_NXFFS),y)
CSRCS += nxffs/nxffs_block.c nxffs/nxffs_blockstats.c nxffs/nxffs_cache.c nxffs/nxffs_dirent.c nxffs/nxffs_dump.c \
		nxffs/nxffs_initialize.c nxffs/nxffs_inode.c nxffs/nxffs_ioctl.c nxffs/nxffs_open.c nxffs/nxffs_pack.c \
		nxffs/nxffs_read.c nxffs/nxffs_reformat.c nxffs/nxffs_stat.c nxffs/nxffs_unlink.c nxffs/nxffs_util.c \
		nxffs/nxffs_write.c
endif
