/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "main.h"
#include "clock_config.h"
#include "fsl_flash.h"
#include "FunctionLib.h"
#ifdef CPU_QN9080C
#include "fsl_iocon.h"
#endif

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
uint32_t gMcuFlashSize;
#if defined(__IAR_SYSTEMS_ICC__)
/* System clock frequency variable definition. */
uint32_t SystemCoreClock;
#endif

/* ROM ISR vector table */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "IntVectTable"
__root vector_entry __vector_table[] =
{
    (pointer*)__BOOT_STACK_ADDRESS,  /* Initial SP           */

#elif defined(__GNUC__)
const vector_entry __vector_table[] __attribute__ ((used, section(".isr_vector"))) =
{
    (pointer*)__SP_INIT,   /* Initial SP           */

#elif defined (__CC_ARM)
extern uint32_t Image$$RAM_ER$$Limit[];

#define __BOOT_STACK_ADDRESS ((pointer*)Image$$RAM_ER$$Limit)
vector_entry __vector_table[] __attribute__ ((section(".IntVectTable"))) =
{
    __BOOT_STACK_ADDRESS,  /* Initial SP           */

#endif

	ResetISR,             /* Initial PC           */
    NMI_handler,          /* Non-maskable Interrupt (NMI) */
    defaultISR,           /* Hard Fault */
    defaultISR,           /* MemManage Fault */
    defaultISR,           /* Bus Fault */
    defaultISR,           /* Usage Fault */
#ifdef CPU_QN9080C
    (pointer*)0x00000000, /* Checksum of the first 7 words */
    (pointer*)0x000AA8FF, /* CRP */
    (pointer*)0x00000000, /* Image type: Legacy = 0x00000000, Enhanced = 0xEDDC9494 */
    (pointer*)0x00000000, /* Pointer to image header */
#else
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
#endif
    defaultISR,           /* SVC Handler */
    defaultISR,           /* Debug Monitor */
    defaultISR,           /* Reserved */
    defaultISR,           /* Pend SVC */
#if gFsciRxAck_c
    SysTickHandler,
#else
    defaultISR,           /* SysTick */
#endif
#ifdef CPU_QN9080C
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
    defaultISR,           /* Reserved */
#endif
};


/* RAM ISR vector table */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "IntVectTableRam"
vector_entry __ram_vector_table[NUMBER_OF_INT_VECTORS];
#elif defined(__GNUC__)
vector_entry __ram_vector_table[NUMBER_OF_INT_VECTORS]  __attribute__ ((section(".ramvectortable")));
#elif defined(__CC_ARM)
vector_entry __ram_vector_table[NUMBER_OF_INT_VECTORS]  __attribute__ ((section(".IntVectTableRam")));
#endif

/* Flash Configuration */
#ifndef CPU_QN9080C
/* For Kinetis Wireless platform, the Flash Config section should always be placed in Flash. */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "FlashConfig"
__root const FlashConfig_t gFlashConfig =
#elif defined(__GNUC__)
const FlashConfig_t gFlashConfig __attribute__ ((section(".FlashConfig"))) =
#elif defined(__CC_ARM)
const FlashConfig_t gFlashConfig __attribute__ ((section(".FlashConfig"))) =
#endif
{
    {gFlashBackDoorKey1_d, gFlashBackDoorKey2_d, gFlashProtection_c, gFlashConfigOptions_d}
};

#elif gBootUseFlashProtection_c /* QN9080 */
/* On QN908x place Flash Config section only if Flash protection is enabled */
#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = "FlashConfig"
__root const FlashConfig_t gFlashConfig =
#elif defined(__GNUC__)
const FlashConfig_t gFlashConfig __attribute__ ((section(".FlashConfig"))) =
#elif defined(__CC_ARM)
const FlashConfig_t gFlashConfig __attribute__ ((section(".FlashConfig"))) =
#endif
{
    {gFlashblock0Lock_d, gFlashblock1Lock_d, gFlashProtection_c,}
};
#endif /* gBootUseFlashProtection_c */


/* Bootloader switch debounce timeout */
#define gDebounceTimeout_c    255

/* Bootloader debounce shift register init value */
#define gDebounceShiftRegInitVal_c     0xF0F0F0F0UL


#if defined(__IAR_SYSTEMS_ICC__)
#pragma location = ".BOOT_MODULE_INFO"
__root const moduleInfo_t mBootloaderInfo =
#elif defined(__GNUC__)
const moduleInfo_t mBootloaderInfo __attribute__ ((section(".BOOT_MODULE_INFO"), used)) =
#elif defined(__CC_ARM)
const moduleInfo_t mBootloaderInfo __attribute__ ((section(".BOOT_MODULE_INFO"), used)) =
#endif
{
  {'B', 'O', 'O', 'T'},
  gBootloaderVerString_c,
  gBootloaderModuleId_c,
  {gBootloaderVerMajor_c, gBootloaderVerMinor_c, gBootloaderVerPatch_c},
  gBootloaderBuildNo_c
};

flash_config_t gFTFx_config;

/*! *********************************************************************************
*************************************************************************************
* Public Functions
*************************************************************************************
********************************************************************************** */
#if defined(__CC_ARM)
extern void JumpToApplication(volatile uint32_t userStartup);
#endif

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
    for(i=sizeof(__vector_table)/sizeof(uint32_t*); i<NUMBER_OF_INT_VECTORS; i++)
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
/* Do not use static because of GCC optimization problems:
 * Could cause inline function and the R0 register used will have a bad value */
#if !defined(__CC_ARM)
void JumpToApplication(volatile uint32_t userStartup)
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
#endif /* !defined(__CC_ARM) */


/*! *********************************************************************************
* \brief   This is the Bootloader's entry point
*
********************************************************************************** */
void ResetISR(void)
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
    uint32_t temp;
    bootFlags_t *pBootInfo = (bootFlags_t*)gBootImageFlagsAddress_c;
#endif

#ifdef CPU_QN9080C
    //enable swd pins
    SYSCON->PIO_FUNC_CFG[2] = 0x00000000;
    
    //remap Flash region to address 0
    SYSCON->SYS_MODE_CTRL |= 0x1;
    
    //__BKPT(0); // break here, manually substract 0x21000000 from PC to enable debuging
#endif

    /* Disable interrupt by default */
    __asm("     CPSID   i       ");

    /* Set the start address of the interrupt vector*/
    SCB->VTOR = (uint32_t)__vector_table;

    /* Disable watchdog */
#if FSL_FEATURE_SOC_WDOG_COUNT
#if defined(CPU_QN9080C)
    /* Disable watchdog timer */
    WDT->LOCK = 0x1ACCE551;
    WDT->CTRL = 0;
    WDT->LOCK = 0;
#else
    WDOG->UNLOCK = 0xC520;
    WDOG->UNLOCK = 0xD928;
    WDOG->STCTRLH &= ~WDOG_STCTRLH_WDOGEN_MASK;
#endif
#elif FSL_FEATURE_SOC_COP_COUNT
    SIM->COPC = SIM_COPC_COPT(0);
#endif

    /* Set the maximum available MCU Flash size */
#if (defined(CPU_MKW36Z512VFP4) || defined(CPU_MKW36Z512VHT4))
    /* Check if FlexNVM is disabled */
    if (((SIM->FCFG2 & SIM_FCFG2_MAXADDR1_MASK) >> SIM_FCFG2_MAXADDR1_SHIFT) == 0x20U)
    {
        /* Update the Flash size to use FlexNVM as program Flash. */
        gMcuFlashSize = (FSL_FEATURE_FLASH_PFLASH_BLOCK_SIZE + FSL_FEATURE_FLASH_FLEX_NVM_BLOCK_SIZE);
    }
    else
#endif
    {
        gMcuFlashSize = gMcuFlashSize_c;
    }

    /* Initialize the FLASH config structure to 0 */
    FLib_MemSet (&gFTFx_config, 0x00, sizeof(gFTFx_config));
    
#if gFsciIncluded_c
  #if !defined(CPU_QN9080C)
    BOOT_PIN_ENABLE_SIM_SCG_REG |= BOOT_PIN_ENABLE_SIM_SCG_MASK;
    BOOT_PIN_ENABLE_PORT_BASE->PCR[BOOT_PIN_ENABLE_NUM] = PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
  #else
    SYSCON->PIO_PULL_CFG[1] |= (0x10) << ((BOOT_PIN_ENABLE_NUM & 0xF) << 1); //enable pull-up for sw2
    IOCON_PullSet(BOOT_PIN_ENABLE_IOCON_BASE, 0,  BOOT_PIN_ENABLE_NUM, 0x10);
    SYSCON->CLK_EN          |= SYSCON_CLK_EN_CLK_GPIO_EN_MASK;
  #endif
    /* Debounce the switch pin */
    do 
    {
  #if !defined(CPU_QN9080C)
      temp = ((BOOT_PIN_ENABLE_GPIO_BASE->PDIR >> BOOT_PIN_ENABLE_NUM) & 0x1);
  #else
      temp = ((BOOT_PIN_ENABLE_GPIO_BASE->DATA >> BOOT_PIN_ENABLE_NUM) & 0x1);
  #endif
      mDebounceShiftReg |= temp;

      if((mDebounceShiftReg == 0xFFFFFFFF) || (mDebounceShiftReg == 0)) // pin stable values
      {
        break;
      }
      mDebounceShiftReg <<= 1;
    } while (mDebounceCnt++ <= gDebounceTimeout_c);
           
    if( (mDebounceShiftReg == 0) ||  /* GPIO trigger */
        (pBootInfo->u0.newBootImageAvailable == gFsciBootTrigger_c) ||        /* Flag set by Application */
        (*((uint32_t*)gUserFlashStart_d) < KINETIS_RAM_START) ||              /* Invalid Application */
        (*((uint32_t*)gUserFlashStart_d) > (KINETIS_RAM_END+1)) )             /* Invalid Application */
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
#ifndef CPU_QN9080C
    /* Enable PORT Clock */
    NMI_PIN_SIM_SCG_REG |= NMI_PIN_SIM_SCG_MASK;
    /* Change PIN mux */
    NMI_PIN_PORT_BASE->PCR[NMI_PIN_NUM] = PORT_PCR_MUX(NMI_PIN_ALT);
#endif
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
#if defined(CPU_MKW41Z512VHT4) || defined(CPU_MKW36Z512VHT4)
    RSIM->ANA_TRIM |= RSIM_ANA_TRIM_BB_LDO_XO_TRIM_MASK;
    /* Enable RF OSC in RSIM and wait for ready */
    RSIM->CONTROL &= ~RSIM_CONTROL_RF_OSC_EN_MASK;
    RSIM->CONTROL |= RSIM_CONTROL_RF_OSC_EN(1);
    /* wait for RF_OSC_READY */
    while((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) == 0) {}
#elif defined(CPU_QN9080C)
    /* Speed up the startup of XTAL by decrease load cap */
    SYSCON->XTAL_CTRL = (SYSCON->XTAL_CTRL &
                         ~(SYSCON_XTAL_CTRL_XTAL_SU_CA_REG_MASK | SYSCON_XTAL_CTRL_XTAL_SU_CB_REG_MASK |
                           SYSCON_XTAL_CTRL_XTAL_XCUR_BOOST_REG_MASK)) |
                        SYSCON_XTAL_CTRL_XTAL_SU_CA_REG(0U) | SYSCON_XTAL_CTRL_XTAL_SU_CB_REG(0U) |
                        SYSCON_XTAL_CTRL_XTAL_XCUR_BOOST_REG_MASK;

    /* Change crystal load cap to (0.35 * 0x08 + 5) = 7.8pF for DK board, and half of default voltage */
    SYSCON->ANA_CTRL0 = (SYSCON->ANA_CTRL0 & ~(SYSCON_ANA_CTRL0_XTAL_LOAD_CAP_MASK | SYSCON_ANA_CTRL0_XTAL_AMP_MASK)) |
                        SYSCON_ANA_CTRL0_XTAL_LOAD_CAP(8U) | SYSCON_ANA_CTRL0_XTAL_AMP(2U);

    /* Enable Data Path 16/8MHz clock(some of the flash operations need this, too)
     * enable BiV clock include RTC BiV register  */
    SYSCON->CLK_EN = SYSCON_CLK_EN_CLK_DP_EN_MASK | SYSCON_CLK_EN_CLK_BIV_EN_MASK;
#endif

    BOARD_BootClockRUN();
}

#if defined(__GNUC__)
void __attribute__((weak)) __assertion_failed(char* s)
{
    while(1){} /*This is a noreturn function so loop forever*/
}
#endif
//-----------------------------------------------------------------------------
