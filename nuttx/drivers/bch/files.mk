#
# file: nuttx/drivers/bch/files.mk
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
ifneq ($(CONFIG_DISABLE_MOUNTPOINT),y)

# Include BCH driver
CSRCS += bch/bchlib_setup.c bch/bchlib_teardown.c bch/bchlib_read.c bch/bchlib_write.c bch/bchlib_cache.c \
		bch/bchlib_sem.c bch/bchdev_register.c bch/bchdev_unregister.c bch/bchdev_driver.c

endif
endif
