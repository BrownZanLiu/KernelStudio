#include <linux/module.h>
#include <linux/init.h>

static int __init hellokernel_init(void)
{
	pr_info("Hello kernel. I'm initializer.\n");
	return 0;
}

static void __exit hellokernel_exit(void)
{
	pr_info("Goodbye kernel. I'm finisher.\n");
}

module_init(hellokernel_init);
module_exit(hellokernel_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brown LiuZan<brown.liuzan@outlook.com");
MODULE_DESCRIPTION("Linux kernel module demo.");
MODULE_INFO(my_field_name, "This is my special module field.");
