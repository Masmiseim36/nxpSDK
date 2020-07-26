/*
*                     Copyright 2016-2019, NXP
*
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Example Source abstracting component data structure and code initialization and code specific to HW used in the examples
* This file shall be present in all examples. A customer does not need to touch/modify this file. This file
* purely depends on the phNxpBuild_Lpc.h or phNxpBuild_App.h
* The phAppInit.h externs the component data structures initialized here that is in turn included by the core examples.
* The core example shall not use any other variable defined here except the RdLib component data structures(as explained above)
* The RdLib component initialization requires some user defined data and function pointers.
* These are defined in the respective examples and externed here.
*
* Keystore and Crypto initialization needs to be handled by application.
*
* $Author: jenkins_ cm (nxp92197) $
* $Revision: 4184 $ (v06.01.00)
* $Date: 2016-01-22 18:04:59 +0530 (Fri, 22 Jan 2016) $
*
* History:
* BK: Generated 25. Sept 2014
*
*/

/* Status header */
#include <ph_Status.h>

#include "phApp_Init.h"

/* NFCLIB Header */
#include <phNfcLib.h>

/* LLCP header */
#include <phlnLlcp.h>

#include <phOsal.h>

#ifdef PH_PLATFORM_HAS_ICFRONTEND
#include "BoardSelection.h"
#endif

#ifdef PHDRIVER_LPC1769
#include "board.h"
#endif /* PHDRIVER_LPC1769 */

#ifdef NXPBUILD__PHHAL_HW_PN5180
/* HAL specific headers */
#include <phhalHw_Pn5180_Instr.h>
#endif

#ifdef PHDRIVER_KINETIS_K82
#include <fsl_port.h>
#include <fsl_pit.h>
#ifdef DEBUG
#include <fsl_clock.h>
#endif
#endif /* PHDRIVER_KINETIS_K82 */

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
#include <phFlashBoot.h>
#include <phhalTimer.h>
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

#ifdef PHDRIVER_KINETIS_K82
#ifdef DEBUG

#define KINETIS_K82_DEBUG_UART_CLK_FREQ         CLOCK_GetOsc0ErClkFreq()
#define KINETIS_K82_DEBUG_UART_BASEADDR        (uint32_t)(LPUART4)
#define KINETIS_K82_DEBUG_UART_INSTANCE        4U
#define KINETIS_K82_DEBUG_UART_BAUDRATE         115200
#define KINETIS_K82_DEBUG_UART_TYPE             DEBUG_CONSOLE_DEVICE_TYPE_LPUART

#endif

/*! @brief Clock configuration structure. */
typedef struct _clock_config
{
    mcg_config_t mcgConfig;       /*!< MCG configuration.      */
    sim_clock_config_t simConfig; /*!< SIM configuration.      */
    osc_config_t oscConfig;       /*!< OSC configuration.      */
    uint32_t coreClock;           /*!< core clock frequency.   */
} clock_config_t;
#endif /* PHDRIVER_KINETIS_K82 */

#ifdef PH_OSAL_LINUX
#    define PI_IRQ_POLLING_TASK_PRIO    0
#    define PI_IRQ_POLLING_TASK_STACK   0x20000
     phOsal_ThreadObj_t gphPiThreadObj;
#endif /* PH_OSAL_LINUX */

#if defined(PHDRIVER_LPC1769) && defined(__CC_ARM)
uint32_t SystemCoreClock;
#endif
/*******************************************************************************
**   Function Declarations
*******************************************************************************/
#ifdef PHDRIVER_KINETIS_K82
static void phApp_K82_Init(void);
#endif /* PHDRIVER_KINETIS_K82 */


#ifdef PH_OSAL_LINUX
static void phExample_IrqPolling(void* param);
#endif


#ifdef PH_PLATFORM_HAS_ICFRONTEND
void CLIF_IRQHandler(void);
#endif

phStatus_t phApp_Configure_IRQ();

/*******************************************************************************
**   Clock configuration of K82 Platform
*******************************************************************************/

