#
# file: nuttx/configs/stm32f4discovery/src/files.mk
#
# List of files with full path (relative to nuttx/configs/stm32f4discovery/src) for build of libboard.a. It is
# meant to be included by both make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-05
#

# this file defines following variables:
# CSRCS

CSRCS		= up_boot.c up_spi.c

ifeq ($(CONFIG_HAVE_CXX),y)
CSRCS		+= up_cxxinitialize.c
endif

ifeq ($(CONFIG_ARCH_LEDS),y)
CSRCS		+= up_autoleds.c
else
CSRCS		+= up_userleds.c
endif

ifeq ($(CONFIG_ARCH_BUTTONS),y)
CSRCS		+=  up_buttons.c
endif

ifeq ($(CONFIG_STM32_OTGFS),y)
CSRCS		+= up_usb.c
endif

ifeq ($(CONFIG_PWM),y)
CSRCS		+= up_pwm.c
endif

ifeq ($(CONFIG_QENCODER),y)
CSRCS		+= up_qencoder.c
endif

ifeq ($(CONFIG_WATCHDOG),y)
CSRCS		+= up_watchdog.c
endif

ifeq ($(CONFIG_NSH_ARCHINIT),y)
CSRCS		+= up_nsh.c
endif

ifeq ($(CONFIG_PM_CUSTOMINIT),y)
CSRCS		+= up_pm.c
endif

ifeq ($(CONFIG_PM_BUTTONS),y)
CSRCS		+= up_pmbuttons.c 
endif

ifeq ($(CONFIG_IDLE_CUSTOM),y)
CSRCS		+= up_idle.c
endif

ifeq ($(CONFIG_STM32_FSMC),y)
CSRCS		+= up_extmem.c

ifeq ($(CONFIG_LCD_SSD1289),y)
CSRCS		+= up_ssd1289.c
endif
endif

ifeq ($(CONFIG_LCD_UG2864AMBAG01),y)
CSRCS		+= up_ug2864ambag01.c
endif
