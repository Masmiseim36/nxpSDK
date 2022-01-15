/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_cm7_imxrt.h"

#if (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    #include "linker_config.h"
#endif

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    #pragma section =  ".safety_ram"
    #pragma section =  ".checksum"
    #pragma location = ".checksum"
#endif

/*******************************************************************************
* Variables
******************************************************************************/
#if (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    const uint32_t c_wdBackupAddress = (uint32_t)m_wd_test_backup;
    #define WATCHDOG_TEST_VARIABLES ((fs_wdog_test_t *) c_wdBackupAddress)

    const uint32_t programCounterTestFlag = (uint32_t)m_pc_test_flag;
    #define PC_TEST_FLAG ((uint32_t *) programCounterTestFlag)

    const uint32_t safetyErrorCodeAddress = (uint32_t)m_safety_error_code;
    #define SAFETY_ERROR_CODE ((uint32_t *) safetyErrorCodeAddress)

    const uint32_t c_backupAddress = (uint32_t)m_ram_test_backup;

    /* put values from extern symbols to const variables */
    const uint32_t c_stackTestFirstAddress = (uint32_t)m_stack_test_p_2;
    const uint32_t c_stackTestSecondAddress = (uint32_t)m_stack_test_p_3;

    /* The safety-related FLASH end border marker. */
    extern uint32_t Load$$ER_IROM1$$Limit;

    /* The safety-related FLASH CRC value. */
    fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
    {
        .ui16Start      = 0xA55AU,
        .ui32FlashStart = (uint32_t)m_flash_config_start,
        .ui32FlashEnd   = (uint32_t)&Load$$ER_IROM1$$Limit,
        .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
        .ui16End        = 0x5AA5U
    };
    
#else /* IAR + MCUXpresso */
    extern uint32_t m_wd_test_backup;   /* from Linker command file */
    const uint32_t c_wdBackupAddress = (uint32_t)&m_wd_test_backup;
    #define WATCHDOG_TEST_VARIABLES ((fs_wdog_test_t *) c_wdBackupAddress)

    extern uint32_t m_pc_test_flag;   /* from Linker command file */
    const uint32_t programCounterTestFlag = (uint32_t)&m_pc_test_flag;
    #define PC_TEST_FLAG ((uint32_t *) programCounterTestFlag)

    extern uint32_t m_safety_error_code; /* from Linker command file */
    const uint32_t safetyErrorCodeAddress = (uint32_t)&m_safety_error_code;
    #define SAFETY_ERROR_CODE ((uint32_t *) safetyErrorCodeAddress)

    extern uint32_t m_ram_test_backup; /* symbol from Linker command file */
    const uint32_t c_backupAddress = (uint32_t)&m_ram_test_backup;

    extern uint32_t m_stack_test_p_2;   /* symbol from Linker command file */
    extern uint32_t m_stack_test_p_3;   /* symbol from Linker command file */

    /* put values from extern symbols to const variables */
    const uint32_t c_stackTestFirstAddress = (uint32_t)&m_stack_test_p_2;
    const uint32_t c_stackTestSecondAddress = (uint32_t)&m_stack_test_p_3;

    #if defined(__IAR_SYSTEMS_ICC__) /* IAR */
        extern const uint32_t __checksum;  /* calculated by Linker */
        const uint32_t        c_checksumStart @ "checksum_start_mark";
        const uint32_t        c_checksumEnd   @ "checksum_end_mark";

    #else /* MCUXpresso */
        extern uint32_t stack_test_block_size; /* from Linker command file */
        #define STACK_TEST_BLOCK_SIZE (uint32_t)&stack_test_block_size
        extern uint32_t ram_test_backup_size; /* from Linker command file */
        #define RAM_TEST_BACKUP_SIZE (uint32_t)&ram_test_backup_size
        
        extern uint32_t m_flash_config_start; /* symbol from Linker command file */
        extern uint32_t m_safety_flash_end; /* symbol from Linker command file */

        /* The safety-related FLASH CRC value. */
        fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
        {
            .ui16Start      = 0xA55AU,
            .ui32FlashStart = (uint32_t) &m_flash_config_start,
            .ui32FlashEnd   = (uint32_t) &m_safety_flash_end,
            .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
            .ui16End        = 0x5AA5U
        };
    #endif
#endif

#if defined(DCP_BASE)
/* DCP channel enum */
extern fs_flash_dcp_channels_t g_dcpSafetyChannel;
#endif

/* Analog test limits and inputs */
fs_aio_test_t aio_Str;
const fs_aio_limits_t FS_ADC_Limits[FS_CFG_AIO_CHANNELS_CNT] = FS_CFG_AIO_CHANNELS_LIMITS_INIT;
const uint8_t FS_ADC_inputs[FS_CFG_AIO_CHANNELS_CNT]         = FS_CFG_AIO_CHANNELS_INIT;
#ifdef FSL_FEATURE_SOC_LPADC_COUNT
const uint8_t FS_ADC_channel_sides[FS_CFG_AIO_CHANNELS_CNT]  = FS_CFG_AIO_CHANNELS_SIDE_INIT;
#endif