#ifdef PHDRIVER_KINETIS_K82
/* Configuration for enter RUN mode. Core clock = 50MHz. */
const clock_config_t g_defaultClockConfigRun = {
    .mcgConfig =
        {
            .mcgMode = kMCG_ModePEE,             /* Work in PEE mode. */
            .irclkEnableMode = kMCG_IrclkEnable, /* MCGIRCLK enable. */
            .ircs = kMCG_IrcSlow,                /* Select IRC32k. */
            .fcrdiv = 0U,                        /* FCRDIV is 0. */

            .frdiv = 4U,
            .drs = kMCG_DrsLow,         /* Low frequency range. */
            .dmx32 = kMCG_Dmx32Default, /* DCO has a default range of 25%. */
            .oscsel = kMCG_OscselOsc,   /* Select OSC. */

            .pll0Config =
                {
                    .enableMode = 0U, .prdiv = 0x01U, .vdiv = 0x01U,
                },
        },
    .simConfig =
        {
            .pllFllSel = 1U,        /* PLLFLLSEL select PLL. */
            .pllFllDiv = 0U,        /* PLLFLLSEL clock divider divisor. */
            .pllFllFrac = 0U,       /* PLLFLLSEL clock divider fraction. */
            .er32kSrc = 5U,         /* ERCLK32K selection, use RTC. */
            .clkdiv1 = 0x01140000U, /* SIM_CLKDIV1. */
        },
    .oscConfig = {.freq = CPU_XTAL_CLK_HZ,
                  .capLoad = 0,
                  .workMode = kOSC_ModeOscLowPower,
                  .oscerConfig =
                      {
                          .enableMode = kOSC_ErClkEnable,
#if (defined(FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER) && FSL_FEATURE_OSC_HAS_EXT_REF_CLOCK_DIVIDER)
                          .erclkDiv = 0U,
#endif
                      }},

};
#endif /* PHDRIVER_KINETIS_K82 */

/*******************************************************************************
**   Global Variable Declaration
*******************************************************************************/
#ifdef PH_PLATFORM_HAS_ICFRONTEND
phbalReg_Type_t                 sBalParams;
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

#ifdef NXPBUILD__PHHAL_HW_PN5180
phhalHw_Pn5180_DataParams_t   * pHal;
#endif

#ifdef NXPBUILD__PHHAL_HW_PN5190
phhalHw_Pn5190_DataParams_t   * pHal;
#endif

#ifdef NXPBUILD__PHHAL_HW_RC663
phhalHw_Rc663_DataParams_t    * pHal;
#endif

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
phhalHw_PN7462AU_DataParams_t * pHal;
#endif

#ifdef NXPBUILD__PHLN_LLCP_SW
phlnLlcp_Sw_DataParams_t           slnLlcp;            /* LLCP component */
#endif /* NXPBUILD__PHLN_LLCP_SW */

/* General information bytes to be sent with ATR Request */
#if defined(NXPBUILD__PHPAL_I18092MPI_SW) || defined(NXPBUILD__PHPAL_I18092MT_SW)
uint8_t aLLCPGeneralBytes[36] = { 0x46,0x66,0x6D,
                                  0x01,0x01,0x10,       /*VERSION*/
                                  0x03,0x02,0x00,0x01,  /*WKS*/
                                  0x04,0x01,0xF1        /*LTO*/
                                 };
uint8_t   bLLCPGBLength = 13;
#endif

/* ATR Response or ATS Response holder */
#if defined(NXPBUILD__PHPAL_I14443P4A_SW)     || \
    defined(NXPBUILD__PHPAL_I18092MPI_SW)
uint8_t    aResponseHolder[64];
#endif

#ifdef NXPBUILD__PHHAL_HW_TARGET
/* Parameters for L3 activation during Autocoll */
extern uint8_t  sens_res[2]    ;
extern uint8_t  nfc_id1[3]     ;
extern uint8_t  sel_res        ;
extern uint8_t  nfc_id3        ;
extern uint8_t  poll_res[18]   ;
#endif /* NXPBUILD__PHHAL_HW_TARGET */

/* prints if error is detected */
#define CHECK_SUCCESS(x)              \
    if ((x) != PH_ERR_SUCCESS)        \
{                                     \
    DEBUG_PRINTF("\nLine: %d   Error - (0x%04X) has occurred : 0xCCEE CC-Component ID, EE-Error code. Refer-ph_Status.h\n ", __LINE__, (x)); \
    return (x);                       \
}

