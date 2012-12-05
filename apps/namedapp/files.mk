#
# file: apps/namedapp/files.mk
#
# List of files with full path (relative to apps/namedapp) for build of libapps.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# ASRCS, CSRCS, CXXSRCS

ASRCS =
CSRCS = namedapp.c exec_namedapp.c
CXXSRCS =

ifeq ($(CONFIG_APPS_BINDIR),y)
	CSRCS += binfs.c
endif
