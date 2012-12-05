#
# file: nuttx/drivers/pipes/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
	CSRCS += pipes/pipe.c pipes/fifo.c pipes/pipe_common.c
endif
