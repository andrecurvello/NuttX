#
# file: apps/nshlib/files.mk
#
# List of files with full path (relative to apps/nshlib) for build of libapps.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# ASRCS, CSRCS, CXXSRCS

ASRCS =
CSRCS = nsh_init.c nsh_parse.c nsh_console.c nsh_fscmds.c nsh_ddcmd.c nsh_proccmds.c nsh_mmcmds.c nsh_envcmds.c \
		nsh_dbgcmds.c
CXXSRCS =

ifeq ($(CONFIG_NSH_BUILTIN_APPS),y)
	CSRCS += nsh_apps.c
endif

ifeq ($(CONFIG_NSH_ROMFSETC),y)
	CSRCS += nsh_romfsetc.c
endif

ifeq ($(CONFIG_NET),y)
	CSRCS += nsh_netinit.c nsh_netcmds.c
endif

ifeq ($(CONFIG_RTC),y)
	CSRCS += nsh_timcmds.c
endif

ifneq ($(CONFIG_DISABLE_MOUNTPOINT),y)
	CSRCS += nsh_mntcmds.c
endif

ifeq ($(CONFIG_NSH_CONSOLE),y)
	CSRCS += nsh_consolemain.c
endif

ifeq ($(CONFIG_NSH_TELNET),y)
	CSRCS += nsh_telnetd.c
endif

ifneq ($(CONFIG_NSH_DISABLESCRIPT),y)
	CSRCS += nsh_test.c
endif

ifeq ($(CONFIG_USBDEV),y)
	CSRCS += nsh_usbdev.c
endif

ifeq ($(CONFIG_NETUTILS_CODECS),y)
	CSRCS += nsh_codeccmd.c
endif
