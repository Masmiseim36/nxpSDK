/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/*****************************************************************************
 *                               INCLUDED HEADERS                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the headers that this module needs to include.    *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"
#include "board.h"
#include "PWR_Configuration.h"
#include "PWRLib.h"
#include "TimersManager.h"
#include "fsl_os_abstraction.h"
#include "Keyboard.h"
#include "GPIO_Adapter.h"
#include "gpio_pins.h"
#include "MemManager.h"
#include "DCDC.h"
#include "fsl_lptmr.h"
#include "NVM_Interface.h"

#if (cPWR_UsePowerDownMode)
#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
#include "controller_interface.h"
#endif
#if (cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_9)
#include "setjmp.h"
#endif
#endif

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
#include "Flash_adapter.h"
#include "fsl_edma.h"
#include "fsl_dmamux.h"
#endif

/*****************************************************************************
 *                             PRIVATE MACROS                                *
 *---------------------------------------------------------------------------*
 * Add to this section all the access macros, registers mappings, bit access *
 * macros, masks, flags etc ...                                              *
 *---------------------------------------------------------------------------*
 *****************************************************************************/
#define mLptmrTimoutMaxMs_c             (65535000)

/* Define KW35/KW36 LPUART IRQ number */
#if FSL_FEATURE_LPUART_HAS_SHARED_IRQ0_IRQ1
#define mUartIrq_d (LPUART0_LPUART1_IRQn)
#else
#define mUartIrq_d (LPUART0_IRQn)
#endif

#define LPTMR_MAX_CNT_VALUE (( (uint32_t)1U << 16U)- (uint32_t)1U)
#define LPTMR_CLK_FREQ                  32768U

#define PRWLIB_RAM_BANK_16K             0U
#define PRWLIB_RAM_BANK_32K             1U
#define PRWLIB_RAM_BANK_ALL             2U

#ifndef cPWR_UsePartialStopMode
/* If DCDC is configured in bypass mode, use a partial stop mode by keeping the bus clock enabled
    This is a workaround for a modified board for DCDC Bypass which faces issues caused
    by normal stop mode */
#if (APP_DCDC_MODE == gDCDC_Bypass_c)
#define cPWR_UsePartialStopMode         1
#else
#define cPWR_UsePartialStopMode         0
#endif
#endif

typedef void (*ram_func_t)(void);
typedef union ram_func_ptr_tag
{
    uint32_t    instructionArrayAddress;
    ram_func_t  ram_func_ptr;
} ram_func_ptr_t;

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
extern uint32_t m_ram_img_start;
extern uint32_t m_ram_img_end;
extern uint32_t m_max_ram_img_size;

#define gPWR_RamContextFlashBaseAddr   (uint32_t)(&m_ram_img_start)

#define PWRLIB_IMG_MAGIC               0x12345678u
#define PWRLIB_IMG_END_MAGIC           0x87654321u
#define PWRLIB_STACK_ADDR_MARGIN       0xC0u
#define PWRLIb_MAX_RAM_SIZE            (uint32_t)(&m_max_ram_img_size)

/* set 1Khz frequency clock (/32) */
#ifndef PWRLib_LPTMR_FREQ_DIV
#define PWRLib_LPTMR_FREQ_DIV           4u
#endif

uint8_t gPWRLib_RamSavedStatus = PWRLIB_RAM_SAVE_COUNT_DOWN;

typedef struct
{
    uint32_t img_valid_magic;
    uint32_t img_size;
    uint32_t img_location_addr;
}  pwrlib_img_header_t;

typedef struct
{
    volatile uint32_t img_end_magic;
}  pwrlib_img_footer_t;
#endif


#if (!defined(GCOV_DO_COVERAGE) || (GCOV_DO_COVERAGE == 0))
#define PWRLIB_NOP    PWRLib_ExecuteNop()
#define PWRLIB_STATIC static
#define PWRLIB_PUBLIC
#else
#define PWRLIB_NOP    break
#define PWRLIB_STATIC __WEAK
#define PWRLIB_PUBLIC __WEAK
#endif


/*****************************************************************************
 *                               PRIVATE VARIABLES                           *
 *---------------------------------------------------------------------------*
 * Add to this section all the variables and constants that have local       *
 * (file) scope.                                                             *
 * Each of this declarations shall be preceded by the 'static' keyword.      *
 * These variables / constants cannot be accessed outside this module.       *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
#if (cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_9)
static jmp_buf                 pwr_CPUContext;
#endif

/*
 * When entring the stop modes, the flash prefetch might be interrupted, thus
 * the prefetched code or data might be broken. To make sure the flash is idle
 * when entring the stop modes, the code is moved to ram. And delay for a while
 * before WFI to make sure previous flash prefetch is finished.
 *
 * Only need to do like this when code is in flash, if code is in rom or ram,
 * this is not necessary.
 * The ram function code is:
 *
 *  uint32_t i;
 *  for (i=0; i<0x8; i++)
 *  {
 *      __NOP();
 *  }
 *  __DSB();
 *  __WFI();
 *  __ISB();
 */
static uint16_t s_stopRamFuncArray[] = {
    0x2000,         /* MOVS R0, #0 */
    0x2808,         /* CMP R0, #8 */
    0xD202,         /* BCS.N */
    0xBF00,         /* NOP */
    0x1C40,         /* ADDS R0, R0, #1 */
    0xE7FA,         /* B.N */
    0xF3BF, 0x8F4F, /* DSB */
    0xBF30,         /* WFI */
    0xF3BF, 0x8F6F, /* ISB */
    0x4770,         /* BX LR */
};

/* LPTMR variables */
static uint32_t mPWRLib_RTIElapsedTicks = 0U;

extern uint32_t __FIRMWARE_END_ADDRESS[];
extern uint32_t __CSTACK_end__[];
#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
extern uint32_t m_warmboot_stack_end;
#endif
#endif /* (cPWR_UsePowerDownMode == 1) */

/* For LVD function */
#if (cPWR_LVD_Enable == 2)
tmrTimerID_t PWRLib_LVD_PollIntervalTmrID;
PWRLib_LVD_VoltageLevel_t PWRLib_LVD_SavedLevel;
#endif /* (cPWR_LVD_Enable == 2) */

/*****************************************************************************
 *                               PUBLIC VARIABLES                            *
 *---------------------------------------------------------------------------*
 * Add to this section all the variables and constants that have global      *
 * (project) scope.                                                          *
 * These variables / constants can be accessed outside this module.          *
 * These variables / constants shall be preceded by the 'extern' keyword in  *
 * the interface header.                                                     *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

volatile PWRLib_WakeupReason_t PWRLib_MCU_WakeupReason;
uint32_t mPWR_DeepSleepTimeMs     = cPWR_DeepSleepDurationMs;

/* Application state stored */
PWR_AppState PWRLib_AppState            = PWR_APP_STATE_INVALID;
uint8_t      mPWRLib_DeepSleepMode      = cPWR_DeepSleepMode;
uint32_t     PWRLib_wkup_pin_setup      = 0u;

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
static volatile uint32_t             s_pu8Stack;
static pwrlib_img_header_t* const pwrlib_img_flash_header_p = (pwrlib_img_header_t*)(gPWR_RamContextFlashBaseAddr);
#endif

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
/*****************************************************************************
 *                           PRIVATE FUNCTIONS PROTOTYPES                    *
 *---------------------------------------------------------------------------*
 * Add to this section all the functions prototypes that have local (file)   *
 * scope.                                                                    *
 * These functions cannot be accessed outside this module.                   *
 * These declarations shall be preceded by the 'static' keyword.             *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

/*---------------------------------------------------------------------------
* Name: PWRLib_GetCstaskAddr
* Description: returns C-stack end address
*---------------------------------------------------------------------------*/
PWRLIB_STATIC uint32_t PWRLib_GetCstaskAddr(void)
{
	/* returns C-stack end address */
	return (uint32_t)&__CSTACK_end__;
}

/*---------------------------------------------------------------------------
* Name: PWRLib_GetFwEndAddr
* Description: returns Firmware end address
*---------------------------------------------------------------------------*/
PWRLIB_STATIC uint32_t PWRLib_GetFwEndAddr(void)
{
	/* returns Firmware end address */
	return (uint32_t)&__FIRMWARE_END_ADDRESS;
}

/*****************************************************************************
 *                                PRIVATE FUNCTIONS                          *
 *---------------------------------------------------------------------------*
 * Add to this section all the functions that have local (file) scope.       *
 * These functions cannot be accessed outside this module.                   *
 * These definitions shall be preceded by the 'static' keyword.              *
 *---------------------------------------------------------------------------*
*****************************************************************************/

