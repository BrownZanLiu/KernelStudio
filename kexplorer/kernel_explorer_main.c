#include <linux/init.h>  // __init, __exit
#include <linux/kernel.h>
#include <linux/module.h>  // MODULE_*(), module_init(), module_exit()

MODULE_AUTHOR("Brown LiuZan<brown.liuzan@outlook.com");
MODULE_DESCRIPTION("Utility to check kernel status.");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");
MODULE_INFO(commands, "No utilities for now.");

static __init int kernel_checker_init(void)
{
	pr_info("Hello, LKM.\n");

	return 0;
}

static __exit void kernel_checker_exit(void)
{
	pr_info("Bye, LKM.\n");
}

module_init(kernel_checker_init);
module_exit(kernel_checker_exit);
