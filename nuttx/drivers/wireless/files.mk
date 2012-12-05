#
# file: nuttx/drivers/wireless/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_WIRELESS),y)
# Include wireless drivers
	CSRCS += wireless/cc1101/cc1101.c wireless/cc1101/ISM1_868MHzGFSK100kbps.c wireless/cc1101/ISM2_905MHzGFSK250kbps.c
endif