/*******************************************************************************
**   Function Definitions
*******************************************************************************/
#ifdef PHDRIVER_KINETIS_K82
static void phApp_K82_Init(void)
{
#ifdef DEBUG
    uint32_t uartClkSrcFreq;
#endif /* DEBUG */

    pit_config_t pitConfig;         /* Structure of initialize PIT */

    CLOCK_SetSimSafeDivs();

    CLOCK_InitOsc0(&g_defaultClockConfigRun.oscConfig);
    CLOCK_SetXtal0Freq(CPU_XTAL_CLK_HZ);

    CLOCK_BootToPeeMode(g_defaultClockConfigRun.mcgConfig.oscsel, kMCG_PllClkSelPll0,
        &g_defaultClockConfigRun.mcgConfig.pll0Config);

    CLOCK_SetInternalRefClkConfig(g_defaultClockConfigRun.mcgConfig.irclkEnableMode,
        g_defaultClockConfigRun.mcgConfig.ircs, g_defaultClockConfigRun.mcgConfig.fcrdiv);

    CLOCK_SetSimConfig(&g_defaultClockConfigRun.simConfig);

    SystemCoreClockUpdate();

    /*
     * pitConfig.enableRunInDebug = false;
     */
    PIT_GetDefaultConfig(&pitConfig);

    /* Init pit module */
    PIT_Init(PIT, &pitConfig);

#ifdef DEBUG

    /* Initialize LPUART4 pins below used to Print */
    /* Ungate the port clock */
    CLOCK_EnableClock(kCLOCK_PortC);
    /* Affects PORTC_PCR14 register */
    PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);
    /* Affects PORTC_PCR15 register */
    PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);

    /* SIM_SOPT2[27:26]:
     *  00: Clock Disabled
     *  01: MCGFLLCLK, or MCGPLLCLK, or IRC48M
     *  10: OSCERCLK
     *  11: MCGIRCCLK
     */
    CLOCK_SetLpuartClock(2);

    uartClkSrcFreq = KINETIS_K82_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(KINETIS_K82_DEBUG_UART_INSTANCE, KINETIS_K82_DEBUG_UART_BAUDRATE, KINETIS_K82_DEBUG_UART_TYPE, uartClkSrcFreq);
#endif /* DEBUG */
}
#endif /* PHDRIVER_KINETIS_K82 */

/**
* This function will initialize Host Controller.
* Any initialization which is not generic across Platforms, should be done here.
*/
void phApp_CPU_Init(void)
{
#if defined PHDRIVER_KINETIS_K82
    phApp_K82_Init();
#elif defined NXPBUILD__PHHAL_HW_PN7462AU
    phFlashBoot_Main();

    phhalTimer_Init();
#elif defined(PHDRIVER_LPC1769) && defined(__CC_ARM)
    SystemCoreClock =  (( unsigned long ) 96000000);
#else
    /* In case of LPC series, startup file takes care of initializing clock and ports.
     * No initialization is required in Linux environment. */
#endif
}

/**
* This function will initialize Reader LIbrary Component
*/
phStatus_t phApp_Comp_Init(void * pDiscLoopParams)
{
    phStatus_t wStatus = PH_ERR_SUCCESS;
#if defined(NXPBUILD__PHPAL_I18092MPI_SW) || defined(NXPBUILD__PHPAL_I18092MT_SW) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEF212_P2P_ACTIVE) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEF424_P2P_ACTIVE)

    phacDiscLoop_Sw_DataParams_t * pDiscLoop = (phacDiscLoop_Sw_DataParams_t *)pDiscLoopParams;
#endif

/* Initialize the LLCP component */
#ifdef NXPBUILD__PHLN_LLCP_SW
    slnLlcp.sLocalLMParams.wMiu = 0x00; /* 128 bytes only */
    slnLlcp.sLocalLMParams.wWks = 0x11; /* SNEP & LLCP */
    slnLlcp.sLocalLMParams.bLto = 100; /* Maximum LTO */
    slnLlcp.sLocalLMParams.bOpt = 0x02;
    slnLlcp.sLocalLMParams.bAvailableTlv = PHLN_LLCP_TLV_MIUX_MASK | PHLN_LLCP_TLV_WKS_MASK |
        PHLN_LLCP_TLV_LTO_MASK | PHLN_LLCP_TLV_OPT_MASK;

    wStatus = phlnLlcp_Sw_Init(
        &slnLlcp,
        sizeof(phlnLlcp_Sw_DataParams_t),
        aLLCPGeneralBytes,
        &bLLCPGBLength);
#endif /* NXPBUILD__PHLN_LLCP_SW */

#ifdef NXPBUILD__PHAC_DISCLOOP_SW
#if defined(NXPBUILD__PHPAL_I18092MPI_SW) || defined(NXPBUILD__PHPAL_I18092MT_SW)
    /* Assign the GI for Type A */
    pDiscLoop->sTypeATargetInfo.sTypeA_P2P.pGi       = (uint8_t *)aLLCPGeneralBytes;
    pDiscLoop->sTypeATargetInfo.sTypeA_P2P.bGiLength = bLLCPGBLength;
    /* Assign the GI for Type F */
    pDiscLoop->sTypeFTargetInfo.sTypeF_P2P.pGi       = (uint8_t *)aLLCPGeneralBytes;
    pDiscLoop->sTypeFTargetInfo.sTypeF_P2P.bGiLength = bLLCPGBLength;
#endif

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    /* Assign ATR response for Type A */
    pDiscLoop->sTypeATargetInfo.sTypeA_P2P.pAtrRes   = aResponseHolder;
#endif
#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_P2P_TAGS) ||  defined(NXPBUILD__PHAC_DISCLOOP_TYPEF212_P2P_ACTIVE) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEF424_P2P_ACTIVE)
    /* Assign ATR response for Type F */
    pDiscLoop->sTypeFTargetInfo.sTypeF_P2P.pAtrRes   = aResponseHolder;
#endif
#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS
    /* Assign ATS buffer for Type A */
    pDiscLoop->sTypeATargetInfo.sTypeA_I3P4.pAts     = aResponseHolder;
#endif /* NXPBUILD__PHAC_DISCLOOP_TYPEA_I3P4_TAGS */
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */
    return wStatus;
}

