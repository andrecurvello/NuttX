#
# file: Makefile
#
# Makefile for whole project, just a proxy for nuttx/Makefile with some additional logic
#
# author: Freddie Chopin, http://www.distortec.com http://www.freddiechopin.info
# date: 2012-12-05
#

# get project's working directory in Windows format, but with forward slashes
TOPDIR := "${shell echo `pwd -W`}/nuttx"

all: nuttx/.config nuttx/Make.defs
	$(MAKE) -C nuttx V=1 TOPDIR=$(TOPDIR)
	@echo ' '
	@echo "Creating extended listing nuttx.lss..."
	arm-none-eabi-objdump --demangle -S nuttx/nuttx.elf > nuttx/nuttx.lss 
	@echo ' '
	arm-none-eabi-gcc --version
	arm-none-eabi-size -B nuttx/nuttx.elf
	
clean:
	$(MAKE) -C nuttx clean
	
clean_context:
	$(MAKE) -C nuttx clean_context
	
distclean:
	$(MAKE) -C nuttx distclean

tup: nuttx/.config nuttx/Make.defs tup.config
	$(MAKE) -C nuttx TOPDIR=$(TOPDIR) context
	tup upd

nuttx/.config: nuttx/configs/stm32f4discovery/ostest/defconfig
	cp $< $@
	
nuttx/Make.defs: nuttx/configs/stm32f4discovery/ostest/Make.defs
	cp $< $@
	
tup.config: nuttx/.config
	cp $< $@

.PHONY: all clean clean_context distclean tup
