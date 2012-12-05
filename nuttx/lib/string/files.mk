#
# file: nuttx/lib/string/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the string C files to the build
CSRCS += string/lib_checkbase.c string/lib_isbasedigit.c string/lib_memset.c string/lib_memchr.c string/lib_memccpy.c \
		string/lib_memcmp.c string/lib_memmove.c string/lib_skipspace.c string/lib_strcasecmp.c string/lib_strcat.c \
		string/lib_strchr.c string/lib_strcpy.c string/lib_strcmp.c string/lib_strcspn.c string/lib_strdup.c \
		string/lib_strerror.c string/lib_strlen.c string/lib_strnlen.c string/lib_strncasecmp.c string/lib_strncat.c \
		string/lib_strncmp.c string/lib_strncpy.c string/lib_strndup.c string/lib_strcasestr.c string/lib_strpbrk.c \
		string/lib_strrchr.c string/lib_strspn.c string/lib_strstr.c string/lib_strtok.c string/lib_strtokr.c \
		string/lib_strtol.c string/lib_strtoll.c string/lib_strtoul.c string/lib_strtoull.c string/lib_strtod.c

ifneq ($(CONFIG_ARCH_MEMCPY),y)
ifeq ($(CONFIG_MEMCPY_VIK),y)
	CSRCS += string/lib_vikmemcpy.c
else
	CSRCS += string/lib_memcpy.c
endif
endif
