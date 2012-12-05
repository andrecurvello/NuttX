#
# file: nuttx/lib/termios/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# termios.h support requires file descriptors and that CONFIG_SERIAL_TERMIOS is defined
ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)
ifeq ($(CONFIG_SERIAL_TERMIOS),y)
	# Add the termios C files to the build
	CSRCS += termios/lib_cfgetspeed.c termios/lib_cfsetspeed.c termios/lib_tcflush.c termios/lib_tcgetattr.c \
			termios/lib_tcsetattr.c
endif
endif
