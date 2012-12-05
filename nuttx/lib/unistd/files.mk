#
# file: nuttx/lib/unistd/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the unistd C files to the build
CSRCS += unistd/lib_getopt.c unistd/lib_getoptargp.c unistd/lib_getoptindp.c unistd/lib_getoptoptp.c

ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
ifneq ($(CONFIG_DISABLE_ENVIRON),y)
	CSRCS += unistd/lib_chdir.c unistd/lib_getcwd.c
endif
endif
