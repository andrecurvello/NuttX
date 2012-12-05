#
# file: nuttx/lib/stdlib/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the stdlib C files to the build
CSRCS += stdlib/lib_abs.c stdlib/lib_abort.c stdlib/lib_imaxabs.c stdlib/lib_labs.c stdlib/lib_llabs.c \
		stdlib/lib_rand.c stdlib/lib_qsort.c