/*---------------------------------------------------------------------------
* Name: PWRLib_ExecuteNop
* Description: Executes NOP instruction, used mainly for Misra
*---------------------------------------------------------------------------*/
#if (cPWR_UsePowerDownMode) && (cPWR_BLE_LL_Enable)
PWRLIB_STATIC void PWRLib_ExecuteNop(void)
{
    /* Putting ASM instruction in a static function to isolate the code
       and fix related MISRA issue */
    asm volatile("NOP");
}
#endif

static void PWRLib_LLWU_UpdateWakeupReasonExt(uint32_t llwuFlags)
{
    uint32_t llwuFlags_wkup_pins;

    /* Filter on wakeup pins */
    llwuFlags_wkup_pins = (llwuFlags & 0xFFFFU);

#if (defined(gSerialManagerMaxInterfaces_c)&&(gSerialManagerMaxInterfaces_c > 0))
    if((llwuFlags_wkup_pins & pwr_llwu_serial_bitmap) != 0U)
    {
        //PWR_DBG_LOG("%x", llwuFlags_wkup_pins);

        PWRLib_MCU_WakeupReason.Bits.FromSerial = 1;

        /* remove these pins from the llwuFlags so we can then detect the keyboard pins */
        llwuFlags_wkup_pins &= ~pwr_llwu_serial_bitmap;
    }
#endif

    if ( llwuFlags_wkup_pins != 0U )
    {
        /* if wakeup pin is not from serial, assume this is from Keyboard */
        PWRLib_MCU_WakeupReason.Bits.FromKeyBoard = 1;
    }

    if((llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_LPTMR_c) != 0U)
    {
        PWRLib_MCU_WakeupReason.Bits.FromLPTMR = 1U;
        PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1U;
    }

    if((llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_Radio_c) != 0U)
    {
        PWRLib_MCU_WakeupReason.Bits.FromBLE_LLTimer = 1U;

        if( RSIM_CONTROL_READ_FIELD(RSIM_DSM_CONTROL_GEN_SYSCLK_REQ_INT) != 0U)
        {
            PWRLib_MCU_WakeupReason.Bits.FromRadio = 1U;
        }

        PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1U;
    }

    if((llwuFlags & gPWRLib_LLWU_WakeupModuleFlag_DCDC_c) != 0U)
    {
        PWRLib_MCU_WakeupReason.Bits.FromPSwitch = 1;

        /* Clear pSwitch interrupt */
        DCDC->REG6 |= DCDC_REG6_PSWITCH_INT_CLEAR_MASK;
        DCDC->REG6 &= ~DCDC_REG6_PSWITCH_INT_CLEAR_MASK;
    }
}

static void PWRLib_DisableIRQs(void)
{
    int irq = (int)0;
    while(irq <= (int)PORTB_PORTC_IRQn)
    {
        NVIC_DisableIRQ( (IRQn_Type)irq );
        irq++;
    }
}

PWRLIB_STATIC void PWRLib_ExecuteWfiFromRam(void)
{
    ram_func_ptr_t ram_func_ptr;

    /* The function is represented by an array of thumb instructions (16 bits)
       To indicate it's a thumb code, the less significant bit must be set to 1 */
    ram_func_ptr.instructionArrayAddress = ((uint32_t)(s_stopRamFuncArray)) + 1U;
    ram_func_ptr.ram_func_ptr();
}

static uint32_t PWRLib_GetFwUpperLimit(void)
{
    uint32_t heap_upper_limit;
    uint32_t firmware_upper_limit;

    heap_upper_limit = MEM_GetHeapUpperLimit();

    if( heap_upper_limit > (PWRLib_GetCstaskAddr() )) /* Optimized linker script and memory manager, stack is placed below the heap */

    {
        /* Light MemManager and _lp linker script used         */
        firmware_upper_limit = heap_upper_limit;
    }
    else
    {
        /* legacy MemManager used or linker script not optimized for lowpower
            => Use the __FIRMWARE_END_ADDRESS symbol from the linker to assess the RAM end of the firmware */
        firmware_upper_limit = PWRLib_GetFwEndAddr();
    }
    return firmware_upper_limit;
}

static void PWRLib_LLWU_Init(void)
{
#if defined(BOARD_LLWU_PIN_ENABLE_BITMAP) && (BOARD_LLWU_PIN_ENABLE_BITMAP)
    uint16_t pinEn16 = BOARD_LLWU_PIN_ENABLE_BITMAP;
    uint32_t PinEn32 = 0UL;
    uint32_t i;

    /* Can not use dynamic and static wakeup pin configurations, please choose */
    assert( PWRLib_wkup_pin_setup == 0u );

    for(i=0U; pinEn16 != 0U ; i++)
    {
        if((pinEn16 & 0x1) != 0U)
        {
            PinEn32 |= 0x3UL<<(i<<1);
        }
        pinEn16 >>= 1;
    }
    LLWU->PE1 = PinEn32&0xffUL;
    LLWU->PE2 = (PinEn32>>8)&0xffUL;
    LLWU->PE3 = (PinEn32>>16)&0xffUL;
    LLWU->PE4 = (PinEn32>>24)&0xffUL;
#else

    PWRLib_SetWakeupPins(PWRLib_wkup_pin_setup);

#endif /* BOARD_LLWU_PIN_ENABLE_BITMAP */

    /* LLWU_FILT1: FILTF=1,FILTE=0,??=0,FILTSEL=0 */
    LLWU->FILT1 = LLWU_FILT1_FILTF_MASK |
                  LLWU_FILT1_FILTE(0x00) |
                  LLWU_FILT1_FILTSEL(0x00);

    /* LLWU_FILT2: FILTF=1,FILTE=0,??=0,FILTSEL=0 */
    LLWU->FILT2 = LLWU_FILT2_FILTF_MASK |
                  LLWU_FILT2_FILTE(0x00) |
                  LLWU_FILT2_FILTSEL(0x00);

    /* SMC_PMPROT: ??=0,??=0,AVLP=1,??=0,ALLS=1,??=0,AVLLS=1,??=0 */
    SMC->PMPROT = SMC_PMPROT_AVLP_MASK |
                  SMC_PMPROT_ALLS_MASK |
                  SMC_PMPROT_AVLLS_MASK;  /* Setup Power mode protection register */
}

/*---------------------------------------------------------------------------
* Name: PWRLib_RamBankRetained
* Description: Provide RAM banks to be retained : 16K, 32K or all banks
*---------------------------------------------------------------------------*/
static uint8_t PWRLib_RamBankRetained(void)
{
    uint8_t  ram_retained;
    uint32_t firmware_upper_limit;

    firmware_upper_limit = PWRLib_GetFwUpperLimit();

    if( firmware_upper_limit < 0x20004000U)
    {
        ram_retained = PRWLIB_RAM_BANK_16K;
    }
    else if( firmware_upper_limit < 0x20008000U)
    {
        ram_retained = PRWLIB_RAM_BANK_32K;
    }
    else
    {
        ram_retained = PRWLIB_RAM_BANK_ALL;
    }

    //PWR_DBG_LOG("heap_upper_limit=%x CSTACK_end=%x", heap_upper_limit, (uint32_t)&__CSTACK_end__);
    //PWR_DBG_LOG("fw_upper_limit=%x %d", firmware_upper_limit, ram_retained);

    return ram_retained;
}

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
static bool PWR_IsRamImageValid(void)
{
    bool img_valid = FALSE;

    /* Check Magic value in image flash header - TODO : improve robustness by adding a checksum */
    if (  pwrlib_img_flash_header_p->img_valid_magic == PWRLIB_IMG_MAGIC )
    {
        img_valid = TRUE;
    }

    return img_valid;
}

static void PWRLib_SetMspWarmStack(void)
{
    /* Set stack pointer to warmboot stack */
    asm volatile(" LDR     R0, =m_warmboot_stack_end ");
    asm volatile(" MSR     MSP, R0                  ");
}

static void PWRLib_PrepareFlashForRamImage(void)
{
    NV_Init();

    BOARD_DBGINITSET(0,0);
    BOARD_DBGINITSET(1,0);

    /* check if RAM image section size in flash is not null */
    assert( PWRLIb_MAX_RAM_SIZE > 0u );

    /* Erase Flash to prepare for RAM image saving later on */
    uint32_t  status = NV_FlashEraseSector(gPWR_RamContextFlashBaseAddr, PWRLIb_MAX_RAM_SIZE );
    assert( status == (uint32_t)kStatus_FLASH_Success );
    NOT_USED(status);

    BOARD_DBGINITSET(0,1);
    BOARD_DBGINITSET(1,1);
}

static void PWRLib_GetStackAddr(void)
{
    /* use R0 to get the current stack pointer and store the content in s_pu8Stack pointed by R1 */
    asm volatile("MRS R0, MSP");
    asm volatile("LDR R1,=s_pu8Stack");
    asm volatile("STR R0,[R1]");
}

