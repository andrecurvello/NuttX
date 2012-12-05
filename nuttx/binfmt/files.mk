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

BINFMT_ASRCS +=
BINFMT_CSRCS += binfmt_globals.c binfmt_register.c binfmt_unregister.c binfmt_loadmodule.c binfmt_unloadmodule.c \
		binfmt_execmodule.c binfmt_exec.c binfmt_dumpmodule.c symtab_findbyname.c symtab_findbyvalue.c \
		symtab_findorderedbyname.c symtab_findorderedbyvalue.c
BINFMT_CXXSRCS +=
