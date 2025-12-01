# obj-<X> := <module_name>.o
# If <X> is set to y, the obj-y variable is used and <module_name>.o will be built as part of the
# the kernel.
# If <X> is set to m, the obj-m variable is ueed and <module_name>.o wiil be built as a loadable
# kernel module.
# If <X> is not set, the obj- variable is used and <module_name>.o will no be built at all.
# Kernel configuration option CONFIG-XXX is used as obj-$(CONFIG-XXX).
# Check current kernel configurations with cmd menuconfig or see /boot/config-$(uname -r)

### module: hellokernel ###
obj-m := hellokernel.o
# If the module has only one source file and the soure file has the same name as module name, obj-m
# statement is enough and it won't work to use <module_name>-y.
# If the module has no more than one .c file, use <module_name>-y to specify source file list.
# With <module_name>-y, use .o instead of .c. You must has no .c file that has the same name as
# module name. Otherwise, you will encounter some strange building issues.
hellokernel-y :=  hellokernel_main.o hellokernel_ops.o
# ccflags-y := -I $(src)/include
# ccflags-y += -I $(src)/src/hal/include
# ldflags-y := -T $(src)/foo_sections.lds

### module: zanfs ###
#obj-m += zanfs.o
#obj-$(CONFIG_ZANFS) += zanfs.o  # Could be configured by /boot/config-$(uname -r)

### module: kernel checker###
obj-m += kexplorer.o
kexplorer-y := kernel_explorer_main.o

### modules under some subdir ###
# Go into some_subdir and look for any Kbuild or Makefile to do work recursively.
# obj-<X> += some_subdir/

KERNEL_SRC ?= /lib/modules/$(shell uname -r)/build

all default: modules
install: modules_install

modules modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(shell pwd) $@
clean:
	rm -f ./*.o
	rm -f ./*.mod
	rm -f ./*.mod.c
	rm -f ./*.mod.o
	rm -f ./*.ko
	rm -f ./.*.cmd
	rm -f ./modules.order
	rm -f ./Module.symvers
