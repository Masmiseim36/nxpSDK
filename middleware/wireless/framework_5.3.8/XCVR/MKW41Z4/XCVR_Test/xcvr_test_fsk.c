/*!
* Copyright 2016-2017 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "fsl_xcvr.h"
#include "xcvr_test_fsk.h"

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */
enum {
    gDftNormal_c          = 0,
    gDftTxNoMod_Carrier_c = 1,
    gDftTxPattern_c       = 2,
    gDftTxRandom_c        = 3,
};

/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */
void XcvrFskModTx(void);
void XcvrFskNoModTx(void);
void XcvrFskIdle(void);
void XcvrFskTxRand(void);
void XcvrFskLoadPattern(uint32_t u32Pattern);
void XcvrFskSetTxPower(uint8_t u8TxPow);
void XcvrFskSetTxChannel(uint8_t u8TxChan);
void XcvrFskRestoreTXControl(void);
uint8_t XcvrFskGetInstantRssi(void);

/*! *********************************************************************************
* XcvrFskModTx
***********************************************************************************/
void XcvrFskModTx(void)
{
    XcvrFskIdle();
    XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                           XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
    XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxPattern_c) |
    XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK;
    XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
}

/*! *********************************************************************************
* XcvrFskNoModTx
***********************************************************************************/
void XcvrFskNoModTx(void)
{
    XcvrFskIdle();
    XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                           XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
    XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxNoMod_Carrier_c);
    XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
}

/*! *********************************************************************************
* XcvrFskIdle
***********************************************************************************/
void XcvrFskIdle(void)
{
    XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_TX_DFT_EN_MASK | 
                           XCVR_TX_DIG_CTRL_LFSR_EN_MASK | 
                           XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK);
    XCVR_TSM->CTRL &= ~XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
    XCVR_MISC->DTEST_CTRL &= ~XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
}

/*! *********************************************************************************
* XcvrFskTxRand
***********************************************************************************/
void XcvrFskTxRand(void)
{
    XcvrFskIdle();
    XCVR_TX_DIG->CTRL &= ~(XCVR_TX_DIG_CTRL_DFT_CLK_SEL_MASK |
                           XCVR_TX_DIG_CTRL_RADIO_DFT_MODE_MASK |
                           XCVR_TX_DIG_CTRL_LFSR_LENGTH_MASK);
    XCVR_TX_DIG->CTRL |= XCVR_TX_DIG_CTRL_RADIO_DFT_MODE(gDftTxRandom_c) |
                         XCVR_TX_DIG_CTRL_LFSR_LENGTH(0) | /* length 9 */
                         XCVR_TX_DIG_CTRL_LFSR_EN_MASK;
    XCVR_MISC->DTEST_CTRL |= XCVR_CTRL_DTEST_CTRL_DTEST_EN_MASK;
    XCVR_TSM->CTRL |= XCVR_TSM_CTRL_FORCE_TX_EN_MASK;
}

/*! *********************************************************************************
* XcvrFskLoadPattern
***********************************************************************************/
void XcvrFskLoadPattern(uint32_t u32Pattern)
{
    XCVR_TX_DIG->DFT_PATTERN = u32Pattern;
}

/*! *********************************************************************************
* XcvrFskGetInstantRssi
***********************************************************************************/
uint8_t XcvrFskGetInstantRssi(void)
{
    uint8_t u8Rssi;
    uint32_t t1,t2,t3;
    t1 = XCVR_RX_DIG->RX_DIG_CTRL;
    t2 = XCVR_RX_DIG->RSSI_CTRL_0;
    t3 = XCVR_PHY->CFG1;
    XCVR_RX_DIG->RX_DIG_CTRL = XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_NEGEDGE(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_CH_FILT_BYPASS(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_RAW_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_ADC_POL(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_OSR(1) | /* 1=OSR8, 2=OSR16, 4=OSR32 */
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_FSK_ZB_SEL(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_NORM_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_RSSI_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_CAL_EN(1) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_IQ_SWAP(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_SRC_RATE(0) | /* Source Rate 0 is default */
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DMA_DTEST_EN(0) |
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_GAIN(22) | /* Dec filt gain for SRC rate ==  0 */
                               XCVR_RX_DIG_RX_DIG_CTRL_RX_DEC_FILT_HZD_CORR_DIS(1) ;

    XCVR_RX_DIG->RSSI_CTRL_0 &= ~XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_WEIGHT_MASK;
    XCVR_RX_DIG->RSSI_CTRL_0 |=  XCVR_RX_DIG_RSSI_CTRL_0_RSSI_IIR_WEIGHT(0x5);

    XCVR_RX_DIG->RSSI_CTRL_0 &= ~XCVR_RX_DIG_RSSI_CTRL_0_RSSI_N_WINDOW_AVG_MASK;
    XCVR_RX_DIG->RSSI_CTRL_0 |=  XCVR_RX_DIG_RSSI_CTRL_0_RSSI_N_WINDOW_AVG(0x3); 

    uint32_t temp = XCVR_PHY->CFG1;
    temp &= ~XCVR_PHY_CFG1_CTS_THRESH_MASK;
    temp |= XCVR_PHY_CFG1_CTS_THRESH(0xFF);
    XCVR_PHY->CFG1 = temp;

    XCVR_ForceRxWu();
    for(uint32_t i = 0; i < 10000; i++)
    {
        __asm("nop");
    }
    u8Rssi = (uint8_t)((XCVR_RX_DIG->RSSI_CTRL_1 & 
                        XCVR_RX_DIG_RSSI_CTRL_1_RSSI_OUT_MASK) >> 
                        XCVR_RX_DIG_RSSI_CTRL_1_RSSI_OUT_SHIFT);
    XCVR_ForceRxWd();

    XCVR_RX_DIG->RX_DIG_CTRL = t1;
    XCVR_RX_DIG->RSSI_CTRL_0 = t2;
    XCVR_PHY->CFG1 = t3;
    return u8Rssi;
}

/*! *********************************************************************************
* XcvrFskSetTxPower
***********************************************************************************/
void XcvrFskSetTxPower(uint8_t u8TxPow)
{
    return;
}

/*! *********************************************************************************
* XcvrFskSetTxChannel
***********************************************************************************/
void XcvrFskSetTxChannel(uint8_t u8TxChan)
{
    return;
}

/*! *********************************************************************************
* XcvrFskRestoreTXControl
* After calling this function user should switch to
* previous protocol and set the protocol channel to default
***********************************************************************************/
void XcvrFskRestoreTXControl(void)
{
    return;
}

