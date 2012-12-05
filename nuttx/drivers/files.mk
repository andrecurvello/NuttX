#
# file: nuttx/drivers/files.mk
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
	CSRCS += dev_null.c dev_zero.c loop.c

ifneq ($(CONFIG_DISABLE_MOUNTPOINT),y)
	CSRCS += ramdisk.c rwbuffer.c
endif

ifeq ($(CONFIG_CAN),y)
	CSRCS += can.c
endif

ifeq ($(CONFIG_PWM),y)
	CSRCS += pwm.c
endif

ifeq ($(CONFIG_WATCHDOG),y)
	CSRCS += watchdog.c
endif

endif
