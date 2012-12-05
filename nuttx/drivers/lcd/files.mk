#
# file: nuttx/drivers/lcd/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_NX_LCDDRIVER),y)

# Include LCD drivers
ifeq ($(CONFIG_LCD_P14201),y)
	CSRCS += lcd/p14201.c
endif

ifeq ($(CONFIG_LCD_NOKIA6100),y)
	CSRCS += lcd/nokia6100.c
endif

ifeq ($(CONFIG_LCD_UG2864AMBAG01),y)
	CSRCS += lcd/ug-2864ambag01.c
endif

ifeq ($(CONFIG_LCD_UG9664HSWAG01),y)
	CSRCS += lcd/ug-9664hswag01.c
endif

ifeq ($(CONFIG_LCD_SSD1289),y)
	CSRCS += lcd/ssd1289.c
endif

ifeq ($(CONFIG_LCD_MIO283QT2),y)
	CSRCS += lcd/mio283qt2.c
endif

endif
