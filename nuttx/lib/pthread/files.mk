#
# file: nuttx/lib/pthread/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the pthread C files to the build
CSRCS += pthread/pthread_attrinit.c pthread/pthread_attrdestroy.c pthread/pthread_attrsetschedpolicy.c \
		pthread/pthread_attrgetschedpolicy.c pthread/pthread_attrsetinheritsched.c \
		pthread/pthread_attrgetinheritsched.c pthread/pthread_attrsetstacksize.c pthread/pthread_attrgetstacksize.c \
		pthread/pthread_attrsetschedparam.c pthread/pthread_attrgetschedparam.c pthread/pthread_barrierattrinit.c \
		pthread/pthread_barrierattrdestroy.c pthread/pthread_barrierattrgetpshared.c \
		pthread/pthread_barrierattrsetpshared.c pthread/pthread_condattrinit.c pthread/pthread_condattrdestroy.c \
		pthread/pthread_mutexattrinit.c pthread/pthread_mutexattrdestroy.c pthread/pthread_mutexattrgetpshared.c \
		pthread/pthread_mutexattrsetpshared.c

ifeq ($(CONFIG_MUTEX_TYPES),y)
	CSRCS += pthread/pthread_mutexattrsettype.c pthread/pthread_mutexattrgettype.c
endif
