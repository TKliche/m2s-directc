#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "jtag.h"

int main(int argc, char *argv[])
{
    int fd;
    int ret;
    int k = 0;
    unsigned char m[10] = {9, 5, 9, 5, 9, 5, 9, 5, 9, 5};
	int i = -1;
    
    fd = open("/dev/jtag", O_RDWR);
    if(fd < 0)
    {
        fprintf(stdout, "Can't get file descriptor\n");
        exit(-1);
    } else ret = ioctl(fd, JTAG_READ_TDO, &i);
    close(fd);

    fprintf(stdout, "ret value is equal to %d\n", ret);
    fprintf(stdout, "readed parameter is equal to %d \n", i);

    fd = open("/dev/jtag", O_RDWR);
    for(; k < 10; k++){
        ioctl(fd, JTAG_WRITE_BYTE, m[k]);
    }
    close(fd);
    return 0;
}
