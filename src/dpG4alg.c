/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpG4alg.c                                               */
/*                                                                          */
/*  Description:    Contains initialization and data checking functions.    */
/*                                                                          */
/* ************************************************************************ */



#include "dpuser.h"
#ifdef ENABLE_G4_SUPPORT
#include "dputil.h"
#include "dpcom.h"
#include "dpalg.h"
#include "dpG4alg.h"
#include "dpjtag.h"

DPUCHAR pgmmode;
DPUCHAR pgmmode_flag;
DPUCHAR shared_buf[48]; // Maximum of 768
DPUCHAR poll_buf[17];
DPULONG poll_index;

DPUCHAR security_queried;

/****************************************************************************
* Purpose: main entry function                                              
*  This function needs to be called from the main application function with 
*  the approppriate action code set to intiate the desired action.
****************************************************************************/
DPUCHAR dp_top_g4 (void) 
{
	dp_init_vars();
	dp_init_G4_vars();
	goto_jtag_state(JTAG_TEST_LOGIC_RESET,0u);
	dp_check_G4_action();
	if (error_code == DPE_SUCCESS)
	{
		dp_perform_G4_action();
	}

    return error_code;
}

void dp_init_G4_vars(void)
{
	pgmmode = 0u;
	pgmmode_flag = FALSE;
	security_queried = FALSE;
	return;
}

void dp_check_G4_action(void)
{
	if (device_rev < 0u) // was <2u
	{
		#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nDirectC does not support engineering sample devices.");
		#endif
		error_code = DPE_ACTION_NOT_SUPPORTED;
	}

	if ( (Action_code == DP_READ_IDCODE_ACTION_CODE) || (Action_code == DP_DEVICE_INFO_ACTION_CODE) )
	{
		#ifndef ENABLE_DISPLAY
        error_code = DPE_CODE_NOT_ENABLED;
        #endif 
	}
	else if (! (
		(Action_code == DP_ERASE_ACTION_CODE) ||
		(Action_code == DP_PROGRAM_ACTION_CODE) ||
		(Action_code == DP_VERIFY_ACTION_CODE) ||
		(Action_code == DP_ENC_DATA_AUTHENTICATION_ACTION_CODE)
		))
	{
		error_code = DPE_ACTION_NOT_SUPPORTED;
		#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nInvalid action.");
		#endif
	}
	dp_get_data(G4M_VerifyDataStream_ID,0u);
	if (return_bytes != 0u)
	{
		error_code = DPE_ACTION_NOT_SUPPORTED;
#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nSecurity operations are not supported with DirectC.");
#endif
	}

	return;
}

void dp_perform_G4_action (void)
{
    #ifdef ENABLE_DISPLAY
    if (Action_code == DP_READ_IDCODE_ACTION_CODE)
    {
        dp_read_idcode_action();
        Action_done = TRUE;
    }
	else if (Action_code == DP_DEVICE_INFO_ACTION_CODE)
	{
		dp_G4M_device_info_action();
		Action_done = TRUE;
	}
    #endif
	if (Action_done == FALSE)
	{
		dp_check_image_crc();
		if (error_code == DPE_SUCCESS)
		{
			dp_check_G4_device_ID();
			if (error_code == DPE_SUCCESS)
			{
				switch (Action_code) 
                {
					case DP_ERASE_SECURITY_ACTION_CODE: 
						dp_G4M_erase_security_action();
                       break;         
					case DP_ERASE_ACTION_CODE: 
                        dp_G4M_erase_action();
                        break;         
                    case DP_PROGRAM_ACTION_CODE: 
                        dp_G4M_program_action();
                        break;
                    case DP_VERIFY_ACTION_CODE: 
                        dp_G4M_verify_action();
                        break;
					case DP_ENC_DATA_AUTHENTICATION_ACTION_CODE: 
						dp_G4M_enc_data_authentication_action();
                        break;
				}
			}
		}
	}
	dp_G4M_exit();
	return;
}

