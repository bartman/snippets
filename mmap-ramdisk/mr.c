#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/mm.h>

static int mr_mmap(struct file *filp, struct vm_area_struct *vma)
{
	if (remap_pfn_range(vma, vma->vm_start, vma->vm_pgoff,
			vma->vm_end - vma->vm_start, vma->vm_page_prot)) {
		return -EAGAIN;
	}
	return 0;
}

static const struct file_operations mr_fops = {
	.owner = THIS_MODULE,
	.open  = nonseekable_open,
	.mmap  = mr_mmap,
};

static struct miscdevice mr_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = "mr",
	.fops  = &mr_fops,
};

static int __init mr_init(void)
{
	printk(KERN_INFO "mr: init\n");

	if (misc_register(&mr_misc)) {
		printk("unable to get major for mr module\n");
		return -EBUSY;
	}

	return 0;
}

static void __exit mr_exit(void)
{
	printk(KERN_INFO "mr: exit\n");

	misc_deregister(&mr_misc);
}

module_init(mr_init);
module_exit(mr_exit);
