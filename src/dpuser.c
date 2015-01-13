/****************************************************************************/
/*                                                                          */
/*  DirectC                 Copyright (C) Actel Corporation 2007            */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*  Module:         dpuser.c												*/
/*                                                                          */
/*  Description:    user specific file containing JTAG interface functions  */
/*                  and delay function                                      */
/*                                                                          */
/****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include "dpuser.h"
#include "dpalg.h"
#include "dputil.h"

/*
 * User attention:
 * Include files needed to support hardware JTAG interface operations.
 *
*/
#include "jtag.h"
/* This variable is used to select external programming vs IAP programming */
DPUCHAR hardware_interface = GPIO_SEL;
DPUCHAR enable_mss_support = FALSE;

/*
 * User attention:
 * jtag_port_reg:   8 bit Static variable to keep track of the state of all the JTAG pins
 *                  at all times during the programming operation.
 * Note: User check the variable size to make sure it can cover the hardware IO register.
 *
*/

static DPUCHAR jtag_port_reg;
extern int jtag_fd;
/*
 * User attention:
 * Module: jtag_inp
 *      purpose: report the logic state of tdo jtag pin
 * Arguments: None
 * Return value: 8 bit value
 *      0, 0x80
 *
*/
DPUCHAR jtag_inp(void)
{
    int tdo = -1;
    DPUCHAR ret = 0x80;

    ioctl(jtag_fd, JTAG_READ_TDO, &tdo);
    if(tdo == 1)
        ret = 0x80;
    else
        ret = 0;
   return ret;
}
/*
 * User attention:
 * Module: jtag_outp
 *      purpose: Set the JTAG port (all JTAG pins)
 * Arguments: 8 bit value containing the new state of all the JTAG pins
 * Return value: None
 *
*/
void jtag_outp(DPUCHAR outdata)
{
    /* User Specific Code */
    ioctl(jtag_fd, JTAG_WRITE_BYTE, (int)outdata);
}

/*
 * No need to change this function
 * Module: dp_jtag_init
 *      purpose: Set tck and trstb pins to logic level one
 * Arguments:
 *      None
 * Return value: None
 *
*/
void dp_jtag_init(void)
{
    jtag_port_reg = TCK | TRST;
    jtag_outp(jtag_port_reg);
}

/*
 * No need to change this function
 * Module: dp_jtag_tms
 *      purpose: Set tms pin to a logic level one or zero and pulse tck.
 * Arguments:
 *      tms: 8 bit value containing the new state of tms
 * Return value: None
 * Constraints: Since jtag_outp function sets all the jtag pins, jtag_port_reg is used
 *              to modify the required jtag pins and preseve the reset.
 *
*/
void dp_jtag_tms(DPUCHAR tms)
{
    jtag_port_reg &= ~(TMS | TCK);
    jtag_port_reg |= (tms ? TMS : 0);
    jtag_outp(jtag_port_reg);
    jtag_port_reg |= TCK;
    jtag_outp(jtag_port_reg);
}

/*
 * No need to change this function
 * Module: dp_jtag_tms_tdi
 *      purpose: Set tms amd tdi pins to a logic level one or zero and pulse tck.
 * Arguments:
 *      tms: 8 bit value containing the new state of tms
 *      tdi: 8 bit value containing the new state of tdi
 * Return value: None
 * Constraints: Since jtag_outp function sets all the jtag pins, jtag_port_reg is used
 *              to modify the required jtag pins and preseve the reset.
 *
*/
void dp_jtag_tms_tdi(DPUCHAR tms, DPUCHAR tdi)
{
    jtag_port_reg &= ~(TMS | TCK | TDI);
    jtag_port_reg |= ((tms ? TMS : 0) | (tdi ? TDI : 0));
    jtag_outp(jtag_port_reg);
    jtag_port_reg |= TCK;
    jtag_outp(jtag_port_reg);
}

/*
 * No need to change this function
 * Module: dp_jtag_tms_tdi_tdo
 *      purpose: Set tms amd tdi pins to a logic level one or zero,
 *               pulse tck and return tdi level
 * Arguments:
 *      tms: 8 bit value containing the new state of tms
 *      tdi: 8 bit value containing the new state of tdi
 * Return value:
 *      ret: 8 bit variable ontaining the state of tdo.
 * Valid return values:
 *      0x80: indicating a logic level high on tdo
 *      0: indicating a logic level zero on tdo
 * Constraints: Since jtag_outp function sets all the jtag pins, jtag_port_reg is used
 *              to modify the required jtag pins and preseve the reset.
 *
*/
DPUCHAR dp_jtag_tms_tdi_tdo(DPUCHAR tms, DPUCHAR tdi)
{
    DPUCHAR ret = 0x80;
    jtag_port_reg &= ~(TMS | TCK | TDI);
    jtag_port_reg |= ((tms ? TMS : 0) | (tdi ? TDI : 0));
    jtag_outp(jtag_port_reg);
    ret = jtag_inp() ;
    jtag_port_reg |= TCK;
    jtag_outp(jtag_port_reg);
    return ret;
}

char *array2str(unsigned char *value, int bytes, int descriptive)
{
    DPINT i;
    DPINT j;
    static DPCHAR str1[256];
    DPCHAR str2[10];
    for (i = 0 ; i < 256 ; i++) str1[i] = '\0';
    for (i = 0 ; i < bytes ; i++)
    {
        for (j = 0 ; j < 10 ; j++) str2[j] = '\0';
        switch(descriptive)
        {
            case HEX:
                sprintf(str2, "%X", (unsigned int)value[i]);
                break;
            case DEC:
                sprintf(str2, "%d", (int)value[i]);
                break;
            default:
                sprintf(str2, "%c", value[i]);
        }
        strcat(str1, str2);
    }
    return str1;
}

char *value2str(unsigned long value, int descriptive)
{
    DPINT i;
    static DPCHAR str1[256];
    for (i = 0 ; i < 256 ; i++) str1[i] = '\0';
    if (descriptive == HEX)
    {
        sprintf(str1, "%X", (unsigned int)value);
    }
    else
    {
        sprintf(str1, "%d", (int)value);
    }
    return str1;
}

/*
 * User attention:
 * Module: dp_delay
 *      purpose: Execute a time delay for a specified amount of time.
 * Arguments:
 *      microseconeds: 32 bit value containing the amount of wait time in microseconds.
  * Return value: None
 *
*/
void dp_delay(DPULONG microseconds)
{
//	microseconds *= 10000000;
    //while(microseconds--);
//	struct timespec req={0};
//	time_t sec = (int)(microseconds/1000000);
//	microseconds = microseconds - (sec*1000000);
//	req.tv_sec = sec;
//	req.tv_nsec = microseconds*1000L;
//	while(nanosleep(&req, &req) == -1)
//		continue;

}

#ifdef ENABLE_DEBUG
void dp_display_text(DPCHAR *text)
{
    /* User Specific Code    */
    fprintf(stdout, "%s", text);

}

void dp_display_value(DPULONG value,int descriptive)
{
    /* User Specific Code    */
    fprintf(stdout, "%s", value2str(value, descriptive));
}
void dp_display_array(DPUCHAR *value, int bytes, int descriptive)
{
    /* User Specific Code */
    fprintf(stdout, "%s", array2str(value, bytes, descriptive));
}
#endif
