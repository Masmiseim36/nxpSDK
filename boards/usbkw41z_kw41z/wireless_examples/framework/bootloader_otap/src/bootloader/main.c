/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "main.h"
#if (gUseOTAPBootloader_d==1)
#include "OtapBootloader.h"
#endif /* (gUseOTAPBootloader_d==1) */
#if (gFsciIncluded_c==1)
#include "FsciBootloader.h"
#endif /* (gFsciIncluded_c==1) */
#if gFsciRxAck_c
#include "timer.h"
#endif


/*! *********************************************************************************
*************************************************************************************
* Private Memory Declarations
*************************************************************************************
********************************************************************************** */
/* ROM ISR vector table */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "IntVectTable"
__root vector_entry __vector_table[] =
{
    (pointer*)__BOOT_STACK_ADDRESS,  /* Initial SP           */

#elif defined(__GNUC__)
vector_entry __vector_table[] __attribute__ ((section(".vectortable"))) =
{
    (pointer*)__SP_INIT,  /* Initial SP           */
#endif

    __thumb_startup, /* Initial PC           */
    NMI_handler,      /* Non-maskable Interrupt (NMI) */
    defaultISR,      /* Hard Fault */
    defaultISR,      /* MemManage Fault */
    defaultISR,      /* Bus Fault */
    defaultISR,      /* Usage Fault */
    defaultISR,      /* Reserved */
    defaultISR,      /* Reserved */
    defaultISR,      /* Reserved */
    defaultISR,      /* Reserved */
    defaultISR,      /* SVC Handler */
    defaultISR,      /* Debug Monitor */
    defaultISR,      /* Reserved */
    defaultISR,      /* Pend SVC */
#if gFsciRxAck_c
    SysTickHandler
#else
    defaultISR       /* SysTick */
#endif
};


/* RAM ISR vector table */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "IntVectTableRam"
vector_entry __ram_vector_table[VECTOR_TABLE_SIZE];
#elif defined(__GNUC__)
vector_entry __ram_vector_table[VECTOR_TABLE_SIZE]  __attribute__ ((section(".ramvectortable")));
#endif


/* Flash Configuration */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "FlashConfig"
__root const FlashConfig_t gFlashConfig =
#elif defined(__GNUC__)
const FlashConfig_t gFlashConfig __attribute__ ((section(".cfmconfig"))) =
#endif
{
    {gFlashBackDoorKey1_d, gFlashBackDoorKey2_d, gFlashProtection_c, gFlashConfigOptions_d}
};


/* Bootloader Flasg */
#if gFsciIncluded_c
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "BootFlags"
__root volatile const bootFlags_t gBootFlags =
#elif defined(__GNUC__)
volatile const bootFlags_t gBootFlags __attribute__ ((section(".BootFlags"))) =
#endif
{
    0xFFFFFFFFFFFFFFFF
};
#endif

/* Bootloader switch debounce timeout */
#define gDebounceTimeout_c    255

/* Bootloader debounce shift register init value */
#define gDebounceShiftRegInitVal_c     0xF0F0F0F0UL

/*! *********************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
* \brief   Enable RAM vecor table, to allow instalation of ISRs
*
********************************************************************************** */
void EnableRamVectorTable(void)
{
    uint32_t i;

    for(i=0; i<sizeof(__vector_table)/sizeof(uint32_t*); i++)
    {
        __ram_vector_table[i] = __vector_table[i];
    }
    for(i=sizeof(__vector_table)/sizeof(uint32_t*); i<VECTOR_TABLE_SIZE; i++)
    {
        __ram_vector_table[i] = defaultISR;
    }

    SCB->VTOR = (uint32_t)__ram_vector_table;
    /* Enable ALL interrupts */
    __asm("     cpsie   i       ");
}

/*! *********************************************************************************
* \brief   Installs a handler function for the specified IRQ number.
*          If ROM vector table is used, VTOR will be updated to point to the RAM vector table
*
* \param[in] irqNo The IRQ number
* \param[in] handler The handler funcion for the specified IRQ
*
********************************************************************************** */
void InstallIrqHandler(IRQn_Type irqType, void (*handler)(void))
{
    __ram_vector_table[irqType + 16] = handler;
    NVIC_ClearPendingIRQ(irqType);
    NVIC_EnableIRQ(irqType);
}

/*! *********************************************************************************
* \brief  This function copies bytes from one buffer to another.
*
* \param[in, out]  pDst Pointer to the destination buffer.
* \param[in]  pSrc Pointer to the source buffer.
* \param[in]  cBytes Number of bytes to copy.
*
* \post  The source and destination buffers must not overlap.
*
********************************************************************************** */
void Boot_MemCpy (void* pDst, void* pSrc, uint32_t cBytes)
{
    while (cBytes)
    {
        *((uint8_t*)pDst) = *((uint8_t*)pSrc);
        pDst = ((uint8_t*)pDst)+1;
        pSrc = ((uint8_t*)pSrc)+1;
        cBytes--;
    }
}

/*! *********************************************************************************
* \brief   The function resets the MCU
*
********************************************************************************** */
void Boot_ResetMCU(void)
{
    NVIC_SystemReset();
    while(1);
}


/*! *********************************************************************************
* \brief   Start the user application
*
* \param[in] userStartup  Address of the application's interrupt vector
*
********************************************************************************** */
static void JumpToApplication(volatile uint32_t userStartup)
{
    /* Enable ALL interrupts */
    __asm("     cpsie   i       ");

    /* set up stack pointer */
    __asm("LDR      r1,  [r0]");
    __asm("MSR      MSP, r1");

    /* jump to application reset vector */
#if defined(__IAR_SYSTEMS_ICC__)
    __asm("ADDS     r0,r0,#0x04 ");
#else
    __asm("ADD      r0,r0,#0x04 ");
#endif
    __asm("LDR      r0, [r0]");
    __asm("BX       r0");
}


/*! *********************************************************************************
* \brief   This is the Bootloader's entry point
*
********************************************************************************** */
void __thumb_startup(void)
{
    (void)main(0, 0);
}

/*! *********************************************************************************
* \brief   This is the main Bootloader function.
*          It decides if it will load a new image or jump to the application
*
* \param[in] argc
* \param[in] argv
*
* \return int
*
********************************************************************************** */
int main(int argc, char **argv)
{
#if gFsciIncluded_c
    volatile uint32_t mDebounceShiftReg = gDebounceShiftRegInitVal_c;
    uint32_t mDebounceCnt = 0;
#endif
  
    /* Disable interrupt by default */
    __asm("     CPSID   i       ");

    /* Set the start address of the interrupt vector*/
    SCB->VTOR = (uint32_t)__vector_table;

    /* Disable watchdog */
#if defined(CPU_MKW24D512VHA5) || defined(MCU_MK21DX256) || defined(CPU_MK64FN1M0VLL12)
    WDOG->UNLOCK = 0xC520;
    WDOG->UNLOCK = 0xD928;
    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
#elif defined(CPU_MKL46Z256VLL4) || defined(MCU_MKW40Z160) || defined(CPU_MKW41Z512VHT4)
    SIM->COPC = SIM_COPC_COPT(0);
#endif

    
#if gFsciIncluded_c
    BOOT_PIN_ENABLE_SIM_SCG_REG |= BOOT_PIN_ENABLE_SIM_SCG_MASK;
    BOOT_PIN_ENABLE_PORT_BASE->PCR[BOOT_PIN_ENABLE_NUM] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;    

    /* Debounce the switch pin */
    do 
    {      
      mDebounceShiftReg |= (((BOOT_PIN_ENABLE_GPIO_BASE->PDIR & (1 << BOOT_PIN_ENABLE_NUM))>>BOOT_PIN_ENABLE_NUM) & 0x1);
      if((mDebounceShiftReg == 0xFFFFFFFF) || (mDebounceShiftReg == 0)) // pin stable values
      {
        break;
      }
      mDebounceShiftReg <<= 1;
    } while (mDebounceCnt++ <= gDebounceTimeout_c);
           
    if( (mDebounceShiftReg == 0) ||  /* GPIO trigger */
        (gBootFlags.newImageFlag != gBootFlagInvalid_d) ||        /* Flag set by Application */
        (*((uint32_t*)gUserFlashStart_d) < KINETIS_RAM_START) ||  /* Invalid Application */
        (*((uint32_t*)gUserFlashStart_d) > (KINETIS_RAM_END+1)) )     /* Invalid Application */
    {
        BOOT_ClockInit();
        FSCI_BOOT_main();
    }
#endif

#if gUseOTAPBootloader_d
    Boot_CheckOtapFlags();
#endif

    /* Set the start address of the interrupt vector*/
    SCB->VTOR = gUserFlashStart_d;
    JumpToApplication(gUserFlashStart_d);

    return 0;
}

/*! *********************************************************************************
* \brief   NMI handler function
*
********************************************************************************** */
void NMI_handler(void)
{
    /* Enable PORT Clock */
    NMI_PIN_SIM_SCG_REG |= NMI_PIN_SIM_SCG_MASK;
    /* Change PIN mux */
    NMI_PIN_PORT_BASE->PCR[NMI_PIN_NUM] = PORT_PCR_MUX(NMI_PIN_ALT);
}

/*! *********************************************************************************
* \brief   Default ISR handler
*
********************************************************************************** */
void defaultISR(void)
{
    /* ISR code */
    gHandleBootError_d();
}

/*! *********************************************************************************
* \brief   Initialize the MCU clock
*
********************************************************************************** */
void BOOT_ClockInit(void)
{
#if defined(CPU_MKW24D512VHA5) || defined(MCU_MK21DX256)
    /* KW24 and KW21 have the XCVR connected internally, and for this reason the XCVR 4MHz CLK_OUT
     * signal is used as input for the MCG, which will output 48MHz (PLL) */
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK; /* Ungate PORTB and PORTC clock*/
    GPIOB->PDDR |= 0x00080000u; /* Set PORTB.19 as output - XCVR RESET pin */
    GPIOC->PDDR |= 0x00000001u; /* Set PORTC.0 as output - XCVR GPIO5 pin */
    PORTB->PCR[19] = (PORTB->PCR[19] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(0x01u); /* PORTB.19 as GPIO */
    PORTC->PCR[0]  = (PORTC->PCR[0] & ~PORT_PCR_MUX_MASK) | PORT_PCR_MUX(0x01u); /* PORTC.0 as GPIO*/
    GPIOC->PCOR = 0x00000001u; /* Clear XCVR GPIO5 pin*/
    GPIOB->PCOR = 0x00080000u; /* Clear XCVR RESET pin*/
    GPIOB->PSOR = 0x00080000u; /* Set XCVR RESET pin*/

    /************************
    * Transition: FEI->FBE
    ************************/
    MCG->C2 = MCG_C2_RANGE0(2) | MCG_C2_HGO0_MASK | MCG_C2_IRCS_MASK;
    /* Select external oscillator and Reference Divider and clear IREFS to start external oscillator
    * CLKS = 2, FRDIV = 3, IREFS = 0, IRCLKEN = 0, IREFSTEN = 0 */
    MCG->C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);
    /* wait for Reference clock Status bit to clear */
    while (MCG->S & MCG_S_IREFST_MASK) {};
    /* The source of FLL reference clock is the external reference clock */
    /* Wait for clock status bits to show that clock source is External Reference Clock */
    while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) {};

    /************************
    * Transition: FBE->PBE
    ************************/
    MCG->C5 = MCG_C5_PRDIV0(1) | MCG_C5_PLLCLKEN0_MASK;  /* divide by 2 the external reference clock (4MHz /2 = 2MHz) used by PLL */
    /* Ensure MCG_C6 is at the reset default of 0 */
    MCG->C6 = 0x0;
    /* Set system options dividers
    * MCG = PLL, core = MCG, bus = MCG, FlexBus = MCG, Flash clock= MCG/2 */
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(0) | SIM_CLKDIV1_OUTDIV4(1);

    if(MCG->SC & MCG_SC_LOCS0_MASK)
    {
        MCG->SC |= MCG_SC_LOCS0_MASK;
    }
    /* Set the VCO divider and enable the PLL for 48MHz operation */
    MCG->C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0); /* VDIV = 0 (x24) */
    /* wait for PLL status bit to set */
    while (!(MCG->S & MCG_S_PLLST_MASK)) {};
    /* Wait for LOCK bit to set */
    while (!(MCG->S & MCG_S_LOCK0_MASK)){};

    /************************
    * Transition: PBE->PEE
    ************************/
    /* Transition into PEE by setting CLKS to 0
    * CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0 */
    MCG->C1 &= ~MCG_C1_CLKS_MASK;
    /* Wait for clock status bits to update */
    while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};
    /* Enable the ER clock of oscillators */
    OSC->CR = OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;

