#
# file: nuttx/lib/mics/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the internal C files to the build
CSRCS += misc/lib_init.c misc/lib_filesem.c

# Add C files that depend on file OR socket descriptors
ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
	CSRCS += misc/lib_sendfile.c
ifneq ($(CONFIG_NFILE_STREAMS),0)
	CSRCS += misc/lib_streamsem.c
endif

else

ifneq ($(CONFIG_NSOCKET_DESCRIPTORS),0)
	CSRCS += misc/lib_sendfile.c
ifneq ($(CONFIG_NFILE_STREAMS),0)
	CSRCS += misc/lib_streamsem.c
endif
endif

endif

# Add the miscellaneous C files to the build
CSRCS += misc/lib_match.c misc/lib_crc32.c misc/lib_dbg.c misc/lib_dumpbuffer.c
