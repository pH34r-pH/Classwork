#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "CircularBuffer.h"
#define DEVICE_NAME "bdreader"
#define CLASS_NAME "bdread"

MODULE_LICENSE("GPL");

static int registered_number = 0; // device number
extern CircularBuffer cBuffer;
static struct class *character_class = NULL;
static struct device *character_device = NULL;

static int bd_open(struct inode *, struct file *);
static ssize_t bd_read(struct file *, char *, size_t, loff_t *);
static int bd_release(struct inode *, struct file *);

static struct file_operations fops = {
	.open = bd_open,
	.read = bd_read,
	.release = bd_release,
};

// initialize the module
static int __init bd_init(void)
{
	printk(KERN_INFO "Initializing BufferDriverReader\n");
	// receive a registered_number
	registered_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (registered_number < 0)
	{
		printk(KERN_ALERT "Failed to register BufferDriverReader\n");
		return registered_number;
	}
	printk(KERN_INFO "Registered BufferDriverReader with number %d\n", registered_number);

	// register the class
	character_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(character_class))
	{
		unregister_chrdev(registered_number, CLASS_NAME);
		printk(KERN_ALERT "Failed to register BufferDriverReader class to the kernel\n");
		return PTR_ERR(character_class);
	}

	// register the driver
	character_device = device_create(character_class, NULL, MKDEV(registered_number, 0), NULL, DEVICE_NAME);
	if (IS_ERR(character_device))
	{
		unregister_chrdev(registered_number, CLASS_NAME);
		printk(KERN_ALERT "Failed to register BufferDriverReader to the kernel\n");
		return PTR_ERR(character_device);
	}

	printk(KERN_INFO "BufferDriverReader successfullys initialized\n");
	return 0;
}

// module exit function
static void __exit bd_exit(void)
{
	device_destroy(character_class, MKDEV(registered_number, 0));
	class_unregister(character_class);
	class_destroy(character_class);
	unregister_chrdev(registered_number, DEVICE_NAME);
	printk(KERN_INFO "Successfully shut down BufferDriverReader\n");
}

// device open
static int bd_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "BufferDriverReader %d opened\n", registered_number);
	return 0;
}

// device read - user wants data
static ssize_t bd_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	CircularBufferPointer bufferPointers[2];
	printk(KERN_INFO "Reading from BufferDriverReader\n");

	// Given the requested amount of data 'len', get as much data as there is in the buffer.
	CircularBufferGetBytes(min(len, CircularBufferCount(&cBuffer)), bufferPointers, &cBuffer);

	// Copy the first part of data to the user.
	copy_to_user(buffer, bufferPointers[0].bufferPointer, bufferPointers[0].length);

	// If there is a second part of data, copy that to the user as well.
	if (bufferPointers[1].length > 0)
	{
		copy_to_user(buffer + bufferPointers[0].length, 
			bufferPointers[1].bufferPointer, bufferPointers[1].length);
	}

	if (CircularBufferIsEmpty(&cBuffer))
	{
		printk(KERN_INFO "Buffer has been emptied.\n");
	}

	return bufferPointers[0].length + bufferPointers[1].length;
}

// close device
static int bd_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "BufferDriverReader %d closed\n", registered_number);
	return 0;
}

module_init(bd_init);
module_exit(bd_exit);
