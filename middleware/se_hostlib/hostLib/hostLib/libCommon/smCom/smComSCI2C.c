/**
 * @file smComSCI2c.c
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
 * This file implements the SmCom SCI2C communication layer.
 *
 *****************************************************************************/

#include <assert.h>

#include "smComSCI2C.h"
#include "sci2c.h"
#include "i2c_a7.h"
#include "sm_printf.h"

// #define NX_LOG_ENABLE_SMCOM_DEBUG 1

#include "nxLog_smCom.h"
#include "nxEnsure.h"

static U32 smComSCI2C_Transceive(apdu_t *pApdu);
static U32 smComSCI2C_TransceiveRaw(U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen);

U16 smComSCI2C_Close(U8 mode)
{
    sci2c_TerminateI2C(mode);
    return SW_OK;
}

U16 smComSCI2C_Open(U8 mode, U8 seqCnt, U8 *SCI2Catr, U16 *SCI2CatrLen)
{
    eSci2c_Error_t st = eSci2c_Error;
    i2c_error_t i2c_status;
    U16 rv = SMCOM_PROTOCOL_FAILED;

    i2c_status = axI2CInit();
    if (i2c_status != I2C_OK) {
        return SMCOM_COM_FAILED;
    }

    if (mode == ESTABLISH_SCI2C) {
        st = sci2c_Init(SCI2Catr, SCI2CatrLen);
        if (st != eSci2c_No_Error) {
            LOG_E("sci2c_Init failed %x", st);
            *SCI2CatrLen = 0;
            return SMCOM_PROTOCOL_FAILED;
        }
    }
    else if (mode == RESUME_SCI2C) {
        LOG_I("Initializing SCI2C stack with Sequence Counter value: 0x%02X", seqCnt);
        sci2c_SetSequenceCounter(seqCnt);
        SCI2CatrLen = 0;
    }
    else {
        ENSURE_OR_GO_EXIT(0);
    }

    smCom_Init(&smComSCI2C_Transceive, &smComSCI2C_TransceiveRaw);
    rv = SMCOM_OK;
exit:
    return rv;
}

static U32 smComSCI2C_Transceive(apdu_t *pApdu)
{
    U32 status;
    LOG_MAU8_D("Tx>", pApdu->pBuf, pApdu->buflen);
    status = sci2c_Transceive(pApdu);
    if ((status == SMCOM_OK) && (pApdu->rxlen > 0))
        LOG_MAU8_D("<Rx", pApdu->pBuf, pApdu->rxlen);
    return status;
}

static U32 smComSCI2C_TransceiveRaw(U8 *pTx, U16 txLen, U8 *pRx, U32 *pRxLen)
{
    U32 status;
    LOG_MAU8_D("Tx>", pTx, txLen);
    status = sci2c_TransceiveRaw(pTx, txLen, pRx, pRxLen);
    LOG_MAU8_D("<Rx", pRx, *pRxLen);
    return status;
}
