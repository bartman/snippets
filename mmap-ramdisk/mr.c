#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/mm.h>
#include <linux/atomic.h>
#include <linux/writeback.h>
#include <linux/backing-dev.h>

#define USE_INSERT_PFN

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
		pr_info("mr: freeing page=%p "
			"Ref=%d Act=%d Drt=%u Rsr=%d U2d=%d Err=%d\n",
			page,
			PageReferenced(page),
			PageActive(page),
			PageDirty(page),
			PageReserved(page),
			PageUptodate(page),
			PageError(page)
			);
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
#ifndef USE_INSERT_PFN
	unsigned long pfn;
#endif

	vp = vma->vm_private_data;

	pr_info("fault: vma=%p file=%p as=%p\n", vma,
			vma->vm_file,
			vma->vm_file ? vma->vm_file->f_mapping : NULL);

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

#if 0
	SetPageReserved(page);
#endif

	get_page(page);

#ifndef USE_INSERT_PFN
	pfn = page_to_pfn(page);
	vm_insert_pfn(vma, (unsigned long)vmf->virtual_address, pfn);
	return VM_FAULT_NOPAGE;
#else
	vmf->page = page;
	return 0;
#endif
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

	pr_info("mmap: new context for pages=%u\n", pages);

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
	vma->vm_flags |= VM_IO;           // uses vmops->access()
	vma->vm_flags |= VM_LOCKED;       // pages are mlock()'ed
	vma->vm_flags |= VM_PFNMAP;       // linear mapping ptr -> PFN
	vma->vm_flags |= VM_RESERVED;     // unevictable, kernel allocated
	vma->vm_flags |= VM_DONTEXPAND;   // no mremap(), kernel allocated
#endif

	vma->vm_ops = &mr_vmops;

	pr_info("mmap: vm_file=%p\n", vma->vm_file);

	return 0;
}

static int mr_writepages(struct address_space *mapping,
		struct writeback_control *wbc)
{
	pr_info("writepages: mapping=%p nr=%lu skip=%lu range=%llx:%llx sync=%d\n",
			mapping, wbc->nr_to_write, wbc->pages_skipped,
			wbc->range_start, wbc->range_end, wbc->sync_mode);
	return 0;
}

static int mr_writepage(struct page *page, struct writeback_control *wbc)
{
	pr_info("writepage: page=%p nr=%lu skip=%lu range=%llx:%llx sync=%d\n",
			page, wbc->nr_to_write, wbc->pages_skipped,
			wbc->range_start, wbc->range_end, wbc->sync_mode);
	return 0;
}

static int mr_readpage(struct file *filp, struct page *page)
{
	pr_info("readpage: page=%p\n", page);
	return 0;
}

static int mr_write_begin(struct file *file, struct address_space *mapping,
		loff_t pos, unsigned len, unsigned flags,
		struct page **pagep, void **fsdata)
{
	pr_info("write_begin: mapping=%p pos=%llx len=%u fl=%u "
		"page=%p fsdata=%p\n",
		mapping, pos, len, flags, pagep ? *pagep : NULL,
		fsdata ? *fsdata : NULL);
	return 0;
}

static int mr_write_end(struct file *file, struct address_space *mapping,
		loff_t pos, unsigned len, unsigned copied,
		struct page *page, void *fsdata)
{
	pr_info("write_end: mapping=%p pos=%llx len=%u copied=%u "
		"page=%p fsdata=%p\n",
		mapping, pos, len, copied, page, fsdata);
	return 0;
}

static int mr_set_page_dirty(struct page *page)
{
	int rc;

	rc = __set_page_dirty_nobuffers(page);

	return rc;
}

static struct backing_dev_info mr_bdi = {
	.capabilities   = (BDI_CAP_MAP_COPY | BDI_CAP_MAP_DIRECT |
			   BDI_CAP_EXEC_MAP | BDI_CAP_READ_MAP |
			   BDI_CAP_WRITE_MAP),
};


static struct address_space_operations mr_asops = {
	.readpage = mr_readpage,
	.writepage = mr_writepage,
	.writepages = mr_writepages,
	.write_begin = mr_write_begin,
	.write_end = mr_write_end,
	.set_page_dirty = mr_set_page_dirty,
};