/**
* This function will initialize Hal Target Config
*/
phStatus_t phApp_HALConfigAutoColl(void)
{
#ifdef NXPBUILD__PHHAL_HW_TARGET
    phStatus_t wStatus;
#endif /* NXPBUILD__PHHAL_HW_TARGET */

#if defined(NXPBUILD__PHHAL_HW_PN5180) && defined(NXPBUILD__PHHAL_HW_TARGET)
    uint8_t aCmd[1] = {0x01};   /* Enable Random UID feature on Pn5180 FW. */
#endif

#if defined(NXPBUILD__PHHAL_HW_PN7462AU) && defined(NXPBUILD__PHHAL_HW_TARGET)
    uint8_t bSystemCode[] = {0xFF, 0xFF};
#endif /* NXPBUILD__PHHAL_HW_PN7462AU && NXPBUILD__PHHAL_HW_TARGET */

#if defined(NXPBUILD__PHHAL_HW_PN5180) && defined(NXPBUILD__PHHAL_HW_TARGET)
    /* Set Listen Parameters in HAL Buffer used during Autocoll */
    wStatus = phhalHw_Pn5180_SetListenParameters(
        pHal,
        &sens_res[0],
        &nfc_id1[0],
        sel_res,
        &poll_res[0],
        nfc_id3);
    CHECK_SUCCESS(wStatus);

    if (pHal->wFirmwareVer < 0x308)
    {
        /* With Pn5180 FW version < 3.8, static UID is supported by default. */
        aCmd[0] = 0x00;
    }
    /* Enabling the Random UID in 5180 EEPROM */
    wStatus = phhalHw_Pn5180_Instr_WriteE2Prom(
        (phhalHw_Pn5180_DataParams_t *) pHal,
        0x58,
        aCmd,
        0x01
        );

    CHECK_SUCCESS(wStatus);
#endif

#if defined(NXPBUILD__PHHAL_HW_PN7462AU) && defined(NXPBUILD__PHHAL_HW_TARGET)
    /* Set Listen Parameters in HAL Buffer used during Autocoll */
    wStatus = phhalHw_PN7462AU_SetListenParam(
       pHal,
       &sens_res[0],
       &nfc_id1[0],
       sel_res,
       (poll_res[0] == 0x02 ? PH_ON : PH_OFF),
       &poll_res[2],
       &poll_res[8],
       bSystemCode);
    CHECK_SUCCESS(wStatus);
#endif

    return PH_ERR_SUCCESS;
}

phStatus_t phApp_Configure_IRQ()
{
#ifdef PH_OSAL_LINUX
    phStatus_t  wStatus;
#endif /* PH_OSAL_LINUX */

#ifdef PH_PLATFORM_HAS_ICFRONTEND
    phDriver_Pin_Config_t pinCfg;

    pinCfg.bOutputLogic = PH_DRIVER_SET_LOW;
    pinCfg.bPullSelect = PHDRIVER_PIN_IRQ_PULL_CFG;

    pinCfg.eInterruptConfig = PIN_IRQ_TRIGGER_TYPE;
    phDriver_PinConfig(PHDRIVER_PIN_IRQ, PH_DRIVER_PINFUNC_INTERRUPT, &pinCfg);

#ifdef PHDRIVER_LPC1769
    NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
    /* Enable interrupt in the NVIC */
    NVIC_ClearPendingIRQ(EINT_IRQn);
    NVIC_EnableIRQ(EINT_IRQn);
#endif /* PHDRIVER_LPC1769 */

#ifdef PH_OSAL_LINUX

    gphPiThreadObj.pTaskName = (uint8_t *) "IrqPolling";
    gphPiThreadObj.pStackBuffer = NULL;
    gphPiThreadObj.priority = PI_IRQ_POLLING_TASK_PRIO;
    gphPiThreadObj.stackSizeInNum = PI_IRQ_POLLING_TASK_STACK;
    PH_CHECK_SUCCESS_FCT(wStatus, phOsal_ThreadCreate(&gphPiThreadObj.ThreadHandle, &gphPiThreadObj,
        &phExample_IrqPolling, NULL));

#endif /* PH_OSAL_LINUX */

#ifdef PHDRIVER_KINETIS_K82
    NVIC_SetPriority(EINT_IRQn, EINT_PRIORITY);
    NVIC_ClearPendingIRQ(EINT_IRQn);
    EnableIRQ(EINT_IRQn);
#endif /* PHDRIVER_KINETIS_K82 */

#endif /* #ifdef PH_PLATFORM_HAS_ICFRONTEND */

    return PH_ERR_SUCCESS;
}