static void PWRLib_SaveRamImage(void)
{
    pwrlib_img_header_t* img_header_p;
    pwrlib_img_footer_t* img_foot_p;
    uint32_t             img_location_addr;
    uint32_t             firmware_upper_limit;
    uint32_t             ram_size;

    /* update s_pu8Stack */
    PWRLib_GetStackAddr();

    /* we need to make sure the img_header will not be corrupted by the NV_FlashProgramUnaligned function */
    img_location_addr                   = (uint32_t)s_pu8Stack - PWRLIB_STACK_ADDR_MARGIN;
    firmware_upper_limit                = PWRLib_GetFwUpperLimit();
    ram_size                            = firmware_upper_limit - img_location_addr;
    assert( ram_size < PWRLIb_MAX_RAM_SIZE );

    /* build the RAM image header */
    img_header_p                        = (pwrlib_img_header_t*)img_location_addr - 1;
    img_header_p->img_valid_magic       = PWRLIB_IMG_MAGIC;
    img_header_p->img_size              = ram_size;
    img_header_p->img_location_addr     = img_location_addr;

    /* build the RAM image footer */
    img_foot_p                          = (pwrlib_img_footer_t*)(firmware_upper_limit);
    img_foot_p->img_end_magic           = PWRLIB_IMG_END_MAGIC;

    uint32_t status = NV_FlashProgramUnaligned( gPWR_RamContextFlashBaseAddr, ram_size + sizeof(pwrlib_img_header_t) + sizeof(pwrlib_img_footer_t) , (uint8_t*)img_header_p);

    assert( status == (uint32_t)kStatus_FLASH_Success );
    NOT_USED( status );
}

/* User callback function for EDMA transfer - not used */
static void EDMA_Callback(edma_handle_t *handle, void *param, bool transferDone, uint32_t tcds)
{
}

static void PWRLib_StartTransferDma(uint32_t ram_size, uint32_t img_start_addr)
{
    edma_transfer_config_t      transferConfig;
    edma_config_t               userConfig;
    edma_handle_t               g_EDMA_Handle;
    status_t                    status;

    /* Configure DMAMUX */
    DMAMUX_Init(DMAMUX0);
#if defined(FSL_FEATURE_DMAMUX_HAS_A_ON) && FSL_FEATURE_DMAMUX_HAS_A_ON
    DMAMUX_EnableAlwaysOn(DMAMUX0, 0, true);
#else
    DMAMUX_SetSource(DMAMUX0, 0, 63);
#endif /* FSL_FEATURE_DMAMUX_HAS_A_ON */
    DMAMUX_EnableChannel(DMAMUX0, 0);
    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DMA0, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DMA0, 0);
    EDMA_SetCallback(&g_EDMA_Handle, EDMA_Callback, NULL);
    EDMA_PrepareTransfer(&transferConfig, (void*)(uint32_t*)(gPWR_RamContextFlashBaseAddr + sizeof(pwrlib_img_header_t)),
                         4, (void*)(uint32_t*)img_start_addr, 4, 4, ram_size + 4u /* ? */, kEDMA_MemoryToMemory);

    status = EDMA_SubmitTransfer(&g_EDMA_Handle, &transferConfig);
    assert(status == kStatus_Success);
    NOT_USED(status);

    EDMA_StartTransfer(&g_EDMA_Handle);
}


static void PWR_RestoreRamImageDma(void)
{
    uint32_t                            img_ram_size;
    uint32_t                            img_start_addr;
    volatile pwrlib_img_footer_t*       img_footer_p;

    img_ram_size                = pwrlib_img_flash_header_p->img_size + sizeof(pwrlib_img_footer_t);
    img_start_addr              = pwrlib_img_flash_header_p->img_location_addr;
    img_footer_p                = (pwrlib_img_footer_t*)(img_start_addr + img_ram_size - sizeof(pwrlib_img_footer_t));
    img_footer_p->img_end_magic = 0u;

    PWRLib_StartTransferDma(img_ram_size, img_start_addr);

    /* Wait for EDMA transfer finish */
    __WFI();

    /* Make sure the last word have been written */
    while ( img_footer_p->img_end_magic != PWRLIB_IMG_END_MAGIC ) {};

    /* Make sure the magic does not remain on next reboot */
    //img_footer_p->img_end_magic = 0u;

    /* Make sure the DMA does not fire when unmasking the interrupts */
    NVIC_ClearPendingIRQ(DMA0_IRQn);
    NVIC_DisableIRQ(DMA0_IRQn);
}

static void PRWLib_RestoreRamImage(void)
{
    bool        is_ram_ctxt_valid;
    is_ram_ctxt_valid = PWR_IsRamImageValid();

    if ( is_ram_ctxt_valid )
    {
        /* change the stack pointer to warmboot stack
           warning, all local variables and stack content will be obsoleted from this point */
        PWRLib_SetMspWarmStack();

        BOARD_DBGLPIOSET(3,0);

#if 0
        static volatile int once = 1;
        while (once) {};
#endif

        PWR_RestoreRamImageDma();

        BOARD_DBGLPIOSET(3,1);

        warmmain();
        /* do not go there */
    }
}

static void PWRLib_RestoreAlwaysOnDomain(void)
{
    /* reconfigure the LLWU unit */
    PWRLib_LLWU_Init();

    /* Not required */
    //SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK ;
    //BOARD_RTC_Init();
    //CLOCK_SetTpmClock(2);

    /* reconfigure the lptmr timer if needed */
    LPTMR0->CSR |= (LPTMR_CSR_TPS(0x00) | LPTMR_CSR_TIE_MASK );
}
#endif
#endif /* #if (cPWR_UsePowerDownMode == 1) */


/*****************************************************************************
 *                             PUBLIC FUNCTIONS                              *
 *---------------------------------------------------------------------------*
 * Add to this section all the functions that have global (project) scope.   *
 * These functions can be accessed outside this module.                      *
 * These functions shall have their declarations (prototypes) within the     *
 * interface header file and shall be preceded by the 'extern' keyword.      *
 *---------------------------------------------------------------------------*
 *****************************************************************************/

#if defined(cPWR_BLE_LL_Enable) && (cPWR_BLE_LL_Enable == 1)
/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetInstantTimer
 *---------------------------------------------------------------------------*/
uint16_t PWRLib_BLL_GetInstantTimer(void)
{
    return BLE_LL_REG(TIM_COUNTER_L);
}
#endif

#if defined(cPWR_UsePowerDownMode) && (cPWR_UsePowerDownMode == 1)
/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_ClearWakeUpSources
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_ClearWakeUpSources(void)
{
    LLWU->F1     &= (uint8_t)0xFFU;
    LLWU->F2     &= (uint8_t)0xFFU;
    LLWU->FILT1  |= LLWU_FILT1_FILTF_MASK;
    LLWU->FILT2  |= LLWU_FILT2_FILTF_MASK;
}

#if (cPWR_BLE_LL_Enable)
/*---------------------------------------------------------------------------
 * Name: PWRLib_ClearRSIMInt
 *---------------------------------------------------------------------------*/
