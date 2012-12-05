#
# file: nuttx/fs/files.mk
#
# List of files with full path (relative to nuttx/fs) for build of libfs.a. It is meant to be included by both make and
# tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_NFILE_DESCRIPTORS),0)
# Socket descriptor support
ifneq ($(CONFIG_NSOCKET_DESCRIPTORS),0)
	CSRCS += fs_close.c fs_read.c fs_write.c fs_ioctl.c fs_poll.c fs_select.c
endif

# Support for network access using streams
ifneq ($(CONFIG_NFILE_STREAMS),0)
	CSRCS += fs_fdopen.c 
endif

else

# Common file/socket descriptor support

CSRCS += fs_close.c fs_closedir.c fs_dup.c fs_dup2.c fs_fcntl.c fs_filedup.c fs_filedup2.c fs_ioctl.c fs_lseek.c \
		fs_open.c fs_opendir.c fs_poll.c fs_read.c fs_readdir.c fs_rewinddir.c fs_seekdir.c fs_stat.c fs_statfs.c \
		fs_select.c fs_write.c fs_files.c fs_foreachinode.c fs_inode.c fs_inodeaddref.c fs_inodefind.c \
		fs_inoderelease.c fs_inoderemove.c fs_inodereserve.c fs_registerdriver.c fs_unregisterdriver.c \
		fs_registerblockdriver.c fs_unregisterblockdriver.c fs_findblockdriver.c fs_openblockdriver.c \
		fs_closeblockdriver.c

# Stream support
ifneq ($(CONFIG_NFILE_STREAMS),0)
	CSRCS += fs_fdopen.c 
endif

# System logging to a character device (or file)
ifeq ($(CONFIG_SYSLOG),y)
ifeq ($(CONFIG_SYSLOG_CHAR),y)
	CSRCS += fs_syslog.c 
endif
endif

# Additional files required is mount-able file systems are supported
ifneq ($(CONFIG_DISABLE_MOUNTPOINT),y)
	CSRCS += fs_fsync.c fs_mkdir.c fs_mount.c fs_rename.c fs_rmdir.c fs_umount.c fs_unlink.c fs_foreachmountpoint.c  
endif

endif
