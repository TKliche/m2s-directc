/* NOTE
 * Followind driver is using ioctl interface to implement
 * new system calls to interact with hardware. It is valid
 * only for kernels under 2.6.36. Implementation for newer
 * kernels is described here
 * http://tuxthink.blogspot.com/2012/12/implementing-ioctl-call-for-kernel.html
 */
#include <linux/init.h> 	/* Every module must include */
#include <linux/module.h>	/* Every module must include */
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/cdev.h>
#include <asm/system.h>
#include <asm/uaccess.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/ioctl.h>
#include <linux/delay.h>

#include "jtag.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Okhrimenko <mrromanjoe@gmail.com>");

/*Declarations of functions*/

int jtag_init(void);
void jtag_exit(void);

/* TODO: implement below functions */
int jtag_open(struct inode * inode, struct file * filp);
int jtag_release(struct inode * inode, struct file * filp);
ssize_t jtag_read(struct file * filp, char * buf, size_t count, loff_t * f_pos);
ssize_t jtag_write(struct file * filp, const char * buf, size_t count, loff_t * f_pos);

/* User interface to kernel module */
int jtag_ioctl(struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg);

/* TODO:
   Declare user defined functions
*/
//void tck_switch(struct file * filp);

void jtag_write_byte(struct file * fipl, unsigned long arg);
void jtag_read_tdo(struct file * filp, int * arg);

//File operations
struct file_operations jtag_fops = {
        .owner = THIS_MODULE,
        .read = jtag_read,
        .write = jtag_write,
        .ioctl = jtag_ioctl,
        .open = jtag_open,
        .release = jtag_release,
};

/* Global variables for jtag driver */
#define MMAP_OFFSET 0x40013000 // base adress of gpios in processor mem map
#define MMAP_SIZE	(0x40013FFF-MMAP_OFFSET) // gpio address space size
#define GPIO_OUT	0x40013088
#define GPIO_IN		0x40013084

#define MMAP_BB_OFFSET 0x42000000
#define MMAP_BB_SIZE (0x43FFFFFF-MMAP_BB_OFFSET)

#define TCK			3 // GPIO3, pin 3 on P14 header of Emcraft SOM SM2
#define TMS			4 // GPIO4, pin 4 on P14
#define TDI			5 // GPIO5, pin 5 on P14
#define TDO			6 // GPIO6, pin 6 on P14


struct resource * gpio_bb;
static void * base_addr;
static void * base_bb_addr;
//static void * gpio_out_addr;
//static void * gpio_in_addr;

static void * tck_config_addr;
static void * tms_config_addr;
static void * tdi_config_addr;
static void * tdo_config_addr;

static void * tck;
static void * tms;
static void * tdi;
static void * tdo;
/*Magic number of device in Linux device system */

static int jtag_major_n;

int jtag_open(struct inode * inode, struct file * filp)
{
    return 0;
}
int jtag_release(struct inode * inode, struct file * filp)
{
    return 0;
}

ssize_t jtag_read(struct file * filp, char * buf, size_t count, loff_t * f_pos)
{
    return 1;
}
ssize_t jtag_write(struct file * filp, const char * buf, size_t count, loff_t * f_pos)
{
    return 1;
}

static void jtag_gpio_cfg(void)
{
    gpio_bb = request_mem_region(MMAP_BB_OFFSET, MMAP_BB_SIZE, "gpio_bb");

    base_addr = ioremap(MMAP_OFFSET, MMAP_SIZE);
    base_bb_addr = ioremap(MMAP_BB_OFFSET, MMAP_BB_SIZE);

    tck = (unsigned int *) ( (unsigned int)base_bb_addr | (0x00013088 << 5) | (3 << 2));
    tms = (unsigned int *) ( (unsigned int)base_bb_addr | (0x00013088 << 5) | (4 << 2));
    tdi = (unsigned int *) ( (unsigned int)base_bb_addr | (0x00013088 << 5) | (5 << 2));
    tdo = (unsigned int *) ( (unsigned int)base_bb_addr | (0x00013084 << 5) | (6 << 2));
    printk(KERN_INFO "+++ Maped adresses base_gpio = %p \n", base_addr);
    printk(KERN_INFO "+++ Maped adresses base_bb = %p \n", base_bb_addr);
    printk(KERN_INFO "+++ Maped adresses tck = %p \n", tck);
    printk(KERN_INFO "+++ Maped adresses tms = %p \n", tms);
    printk(KERN_INFO "+++ Maped adresses tdi = %p \n", tdi);
    printk(KERN_INFO "+++ Maped adresses tdo = %p \n", tdo);

    tck_config_addr = base_addr + (TCK * 4); //(0x4001300C - MMAP_OFFSET);
    tms_config_addr = base_addr + (TMS * 4);
    tdi_config_addr = base_addr + (TDI * 4);
    tdo_config_addr = base_addr + (TDO * 4);
    // config GPIO pin 3 for output set 1 to first bit of config reg
    *((volatile unsigned int *) tck_config_addr) |= (1 << 0);
    *((volatile unsigned int *) tms_config_addr) |= (1 << 0);
    *((volatile unsigned int *) tdi_config_addr) |= (1 << 0);
    *((volatile unsigned int *) tdo_config_addr) |= (1 << 1);

    printk(KERN_INFO "+++ Initialized\n");
}

