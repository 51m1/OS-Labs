#include <linux/init.h>
#include <linux/module.h>
//#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/utsname.h>

void uptime(void);
void getSystemInfo(void);

struct Student{
	int ID;
	char name[50];
	int age;
};

struct Student Students[30];
int stpoint = 0;

void add(struct Student s){
	if(stpoint < 30){
		Students[stpoint] = s;
		stpoint++;
	}
	else{
		printk("No more students can be accepted\n");
	}
}

void list(void){
	int i;
	for(i = 0; i<stpoint; i++){
		printk("%s\n",Students[i].name);
	}
}

/* This function is called when the module is loaded. */
int simple_init(void)
{
	struct Student dad;
	struct Student ded;
	struct Student did;
	uptime();
	getSystemInfo();

	dad.ID = 1;
	strcpy(dad.name,"Pepe");
	dad.age = 16;

	ded.ID = 2;
	strcpy(ded.name,"Papa");
	ded.age = 27;

	did.ID = 3;
	strcpy(did.name,"Pipo");
	did.age = 11;

	add(dad);
	add(ded);
	add(did);

	list();

        printk(KERN_INFO "Loading Chava Module\n");

        return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Chava Module\n");
}

void uptime(){
       printk("Uptime: %i\n", jiffies_to_msecs(get_jiffies_64())/1000);
}

void getSystemInfo(){
       struct new_utsname *buf;
       buf = utsname();
       printk("Nodename: %s\n", buf->nodename);
       printk("Machine: %s\n", buf->machine);
       printk("Sysname: %s\n", buf->sysname);
       printk("Release: %s\n", buf->release);
       printk("Version: %s\n", buf->version);
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

