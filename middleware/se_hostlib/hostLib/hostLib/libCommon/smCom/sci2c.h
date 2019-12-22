/**
 * @file sci2c.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2017 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * API of sci2c stack.
 * @par History
 *
 **/

#ifndef _SCI2C_H_
#define _SCI2C_H_

#include "sm_types.h"

#define SW_LENGTH                    (0x02) // ISO 7816-3 Status Word length = 2 bytes
// #define APDU_HEADER_LENGTH           (5)
// #define APDU_EXTENDED_HEADER_LENGTH  (7)

#define LOW_LEVEL_TAG           (0xB8)
#define PROTOCOL_BINDING_TAG    (0xB9)
#define HIGHER_LAYER_TAG        (0xBA)
#define OPERATING_SYS_TAG       (0xBB)
#define IDENTIFICATION_TAG      (0xBC)

/* protocol control byte codes */
#define PCB_DATA_EXCH           (0x00)
#define PCB_WAKEUP              (0x0F)
#define PCB_SOFT_RESET          (0x1F)
#define PCB_READ_ATR            (0x2F)
#define PCB_PARAM_EXCH          (0x3F)
#define PCB_PROT_BIND_SELECT    (0x03)
#define PCB_STATUS              (0x07)
#define PCB_UNUSED              (0x0B)

/* status codes */
#define SCI2C_STATUS_NORMAL_READY               (0x0)
#define SCI2C_STATUS_NORMAL_BUSY                (0x1)
#define SCI2C_STATUS_UNDEFINED                  (0x4) // Not defined in SCI2C spec, used in implementation of Host Sw.
#define SCI2C_STATUS_EXCEPTION_NO_PRECISE_DIAG  (0x8)
#define SCI2C_STATUS_EXCEPTION_OVERCLOCKING     (0x9)
#define SCI2C_STATUS_EXCEPTION_UNEXPECTED_SEQ   (0xA)
#define SCI2C_STATUS_EXCEPTION_INVALID_DATA_LEN (0xB)
#define SCI2C_STATUS_EXCEPTION_UNEXPECTED_CMD   (0xC)
#define SCI2C_STATUS_EXCEPTION_INVALID_EDC      (0xD)
#define SCI2C_STATUS_EXCEPTION_OTHER            (0xF)


/* ------------------------------------------------------------------------- */
/* i2c_status_t indicates the result of the I2C command transmission */
typedef enum
{
    i2c_Okay,      /* the command has been transmitted succesfully */
    i2c_NoAddrAck, /* the slave does not acknowledge the address byte */
    i2c_Failed,    /* an error or timeout occured, the command has not succesfully been transmitted */
    i2c_Sci2cException /* PCB indicates protocol exception */
} i2c_status_t;

typedef enum
{
    eSci2c_No_Error = 0,
    eSci2c_Protocol_Error,
    eSci2c_Error,
    eSci2c_NackOnAddr,      // Constant value was not used earlier
    eSci2c_Not_Implemented
} eSci2c_Error_t;

typedef enum
{
    eSci2c_DirectionM2S = 0,/* ------00b */
    eSci2c_DirectionS2M = 2 /* ------10b */
} eSci2c_Direction_t;

typedef enum
{
    eEdc_NoErrorDetection   = 0x00, /* ----00--b */
    eEdc_LRCErrorDetection  = 0x04  /* ----01--b */
} eSci2c_Edc_t;

typedef enum
{
   eMax32BytesM2S = 0,
   eMax64BytesM2S,
   eMax128BytesM2S,
   eMax255BytesM2S
} sci2c_maxDataBytesM2S_t;

typedef enum
{
   eMax31BytesS2M = 0,
   eMax63BytesS2M = 1,
   eMax127BytesS2M = 2,
   eMax254BytesS2M = 3
} sci2c_maxDataBytesS2M_t;

/* structure to be filled in and pass to sci2c_init() */
typedef struct
{
   U8 i2cAddr;          /* the i2c address of the device authentication chip */
   eSci2c_Edc_t edc;    /* the error detection code to be used */
   sci2c_maxDataBytesS2M_t maxBytes;
} sci2c_Config_t;

typedef struct
{
    U8 length;
    U8 sdpvMa;      /* major slave device protocol version */
    U8 sdpvMi;      /* minor slave device protocol version */
    U8  edc;        /* error detection codes */
    U8 fwi;     /* frame waiting integer */
    U8 comm_speed;
} sci2c_LowLevelProtocol_t;

typedef struct
{
    U8 length;
    U8 suppProtBindings;
    U8  defaultSelectedProt;
} sci2c_ProtocolBinding_t;

typedef struct
{
    U8 length;
    U8 extendedApduSupport;
    U32 maxApduLength;
} sci2c_HigherLayer_t;

typedef struct
{
    U8 length;
    U8 data[16];
} sci2c_OperatingSystem_t;

typedef struct
{
    U8 length;
    U8 data[16];
} sci2c_Identification_t;

typedef struct
{
    sci2c_LowLevelProtocol_t    llProtocol;
    sci2c_ProtocolBinding_t     protocolBinding;
    sci2c_HigherLayer_t         hghLayer;
    sci2c_OperatingSystem_t     opSys;
    sci2c_Identification_t      id;
} sci2c_ATR_t;

/* interface to lower layers */
typedef struct
{
    U8 pcb;
    U8 dataLen; // only data, excl. pcb (max 255)
    U8 * pData;
} sci2c_Data_t;

/* interface to upper layers */
typedef struct
{
    eSci2c_Direction_t dir;
    eSci2c_Edc_t edc;
    U8 * pBuf;
    U16 buflen;
} tSCI2C_Data_t;

/* ------------------------------------------------------------------------- */
eSci2c_Error_t sci2c_Init(U8 *SCI2Catr, U16 *SCI2CatrLen);
U32 sci2c_Transceive(apdu_t * pApdu);
U32 sci2c_TransceiveRaw(U8 * pTx, U16 txLen, U8 * pRx, U32 * pRxLen);
void sci2c_SetSequenceCounter(U8 seqCounter);
U8 sci2c_GetSequenceCounter(void);
void sci2c_TerminateI2C(U8 full);

#endif //_SCI2C_H_
