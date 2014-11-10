#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

RESULT sysfs_gpio_pin_config(int gpio_pin, SYSFS_COMMAND export_status)
{
	int export_fd = -1;
	char * export_str = "/sys/class/gpio/export";
	char * unexport_str = "/sys/class/gpio/unexport";
	char tmp_buf[1];

	switch(export_status)
	{
		case(EXPORT):
			export_fd = open(export_str, O_WRONLY);
			sprintf(tmp_buf, "%d", gpio_pin);
			write(export_fd, tmp_buf, sizeof(tmp_buf));
				close(export_fd);
			return SUCCESS;
		case(UNEXPORT):
			export_fd = open(unexport_str, O_WRONLY);
			sprintf(tmp_buf, "%d", gpio_pin);
			if(write(export_fd, tmp_buf, sizeof(tmp_buf)))
				close(export_fd);
			return SUCCESS;
		default: return FAIL;
	}
}

RESULT sysfs_gpio_pin_direction(int gpio_pin, SYSFS_DIRECTION direction)
{
	int direction_fd = -1;
	char direction_str[32];

	sprintf(direction_str, "/sys/class/gpio/gpio%d/direction", gpio_pin);
	direction_fd = open(direction_str, O_WRONLY);

	switch(direction)
	{
		case(INPUT):
			if(write(direction_fd, "in", 2))
				close(direction_fd);
			return SUCCESS;
		case(OUTPUT):
			if(write(direction_fd, "out", 3))
				close(direction_fd);
			return SUCCESS;
		default: return FAIL;
	}
	return SUCCESS;
}

RESULT sysfs_gpio_pin_setvalue(int gpio_pin, SYSFS_STATE state)
{
	int value_fd = -1;
	char value_str[35];

	sprintf(value_str, "/sys/class/gpio/gpio%d/value", gpio_pin);
	value_fd = open(value_str, O_WRONLY);

	switch(state)
	{
		case(LOW):
			if(write(value_fd, "0", 1))
				close(value_fd);
			return SUCCESS;
		case(HIGH):
			if(write(value_fd, "1", 1))
				close(value_fd);
			return SUCCESS;
		default: return FAIL;
	}
}

RESULT sysfs_gpio_port_export(unsigned int * gpio_port, SYSFS_COMMAND command)
{
	int i = 0;

	switch(command)
	{
		case(EXPORT):
			while(gpio_port[i] != TERMINATION)
			{
				if(!sysfs_gpio_pin_config(gpio_port[i], EXPORT))
					i++;
			}
			return SUCCESS;
		case(UNEXPORT):
			while(gpio_port[i] != TERMINATION)
			{
				if(!sysfs_gpio_pin_config(gpio_port[i], UNEXPORT))
					i++;
			}
			return SUCCESS;
		default: return FAIL;
	}
}

RESULT sysfs_gpio_jtag_setdirections(unsigned int * gpio_port)
{
	int i = 0;

	while(gpio_port[i] != TERMINATION)
	{
		if (gpio_port[i] == sysfsTDO){
			if(!sysfs_gpio_pin_direction(gpio_port[i], INPUT))
				i++;}
		else
			{if(!sysfs_gpio_pin_direction(gpio_port[i], OUTPUT))
				i++;}
	}
	return SUCCESS;
}

RESULT sysfs_gpio_port_value(DPUCHAR byte)
{
	if(evaluate_TCK(byte))
		sysfs_gpio_pin_setvalue(sysfsTCK, HIGH);
	else
		sysfs_gpio_pin_setvalue(sysfsTCK, LOW);

	if(evaluate_TDI(byte))
		sysfs_gpio_pin_setvalue(sysfsTDI, HIGH);
	else
		sysfs_gpio_pin_setvalue(sysfsTDI, LOW);

	if(evaluate_TMS(byte))
		sysfs_gpio_pin_setvalue(sysfsTMS, HIGH);
	else
		sysfs_gpio_pin_setvalue(sysfsTMS, LOW);

	return SUCCESS;
}
