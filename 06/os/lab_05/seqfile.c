#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/vmalloc.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aleksey Knyazhev");

#define DIRNAME "seqfiles"
#define FILENAME "seqfile"
#define SYMLINK "seqfile_link"
#define FILEPATH DIRNAME "/" FILENAME

#define BUF_SIZE PAGE_SIZE

static struct proc_dir_entry *seqfile_file = NULL;
static struct proc_dir_entry *seqfile_dir = NULL;
static struct proc_dir_entry *seqfile_link = NULL;

static char *cookie_buffer = NULL;  
static int index = 0;  

ssize_t custom_write(struct file *file, const char __user *buf, size_t len, loff_t *offp) 
{
	printk(KERN_INFO "+ seqfile: called write");
	if(len > BUF_SIZE - index + 1) 
	{
		printk(KERN_ERR "+ seqfile: cookie_buffer overflow");
		return -ENOSPC;
	}
	if(copy_from_user(&cookie_buffer[index], buf, len)) 
	{
		printk(KERN_ERR "+ seqfile: copy from user error");
		return -EFAULT;
	}
	index += len;
	cookie_buffer[index-1] = '\n';
	return len;
}

static int seqfile_show(struct seq_file *m, void *v) 
{
	printk(KERN_INFO "+ seqfile: called show");
	seq_printf(m, "%s%c", cookie_buffer, -1);
	return 0;
} 

ssize_t seqfile_read(struct file *file, char __user *buf, size_t count, loff_t *offp) 
{
	printk(KERN_INFO "+ seqfile: called read");
	return seq_read(file, buf, count, offp);
}

int seqfile_open(struct inode *inode, struct file *file) 
{
	printk(KERN_INFO "+ seqfile: called open");
	return single_open(file, seqfile_show, NULL);
}

int seqfile_release(struct inode *inode, struct file *file) 
{
	printk(KERN_INFO "+ seqfile: called release");
	return single_release(inode, file);
}

static struct proc_ops fops = {
	.proc_open = seqfile_open,
	.proc_read = seqfile_read,
	.proc_write = custom_write,
	.proc_release = seqfile_release, 
};

void freemem(void) 
{
	if (cookie_buffer)
		vfree(cookie_buffer);
	if (seqfile_link)
		remove_proc_entry(SYMLINK, NULL);
	if (seqfile_file)
		remove_proc_entry(FILENAME, seqfile_dir);
	if (seqfile_dir)
		remove_proc_entry(DIRNAME, NULL);
}

static int __init init_seqfile_module(void) 
{	
	if (!(cookie_buffer = vmalloc(BUF_SIZE))) 
	{
		freemem();
		printk(KERN_ERR "+ seqfile: error during vmalloc");
		return -ENOMEM;
	}
	memset(cookie_buffer, 0, BUF_SIZE);
	if (!(seqfile_dir = proc_mkdir(DIRNAME, NULL))) 
	{
		freemem();
		printk(KERN_ERR "+ seqfile: error during directory creation");
		return -ENOMEM;
	} 
	else if (!(seqfile_file = proc_create(FILENAME, 0666, seqfile_dir, &fops))) 
	{
		freemem();
		printk(KERN_ERR "+ seqfile: error during file creation");
		return -ENOMEM;
	} 
	else if (!(seqfile_link = proc_symlink(SYMLINK, NULL, FILEPATH))) 
	{
		freemem();
		printk(KERN_ERR "+ seqfile: error during symlink creation");
		return -ENOMEM;
	}
	index = 0;
	printk(KERN_INFO "+ module loaded");
	return 0;
}

static void __exit exit_seqfile_module(void) 
{
	freemem();
	printk(KERN_INFO "+ seqfile: unloaded");
}

module_init(init_seqfile_module);
module_exit(exit_seqfile_module);
