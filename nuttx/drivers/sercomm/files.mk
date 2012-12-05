#
# file: nuttx/drivers/sercomm/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# File descriptor support is needed for this driver
ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
# The sercomm driver should not be build for all platforms.  Only build it is so configured
ifeq ($(CONFIG_SERCOMM_CONSOLE),y)
# Include serial drivers
	CSRCS += sercomm/console.c sercomm/uart.c
endif
endif
