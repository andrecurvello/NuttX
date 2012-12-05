#
# file: nuttx/lib/queue/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the queue C files to the build
CSRCS += queue/sq_addlast.c queue/sq_addfirst.c queue/sq_addafter.c queue/sq_rem.c queue/sq_remlast.c \
		queue/sq_remfirst.c queue/sq_remafter.c queue/dq_addlast.c queue/dq_addfirst.c queue/dq_addafter.c \
		queue/dq_addbefore.c queue/dq_rem.c queue/dq_remlast.c queue/dq_remfirst.c
