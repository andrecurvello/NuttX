#
# file: nuttx/fs/nfs/files.mk
#
# List of files with full path (relative to nuttx/fs) for build of libfs.a. It is meant to be included by both make and
# tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_NFS),y)
	CSRCS += nfs/rpc_clnt.c nfs/nfs_util.c nfs/nfs_vfsops.c
endif
