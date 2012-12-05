#
# file: nuttx/drivers/analog/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Check for DAC devices
ifeq ($(CONFIG_DAC),y)

# Include the common DAC character driver
CSRCS += analog/dac.c

# Include DAC device drivers
ifeq ($(CONFIG_DAC_AD5410),y)
	CSRCS += analog/ad5410.c
endif
endif

# Check for ADC devices
ifeq ($(CONFIG_ADC),y)

# Include the common ADC character driver
CSRCS += analog/adc.c

# Amplifiers/multiplexers
ifeq ($(CONFIG_ADC_PGA11X),y)
	CSRCS += analog/pga11x.c
endif

# Include ADC device drivers
ifeq ($(CONFIG_ADC_ADS125X),y)
	CSRCS += analog/ads1255.c
endif
endif
