/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpcore.h                                                */
/*                                                                          */
/*  Description:    Contains the function prototypes.                       */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_dpcore_H
#define INC_dpcore_H

/****************************************************************************/
/*                                                                          */
/****************************************************************************/
extern DPUCHAR bol_eol_verify;

/****************************************************************************/
/* CORE Opcodes                                                             */
/****************************************************************************/
#define ISC_PROGRAM              0x83u
#define ISC_VERIFY0              0x8Du
#define ISC_VERIFY1              0x8Eu
#define ISC_INCREMENT            0x87u
#define ISC_DATA_SHIFT           0x89u
#define ISC_ADDRESS_SHIFT        0xA9u

/****************************************************************************/
/* CORE Register Length	                                                    */
/****************************************************************************/
#define ARRAY_ROW_LENGTH         26u
#define UROW_BIT_LENGTH          128u
#define UROW_BYTE_LENGTH         16u
#define SILSIG_BIT_LENGTH        32u
#define SILSIG_BYTE_LENGTH       4u

#define DESCRAMBLE               0xDFU
#define AES_BLOCK_LENGTH         128U
#define ISC_PROGRAM_RDLC         0x8CU

/* ARRAY delay and cycle parameters */
#define ISC_VERIFY0_CYCLES             3u
#define ISC_VERIFY0_DELAY              264u
#define ISC_VERIFY1_CYCLES             3u
#define ISC_VERIFY1_DELAY              264u
#define DESCRAMBLE_CYCLES              3u
#define DESCRAMBLE_DELAY               69u
#define ISC_PROGRAM_CYCLES             3u
#define ISC_INCREMENT_CYCLES           3u
#define ISC_ADDRESS_SHIFT_CYCLES       1u
#define ISC_DATA_SHIFT_CYCLES          3u
#define ISC_PROGRAM_RLOCK_CYCLES       3u
#define ISC_PROGRAM_RDLC_CYCLES        1u
#define READ_FACTORY_DELAY             330u
#define READ_FACTORY_CYCLES            3u
#define FACTORY_ADDRESS_SHIFT_CYCLES   1u                                       

extern DPUINT cycle_count;
/****************************************************************************/
/* Function prototypes                                                      */
/****************************************************************************/
void dp_program_array(void);
void dp_exe_program(void);
void dp_load_row_data(void);
void dp_load_row_address(void);
void dp_exe_verify(void);
void dp_verify_array_eol(void);
void dp_verify_array(void);
void dp_program_rlock(void);
void dp_reset_address(void);
void dp_increment_address(void);

void dp_load_enc_row_data(void);
void dp_enc_verify_array(void);
void dp_enc_program_array(void);
void dp_enc_program_rlock(void);
void dp_erase_array_action(void);
void dp_erase_array(void);
void dp_program_array_action(void);
void dp_verify_array_action(void);
void dp_enc_data_authentication_action(void);
void dp_enc_data_authentication(void);
#endif