#elif defined(MCU_MKW40Z160) || defined(CPU_MKW41Z512VHT4)

    /* KW40 and KW41 will allways have an external 32MHz crystal oscilator for the XCVR.
       Setup MCG to use this XTAL oscilator for the MCU clock. */
#if defined(CPU_MKW41Z512VHT4)
    RSIM->ANA_TRIM |= RSIM_ANA_TRIM_BB_LDO_XO_TRIM_MASK;
    /* Enable RF OSC in RSIM and wait for ready */
    RSIM->CONTROL &= ~RSIM_CONTROL_RF_OSC_EN_MASK;
    RSIM->CONTROL |= RSIM_CONTROL_RF_OSC_EN(1);
    /* wait for RF_OSC_READY */
    while((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) == 0) {}
#endif
    /* Init Osc0 */
    SIM->CLKDIV1 = 0x00040000U;
    MCG->C2 &= ~(MCG_C2_EREFS_MASK | MCG_C2_HGO_MASK | MCG_C2_RANGE_MASK);
    MCG->C2 |= MCG_C2_RANGE(2);
    /* Set external ref clock */
    MCG->C7 &= ~MCG_C7_OSCSEL_MASK;
    /* Set to FBE mode. */
    MCG->C1 &= ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK);
    MCG->C1 |= MCG_C1_CLKS(2);
    /* Wait for MCG_S[CLKST] and MCG_S[IREFST]. */
    while ((MCG->S & (MCG_S_IREFST_MASK | MCG_S_CLKST_MASK)) != (MCG_S_CLKST(2))) {}
    /* In FBE now, start to enter BLPE. */
    MCG->C2 |= MCG_C2_LP_MASK;
    /* Set dividers */
    SIM->CLKDIV1 = 0x00010000U;
    /* Set LPUART clock */
    SIM->SOPT2 |= SIM_SOPT2_LPUART0SRC(2);
    
