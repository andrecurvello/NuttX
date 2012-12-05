#
# file: nuttx/lib/stdio/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

# Add the stdio C files to the build
# This first group of C files do not depend on having file descriptors or
# C streams.
CSRCS += stdio/lib_fileno.c stdio/lib_printf.c stdio/lib_rawprintf.c stdio/lib_lowprintf.c stdio/lib_sprintf.c \
		stdio/lib_asprintf.c stdio/lib_snprintf.c stdio/lib_libsprintf.c stdio/lib_vsprintf.c stdio/lib_avsprintf.c \
		stdio/lib_vsnprintf.c stdio/lib_libvsprintf.c stdio/lib_meminstream.c stdio/lib_memoutstream.c \
		stdio/lib_lowinstream.c stdio/lib_lowoutstream.c stdio/lib_zeroinstream.c stdio/lib_nullinstream.c \
		stdio/lib_nulloutstream.c stdio/lib_sscanf.c

# The remaining sources files depend upon file descriptors

ifneq ($(CONFIG_NFILE_DESCRIPTORS),0)

	CSRCS += stdio/lib_rawinstream.c stdio/lib_rawoutstream.c

# And these depend upon both file descriptors and C streams
ifneq ($(CONFIG_NFILE_STREAMS),0)
	CSRCS += stdio/lib_fopen.c stdio/lib_fclose.c stdio/lib_fread.c stdio/lib_libfread.c stdio/lib_fseek.c \
			stdio/lib_ftell.c stdio/lib_fsetpos.c stdio/lib_fgetpos.c stdio/lib_fgetc.c stdio/lib_fgets.c \
			stdio/lib_gets.c stdio/lib_fwrite.c stdio/lib_libfwrite.c stdio/lib_fflush.c stdio/lib_libflushall.c \
			stdio/lib_libfflush.c stdio/lib_rdflush.c stdio/lib_wrflush.c stdio/lib_fputc.c stdio/lib_puts.c \
			stdio/lib_fputs.c stdio/lib_ungetc.c stdio/lib_vprintf.c stdio/lib_fprintf.c stdio/lib_vfprintf.c \
			stdio/lib_stdinstream.c stdio/lib_stdoutstream.c stdio/lib_perror.c stdio/lib_feof.c stdio/lib_ferror.c \
			stdio/lib_clearerr.c
endif
endif

# Other support that depends on specific, configured features.
ifeq ($(CONFIG_SYSLOG),y)
	CSRCS += stdio/lib_syslogstream.c
endif

ifeq ($(CONFIG_LIBC_FLOATINGPOINT),y)
	CSRCS += stdio/lib_dtoa.c
endif

ifeq ($(CONFIG_STDIO_LINEBUFFER),y)
	CSRCS += stdio/lib_libnoflush.c
endif
