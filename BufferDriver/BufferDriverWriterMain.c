#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "CircularBuffer.h"
#define DEVICE_NAME "bdwriter"
#define CLASS_NAME "bdwrite"

MODULE_LICENSE("GPL");

static int registered_number = 0; // device number
static CircularBuffer cBuffer;
static struct class *character_class = NULL;
static struct device *character_device = NULL;

static int bd_open(struct inode *, struct file *);
static ssize_t bd_write(struct file *, const char *, size_t, loff_t *);
static int bd_release(struct inode *, struct file *);

static struct file_operations fops = {
	.open = bd_open,
	.write = bd_write,
	.release = bd_release,
};

EXPORT_SYMBOL(cBuffer);

// initialize the module
static int __init bd_init(void)
{
	printk(KERN_INFO "Initializing BufferDriverWriter\n");
	// receive a registered_number
	registered_number = register_chrdev(0, DEVICE_NAME, &fops);
	if (registered_number < 0)
	{
		printk(KERN_ALERT "Failed to register BufferDriverWriter\n");
		return registered_number;
	}
	printk(KERN_INFO "Registered BufferDriverWriter with number %d\n", registered_number);

	// register the class
	character_class = class_create(THIS_MODULE, CLASS_NAME);
	if (IS_ERR(character_class))
	{
		unregister_chrdev(registered_number, CLASS_NAME);
		printk(KERN_ALERT "Failed to register BufferDriverWriter class to the kernel\n");
		return PTR_ERR(character_class);
	}

	// register the driver
	character_device = device_create(character_class, NULL, MKDEV(registered_number, 0), NULL, DEVICE_NAME);
	if (IS_ERR(character_device))
	{
		unregister_chrdev(registered_number, CLASS_NAME);
		printk(KERN_ALERT "Failed to register BufferDriverWriter to the kernel\n");
		return PTR_ERR(character_device);
	}

	//Set up the circular buffer
	CircularBufferClear(&cBuffer);

	printk(KERN_INFO "BufferDriverWriter successfully initialized\n");
	return 0;
}

// module exit function
static void __exit bd_exit(void)
{
	device_destroy(character_class, MKDEV(registered_number, 0));

	class_unregister(character_class);
	class_destroy(character_class);
	unregister_chrdev(registered_number, DEVICE_NAME);
	printk(KERN_INFO "successfully shut down BufferDriverWriter\n");
}

// device open
static int bd_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "BufferDriverWriter %d opened\n", registered_number);
	return 0;
}

// write to buffer
static ssize_t bd_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	int i = 0;
	
	char* ucfReplaceMessage = "Undefeated 2018 National Champions UCF";
	
	// read in message to buffer
	printk(KERN_INFO "Writing to BufferDriverWriter\n");

	for (; i < len; ++i)
	{
		if (CircularBufferIsFull(&cBuffer))
		{
			printk(KERN_INFO "Buffer has been filled, and can no longer be written to.\n");
			break;
		}
		
		// Special input filter section
		if(i < len-2 && buffer[i] == 'U' && buffer[i+1] == 'C' && buffer[i+2] == 'F'){
			int j = 0;
			for(; j < 38; ++j){
				if (CircularBufferIsFull(&cBuffer))
				{
					printk(KERN_INFO "Buffer has been filled, and can no longer be written to.\n");
					break;
				}
				CircularBufferAddByte(ucfReplaceMessage[j], &cBuffer);
			}
			i += 2;
		}
		
		CircularBufferAddByte(buffer[i], &cBuffer);
	}

	return len;
}

// close device
static int bd_release(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "BufferDriverWriter %d closed\n", registered_number);
	return 0;
}

module_init(bd_init);
module_exit(bd_exit);
