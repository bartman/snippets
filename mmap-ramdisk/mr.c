#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/atomic.h>

struct mr_vma_priv {
	atomic_t refcnt;
	spinlock_t lock;
	int count, max;
	unsigned long vm_start;
	unsigned long vm_end;
	struct page *pages[0];
};


static void mr_vm_open(struct vm_area_struct * vma)
{
	struct mr_vma_priv *vp;
	vp = vma->vm_private_data;
	atomic_inc(&vp->refcnt);
}

static void mr_vm_close(struct vm_area_struct * vma)
{
	int i;
	struct mr_vma_priv *vp;
	vp = vma->vm_private_data;

	pr_info("close: refcnt=%u\n", atomic_read(&vp->refcnt));

	if (!atomic_dec_and_test(&vp->refcnt))
		return;

	for(i=0; i<vp->max; i++) {
		struct page *page;
		page = vp->pages[i];

		if (!page)
			continue;

		vp->pages[i] = NULL;
		
#if 0
		ClearPageReserved(page);
#endif
		pr_info("mr: freeing page=%p\n", page);
		__free_page(page);
		vp->count --;
	}

	pr_info("close: free\n");

	vfree(vp);
}

static int mr_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct mr_vma_priv *vp;
	struct page *page;
	unsigned long index;

	vp = vma->vm_private_data;

	pr_info("fault: fault pfoff=%lu\n", vmf->pgoff);

	index = vmf->pgoff;

	pr_info("fault: fault index=%lu/%u\n", index, vp->max);
	if (index >= vp->max)
		return VM_FAULT_SIGBUS;

	page = vp->pages[index];
	pr_info("fault: have page=%p\n", page);
	if (!page) {
		page = alloc_page(GFP_KERNEL);
		if (!page)
			return VM_FAULT_OOM;

		spin_lock(&vp->lock);
		if (!vp->pages[index]) {
			vp->pages[index] = page;
			vp->count ++;
		} else {
			__free_page(page);
			page = vp->pages[index];
		}
		spin_unlock(&vp->lock);
	}
	pr_info("fault: using page=%p\n", page);

	//pfn = paddr >> PAGE_SHIFT;
	//vm_insert_pfn(vma, (unsigned long)vmf->virtual_address, pfn);

#if 0
	SetPageReserved(page);
#endif

	get_page(page);
	vmf->page = page;

	return 0;
}

#if 0
static int mr_vm_access(struct vm_area_struct *vma, unsigned long addr,
		void *buf, int len, int write)
{
	return 0;
}
#endif








static struct vm_operations_struct mr_vmops = {
	.open   = mr_vm_open,
	.close  = mr_vm_close,
	.fault  = mr_vm_fault,
#if 0
	.access = mr_vm_access,
#endif
};


static int mr_mmap(struct file *filp, struct vm_area_struct *vma)
{
	struct mr_vma_priv *vp;
	unsigned long size = vma->vm_end - vma->vm_start;
	unsigned pages, vp_size;

	pr_info("mmap: vma: start=%lx end=%lx size=%lu pgoff=%lx\n",
			vma->vm_start,
			vma->vm_end,
			size,
			vma->vm_pgoff);

	if (vma->vm_pgoff) {
		pr_err("memory region assigned\n");
		return -EINVAL;
	}

	if (size & ~PAGE_MASK) {
		pr_err("size not aligned: %ld\n", size);
		return -ENXIO;
	}

	/* we only support shared mappings. Copy on write mappings are
	   rejected here. A shared mapping that is writeable must have the
	   shared flag set.
	   */
	if ((vma->vm_flags & VM_WRITE) && !(vma->vm_flags & VM_SHARED)) {
		pr_err("writeable mappings must be shared, rejecting\n");
		return -EINVAL;
	}

	pages = size >> PAGE_SHIFT;
	vp_size = sizeof(*vp) + pages * sizeof(vp->pages[0]);

	pr_info("mr: new context for pages=%u\n", pages);

	vp = vzalloc(vp_size);
	if (!vp) {
		pr_err("failed to allocate %u bytes\n", vp_size);
		return -ENOMEM;
	}

	vp->vm_start = vma->vm_start;
	vp->vm_end = vma->vm_end;
	vp->max = pages;

	spin_lock_init(&vp->lock);
	atomic_set(&vp->refcnt, 1);

	vma->vm_private_data = vp;
#if 0
	vma->vm_flags |= VM_IO
		     //| VM_LOCKED
		     //| VM_PFNMAP
		       | VM_RESERVED
		       | VM_DONTEXPAND
		       ;
#endif

	vma->vm_ops = &mr_vmops;

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
	pr_info("mr: init\n");

	if (misc_register(&mr_misc)) {
		pr_err("unable to get major for mr module\n");
		return -EBUSY;
	}

	return 0;
}

static void __exit mr_exit(void)
{
	pr_info("mr: exit\n");

	misc_deregister(&mr_misc);
}

module_init(mr_init);
module_exit(mr_exit);
