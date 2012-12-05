 #
# file: nuttx/fs/romfs/files.mk
#
# List of files with full path (relative to nuttx/fs) for build of libfs.a. It is meant to be included by both make and
# tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_FS_ROMFS),y)
	CSRCS += romfs/fs_romfs.c romfs/fs_romfsutil.c
endif
