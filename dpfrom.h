/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpfrom.h                                                */
/*                                                                          */
/*  Description:    Contains the function prototypes.                       */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_dpfrom_H
#define INC_dpfrom_H

/****************************************************************************/
/* UFROM Opcodes                                                            */
/****************************************************************************/
#define ISC_UFROM_ADDR_SHIFT	0x9FU
#define ISC_PROGRAM_UFROM	0x9BU
#define ISC_VERIFY_UFROM	0x9CU
#define ISC_READ_UFROM  	0x93U


#define ISC_DESCRAMBLE_UFROM	0xA5U
#define ISC_PROGRAM_ENC_UFROM	0xA6U
#define ISC_VERIFY_ENC_UFROM	0x9DU

/****************************************************************************/
/* UFROM Register Length                                                    */
/****************************************************************************/
#define FROM_ROW_BIT_LENGTH     128U
#define FROM_ROW_BYTE_LENGTH    16U

/* FROM delay and cycle parameters */
#define ISC_READ_UFROM_CYCLES          3u
#define ISC_READ_UFROM_DELAY           159u
#define ISC_VERIFY_UFROM_CYCLES        3u
#define ISC_VERIFY_UFROM_DELAY         159u
#define ISC_PROGRAM_UFROM_CYCLES       5u
#define ISC_PROGRAM_UFROM_DELAY        10000u
#define ISC_PROGRAM_ENC_UFROM_CYCLES   5u
#define ISC_UFROM_ADDR_SHIFT_CYCLES    1u
#define ISC_DESCRAMBLE_UFROM_CYCLES    1u
#define ISC_DESCRAMBLE_UFROM_DELAY     69u


/****************************************************************************/
/* Function prototypes                                                      */
/****************************************************************************/
void dp_program_from(void);
void dp_verify_from(void);


void dp_enc_program_from(void);
void dp_erase_from_action(void);
void dp_program_from_action(void);
void dp_verify_from_action(void);
void dp_erase_from(void);
#ifdef ENABLE_DISPLAY
void dp_read_from(void);
#endif

#endif
