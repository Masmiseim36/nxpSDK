/**
 * @file smCom.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2016 NXP
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
 * Interface of installable communication layer to exchange APDU's between Host and Secure Module.
 */

#ifndef _SCCOM_H_
#define _SCCOM_H_

#include "sm_types.h"
#include "apduComm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SMCOM_OK              0x9000  //!< Communication successful
#define SMCOM_SND_FAILED      0x7010  //!< Communication failed while sending data
#define SMCOM_RCV_FAILED      0x7011  //!< Communication failed while receiving data
#define SMCOM_COM_FAILED      0x7012  //!< Cannot open communication link with ax device
#define SMCOM_PROTOCOL_FAILED 0x7013  //!< APDU exchange protocol failed to be established successfully
#define SMCOM_NO_ATR          0x7014  //!< No ATR can be retrieved
#define SMCOM_NO_PRIOR_INIT   0x7015  //!< The callbacks doing the actual transfer have not been installed
#define SMCOM_COM_ALREADY_OPEN      0x7016  //!< Communication link is already open with device


/* ------------------------------------------------------------------------- */
typedef U32 (*ApduTransceiveFunction_t) (apdu_t * pAdpu);
typedef U32 (*ApduTransceiveRawFunction_t) (U8 * pTx, U16 txLen, U8 * pRx, U32 * pRxLen);

void smCom_Init(ApduTransceiveFunction_t pTransceive, ApduTransceiveRawFunction_t pTransceiveRaw);
void smCom_DeInit(void);
U32 smCom_Transceive(apdu_t *pApdu);
U32 smCom_TransceiveRaw(U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen);

#ifdef __cplusplus
}
#endif
#endif