void dp_G4M_erase_security_action(void)
{
	return;
}


void dp_G4M_erase_action(void)
{
	dp_G4M_initialize();
	if (error_code == DPE_SUCCESS)
	{
		pgmmode = 0x1u;
		dp_set_mode();

		if (error_code == DPE_SUCCESS)
		{
			/* Global unit1 is used to hold the number of components */
			global_uint1 = (DPUINT)dp_get_bytes(Header_ID,G4M_NUMOFCOMPONENT_OFFSET,G4M_NUMOFCOMPONENT_BYTE_LENGTH);
			global_uint2 = global_uint1 - ((DPUINT)dp_get_bytes(Header_ID,G4M_ERASEDATASIZE_OFFSET,G4M_DATASIZE_BYTE_LENGTH) - 1u);

			dp_G4M_process_data(G4M_erasedatastream_ID);
			if(error_code != DPE_SUCCESS)
			{
				error_code = DPE_ERASE_ERROR;
			}
		}
	}

	return;
}
void dp_G4M_program_action(void)
{
	dp_G4M_initialize();
	if (error_code == DPE_SUCCESS)
	{
		pgmmode = 0x1u;
		dp_set_mode();

		if (error_code == DPE_SUCCESS)
		{
			/* Global unit1 is used to hold the number of components */
			global_uint1 = (DPUINT)dp_get_bytes(Header_ID,G4M_DATASIZE_OFFSET,G4M_DATASIZE_BYTE_LENGTH);
			global_uint2 = 1u;

			dp_G4M_process_data(G4M_datastream_ID);
			if(error_code != DPE_SUCCESS)
			{
				error_code = DPE_CORE_PROGRAM_ERROR;
			}
		}
	}
	return;
}

void dp_G4M_verify_action(void)
{
	dp_G4M_initialize();
	if (error_code == DPE_SUCCESS)
	{
		pgmmode = 0x2u;
		dp_set_mode();

		if (error_code == DPE_SUCCESS)
		{
			/* Global unit1 is used to hold the number of components */
			global_uint1 = (DPUINT)dp_get_bytes(Header_ID,G4M_DATASIZE_OFFSET,G4M_DATASIZE_BYTE_LENGTH);
			global_uint2 = 1u;

			dp_G4M_process_data(G4M_datastream_ID);
			if(error_code != DPE_SUCCESS)
			{
				error_code = DPE_CORE_VERIFY_ERROR;
			}
		}
	}
	return;
}

void dp_G4M_enc_data_authentication_action(void)
{
	dp_G4M_initialize();
	if (error_code == DPE_SUCCESS)
	{
		pgmmode = 0x0u;
		dp_set_mode();

		if (error_code == DPE_SUCCESS)
		{
			/* Global unit1 is used to hold the number of components */
			global_uint1 = (DPUINT)dp_get_bytes(Header_ID,G4M_DATASIZE_OFFSET,G4M_DATASIZE_BYTE_LENGTH);
			global_uint2 = 1u;

			dp_G4M_process_data(G4M_datastream_ID);
			if(error_code != DPE_SUCCESS)
			{
				error_code = DPE_AUTHENTICATION_FAILURE;
			}
		}
	}
	return;
}
void dp_G4M_check_core_status(void)
{
	opcode = G4M_ISC_NOOP;
	IRSCAN_out(&global_uchar1);
	goto_jtag_state(JTAG_RUN_TEST_IDLE,1u);

	#ifdef ENABLE_DISPLAY
    if ((global_uchar1 & 0x4u) == 0x4u)
	{
		dp_display_text("\r\nFPGA Array is programmed and enabled.");
	}
	else
	{
		dp_display_text("\r\nFPGA Array is not enabled.");
	}
	#endif
	return;
}


