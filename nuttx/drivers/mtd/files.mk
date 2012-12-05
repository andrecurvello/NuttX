#
# file: nuttx/drivers/mtd/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

CSRCS += mtd/at45db.c mtd/flash_eraseall.c mtd/ftl.c mtd/m25px.c mtd/rammtd.c mtd/ramtron.c

ifeq ($(CONFIG_MTD_AT24XX),y)
	CSRCS += mtd/at24xx.c
endif

ifeq ($(CONFIG_MTD_SST25),y)
	CSRCS += mtd/sst25.c
endif

ifeq ($(CONFIG_MTD_W25),y)
	CSRCS += mtd/w25.c
endif
