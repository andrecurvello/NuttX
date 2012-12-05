#
# file: nuttx/lib/semaphore/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the semaphore C files to the build
CSRCS += semaphore/sem_init.c semaphore/sem_getvalue.c
