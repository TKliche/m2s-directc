/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpG4alg.h                                               */
/*                                                                          */
/*  Description:    Contains function prototypes.                           */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_DPG4ALG_H
#define INC_DPG4ALG_H

/* 
* Data block ID definitions
*/
#define G4M_ALGO_VERSION					1u
#define DIRECTC_PROGRAMMING					2u
#define G4M_DIRECTC_VERSION					0u
#define SPI_PROGRAMMING_PROTOCOL			2u
#define JTAG_PROGRAMMING_PROTOCOL			3u

#define G4M_Header_ID						0u
#define G4M_USER_INFO_ID					1u
#define G4M_ACT_UROW_DESIGN_NAME_ID			2u
#define G4M_BsrPattern_ID					3u  
#define G4M_BsrPatternMask_ID				4u  
#define G4M_NUMBER_OF_BLOCKS_ID				5u
#define G4M_UPK1_ID							6u
#define G4M_UPK2_ID							7u
#define G4M_datastream_ID					8u
#define G4M_erasedatastream_ID				9u
#define G4M_VerifyDataStream_ID				10u
#define G4M_EnvmDataStream_ID				11u
#define G4M_EnvmVerifyDataStream_ID			12u

#define G4M_DEVICE_FAMILY_OFFSET			36u
#define G4M_DEVICE_FAMILY_BYTE_LENGTH		1u
#define G4M_ID_OFFSET						37u
#define G4M_ID_BYTE_LENGTH					4u
#define G4M_ID_MASK_OFFSET					41u
#define G4M_ID_MASK_BYTE_LENGTH				4u
#define G4M_SILSIG_OFFSET					45u
#define G4M_SILSIG_BYTE_LENGTH				4u
#define G4M_CHECKSUM_OFFSET					49u
#define G4M_CHECKSUM_BYTE_LENGTH			2u
#define G4M_NUMOFBSRBITS_OFFSET				51u
#define G4M_NUMOFBSRBITS_BYTE_LENGTH		2u
#define G4M_NUMOFCOMPONENT_OFFSET			53u
#define G4M_NUMOFCOMPONENT_BYTE_LENGTH		2u
#define G4M_DATASIZE_OFFSET					55u
#define G4M_DATASIZE_BYTE_LENGTH			2u
#define G4M_ERASEDATASIZE_OFFSET			57u
#define G4M_ERASEDATASIZE_BYTE_LENGTH		2u
#define G4M_VERIFYDATASIZE_OFFSET			59u
#define G4M_VERIFYDATASIZE_BYTE_LENGTH		2u
#define G4M_ENVMDATASIZE_OFFSET				61u
#define G4M_ENVMDATASIZE_BYTE_LENGTH		2u
#define G4M_ENVMVERIFYDATASIZE_OFFSET		63u
#define G4M_ENVMVERIFYDATASIZE_BYTE_LENGTH	2u

/*
* G4Main JTAG instructions
*/
#define G4M_ISC_NOOP					0x8fu
#define G4M_FRAME_INIT					0xaeu
#define G4M_READ_DESIGN_INFO			0xa6u
#define G4M_READ_PROG_INFO				0xe7u
#define G4M_READ_BUFFER					0xf2u
#define G4M_READ_FSN					0xf0u
#define G4M_QUERY_SECURITY				0xb8u
#define G4M_MODE						0xafu
#define G4M_FRAME_DATA					0xeeu
#define G4M_FRAME_STATUS				0xc9u

#define	G4M_MAX_CONTROLLER_POLL			1000000u

#define ISC_STATUS_REGISTER_BIT_LENGTH	32u
#define G4M_STANDARD_CYCLES				3u
#define G4M_STANDARD_DELAY				10u

#define G4M_STATUS_REGISTER_BIT_LENGTH				8u
#define G4M_SECURITY_STATUS_REGISTER_BIT_LENGTH		16u
#define G4M_FRAME_BIT_LENGTH						128u
#define G4M_FRAME_STATUS_BIT_LENGTH					129u



DPUCHAR dp_top_g4 (void);
void dp_init_G4_vars(void);
void dp_check_G4_action(void);
void dp_perform_G4_action (void);

/* Supported Actions */
void dp_G4M_device_info_action(void);
void dp_G4M_erase_security_action(void);
void dp_G4M_erase_action(void);
void dp_G4M_program_action(void);
void dp_G4M_verify_action(void);
void dp_G4M_enc_data_authentication_action(void);

void dp_check_G4_device_ID (void);
void dp_G4M_poll_device_ready(void);
void dp_G4M_check_core_status(void);
void dp_G4M_read_design_info(void);
void dp_G4M_read_prog_info(void);
void dp_G4M_read_fsn(void);
void dp_G4M_read_security(void);
void dp_read_shared_buffer(DPUCHAR ucNumOfBlocks);
void dp_set_pgm_mode(void);
void dp_G4M_load_bsr(void);
void dp_G4M_perform_isc_enable(void);
void dp_G4M_process_data(DPUCHAR BlockID);
void dp_G4M_get_data_status(void);


/* Initialization functions */
void dp_G4M_device_poll(DPUCHAR bits_to_shift, DPUCHAR Busy_bit);
void dp_G4M_initialize(void);
void dp_G4M_exit(void);
void dp_set_mode(void);

/* Erase function */
void dp_G4M_erase(void);

#endif /* INC_DPG4ALG_H */


