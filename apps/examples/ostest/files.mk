#
# file: apps/examples/ostest/files.mk
#
# List of files with full path (relative to apps/examples/ostest) for build of libapps.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-05
#

# this file defines following variables:
# CSRCS

CSRCS		= ostest_main.c dev_null.c

ifeq ($(CONFIG_ARCH_FPU),y)
CSRCS		+= fpu.c
endif

ifneq ($(CONFIG_DISABLE_PTHREAD),y)
CSRCS		+= cancel.c cond.c mutex.c sem.c barrier.c
ifneq ($(CONFIG_RR_INTERVAL),0)
CSRCS		+= roundrobin.c
endif

ifeq ($(CONFIG_MUTEX_TYPES),y)
CSRCS		+= rmutex.c
endif
endif

ifneq ($(CONFIG_DISABLE_SIGNALS),y)
CSRCS		+= sighand.c
ifneq ($(CONFIG_DISABLE_PTHREAD),y)
ifneq ($(CONFIG_DISABLE_CLOCK),y)
CSRCS		+= timedwait.c
endif
endif
endif

ifneq ($(CONFIG_DISABLE_MQUEUE),y)
ifneq ($(CONFIG_DISABLE_PTHREAD),y)
CSRCS		+= mqueue.c 
ifneq ($(CONFIG_DISABLE_CLOCK),y)
CSRCS		+= timedmqueue.c 
endif
endif
endif

ifneq ($(CONFIG_DISABLE_POSIX_TIMERS),y)
CSRCS		+= posixtimer.c
endif

ifneq ($(CONFIG_DISABLE_SIGNALS),y)
ifneq ($(CONFIG_DISABLE_PTHREAD),y)
ifeq ($(CONFIG_PRIORITY_INHERITANCE),y)
CSRCS		+= prioinherit.c
endif
endif
endif
