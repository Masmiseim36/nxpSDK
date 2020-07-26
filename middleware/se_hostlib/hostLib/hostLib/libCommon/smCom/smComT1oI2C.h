/**
 * @file smComT1oI2C.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2018,2020 NXP
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
 * This file provides the API of the SmCom T1oI2C communication layer.
 *
 *****************************************************************************/

#ifndef _SMCOMT1OI2C_H_
#define _SMCOMT1OI2C_H_

#include "smCom.h"

/**
 * \ingroup spi_libese
 * \brief Ese Channel mode
 *
 */
typedef enum
{
    ESE_MODE_NORMAL = 0, /*!< All wired transaction other OSU */
    ESE_MODE_OSU /*!< Jcop Os update mode */
} phNxpEse_initMode;

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * closes  the T=1 o I2C communication layer.
 * @param conn_ctx  connection context
 * @param mode      Ese Communication mode either
 *                  ESE_MODE_NORMAL: All wired transaction other OSU or
 *                  ESE_MODE_OSU :Jcop Os update mode
 * @return
 */
U16 smComT1oI2C_Close(void *conn_ctx, U8 mode);

/**
 * @param conn_ctx  connection context
 * Reset  the T=1 o protocol instance.
 * @return
 */
U16 smComT1oI2C_ComReset(void *conn_ctx);

/**
 * Initializes or resumes the T=1 o I2C communication layer.
 * @param conn_ctx      IN: connection context
 * @param mode          Ese Communication mode either ESE_MODE_NORMAL: All wired transaction other OSU or ESE_MODE_OSU :Jcop Os update mode
 * @param T1oI2Catr     IN: Pointer to buffer to contain SCI2C_ATR value
 * @param T1oI2CatrLen  IN: Size of buffer provided; OUT: Actual length of atr retrieved
 * @return
 */
U16 smComT1oI2C_Open(void *conn_ctx, U8 mode, U8 seqCnt, U8 *T1oI2Catr, U16 *T1oI2CatrLen);

/**
* Open I2C device.
* @param conn_ctx      IN: pointer connection context
* @param pConnParam    IN: I2C address
* @return
*/
U16 smComT1oI2C_Init(void **conn_ctx, const char *pConnString);

#if defined(__cplusplus)
}
#endif
#endif /* _SMCOMT1OI2C_H_ */
