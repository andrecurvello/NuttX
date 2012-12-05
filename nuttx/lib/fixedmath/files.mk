#
# file: nuttx/lib/fixedmath/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the fixed precision math C files to the build
CSRCS += fixedmath/lib_rint.c fixedmath/lib_fixedmath.c fixedmath/lib_b16sin.c fixedmath/lib_b16cos.c \
		fixedmath/lib_b16atan2.c