/** This variable is used as flag for automatic "run" test during example development,
these test have a 3 return value:
1. All tests pass - wait until all after/reset and runtime test passed
2. Safety errror - return from SafetyErrorHandling() function
3. timeout - if case of that all tests are not finished until setted timeout */
#define CLOCK_PASSED              (1 << 0)
#define INVARIABLE_RUNTIME_PASSED (1 << 1)
#define VARIABLE_RUNTIME_PASSED   (1 << 2)
#define ALL_RUNTIME_PASSED        0x07

/***************************************************
 *                 BIT MASK
 * b.0  - Clock test
 * b.1  - Invariable memory RunTime
 * b.2  - Variable memory Runtime
 ***************************************************/
uint32_t runtime_test_finish_flag = 0; /* no test performed */

/* This function is only for development validation -
 * indicate that runtime test run at leaset one. */
static void test_end(void);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief   Safety watchdog test.
 *
 *          This function is used to test the Watchdog.
 *          Sets up LPTMR for the test.
 *          Calculates limit values for watchdog timeout.
 *          Performs the watchdog test.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   psSafetyWdTest - The pointer of the Safety Watchdog test structure
 * @param   peClockFreq     - The pointer of the clock name enumeration
 *
 * @return  None
 */
void SafetyWatchdogTest(safety_common_t *psSafetyCommon, wd_test_t *psSafetyWdTest)
{
#if WATCHDOG_ENABLED
    uint32_t counterLimitHigh;
    uint32_t counterLimitLow;
    uint32_t runTestCondition;
    uint32_t checkTestCondition;

    /* calculate counter limit values */
    psSafetyWdTest->wdTestTemp1     = (uint64_t)WATCHDOG_TIMEOUT_VALUE * (uint64_t)WD_REF_TIMER_CLOCK_FREQUENCY;
    psSafetyWdTest->wdTestExpected  = psSafetyWdTest->wdTestTemp1 / (uint32_t)WATCHDOG_CLOCK;
    psSafetyWdTest->wdTestTolerance = (psSafetyWdTest->wdTestExpected * (uint32_t)WD_TEST_TOLERANCE) / (uint32_t)100;
    psSafetyWdTest->wdTestLimitHigh = psSafetyWdTest->wdTestExpected + psSafetyWdTest->wdTestTolerance;
    psSafetyWdTest->wdTestLimitLow  = psSafetyWdTest->wdTestExpected - psSafetyWdTest->wdTestTolerance;
    counterLimitHigh                = psSafetyWdTest->wdTestLimitHigh;
    counterLimitLow                 = psSafetyWdTest->wdTestLimitLow;

    runTestCondition   = WD_RUN_TEST_CONDITION;
    checkTestCondition = WD_CHECK_TEST_CONDITION;

    /* Safety library structure initialization */
    WATCHDOG_TEST_VARIABLES->RefTimerBase         = (uint32_t)WDOG_REF_TIMER_BASE;
    WATCHDOG_TEST_VARIABLES->WdogBase             = (uint32_t)USED_WDOG;
    WATCHDOG_TEST_VARIABLES->pResetDetectRegister = (uint32_t)(RESET_DETECT_REGISTER);
    WATCHDOG_TEST_VARIABLES->ResetDetectMask      = (uint32_t)RESET_DETECT_MASK;

    /* Init timer */
    GPT1_Init(WD_GPT1_SRC, WD_GPT1_CMP, WD_GPT1_DIV);

    if (*(RESET_DETECT_REGISTER)&runTestCondition) /* If non WD reset */
    {
        /* Clear tested flags */
        SRC->SRSR = runTestCondition;

        FS_WDOG_Setup_IMX_GPT(WATCHDOG_TEST_VARIABLES, REFRESH_INDEX);
    }

    if (*(RESET_DETECT_REGISTER)&checkTestCondition) /* If WD reset */
    {
        psSafetyCommon->WDOG_test_result =
            FS_WDOG_Check(counterLimitHigh, counterLimitLow, WATCHDOG_RESETS_LIMIT, ENDLESS_LOOP_ENABLE,
                          WATCHDOG_TEST_VARIABLES, CLEAR_FLAG, REG_WIDE);
        if (psSafetyCommon->WDOG_test_result != FS_PASS) /* WDOG can return more error messages */
        {
            psSafetyCommon->safetyErrors |= WDOG_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }
    }

#endif /* WATCHDOG_ENABLED */

    psSafetyWdTest->watchdogResets       = WATCHDOG_TEST_VARIABLES->resets;
    psSafetyWdTest->watchdogTimeoutCheck = WATCHDOG_TEST_VARIABLES->counter;
    psSafetyWdTest->watchdogRefreshRatio = 0;
}

/*!
 * @brief Safety watchdog refresh.
 *
 * This function is used for adjusting of the watchdog refresh when using in fast interrupt.
 *
 * @param psSafetyWdTest    The pointer of the Safety Watchdog test structure
 *
 * @return None
 */
void SafetyWatchdogRuntimeRefresh(wd_test_t *psSafetyWdTest)
{
    psSafetyWdTest->watchdogRefreshRatio++;
    if (psSafetyWdTest->watchdogRefreshRatio == WATCHDOG_REFRESH_RATIO)
    {
#if WATCHDOG_ENABLED
        Watchdog_refresh; /* refreshing the watchdog */
#endif
        psSafetyWdTest->watchdogRefreshRatio = 0;
    }
}

