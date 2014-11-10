/* ************************************************************************ */
/*                                                                          */
/*  DirectC         Copyright (C) Microsemi Corporation 2013                */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/* ************************************************************************ */
/*                                                                          */
/*  Module:         dpalg.h                                                 */
/*                                                                          */
/*  Description:    Contains the function prototypes.                       */
/*                                                                          */
/* ************************************************************************ */

#ifndef INC_DPALG_H
#define INC_DPALG_H


extern DPUCHAR Action_code;
extern DPUCHAR Action_done; 
extern DPUCHAR opcode;
extern DPULONG device_ID;
extern DPUCHAR device_rev;
extern DPUCHAR device_family;
extern DPULONG DataIndex;
extern DPUCHAR error_code;


/************************************************************/
/* Instrunction Set                                         */
/************************************************************/
/* General opcodes */
#define ISC_ENABLE               0x80u
#define ISC_DISABLE              0x81u
#define ISC_SAMPLE               0x01u

#define IDCODE                   0x0Fu
#define OPCODE_BIT_LENGTH        8u

#define IDCODE_LENGTH            32u

/* Action Names -- match actions function */
/* These codes are passed to the main entry function "dp_top" to indicate 
* which action to perform */
#define DP_NO_ACTION_FOUND                      0u
#define DP_DEVICE_INFO_ACTION_CODE              1u
#define DP_READ_IDCODE_ACTION_CODE              2u
#define DP_ERASE_ACTION_CODE                    3u
#define DP_PROGRAM_ACTION_CODE                  5u
#define DP_VERIFY_ACTION_CODE                   6u
/* Array only actions */
#define DP_ENC_DATA_AUTHENTICATION_ACTION_CODE  7u
#define DP_ERASE_ARRAY_ACTION_CODE              8u
#define DP_PROGRAM_ARRAY_ACTION_CODE            9u
#define DP_VERIFY_ARRAY_ACTION_CODE             10u
/* FROM only actions */
#define DP_ERASE_FROM_ACTION_CODE               11u
#define DP_PROGRAM_FROM_ACTION_CODE             12u
#define DP_VERIFY_FROM_ACTION_CODE              13u
/* Security only actions */
#define DP_ERASE_SECURITY_ACTION_CODE           14u
#define DP_PROGRAM_SECURITY_ACTION_CODE         15u
/* NVM only actions */
#define DP_PROGRAM_NVM_ACTION_CODE              16u
#define DP_VERIFY_NVM_ACTION_CODE               17u
#define DP_VERIFY_DEVICE_INFO_ACTION_CODE       18u
#define DP_READ_USERCODE_ACTION_CODE            19u
/* For P1 device, The following two actions are only supported with data files
* generated form V86 or later.  ENABLE_V85_DAT_SUPPORT must be disabled */
#define DP_PROGRAM_NVM_ACTIVE_ARRAY_ACTION_CODE 20u
#define DP_VERIFY_NVM_ACTIVE_ARRAY_ACTION_CODE  21u
#define DP_IS_CORE_CONFIGURED_ACTION_CODE       22u
#define DP_PROGRAM_PRIVATE_CLIENTS_ACTION_CODE  23u
#define DP_VERIFY_PRIVATE_CLIENTS_ACTION_CODE   24u
#define DP_PROGRAM_PRIVATE_CLIENTS_ACTIVE_ARRAY_ACTION_CODE  25u
#define DP_VERIFY_PRIVATE_CLIENTS_ACTIVE_ARRAY_ACTION_CODE   26u

/************************************************************/
/* Error code definitions                                   */
/************************************************************/
#define DPE_SUCCESS                   0u
#define DPE_PROCESS_DATA_ERROR        2u
#define DPE_IDCODE_ERROR              6u
#define DPE_CHECKSUM_ERROR            9u
#define DPE_INIT_FAILURE             25u
#define DPE_ERASE_ERROR               8u
#define DPE_CORE_PROGRAM_ERROR       10u
#define DPE_CORE_VERIFY_ERROR        11u
#define DPE_PROGRAM_RLOCK_ERROR      10u
#define DPE_POLL_ERROR                7u
#define DPE_PROGRAM_UROW_ERROR       24u
#define DPE_FROM_VERIFY_ERROR        20u
#define DPE_FROM_PROGRAM_ERROR       10u
#define DPE_DMK_VERIFY_ERROR         31u
#define DPE_UROW_ERROR               11u
#define DPE_VERIFY_ERROR             12u
#define DPE_NVM_PROGRAM_ERROR        41u
#define DPE_NVM_VERIFY_ERROR         39u
#define DPE_SEC_PROGRAMMING_ERROR    22u
#define DPE_AES_PROGRAMMING_ERROR    23u
#define DPE_ULOCK_ERROR              16u
#define DPE_MATCH_ERROR              35u
#define DPE_AUTHENTICATION_FAILURE   18u
#define DPE_CORE_ENC_ERROR           33u
#define DPE_CORE_PLAIN_ERROR         37u
#define DPE_FROM_ENC_ERROR           34u
#define DPE_FROM_PLAIN_ERROR         36u
#define DPE_NVM0_ENC_ERROR           47u
#define DPE_NVM0_PLAIN_ERROR         49u
#define DPE_NVM1_ENC_ERROR           47u
#define DPE_NVM1_PLAIN_ERROR         49u
#define DPE_NVM2_ENC_ERROR           47u
#define DPE_NVM2_PLAIN_ERROR         49u
#define DPE_NVM3_ENC_ERROR           47u
#define DPE_NVM3_PLAIN_ERROR         49u
#define DPE_USER_LOCK_SETTING_ERROR  42u
#define DPE_UROW_SETTING_ERROR       43u
#define DPE_AES_SEC_ERROR            31u
#define DPE_SILSIG_PROGRAM_ERROR     14u
#define DPE_PROGRAM_SECURITY_ERROR   38u
#define DPE_NVM_ACCESS_ERROR         54u
#define DPE_CRC_MISMATCH            100u
#define DPE_JTAG_STATE_NOT_HANDLED  110u
#define DPE_ACTION_NOT_FOUND        150u
#define DPE_ACTION_NOT_SUPPORTED    151u
#define DPE_CODE_NOT_ENABLED        152u
#define CALIBRATION_OVERLAP_ERROR   153u
#define DPE_SECURITY_BIT_MISMATCH   154u
#define DPE_DAT_VERSION_MISMATCH    160u
#define DPE_HARDWARE_NOT_SELECTED   170u
#define DPE_DAT_ACCESS_FAILURE      180u



/************************************************************/
/* Family code definitions                                  */
/************************************************************/
#define AXX_BIT         0x1u
#define AFS_BIT         0x2u
#define SFS_BIT         0x4u
#define A4P_BIT         0x8u

DPUCHAR dp_top(void);
void dp_read_idcode(void);
#ifdef ENABLE_DISPLAY
void dp_read_idcode_action(void);
#endif

#endif /* INC_DPALG_H */


