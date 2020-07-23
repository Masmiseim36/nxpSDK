/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "main.h"
#include "m1_state_machine.h"
#include "safety_periph_init.h"
#include "mcdrv_periph_init.h"
#include "app_periph_init.h"
#include "safety_routines.h"
#include "safety_error_handler.h"
#include "safety_flow_check.h"
#if defined(MID)
#include "mid_sm_states.h"
#endif


MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "freemaster_tsa_pmsm.h"

MISRAC_ENABLE /* MISRA check enable (IAR IDE only). */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Initialization of RAM sections after startup. Defined in start-up module. */
extern void fsCommonStartup(void);

/*******************************************************************************
 * Variables
******************************************************************************/
/* Fast-control loop CPU usage measurement. */
extern cpu_tick_meas_t g_sCpuTickMeas;

/* The FOC motor control variables. */
static struct
{
    foc_ctrl_mode_t eCtrlMode;      /* Selected control mode. */
    foc_app_cmd_t   eAppCmd;        /* The application command. */
    GMCLIB_2COOR_DQ_T_F16 sIDQCmd;  /* The voltage control command. */
    GMCLIB_2COOR_DQ_T_F16 sUDQCmd;  /* The current control command. */
    frac16_t        f16SpdElCmd;    /* The speed command. */
    frac16_t        f16PosElCmd;    /* The speed command. */
    frac16_t        f16FrqElCmd;    /* The frequency command. */
    bool_t          bValidCmd;      /* Command validity flag. */
} sCmdM1;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief   The application FreeMASTER TSA table static function.
 *
 * @details See freemaster_tsa.h for more details about TSA table construction.
 *
 * @param   TSA table identificator.
 *
 * @return  FMSTR_TSA_ENTRY - The TSA table entry data type.
 */
MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */
FMSTR_TSA_TABLE_BEGIN_FS(TSA_MAIN)
    /* Global variables & control. */
    FMSTR_TSA_RO_VAR_FS(D00, g_sCpuTickMeas.ui32MeasAct, FMSTR_TSA_UINT32)  /* Actual measured ticks per fast loop. */
    FMSTR_TSA_RW_VAR_FS(D01, g_sCpuTickMeas.ui32MeasMax, FMSTR_TSA_UINT32)  /* Maximal measured ticks per fast loop. */

    FMSTR_TSA_RW_VAR_FS(C00, sCmdM1.f16SpdElCmd,  FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(C01, sCmdM1.f16PosElCmd,  FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(C02, sCmdM1.f16FrqElCmd,  FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(C03, sCmdM1.sIDQCmd.f16D, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(C04, sCmdM1.sUDQCmd.f16D, FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR_FS(C05, sCmdM1.eAppCmd,      FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR_FS(C06, sCmdM1.eCtrlMode,    FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR_FS(C07, sCmdM1.bValidCmd,    FMSTR_TSA_SINT16)
FMSTR_TSA_TABLE_END_FS(TSA_MAIN)
MISRAC_ENABLE /* MISRA check enable (IAR IDE only). */

/*!
 * @brief  The main function.
 *
 * @param  void
 *
 * @return none
 */
int32_t main(void)
{
    uint32_t ui32fsPrimaskReg;
    /* In-rush relay control flag. */
    uint16_t ui16fsInrushRelayDly = M1_INRUSH_RELAY_DLY;

    /* Disable all interrupts. */
    ui32fsPrimaskReg = __get_PRIMASK();
    __disable_irq();

    /* Initialize  safety related peripherals (clocks, pin-muxes, watchdog,...). */
    FSPER_fsInit();

    /* Initialization of common safety functions and watchdog enable/disable. */
    FS_fsCommonInit_AR();

    /* Watchdog after-reset test. */
    FS_fsWatchdogTest_AR();

    /* Initialize  serial port, and CPU load measurement. */
    APPPER_Init();

    /* Start the CPU tick number counting for in-rush relay control. */
    APPPER_CpuTickMeasStart();

    /* After-reset flash test. */
    FS_fsFlashTest_AR();

    /* RAM after-reset test for safety-related and stack memories. */
    FS_fsRamTest_AR();

    /* Program counter test. */
    FS_fsPcTest_AR();

    /* CPU registers test after reset. */
    FS_fsCpuTest_AR();

    /* Stack overflow and underflow test. */
    FS_fsStackTest_AR();

    /* Refresh watchdog. */
    FS_WDOG_REFRESH();

    /* Initialize FreeMASTER. */
    FMSTR_Init();

    /* Initialize FreeMASTER pipe messages. */
    FMSTR_PipeMessage_Init();

    /* Signalize that the initialization was completed. */
    FMSTR_PipeMessage(FMSTR_MSG_INIT_DONE);

    /* Refresh watchdog. */
    FS_WDOG_REFRESH();

    /* Initialize motor control peripherals. */
    MCDRV_fsInit();

#if defined(MID)
    /* MID init */
    MID_Init_AR();
#endif

    /* Execute the fast control loop state machine at least once to perform
       initialization. */
    M1_fsStateMachine_AR();

    /* Evaluate the after-reset software flow. */
    FS_fsFlowCheck_AR();

    /* The background loop including in-rush relay waiting time. */
    while(TRUE)
    {
        /* Check whether the SysTick timer overflowed. */
        if(0U < APPPER_CpuTickMeasTmrOverflow())
        {
            /* Refresh watchdog. */
            FS_WDOG_REFRESH();

            /* Check whether the in-rush relay activation delay passed. */
            if(0U < ui16fsInrushRelayDly)
            {
                /* Decrement the in-rush relay activation delay timer. */
                ui16fsInrushRelayDly--;
            }
            else
            {
                /* Activate the in-rush relay. */
                M1_RELAY_ON();

                /* Signalize that the in-rush relay was enabled. */
                FMSTR_PipeMessage(FMSTR_MSG_INRUSH_ON);

                /* Start the slow and fast control loop timers. */
                MCDRV_fsStartTimers();

                /* Enable the interrupts. */
                __set_PRIMASK(ui32fsPrimaskReg);
            }
        }

        /* The background loop. */
        do
        {
            /* Perform background CPU registers test. */
            FS_fsCpuTest_BL();

            /* Perform run-time flash test. */
            FS_fsFlashTest_BL();

            /* Evaluate run-time clock test. */
            FS_fsClockTestCheck_BL();

            /* Stack overflow and underflow test. */
            FS_fsStackTest_BL();

            /* The ISR handling test (background to slow-loop rate check). */
            FS_fsISRTest_BL();

            /* FreeMASTER communication. */
            FMSTR_Poll();

            /* Check the required control mode. */
            switch(sCmdM1.eCtrlMode)
            {
            case kFOC_CtrlMode_VoltOL:
                /* Set the open-loop voltage control command. */
                sCmdM1.bValidCmd = FOC_SetCtrlVoltOL_BL(sCmdM1.eAppCmd,
                                                        sCmdM1.sUDQCmd,
                                                        sCmdM1.f16FrqElCmd,
                                                        sCmdM1.f16PosElCmd);
                break;

            case kFOC_CtrlMode_CurrOL:
                /* Set the open-loop current control command. */
                sCmdM1.bValidCmd = FOC_SetCtrlCurrOL_BL(sCmdM1.eAppCmd,
                                                        sCmdM1.sIDQCmd,
                                                        sCmdM1.f16FrqElCmd,
                                                        sCmdM1.f16PosElCmd);
                break;

           case kFOC_CtrlMode_ScalarOL:
                /* Set the open-loop scalar frequency control command. */
                sCmdM1.bValidCmd = FOC_SetCtrlScalarOL_BL(sCmdM1.eAppCmd,
                                                          sCmdM1.f16FrqElCmd);
                break;

            case kFOC_CtrlMode_CurrCL:
                /* Set the closed-loop current FOC control command. */
                sCmdM1.bValidCmd = FOC_SetCtrlCurrCL_BL(sCmdM1.eAppCmd,
                                                        sCmdM1.sIDQCmd);
                break;

            case kFOC_CtrlMode_MID:
#if defined(MID)
                /* Get the MID status and check whether the measurement is
                   currently active. */
                if(TRUE == MID_GetStatus_BL(&sUserMIDStatus))
                {
                    /* Check whether the measurement stop was requested. */
                    if(kFOC_Cmd_Stop == sCmdM1.eAppCmd)
                    {
                        /* Stop the identification. */
                        MID_Stop_BL();
                    }
                }
                else
                {
                    /* Check whether the measurement start was requested. */
                    if(kFOC_Cmd_Run == sCmdM1.eAppCmd)
                    {
                        /* Update motor parameters in the MID. All valid
                           electrical and mechanical parameters wont be
                           identified again. */
                        MID_SetKnownMotorParams_BL(&sUserMIDMotorParamsKnown);

                        /* Set the measurement configuration. */
                        if(kMID_ConfigSuccessful == MID_SetMeasConfig_BL(&sUserMIDMeasConfig))
                        {
                            /* Start/trigger the MID. */
                            ui32UserMIDStartResult = MID_Start_BL(eUserMIDMeasType);

                            /* Clear the start command. */
                            sCmdM1.eAppCmd = kFOC_Cmd_Ready;
                        }
                    }
                    /* Check whether the measurements were completed. */
                    else if(0UL < sUserMIDStatus.ui32FinishedMeas)
                    {
                        /* Get the measured motor parameters. */
                        MID_GetMotorParams_BL(&sUserMIDMotorParamsMeas);

                        /* Stop the measurement. */
                        sCmdM1.eAppCmd = kFOC_Cmd_Stop;
                    }
                    else{}
                }
#endif
                break;

            default: /* kFOC_CtrlMode_SpeedCL: */
                /* Set the closed-loop speed FOC control command. */
                sCmdM1.bValidCmd = FOC_SetCtrlSpeedCL_BL(sCmdM1.eAppCmd,
                                                         sCmdM1.f16SpdElCmd);
                break;
            }

            /* Evaluate the background loop software flow. */
            FS_fsFlowCheck_BL();
        }
        while(0U == __get_PRIMASK());
    }
}


/*!
 * @brief   ADC conversion complete ISR.
 *
 * @details The handler is executed with 100us period and processes the
 *          fast motor-control state-machine. The ADC measurement is triggered
 *          by PDB.
 *
 * @note    All the ADC0 and ADC1 results must be read during the ISR,
 *          otherwise the ISR will not reinitialize properly.
 *
 * @param   void
 *
 * @return  none
 */
void IRQHNDLR_fsADC0(void)
{
    /* PDB software trigger off. */
    PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0xF);
    PDB0->SC |= PDB_SC_LDOK_MASK;

    /* Start the CPU tick number counting. */
    APPPER_CpuTickMeasStart();

#if defined(MID)
    /* Check whether the MID control mode is selected */
    if(sCmdM1.eCtrlMode == kFOC_CtrlMode_MID)
    {
        /* Process the MID measurement. */
        MID_ProcessFast_FL();
    }
#endif
	
    /* Fast motor control state machine call. */
    M1_fsStateMachine_FL();

    /* Call the FreeMASTER recorder. */
    FMSTR_Recorder(0);

    /* Safety tests which cannot be interrupted and the watchdog refresh. */
    FS_fsUninterruptibleTests_FL();

    /* The ISR handling test (fast-loop to slow-loop rate check). */
    FS_fsISRTest_FL();

    /* Evaluate the fast control loop software flow. */
    FS_fsFlowCheck_FL();

    /* Stop CPU tick number counting and store the actual and maximum ticks. */
    APPPER_CpuTickMeasGetTicks(&g_sCpuTickMeas);

    /* Add instruction and data barrier for correct interrupt flag clearing. */
    __DSB();
    __ISB();
}

/*!
 * @brief   FTM2 reload ISR.
 *
 * @details The routine is called with 1ms period and processes the slow
 *          motor-control state-machine machine.
 *
 * @param   void
 *
 * @return  none
 */
void IRQHNDLR_fsFTM2(void)
{
    /* Clock test function. */
    FS_fsClockTestIsr_SL();

    /* The ISR handling test (fast-loop to slow-loop rate check). */
    FS_fsISRTest_SL();

    /* Slow motor-control state machine call. */
    M1_fsStateMachine_SL();

    /* Evaluate the slow control loop software flow. */
    FS_fsFlowCheck_SL();

    /* Clear the interrupt flag. */
    FTM2->SC &= ~(FTM_SC_TOF_MASK);

    /* Add instruction and data barrier for correct interrupt flag clearing. */
    __DSB();
    __ISB();
}

/*!
 * @brief   PDB error ISR
 *
 * @details Routine handles the PDB sequence error interrupt and re-initiates
 *          the PDB. It also enables PDB trigger upon the FTM0 initialization
 *          event.
 *
 * @param   void
 *
 * @return  none
 */
void IRQHNDLR_fsPDB(void)
{
    uint32_t ui32fsPDBStatus;

    /* Get PDB status for both channels. */
    ui32fsPDBStatus  = PDB0->CH[0U].S;
    ui32fsPDBStatus |= PDB0->CH[1U].S;

    /* Check for PDB error interrupt. */
    if(0UL < (ui32fsPDBStatus & PDB_S_ERR_MASK))
    {
        /* Disable the PDB. */
        PDB0->SC &= (~PDB_SC_PDBEN_MASK);

        /* Clear channel and sequence error flags */
        PDB0->CH[0U].S = 0UL;
        PDB0->CH[1U].S = 0UL;

        /* Enable PDB. */
        PDB0->SC |= PDB_SC_PDBEN_MASK;
    }
    /* Check for PDB delay interrupt. */
    else
    {
        /* PDB Trigger on (FTM0 initialization trigger) */
        PDB0->SC = (PDB0->SC & (~PDB_SC_TRGSEL_MASK)) | PDB_SC_TRGSEL(0x8U);
        PDB0->SC |= PDB_SC_LDOK_MASK;

        /* Clear PDB interrupt flag. */
        PDB0->SC &= (~PDB_SC_PDBIF_MASK);
    }

    /* Add instruction and data barrier for correct interrupt flag clearing. */
    __DSB();
    __ISB();
}

/*!
 * @brief   The MCU startup.
 *
 * @details This routine is called upon the MCU reset. It sets up the watchdog,
 *          calls all of the needed startup routines and then jumps to the main
 *          process.
 *
 * @param   void
 *
 * @return  none
 */
void IRQHNDLR_fsStart(void)
{
    /* Start the after-reset flow-check. */
    FS_FLOW_CHK_INIT(g_sfsFlowChkAR, FS_FLCK_AR_NODE_START);

#if FS_CFG_ENABLE_WATCHDOG
    /* Configure the watchdog to be enabled. */
    FSPER_fsWatchdogEnable();
#else
    /* Configure the watchdog to be disabled. */
    FSPER_fsWatchdogDisable();
#endif /* FS_CFG_ENABLE_WATCHDOG */

    /* Initialize RAM sections. */
    fsCommonStartup();

    /* Jump to the main function. */
    main();
}


/*!
 * @brief   Default IRQ handler.
 *
 * @param   void
 *
 * @return  none
 */
#if defined(__ARMCC_VERSION)
void DefaultISR(void)
#else
void IRQHNDLR_fsDefault(void)
#endif /* defined(__ARMCC_VERSION) */
{
    /* An unexpected IRQ was reached. */
    FS_fsErrorHandling(FS_ERROR_UNEXPECTED_IRQ, __get_IPSR());
}

/*!
 * @brief   Non-maskable (NMI) IRQ handler.
 *
 * @param   void
 *
 * @return  none
 */
void IRQHNDLR_fsNMI(void)
{
    /* An unexpected IRQ was reached. */
    FS_fsErrorHandling(FS_ERROR_UNEXPECTED_IRQ, __get_IPSR());
}

/*!
 * @brief   Hard-fault IRQ handler.
 *
 * @param   void
 *
 * @return  none
 */
void IRQHNDLR_fsHardFault(void)
{
    /* An unexpected IRQ was reached. */
    FS_fsErrorHandling(FS_ERROR_UNEXPECTED_IRQ, __get_IPSR());

    /* Disable all interrupts - the FS_fsErrorHandling() might be configured
       not to disable interrupts. */
    __asm("CPSID i");

    /* Lock the MCU in the endless loop. */
    while(TRUE)
    {
    }
}

