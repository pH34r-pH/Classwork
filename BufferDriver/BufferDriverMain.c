#include <linux/init.h>          
#include <linux/module.h>        
#include <linux/device.h>
#include <linux/kernel.h> 
#include <linux/fs.h>
#include <linux/uaccess.h> 
#include "CircularBuffer.h"
#define  DEVICE_NAME "bdmodule"
#define  CLASS_NAME  "bd"

MODULE_LICENSE("GPL");

static int registered_number = 0; // device number 
static char message[CIRCULAR_BUFFER_CAPACITY_BYTES] = {}; // current message
static short size_of_message; // size of current message
static CircularBuffer cBuffer;
static struct class* character_class = NULL; 
static struct device* character_device = NULL;

static int bd_open(struct inode *, struct file *);
static ssize_t bd_read(struct file *, char *, size_t, loff_t *);
static ssize_t bd_write(struct file *, const char *, size_t, loff_t *);
static int bd_release(struct inode *, struct file *);

static struct file_operations fops = {
  .open = bd_open,
  .read = bd_read,
  .write = bd_write,
  .release = bd_release,
};

// initialize the module
static int __init bd_init(void) {
  printk(KERN_INFO "initializing BufferDriver\n");
  // receive a registered_number
  registered_number = register_chrdev(0, DEVICE_NAME,  &fops);
  if(registered_number < 0) {
    printk(KERN_ALERT "failed to register device\n");
    return registered_number;
  }
  sprintf(message, "%d", registered_number);
  printk(KERN_INFO "Registered device with number %s\n", message);

  // register the class
  character_class = class_create(THIS_MODULE, CLASS_NAME);
  if(IS_ERR(character_class)) {
    unregister_chrdev(registered_number, CLASS_NAME);
    printk(KERN_ALERT "failed to register class to the kernel\n");
    return PTR_ERR(character_class);
  }

  // register the driver
  character_device = device_create(character_class, NULL, MKDEV(registered_number, 0), NULL, DEVICE_NAME);
  if(IS_ERR(character_device)) {
    unregister_chrdev(registered_number, CLASS_NAME);
    printk(KERN_ALERT "failed to register device to the kernel\n");
    return PTR_ERR(character_device);
  }
  
  //Set up the circular buffer
  CircularBufferClear(&cBuffer);
  
  printk(KERN_INFO "successful initialization\n");
  return 0;
}

// module exit function
static void __exit bd_exit(void){
   device_destroy(character_class, MKDEV(registered_number, 0)); 
   class_unregister(character_class);                        
   class_destroy(character_class);                             
   unregister_chrdev(registered_number, DEVICE_NAME);   
   printk(KERN_INFO "successfully shut down\n");
}

// device open
static int bd_open(struct inode *inodep, struct file *filep) {
  printk(KERN_INFO "Device %d opened\n", registered_number);
  return 0;
}

// device read - user wants data
static ssize_t bd_read(struct file *filep, char *buffer, size_t len, loff_t *offset) {
  printk(KERN_INFO "Reading from device\n");
  int i = 0;
  for(; i < len; i++)
  {
    if(CircularBufferIsEmpty(&cBuffer)) {
      printk(KERN_INFO "Buffer has been emptied.\n");
      break;
    }
    message[i] = CircularBufferGetByte(&cBuffer);
  }

  printk(KERN_INFO "message sent to user: %s", message);
  copy_to_user(buffer, message, len);
  return i;
}

// write to buffer
static ssize_t bd_write(struct file *filep, const char *buffer, size_t len, loff_t *offset) {
  // read in message to buffer
  printk(KERN_INFO "Writing to device\n");
  int i = 0;
  for(; i < len; i++)
  {
    if(CircularBufferIsFull(&cBuffer)) {
      printk(KERN_INFO "Buffer has been filled, and can no longer be written to.\n");
      break;
    }

    CircularBufferAddByte(buffer[i], &cBuffer);
  }

  return i;
}

// close device
static int bd_release(struct inode *inodep, struct file *filep) {
  printk(KERN_INFO "Device %d closed\n", registered_number);
  return 0;
}

module_init(bd_init);
module_exit(bd_exit);