void jtag_write_byte(struct file * fipl, unsigned long arg)
{
/*!!!  insertion of this this line highly slow toggling frequency */
//    printk(KERN_INFO "Transfered argument is %d\n", (int)arg);

    if (arg & 0x01)
        *((volatile unsigned int *) tdi) = 1;
    else
        *((volatile unsigned int *) tdi) = 0;
    if (arg & 0x04)
        *((volatile unsigned int *) tms) = 1;
    else
        *((volatile unsigned int *) tms) = 0;
    if (arg & 0x08)
        *((volatile unsigned int *) tck) = 1;
    else
        *((volatile unsigned int *) tck) = 0;
}

void jtag_read_tdo(struct file * filp, int * arg)
{
//    printk(KERN_INFO "+++ Inside jtag_read_tdo\n");
//    printk(KERN_INFO "+++ gpio_in_value = %p\n", ((volatile unsigned int *)gpio_in_addr) );
//    printk(KERN_INFO "+++ value of register gpio_in_value = %d\n", *((volatile unsigned int *)gpio_in_addr) );
    if ( *((volatile unsigned long *) tdo) == 1 )
        *arg = 1;
    else
        *arg = 0;
}

/* void tck_switch(struct file * filp) */
/* { */
/*     long i; */
/*     for (i = 0; i < 20000; i++) */
/*     { */
/*         *((volatile unsigned int *)gpio_out_addr) |= (1 << 4); */
/* //        ndelay(5); */
/*         *((volatile unsigned int *)gpio_out_addr) &= ~(1 << 4); */
/* //        ndelay(5); */
/*     } */
/* } */

int jtag_ioctl(struct inode * inode, struct file * filp, unsigned int cmd, unsigned long arg)
{
    static int * tdo;
    int * temp;

    switch (cmd)
    {
    /* case TCK_SWITCH: */
    /*     tck_switch(filp); */
    /*     break; */
    case JTAG_WRITE_BYTE:
        jtag_write_byte(filp, arg);
        break;
    case JTAG_READ_TDO:
        temp = (int *)arg;
        jtag_read_tdo(filp, tdo);
        put_user(*tdo, temp);
        break;
    default:
        return -ENOTTY;
    }
    return 1;
}

struct cdev *jtag_dev;

int __init jtag_init(void)
{
    int opret;
    dev_t dev_no, dev;

    dev = MKDEV(jtag_major_n, 0);
    jtag_dev = cdev_alloc();
    jtag_dev->ops = &jtag_fops;
    jtag_dev->owner = THIS_MODULE;
    printk(KERN_INFO "+++ Initialization of JTAG module\n");
    opret = alloc_chrdev_region(&dev_no, 0, 1, DEVICE_NAME);
    if(opret < 0)
    {
        printk(KERN_INFO "+++ Can't assign major number to jtag\n");
        return opret;
    }
    jtag_major_n = MAJOR(dev_no);
    dev = MKDEV(jtag_major_n, 0);
    printk(KERN_INFO "+++ Major number assigned to JTAG is %d\n"
                        "+++ Do not forget to create device, type: "
                        "mknod /dev/jtag c %d 0\n", jtag_major_n,
               jtag_major_n);
    opret = cdev_add(jtag_dev, dev, 1);
    if(opret < 0)
    {
        printk(KERN_INFO "+++ Can't allocate jtag cdev in kernel\n");
        return opret;
    }
    jtag_gpio_cfg();
    return 0;
}

void __exit jtag_exit(void)
{
    /* unmap memory space allocated for gpios */
    iounmap(base_bb_addr);
    release_mem_region(MMAP_BB_OFFSET, MMAP_BB_SIZE);
    /* unregister char device from system */
    cdev_del(jtag_dev);
    unregister_chrdev_region(jtag_major_n, 0);
    printk(KERN_INFO "+++ JTAG device removed, do not forget "
                "to remove device file, type: rm /dev/jtag\n");
}

/*module init and exit macroses*/
module_init(jtag_init);
module_exit(jtag_exit);
