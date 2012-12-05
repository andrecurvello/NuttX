#
# file: nuttx/drivers/syslog/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_SYSLOG),y)

# If no special loggin devices are implemented, then the default SYSLOG
# logic at fs/fs_syslog.c will be used

# (Add other SYSLOG drivers here)
ifeq ($(CONFIG_RAMLOG),y)
	CSRCS += syslog/ramlog.c
endif

else

# The RAMLOG can be used even if system logging is not enabled.
ifeq ($(CONFIG_RAMLOG),y)
# Include RAMLOG build support
	CSRCS += syslog/ramlog.c
endif

endif
