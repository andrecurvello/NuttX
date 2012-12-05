#
# file: apps/modbus/ascii/files.mk
#
# List of files with full path (relative to apps/modbus) for build of libapps.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-02
#

# this file defines following variables:
# ASRCS, CSRCS, CXXSRCS

ifeq ($(CONFIG_MB_ASCII_ENABLED),y)

ASRCS +=
CSRCS += ascii/mbascii.c
CXXSRCS +=

endif
