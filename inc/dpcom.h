/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpcom.h                                                 */
/*                                                                          */
/*  Description:    Contains functions prototypes needed to access the data */
/*  from external flash or other means of communication                     */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_DPCOM_H
#define INC_DPCOM_H

extern DPULONG return_bytes;
extern DPULONG image_size;
extern DPULONG requested_bytes;
extern DPUCHAR *page_buffer_ptr;

/* user attention is required.  PAGE_BUFFER_SIZE needs to be specified in bytes */
#define PAGE_BUFFER_SIZE 16u
#define MIN_VALID_BYTES_IN_PAGE 16u

#ifdef USE_PAGING
extern DPUCHAR page_global_buffer[PAGE_BUFFER_SIZE];  /* Page_global_buffer simulating the global_buf1fer that is accessible by DirectC code*/
#endif

/*
* Location of special variables needed in the header section of the image file
*/
#define Header_ID                 0u
#define BTYES_PER_TABLE_RECORD    9u
#define ACTEL_HEADER_SIZE        24u
#define HEADER_SIZE_OFFSET       24u
#define IMAGE_SIZE_OFFSET        25u
#define MIN_IMAGE_SIZE           56u


DPUCHAR * dp_get_data(DPUCHAR var_ID,DPULONG bit_index);
DPUCHAR * dp_get_header_data(DPULONG bit_index);
void dp_get_page_data(DPULONG image_requested_address);
void dp_get_data_block_address(DPUCHAR requested_var_ID);
DPUCHAR * dp_get_data_block_element_address(DPULONG bit_index);
DPULONG dp_get_bytes(DPUCHAR var_ID,DPULONG byte_index,DPUCHAR bytes_requested);
DPULONG dp_get_header_bytes(DPULONG byte_index,DPUCHAR bytes_requested);
#endif /* INC_DPCOM_H */


