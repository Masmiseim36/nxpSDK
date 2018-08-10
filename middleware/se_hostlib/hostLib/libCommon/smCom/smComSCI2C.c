/**
 * @file smComSCI2c.c
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
