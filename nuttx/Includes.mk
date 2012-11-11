############################################################################
# Includes.mk
#
#	Copyright (C) 2012 Freddie Chopin <freddie.chopin@gmail.com>
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 3. Neither the name NuttX nor the names of its contributors may be
#    used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
############################################################################

include ${TOPDIR}/.config
include ${TOPDIR}/tools/Config.mk

# apps dir may be overriden by .config file, the string needs to be unquoted in that case
ifeq ($(CONFIG_APPS_DIR),)
	CONFIG_APPS_DIR_UNQUOTED = ../apps
else
	CONFIG_APPS_DIR_UNQUOTED = ${patsubst "%",%,${strip $(CONFIG_APPS_DIR)}}
endif

# ther might be no chip directory for some architectures
ifneq ($(CONFIG_ARCH_CHIP),)
	CONFIG_ARCH_CHIP_INC = $(TOPDIR)/arch/$(CONFIG_ARCH)/include/$(CONFIG_ARCH_CHIP)
endif

# folders included as "system" includes
GLOBAL_SYSTEM_INCLUDES = $(TOPDIR)/include $(TOPDIR)/$(CONFIG_APPS_DIR_UNQUOTED)/include $(CONFIG_ARCH_CHIP_INC)
GLOBAL_SYSTEM_CXX_INCLUDES = $(GLOBAL_SYSTEM_INCLUDES) $(TOPDIR)/include/cxx

# folder included in a standard way
GLOBAL_INCLUDES =
GLOBAL_CXX_INCLUDES = $(GLOBAL_INCLUDES)