#ifdef PH_OSAL_LINUX
/*
 * \brief: The purpose of this Thread is to detect RF signal from an External Peer .
 */
static void phExample_IrqPolling(void* param)
{
    uint8_t bgpioVal = 0;
    uint8_t bhighOrLow = 0;

    if(PIN_IRQ_TRIGGER_TYPE ==  PH_DRIVER_INTERRUPT_RISINGEDGE)
    {
        bhighOrLow = 1;
    }

    while(PiGpio_read(PHDRIVER_PIN_IRQ, &bgpioVal) != PH_ERR_SUCCESS)
    {
        PiGpio_unexport(PHDRIVER_PIN_IRQ);
        PiGpio_export(PHDRIVER_PIN_IRQ);
        PiGpio_set_direction(PHDRIVER_PIN_IRQ, false);

        if(PIN_IRQ_TRIGGER_TYPE ==  PH_DRIVER_INTERRUPT_RISINGEDGE)
        {
            PiGpio_set_edge(PHDRIVER_PIN_IRQ, true, false);
        }
        else
        {
            PiGpio_set_edge(PHDRIVER_PIN_IRQ, false, true);
        }
    }

    /* Initial status: If pin is already Active, post an event. */
    if(bgpioVal == bhighOrLow)
    {
        CLIF_IRQHandler();
    }

    while(1)
    {
        /* Block forever for Raising Edge in PHDRIVER_PIN_IRQ. */
        if(PiGpio_poll(PHDRIVER_PIN_IRQ, bhighOrLow, -1) == PH_ERR_SUCCESS)
        {
            CLIF_IRQHandler();
        }
        else
        {
            PiGpio_unexport(PHDRIVER_PIN_IRQ);

            PiGpio_export(PHDRIVER_PIN_IRQ);

            PiGpio_set_direction(PHDRIVER_PIN_IRQ, false);

            if(PIN_IRQ_TRIGGER_TYPE ==  PH_DRIVER_INTERRUPT_RISINGEDGE)
            {
                PiGpio_set_edge(PHDRIVER_PIN_IRQ, true, false);
            }
            else
            {
                PiGpio_set_edge(PHDRIVER_PIN_IRQ, false, true);
            }
        }


    }
}
#endif /* PH_OSAL_LINUX */


#ifdef PH_PLATFORM_HAS_ICFRONTEND
void CLIF_IRQHandler(void)
{
    /* Read the interrupt status of external interrupt attached to the reader IC IRQ pin */
    if (phDriver_PinRead(PHDRIVER_PIN_IRQ, PH_DRIVER_PINFUNC_INTERRUPT))
    {
        phDriver_PinClearIntStatus(PHDRIVER_PIN_IRQ);
        /* Call application registered callback. */
        if (pHal->pRFISRCallback != NULL)
        {
            pHal->pRFISRCallback(pHal);
        }
    }
}
#endif /* PH_PLATFORM_HAS_ICFRONTEND */


#ifdef NXPBUILD__PHHAL_HW_RC663
/* Configure LPCD (for CLRC663) */
phStatus_t phApp_ConfigureLPCD(void)
{
    phStatus_t status;
    uint8_t bValueI;
    uint8_t bValueQ;

    /* Configure LPCD calibration standby time (T4) and RF ON time (T3). */
    status = phhalHw_Rc663_Cmd_Lpcd_SetConfig(
        pHal,
        PHHAL_HW_RC663_CMD_LPCD_MODE_DEFAULT,
        0,
        0,
        PH_NFCRDLIB_EXAMPLE_LPCD_GUARDTIME,
        PH_NFCRDLIB_EXAMPLE_LPCD_RFON_TIME);

    /* Perform LPCD calibration to get I and Q values. */
    status = phhalHw_Rc663_Cmd_Lpcd_GetConfig(pHal, &bValueI, &bValueQ);
    CHECK_SUCCESS(status);

    /* Configure I and Q values for LPCD detection cycle. */
    status = phhalHw_Rc663_Cmd_Lpcd_SetConfig(
        pHal,
        PHHAL_HW_RC663_CMD_LPCD_MODE_POWERDOWN,
        bValueI,
        bValueQ,
        PH_NFCRDLIB_EXAMPLE_LPCD_GUARDTIME,
        PH_NFCRDLIB_EXAMPLE_LPCD_RFON_TIME);

    return status;
}
#endif

