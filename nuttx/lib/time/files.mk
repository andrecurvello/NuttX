#
# file: nuttx/lib/time/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the time C files to the build
CSRCS += time/lib_mktime.c time/lib_gmtime.c time/lib_gmtimer.c time/lib_strftime.c time/lib_calendar2utc.c \
		time/lib_daysbeforemonth.c time/lib_isleapyear.c time/lib_time.c
