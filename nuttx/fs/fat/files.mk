#
# file: nuttx/fs/fat/files.mk
#
# List of files with full path (relative to nuttx/fs) for build of libfs.a. It is meant to be included by both make and
# tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_FS_FAT),y)

	# Files required for FAT file system support
	CSRCS += fat/fs_fat32.c fat/fs_fat32dirent.c fat/fs_fat32attrib.c fat/fs_fat32util.c

	# Files required for mkfatfs utility function
	CSRCS += fat/fs_mkfatfs.c fat/fs_configfat.c fat/fs_writefat.c

endif
