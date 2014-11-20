#include <linux/ioctl.h>

#define DEVICE_NAME "jtag"

#define IOC_MAGIC 61

// Definition of ioctl operations
#define TCK_SWITCH _IO(IOC_MAGIC, 1)
#define JTAG_READ_TDO _IOR(IOC_MAGIC, 2, int *)
#define JTAG_WRITE_BYTE _IOW(IOC_MAGIC, 3, int)
