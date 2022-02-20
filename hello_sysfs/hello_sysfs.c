#include <linux/module.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

MODULE_LICENSE("Dual BSD/GPL");

static int value;	// 書込み用の変数
					// /sys/module/<module name>/fooattrs/value に書き込まれた
					// 値が store_value()によって、ここに代入される

// コールバック関数
// /sys/module/<module name>/fooattrs/name をユーザが読んだときにコールされる
static ssize_t show_name(struct kobject *kobj, struct kobj_attribute *attr,
						char *buf)
{
	printk(KERN_INFO "hello_sysfs: user read 'name'\n");

	return scnprintf(buf, PAGE_SIZE, "sysfs-module\n");
}

// コールバック関数
// /sys/module/<module name>/fooattrs/value をユーザが読んだときにコールされる
static ssize_t show_value(struct kobject *kobj, struct kobj_attribute *attr,
						  char *buf)
{
	printk(KERN_INFO "hello_sysfs: user read 'value'\n");

	return scnprintf(buf, PAGE_SIZE, "%d\n", value);	// valueの値を書き込む
	// show関数は末尾のnull文字以外のバイト数を返す
}

// コールバック関数
// /sys/module/<module name>/fooattrs/value にユーザが書き込んだときにコールされる
static ssize_t store_value(struct kobject *kobj, struct kobj_attribute *attr,
						  const char *buf, size_t count)
{
	int res;

	printk(KERN_INFO "hello_sysfs: user write 'value'\n");
   
	res	= kstrtoint(buf, 10, &value);	// bufに文字列として入っているので
											// 10進と解釈してintに変換したものを
											// グローバル変数のvalueに入れる
	if(res < 0) {
		return res;		// エラー時はエラーコードを返す
	}
	return count;		// 成功時はcountを返す
}

static struct kobject *hello_kobj;

static struct kobj_attribute name_attribute  = 
	__ATTR(name,  0444, show_name,  NULL);
static struct kobj_attribute value_attribute =
	__ATTR(value, 0664, show_value, store_value);

static struct attribute *attrs[] = {
	&name_attribute.attr,
	&value_attribute.attr,
	NULL
};
static struct attribute_group attribute_group = {
	.attrs = attrs
};

// insmod時にコールされる
static int hellosysfs_init(void)
{
	int ret;

	printk(KERN_INFO "hello sysfs module\n");

	hello_kobj = kobject_create_and_add("fooattrs", &THIS_MODULE->mkobj.kobj);
	if(!hello_kobj) {
		return -ENOMEM;		// abend
	}

	ret = sysfs_create_group(hello_kobj, &attribute_group);
	if(ret) {
		kobject_put(hello_kobj);
	}

	return ret;
}

// rmmod時にコールされる
static void hellosysfs_exit(void)
{
	printk(KERN_INFO "exiting hello sysfs module...\n");

	// hello_kobj の参照カウントを減らす
	kobject_put(hello_kobj);
}

module_init(hellosysfs_init);
module_exit(hellosysfs_exit);
