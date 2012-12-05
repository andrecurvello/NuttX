#
# file: nuttx/drivers/usbdev/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_USBDEV),y)

ifeq ($(CONFIG_PL2303),y)
	CSRCS += usbdev/pl2303.c
endif

ifeq ($(CONFIG_CDCACM),y)
	CSRCS += usbdev/cdcacm.c usbdev/cdcacm_desc.c
endif

ifeq ($(CONFIG_USBMSC),y)
	CSRCS += usbdev/usbmsc.c usbdev/usbmsc_desc.c usbdev/usbmsc_scsi.c
endif

ifeq ($(CONFIG_USBDEV_COMPOSITE),y)
	CSRCS += usbdev/composite.c usbdev/composite_desc.c
endif

CSRCS += usbdev/usbdev_trace.c usbdev/usbdev_trprintf.c

endif
