/****************************************************************************/
/*                                                                          */
/*  DirectC			        Copyright (C) Actel Corporation 2007            */
/*  Version 3.0     Release date September 30, 2013                         */
/*                                                                          */
/****************************************************************************/
/*                                                                          */
/*  Module:         dpuser.h                                                */
/*                                                                          */
/*  Description:    DP user specific file                                   */
/*                   users should define their own functions                */
/*                                                                          */
/****************************************************************************/
#ifndef INC_DPUSER_H
#define INC_DPUSER_H


/*************** Hardware related constants *****************************/
/*
 * User Attention: 
 * Bit assignments in the hardware JTAG port register 
 * 
 */
#define TCK  0x8    /* User code ... */
#define TDI  0x1    /* User code ... */
#define TMS  0x4   /* User code ... */
#define TRST 0x0   /* User code ... */ /* 0 Means it does not exist */
#define TDO  0x2  	/* User code ... */
/*************** End of hardware related constants ************************/
/* Compiler switches */
#define ENABLE_DISPLAY
#define ENABLE_FILE_SYSTEM
#define ENABLE_GPIO_SUPPORT
#define ENABLE_DEBUG

//#define USE_PAGING
#define CHAIN_SUPPORT
/* Enable BSR_SAMPLE switch maintains the last known state of the IOs regardless 
*  of the data file setting. */
#define BSR_SAMPLE

//#define ENABLE_G3_SUPPORT
#define ENABLE_G4_SUPPORT

/*************** End of compiler switches ***********************************/


/***********************************************/
/* DPCHAR    -- 8-bit Windows (ANSI) character */
/*              i.e. 8-bit signed integer      */
/* DPINT     -- 16-bit signed integer          */
/* DPLONG    -- 32-bit signed integer          */
/* DPBOOL    -- boolean variable (0 or 1)      */
/* DPUCHAR   -- 8-bit unsigned integer         */
/* DPUSHORT  -- 16-bit unsigned integer        */
/* DPUINT    -- 16-bit unsigned integer        */
/* DPULONG   -- 32-bit unsigned integer        */
/***********************************************/
typedef unsigned char  DPUCHAR;
typedef unsigned short DPUSHORT;
typedef unsigned int   DPUINT;
typedef unsigned long  DPULONG;
typedef unsigned char  DPBOOL;
typedef          char  DPCHAR;
typedef          int   DPINT;
typedef          long  DPLONG;

#define DPNULL ((void*)0)
#define TRUE 1
#define FALSE 0

#define GPIO_SEL 1u
#define IAP_SEL 2u

extern DPUCHAR *image_buffer;
extern DPUCHAR hardware_interface;
extern DPUCHAR enable_mss_support;

DPUCHAR jtag_inp(void);
void jtag_outp(DPUCHAR outdata);
void dp_jtag_init(void);
void dp_jtag_tms(DPUCHAR tms);
void dp_jtag_tms_tdi(DPUCHAR tms, DPUCHAR tdi);
DPUCHAR dp_jtag_tms_tdi_tdo(DPUCHAR tms, DPUCHAR tdi);
void dp_delay(DPULONG microseconds);

#ifdef ENABLE_DEBUG
#define HEX 0
#define DEC 1
#define CHR 2

/******************************************************************************/
/* users should define their own functions to replace the following functions */
/******************************************************************************/
char *array2str(unsigned char *value, int bytes, int descriptive);
char *value2str(unsigned long value, int descriptive);

void dp_display_text(DPCHAR *text);
void dp_display_value(DPULONG value,int descriptive);
void dp_display_array(DPUCHAR *value,int bytes, int descriptive);
#endif


#ifdef ENABLE_FILE_SYSTEM
#endif
#endif /* INC_DPUSER_H */


