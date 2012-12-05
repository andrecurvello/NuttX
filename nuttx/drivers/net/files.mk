#
# file: nuttx/drivers/net/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_NET),y)

ifeq ($(CONFIG_NET_DM90x0),y)
	CSRCS += net/dm90x0.c
endif

ifeq ($(CONFIG_NET_CS89x0),y)
	CSRCS += net/cs89x0.c
endif

ifeq ($(CONFIG_ENC28J60),y)
	CSRCS += net/enc28j60.c
endif

ifeq ($(CONFIG_NET_VNET),y)
	CSRCS += net/vnet.c
endif

ifeq ($(CONFIG_NET_E1000),y)
	CSRCS += net/e1000.c
endif

ifeq ($(CONFIG_NET_SLIP),y)
	CSRCS += net/slip.c
endif

endif