#elif defined(FRDM_K64F)
    /************************
    * Transition: FEI->FBE
    ************************/
    /* Enable the ER clock of oscillators */
    OSC->CR = OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
    MCG->C2 = MCG_C2_RANGE0(2) | MCG_C2_IRCS_MASK;
    /* Select external oscillator and Reference Divider and clear IREFS to start external oscillator */
    MCG->C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(0) | MCG_C1_IRCLKEN_MASK;
    /* wait for Reference clock Status bit to clear */
    while (MCG->S & MCG_S_IREFST_MASK) {};
    /* The source of FLL reference clock is the external reference clock */
    /* Wait for clock status bits to show that clock source is External Reference Clock */
    while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) {};

    /************************
    * Transition: FBE->PBE
    ************************/
    MCG->C5 = MCG_C5_PRDIV0(0xE) | MCG_C5_PLLCLKEN0_MASK;
    /* Ensure MCG_C6 is at the reset default of 0 */
    MCG->C6 = 0x0;
    /* Set system options dividers */
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV2(1) | SIM_CLKDIV1_OUTDIV3(2) | SIM_CLKDIV1_OUTDIV4(4);

    if(MCG->SC & MCG_SC_LOCS0_MASK)
    {
        MCG->SC |= MCG_SC_LOCS0_MASK;
    }
    /* Set the VCO divider and enable the PLL for 120MHz operation */
    MCG->C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0xC);
    /* wait for PLL status bit to set */
    while (!(MCG->S & MCG_S_PLLST_MASK)) {};
    /* Wait for LOCK bit to set */
    while (!(MCG->S & MCG_S_LOCK0_MASK)){};

    /************************
    * Transition: PBE->PEE
    ************************/
    /* Transition into PEE by setting CLKS to 0 */
    MCG->C1 &= ~MCG_C1_CLKS_MASK;
    /* Wait for clock status bits to update */
    while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) {};
#endif
}
//-----------------------------------------------------------------------------
