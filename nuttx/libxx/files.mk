#
# file: nuttx/libxx/files.mk
#
# List of files with full path (relative to nuttx/libxx) for build of liblibxx.a. It is meant to be included by both
# make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CXXSRCS

CXXSRCS = libxx_cxapurevirtual.cxx libxx_eabi_atexit.cxx  libxx_cxa_atexit.cxx

# Some of the libxx/ files are not need if uClibc++ is installed because
# uClibx++ replaces them

# !! no support for uClibc++ !!

ifneq ($(CONFIG_UCLIBCXX),y)
	CXXSRCS += libxx_delete.cxx libxx_deletea.cxx libxx_new.cxx libxx_newa.cxx libxx_stdthrow.cxx
else
ifneq ($(UCLIBCXX_EXCEPTION),y)
	CXXSRCS += libxx_stdthrow.cxx
endif
endif