#ifdef ENABLE_DISPLAY
void dp_G4M_device_info_action(void)
{
	dp_G4M_check_core_status();
	dp_G4M_read_design_info();
	dp_G4M_read_prog_info();
	dp_G4M_read_fsn();
	dp_G4M_read_security();

	return;
}

void dp_G4M_read_design_info(void)
{
	opcode = G4M_READ_DESIGN_INFO;
	IRSCAN_in();
	DRSCAN_in(0u, G4M_STATUS_REGISTER_BIT_LENGTH, (DPUCHAR*)(DPUCHAR*)DPNULL);
    goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
	dp_delay(G4M_STANDARD_DELAY);
	opcode = G4M_READ_DESIGN_INFO;
	dp_G4M_device_poll(8u, 7u);
	if (error_code == DPE_SUCCESS)
	{
		dp_read_shared_buffer(3u);
		if (error_code == DPE_SUCCESS)
		{
			dp_display_text("\r\nDesign Name: ");
			for (global_uchar1 = 2u; global_uchar1 < 32u; global_uchar1++)
			{
				dp_display_value(shared_buf[global_uchar1],CHR);
			}
			dp_display_text("\r\nChecksum: ");
			dp_display_array(shared_buf,2u,HEX);
			dp_display_text("\r\nDesign Info: ");
			dp_display_array(shared_buf,34u,HEX);
			dp_display_text("\r\nDESIGNVER: ");
			dp_display_array(&shared_buf[32],2u,HEX);
			dp_display_text("\r\nBACKLEVEL: ");
			dp_display_array(&shared_buf[34],2u,HEX);
		}
	}
	return;
}

void dp_G4M_read_prog_info(void)
{
	opcode = G4M_READ_PROG_INFO;
	IRSCAN_in();
	DRSCAN_in(0u, G4M_STATUS_REGISTER_BIT_LENGTH, (DPUCHAR*)(DPUCHAR*)DPNULL);
    goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
	dp_delay(G4M_STANDARD_DELAY);
	
	opcode = G4M_READ_PROG_INFO;
	dp_G4M_device_poll(128u, 127u);
	if (error_code == DPE_SUCCESS)
	{
		dp_display_text("\r\nPROG_INFO: ");
		dp_display_array(poll_buf,16u,HEX);
		if ((poll_buf[6] & 0x1) == 0u)
		{
			dp_display_text("\r\nVCC was programmed at 1.2V");
		}
		else
		{
			dp_display_text("\r\nVCC was programmed at 1.0v");
		}
		if  ( ((poll_buf[8] & 0x3f) != 0u) && ((poll_buf[8] & 0x3f) != 0x3fu) )
		{
			dp_display_text("\r\nAlgorithm Version: ");
			dp_display_value((poll_buf[8] & 0x3f),DEC);
		}

		global_uchar1 = ((poll_buf[8] >> 6) | (poll_buf[9] << 2)) & 0xfu;
		dp_display_text("\r\nProgrammer code: ");
		dp_display_value(global_uchar1, DEC);

		global_uchar1 = ((poll_buf[10] >> 1)) & 0x3fu;
		dp_display_text("\r\nSoftware version code: ");
		dp_display_value(global_uchar1, DEC);

		global_uchar1 = ((poll_buf[10] >> 7) | (poll_buf[11] << 1)) & 0x7u;
		dp_display_text("\r\nProgramming Software code: ");
		dp_display_value(global_uchar1, DEC);

		global_uchar1 = ((poll_buf[11] >> 2)) & 0x7u;
		dp_display_text("\r\nProgramming Interface Protocol code: ");
		dp_display_value(global_uchar1, DEC);

		global_uchar1 = ((poll_buf[11] >> 5)) & 0x7u;
		dp_display_text("\r\nProgramming File Type code: ");
		dp_display_value(global_uchar1, DEC);
	}
	return;
}

