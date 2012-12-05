#
# file: nuttx/lib/dirent/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
	# Add the dirent C files to the build
	CSRCS += dirent/lib_readdirr.c dirent/lib_telldir.c
endif
