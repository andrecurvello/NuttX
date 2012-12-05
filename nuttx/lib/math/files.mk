#
# file: nuttx/lib/math/files.mk
#
# List of files with full path (relative to nuttx/lib) for build of liblib.a. It is meant to be included by both make
# and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-03
#

# this file defines following variables:
# CSRCS

ifeq ($(CONFIG_LIBM),y)
	# Add the floating point math C files to the build
	CSRCS += math/lib_acosf.c math/lib_asinf.c math/lib_atan2f.c math/lib_atanf.c math/lib_ceilf.c math/lib_cosf.c \
			math/lib_coshf.c  math/lib_expf.c math/lib_fabsf.c math/lib_floorf.c math/lib_fmodf.c math/lib_frexpf.c \
			math/lib_ldexpf.c math/lib_logf.c math/lib_log10f.c math/lib_log2f.c math/lib_modff.c math/lib_powf.c \
			math/lib_sinf.c math/lib_sinhf.c math/lib_sqrtf.c math/lib_tanf.c math/lib_tanhf.c math/lib_acos.c \
			math/lib_asin.c math/lib_atan.c math/lib_atan2.c math/lib_ceil.c math/lib_cos.c math/lib_cosh.c \
			math/lib_exp.c math/lib_fabs.c math/lib_floor.c math/lib_fmod.c math/lib_frexp.c math/lib_ldexp.c \
			math/lib_log.c math/lib_log10.c math/lib_log2.c math/lib_modf.c math/lib_pow.c math/lib_sin.c \
			math/lib_sinh.c math/lib_sqrt.c math/lib_tan.c math/lib_tanh.c math/lib_acosl.c math/lib_asinl.c \
			math/lib_atan2l.c math/lib_atanl.c math/lib_ceill.c math/lib_cosl.c math/lib_coshl.c math/lib_expl.c \
			math/lib_fabsl.c math/lib_floorl.c math/lib_fmodl.c math/lib_frexpl.c math/lib_ldexpl.c math/lib_logl.c \
			math/lib_log10l.c math/lib_log2l.c math/lib_modfl.c math/lib_powl.c math/lib_sinl.c math/lib_sinhl.c \
			math/lib_sqrtl.c math/lib_tanl.c math/lib_tanhl.c math/lib_libexpi.c math/lib_libsqrtapprox.c
endif
