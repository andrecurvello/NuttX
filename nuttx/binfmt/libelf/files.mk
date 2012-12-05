#
# file: nuttx/binfmt/libelf/files.mk
#
# List of files with full path (relative to nuttx/binfmt) for build of libbinfmt.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# ASRCS, CSRCS, CXXSRCS

ifeq ($(CONFIG_ELF),y)

BINFMT_ASRCS +=
BINFMT_CSRCS += libelf/elf.c libelf/libelf_bind.c libelf/libelf_init.c libelf/libelf_iobuffer.c libelf/libelf_load.c \
		libelf/libelf_read.c libelf/libelf_sections.c libelf/libelf_symbols.c libelf/libelf_uninit.c \
		libelf/libelf_unload.c libelf/libelf_verify.c
BINFMT_CXXSRCS +=

ifeq ($(CONFIG_BINFMT_CONSTRUCTORS),y)
	BINFMT_CSRCS += libelf/libelf_ctors.c libelf/libelf_dtors.c
endif

endif