void PWRLib_ClearRSIMInt(void)
{
    if(RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_BLE_RF_OSC_REQ_INT) != 0U)
    {
        RSIM->CONTROL |= RSIM_CONTROL_BLE_RF_OSC_REQ_INT_MASK;
    }
    if( ((BLE_LL_REG_EVENT_STATUS & BLE_LL_EVENT_STATUS_DSM_INT_MASK) != 0U) && ((RSIM_CONTROL_READ_FIELD(RSIM_CONTROL_RSIM_DSM_EXIT)) != 0U) )
    {
        RSIM->CONTROL &= ~RSIM_CONTROL_RSIM_DSM_EXIT_MASK;
        BLE_LL_REG(EVENT_CLEAR) = BLE_LL_EVENT_CLEAR_DSM_INT_MASK;
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_EnterDSM
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_EnterDSM(void)
{
    while((BLE_LL_REG_CLOCK_CONFIG & BLE_LL_CLOCK_CONFIG_LLH_IDLE_MASK ) == 0U)
    {
        PWRLIB_NOP;
    }
    BLE_LL_REG(EVENT_CLEAR) = BLE_LL_EVENT_CLEAR_DSM_INT_MASK;
#if (cPWR_EnableDeepSleepMode_8 == 1) || (cPWR_EnableDeepSleepMode_5 == 1) || (cPWR_EnableDeepSleepMode_9 == 1)
    if( (mPWRLib_DeepSleepMode == PWR_LOWPOWER_VLLS2_3_LL_DSM3_MODE)
          || (mPWRLib_DeepSleepMode == PWR_LOWPOWER_VLLS2_3_NO_DSM_MODE)
          || (mPWRLib_DeepSleepMode == PWR_LOWPOWER_VLLS2_3_LL_RESET_ADV_MODE) )
    {
        Controller_SaveLlhState();
        BLE_LL_REG(COMMAND_REGISTER) = ENTER_DSM3;
    }
    else
#endif
    {
        BLE_LL_REG(COMMAND_REGISTER) = ENTER_DSM;
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_SetWakeupInstant
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_SetWakeupInstant(uint16_t wakeupInstant)
{
    BLE_LL_REG(WAKEUP_CONTROL) = wakeupInstant;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_WaitForDSMExit
 *---------------------------------------------------------------------------*/
PWRLIB_PUBLIC void PWRLib_BLL_WaitForDSMExit(void)
{
    while((BLE_LL_REG_EVENT_STATUS & BLE_LL_EVENT_STATUS_DSM_INT_MASK) == 0U)
    {
        BOARD_DBGLPIOSET(4,0);
        PWRLIB_NOP;
        BOARD_DBGLPIOSET(4,1);
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_GetIntEn
 *---------------------------------------------------------------------------*/
uint16_t PWRLib_BLL_GetIntEn(void)
{
    return BLE_LL_REG(EVENT_ENABLE);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_DisableInterrupts
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_DisableInterrupts(uint16_t bmInterrupts)
{
    BLE_LL_REG(EVENT_ENABLE) &= ~ bmInterrupts;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_EnableInterrupts
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_EnableInterrupts(uint16_t bmInterrupts)
{
    BLE_LL_REG(EVENT_ENABLE) |= bmInterrupts;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_BLL_ClearInterrupts
 *---------------------------------------------------------------------------*/
void PWRLib_BLL_ClearInterrupts(uint16_t bmInterrupts)
{
    BLE_LL_REG(EVENT_CLEAR) |= bmInterrupts;
}
#endif /* (cPWR_BLE_LL_Enable) */

/*---------------------------------------------------------------------------
 * Name: PWRLib_ClearRSIMDsmInt
 *---------------------------------------------------------------------------*/
bool_t PWRLib_ClearRSIMDsmInt(void)
{
#if cPWR_GENFSK_LL_Enable
    if (RSIM->DSM_CONTROL & RSIM_DSM_CONTROL_GEN_SYSCLK_REQ_INT_MASK)
    {
        RSIM->DSM_CONTROL = RSIM->DSM_CONTROL;

        return TRUE;
    }
#endif /* cPWR_GENFSK_LL_Enable */

    return FALSE;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_SetDeepSleepMode
 *---------------------------------------------------------------------------*/
void PWRLib_SetDeepSleepMode(uint8_t lpMode)
{
    mPWRLib_DeepSleepMode = lpMode;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_GetDeepSleepMode
 *---------------------------------------------------------------------------*/
uint8_t PWRLib_GetDeepSleepMode(void)
{
    return mPWRLib_DeepSleepMode;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCU_Enter_Mode5_8_9
 * Description: Puts the processor into VLLS2 or VLLS3 (Very Low Leakage Stop 2-3).
 *---------------------------------------------------------------------------*/
#if (cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_9)
void PWRLib_MCU_Enter_vlls2_3(void)
{
    uint32_t i;
    uint32_t scb_vtor;
#if defined(USE_RTOS) && (USE_RTOS)
    uint32_t scb_icsr;
    uint32_t scb_aircr;
    uint32_t scb_shp[2];
#endif
    uint32_t sim_scgc5;
    uint32_t sim_sopt2;
    uint32_t nvic_iser;
    uint32_t nvic_ip[8];

    /* Store SCB registers */
    scb_vtor  = SCB->VTOR;
#if defined(USE_RTOS) && (USE_RTOS)
    scb_icsr  = SCB->ICSR;
    scb_aircr = SCB->AIRCR;
    scb_shp[0] = SCB->SHP[0];
    scb_shp[1] = SCB->SHP[1];
#endif
    sim_scgc5 = SIM->SCGC5;
    sim_sopt2 = SIM->SOPT2;
    nvic_iser = NVIC->ISER[0U];

    for(i=0;i<8U;i++)
    {
        nvic_ip[i]=NVIC->IP[i];
    }

    if ( 0 == setjmp(pwr_CPUContext))
    {
        uint8_t mode;

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
        BOARD_DBGLPIOSET(5 ,0 );

        if ( PWR_IsRamImageValid() == FALSE )
        {
            if ( gPWRLib_RamSavedStatus == PWRLIB_RAM_TO_BE_SAVED)
            {
                gPWRLib_RamSavedStatus = PWRLIB_RAM_SAVED;
                PWRLib_SaveRamImage();
            }
            else if (gPWRLib_RamSavedStatus > PWRLIB_RAM_TO_BE_SAVED)
            {
                /* If RAM not yet saved, decrease the count down counter */
                gPWRLib_RamSavedStatus--;
            }
            else
            {
                /* nothing to do otherwise */
            }
        }
        BOARD_DBGLPIOSET(5 ,1 );

        if (( PWRLib_AppState == PWR_APP_STATE_ADV ) && ( gPWRLib_RamSavedStatus == PWRLIB_RAM_SAVED ))
        {
#if defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
            /* Switch OFF the DCDC immediatly */
            DCDC_ShutDown();
#else
#if (APP_DCDC_MODE == gDCDC_Bypass_c)
            mode = PWRLIB_VLLS_0;
#else
            mode = PWRLIB_VLLS_1;
#endif
#endif  // defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
        }
        else
#endif  // defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
        {
            mode = PWRLIB_VLLS_2;
        }

        PWRLib_MCU_Enter_VLLSx(mode);

        PWR_DBG_LOG("Incorrect entering in Lowpower: %d", mode);
        DBG_LOG_DUMP(TRUE);
        assert(0);
    }

    BOARD_DBGLPIOSET(0,0);

    /* Restore SCB registers */
    SCB->VTOR   = scb_vtor;
#if defined(USE_RTOS) && (USE_RTOS)
    SCB->ICSR   = scb_icsr;
    SCB->AIRCR  = scb_aircr;
    SCB->SHP[0] = scb_shp[0];
    SCB->SHP[1] = scb_shp[1];
#endif

    /* Restore peripheral clocks */
    SIM->SCGC5 = sim_scgc5;
    /* Restore peripheral clock source option */
    SIM->SOPT2 = sim_sopt2;

    for(i=0;i<8U;i++)
    {
        NVIC->IP[i]=nvic_ip[i];
    }
    NVIC->ICPR[0U] = nvic_iser;
    NVIC->ISER[0U] = nvic_iser;

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
    PWRLib_RestoreAlwaysOnDomain();
#endif

    BOARD_DBGLPIOSET(0,1);
}


/*---------------------------------------------------------------------------
* Name: warmmain
* Description: - Overide the weak warmmain function in Base SDK
*              - Entry point for warm boot (no data/bss initialization)
*---------------------------------------------------------------------------*/
void warmmain(void)
{
    /* Entering warmmain with interrupts disabled */
    __disable_irq();

#if defined(USE_RTOS) && (USE_RTOS)
    /* Switch to the psp stack */
    asm volatile("movs r0, #2");
    asm volatile("msr CONTROL, r0");
#endif

    /* Restore previous context */
    longjmp(pwr_CPUContext, 1);
}
#endif   /* #if cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5 || cPWR_EnableDeepSleepMode_9 */


void SystemInitHook (void)
{
    /* restart 32MHz oscillator as soon as possible in case it has not been restarted yet by HW */
    RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 1);

    if((PMC_REGSC & PMC_REGSC_ACKISO_MASK) != 0x00U)
    {
        PMC->REGSC |= PMC_REGSC_ACKISO_MASK; /* Release hold with ACKISO:  Only has an effect if recovering from VLLSx.*/
    }

    BOARD_DBGINITDBGIO();
    BOARD_DBGTOGGLEDBGIO();
    BOARD_DBGINITSET(0,0);

    /* In case of POR or VLLS0/1 exit */
    /* Increase MCU clock frequency ( 48MHz instead of default 20~25 MHz) */
    BOARD_SetCoreClock48Mhz();

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
    PRWLib_RestoreRamImage();
#endif
    BOARD_DBGINITSET(0,1);
}

void SystemInitHookWarm (void)
{
    /* restart 32MHz oscillator as soon as possible in case it has not been restarted yet by HW */
    RSIM->CONTROL = RSIM_CONTROL_MODIFY_FIELD(RSIM_CONTROL_RF_OSC_EN, 1);

    if((PMC_REGSC & PMC_REGSC_ACKISO_MASK) != 0x00U)
    {
        PMC->REGSC |= PMC_REGSC_ACKISO_MASK; /* Release hold with ACKISO:  Only has an effect if recovering from VLLSx.*/
    }

    BOARD_DBGINITDBGIO();
    BOARD_DBGTOGGLEDBGIO();
    BOARD_DBGINITSET(0,0);
    BOARD_DBGINITSET(0,1);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_VLLSx
 * Description: Puts the processor into VLLS0 - VLLS3 (Very Low Leakage Stop 0-3).
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_VLLSx(uint8_t mode)
{
    uint8_t stopctrl      = 0U;

    BOARD_DBGLPIOSET(2,1);

    PWRLib_BLPEtoBLPI();

    if ( mode == PWRLIB_VLLS_2 )
    {
        uint8_t ram_retained;
		ram_retained = PWRLib_RamBankRetained();

        switch (ram_retained)  {
        case PRWLIB_RAM_BANK_16K:
            stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(PWRLIB_VLLS_2);   /* do not set SMC_STOPCTRL_RAM2PO_MASK */
            break;

        case PRWLIB_RAM_BANK_32K:
            stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(PWRLIB_VLLS_2) | SMC_STOPCTRL_RAM2PO_MASK;
            break;

        case PRWLIB_RAM_BANK_ALL:
        default:
            /* VLLS2 can not be achieved, -> switch to VLLS3 */
            stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(PWRLIB_VLLS_3);
            break;
        }
    }
    else
    {
#if defined(cPWR_POR_DisabledInVLLS0)
        /* VLLS0 with POR disabled */
        stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(mode) | SMC_STOPCTRL_PORPO_MASK;
#else
        /* VLLS0 with POR enabled or VLLS1 */
        stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(mode);
#endif
    }

    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR      = (SCB->SCR &  ~(uint32_t)SCB_SCR_SLEEPONEXIT_Msk) | SCB_SCR_SLEEPDEEP_Msk;

    SMC->PMCTRL   = (uint8_t)((SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x03))) | (uint8_t)(SMC_PMCTRL_STOPM(0x04)));

    SMC->STOPCTRL = stopctrl;

    /* To be sure to stay in WFI while we cut power, we make sure all IRQs are disabled.
     * The core won't go out of WFI if an IRQ is disabled, even if it is pending.
     * Doing so is safe as we are waiting for power to be cut, so we would drop all pending IRQs anyway. */
    PWRLib_DisableIRQs();

    /* Clear any systick interrupts that could happen before the systick disabled */
    SCB->ICSR = SCB_ICSR_PENDSTCLR_Msk;

    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGSETDBGIOFORLOWPOWER();

    /* Execute WFI instruction from RAM */
    PWRLib_ExecuteWfiFromRam();
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_LLSx
 * Description: Puts the processor into LLSx (Low Leakage Stop1).
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_LLSx(void)
{
    uint8_t stopctrl      = 0U;
    uint8_t ram_retained;

    BOARD_DBGLPIOSET(2,1);

    PWRLib_BLPEtoBLPI();

    ram_retained = PWRLib_RamBankRetained();

    switch (ram_retained)  {
    case PRWLIB_RAM_BANK_16K:
        stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(PWRLIB_LLS_2);   /* do not set SMC_STOPCTRL_RAM2PO_MASK */
        break;

    case PRWLIB_RAM_BANK_32K:
        stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(PWRLIB_LLS_2) | SMC_STOPCTRL_RAM2PO_MASK;
        break;

    case PRWLIB_RAM_BANK_ALL:
    default:
        /* LLS2 can not be achieved, -> switch to LLS3 */
        stopctrl = SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(PWRLIB_LLS_3);
        break;
    }

    PWRLib_LLWU_ClearWakeUpSources();

    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR      = (SCB->SCR &  ~(uint32_t)SCB_SCR_SLEEPONEXIT_Msk) | SCB_SCR_SLEEPDEEP_Msk;

    SMC->PMCTRL   = (uint8_t)((SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x04))) | (uint8_t)(SMC_PMCTRL_STOPM(0x03)));

    SMC->STOPCTRL = stopctrl;

    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGSETDBGIOFORLOWPOWER();

    /* Execute WFI instruction from RAM */
    PWRLib_ExecuteWfiFromRam();

    BOARD_DBGINITDBGIO();
    BOARD_DBGLPIOSET(2,0);
    BOARD_DBGLPIOSET(2,1);
    BOARD_DBGLPIOSET(2,0);
}


/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_Stop
 * Description: Puts the processor into Stop
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_Stop(void)
{
    BOARD_DBGLPIOSET(3,0);

    /* Set the SLEEPDEEP bit to enable CORTEX M0 deep sleep mode */
    SCB->SCR = (SCB->SCR &  ~(uint32_t)SCB_SCR_SLEEPONEXIT_Msk) | SCB_SCR_SLEEPDEEP_Msk;

    SMC->PMCTRL   = (uint8_t)(SMC->PMCTRL & (uint8_t)~(uint8_t)(SMC_PMCTRL_STOPM(0x07)));
#if defined(cPWR_UsePartialStopMode) && (cPWR_UsePartialStopMode == 1)
    /* Reduce Core Clock to its minimum so the consumption can be reduced
       We observed a descrease of about 0.6-0.7mA in power consumption */
    BOARD_ResetCoreClock();
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x02) | SMC_STOPCTRL_LLSM(0x00));
#else
    SMC->STOPCTRL = (SMC_STOPCTRL_PSTOPO(0x00) | SMC_STOPCTRL_LLSM(0x00));
#endif

    /* Execute WFI instruction from RAM */
    PWRLib_ExecuteWfiFromRam();

#if defined(cPWR_UsePartialStopMode) && (cPWR_UsePartialStopMode == 1)
    /* set the core clock back to 48MHz */
    BOARD_SetCoreClock48Mhz();
#endif

    BOARD_DBGLPIOSET(3,1);
}


/*---------------------------------------------------------------------------
 * Name: PWRLib_MCUEnter_Sleep
 *---------------------------------------------------------------------------*/
void PWRLib_MCU_Enter_Sleep(void)
{
    BOARD_DBGLPIOSET(1,0);

    /* SCB_SCR: SLEEPDEEP=0 & SLEEPONEXIT=0 */
    SCB->SCR &= (uint32_t)~(uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);

    /* Execute WFI instruction from RAM */
    PWRLib_ExecuteWfiFromRam();

    BOARD_DBGLPIOSET(1,1);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_GetWakeUpFlags
 *---------------------------------------------------------------------------*/
PWRLIB_PUBLIC uint32_t PWRLib_LLWU_GetWakeUpFlags(void)
{
    uint32_t Flags;

    Flags = LLWU->F1;
    Flags |= (uint32_t)((uint32_t)LLWU->F2 << 8U);
    Flags |= (uint32_t)((uint32_t)LLWU->F3 << 16U);

    if ((LLWU_REG_FILT1 & 0x80U) != 0x00U )
    {
        Flags |= LLWU_FILTER1;
    }

    if ((LLWU_REG_FILT2 & 0x80U) != 0x00U )
    {
        Flags |= LLWU_FILTER2;
    }

    return Flags;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_UpdateWakeupReason
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_UpdateWakeupReason(void)
{
    uint32_t  llwuFlags;

    llwuFlags =  PWRLib_LLWU_GetWakeUpFlags();

    /* Update PWRLib_MCU_WakeupReason - this variable will be used later :
      - by the application in case of wakeup up from RAM off
      - On exit from */
    PWRLib_LLWU_UpdateWakeupReasonExt(llwuFlags);

    /* Clear LLWU wakeup source - now use PWRLib_MCU_WakeupReason instead */
    PWRLib_LLWU_ClearWakeUpSources();
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_DisableAllWakeupSources
 *---------------------------------------------------------------------------*/
void PWRLib_LLWU_DisableAllWakeupSources(void)
{
    LLWU->PE1 = 0U;
    LLWU->PE2 = 0U;
    LLWU->PE3 = 0U;
    LLWU->PE4 = 0U;
    LLWU->ME = 0U;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_GetTimeSettings
 * Parameters: timeInMs can't go over (2^32)/LPTMR_CLK_FREQ to avoid overflow
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_GetTimeSettings(uint32_t timeInMs, uint8_t* pClkMode,uint32_t* pTicks, uint32_t* pFreq)
{
    uint64_t ticks64;
    uint32_t ticks;
    uint8_t divide_order = 1U;
    uint8_t prescaler = 0U;
    uint32_t freq = LPTMR_CLK_FREQ;

    if(timeInMs > (uint32_t)mLptmrTimoutMaxMs_c)
    {
        timeInMs = mLptmrTimoutMaxMs_c;
    }
    ticks64  = (uint64_t)timeInMs;
    ticks64 *= (uint64_t)freq;
    ticks64 /= 1000U;
    ticks = (uint32_t)ticks64;

#if defined(PWRLib_LPTMR_FREQ_DIV)
    divide_order +=PWRLib_LPTMR_FREQ_DIV;
    ticks >>= PWRLib_LPTMR_FREQ_DIV;
#else
    while(ticks > (LPTMR_MAX_CNT_VALUE >> 1))
    {
        divide_order++;
        ticks >>= 1U;
    }
#endif
    /* we divide at least 1 time */
    ticks     >>= 1U;
    freq      >>= divide_order;
    prescaler   = divide_order - 1U; /* prescaler to 0 already divides clock by 2 */

    //PWR_DBG_LOG("ord=%d freq=%d", divide_order, freq);

    *pClkMode =  (uint8_t)(prescaler<<LPTMR_PSR_PRESCALE_SHIFT) | (uint8_t)cLPTMR_Source_Ext_ERCLK32K;
    *pTicks   = ticks;
    *pFreq    = freq;
}

#if defined(PWRLib_LPTMR_KEEP_RUNNING)
/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStart
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStart(uint8_t ClkMode, uint32_t Ticks)
{
    /* Set compare value */
    if(Ticks > 0U)
    {
        Ticks--;
    }

    mPWRLib_RTIElapsedTicks = LPTMR0->CNR;

    /* set compare register considering the counter value - register will wrap if it exceeds 16bits*/
    LPTMR0->CMR = Ticks + mPWRLib_RTIElapsedTicks;

    /* Configure prescaler, bypass prescaler and clck source */
    LPTMR0->PSR = ClkMode;
    LPTMR0->CSR |= LPTMR_CSR_TFC_MASK;

    //PWR_DBG_LOG("elapsed_ticks=%d CMR=%d", mPWRLib_RTIElapsedTicks, LPTMR0->CMR);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockCheck
 *---------------------------------------------------------------------------*/
uint32_t PWRLib_LPTMR_ClockCheck(void)
{
    /* LPTMR is still running */
    if((LPTMR0->CSR & LPTMR_CSR_TEN_MASK) != 0U)
    {
        uint32_t cnr;

        LPTMR0->CNR     = 0U;  /* CNR must be written first in order to be read */
        cnr             = LPTMR0->CNR;

        //PWR_DBG_LOG("CNR=%d prev=%d cmp=%d", cnr, mPWRLib_RTIElapsedTicks, LPTMR0->CMR);

        if(cnr < mPWRLib_RTIElapsedTicks)
        {
            cnr += 0x10000U;
            //PWR_DBG_LOG("wrapped: new CNR=%d ", cnr);
        }

        /* clear compare flag if set */
        LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;

        mPWRLib_RTIElapsedTicks = cnr - mPWRLib_RTIElapsedTicks;

        //PWR_DBG_LOG("elapsed=%d", mPWRLib_RTIElapsedTicks);
    }
    else
    {
        /* Start running the lptmr counter */
        mPWRLib_RTIElapsedTicks = 0U;
        LPTMR0->CNR = 0U;
        LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
    }

    return mPWRLib_RTIElapsedTicks;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStop
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStop(void)
{
	PWRLIB_NOP;
}

#else
/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStart
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStart(uint8_t ClkMode, uint32_t Ticks)
{
    OSA_InterruptDisable();

    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;

    /* Set compare value */
    if(Ticks > 0U)
    {
        Ticks--;
    }

    LPTMR0->CMR = Ticks;
    /* Use specified tick count */
    mPWRLib_RTIElapsedTicks = 0;
    /* Configure prescaler, bypass prescaler and clck source */
    LPTMR0->PSR = ClkMode;
    LPTMR0->CSR |= LPTMR_CSR_TFC_MASK;
    /* Start counting */
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;

    OSA_InterruptEnable();
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockCheck
 *---------------------------------------------------------------------------*/
uint32_t PWRLib_LPTMR_ClockCheck(void)
{
    /* LPTMR should be stopped before calling this function */
    assert( (LPTMR0->CSR & LPTMR_CSR_TEN_MASK) == 0U);

    return mPWRLib_RTIElapsedTicks;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ClockStop
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ClockStop(void)
{
    //PWR_DBG_LOG("%x", LPTMR0->CSR & LPTMR_CSR_TEN_MASK);

    /* LPTMR is still running */
    if((LPTMR0->CSR & LPTMR_CSR_TEN_MASK) != 0U)
    {
        LPTMR0->CNR = 0U;  /* CNR must be written first in order to be read */
        mPWRLib_RTIElapsedTicks = LPTMR0->CNR;

#ifndef NDEBUG
        /* timer compare flag is set, counter should not wrapp */
        if((LPTMR0->CSR & LPTMR_CSR_TCF_MASK) != 0U)
        {
            uint32_t compareReg;

            compareReg = LPTMR0->CMR;
            assert(mPWRLib_RTIElapsedTicks >= compareReg);
        }
#endif
    }

    /* Stop LPTMR */
    LPTMR0->CSR &= ~LPTMR_CSR_TEN_MASK;
}

#endif

#if defined(PWR_FUNCTION_NOT_USED)
/*---------------------------------------------------------------------------
 * Name: PWRLib_StopUpdateWakeupReason
 *---------------------------------------------------------------------------*/
void PWRLib_StopUpdateWakeupReason(void)
{
    uint32_t iser;
    uint32_t ispr;

    iser = NVIC->ISER[0];
    ispr = NVIC->ISPR[0];

    if((( iser & ispr) & (1UL << mUartIrq_d)) != 0U )
    {
        PWRLib_MCU_WakeupReason.Bits.FromSerial = 1U;
    }

    if((( iser & ispr) & (1UL << LVD_LVW_DCDC_IRQn)) != 0U)
    {
        PWRLib_MCU_WakeupReason.Bits.FromPSwitch = 1U;
    }

    if(((iser & ispr) & (1UL << Radio_0_IRQn | 1UL << Radio_1_IRQn)) != 0U)
    {
        PWRLib_MCU_WakeupReason.Bits.FromRadio = 1U;
#if cPWR_GENFSK_LL_Enable
        if((GENFSK->IRQ_CTRL & (GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK |GENFSK_IRQ_CTRL_WAKE_IRQ_MASK)) ==
           (GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK |GENFSK_IRQ_CTRL_WAKE_IRQ_MASK))
        {
            PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1U;
        }
#endif /* cPWR_GENFSK_LL_Enable */
    }

    if(((iser & ispr) & (1UL << LPTMR0_IRQn)) != 0U)
    {
        PWRLib_MCU_WakeupReason.Bits.FromLPTMR = 1U;
        PWRLib_MCU_WakeupReason.Bits.DeepSleepTimeout = 1U;
    }

#if gKeyBoardSupported_d
    if(KBD_IsWakeUpSource())
    {
        PWRLib_MCU_WakeupReason.Bits.FromKeyBoard = 1U;
    }
#endif
}
#endif /* PWR_FUNCTION_NOT_USED */

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_Isr
 *---------------------------------------------------------------------------*/
static void PWRLib_LPTMR_Isr(void)
{
    LPTMR0->CSR |= LPTMR_CSR_TCF_MASK;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_Init
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_Init(void)
{
    OSA_InstallIntHandler((uint32_t)LPTMR0_IRQn,PWRLib_LPTMR_Isr);
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;

    LPTMR0->CSR = (LPTMR_CSR_TCF_MASK | LPTMR_CSR_TPS(0x00));  /* Clear control register */
    LPTMR0->CMR = LPTMR_CMR_COMPARE(0x02);  /* Set up compare register */
    NVIC->IP[7] &= ~0x000000FFUL;
    NVIC->IP[7] |=  0x80UL;
    NVIC->ISER[0] |= 0x10000000UL;
    LPTMR0->CSR = (LPTMR_CSR_TIE_MASK | LPTMR_CSR_TPS(0x00));  /* Set up control register */

#if defined(PWRLib_LPTMR_KEEP_RUNNING)
    /* Start running the lptmr counter */
    mPWRLib_RTIElapsedTicks = 0U;
    LPTMR0->CNR = 0U;
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;
#endif
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_LPTMR_ReInit
 * Description: - ReInitialize the LPTMR after wakeup from lowpower (VLLS2/3)
 *---------------------------------------------------------------------------*/
void PWRLib_LPTMR_ReInit(void)
{
    CLOCK_EnableClock(kCLOCK_Lptmr0);
    NVIC_SetPriority(LPTMR0_IRQn, 0x80UL >> (8 - __NVIC_PRIO_BITS));
    NVIC_EnableIRQ(LPTMR0_IRQn);
}


/*---------------------------------------------------------------------------
 * Name: PWRLib_LLWU_Isr
 *---------------------------------------------------------------------------*/
PWRLIB_STATIC void PWRLib_LLWU_Isr(void)
{
    //PWR_DBG_LOG("");
    //DBG_LOG_DUMP(FALSE);
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_WaitFor32kOscRestart
 *---------------------------------------------------------------------------*/
#if defined(PWR_WaitFor32kOscRestart) && (PWR_WaitFor32kOscRestart == 1)
static void PWRLib_WaitFor32kOscRestart(void)
{
    bool_t       lptmr_was_off = FALSE;
    uint32_t     DeepSleepTimeMs;

    if ( (SIM_REG_SCGC5 & SIM_SCGC5_LPTMR_MASK) == 0U )
    {
        /* if LPTMR clock is not set in SCGC5, Assume LPTMR has not been initialized yet */
        lptmr_was_off = TRUE;
        PWRLib_LPTMR_Init();
    }

    PWR_DBG_LOG("%d", lptmr_was_off);

    OSA_InterruptDisable();

    /* keep initial max deep sleep time and change it temporarily to 10ms */
    DeepSleepTimeMs = mPWR_DeepSleepTimeMs;
    mPWR_DeepSleepTimeMs = 10U;

    PWR_HandleDeepSleepMode();

    mPWR_DeepSleepTimeMs= DeepSleepTimeMs;

    OSA_InterruptEnable();

    /* LPTMR was not initialized before going to lowpower - deinit then now */
    if (lptmr_was_off == TRUE)
    {
        LPTMR_Deinit(LPTMR0);
        SIM->SCGC5 &= ~SIM_SCGC5_LPTMR_MASK;
    }
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_CheckAndWaitFor32kOscRestart
 *---------------------------------------------------------------------------*/
static void PWRLib_CheckAndWaitFor32kOscRestart(void)
{
    uint16_t resetStatus = PWR_GetSystemResetStatus();

     /* We check if we come from POR, Reset or VLLS0/1 */

    if ((resetStatus & ( gPWRLib_ResetStatus_POR | gPWRLib_ResetStatus_WAKEUP | gPWRLib_ResetStatus_PIN | gPWRLib_ResetStatus_SW)) != 0U)
    {
        /* we check if the 32kHz was shut down before going to low power */
        if( BOARD_WasRtcShutDown() == TRUE )
        {
            PWRLib_WaitFor32kOscRestart();
        }
    }
}
#endif   /* #if defined(PWR_WaitFor32kOscRestart) && (PWR_WaitFor32kOscRestart == 1) */

#endif /* #if (cPWR_UsePowerDownMode==1) */


/*---------------------------------------------------------------------------
* Name: PWRLib_LVD_CollectLevel
*---------------------------------------------------------------------------*/
PWRLib_LVD_VoltageLevel_t PWRLib_LVD_CollectLevel(void)
{
#if ((cPWR_LVD_Enable == 1) || (cPWR_LVD_Enable == 2))

    /* Check low detect voltage 1.6V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC1 = PMC_LVDSC1_LVDACK_MASK;

    if(PMC->LVDSC1 & PMC_LVDSC1_LVDF_MASK)
    {
        /* Low detect voltage reached */
        PMC->LVDSC1 = PMC_LVDSC1_LVDACK_MASK;

        return(PWR_LEVEL_CRITICAL);
    }

    /* Check low trip voltage 1.8V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_1_8V);
    }

    /* Check low trip voltage 1.9V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(1);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_1_9V);
    }

    /* Check low trip voltage 2.0V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(2);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_2_0V);
    }

    /* Check low trip voltage 2.1V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(3);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_2_1V);
    }

    /* Check low detect voltage (high range) 2.56V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);  /* Set high trip voltage and clear warning flag */
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC1 |= PMC_LVDSC1_LVDACK_MASK;

    if(PMC->LVDSC1 & PMC_LVDSC1_LVDF_MASK)
    {
        /* Low detect voltage reached */
        PMC->LVDSC1 = PMC_LVDSC1_LVDACK_MASK;  /* Set low trip voltage and clear warning flag */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_2_56V);
    }

    /* Check high trip voltage 2.7V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_2_7V);
    }

    /* Check high trip voltage 2.8V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(1);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_2_8V);
    }

    /* Check high trip voltage 2.9V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(2);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_2_9V);
    }

    /* Check high trip voltage 3.0V */
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(1);
    PMC->LVDSC2 = PMC_LVDSC2_LVWV(3);
    PMC->LVDSC2 |= PMC_LVDSC2_LVWACK_MASK;

    if(PMC->LVDSC2 & PMC_LVDSC2_LVWF_MASK)
    {
        /* Low trip voltage reached */
        PMC->LVDSC2 = PMC_LVDSC2_LVWACK_MASK;  /* Clear flag (and set low trip voltage) */
        PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */

        return(PWR_BELOW_LEVEL_3_0V);
    }

    PMC->LVDSC2 = PMC_LVDSC2_LVWV(0);
    PMC->LVDSC1 = PMC_LVDSC1_LVDV(0);  /* Set low trip voltage */
#endif /* ((cPWR_LVD_Enable == 1) || (cPWR_LVD_Enable == 2)) */

    /*--- Voltage level is okay > 3.0V */
    return(PWR_ABOVE_LEVEL_3_0V);
}


/*---------------------------------------------------------------------------
 * Name: PWRLib_LVD_PollIntervalCallback
 *---------------------------------------------------------------------------*/
#if (cPWR_LVD_Enable == 2)
static void PWRLib_LVD_PollIntervalCallback(void* param)
{
    (void)param;
    PWRLib_LVD_SavedLevel = PWRLib_LVD_CollectLevel();
}
#endif /* (cPWR_LVD_Enable == 2) */

/*---------------------------------------------------------------------------
 * Name: PWRLib_SetWakeupPins
 *---------------------------------------------------------------------------*/
void PWRLib_SetWakeupPins(uint32_t wkup_pin_set)
{
    LLWU->PE1 = (uint8_t)(wkup_pin_set    ) & 0xFFU;
    LLWU->PE2 = (uint8_t)(wkup_pin_set>>8 ) & 0xFFU;
    LLWU->PE3 = (uint8_t)(wkup_pin_set>>16) & 0xFFU;
    LLWU->PE4 = (uint8_t)(wkup_pin_set>>24) & 0xFFU;
}

/*---------------------------------------------------------------------------
 * Name: PWRLib_Init
 *---------------------------------------------------------------------------*/
void PWRLib_Init(void)
{
#if (cPWR_UsePowerDownMode == 1)
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK ;

    BOARD_RTC_Init();

#if defined(gPWR_RamOffDuringAdv_d) && (gPWR_RamOffDuringAdv_d == 1)
    /* Prepare the Flash for storing the RAM image */
    if( gPWRLib_RamSavedStatus == PWRLIB_FLASH_TO_ERASE )
    {
        PWRLib_PrepareFlashForRamImage();
    }
#endif

    /* Wakeup from a LLWU wakeup source */
    if((RCM_REG->SRS0 & RCM_SRS0_WAKEUP_MASK) != 0U)
    {
        /* update the wakeup reason, the handling of the wakeup reason shall be done in the application init */
        PWRLib_LLWU_UpdateWakeupReason();

        /* wakeup from external reset pin */
        if((RCM_REG->SRS0 & RCM_SRS0_PIN_MASK) != 0U)
        {
            PWRLib_MCU_WakeupReason.Bits.FromReset = 1U;
        }
    }

    PWRLib_LLWU_Init();

#if (gTMR_EnableLowPowerTimers_d)
    PWRLib_LPTMR_Init();
#endif

    /* Install LLWU ISR and validate it in NVIC */
    OSA_InstallIntHandler((uint32_t)LLWU_IRQn, PWRLib_LLWU_Isr);
    NVIC_SetPriority(LLWU_IRQn, 2);
    NVIC_EnableIRQ(LLWU_IRQn);

#if (gKeyBoardSupported_d == 0)
#if gLowPower_switchPinsToInitBitmap_d
    {
        uint32_t pinIndex = gLowPower_switchPinsToInitBitmap_d;
        uint32_t i = 0U;

        while(pinIndex)
        {
            if(pinIndex & 0x1U)
            {
                (void)GpioInputPinInit(&switchPins[i], 1U);
            }

            pinIndex >>= 1;
            i++;
        }
    }
#endif /* gLowPower_switchPinsToInitBitmap_d */
#endif /* (gKeyBoardSupported_d == 0) */

#if defined(PWR_WaitFor32kOscRestart) && (PWR_WaitFor32kOscRestart == 1)
    PWRLib_CheckAndWaitFor32kOscRestart();
#endif

#if cPWR_GENFSK_LL_Enable
    /* 32MHz oscillator should be ready to be able to access registers */
    assert((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) != 0U);
    SIM->SCGC5 |= SIM_SCGC5_GEN_FSK_MASK;
    GENFSK->IRQ_CTRL |= GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_GENERIC_FSK_IRQ_EN_MASK;
#endif /* cPWR_GENFSK_LL_Enable */

    RSIM->RF_OSC_CTRL  = (RSIM->RF_OSC_CTRL & ~RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_MASK) | RSIM_RF_OSC_CTRL_RADIO_EXT_OSC_OVRD_EN_MASK;

#endif /* (cPWR_UsePowerDownMode == 1) */

    /* LVD_Init TBD */
#if (cPWR_LVD_Enable == 0)
    PMC->LVDSC1 &= (uint8_t) ~( PMC_LVDSC1_LVDIE_MASK  | PMC_LVDSC1_LVDRE_MASK);
    PMC->LVDSC2 &= (uint8_t) ~( PMC_LVDSC2_LVWIE_MASK );
#elif ((cPWR_LVD_Enable == 1) || (cPWR_LVD_Enable == 2))
    PMC->LVDSC1 &= (uint8_t) ~( PMC_LVDSC1_LVDIE_MASK | PMC_LVDSC1_LVDRE_MASK);
    PMC->LVDSC2 &= (uint8_t) ~( PMC_LVDSC2_LVWIE_MASK );
#elif (cPWR_LVD_Enable==3)
    PMC->LVDSC1 = (PMC->LVDSC1 | (uint8_t)PMC_LVDSC1_LVDRE_MASK) & (uint8_t)(~PMC_LVDSC1_LVDIE_MASK );
    PMC->LVDSC2 &= (uint8_t) ~( PMC_LVDSC2_LVWIE_MASK );
#endif /* (cPWR_LVD_Enable) */

#if (cPWR_LVD_Enable == 2)
#if ((cPWR_LVD_Ticks == 0) || (cPWR_LVD_Ticks > 71582))
#error  "*** ERROR: cPWR_LVD_Ticks invalid value"
#endif /* ((cPWR_LVD_Ticks == 0) || (cPWR_LVD_Ticks > 71582)) */

    PWRLib_LVD_SavedLevel = PWRLib_LVD_CollectLevel();

    /* Allocate a platform timer */
    PWRLib_LVD_PollIntervalTmrID = TMR_AllocateTimer();

    if(gTmrInvalidTimerID_c != PWRLib_LVD_PollIntervalTmrID)
    {
        /* start the timer */
        TMR_StartLowPowerTimer(PWRLib_LVD_PollIntervalTmrID, gTmrIntervalTimer_c,TmrMinutes(cPWR_LVD_Ticks) , PWRLib_LVD_PollIntervalCallback, NULL);
    }
#endif /* (cPWR_LVD_Enable == 2) */
}

/*FUNCTION**********************************************************************
 * Function Name : PWRLib_InitOsc0
 * Description   : This function is used to setup MCG OSC with Ref oscillator for KW40_512.
 *END**************************************************************************/
#if defined(cPWR_PwrUseBlpeMode) && (cPWR_PwrUseBlpeMode == 1)
static void PWRLib_InitOsc0(void)
{
#define PWRLIB_XTAL0_CLK_HZ                         32000000U  /*!< Board xtal0 frequency in Hz */
    const osc_config_t oscConfig = {
      .freq = PWRLIB_XTAL0_CLK_HZ, .workMode = kOSC_ModeExt,
    };

    /* Initializes OSC0 according to previous configuration to meet Ref OSC needs. */
    CLOCK_InitOsc0(&oscConfig);

    /* Passing the XTAL0 frequency to clock driver. */
    CLOCK_SetXtal0Freq(PWRLIB_XTAL0_CLK_HZ);
}
#endif

void PWRLib_BLPItoBPE(void)
{
#if defined(cPWR_PwrUseBlpeMode) && (cPWR_PwrUseBlpeMode == 1)
   BOARD_DBGINITSET(5,0);
   BOARD_DBGINITSET(0,0);
#define MCG_IRCLK_DISABLE                                 0U  /*!< MCGIRCLK disabled */
#define SIM_OSC32KSEL_OSC32KCLK_CLK                       0U  /*!< OSC32KSEL select: OSC32KCLK clock */
   const mcg_config_t PWRLib_BootClockRUN =
    {
        .mcgMode = kMCG_ModeBLPE,                 /* BLPE - Bypassed Low Power External */
        .irclkEnableMode = MCG_IRCLK_DISABLE,     /* MCGIRCLK disabled */
        .ircs = kMCG_IrcSlow,                     /* Slow internal reference clock selected */
        .fcrdiv = 0x1U,                           /* Fast IRC divider: divided by 2 */
        .frdiv = 0x5U,                            /* FLL reference clock divider: divided by 1024 */
        .drs = kMCG_DrsLow,                       /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default,               /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselOsc,                 /* Selects System Oscillator (OSCCLK) */
    };

    /* Initializes OSC0 according to Ref OSC needs. */
    PWRLib_InitOsc0();

    BOARD_DBGINITSET(0,1);
    /* Configure FLL external reference divider (FRDIV). */
    MCG->C1 = (uint8_t)((MCG->C1 & ~MCG_C1_FRDIV_MASK) | MCG_C1_FRDIV(PWRLib_BootClockRUN.frdiv));

    /* Set MCG to BLPE mode. */
    //(void)CLOCK_BootToBlpeMode(PWRLib_BootClockRUN.oscsel);
    /* Set to FBE mode. */
    MCG->C1 = (uint8_t)((MCG->C1 & ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK)) |
                        (MCG_C1_CLKS(kMCG_ClkOutSrcExternal)    /* CLKS = 2 */
                         | MCG_C1_IREFS(kMCG_FllSrcExternal))); /* IREFS = 0 */

    BOARD_DBGINITSET(0,0);
    BOARD_DBGINITSET(5,1);
#else
    /* Here add some code to avoid misra issue */
    __asm("NOP");
#endif
}

void PWRLib_BPEtoBLPE(void)
{
#if defined(cPWR_PwrUseBlpeMode) && (cPWR_PwrUseBlpeMode == 1)
    /* MCG_S_CLKST definition. */
    enum _mcg_clkout_stat
    {
        kMCG_ClkOutStatFll, /* FLL.            */
        kMCG_ClkOutStatInt, /* Internal clock. */
        kMCG_ClkOutStatExt, /* External clock. */
        kMCG_ClkOutStatPll  /* PLL.            */
   };

    BOARD_DBGINITSET(5,0);
    /* make sure BPE is reached */
    while ((MCG->S & (MCG_S_IREFST_MASK | MCG_S_CLKST_MASK)) !=
           (MCG_S_IREFST(kMCG_FllSrcExternal) | MCG_S_CLKST(kMCG_ClkOutStatExt)))
    {
         BOARD_DBGINITSET(4,0);
         BOARD_DBGINITSET(4,1);
    }

    /* In FBE now, start to enter BLPE. */
    MCG->C2 |= MCG_C2_LP_MASK;
    BOARD_DBGINITSET(5,1);
#else
    /* Here add some code to avoid misra issue */
    __asm("NOP");
#endif
}

void PWRLib_BLPEtoBLPI(void)
{
#if defined(cPWR_PwrUseBlpeMode) && (cPWR_PwrUseBlpeMode == 1)
    BOARD_DBGINITSET(5,0);
    const mcg_config_t g_blpi_clock_config =
    {
        .mcgMode = kMCG_ModeBLPI,            /* Work in BLPI mode. */
        .irclkEnableMode = (uint8_t)kMCG_IrclkEnable, /* MCGIRCLK enable. */
        .ircs = kMCG_IrcFast,                /* Select IRC4M. */
        .fcrdiv = 0U,                        /* FCRDIV is 0. */

        .frdiv = 5U,
        .drs = kMCG_DrsLow,         /* Low frequency range */
        .dmx32 = kMCG_Dmx32Default, /* DCO has a default range of 25% */
        .oscsel = kMCG_OscselOsc,   /* Select OSC */
    };
   MCG->C2 &= ~(uint8_t)MCG_C2_LP_MASK; /* Disable lowpower. */
    //CLOCK_SetMcgConfig(&g_blpi_clock_config);

    //CLOCK_SetInternalRefClkConfig(g_blpi_clock_config.irclkEnableMode, g_blpi_clock_config.ircs, g_blpi_clock_config.fcrdiv);
    /* Set internal reference clock selection. */

#define MCG_S_IRCST_VAL (((uint32_t)MCG->S & (uint32_t)MCG_S_IRCST_MASK) >> (uint32_t)MCG_S_IRCST_SHIFT)


    /* Update FCRDIV. */
    MCG->SC = (uint8_t)(MCG->SC & ~(MCG_SC_FCRDIV_MASK | MCG_SC_ATMF_MASK | MCG_SC_LOCS0_MASK)) | MCG_SC_FCRDIV(0);
    MCG->C2 = (uint8_t)((MCG->C2 & ~MCG_C2_IRCS_MASK) | (MCG_C2_IRCS(g_blpi_clock_config.ircs)));

        /* Set CLKS and IREFS. */
    MCG->C1 = (uint8_t)((MCG->C1 & ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK)) |
                        (MCG_C1_CLKS(kMCG_ClkOutSrcOut )    /* CLKS = 1 kMCG_ClkOutSrcInternal   kMCG_ClkOutSrcOut*/
                         | MCG_C1_IREFS( kMCG_FllSrcInternal ))); /* IREFS = 1  kMCG_FllSrcInternal   kMCG_FllSrcExternal*/

    BOARD_DBGINITSET(5,1);

    /* not usefull */
    //CLOCK_SetFbeMode(g_blpi_clock_config.frdiv, g_blpi_clock_config.dmx32, g_blpi_clock_config.drs, (void (*)(void))0);
#else
    /* Here add some code to avoid misra issue */
    __asm("NOP");
#endif
}


