#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h> // msleep(), msleep_interruptible(), etc

#include "hellokernel_ops.h"

static int __init hellokernel_init(void)
{
	pr_info("Hello kernel. I'm initializer.\n");
	msleep_interruptible(3000);
	pr_info("3 + 7 = %d\n", hellokernel_add(3, 7));
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