void dp_G4M_read_fsn(void)
{
	opcode = G4M_READ_FSN;
	IRSCAN_in();
	DRSCAN_in(0u, G4M_STATUS_REGISTER_BIT_LENGTH, (DPUCHAR*)(DPUCHAR*)DPNULL);
    goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
	opcode = G4M_READ_FSN;
	dp_G4M_device_poll(129u, 128u);
	dp_display_text("\r\n=====================================================================");
	dp_display_text("\r\nDSN: ");
	dp_display_array(poll_buf, 16u, HEX);
	dp_display_text("\r\n=====================================================================");

	return;
}
#endif

/* Checking device ID function.  ID is already read in dpalg.c */
void dp_check_G4_device_ID (void)
{
	/* DataIndex is a variable used for loading the array data but not used now.  
    * Therefore, it can be used to store the Data file ID for */
    DataIndex = dp_get_bytes(Header_ID,G4M_ID_OFFSET,G4M_ID_BYTE_LENGTH);
    global_ulong1 = dp_get_bytes(Header_ID,G4M_ID_MASK_OFFSET,4U);
    
    device_ID &= global_ulong1;
    DataIndex &= global_ulong1;
    /* Identifying target device and setting its parms */
	
	if ( device_ID == DataIndex )
    {
		#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nActID = ");
		dp_display_value(device_ID,HEX);
		dp_display_text(" ExpID = ");
		dp_display_value(DataIndex,HEX);
		dp_display_text("\r\nDevice Rev = ");
		dp_display_value(device_rev,HEX);
		#endif
        device_family = (DPUCHAR) dp_get_bytes(Header_ID,G4M_DEVICE_FAMILY_OFFSET,G4M_DEVICE_FAMILY_BYTE_LENGTH);
    }
	else
    {
        error_code = DPE_IDCODE_ERROR;
    }

    return;
}

void dp_G4M_read_security(void)
{
	opcode = G4M_QUERY_SECURITY;
	IRSCAN_in();
	DRSCAN_in(0u, G4M_SECURITY_STATUS_REGISTER_BIT_LENGTH, (DPUCHAR*)(DPUCHAR*)DPNULL);
    goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
	opcode = G4M_QUERY_SECURITY;
	dp_G4M_device_poll(16u, 15u);
	dp_read_shared_buffer(3u);
	for (poll_index = 0u;poll_index < 40u; poll_index++)
	{
		if (shared_buf[poll_index] != 0u)
		{
			security_queried = TRUE;
			break;
		}
	}

	#ifdef ENABLE_DISPLAY
	if (security_queried == TRUE)
	{
		dp_display_text("\r\n--- Security locks and configuration settings ---\r\n");	
		dp_display_array(shared_buf,42u,HEX);
	}
	#endif
	return;
}

/* Check if system controller is ready to enter programming mode */
void dp_G4M_device_poll(DPUCHAR bits_to_shift, DPUCHAR Busy_bit)
{
    for ( poll_index = 0U; poll_index <= G4M_MAX_CONTROLLER_POLL; poll_index++ )
    {
		IRSCAN_in();
        DRSCAN_out(bits_to_shift, (DPUCHAR*)DPNULL, poll_buf);
        dp_delay(G4M_STANDARD_DELAY);
        if ( ((poll_buf[Busy_bit/8] & (1 << (Busy_bit % 8))) == 0x0u))
        {
            break;
        }
    }
    if(poll_index > G4M_MAX_CONTROLLER_POLL)
    {
		#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nDevice polling failed.");
		#endif
        error_code = DPE_POLL_ERROR;
    }
	
    return;
}


void dp_read_shared_buffer(DPUCHAR ucNumOfBlocks)
{
	dp_flush_global_buf1();
	for (global_uchar1 = 0u; global_uchar1 < ucNumOfBlocks; global_uchar1++)
	{
		global_buf1[0] = (global_uchar1 << 1u);
		opcode = G4M_READ_BUFFER;
		IRSCAN_in();
		DRSCAN_in(0u, G4M_FRAME_STATUS_BIT_LENGTH, global_buf1);
		goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
		dp_delay(G4M_STANDARD_DELAY);
		opcode = G4M_READ_BUFFER;
		dp_G4M_device_poll(129u, 128u);
		for (global_uchar2 = 0;global_uchar2 < 16u; global_uchar2++)
		{
			shared_buf[global_uchar1*16u + global_uchar2] = poll_buf[global_uchar2];
		}
	}
	return;
}

