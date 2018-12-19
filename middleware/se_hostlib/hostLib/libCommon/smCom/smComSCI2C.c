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

static U32 smComSCI2C_Transceive(apdu_t * pApdu);
static U32 smComSCI2C_TransceiveRaw(U8 * pTx, U16 txLen, U8 * pRx, U32 * pRxLen);

U16 smComSCI2C_Close(U8 mode)
{
    sci2c_TerminateI2C(mode);
    return SW_OK;
}

//
// smComSCI2C_Init is deprecated, please use smComSCI2C_Open
//
#ifndef TGT_A71CH
void smComSCI2C_Init(U8 mode, U8 seqCnt, U8 *SCI2Catr, U16 *SCI2CatrLen)
{
    eSci2c_Error_t st = eSci2c_Error;

    axI2CInit();

    if (mode == ESTABLISH_SCI2C)
    {
        st = sci2c_Init(SCI2Catr, SCI2CatrLen);
        if (st != eSci2c_No_Error)
        {
            sm_printf(DBGOUT, "sci2c_Init failed %x\r\n", st);
        }
    }
    else if (mode == RESUME_SCI2C)
    {
        sm_printf(DBGOUT, "Initializing SCI2C stack with Sequence Counter value: 0x%02X\r\n", seqCnt);
        sci2c_SetSequenceCounter(seqCnt);
        SCI2CatrLen = 0;
    }
    else
    {
        assert(0);
    }

    smCom_Init(&smComSCI2C_Transceive, &smComSCI2C_TransceiveRaw);
}
#endif

U16 smComSCI2C_Open(U8 mode, U8 seqCnt, U8 *SCI2Catr, U16 *SCI2CatrLen)
{
    eSci2c_Error_t st = eSci2c_Error;
    i2c_error_t i2c_status;

    i2c_status = axI2CInit();
    if (i2c_status != I2C_OK)
    {
        return SMCOM_COM_FAILED;
    }

    if (mode == ESTABLISH_SCI2C)
    {
        st = sci2c_Init(SCI2Catr, SCI2CatrLen);
        if (st != eSci2c_No_Error)
        {
            sm_printf(DBGOUT, "sci2c_Init failed %x\r\n", st);
            *SCI2CatrLen = 0;
            return SMCOM_PROTOCOL_FAILED;
        }
    }
    else if (mode == RESUME_SCI2C)
    {
        sm_printf(DBGOUT, "Initializing SCI2C stack with Sequence Counter value: 0x%02X\r\n", seqCnt);
        sci2c_SetSequenceCounter(seqCnt);
        SCI2CatrLen = 0;
    }
    else
    {
        assert(0);
    }

    smCom_Init(&smComSCI2C_Transceive, &smComSCI2C_TransceiveRaw);
    return SMCOM_OK;
}

static U32 smComSCI2C_Transceive(apdu_t * pApdu)
{
    return sci2c_Transceive(pApdu);
}

static U32 smComSCI2C_TransceiveRaw(U8 * pTx, U16 txLen, U8 * pRx, U32 * pRxLen)
{
    return sci2c_TransceiveRaw(pTx, txLen, pRx, pRxLen);
}