/*!
 * @brief   Initialization of Safety clock test.
 *
 *          Complete Initialization of the clock test.
 *          Function calculates limit values.
 *          Cals clock test init function from the IEC60730B library.
 *
 * @param   psSafetyCommon    - The pointer of the Common Safety structure
 * @param   psSafetyClockTest - The pointer of the Safety Clock test structure
 * @param   peClockFreq       - The pointer of the clock name enumeration
 *
 * @return  None
 */
void SafetyClockTestInit(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest)
{
    psSafetyCommon->refClkFreq           = CLOCK_GPT1_FREQ;
    psSafetyClockTest->clockTestExpected = psSafetyCommon->refClkFreq / (uint32_t)ISR_FREQUENCY;
    psSafetyClockTest->clockTestTolerance =
        (psSafetyClockTest->clockTestExpected * (uint32_t)CLOCK_TEST_TOLERANCE) / (uint32_t)100;
    psSafetyClockTest->clockTestLimitHigh =
        psSafetyClockTest->clockTestExpected + psSafetyClockTest->clockTestTolerance;
    psSafetyClockTest->clockTestLimitLow = psSafetyClockTest->clockTestExpected - psSafetyClockTest->clockTestTolerance;
    psSafetyClockTest->clockTestStart    = 0; /* clock test result will be processed after the first interrupt occurs */

    FS_CLK_Init((uint32_t *)&psSafetyClockTest->clockTestContext);

    /* Initialization of timer */
    GPT1_Init(CLOCK_GPT1_SRC, CLOCK_GPT1_CMP, CLOCK_GPT1_DIV);
}

/*!
 * @brief   Clock test function, called from interrupt.
 *
 *          This function calls clock test function from the IEC60730B library and enable the test evaluation.
 *          It must be called in the Systick interrupt to catch the value of LPTMR counter.
 *
 * @param   psSafetyClockTest - The pointer of the Safety Clock test structure
 *
 * @return  None
 */
void SafetyClockTestIsr(clock_test_t *psSafetyClockTest)
{
    FS_CLK_GPT((fs_gpt_t *)GPT1, (uint32_t *)&psSafetyClockTest->clockTestContext);

    if (psSafetyClockTest->clockTestStart != 2)
    {
        psSafetyClockTest->clockTestStart += 1; /* to prevent checking of result before execution */
    }
}

/*!
 * @brief   Clock test check function.
 *
 *          This function can be called from any place of application.
 *          It calls the IEC60730B_CM4_CM7_CLK_Check function from the IEC60730 library
 *          In case of incorrect clock test result, it updates the safetyErrors variable accordingly.
 *          A node of program flow check is placed here.
 *
 * @param   psSafetyCommon          - The pointer of the Common Safety structure
 * @param   psSafetyClockTest       - The pointer of the Safety Clock test structure
 * @param   psSafetyProgramFlowTest - The pointer of the Program flow test structure
 *
 * @return  None
 */
void SafetyClockTestCheck(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest)
{
    if (psSafetyClockTest->clockTestStart == 2) /* condition is valid after the first Systick interrupt */
    {
        psSafetyCommon->CLOCK_test_result =
            FS_CLK_Check(psSafetyClockTest->clockTestContext, psSafetyClockTest->clockTestLimitLow,
                         psSafetyClockTest->clockTestLimitHigh);
        if (psSafetyCommon->CLOCK_test_result == FS_FAIL_CLK)
        {
            psSafetyCommon->safetyErrors |= CLOCK_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }
        runtime_test_finish_flag |= CLOCK_PASSED; /*Variable for development run test, useless for final application */
    }
}

#if defined(DCP_BASE)
/*!
 * @brief   Initialization of DCP packet to zeros.
 *
 * @param   psFlashDCPState - struct which contains packet per channel.
 *
 * @return  None
 */
void InitPackets(fs_flash_dcp_state_t *psFlashDCPState)
{
    for (uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->CH0Packet[size] = 0;
    }
    for (uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->CH1Packet[size] = 0;
    }
    for (uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->CH2Packet[size] = 0;
    }
    for (uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->CH3Packet[size] = 0;
    }
}
#endif

/*!
 * @brief   Initialization of Safety Flash test.
 *
 *          Enable clock for HW CRC module.
 *          Inits the Flash test variables
 *
 * @param   psFlashCrc    - The pointer of the Flash CRC structure.
 * @param   psFlashConfig - The pointer of the Safety Flash test configuration structure.
 *
 * @return  None
 */
