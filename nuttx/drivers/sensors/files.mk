#
# file: nuttx/drivers/sensors/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_I2C),y)
ifeq ($(CONFIG_I2C_TRANSFER),y)
	CSRCS += sensors/lis331dl.c
endif

ifeq ($(CONFIG_I2C_LM75),y)
	CSRCS += sensors/lm75.c
endif
endif

ifeq ($(CONFIG_QENCODER),y)
	CSRCS += sensors/qencoder.c
endif
