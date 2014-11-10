#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "dpuser.h"

//#define _BSD_SOURCE
#define _XOPEN_SOURCE 500

#define sysfsTDI 5 // P14 pin 5
#define sysfsTDO 6 // P14 pin 6
#define sysfsTCK 3 // P14 pin 3
#define sysfsTMS 4 // P14 pin 4
#define TERMINATION 555

#define evaluate_TDI(byte) byte & 0x1
#define evaluate_TMS(byte) byte & 0x4
#define evaluate_TDO(byte) byte & 0x2
#define evaluate_TCK(byte) byte & 0x8

#define SYSFS_MASK

typedef enum {UNEXPORT = 0, EXPORT = 1} SYSFS_COMMAND;
typedef enum {INPUT = 0, OUTPUT = 1} SYSFS_DIRECTION;
typedef enum {LOW = 0, HIGH = 1} SYSFS_STATE;
typedef enum {SUCCESS = 0, FAIL = 1} RESULT;

RESULT sysfs_gpio_pin_config(int gpio_pin, SYSFS_COMMAND export_status);
RESULT sysfs_gpio_pin_direction(int gpio_pin, SYSFS_DIRECTION direction);
RESULT sysfs_gpio_pin_setvalue(int gpio_pin, SYSFS_STATE state);

RESULT sysfs_gpio_port_export(unsigned int * gpio_port, SYSFS_COMMAND command);
RESULT sysfs_gpio_jtag_setdirections(unsigned int * gpio_port);
RESULT sysfs_gpio_port_value(DPUCHAR byte);

#endif /* GPIO_H */