void SafetyFlashTestInit(flash_runtime_test_parameters_t *psFlashCrc,
                         flash_configuration_parameters_t *psFlashConfig,
                         fs_flash_dcp_state_t *psFlashDCPState)
{
#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    psFlashConfig->startAddress = (uint32_t)&c_checksumStart;
    psFlashConfig->endAddress =
        4 + (uint32_t)&c_checksumEnd; /*We must test also last adress, due to this reason +4 in IAR*/
    psFlashConfig->checksum = __checksum;

#else /* KEIL + MCUXpresso */
    psFlashConfig->startAddress = c_sfsCRC.ui32FlashStart;
    psFlashConfig->endAddress   = c_sfsCRC.ui32FlashEnd;
    psFlashConfig->checksum     = c_sfsCRC.ui32CRC;
#endif

    psFlashConfig->size               = psFlashConfig->endAddress - psFlashConfig->startAddress;
    psFlashConfig->startConditionSeed = (uint32_t)FLASH_TEST_CONDITION_SEED;
    psFlashConfig->blockSize          = FLASH_TEST_BLOCK_SIZE;

    psFlashCrc->actualAddress = psFlashConfig->startAddress;       /* start address */
    psFlashCrc->partCrc       = psFlashConfig->startConditionSeed; /* initial seed */
    psFlashCrc->blockSize =
        (psFlashConfig->size < psFlashConfig->blockSize) ? psFlashConfig->size : psFlashConfig->blockSize;

#if defined(DCP_BASE)
    /* Init DCP channels states and results */
    psFlashDCPState->CH0State  = FS_FLASH_DCP_AVAILABLE;
    psFlashDCPState->CH0Result = 0;
    psFlashDCPState->CH1State  = FS_FLASH_DCP_AVAILABLE;
    psFlashDCPState->CH1Result = 0;
    psFlashDCPState->CH2State  = FS_FLASH_DCP_AVAILABLE;
    psFlashDCPState->CH2Result = 0;
    psFlashDCPState->CH3State  = FS_FLASH_DCP_AVAILABLE;
    psFlashDCPState->CH3Result = 0;
    InitPackets(psFlashDCPState);
    g_dcpSafetyChannel = DCP_CH3;
#endif
}

#if defined(DCP_BASE)
/*!
 * @brief   After-reset Flash test.
 *
 *          This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested at once.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   psFlashConfig  - The pointer of the Safety Flash test configuration structure.
 *
 * @return  None
 */
void SafetyFlashAfterResetTest_DCP(safety_common_t *psSafetyCommon,
                                   flash_configuration_parameters_t *psFlashConfig,
                                   fs_flash_dcp_state_t *psFlashDCPState)
{
    /* CRC calculation of SAFETY_FLASH_BLOCK (channel should be available after reset) */
    do
    {
        FS_CM4_CM7_FLASH_HW32_DCP(psFlashConfig->startAddress, psFlashConfig->size, (uint32_t)FLASH_USED_DCP,
                                  psFlashConfig->startConditionSeed, g_dcpSafetyChannel, psFlashDCPState,
                                  FLASH_DCP_TAG);

        /* Check error. */
        if (psFlashDCPState->CH3State == FS_FAIL_FLASH_DCP)
        {
            psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }
    } while (psFlashDCPState->CH3State == FS_FLASH_DCP_BUSY);

    /* Store the result */
    psSafetyCommon->FLASH_test_result = psFlashDCPState->CH3Result;

    /* Check if result equals precomputed CRC value */
    if (psSafetyCommon->FLASH_test_result != psFlashConfig->checksum)
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}
#else
/*!
 * @brief   After-reset Flash test.
 *
 *          This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested at once.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   psFlashConfig  - The pointer of the Safety Flash test configuration structure.
 *
 * @return  None
 */
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, flash_configuration_parameters_t *psFlashConfig)
{
    psSafetyCommon->FLASH_test_result =
        FS_CM4_CM7_FLASH_SW32(psFlashConfig->startAddress, psFlashConfig->size, 0, psFlashConfig->startConditionSeed);

    if (psSafetyCommon->FLASH_test_result != psFlashConfig->checksum)
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}
#endif

#if defined(DCP_BASE)
/*!
 * @brief   Runtime Flash test.
 *
 *          This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested in sequence.
 *          Calls SafetyFlashTestHandling function.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          A node of program flow check is placed here.
 *
 * @param   psSafetyCommon          - The pointer of the Common Safety structure
 * @param   psFlashCrc              - The pointer of the Flash CRC structure.
 * @param   psFlashConfig           - The pointer of the Safety Flash test configuration structure.
 * @param   psSafetyProgramFlowTest - The pointer of the Program flow test structure
 *
 * @return  None
 */
