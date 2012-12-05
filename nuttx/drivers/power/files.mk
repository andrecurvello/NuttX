#
# file: nuttx/drivers/power/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Include power management sources
ifeq ($(CONFIG_PM),y)
CSRCS += power/pm_activity.c power/pm_changestate.c power/pm_checkstate.c power/pm_initialize.c power/pm_register.c \
		power/pm_update.c
endif

# Add battery drivers
ifeq ($(CONFIG_BATTERY),y)
CSRCS += power/battery.c

# Add I2C-based battery drivers
ifeq ($(CONFIG_I2C),y)
# Add the MAX1704x I2C-based battery driver
ifeq ($(CONFIG_I2C_MAX1704X),y)
	CSRCS += power/max1704x.c
endif
endif

endif
