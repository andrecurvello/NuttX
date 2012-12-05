#
# file: nuttx/drivers/input/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_INPUT),y)

# Include the selected touchscreen drivers
ifeq ($(CONFIG_INPUT_TSC2007),y)
	CSRCS += input/tsc2007.c
endif

ifeq ($(CONFIG_INPUT_ADS7843E),y)
	CSRCS += input/ads7843e.c
endif

ifeq ($(CONFIG_INPUT_MAX11802),y)
	CSRCS += input/max11802.c
endif

ifeq ($(CONFIG_INPUT_STMPE811),y)
	CSRCS += input/stmpe811_base.c
ifneq ($(CONFIG_INPUT_STMPE811_TSC_DISABLE),y)
	CSRCS += input/stmpe811_tsc.c
endif
ifneq ($(CONFIG_INPUT_STMPE811_GPIO_DISABLE),y)
	CSRCS += input/stmpe811_gpio.c
endif
ifneq ($(CONFIG_INPUT_STMPE811_ADC_DISABLE),y)
	CSRCS += input/stmpe811_adc.c
endif
ifneq ($(CONFIG_INPUT_STMPE811_TEMP_DISABLE),y)
	CSRCS += input/stmpe811_temp.c
endif
endif

endif