void SafetyFlashRuntimeTest_DCP(safety_common_t *psSafetyCommon,
                                flash_runtime_test_parameters_t *psFlashCrc,
                                flash_configuration_parameters_t *psFlashConfig,
                                fs_flash_dcp_state_t *psFlashDCPState)
{
    /* Start CRC calculation for a given block of Flash memory */
    FS_CM4_CM7_FLASH_HW32_DCP(psFlashCrc->actualAddress, psFlashCrc->blockSize, (uint32_t)FLASH_USED_DCP,
                              psFlashCrc->partCrc, g_dcpSafetyChannel, psFlashDCPState, FLASH_DCP_TAG);

    /* Check error. */
    if (psFlashDCPState->CH3State == FS_FAIL_FLASH_DCP)
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* Check if calculation finished */
    else if (psFlashDCPState->CH3State == FS_FLASH_DCP_AVAILABLE)
    {
        /* Store partial result */
        psFlashCrc->partCrc = psFlashDCPState->CH3Result;

        /* Handle result */
        if (SafetyFlashTestHandling(psFlashCrc, psFlashConfig) == FS_FLASH_FAIL)
        {
            psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }
    }
}
#else
/*!
 * @brief   Runtime Flash test.
 *
 *          This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested in sequence.
 *          Calls SafetyFlashTestHandling function.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          A node of program flow check is placed here.
 *
 * @param   psSafetyCommon          - The pointer of the Common Safety structure
 * @param   psFlashCrc              - The pointer of the Flash CRC structure.
 * @param   psFlashConfig           - The pointer of the Safety Flash test configuration structure.
 * @param   psSafetyProgramFlowTest - The pointer of the Program flow test structure
 *
 * @return  None
 */
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon,
                            flash_runtime_test_parameters_t *psFlashCrc,
                            flash_configuration_parameters_t *psFlashConfig)
{
    psFlashCrc->partCrc =
        FS_CM4_CM7_FLASH_SW32(psFlashCrc->actualAddress, psFlashCrc->blockSize, 0, psFlashCrc->partCrc);

    if (FS_FLASH_FAIL == SafetyFlashTestHandling(psFlashCrc, psFlashConfig))
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}
#endif
/*!
 * @brief   Handling of flash test when used in runtime.
 *
 *          The function updates the flash test variables, when flash is tested in sequence.
 *
 * @param   __checksum    - Constant that is calculated by Linker and stored in Flash.
 * @param   psFlashCrc    - The pointer of the Flash CRC structure.
 * @param   psFlashConfig - The pointer of the Safety Flash test configuration structure.
 *
 * @return  Result of the flash test: IEC60730B_ST_FLASH_FAIL or IEC60730B_ST_FLASH_PASS
 */
uint32_t SafetyFlashTestHandling(flash_runtime_test_parameters_t *psFlashCrc,
                                 flash_configuration_parameters_t *psFlashConfig)
{
    uint32_t ui32Status;
    ui32Status = FS_FLASH_PROGRESS;

    psFlashCrc->actualAddress += psFlashCrc->blockSize; /* set the actual address for testing */

    if (psFlashCrc->actualAddress == psFlashConfig->endAddress)
    {
        if (psFlashCrc->partCrc == psFlashConfig->checksum) /* checksum must be same as calculated in post-build */
        {
            ui32Status = FS_FLASH_PASS;

            runtime_test_finish_flag |=
                INVARIABLE_RUNTIME_PASSED; /*Variable for development run test, useless for final application */
        }
        else
        {
            ui32Status = FS_FLASH_FAIL;
        }

        psFlashCrc->partCrc       = psFlashConfig->startConditionSeed; /* set start seed as input for CRC calculation */
        psFlashCrc->actualAddress = psFlashConfig->startAddress;       /* set start address */
        psFlashCrc->blockSize     = psFlashConfig->blockSize;          /* size of block for CRC testing */
    }

    if ((psFlashCrc->actualAddress + psFlashCrc->blockSize) >= psFlashConfig->endAddress)
    {
        psFlashCrc->blockSize = psFlashConfig->endAddress - psFlashCrc->actualAddress;
    }

    return (ui32Status);
}

/*!
 * @brief   Initialization of Safety RAM test.
 *
 *          Inits the RAM test variables
 *
 * @param   psSafetyRamTest - The pointer of the RAM test structure.
 * @param   pSafetyRamStart - The pointer of the RAM test start address.
 * @param   pSafetyRamEnd   - The pointer of the RAM test end address.
 *
 * @return  None
 */
void SafetyRamTestInit(ram_test_t *psSafetyRamTest, uint32_t *pSafetyRamStart, uint32_t *pSafetyRamEnd)
{
    psSafetyRamTest->ramTestStartAddress = (uint32_t)pSafetyRamStart;
    psSafetyRamTest->ramTestEndAddress   = (uint32_t)pSafetyRamEnd;
    psSafetyRamTest->defaultBlockSize    = RAM_TEST_BACKUP_SIZE;
    psSafetyRamTest->blockSize           = RAM_TEST_BLOCK_SIZE;
    psSafetyRamTest->actualAddress       = psSafetyRamTest->ramTestStartAddress;

#if (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
    psSafetyRamTest->backupAddress = (uint32_t)m_ram_test_backup;
#else /* IAR + MCUXpresso */
    psSafetyRamTest->backupAddress          = (uint32_t)&m_ram_test_backup;
#endif
}

/*!
 * @brief   After-reset RAM test.
 *
 *          This function calls the RAM test function from IEC60730 library.
 *          Safety-related part of the RAM is tested at once.
 *          In case of incorrect RAM test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon  - The pointer of the Common Safety structure
 * @param   psSafetyRamTest - The pointer of the Safety RAM test structure.
 *
 * @return  None
 */