#ifdef NXPBUILD__PHHAL_HW_PN5180
/* Configure LPCD (for PN5180) */
phStatus_t phApp_ConfigureLPCD(void)
{
    /**
     * PHHAL_HW_CONFIG_SET_LPCD_WAKEUPTIME_MS  0x0070U     //< Used value for wakeup counter in msecs, i.e. after this amount of time IC will wakes up from standby.
     * PHHAL_HW_CONFIG_LPCD_MODE               0x0071U     //< Used to set options  PHHAL_HW_PN5180_LPCD_MODE_DEFAULT or PHHAL_HW_PN5180_LPCD_MODE_POWERDOWN_GUARDED
     * PHHAL_HW_CONFIG_LPCD_REF                0x0072U     //< Used to set or get LPCD Ref
     */
        phStatus_t status = PH_ERR_SUCCESS;
        uint16_t wConfig = PHHAL_HW_CONFIG_LPCD_REF;
        uint16_t wValue;
        uint8_t bLPCD_Threshold_EEPROMAddress = 0x37;
        uint8_t bLPCD_Threshold = 0x10;
        wValue = PHHAL_HW_PN5180_LPCD_MODE_POWERDOWN;
        wConfig = PHHAL_HW_CONFIG_LPCD_MODE;

        //status = phhalHw_Pn5180_Int_LPCD_GetConfig(pHal, wConfig, &wValue);
        status = phhalHw_Pn5180_Instr_WriteE2Prom(pHal,bLPCD_Threshold_EEPROMAddress, &bLPCD_Threshold, 1 );
        CHECK_SUCCESS(status);
        status = phhalHw_Pn5180_Int_LPCD_SetConfig(
            pHal,
            wConfig,
            wValue
            );

        return status;
}
#endif

#ifdef NXPBUILD__PHHAL_HW_PN7462AU
/* Configure LPCD (for PN7462AU) */
phStatus_t phApp_ConfigureLPCD(void)
{
    phStatus_t status;
    uint16_t wLpcdValue;

    wLpcdValue = 0;

    status = phhalHw_GetConfig(pHal, PHHAL_HW_CONFIG_PN7462AU_LPCD_NEW_VALUE, &wLpcdValue);
    CHECK_SUCCESS(status);

    if (wLpcdValue != 0x00)
    {
        status = phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_PN7462AU_LPCD_REF_VALUE, wLpcdValue);
        CHECK_SUCCESS(status);
    }

    return status;
}
#endif /* NXPBUILD__PHHAL_HW_PN7462AU */

