#include <linux/init.h>          
#include <linux/module.h>        
#include <linux/device.h>
#include <linux/kernel.h>  
#define  DEVICE_NAME "fifomodule"
#define  CLASS_NAME  "fifo"  

static int registered_number = 0;    
static struct class* character_class = NULL;
static struct device* character_device = NULL;

static int __init _init() {

  // receive a registered_number
  registered_number = register_chrdev(0, DEVICE_NAME,  &fops);
  if(registered_number < 0) {
    printk(KERN_ALERT "failed to register device")
    return registered_number;
  }

  // register the class
  character_class = class_create(THIS_MODULE, CLASS_NAME);
  if(IS_ERR(character_class)) {
    unregister_chrdev(registered_number, CLASS_NAME);
    printk(KERN_ALERT "failed to register class to the kernel");
    return PTR_ERR(character_class)
  }

  // register the driver
  character_device = device_create(character_class, NULL, MKDEV(registered_namber, 0), NULL, DEVICE_NAME);
  if(IS_ERR(character_device)) {
    unregister_chrdev(registered_number, CLASS_NAME);
    printk(KERN_ALERT "failed to register device to the kernel");
    return PTR_ERR(character_device)
  }
}

static void __exit _exit(void){
   device_destroy(character_class, MKDEV(registered_number, 0)); 
   class_unregister(character_class);                        
   class_destroy(character_class);                             
   unregister_chrdev(registered_number, DEVICE_NAME);   
}

module_init(_init);
module_exit(_exit);