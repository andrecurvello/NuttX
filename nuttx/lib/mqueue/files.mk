#
# file: nuttx/lib/mqueue/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifneq ($(CONFIG_DISABLE_MQUEUE),y)
	# Add the mqueue C files to the build
	CSRCS += mqueue/mq_setattr.c mqueue/mq_getattr.c
endif
