#
# file: nuttx/arch/arm/src/stm32/files.mk
#
# List of files with full path (relative to nuttx/arch/arm/src) for build of libarch.a. It is meant to be included by
# both make and tup
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-02
#

# this file defines following variables:
# HEAD_ASRC, CMN_ASRCS, CMN_CSRCS, CHIP_ASRCS, CHIP_CSRCS

ifeq ($(CONFIG_ARMV7M_CMNVECTOR),y)
HEAD_ASRC	= 
else
HEAD_ASRC	= stm32/stm32_vectors.S
endif

CMN_ASRCS	= armv7-m/up_saveusercontext.S armv7-m/up_fullcontextrestore.S armv7-m/up_switchcontext.S
CMN_CSRCS	= armv7-m/up_assert.c armv7-m/up_blocktask.c armv7-m/up_copystate.c \
			  common/up_createstack.c common/up_mdelay.c common/up_udelay.c common/up_exit.c \
			  common/up_initialize.c armv7-m/up_initialstate.c common/up_interruptcontext.c \
			  armv7-m/up_memfault.c common/up_modifyreg8.c common/up_modifyreg16.c common/up_modifyreg32.c \
			  armv7-m/up_releasepending.c common/up_releasestack.c armv7-m/up_reprioritizertr.c \
			  armv7-m/up_schedulesigaction.c armv7-m/up_sigdeliver.c armv7-m/up_systemreset.c \
			  armv7-m/up_unblocktask.c common/up_usestack.c armv7-m/up_doirq.c armv7-m/up_hardfault.c armv7-m/up_svcall.c

ifeq ($(CONFIG_ARMV7M_CMNVECTOR),y)
CMN_ASRCS	+= armv7-m/up_exception.S
CMN_CSRCS	+= armv7-m/up_vectors.c
endif

ifeq ($(CONFIG_ARCH_MEMCPY),y)
CMN_ASRCS	+= armv7-m/up_memcpy.S
endif

ifeq ($(CONFIG_DEBUG_STACK),y)
CMN_CSRCS	+= armv7-m/up_checkstack.c
endif

ifeq ($(CONFIG_ELF),y)
CMN_CSRCS += armv7-m/up_elf.c
endif

ifeq ($(CONFIG_ARCH_FPU),y)
CMN_ASRCS	+= armv7-m/up_fpu.S
endif

CHIP_ASRCS	= 
CHIP_CSRCS	= stm32/stm32_allocateheap.c stm32/stm32_start.c stm32/stm32_rcc.c stm32/stm32_lse.c \
			  stm32/stm32_lsi.c stm32/stm32_gpio.c stm32/stm32_exti_gpio.c stm32/stm32_flash.c stm32/stm32_irq.c \
			  stm32/stm32_timerisr.c stm32/stm32_dma.c stm32/stm32_lowputc.c stm32/stm32_serial.c \
			  stm32/stm32_spi.c stm32/stm32_sdio.c stm32/stm32_tim.c stm32/stm32_i2c.c stm32/stm32_waste.c

ifeq ($(CONFIG_USBDEV),y)
ifeq ($(CONFIG_STM32_USB),y)
CMN_CSRCS	+= stm32/stm32_usbdev.c
endif
ifeq ($(CONFIG_STM32_OTGFS),y)
CMN_CSRCS	+= stm32/stm32_otgfsdev.c
endif
endif

ifeq ($(CONFIG_USBHOST),y)
ifeq ($(CONFIG_STM32_OTGFS),y)
CMN_CSRCS	+= stm32/stm32_otgfshost.c
endif
endif

ifeq ($(CONFIG_ARMV7M_CMNVECTOR),y)
CHIP_ASRCS	+= stm32/stm32_vectors.S
endif

ifneq ($(CONFIG_IDLE_CUSTOM),y)
CHIP_CSRCS	+= stm32/stm32_idle.c
endif

CHIP_CSRCS	+= stm32/stm32_pmstop.c stm32/stm32_pmstandby.c stm32/stm32_pmsleep.c

ifneq ($(CONFIG_PM_CUSTOMINIT),y)
CHIP_CSRCS	+= stm32/stm32_pminitialize.c
endif

ifeq ($(CONFIG_STM32_ETHMAC),y)
CHIP_CSRCS	+= stm32/stm32_eth.c
endif

ifeq ($(CONFIG_STM32_PWR),y)
CHIP_CSRCS	+= stm32/stm32_pwr.c
endif

ifeq ($(CONFIG_RTC),y)
CHIP_CSRCS	+= stm32/stm32_rtc.c
ifeq ($(CONFIG_RTC_ALARM),y)
CHIP_CSRCS	+= stm32/stm32_exti_alarm.c
endif
endif

ifeq ($(CONFIG_ADC),y)
CHIP_CSRCS	+= stm32/stm32_adc.c
endif

ifeq ($(CONFIG_DAC),y)
CHIP_CSRCS	+= stm32/stm32_dac.c
endif

ifeq ($(CONFIG_DEV_RANDOM),y)
CHIP_CSRCS	+= stm32/stm32_rng.c
endif

ifeq ($(CONFIG_PWM),y)
CHIP_CSRCS	+= stm32/stm32_pwm.c
endif

ifeq ($(CONFIG_QENCODER),y)
CHIP_CSRCS	+= stm32/stm32_qencoder.c
endif

ifeq ($(CONFIG_CAN),y)
CHIP_CSRCS	+= stm32/stm32_can.c
endif

ifeq ($(CONFIG_STM32_IWDG),y)
CHIP_CSRCS	+= stm32/stm32_iwdg.c
endif

ifeq ($(CONFIG_STM32_WWDG),y)
CHIP_CSRCS	+= stm32/stm32_wwdg.c
endif

ifeq ($(CONFIG_DEBUG),y)
CHIP_CSRCS	+= stm32/stm32_dumpgpio.c
endif
