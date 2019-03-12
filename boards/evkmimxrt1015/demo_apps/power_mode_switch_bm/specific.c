/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "specific.h"
#include "fsl_common.h"
#include "fsl_clock.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

void SwitchToRunModePLLs()
{
    /* set the right Div value */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0x1);

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);

    /*ENET PLL as clksource, AHB 500M, IPG 167M, PERCLK 83M*/
    CCM_ANALOG->PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_POWERDOWN_MASK;
    CCM_ANALOG->PLL_ENET_SET = CCM_ANALOG_PLL_ENET_ENET_500M_REF_EN_MASK | CCM_ANALOG_PLL_ENET_BYPASS_MASK;
    CCM_ANALOG->PLL_ENET_CLR = CCM_ANALOG_PLL_ENET_BYPASS_MASK;

    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x3);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);
    CLOCK_SetMux(kCLOCK_PerclkMux, 0x0);
}

void SwitchToFullRunPLLs()
{
    /* set the right Div value */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0x0);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x2);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0x1);

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);

    /* PLL528 PFD3  as clksource AHB 396M, IPG 132M, 66M*/
    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_ENABLE_MASK | CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_BYPASS_MASK;

    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_FRAC_MASK;
    CCM_ANALOG->PFD_528_SET = CCM_ANALOG_PFD_528_PFD3_FRAC(24);
    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_CLKGATE_MASK;

    /* Select PLL2 PFD3 for pre_periph_clock */
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x2);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);
    CLOCK_SetMux(kCLOCK_PerclkMux, 0x0);
}

void SwitchToLowSpeedRunPLLs()
{
    /* set the right Div value */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0x0);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x1);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0x0);

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);

    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_ENABLE_MASK | CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_BYPASS_MASK;

    /*Select PLL528 PFD3 for pre_periph_clock 396MHz,SET AHB 132MHz */
    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_FRAC_MASK;
    CCM_ANALOG->PFD_528_SET = CCM_ANALOG_PFD_528_PFD3_FRAC(24);
    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_CLKGATE_MASK;

    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x2);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);
    CLOCK_SetMux(kCLOCK_PerclkMux, 0x0);
}

void SwitchToLowPowerRunPLLs()
{
    /* set the right Div value */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0x0);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x1);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0x0);
    CLOCK_SetDiv(kCLOCK_SemcDiv, 0x0);

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);

    /*Select SYSTEM PLL BYPASS for pre_periph_clock */
    /* Set PERCLK_CLK_ROOT to 12Mhz */
    CCM_ANALOG->PLL_SYS |= CCM_ANALOG_PLL_SYS_ENABLE_MASK | CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    /* Power off SYSTEM PLL */
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;

    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x0);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);
    CLOCK_SetMux(kCLOCK_PerclkMux, 0x0);
    CLOCK_SetMux(kCLOCK_SemcMux, 0x0);
}

void SwitchToSystemIdlPLLs()
{
    /* set the right Div value */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0x0);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x2);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0x0);

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);

    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_ENABLE_MASK | CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    CCM_ANALOG->PLL_SYS_CLR = CCM_ANALOG_PLL_SYS_BYPASS_MASK;

    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_FRAC_MASK;
    CCM_ANALOG->PFD_528_CLR = CCM_ANALOG_PFD_528_PFD3_CLKGATE_MASK;
    CCM_ANALOG->PFD_528_SET = CCM_ANALOG_PFD_528_PFD3_FRAC(24);

    /*Select PLL528 PFD3 for pre_periph_clock*/
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 2);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0);

    /* SET AHB 132MHz, IPG 33MHz, PER clock to 33MHz */
    CLOCK_SetMux(kCLOCK_PerclkMux, 0);
}

void SwitchToLPIdlPLLs()
{
    /* set the right Div value */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0x0);
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x0);
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x1);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 0x0);
    CLOCK_SetDiv(kCLOCK_SemcDiv, 0x0);

    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1);
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);

    CCM_ANALOG->PLL_SYS |= CCM_ANALOG_PLL_SYS_ENABLE_MASK | CCM_ANALOG_PLL_SYS_BYPASS_MASK;
    /* Power off SYSTEM PLL */
    CCM_ANALOG->PLL_SYS_SET = CCM_ANALOG_PLL_SYS_POWERDOWN_MASK;

    /*Select SYSTEM PLL BYPASS for pre_periph_clock */
    /* SET AHB 12MHz, IPG 12MHz, PER clock to 12MHz */
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x0);
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);
    CLOCK_SetMux(kCLOCK_PerclkMux, 0x0);
    CLOCK_SetMux(kCLOCK_SemcMux, 0x0);
}

void SwitchFlexspi2Clock()
{
    ;

    /* if used internal flash just like flexspi2,
     * please use the follow code for this function.
     */
#if 0
    /* Disable clock gate of flexspi2. */
    CCM->CCGR7 &= (~CCM_CCGR7_CG1_MASK);

    /* SYSTEM BYPASS will be used as flexspi2 clock. */
    CLOCK_SET_MUX(kCLOCK_Flexspi2Mux, 0x3);
    /* Set divider for flexspi2 clock root 0. */
    CLOCK_SET_DIV(kCLOCK_Flexspi2Div, 0x7U);

    /* Enable clock gate of flexspi2. */
    CCM->CCGR7 |= (CCM_CCGR7_CG1_MASK);
#endif
}

void RestoreFlexspi2Clock()
{
    ;
    /* if used internal flash just like flexspi2,
     * please use the follow code for this function.
     */
#if 0
    /* Disable clock gate of flexspi2. */
    CCM->CCGR7 &= (~CCM_CCGR7_CG1_MASK);

    /* PLL3 PFD0 will be used as flexspi2 clock. */
    CLOCK_SET_MUX(kCLOCK_FlexspiMux, 0x1);
    /* Set divider for flexspi2 clock root 0. */
    CLOCK_SET_DIV(kCLOCK_FlexspiDiv, 0x7);

    /* Enable clock gate of flexspi2. */
    CCM->CCGR7 |= (CCM_CCGR7_CG1_MASK);
#endif
}
