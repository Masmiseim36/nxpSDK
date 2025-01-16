/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "ezhv_api.h"

#if (EZHV_DVP_EN)
#include "fsl_flexio.h"
#include "fsl_gpio.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Gets the shifter status flags.
 *
 * @param base FlexIO peripheral base address
 * @return Shifter status flags
 */
static inline void FLEXIO_ClrShifterStatusFlags(FLEXIO_Type *base)
{
    FLEXIO_ClearShifterStatusFlags(base, 0xFF);
    FLEXIO_ClearShifterErrorFlags(base, 0xFF);
}

/*!
 * @brief Initialize BLC and Event Manager
 */
static void EZHV_DvpInit(void)
{
    /* Step1: BLC setting */
    /* clear blc cfg and sel registers */
    EZHV_WriteBlcCfg(0);
    EZHV_WriteBlcSel(0);
    /* configure BLC part, blc0, trigger0, both edge */
    EZHV_EnableBlc(kBLC_IDX_0, kBLC_CFG_RISING_EDGE, kBLC_SEL_INPUT_0); /* enable blc xsync */
    EZHV_EnableBlc(kBLC_IDX_1, kBLC_CFG_RISING_EDGE, kBLC_SEL_INPUT_1); /* enable blc flexio */
    /* enable blc slice 0 status event by setting BLC_OR_EN bit */
    EZHV_EnableBlcEvent(kBLC_IDX_0);
    EZHV_EnableBlcEvent(kBLC_IDX_1);

    /* Step2 EM setting */
    /* clear cfg0, cfg1 registers */
    EZHV_DeInitEmCfg();
    /* enable blc status in Event Management */
    EZHV_EnableEmNormalEvent(EZHV_EM_BLC_EN_MSK(kBLC_IDX_0));
    EZHV_EnableEmNormalEvent(EZHV_EM_BLC_EN_MSK(kBLC_IDX_1));

    EZHV_EnableEmWakeExitEn();
    //EZHV_EnableEmEventAutoClear();
}

void *dspv_memcpy(void *dst, const void *src, uint32_t size);

/*! @brief use FlexIO as parallel camera interface */
void EZHV_FlexioCameraDvp(uint32_t paraAddr)
{
    assert(0 != paraAddr);
 
    ezhv_para_dvp_t *pst_dvp_buf = (ezhv_para_dvp_t*)paraAddr;
    uint32_t loopCnt = 0;
    uint32_t channel = 1;
    uint32_t status = 0;
    uint32_t *pAddr = NULL;
    uint32_t loop = 0;

    /* flexio camera initialization */
    EZHV_DvpInit();
    GPIO_GpioClearInterruptFlags(GPIO2, 1<<5UL);
    while(1)
    {
        /* wait for the vsync signal */
        wfi();
        status = EZHV_GetEmEventStat();
        if (status & kEM_EventBLC1)
        {
            /* this is a flexio interrupt */
            FLEXIO_ClrShifterStatusFlags(FLEXIO);
            continue;
        }
        GPIO_GpioClearInterruptFlags(GPIO2, 1<<5UL);
        /* wait for Cm33 to update buff base ptr and buff len */
        if ((0 == pst_dvp_buf->len) || (NULL == pst_dvp_buf->pBuf))
        {
            //XZMSG_IndIntChan(2);
            continue;              
        }

        pAddr = (uint32_t *)(pst_dvp_buf->pBuf);
        loop = (pst_dvp_buf->len) >> 5;

        FLEXIO_ClrShifterStatusFlags(FLEXIO);
        while (1)
        {
            wfi();
            status = EZHV_GetEmEventStat();
            /* this is a vsync interrupt */
            if (status & kEM_EventBLC0)
            {/* if this is not a complete frame, discard current one */
                loopCnt = 0;
                //bfcnt = 0;
                pAddr = (uint32_t *)(pst_dvp_buf->pBuf);
                GPIO_GpioClearInterruptFlags(GPIO2, 1<<5U);
                continue;
            }
            FLEXIO_ClrShifterStatusFlags(FLEXIO);
            *(pAddr++) = FLEXIO->SHIFTBUF[0];
            *(pAddr++) = FLEXIO->SHIFTBUF[1];
            *(pAddr++) = FLEXIO->SHIFTBUF[2];
            *(pAddr++) = FLEXIO->SHIFTBUF[3];
            *(pAddr++) = FLEXIO->SHIFTBUF[4];
            *(pAddr++) = FLEXIO->SHIFTBUF[5];
            *(pAddr++) = FLEXIO->SHIFTBUF[6];
            *(pAddr++) = FLEXIO->SHIFTBUF[7];
            if (++loopCnt == loop)
            {
                pst_dvp_buf->len = 0;
                pst_dvp_buf->pBuf = NULL;
                XZMSG_IndIntChan(channel);//ezhv to arm irq
                loopCnt = 0;
                break;
            }
        }
    }
}
#endif
