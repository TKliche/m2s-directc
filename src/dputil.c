/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dputil.c                                                */
/*                                                                          */
/*  Description:    Contains initialization and data checking functions.    */
/*                                                                          */
/* ************************************************************************ */

#include "dpuser.h"
#include "dputil.h"
#include "dpcom.h"
#include "dpalg.h"

/*
* General purpose Global variables needed in the program
*/
DPUCHAR global_uchar1;	/* Global tmp should be used once and released	*/
DPUCHAR global_uchar2;	/* Global tmp should be used once and released	*/
DPULONG global_ulong1;
DPULONG global_ulong2;
DPUINT global_uint1;
DPUINT global_uint2;
/* global_buf_SIZE needs to be set to 16 for G3 support.  Increasing the value does not hurt but not needed
as long as it does not exceed 255 */
DPUCHAR global_buf1[global_buf_SIZE];	/* General purpose global_buf1fer */
DPUCHAR global_buf2[global_buf_SIZE];	/* global_buffer to hold UROW data */


void dp_flush_global_buf1(void)
{
    for (global_uchar1=0u; global_uchar1 < global_buf_SIZE; global_uchar1++)
    {
        global_buf1[global_uchar1] = 0u;
    }
    return;
}

void dp_flush_global_buf2(void)
{
    for (global_uchar1=0u; global_uchar1 < global_buf_SIZE; global_uchar1++)
    {
        global_buf2[global_uchar1]=0u;
    }
    return;
}

void dp_init_vars(void)
{
    error_code = DPE_SUCCESS;
    return;
}

/*
* Module: dp_check_image_crc
* 		purpose: Performs crc on the entire image.  
* Return value: 
* 	DPINT: User defined integer value which reports DPE_SUCCESS if there is a match or DPE_CRC_MISMATCH if failed. 
* 
*/
void dp_check_image_crc(void)
{
    DPUINT expected_crc;
    #ifdef ENABLE_DISPLAY        
    dp_display_text("\r\nChecking data CRC...");
    #endif
    
    global_ulong1 = dp_get_bytes(Header_ID,0u,4u);
    if ( (global_ulong1 == 0x69736544u) || (global_ulong1 == 0x65746341u) || (global_ulong1 == 0x2D4D3447u))
    {
        requested_bytes = 0u;
        image_size = dp_get_bytes(Header_ID,IMAGE_SIZE_OFFSET,4u);
        expected_crc = (DPUINT) dp_get_bytes(Header_ID,image_size - 2u,2u);
        #ifdef ENABLE_DISPLAY        
        dp_display_text("\r\nExpected CRC=");
        dp_display_value( expected_crc ,HEX);
        #endif
        if (image_size == 0u)
        {
            error_code = DPE_CRC_MISMATCH;
            #ifdef ENABLE_DISPLAY
            dp_display_text("\r\nData file is not loaded... \r\n");
            #endif
        }
        else 
        {
            #ifdef ENABLE_DISPLAY      
            dp_display_text("\r\nCalculating actual CRC...");
            #endif
            /* Global_uint is used to hold the value of the calculated CRC */
            global_uint1 = 0u;
            /* DataIndex is used to keep track the byte position in the image that is needed per get_data_request */
            DataIndex = 0u;
            requested_bytes = image_size - 2u;
            while (requested_bytes)
            {
                page_buffer_ptr = dp_get_data(Header_ID,DataIndex*8u);
                if (return_bytes > requested_bytes )
                    return_bytes = requested_bytes;
                for (global_ulong1=0u; global_ulong1< return_bytes;global_ulong1++)
                {
                    global_uchar1 = page_buffer_ptr[global_ulong1];
                    dp_compute_crc();
                }
                DataIndex += return_bytes;
                requested_bytes -= return_bytes;
            }
            if (global_uint1 != expected_crc)
            {
                #ifdef ENABLE_DISPLAY
                dp_display_text("\r\nCRC verification failed.  Expected CRC = ");
                dp_display_value(global_uint1,HEX);
                dp_display_text(" Actual CRC = ");
                dp_display_value((DPUINT) dp_get_bytes(Header_ID,image_size - 2,2),HEX);
                dp_display_text("\r\n");
                #endif
                error_code = DPE_CRC_MISMATCH;
            }
        }
    }
    else
    {
        #ifdef ENABLE_DISPLAY
        dp_display_text("\r\nData file is not valid. ");
        #endif
        error_code = DPE_CRC_MISMATCH;
    }
    
    return;
}


void dp_compute_crc(void)
{
    for (global_uchar2 = 0u; global_uchar2 < 8u; global_uchar2++)
    {
        global_uint2 = (global_uchar1 ^ global_uint1) & 0x01u;
        global_uint1 >>= 1u;
        if (global_uint2)
        {
            global_uint1 ^= 0x8408u;
        }
        global_uchar1 >>= 1u;
    }
    return;
}


