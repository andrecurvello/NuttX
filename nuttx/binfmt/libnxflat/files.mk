#
# file: nuttx/binfmt/libnxflat/files.mk
#
# List of files with full path (relative to nuttx/binfmt) for build of libbinfmt.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# BINFMT_ASRCS, BINFMT_CSRCS, BINFMT_CXXSRCS

ifeq ($(CONFIG_NXFLAT),y)

BINFMT_ASRCS +=
BINFMT_CSRCS += libnxflat/nxflat.c libnxflat/libnxflat_init.c libnxflat/libnxflat_uninit.c libnxflat/libnxflat_load.c \
		libnxflat/libnxflat_unload.c libnxflat/libnxflat_verify.c libnxflat/libnxflat_read.c libnxflat/libnxflat_bind.c
BINFMT_CXXSRCS +=

endif
