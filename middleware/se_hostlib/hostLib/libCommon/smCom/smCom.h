/**
 * @file smCom.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
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

/* ------------------------------------------------------------------------- */
typedef U32 (*ApduTransceiveFunction_t) (apdu_t * pAdpu);
typedef U32 (*ApduTransceiveRawFunction_t) (U8 * pTx, U16 txLen, U8 * pRx, U32 * pRxLen);

void smCom_Init(ApduTransceiveFunction_t pTransceive, ApduTransceiveRawFunction_t pTransceiveRaw);
U32 smCom_Transceive(apdu_t *pApdu);
U32 smCom_TransceiveRaw(U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen);

#ifdef __cplusplus
}
#endif
#endif
