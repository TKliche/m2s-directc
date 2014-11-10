#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#
TOOLCHAIN_PATH = /home/romanjoe/dev/linux-cortexm-1.12.0/tools/arm-2010q1/bin
CC_PREFIX = arm-uclinuxeabi-

# define the C compiler to use
CC = $(TOOLCHAIN_PATH)/$(CC_PREFIX)gcc

#define module targets
MODULE = ioctl-jtag
obj-m   := $(MODULE).o

# define user space application targets
APP1 = m2s-gpio
APP2 = us

all:	modules m2s-gpio us
clean:	clean_modules clean_apps

# define any compile-time flags
CFLAGS = -mcpu=cortex-m3 -mthumb -std=c99 -v -g3 -Wall -O3

# define linker flags
LDLAGS = -mcpu=cortex-m3 -mthumb
# Path to the kernel modules directory in context of whichhh
# these loadable modules are built
KERNELDIR	= $(INSTALL_ROOT)/linux

# define any directories containing header files other than /usr/include
#
INCLUDES	= 	-I./ \
				-I/home/romanjoe/dev/linux-cortexm-1.12.0/tools/arm-2010q1/arm-uclinuxeabi/include \
				-I/home/romanjoe/dev/linux-cortexm-1.12.0/tools/arm-2010q1/arm-uclinuxeabi/libc/usr/include \
				-I/home/romanjoe/dev/linux-cortexm-1.12.0/tools/arm-2010q1/lib/gcc/arm-uclinuxeabi/4.4.1/include \
				-I/home/romanjoe/dev/linux-cortexm-1.12.0/tools/arm-2010q1/lib/gcc/arm-uclinuxeabi/4.4.1/include-fixed

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lmylib -lm

# define the C source files
# user space application source files
MAIN-APP-SRCS =  main.c 		\
		gpio.c 		\
		dpalg.c		\
		dpcom.c 	\
		dpuser.c 	\
		dputil.c 	\
		dpchain.c 	\
		dpjtag.c 	\
		dpG4alg.c	\
		dpG3alg.c	\
		dpcore.c	\
		dpfrom.c	\
		dpnvm.c		\
		dpsecurity.c

TEST-APP-SRCS = us.c

#define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
MAIN-APP-OBJS = $(MAIN-APP-SRCS:.c=.o)

TEST-APP-OBJS = $(TEST-APP-SRCS:.c=.o)
#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

m2s-gpio: 	$(MAIN-APP-OBJS)
			$(CC) $(CFLAGS) $(INCLUDES) -o $(APP1) $(MAIN-APP-OBJS) $(LDLAGS) $(LIBS)

us: 		$(TEST-APP-OBJ)
			$(CC) $(CFLAGS) $(INCLUDES) -o $(APP2) $(TEST-APP-OBJ) $(LDLAGS) $(LIBS)

modules:
		echo "make modules CFLAGS="" LDFLAGS="""
		make -C $(KERNELDIR) M=`pwd` modules CFLAGS="" LDFLAGS=""


# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

# Clean-up after user space progs
clean_apps:
		rm -rf *.o *~ $(APP1) $(APP2) *.gdb

# Clean-up after modules
clean_modules:
		make -C $(KERNELDIR) M=`pwd` clean
		rm -f modules.order

depend: $(MAIN-APP-OBJ)
		make depend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it