/* Print technology being resolved */
void phApp_PrintTech(uint8_t TechType)
{
    switch(TechType)
    {
    case PHAC_DISCLOOP_POS_BIT_MASK_A:
        DEBUG_PRINTF ("\tResolving Type A... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_B:
        DEBUG_PRINTF ("\tResolving Type B... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_F212:
        DEBUG_PRINTF ("\tResolving Type F with baud rate 212... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_F424:
        DEBUG_PRINTF ("\tResolving Type F with baud rate 424... \n");
        break;

    case PHAC_DISCLOOP_POS_BIT_MASK_V:
        DEBUG_PRINTF ("\tResolving Type V... \n");
        break;

    default:
        break;
    }
}

/**
* This function will print buffer content
* \param   *pBuff   Buffer Reference
* \param   num      data size to be print
*/
void phApp_Print_Buff(uint8_t *pBuff, uint8_t num)
{
    uint32_t    i;

    for(i = 0; i < num; i++)
    {
        DEBUG_PRINTF(" %02X",pBuff[i]);
    }
}

/**
* This function will print Tag information
* \param   pDataParams      The discovery loop data parameters
* \param   wNumberOfTags    Total number of tags detected
* \param   wTagsDetected    Technology Detected
*/
void phApp_PrintTagInfo(phacDiscLoop_Sw_DataParams_t *pDataParams, uint16_t wNumberOfTags, uint16_t wTagsDetected)
{
#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS) || \
    defined(NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS)
    uint8_t bIndex;
#endif
#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    uint8_t bTagType;
#endif

#if defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_TAGS) || defined(NXPBUILD__PHAC_DISCLOOP_TYPEA_P2P_ACTIVE)
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
    {
        if(pDataParams->sTypeATargetInfo.bT1TFlag)
        {
            DEBUG_PRINTF("\tTechnology  : Type A");
            DEBUG_PRINTF ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].aUid,
                        pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].bUidSize);
            DEBUG_PRINTF ("\n\t\tSAK : 0x%02x",pDataParams->sTypeATargetInfo.aTypeA_I3P3[0].aSak);
            DEBUG_PRINTF ("\n\t\tType: Type 1 Tag\n");
        }
        else
        {
            DEBUG_PRINTF("\tTechnology  : Type A");
            for(bIndex = 0; bIndex < wNumberOfTags; bIndex++)
            {
                DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
                DEBUG_PRINTF ("\n\t\tUID :");
                phApp_Print_Buff( pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aUid,
                            pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].bUidSize);
                DEBUG_PRINTF ("\n\t\tSAK : 0x%02x",pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak);

                if ((pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & (uint8_t) ~0xFB) == 0)
                {
                    /* Bit b3 is set to zero, [Digital] 4.8.2 */
                    /* Mask out all other bits except for b7 and b6 */
                    bTagType = (pDataParams->sTypeATargetInfo.aTypeA_I3P3[bIndex].aSak & 0x60);
                    bTagType = bTagType >> 5;

                    switch(bTagType)
                    {
                    case PHAC_DISCLOOP_TYPEA_TYPE2_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: Type 2 Tag\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE4A_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: Type 4A Tag\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: P2P\n");
                        break;
                    case PHAC_DISCLOOP_TYPEA_TYPE_NFC_DEP_TYPE4A_TAG_CONFIG_MASK:
                        DEBUG_PRINTF ("\n\t\tType: Type NFC_DEP and  4A Tag\n");
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEB_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
    {
        DEBUG_PRINTF("\tTechnology  : Type B");
        /* Loop through all the Type B tags detected and print the Pupi */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID :");
            /* PUPI Length is always 4 bytes */
            phApp_Print_Buff( pDataParams->sTypeBTargetInfo.aTypeB_I3P3[bIndex].aPupi, 0x04);
        }
        DEBUG_PRINTF("\n");
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEF_TAGS
    if( PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212) ||
        PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
    {
        DEBUG_PRINTF("\tTechnology  : Type F");

        /* Loop through all the type F tags and print the IDm */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm,
                        PHAC_DISCLOOP_FELICA_IDM_LENGTH );
            if ((pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[0] == 0x01) &&
                (pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].aIDmPMm[1] == 0xFE))
            {
                /* This is Type F tag with P2P capabilities */
                DEBUG_PRINTF ("\n\t\tType: P2P");
            }
            else
            {
                /* This is Type F T3T tag */
                DEBUG_PRINTF ("\n\t\tType: Type 3 Tag");
            }

            if(pDataParams->sTypeFTargetInfo.aTypeFTag[bIndex].bBaud != PHAC_DISCLOOP_CON_BITR_212)
            {
                DEBUG_PRINTF ("\n\t\tBit Rate: 424\n");
            }
            else
            {
                DEBUG_PRINTF ("\n\t\tBit Rate: 212\n");
            }
        }
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_TYPEV_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
    {
        DEBUG_PRINTF("\tTechnology  : Type V / ISO 15693 / T5T");
        /* Loop through all the Type V tags detected and print the UIDs */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF ("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF ("\n\t\tUID :");
            phApp_Print_Buff( pDataParams->sTypeVTargetInfo.aTypeV[bIndex].aUid, 0x08);
        }
        DEBUG_PRINTF("\n");
    }
#endif

#ifdef NXPBUILD__PHAC_DISCLOOP_I18000P3M3_TAGS
    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_18000P3M3))
    {
        DEBUG_PRINTF("\tTechnology  : ISO 18000p3m3 / EPC Gen2");
        /* Loop through all the 18000p3m3 tags detected and print the UII */
        for (bIndex = 0; bIndex < wNumberOfTags; bIndex++)
        {
            DEBUG_PRINTF("\n\t\tCard: %d",bIndex + 1);
            DEBUG_PRINTF("\n\t\tUII :");
            phApp_Print_Buff(
                pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].aUii,
                (pDataParams->sI18000p3m3TargetInfo.aI18000p3m3[bIndex].wUiiLength / 8));
        }
        DEBUG_PRINTF("\n");
    }
#endif
}

