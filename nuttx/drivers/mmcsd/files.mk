#
# file: nuttx/drivers/mmcsd/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_MMCSD),y)

# Include MMC/SD drivers
ifeq ($(CONFIG_MMCSD_SDIO),y)
	CSRCS += mmcsd/mmcsd_sdio.c
endif

ifeq ($(CONFIG_MMCSD_SPI),y)
	CSRCS += mmcsd/mmcsd_spi.c mmcsd/mmcsd_debug.c
endif

endif
