/**
 * @file smCom.c
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
 * Implements installable communication layer to exchange APDU's between Host and Secure Module.
 * Allows the top half of the Host Library to be independent of the actual interconnect
 * between Host and Secure Module
 */
#include <stdio.h>
#include "smCom.h"
#include "nxLog_smCom.h"

#if (__GNUC__  && !AX_EMBEDDED)
#include<pthread.h>
    /* Only for base session with os */
    static pthread_mutex_t gSmComlock;
#endif

#if (__GNUC__  && !AX_EMBEDDED)
#define LOCK_TXN() \
    LOG_D("Trying to Acquire Lock thread: %ld", pthread_self()); \
    pthread_mutex_lock(&gSmComlock); \
    LOG_D("LOCK Acquired by thread: %ld", pthread_self());

#define UNLOCK_TXN() \
    LOG_D("Trying to Released Lock by thread: %ld", pthread_self()); \
    pthread_mutex_unlock(&gSmComlock); \
    LOG_D("LOCK Released by thread: %ld", pthread_self());
#else
#define LOCK_TXN() LOG_D("no lock mode");
#define UNLOCK_TXN() LOG_D("no lock mode");
#endif

static ApduTransceiveFunction_t pSmCom_Transceive = NULL;
static ApduTransceiveRawFunction_t pSmCom_TransceiveRaw = NULL;

/**
 * Install interconnect and protocol specific implementation of APDU transfer functions.
 *
 */
void smCom_Init(ApduTransceiveFunction_t pTransceive, ApduTransceiveRawFunction_t pTransceiveRaw)
{

#if (__GNUC__  && !AX_EMBEDDED)
    if (pthread_mutex_init(&gSmComlock, NULL) != 0)
    { 
        LOG_E("\n mutex init has failed");
        return;
    }
    else {
        LOG_D("Mutext Init successfull");
    }
#endif
    pSmCom_Transceive = pTransceive;
    pSmCom_TransceiveRaw = pTransceiveRaw;
}

void smCom_DeInit(void)
{
#if (__GNUC__  && !AX_EMBEDDED)
    pthread_mutex_destroy(&gSmComlock);
#endif
}

/**
 * Exchanges APDU without interpreting the message exchanged
 *
 * @param[in,out] pApdu        apdu_t datastructure
 *
 * @retval ::SMCOM_OK          Operation successful
 * @retval ::SMCOM_SND_FAILED  Send Failed
 * @retval ::SMCOM_RCV_FAILED  Receive Failed
 */
U32 smCom_Transceive(apdu_t * pApdu)
{
	U32 ret = SMCOM_NO_PRIOR_INIT;
    if (pSmCom_Transceive != NULL)
    {
        LOCK_TXN();
        ret = pSmCom_Transceive(pApdu);
        UNLOCK_TXN();
    }
    return ret;
}

/**
 * Exchanges APDU without interpreting the message exchanged
 *
 * @param[in] pTx          Command to be sent to secure module
 * @param[in] txLen        Length of command to be sent
 * @param[in,out] pRx      IN: Buffer to contain response; OUT: Response received from secure module
 * @param[in,out] pRxLen   IN: [TBD]; OUT: Length of response received
 *
 * @retval ::SMCOM_OK          Operation successful
 * @retval ::SMCOM_SND_FAILED  Send Failed
 * @retval ::SMCOM_RCV_FAILED  Receive Failed
 */
U32 smCom_TransceiveRaw(U8 * pTx, U16 txLen, U8 * pRx, U32 * pRxLen)
{
	U32 ret = SMCOM_NO_PRIOR_INIT;
    if (pSmCom_TransceiveRaw != NULL)
    {
        LOCK_TXN();
        ret = pSmCom_TransceiveRaw(pTx, txLen, pRx, pRxLen);
        UNLOCK_TXN();
    }
    return ret;
}