static int mr_open(struct inode *inode, struct file *filp)
{
	int rc;

	rc = nonseekable_open(inode, filp);
	if (rc)
		return rc;

	pr_info("open: inode=%p i_mapping=%p i_data=%p\n",
			inode, inode->i_mapping, &inode->i_data);

	pr_info("open: mapping->a_ops=%p data->a_ops=%p\n",
			inode->i_mapping
			? inode->i_mapping->a_ops : NULL,
			inode->i_data.a_ops);

	if (inode->i_mapping && inode->i_mapping->a_ops) {
		pr_info("open: mapping->a_ops writepage=%p\n",
				inode->i_mapping->a_ops->writepage);
	}

	if (inode->i_data.a_ops) {
		pr_info("open: data->a_ops writepage=%p\n",
				inode->i_data.a_ops->writepage);
	}

	inode->i_mapping->a_ops =
		inode->i_data.a_ops = &mr_asops;

	// this might race, do I need to lock? wee bdev_inode_switch_bdi()
	inode->i_data.backing_dev_info = &mr_bdi;

	return rc;
}

static int mr_fsync(struct file *filp, loff_t start, loff_t end, int datasync)
{
	int rc;
	struct inode *inode = filp->f_mapping->host;
#if 0
	struct writeback_control wbc = {
		.sync_mode = WB_SYNC_ALL,
		.nr_to_write = 0,       /* sys_fsync did this */
	};
	if (!inode)
		return -EINVAL;
	rc = sync_inode(inode, &wbc);
#elif 0
	if (!inode)
		return -EINVAL;

	pr_info("fsync: file=%p f->f_m=%p i->i_m=%p range=%llx:%llx\n",
			filp, filp->f_mapping, inode->i_mapping,
			start, end);

	pr_info("fsync: i_mapping->nrpages=%lu\n", inode->i_mapping->nrpages);
	pr_info("fsync: i_data.a_ops->nrpages=%lu\n", inode->i_data.nrpages);

	rc = filemap_write_and_wait_range(inode->i_mapping, start, end);
	pr_info("fsync: rc=%d\n", rc);
	if (!rc) {
		mutex_lock(&inode->i_mutex);
		rc = sync_inode_metadata(inode, 1);
		mutex_unlock(&inode->i_mutex);
	}
#else
	struct address_space *im = inode->i_mapping;
	if (!inode)
		return -EINVAL;

	pr_info("fsync: file=%p f->f_m=%p i->i_m=%p range=%llx:%llx\n",
			filp, filp->f_mapping, im,
			start, end);

	pr_info("fsync: i_mapping->nrpages=%lu\n", im->nrpages);
	pr_info("fsync: i_data.a_ops->nrpages=%lu\n", inode->i_data.nrpages);

	pr_info("fsync: bdi=%p cap=%d\n",
			im->backing_dev_info,
			mapping_cap_writeback_dirty(im));

	pr_info("fsync: writepages=%p, writepage=%p\n",
			im->a_ops->writepages,
			im->a_ops->writepage);

	rc = filemap_fdatawrite_range(im, start, end);
	pr_info("fsync: rc=%d\n", rc);
	if (rc != -EIO) {
		int rc2 = filemap_fdatawait_range(im, start, end);
		pr_info("fsync: rc2=%d\n", rc2);
		if (!rc)
			rc = rc2;
	}

	if (!rc) {
		mutex_lock(&inode->i_mutex);
		rc = sync_inode_metadata(inode, 1);
		mutex_unlock(&inode->i_mutex);
	}
#endif
	return rc;
}

static ssize_t mr_sendpage(struct file *filp, struct page *page, int offset,
		size_t size, loff_t *ppos, int more)
{
	pr_info("sendpage: page=%p ofs=%u size=%lu pos=%llu more=%u\n",
			page, offset, size, *ppos, more);

	return size;
}

ssize_t mr_write(struct file *filp, const char __user *buf, size_t size,
		loff_t *ppos)
{
	pr_info("write: buf=%p size=%lu pos=%llu\n",
			buf, size, *ppos);

	return size;
}

static const struct file_operations mr_fops = {
	.owner = THIS_MODULE,
	.open  = mr_open,
	.fsync = mr_fsync,
	.mmap  = mr_mmap,
	.sendpage = mr_sendpage,
	.write = mr_write,
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
