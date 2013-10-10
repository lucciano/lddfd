
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static int major;

static ssize_t yoda_read_superb(struct file *file, char __user *buf,
				size_t count, loff_t *offset)
{
	const char *yoda_string = "Elegant ways, Linux has.\n";

	pr_info("user wants to read %d bytes at %lld\n", count, *offset);

	return simple_read_from_buffer(buf, count, offset,
				       yoda_string, strlen(yoda_string));
}

static ssize_t yoda_read_works(struct file *file, char __user *buf,
			       size_t count, loff_t *offset)
{
	const char *yoda_string = "Completely fixed, this string is.\n";
	size_t to_read, yoda_size = strlen(yoda_string);

	pr_info("user wants to read %d bytes at %lld\n", count, *offset);

	/* Little hack to detect non-first readings */
	if (*offset > 0)
		return 0;

	to_read = count < yoda_size ? count : yoda_size;

	if (copy_to_user(buf, yoda_string, to_read))
		return -EFAULT;

	/* Update offset */
	*offset += to_read;

        return to_read;
}

static ssize_t yoda_read_buggy(struct file *file, char __user *buf,
			       size_t count, loff_t *offset)
{
	const char *yoda_string = "Completely fixed, this string is.\n";

	pr_info("user wants to read %d bytes at %lld\n", count, *offset);

	if (copy_to_user(buf, yoda_string, strlen(yoda_string)))
		return -EFAULT;
        return strlen(yoda_string);
}

static struct file_operations fops = {
	.read = yoda_read_works,
};

static int yoda_init(void)
{
        major = register_chrdev(0, KBUILD_MODNAME, &fops);
        if (major < 0)
                return major;
	pr_info("device registered with major %d\n", major);

        return 0;
}

static void yoda_exit(void)
{
        return unregister_chrdev(major, KBUILD_MODNAME);
}

module_init(yoda_init);
module_exit(yoda_exit);
