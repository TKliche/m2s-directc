#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "dpuser.h"
#include "dpalg.h"

int jtag_fd = -1;
//int write_byte_fd = -1;

int main(int argc, char *argv[])
{
	FILE * dat_file;
	size_t dat_file_size = 0;
	char * dat_file_name = argv[1];
	char * dat_file_action = argv[2];

//	unsigned int jtag_pins[5] = {sysfsTDI,
//								sysfsTDO,
//								sysfsTCK,
//								sysfsTMS,
//								TERMINATION
//								};

	if(strcmp(dat_file_action, "-PROGRAM") == 0) Action_code = 5;
	else if(strcmp(dat_file_action, "-ERASE") == 0) Action_code = 3;
	else if(strcmp(dat_file_action, "-IDCODE") == 0) Action_code = 2;
	else if(strcmp(dat_file_action, "-DEVINFO") == 0) Action_code = 1;
	else
	{
		fprintf(stdout,
				"[ERROR] Please, define proper action value -PROGRAM\n-ERASE\n-IDCODE\n-DEVINFO");
		exit(1);
	}

	dat_file = fopen(dat_file_name, "r");

	struct stat results;
	if(stat(dat_file_name, &results) == 0)
	{
		dat_file_size = results.st_size;
		dat_file_size += 100000;
		image_buffer = (DPUCHAR *)malloc(dat_file_size + 1);
		memset(image_buffer, 0, sizeof(image_buffer));
		fread(image_buffer, sizeof(char), dat_file_size, dat_file);

		fclose(dat_file);
	}

	jtag_fd = open("/dev/jtag", O_RDWR);
//	tdo_fd = open("/sys/class/jtag_ports/jtag/tdo", O_RDONLY);
//	write_byte_fd = open("/sys/class/jtag_ports/jtag/write_byte", O_WRONLY);
//	if(!sysfs_gpio_port_export(jtag_pins, EXPORT))
//	{
//		if(!sysfs_gpio_jtag_setdirections(jtag_pins))
	dp_top();
//	}
	fprintf(stdout, "\nReturned error code = %d \n", error_code);
	free(image_buffer);

	return 0;
}