void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->RAM_test_result = FS_CM4_CM7_RAM_AfterReset(
        psSafetyRamTest->ramTestStartAddress, psSafetyRamTest->ramTestEndAddress, psSafetyRamTest->defaultBlockSize,
        psSafetyRamTest->backupAddress, FS_CM4_CM7_RAM_SegmentMarchC);
    if (psSafetyCommon->RAM_test_result == FS_FAIL_RAM)
    {
        psSafetyCommon->safetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Runtime RAM test.
 *
 *          This function calls the RAM test function from IEC60730 library.
 *          Safety-related part of the RAM is tested in sequence.
 *          Calls SafetyFlashTestHandling function.
 *          In case of incorrect RAM test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon  - The pointer of the Common Safety structure
 * @param   psSafetyRamTest - The pointer of the Safety RAM test structure.
 *
 * @return  None
 */
void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->RAM_test_result =
        FS_CM4_CM7_RAM_Runtime(psSafetyRamTest->ramTestStartAddress, psSafetyRamTest->ramTestEndAddress,
                               (uint32_t *)&psSafetyRamTest->actualAddress, psSafetyRamTest->blockSize,
                               psSafetyRamTest->backupAddress, FS_CM4_CM7_RAM_SegmentMarchX);

    if (psSafetyCommon->RAM_test_result == FS_FAIL_RAM)
    {
        psSafetyCommon->safetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* Test if whole RAM was tested, only for example validation */
    if (psSafetyRamTest->ramTestStartAddress == psSafetyRamTest->actualAddress)
        runtime_test_finish_flag |=
            VARIABLE_RUNTIME_PASSED; /*Variable for development run test, useless for final application */
}

/*!
 * @brief   Program counter test.
 *
 *          This function uses two addresses: first is defined in linker file (IEC60730_B_CM4_CM7_pc_object.o),
 *          second address comes as function argument (must be RAM address).
 *          Both addresses must be defined by the developer and suitable to test all of the possible PC bits.
 *          This test cannot be interrupted.
 *          In case of incorrect PC test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   pattern        - RAM address, it can vary with multiple function calls
 *
 * @return  None
 */
void SafetyPcTest(safety_common_t *psSafetyCommon, uint32_t pattern)
{
    psSafetyCommon->PC_test_result = FS_CM4_CM7_PC_Test(pattern, FS_PC_Object, (uint32_t *)PC_TEST_FLAG);
    if (psSafetyCommon->PC_test_result == FS_FAIL_PC)
    {
        psSafetyCommon->safetyErrors |= PC_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   After-reset CPU registers test.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          All the registers are tested at once.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon)
{
    /* stacked CPU registers */
    psSafetyCommon->CPU_reg_test_result = FS_CM4_CM7_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* non-stacked CPU registers */
    psSafetyCommon->CPU_non_stacked_test_result = FS_CM4_CM7_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* CONTROL */
#if __FPU_PRESENT
    psSafetyCommon->CPU_control_test_result = FS_CM4_CM7_CPU_ControlFpu();
#else
    psSafetyCommon->CPU_control_test_result = FS_CM4_CM7_CPU_Control();
#endif

    if (psSafetyCommon->CPU_control_test_result == FS_FAIL_CPU_CONTROL)
    {
        psSafetyCommon->safetyErrors |= CPU_CONTROL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* SP main */
    FS_CM4_CM7_CPU_SPmain();

    /* SP process */
    FS_CM4_CM7_CPU_SPprocess();

    /* PRIMASK */
    psSafetyCommon->CPU_primask_test_result = FS_CM4_CM7_CPU_Primask();
    if (psSafetyCommon->CPU_primask_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* Special CPU registers  */
    psSafetyCommon->CPU_special_test_result = FS_CM4_CM7_CPU_Special();
    if (psSafetyCommon->CPU_special_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* group 1 of FPU registers */
#if __FPU_PRESENT
    psSafetyCommon->CPU_float_test_1_result = FS_CM4_CM7_CPU_Float1();
    if (psSafetyCommon->CPU_float_test_1_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* group 2 of FPU registers */
    psSafetyCommon->CPU_float_test_2_result = FS_CM4_CM7_CPU_Float2();
    if (psSafetyCommon->CPU_float_test_2_result == FS_FAIL_CPU_FLOAT_2)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
 * @brief   Uninterruptible test of CPU registers.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          The function must be called from an interrupt with highest priority.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuIsrTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->CPU_primask_test_result = FS_CM4_CM7_CPU_Primask();
    if (psSafetyCommon->CPU_primask_test_result == FS_FAIL_CPU_PRIMASK)
    {
        psSafetyCommon->safetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    FS_CM4_CM7_CPU_SPmain();

    psSafetyCommon->CPU_special_test_result = FS_CM4_CM7_CPU_Special();
    if (psSafetyCommon->CPU_special_test_result == FS_FAIL_CPU_SPECIAL)
    {
        psSafetyCommon->safetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Interruptible test of CPU registers.
 *
 *          This function calls the CPU test functions from IEC60730 library.
 *          The function can be called from the background loop.
 *          In case of incorrect flash test result, it updates the safetyErrors variable accordingly.
 *          See IEC60730 library documentation for CPU errors handling !
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->CPU_reg_test_result = FS_CM4_CM7_CPU_Register();
    if (psSafetyCommon->CPU_reg_test_result == FS_FAIL_CPU_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_non_stacked_test_result = FS_CM4_CM7_CPU_NonStackedRegister();
    if (psSafetyCommon->CPU_non_stacked_test_result == FS_FAIL_CPU_NONSTACKED_REGISTER)
    {
        psSafetyCommon->safetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#if __FPU_PRESENT
    psSafetyCommon->CPU_float_test_1_result = FS_CM4_CM7_CPU_Float1();
    if (psSafetyCommon->CPU_float_test_1_result == FS_FAIL_CPU_FLOAT_1)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->CPU_float_test_2_result = FS_CM4_CM7_CPU_Float2();
    if (psSafetyCommon->CPU_float_test_2_result == FS_FAIL_CPU_FLOAT_2)
    {
        psSafetyCommon->safetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
 * @brief   Initialization of Stack test.
 *
 *          This function calls IEC60730B_CM4_CM7_Stack_Init function from IEC60730 library.
 *
 * @param   void - macros from header files define the parameters
 *
 * @return  None
 */
void SafetyStackTestInit(void)
{
    FS_CM4_CM7_STACK_Init(STACK_TEST_PATTERN, c_stackTestFirstAddress, c_stackTestSecondAddress, STACK_TEST_BLOCK_SIZE);
}

/*!
 * @brief   Stack test.
 *
 *          This function calls the STACK test function from IEC60730 library
 *          Stack is tested for underflow and overflow condition.
 *          In case of incorrect Stack test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyStackTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->STACK_test_result = FS_CM4_CM7_STACK_Test(STACK_TEST_PATTERN, c_stackTestFirstAddress,
                                                              c_stackTestSecondAddress, STACK_TEST_BLOCK_SIZE);
    if (psSafetyCommon->STACK_test_result == FS_FAIL_STACK)
    {
        psSafetyCommon->safetyErrors |= STACK_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Digital Input/Output test.
 *
 *          This function calls output test functions from IEC60730 library
 *          In case of incorrect test result, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   *pTestedPin    - The pointer to the DIO test item structure (pin definition)
 *
 * @return  None
 */
void SafetyDigitalOutputTest(safety_common_t *psSafetyCommon, fs_dio_test_imx_t *pTestedPin)
{
    /* Setup port for testing */
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, PIN_DIRECTION_OUT);

    /* Output test */
    psSafetyCommon->DIO_output_test_result = FS_DIO_Output_IMXRT(pTestedPin, DIO_WAIT_CYCLE);

    /* Error check */
    if (psSafetyCommon->DIO_output_test_result != FS_PASS)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* Restore pin settings */
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, pTestedPin->pinDir);
}

/*!
 * @brief   Digital Input/Output Short to Adjacent pins test
 *
 *          This function calls digital io short test SET and GET functions from IEC60730 library
 *          In case of incorrect test conditions, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   *pTestedPin    - The pointer to the DIO test item structure (pin definition)
 * @param   *pAdjPin       - The pointer of the DIO test item structure for Adjacent pin (pin definition)
 * @param   PinValue       - logical 1 or logical 0 will be set on the tested pin
 *
 * @return  None
 */
void SafetyDigitalInputOutput_ShortAdjTest(safety_common_t *psSafetyCommon,
                                           fs_dio_test_imx_t *pTestedPin,
                                           fs_dio_test_imx_t *pAdjPin,
                                           uint32_t PinValue)

{
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, PIN_DIRECTION_IN);
    PortSetup(pAdjPin->gpio, pAdjPin->pinNum, PIN_DIRECTION_OUT);

    psSafetyCommon->DIO_short_test_result = FS_DIO_ShortToAdjSet_IMXRT(pTestedPin, pAdjPin, PinValue, DIO_BACKUP);
    if ((psSafetyCommon->DIO_short_test_result) != FS_PASS)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* if needed, place some delay loop here */
    for (int32_t i = 0; i < 10000U; i++)
    {
        __asm("nop");
    }

    psSafetyCommon->DIO_input_test_result = FS_DIO_InputExt_IMXRT(pTestedPin, pAdjPin, PinValue, DIO_BACKUP);
    if ((psSafetyCommon->DIO_input_test_result) != FS_PASS)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, pTestedPin->pinDir);
    PortSetup(pAdjPin->gpio, pAdjPin->pinNum, pAdjPin->pinDir);
}

/*!
 * @brief   Digital Input/Output Short to Supply test.
 *
 *          This function calls digital io short test SET and GET functions from IEC60730 library
 *          In case of incorrect test conditions, it updates the safetyErrors variable accordingly.
 *
 * @param   psSafetyCommon   - The pointer of the Common Safety structure
 * @param   *pTestedPin      - The pointer to the DIO test item structure (pin definition)
 * @param   polarity         - macro DIO_SHORT_TO_VDD_TEST or DIO_SHORT_TO_GND_TEST
 *
 * @return  None
 */
void SafetyDigitalInputOutput_ShortSupplyTest(safety_common_t *psSafetyCommon,
                                              fs_dio_test_imx_t *pTestedPin,
                                              uint8_t polarity)
{
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, PIN_DIRECTION_IN);

    psSafetyCommon->DIO_short_test_result = FS_DIO_ShortToSupplySet_IMXRT(pTestedPin, polarity, DIO_BACKUP);
    if ((psSafetyCommon->DIO_short_test_result) != FS_PASS)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* if needed, place some delay loop here */
    for (int32_t i = 0; i < 10000U; i++)
    {
        __asm("nop");
    }

    psSafetyCommon->DIO_input_test_result = FS_DIO_InputExt_IMXRT(pTestedPin, pTestedPin, polarity, DIO_BACKUP);
    if ((psSafetyCommon->DIO_input_test_result) != FS_PASS)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, pTestedPin->pinDir);
}

/*!
 * @brief   Initialization of ADC test.
 *
 *          This function calls InputInit and InputTrigger functions from IEC60730 library
 *
 * @param   void - macros from header files define the parameters
 *
 * @return  None
 */
void SafetyAnalogTestInitialization(void)
{
#ifdef FSL_FEATURE_SOC_LPADC_COUNT
    FS_AIO_InputInit_A1(&aio_Str, (fs_aio_limits_t *)FS_ADC_Limits, (uint8_t *)FS_ADC_inputs, FS_CFG_AIO_CHANNELS_CNT, ADC_COMMAND_BUFFER, (uint8_t *)FS_ADC_channel_sides, (uint8_t) TRIGGER_EVENT);
#else
    FS_AIO_InputInit_A2346(&aio_Str, (fs_aio_limits_t *)FS_ADC_Limits, (uint8_t *)FS_ADC_inputs, FS_CFG_AIO_CHANNELS_CNT);
#endif
    FS_AIO_InputTrigger(&aio_Str);
}

/*!
 * @brief   ADC test.
 *
 *          This function calls functions from IEC60730 library to perform ADC test.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
#if ADC_TEST_ENABLED
void SafetyAnalogTest(safety_common_t *psSafetyCommon)
{
#ifdef FSL_FEATURE_SOC_LPADC_COUNT
    psSafetyCommon->AIO_test_result = FS_AIO_InputCheck_A1(&aio_Str, (fs_aio_a1_t *)TESTED_ADC);
#else
    psSafetyCommon->AIO_test_result = FS_AIO_InputCheck_A6(&aio_Str, (fs_aio_a6_t *)TESTED_ADC);
#endif
    switch (psSafetyCommon->AIO_test_result)
    {
        case FS_AIO_START: /* state START means that everything is ready to trigger the conversion */
#ifdef FSL_FEATURE_SOC_LPADC_COUNT
            FS_AIO_InputSet_A1(&aio_Str, (fs_aio_a1_t *)TESTED_ADC);
#else
            FS_AIO_InputSet_A6(&aio_Str, (fs_aio_a6_t *)TESTED_ADC);
#endif
            break;
        case FS_FAIL_AIO:
            psSafetyCommon->safetyErrors |= AIO_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
            break;
        case FS_AIO_INIT:
            FS_AIO_InputTrigger(&aio_Str);
            break;
        case FS_PASS: /* successfull execution of test, call the trigger function again */
            FS_AIO_InputTrigger(&aio_Str);
            break;
        default:
            __asm("NOP");
            break;
    }
}
#endif
/*!
 * @brief   Handling of the safety functions that must be called in interrupt routine.
 *
 *          This function switches between safety functions that are called in interrupt
 *
 * @param   psSafetyCommon       - The pointer of the Common Safety structure
 * @param   psSafetyRamTest      - The pointer of the Safety RAM test structure.
 * @param   psSafetyRamStackTest - The pointer of the Safety RAM test structure for Stack area.
 *
 * @return  None
 */
void SafetyIsrFunction(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest, ram_test_t *psSafetyRamStackTest)
{
    switch (psSafetyCommon->fastIsrSafetySwitch)
    {
        case 0: /* CPU registers test that cannot be interrupted */
            SafetyCpuIsrTest(psSafetyCommon);
            break;
        case 1: /* Program counter test */
#if PC_TEST_ENABLED
            SafetyPcTest(psSafetyCommon, PC_TEST_PATTERN);
#endif
            break;
        case 2: /* RAM March test for safety related RAM space */
            SafetyRamRuntimeTest(psSafetyCommon, psSafetyRamTest);
            break;
        case 3: /* RAM March test for memory occupied by the Stack */
            SafetyRamRuntimeTest(psSafetyCommon, psSafetyRamStackTest);
            break;
        default:
            __asm("nop");
            break;
    }

    psSafetyCommon->fastIsrSafetySwitch++;
    if (psSafetyCommon->fastIsrSafetySwitch == 4)
        psSafetyCommon->fastIsrSafetySwitch = 0;
}

/* Function for example validation during  development */
void development_test_terminate(void)
{
    static uint8_t runtime_counter = 0;
    if (runtime_test_finish_flag == ALL_RUNTIME_PASSED)
    {
        runtime_test_finish_flag = 0;
        runtime_counter++;
        if (runtime_counter == 10)
        {
            test_end(); /* Mark that runtime test run */
        }
    }
}
/* This function is only for example validation during development */
static void test_end(void)
{
    __asm("nop");
}

/*!
 * @brief   Handling with a safety error.
 *
 *          This function stores the code of recognized safety error into the dedicated RAM memory that is deleted only
 * after POR. If SAFETY_ERROR_ACTION macro is defined, interrupts are disabled and function waits for watchdog reset.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 *
 * @return  None
 */
void SafetyErrorHandling(safety_common_t *psSafetyCommon)
{
    *SAFETY_ERROR_CODE = psSafetyCommon->safetyErrors;
    test_end(); /* Only for example validation. Indicate that some error occured and test can be stoped */
#if SAFETY_ERROR_ACTION
    __asm("CPSID i"); /* disable interrupts */
    while (1)
        ;
#endif
}
