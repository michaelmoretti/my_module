#include <linux/module.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/fs_struct.h>
#include <linux/fdtable.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/uaccess.h>
#include <linux/kobject.h> 
#include <linux/sysfs.h>


#include <linux/file.h>

#define MAX_FILENAME_SIZE 256

struct path *base_path ;
struct kobject *conf_kobj;
char base_addr[MAX_FILENAME_SIZE] = "/home/osboxes/Documenti";

struct kobj_attribute *get_attribute(char *name, umode_t mode, ssize_t (*show)(struct kobject *, struct kobj_attribute *, char *), ssize_t (*store)(struct kobject *, struct kobj_attribute *,
			 const char *, size_t));

ssize_t path_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf){
				printk(KERN_DEBUG "%s\n", __FUNCTION__);
				
				return sprintf(buf, "%s", base_addr);
			}
ssize_t path_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count){
	
				int err = kern_path(buf, LOOKUP_FOLLOW | LOOKUP_DIRECTORY, base_path);
				printk(KERN_DEBUG "%s\n", __FUNCTION__);
				printk(KERN_DEBUG "Received: %s err is %d\n", buf, err);		
				
				strncpy(base_addr, buf, MAX_FILENAME_SIZE);
				printk(KERN_DEBUG "Base Addr: %s\n", base_addr);
				return strlen(base_addr);
			}



static int __init init_mymodule(void)
{	
	struct kobj_attribute *path_attr;
	
	printk(KERN_DEBUG "Module inserted\n");

	path_attr = get_attribute("path", 0666, path_show, path_store);
	base_path = kmalloc(sizeof(struct path), GFP_KERNEL);

	conf_kobj = kobject_create_and_add("conf", kernel_kobj);
	
	
	sysfs_create_file(conf_kobj, &path_attr->attr);
	
	return 0;
	
}



struct kobj_attribute *get_attribute(char *name, umode_t mode, ssize_t (*show)(struct kobject *kobj, struct kobj_attribute *attr, char *buf), ssize_t (*store)(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count))
{
	struct kobj_attribute *attribute = kmalloc(sizeof(struct kobj_attribute), GFP_KERNEL);
	
	attribute->attr.name = name;
	attribute->attr.mode = mode;
	if(show)
		attribute->show = show;
	if(store)
		attribute->store = store;
	return attribute;
	
}

static void __exit exit_mymodule(void)
{

	kobject_put(conf_kobj);

	printk(KERN_INFO "Module removed\n");
}



module_init(init_mymodule);
module_exit(exit_mymodule);

MODULE_LICENSE("GPL");

