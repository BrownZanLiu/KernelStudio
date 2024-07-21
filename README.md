Kernel module development and kernel usage exercises.

# system configurations
Load module: CONFIG_MODULES=y
Unload module: CONFIG_MODULE_UNLOAD=y
Unload module forcefully(Ignore reference count of module): MODULE_FORCE_UNLOAD=y

# module building
Related files:
- Kconfig, mainly used with in-kernel tree building for feature selections
- Kbuild or Makefile, makefile name of kbuild system of linux kernel
Kbuild has higher priority over Makefile.