void dp_G4M_poll_device_ready(void)
{
	opcode = G4M_ISC_NOOP;
    for ( poll_index = 0U; poll_index <= G4M_MAX_CONTROLLER_POLL; poll_index++ )
    {
		IRSCAN_in();
		goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
		dp_delay(G4M_STANDARD_DELAY);
        DRSCAN_out(8u, (DPUCHAR*)DPNULL, poll_buf);
        
        if ((poll_buf[0] & 0x80u) == 0x0u)
        {
            break;
        }
    }
    if(poll_index > G4M_MAX_CONTROLLER_POLL)
    {
		#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nDevice polling failed.");
		#endif
        error_code = DPE_POLL_ERROR;
    }

	return;
}

void dp_set_pgm_mode(void)
{
	opcode = G4M_MODE;
	IRSCAN_in();
	DRSCAN_in(0u, G4M_STATUS_REGISTER_BIT_LENGTH, (DPUCHAR*)(DPUCHAR*)DPNULL);
    goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
	dp_delay(G4M_STANDARD_DELAY);
	opcode = G4M_MODE;
	dp_G4M_device_poll(8u, 7u);
	
	return;
}

/****************************************************************************
* Purpose: Loads the BSR regsiter with data specified in the data file.  
*   If BSR_SAMPLE is enabled, the data is not loaded.  Instead, the last known
*   State of the IOs is maintained by stepping through DRCapture JTAG state.
****************************************************************************/
void dp_G4M_load_bsr(void)
{
	global_uint1 = (DPUINT) dp_get_bytes(G4M_Header_ID,G4M_NUMOFBSRBITS_OFFSET,G4M_NUMOFBSRBITS_BYTE_LENGTH);

	dp_G4M_check_core_status();
    opcode = ISC_SAMPLE;
    IRSCAN_in();
    
    #ifdef BSR_SAMPLE
    /* Capturing the last known state of the IOs is only valid if the core
    was programmed.  Otherwise, load the BSR with what is in the data file. */
	if ((global_uchar1 & 0x4u) != 0x4u)
    {
        dp_get_bytes(G4M_BsrPattern_ID,0u,1u);
        if (return_bytes)
        {
			#ifdef ENABLE_DISPLAY
			dp_display_text("\r\nWarning: FPGA array is not programmed. Loading BSR register...");
			#endif
            dp_get_and_DRSCAN_in(G4M_BsrPattern_ID, global_uint1, 0u);
            goto_jtag_state(JTAG_RUN_TEST_IDLE,0u);
        }
    }
    else 
    {
		#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nMaintaining last known IO states...");
		#endif
        goto_jtag_state(JTAG_CAPTURE_DR,0u);
        goto_jtag_state(JTAG_RUN_TEST_IDLE,0u);
    }
    #else
    dp_get_bytes(G4M_BsrPattern_ID,0u,1u);
    if (return_bytes)
    {
		#ifdef ENABLE_DISPLAY
	    dp_display_text("\r\nLoading BSR...");
		#endif
        dp_get_and_DRSCAN_in(G4M_BsrPattern_ID, global_uint1, 0u);
        goto_jtag_state(JTAG_RUN_TEST_IDLE,0u);
    }
    #endif
    
    return;
}

