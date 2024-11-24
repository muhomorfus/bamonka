#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kprobes.h>
#include <linux/ktime.h>
#include <linux/limits.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <linux/kstrtox.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Aleksey Knyazhev");

#define SYSCALL_OPEN 0
#define SYSCALL_READ 1
#define SYSCALL_WRITE 2
#define SYSCALL_READ_STDIN 3
#define SYSCALL_WRITE_STDOUT 4

#define SYSCALL_NUM 5
#define PROCESS_NUM 4194304

#define MAX_SYMBOL_LEN	64

static const char symbol_open[MAX_SYMBOL_LEN] = "do_sys_openat2";
static const char symbol_read[MAX_SYMBOL_LEN] = "ksys_read";
static const char symbol_write[MAX_SYMBOL_LEN] = "ksys_write";

module_param_string(symbol_open, symbol_open, MAX_SYMBOL_LEN, 0644);
module_param_string(symbol_read, symbol_read, MAX_SYMBOL_LEN, 0644);
module_param_string(symbol_write, symbol_write, MAX_SYMBOL_LEN, 0644);

struct syscall_data_t {
    ktime_t entry_timestamp;
    int syscall_num;
    int pid;
};

struct syscall_info_t {
    long long sum_duration;
    int count;
    long long max_duration;
};

struct process_info_t {
    int exist;
    struct syscall_info_t syscalls[SYSCALL_NUM];
};

#define DIRNAME "informer"
#define FILENAME "syscalls"

static struct proc_dir_entry *seqfile_file = NULL;
static struct proc_dir_entry *seqfile_dir = NULL;
int output_pid = 1;

struct process_info_t processes[PROCESS_NUM] = { 0 };

static int open_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct syscall_data_t *data;
    if (!current->mm)
        return 1; // Skip kernel threads
    data = (struct syscall_data_t *) ri->data;
    data->entry_timestamp = ktime_get();
    data->syscall_num = SYSCALL_OPEN;
    data->pid = current->pid;
    return 0;
}

static int read_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct syscall_data_t *data;
    if (!current->mm)
        return 1; // Skip kernel threads
    data = (struct syscall_data_t *) ri->data;
    data->entry_timestamp = ktime_get();
    data->syscall_num = SYSCALL_READ;
    if (regs->user_regs.regs[0] == 0)
        data->syscall_num = SYSCALL_READ_STDIN;
    data->pid = current->pid;
    return 0;
}

static int write_entry_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    struct syscall_data_t *data;
    if (!current->mm)
        return 1; // Skip kernel threads
    data = (struct syscall_data_t *) ri->data;
    data->entry_timestamp = ktime_get();
    data->syscall_num = SYSCALL_WRITE;
    if (regs->user_regs.regs[0] == 1)
        data->syscall_num = SYSCALL_WRITE_STDOUT;
    data->pid = current->pid;
    return 0;
}

static int return_handler(struct kretprobe_instance *ri, struct pt_regs *regs)
{
    s64 duration;
    ktime_t now;
    struct syscall_info_t info;
    struct syscall_data_t *data = (struct syscall_data_t *) ri->data;
    now = ktime_get();
    duration = ktime_to_ns(ktime_sub(now, data->entry_timestamp));
    if (!processes[data->pid].exist)
        processes[data->pid].exist = 1;
    info = processes[data->pid].syscalls[data->syscall_num];
    info.count++;
    info.sum_duration += duration;
    if (info.max_duration < duration)
        info.max_duration = duration;
    processes[data->pid].syscalls[data->syscall_num] = info;
    return 0;
}

static struct kretprobe kretprobes[3] = {
    {
        .handler		= return_handler,
        .entry_handler	= open_entry_handler,
        .data_size		= sizeof(struct syscall_data_t),
        .maxactive		= 64,
    },
    {
        .handler		= return_handler,
        .entry_handler	= read_entry_handler,
        .data_size		= sizeof(struct syscall_data_t),
        .maxactive		= 64,
    },
    {
        .handler		= return_handler,
        .entry_handler	= write_entry_handler,
        .data_size		= sizeof(struct syscall_data_t),
        .maxactive		= 64,
    }
};

