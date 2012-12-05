#
# file: nuttx/mm/files.mk
#
# List of files with full path (relative to nuttx/mm) for build of libmm.a. It is meant to be included by both make and
# tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

CSRCS = mm_initialize.c mm_sem.c  mm_addfreechunk.c mm_size2ndx.c mm_shrinkchunk.c mm_malloc.c mm_zalloc.c mm_calloc.c \
		mm_realloc.c mm_memalign.c mm_free.c mm_mallinfo.c

ifeq ($(CONFIG_GRAN),y)
	CSRCS += mm_graninit.c mm_granalloc.c mm_granfree.c mm_grancritical.c
endif
