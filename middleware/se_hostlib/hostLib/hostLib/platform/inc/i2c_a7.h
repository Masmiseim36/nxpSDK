/**
 * @file i2c_a7.h
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
 *
 * I2C API used by SCI2C & T=1 over I2C protocol implementation.
 *
 * - SCIIC / SCI2C is the protocol used by A71CH / A71CL family of secure elements.
 *
 * - T=1 over I2C is the protocol used by SE050 family of secure elements.
 *
 * - T=1 over I2C  with GP is the protocol used by SN050  & SE051 family of secure elements.
 *
 * These APIs are to be implemented when porting the Middleware stack to a new
 * host platform.
 *
 * @note Few APIs are only required for the SCI2C protocol and few are only
 *       needed for T=1 over I2C Protocol.  They are marked by the defines
 *       ``SCI2C`` ,  ``T1oI2C`` and ``T1oI2C_GP``
 *
 * # Convention of the APIs.
 *
 *
 * APIs for which a buffer is input. e.g.::
 *
 *   i2c_error_t axI2CWrite(unsigned char bus, unsigned char addr,
 *   unsigned char * pTx, unsigned short txLen);
 *
 *
 * In the above case :samp:`pTx` is a buffer input.  It is assumed that
 * the lengh as set in :samp:`txLen` is same as that pointed to by
 * :samp:`pTx`.  This parameter is used as is and any mistake by the
 * calling/implemented API will have unpredictable errors.
 *
 *
 * APIs for which a buffer is output. e.g.::
 *
 * i2c_error_t axI2CWriteRead(unsigned char bus,
 *     unsigned char addr,
 *     unsigned char *pTx,
 *     unsigned short txLen,
 *     unsigned char *pRx,
 *     unsigned short *pRxLen);
 *
 *
 * In the above case :samp:`pRx` is a buffer output and :samp:`pRxLen`
 * is both input and output. It is assumed that the lengh as set in
 * :samp:`pRxLen` is set to the maximum as available to the pointer
 * pointed by :samp:`pRx`.  This parameter is used as is and any mistake
 * by the calling/implemented API will have unpredictable errors.
 *
 * @par History
 *
 **/

#ifndef _I2C_A7_H
#define _I2C_A7_H

#include "sm_types.h"

#define SCI2C_T_CMDG 180 //!< Minimum delay between stop of Wakeup command and start of subsequent command (Value in micro seconds)

#define I2C_IDLE              0
#define I2C_STARTED           1
#define I2C_RESTARTED         2
#define I2C_REPEATED_START    3
#define DATA_ACK              4
#define DATA_NACK             5
#define I2C_BUSY              6
#define I2C_NO_DATA           7
#define I2C_NACK_ON_ADDRESS   8
#define I2C_NACK_ON_DATA      9
#define I2C_ARBITRATION_LOST  10
#define I2C_TIME_OUT          11
#define I2C_OK                12
#define I2C_FAILED            13

typedef unsigned int i2c_error_t;
#define I2C_BUS_0   (0)

#if defined(__cplusplus)
extern "C"{
#endif
/** Initialize the I2C platform HW/Driver*/

i2c_error_t axI2CInit( void );

/** Terminate / de-initialize the I2C platform HW/Driver
 *
 *
 * @param[in] mode Can be either 0 or 1.
 *
 *            Where applicable, and implemented a value of 0 corresponds
 *            to a 'light-weight' terminate.
 *
 *            In genral, this is not used for most of the porting
 *            platforms and use cases.
 *
 *
 */
void axI2CTerm(int mode);

#if AX_EMBEDDED
/** Smarter handling of back off logic
 *
 *  When we get a NAK from SE, we back off and keep on increasing the delay for next I2C Read/Write.
 *
 *  When we get an ACK from SE, we reset this back off delay.
 */
void axI2CResetBackoffDelay( void );
#endif /* FREEDOM */

#if defined(SCI2C) /* Means SCI2C SCIIC */
/** Write a byte.
 *
 * Needed only for SCI2C */
i2c_error_t axI2CWriteByte(unsigned char bus, unsigned char addr, unsigned char * pTx);
/** Write and read only after an ACK.
 *
 * Needed only for SCI2C */
i2c_error_t axI2CWriteRead(unsigned char bus,
    unsigned char addr,
    unsigned char *pTx,
    unsigned short txLen,
    unsigned char *pRx,
    unsigned short *pRxLen);
#endif

#if defined(SCI2C) /* Means SCI2C SCIIC */ || defined(T1oI2C)
/** Write a frame.
 *
 * Needed for SCI2C and T=1 over I2C */
i2c_error_t axI2CWrite(unsigned char bus, unsigned char addr, unsigned char * pTx, unsigned short txLen);
#endif

#ifdef T1oI2C
/** Read a byte.
 *
 * Needed only for T=1 over I2C */
i2c_error_t axI2CRead(unsigned char bus, unsigned char addr, unsigned char * pRx, unsigned short rxLen);
#endif /* T1oI2C */
#if defined(__cplusplus)
}
#endif

#endif // _I2C_A7_H
