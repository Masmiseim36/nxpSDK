/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_common.h"
#include "clock_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* ARM PLL configuration for RUN mode */
const clock_arm_pll_config_t armPllConfig = {
  .loopDivider = 100U}; 

/* SYS PLL configuration for RUN mode */
const clock_sys_pll_config_t sysPllConfig = {
  .loopDivider = 1U};

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_BootClockGate(void)
{
    /* Disable all unused peripheral clock */  
    CCM->CCGR0 = 0x00C0000FU;
    CCM->CCGR1 = 0x30000000U;
    CCM->CCGR2 = 0x003F0030U;
    CCM->CCGR4 = 0x0000FF3CU;
    CCM->CCGR5 = 0xF000330FU;
    CCM->CCGR6 = 0x00FC0F00U;
}

static void BOARD_BootClockOutputs(void)
{
    /* Enable selected clocks generation on CLKO1 and CLKO2 pads */  
    CCM->CCOSR = 0x011B008C; 
}

void BOARD_BootClockRUN(void)
{
    /* Boot ROM did initialize the XTAL, here we only sets external XTAL OSC freq */
    CLOCK_SetXtalFreq(24000000U);
    CLOCK_SetRtcXtalFreq(32768U);
      
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0x1); /* Set PERIPH_CLK2 MUX to OSC */
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x1);     /* Set PERIPH_CLK MUX to PERIPH_CLK2 */

    CLOCK_InitArmPll(&armPllConfig);  /* Configure ARM PLL to 1200M */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 0x1); /* Set ARM PODF to 0, divide by 2 */
     
    CLOCK_InitSysPll(&sysPllConfig);  /* Configure SYS PLL to 528M */
      
    /* Configure clock for ADC - XBAR (IPG_CLK_ROOT) */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 0x1); /* Set ARM PODF to 0, divide by 2, 1200MHz/2 = 600MHz*/
    CLOCK_SetDiv(kCLOCK_AhbDiv, 0x0); /* Set AHB PODF to 0, divide by 1, 528MHz/1 = 528MHz */
    CLOCK_SetDiv(kCLOCK_IpgDiv, 0x3); /* Set IPG PODF to 3, divide by 4, 528MHz/4 = 132MHz */
    
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 0x0); /* Now ARM Core clock runs on SYS PLL at 528M */  
    CLOCK_SetMux(kCLOCK_PeriphMux, 0x0);

    /* Disable unused clock */
    BOARD_BootClockGate();

    /* Power down all unused PLL */    
    CLOCK_DeinitAudioPll();
    CLOCK_DeinitVideoPll();
    CLOCK_DeinitEnetPll();
    CLOCK_DeinitUsb2Pll();
    
    /* Configure UART divider to default */
    CLOCK_SetMux(kCLOCK_UartMux, 0U); /* Set UART source to PLL3 80M */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0U); /* Set UART divider to 1 */
    
    /* Update core clock */
    SystemCoreClockUpdate();
    BOARD_BootClockOutputs();
}
