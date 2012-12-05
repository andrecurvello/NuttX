#
# file: nuttx/lib/net/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the networking C files to the build
CSRCS += net/lib_etherntoa.c net/lib_htons.c net/lib_htonl.c net/lib_inetaddr.c net/lib_inetntoa.c net/lib_inetntop.c \
		net/lib_inetpton.c