void dp_G4M_perform_isc_enable(void)
{
	dp_flush_global_buf1();
	global_buf1[0] |= (G4M_ALGO_VERSION & 0x3fu);
	global_buf1[2] |= (G4M_DIRECTC_VERSION & 0x3fu) << 1u;
	global_buf1[2] |= (DIRECTC_PROGRAMMING & 0x7u) << 7u;
	global_buf1[3] |= (DIRECTC_PROGRAMMING & 0x7u) >> 1u;
	global_buf1[3] |= (JTAG_PROGRAMMING_PROTOCOL & 0x7u) << 2u;

	opcode = ISC_ENABLE;
    IRSCAN_in();
    DRSCAN_in(0u, ISC_STATUS_REGISTER_BIT_LENGTH, global_buf1);
    goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
	dp_delay(G4M_STANDARD_DELAY);
	opcode = ISC_ENABLE;
	dp_G4M_device_poll(32u, 31u);
	
	
	if ( (error_code != DPE_SUCCESS) || ((poll_buf[0] & 0x1u) == 1u)	)
	{
		#ifdef ENABLE_DISPLAY
		dp_display_text("\r\nFailed to enter programming mode.");
		#endif
		error_code = DPE_INIT_FAILURE;
	}

	#ifdef ENABLE_DISPLAY
	dp_display_text("\r\nISC_ENABLE_RESULT: ");
	dp_display_array(poll_buf,4u,HEX);

	/* Display CRCERR */
	global_uchar1 = poll_buf[0] & 0x1u;
	dp_display_text("\r\nCRCERR: ");
	dp_display_value(global_uchar1,HEX);

	/* Display EDCERR */
	global_uchar1 = (poll_buf[0] & 0x2u) >> 1u;
	dp_display_text("\r\nEDCERR: ");
	dp_display_value(global_uchar1,HEX);

	/* Display TEMPRANGE */
	global_uchar1 = (poll_buf[0] >> 2) & 0x7u;
	dp_display_text("\r\nTEMPRANGE:");
	dp_display_value(global_uchar1,HEX);
	if (global_uchar1 == 0u)
	{
		dp_display_text("\r\nTEMPRANGE: COLD");
	}
	else if (global_uchar1 == 1u)
	{
		dp_display_text("\r\nTEMPRANGE: ROOM");
	}
	if (global_uchar1 == 2u)
	{
		dp_display_text("\r\nTEMPRANGE: HOT");
	}


	/* Display VPPRANGE */
	global_uchar1 = (poll_buf[0] >> 5) & 0x7u;
	dp_display_text("\r\nVPPRANGE:");
	dp_display_value(global_uchar1,HEX);
	if (global_uchar1 == 0u)
	{
		dp_display_text("\r\nVPPRANGE: LOW");
	}
	else if (global_uchar1 == 1u)
	{
		dp_display_text("\r\nVPPRANGE: NOMINAL");
	}
	if (global_uchar1 == 2u)
	{
		dp_display_text("\r\nVPPRANGE: HIGH");
	}
	
	/* Display TEMP */
	dp_display_text("\r\nTEMP:");
	dp_display_value(poll_buf[1],HEX);

	/* Display VPP */
	dp_display_text("\r\nVPP:");
	dp_display_value(poll_buf[2],HEX);

	#endif
	

	return;
}
/* Enter programming mode */
void dp_G4M_initialize(void)
{
	dp_G4M_poll_device_ready();
	if (error_code == DPE_SUCCESS)
	{
		dp_set_pgm_mode();
		if (error_code == DPE_SUCCESS)
		{
			dp_G4M_load_bsr();
			if (error_code == DPE_SUCCESS)
			{
				dp_G4M_perform_isc_enable();
			}
		}
	}

	return;
}


/* Function is used to exit programming mode */
void dp_G4M_exit(void)
{
	if (pgmmode_flag == TRUE)
	{
		opcode = ISC_DISABLE;
		IRSCAN_in();
		goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
		dp_delay(G4M_STANDARD_DELAY);

		opcode = ISC_DISABLE;
		dp_G4M_device_poll(32u, 31u);
		#ifdef ENABLE_DISPLAY
		if (error_code != DPE_SUCCESS)
		{
			dp_display_text("\r\nFailed to disable programming mode.");
		}
		#endif
	}
	#ifdef ENABLE_DISPLAY
	dp_G4M_read_fsn();
	#endif
	goto_jtag_state(JTAG_TEST_LOGIC_RESET,0u);
	return;
}

