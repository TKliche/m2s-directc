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

#ifndef INC_DPUTIL_H
#define INC_DPUTIL_H

/****************************************************************************/
/* External common global variables                                         */
/****************************************************************************/
/* global_buf_SIZE should not exceed 255 bytes.  Only 16 is needed for G3 */
#define global_buf_SIZE 17u

extern DPUCHAR global_uchar1; /* Global tmp should be used once and released */
extern DPUCHAR global_uchar2;
extern DPUINT global_uint1;
extern DPUINT global_uint2;
extern DPULONG global_ulong1;
extern DPULONG global_ulong2;
extern DPUCHAR global_buf1[global_buf_SIZE]; /* General purpose global_buffer */
extern DPUCHAR global_buf2[global_buf_SIZE];

void dp_flush_global_buf1(void);
void dp_flush_global_buf2(void);
void dp_init_vars(void);
/* Function used to identify which block is supported in the dat file and their encryption status. */

void dp_compute_crc(void);
void dp_check_image_crc(void);

#endif /* INC_DPUTIL_H */


