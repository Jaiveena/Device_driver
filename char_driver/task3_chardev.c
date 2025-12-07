#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Madhan");
MODULE_DESCRIPTION("Task 3 - Simple Char Device Driver");

#define DEVICE_NAME "task3char"
#define BUFFER_SIZE 1024

static int major;
static char device_buffer[BUFFER_SIZE];

/* -------------------- File Operations -------------------- */

static int dev_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "task3: Device opened\n");
    return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "task3: Device closed\n");
    return 0;
}

static ssize_t dev_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    int bytes_read;

    bytes_read = simple_read_from_buffer(user_buf, count, ppos, device_buffer, BUFFER_SIZE);
    printk(KERN_INFO "task3: Read %d bytes\n", bytes_read);
    return bytes_read;
}

static ssize_t dev_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    int bytes_written;

    bytes_written = simple_write_to_buffer(device_buffer, BUFFER_SIZE, ppos, user_buf, count);
    printk(KERN_INFO "task3: Wrote %d bytes\n", bytes_written);
    return bytes_written;
}

/* File operations structure */
static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = dev_open,
    .release = dev_release,
    .read = dev_read,
    .write = dev_write,
};

/* -------------------- Module Init & Exit -------------------- */

static int __init task3_init(void)
{
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "task3: Failed to register char device\n");
        return major;
    }

    printk(KERN_INFO "task3: Registered with major number %d\n", major);
    printk(KERN_INFO "task3: Create device using: sudo mknod /dev/%s c %d 0\n", DEVICE_NAME, major);

    return 0;
}

static void __exit task3_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "task3: Device unregistered\n");
}

module_init(task3_init);
module_exit(task3_exit);