/**
* This function will print Error information received from Reader Lib
* \param   wStatus      Error status
*/
void PrintErrorInfo(phStatus_t wStatus)
{
    DEBUG_PRINTF("\n ErrorInfo Comp:");

    switch(wStatus & 0xFF00)
    {
    case PH_COMP_BAL:
        DEBUG_PRINTF("\t PH_COMP_BAL");
        break;
    case PH_COMP_HAL:
        DEBUG_PRINTF("\t PH_COMP_HAL");
        break;
    case PH_COMP_PAL_ISO14443P3A:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P3A");
        break;
    case PH_COMP_PAL_ISO14443P3B:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P3B");
        break;
    case PH_COMP_PAL_ISO14443P4A:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P4A");
        break;
    case PH_COMP_PAL_ISO14443P4:
        DEBUG_PRINTF("\t PH_COMP_PAL_ISO14443P4");
        break;
    case PH_COMP_PAL_FELICA:
        DEBUG_PRINTF("\t PH_COMP_PAL_FELICA");
        break;
    case PH_COMP_PAL_EPCUID:
        DEBUG_PRINTF("\t PH_COMP_PAL_EPCUID");
        break;
    case PH_COMP_PAL_SLI15693:
        DEBUG_PRINTF("\t PH_COMP_PAL_SLI15693");
        break;
    case PH_COMP_PAL_I18000P3M3:
        DEBUG_PRINTF("\t PH_COMP_PAL_I18000P3M3");
        break;
    case PH_COMP_PAL_I18092MPI:
        DEBUG_PRINTF("\t PH_COMP_PAL_I18092MPI");
        break;
    case PH_COMP_PAL_I18092MT:
        DEBUG_PRINTF("\t PH_COMP_PAL_I18092MT");
        break;
    case PH_COMP_PAL_GENERALTARGET:
        DEBUG_PRINTF("\t PH_COMP_PAL_GENERALTARGET");
        break;
    case PH_COMP_PAL_I14443P4MC:
        DEBUG_PRINTF("\t PH_COMP_PAL_I14443P4MC");
        break;
    case PH_COMP_AC_DISCLOOP:
        DEBUG_PRINTF("\t PH_COMP_AC_DISCLOOP");
        break;
    case PH_COMP_OSAL:
        DEBUG_PRINTF("\t PH_COMP_OSAL");
        break;
    default:
        DEBUG_PRINTF("\t 0x%x",(wStatus & PH_COMPID_MASK));
        break;
    }

    DEBUG_PRINTF("\t type:");

    switch(wStatus & PH_ERR_MASK)
    {
    case PH_ERR_SUCCESS_INCOMPLETE_BYTE:
        DEBUG_PRINTF("\t PH_ERR_SUCCESS_INCOMPLETE_BYTE");
        break;
    case PH_ERR_IO_TIMEOUT:
        DEBUG_PRINTF("\t PH_ERR_IO_TIMEOUT");
        break;
    case PH_ERR_INTEGRITY_ERROR:
        DEBUG_PRINTF("\t PH_ERR_INTEGRITY_ERROR");
        break;
    case PH_ERR_COLLISION_ERROR:
        DEBUG_PRINTF("\t PH_ERR_COLLISION_ERROR");
        break;
    case PH_ERR_BUFFER_OVERFLOW:
        DEBUG_PRINTF("\t PH_ERR_BUFFER_OVERFLOW");
        break;
    case PH_ERR_FRAMING_ERROR:
        DEBUG_PRINTF("\t PH_ERR_FRAMING_ERROR");
        break;
    case PH_ERR_PROTOCOL_ERROR:
        DEBUG_PRINTF("\t PH_ERR_PROTOCOL_ERROR");
        break;
    case PH_ERR_RF_ERROR:
        DEBUG_PRINTF("\t PH_ERR_RF_ERROR");
        break;
    case PH_ERR_EXT_RF_ERROR:
        DEBUG_PRINTF("\t PH_ERR_EXT_RF_ERROR");
        break;
    case PH_ERR_NOISE_ERROR:
        DEBUG_PRINTF("\t PH_ERR_NOISE_ERROR");
        break;
    case PH_ERR_ABORTED:
        DEBUG_PRINTF("\t PH_ERR_ABORTED");
        break;
    case PH_ERR_INTERNAL_ERROR:
        DEBUG_PRINTF("\t PH_ERR_INTERNAL_ERROR");
        break;
    case PH_ERR_INVALID_DATA_PARAMS:
        DEBUG_PRINTF("\t PH_ERR_INVALID_DATA_PARAMS");
        break;
    case PH_ERR_INVALID_PARAMETER:
        DEBUG_PRINTF("\t PH_ERR_INVALID_PARAMETER");
        break;
    case PH_ERR_PARAMETER_OVERFLOW:
        DEBUG_PRINTF("\t PH_ERR_PARAMETER_OVERFLOW");
        break;
    case PH_ERR_UNSUPPORTED_PARAMETER:
        DEBUG_PRINTF("\t PH_ERR_UNSUPPORTED_PARAMETER");
        break;
    case PH_ERR_OSAL_ERROR:
        DEBUG_PRINTF("\t PH_ERR_OSAL_ERROR");
        break;
    case PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED:
        DEBUG_PRINTF("\t PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED");
        break;
    case PHAC_DISCLOOP_COLLISION_PENDING:
        DEBUG_PRINTF("\t PHAC_DISCLOOP_COLLISION_PENDING");
        break;
    default:
        DEBUG_PRINTF("\t 0x%x",(wStatus & PH_ERR_MASK));
        break;
    }
}


/******************************************************************************
**                            End Of File
******************************************************************************/
