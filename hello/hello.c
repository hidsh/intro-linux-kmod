#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_INFO "Good bye...\n");
}

module_init(hello_init);
module_exit(hello_exit);