void dp_set_mode(void)
{
	opcode = G4M_FRAME_INIT;
	IRSCAN_in();
	DRSCAN_in(0u, G4M_STATUS_REGISTER_BIT_LENGTH, &pgmmode);
	goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);	
	dp_delay(G4M_STANDARD_DELAY);
	dp_G4M_device_poll(8u, 7u);
	#ifdef ENABLE_DISPLAY
	if (error_code != DPE_SUCCESS)
	{
		dp_display_text("r\nFailed to set programming mode.");
	}
	#endif
	
	return;
}


void dp_G4M_process_data(DPUCHAR BlockID)
{
	DataIndex = 0u;  
	/* Global unit1 is used to hold the number of components */
	/* Loop through the number of components */
	#ifdef ENABLE_DISPLAY
	dp_display_text("\r\n");
	#endif

	
	for (; global_uint2 <= global_uint1; global_uint2++)
	{
		/* get the number of blocks */
		/* Global ulong1 is used to hold the number of blocks within the components */
		global_ulong1 = dp_get_bytes(G4M_NUMBER_OF_BLOCKS_ID,(DPULONG)(((global_uint2 - 1u) * 22u) / 8u),4u);
		global_ulong1 >>= ((global_uint2 - 1U)* 22u) % 8u;
		global_ulong1 &= 0x3FFFFFu;


		for (global_ulong2 = 1u; global_ulong2 <= global_ulong1; global_ulong2++)
		{
			#ifdef ENABLE_DISPLAY
				dp_display_text("\rProcessing frame: ");
				dp_display_value(global_ulong2,DEC);
			#endif

			opcode = G4M_FRAME_DATA;
			IRSCAN_in();
			dp_get_and_DRSCAN_in(BlockID, G4M_FRAME_BIT_LENGTH, DataIndex);
			goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
			dp_delay(G4M_STANDARD_DELAY);
				
			opcode = G4M_FRAME_DATA;
			dp_G4M_device_poll(128u, 127u);
			
			if ( ((error_code != DPE_SUCCESS) || ((poll_buf[0] & 0x18u) != 0u)) )
			{
				dp_G4M_get_data_status();
				#ifdef ENABLE_DISPLAY
				dp_display_text("\r\ncomponentNo: ");
				dp_display_value(global_uint2, DEC);
				dp_display_text("\r\nblockNo: ");
				dp_display_value(global_ulong2, DEC);
				dp_display_text("\r\nDATA_STATUS_RESULT: ");
				dp_display_array(poll_buf,4u,HEX);
				dp_display_text("\r\nERRORCODE: ");
				dp_display_value((poll_buf[0]>>3u) & 0x1fu,HEX);
				dp_display_text("\r\nAUTHERRCODE: ");
				dp_display_value(poll_buf[1],HEX);
				#endif
				error_code = DPE_PROCESS_DATA_ERROR;
				global_uint2 = global_uint1;
				break;
			}
				
			DataIndex += G4M_FRAME_BIT_LENGTH;
		}
	}
	return;
}

void dp_G4M_get_data_status(void)
{
	opcode = G4M_FRAME_STATUS;
	IRSCAN_in();
	DRSCAN_in(0u, ISC_STATUS_REGISTER_BIT_LENGTH, (DPUCHAR*)(DPUCHAR*)DPNULL);
	goto_jtag_state(JTAG_RUN_TEST_IDLE,G4M_STANDARD_CYCLES);
	dp_delay(G4M_STANDARD_DELAY);
			
	opcode = G4M_FRAME_STATUS;
	dp_G4M_device_poll(32u, 31u);

	return;
}

#endif /* ENABLE_G4_SUPPORT */