ssize_t custom_write(struct file *file, const char __user *buf, size_t len, loff_t *offp)
{
    printk(KERN_INFO "informer: called write");
    if(kstrtoint_from_user(buf, len, 10, &output_pid))
    {
        printk(KERN_ERR "informer: read int from user error");
        return -1;
    }
    printk(KERN_INFO "informer: saved pid %d to output", output_pid);
    return len;
}

void show_syscall(struct seq_file *m, struct syscall_info_t info)
{
    seq_printf(m, "\tmax_duration = %lld ns\n", info.max_duration);
    seq_printf(m, "\tavg_duration = %lld ns\n", (info.sum_duration - info.max_duration) / (info.count - 1));
    seq_printf(m, "\tcount = %d\n", info.count);
}

static int seqfile_show(struct seq_file *m, void *v)
{
    printk(KERN_INFO "informer: called show");
    if (!processes[output_pid].exist) {
        seq_printf(m, "No information about PID=%d.\n", output_pid);
        seq_printf(m, "You can change PID via writing it to /proc/syscalls file.\n");
        return 0;
    }
    seq_printf(m, "System calls statistics of PID=%d.\n", output_pid);
    seq_printf(m, "\nopen\n");
    show_syscall(m, processes[output_pid].syscalls[SYSCALL_OPEN]);
    seq_printf(m, "\nread\n");
    show_syscall(m, processes[output_pid].syscalls[SYSCALL_READ]);
    seq_printf(m, "\nwrite\n");
    show_syscall(m, processes[output_pid].syscalls[SYSCALL_WRITE]);
    seq_printf(m, "\nread from STDIN\n");
    show_syscall(m, processes[output_pid].syscalls[SYSCALL_READ_STDIN]);
    seq_printf(m, "\nwrite to STDOUT\n");
    show_syscall(m, processes[output_pid].syscalls[SYSCALL_WRITE_STDOUT]);
    return 0;
}

ssize_t seqfile_read(struct file *file, char __user *buf, size_t count, loff_t *offp)
{
    printk(KERN_INFO "informer: called read");
    return seq_read(file, buf, count, offp);
}

int seqfile_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "informer: called open");
    return single_open(file, seqfile_show, NULL);
}

int seqfile_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "informer: called release");
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
    int i;
    for (i = 0; i < 3; i++)
    {
        unregister_kretprobe(&(kretprobes[i]));
        printk(KERN_INFO "informer: unregistered kretprobe for %s", kretprobes[i].kp.symbol_name);
    }
    if (seqfile_file)
        remove_proc_entry(FILENAME, seqfile_dir);
    if (seqfile_dir)
        remove_proc_entry(DIRNAME, NULL);
}

static int __init informer_init(void)
{
    int rc;
    int i;
    kretprobes[SYSCALL_OPEN].kp.symbol_name = symbol_open;
    kretprobes[SYSCALL_READ].kp.symbol_name = symbol_read;
    kretprobes[SYSCALL_WRITE].kp.symbol_name = symbol_write;
    for (i = 0; i < 3; i++)
    {
        rc = register_kretprobe(&(kretprobes[i]));
        if (rc < 0) {
            printk(KERN_ERR "informer: register_kretprobe failed, returned %d", rc);
            i--;
            while (i >= 0)
            {
                unregister_kretprobe(&(kretprobes[i]));
                i--;
            }
            return -1;
        }
        printk(KERN_INFO "informer: registered return probe at %s: %p", kretprobes[i].kp.symbol_name, kretprobes[i].kp.addr);
    }
    if (!(seqfile_dir = proc_mkdir(DIRNAME, NULL)))
    {
        freemem();
        printk(KERN_ERR "informer: error during directory creation");
        return -1;
    }
    if (!(seqfile_file = proc_create(FILENAME, 0666, seqfile_dir, &fops)))
    {
        freemem();
        printk(KERN_ERR "informer: error during file creation");
        return -1;
    }
    printk(KERN_INFO "informer: module loaded");
    return 0;
}

static void __exit informer_exit(void)
{
    freemem();
    printk(KERN_INFO "informer: module unloaded");
}

module_init(informer_init)
module_exit(informer_exit)
