#
# file: nuttx/lib/signal/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifneq ($(CONFIG_DISABLE_SIGNALS),y)
	# Add the signal C files to the build
	CSRCS += signal/sig_emptyset.c signal/sig_fillset.c signal/sig_addset.c signal/sig_delset.c signal/sig_ismember.c
endif
