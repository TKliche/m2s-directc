/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpalg.c                                                 */
/*                                                                          */
/*  Description:    Contains initialization and data checking functions.    */
/*                                                                          */
/* ************************************************************************ */


#include "dpuser.h"
#include "dputil.h"
#include "dpalg.h"
#include "dpG3alg.h"
#include "dpG4alg.h"
#include "dpjtag.h"


DPUCHAR Action_code; /* used to hold the action codes as defined in dpalg.h */
DPUCHAR Action_done; /* used to hold the action codes as defined in dpalg.h */
DPUCHAR opcode; /* Holds the opcode value of the IR register prior to loading */

DPULONG device_ID;  /* Holds the device ID */
DPUCHAR device_rev; /* Holds the device revision */
DPUCHAR device_family = 0U;    /* Read from the data file AFS, or G3 */
DPUINT device_bsr_bit_length; /* Holds the bit length of the BSR register */

/* DataIndex variable is used to keep track of the position of the data 
* loaded in the file 
*/
DPULONG DataIndex;   

/* error_code holds the error code that could be set in the programming 
* functions 
*/
DPUCHAR error_code; 


DPUCHAR dp_top (void)
{
	#ifdef ENABLE_DISPLAY
	dp_display_text("\r\nIdentifying device...");
	#endif

	goto_jtag_state(JTAG_TEST_LOGIC_RESET,0u);
	error_code = DPE_CODE_NOT_ENABLED;
    Action_done = FALSE;
	
	#ifdef ENABLE_G3_SUPPORT
	error_code = DPE_SUCCESS;
	dp_read_idcode();
	dp_check_device_ID();
	if (error_code == DPE_SUCCESS)
	{
		dp_top_g3();
        Action_done = TRUE;
	}	
	#endif

	#ifdef ENABLE_G4_SUPPORT
    if (Action_done == FALSE)
    {
	    error_code = DPE_SUCCESS;
	    dp_read_idcode();
	    dp_check_G4_device_ID();
	    if (error_code == DPE_SUCCESS)
	    {
		    dp_top_g4 ();
            Action_done = TRUE;
	    }
    }
	#endif

	return error_code;
}

void dp_read_idcode(void)
{
	opcode = IDCODE;
    IRSCAN_in();
    goto_jtag_state(JTAG_RUN_TEST_IDLE,0u);
    DRSCAN_out(IDCODE_LENGTH, (DPUCHAR*)DPNULL, global_buf1);
    device_ID = (DPULONG)global_buf1[0] | (DPULONG)global_buf1[1] << 8u | 
    (DPULONG)global_buf1[2] << 16u | (DPULONG)global_buf1[3] << 24u;
    device_rev = (DPUCHAR) (device_ID >> 28);
    
    #ifdef ENABLE_DISPLAY
	dp_display_text("\r\nActID = ");
	dp_display_value(device_ID,HEX);
    #endif
    
	return;
}

#ifdef ENABLE_DISPLAY
void dp_read_idcode_action(void)
{
    return;
}
#endif
