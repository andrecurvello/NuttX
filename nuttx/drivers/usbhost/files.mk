#
# file: nuttx/drivers/usbhost/files.mk
#
# List of files with full path (relative to nuttx/drivers) for build of libdrivers.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

CSRCS += usbhost/hid_parser.c

ifeq ($(CONFIG_USBHOST),y)

# Include built-in USB host driver logic
CSRCS += usbhost/usbhost_registry.c usbhost/usbhost_registerclass.c usbhost/usbhost_findclass.c \
		usbhost/usbhost_enumerate.c usbhost/usbhost_storage.c usbhost/usbhost_hidkbd.c

# Include add-on USB host driver logic (see misc/drivers)

# !! THIS IS BROKEN FOR TUP !!
ifeq ($(CONFIG_NET),y)
	RTL8187_CSRC := ${shell if [ -f usbhost/rtl8187x.c ]; then echo "rtl8187x.c"; fi}
	CSRCS += $(RTL8187_CSRC)
endif

endif
