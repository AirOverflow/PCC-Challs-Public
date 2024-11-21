#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>

#include "flag.h"

MODULE_DESCRIPTION("TheFlash2k's very useful driver...");
MODULE_AUTHOR("TheFlash2k");
MODULE_LICENSE("Ashfaq Licensing Service.");

static int __init init(void){
   printk(KERN_ALERT "Hi! You're not looking deep enough, I think...\n");
   return 0;
}

static void __exit leave(void){
   printk(KERN_DEBUG "Well, divide and conquer I guess...\n");
}

module_init(init);
module_exit(leave);
