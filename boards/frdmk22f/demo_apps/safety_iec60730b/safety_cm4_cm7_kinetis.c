/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_cm4_cm7_kinetis.h"

#if (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
#include "linker_config.h"
#endif

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
#pragma section  = ".safety_ram"
#pragma section  = ".checksum"
#pragma location = ".checksum"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef PCC_CRC_INDEX
#define PCC_CRC_INDEX PCC_CRC0_INDEX
#endif

#ifndef PCC
#define PCC PCC0
#endif

#ifndef CRC_BASE
#define CRC_BASE                                                                                                      \
    0x0u /*Function API for HW and SW CRC is the same, due to this reason, we must define this symbol, even though it \
            is not used  */
#endif

/*******************************************************************************
* Variables
******************************************************************************/	
#if (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    extern uint32_t crcPostbuild32; /* defined in main.c */
    const uint32_t c_wdBackupAddress = (uint32_t)m_wd_test_backup;
    #define WATCHDOG_TEST_VARIABLES ((fs_wdog_test_t *) c_wdBackupAddress)

    const uint32_t c_programCounterTestFlag = (uint32_t)m_pc_test_flag;
    #define PC_TEST_FLAG ((uint32_t *) c_programCounterTestFlag)

    const uint32_t c_safetyErrorCodeAddress = (uint32_t)m_safety_error_code;
    #define SAFETY_ERROR_CODE ((uint32_t *) c_safetyErrorCodeAddress)

    const uint32_t c_backupAddress = (uint32_t)m_ram_test_backup;

    /* put values from extern symbols to const variables */
    const uint32_t c_stackTestFirstAddress = (uint32_t)m_stack_test_p_2;
    const uint32_t c_stackTestSecondAddress = (uint32_t)m_stack_test_p_3;

    const uint32_t c_ramStart = (uint32_t)__RAM_start__;/* symbol from Linker command file */
    const uint32_t c_ramEnd = (uint32_t)__RAM_end__; /* symbol from Linker command file */

    const uint32_t c_romStart = (uint32_t)__ROM_start__;/* symbol from Linker command file */
    const uint32_t c_romEnd = (uint32_t)__ROM_end__;/* symbol from Linker command file */
    
     /* The safety-related memory marker */
    extern uint32_t Image$$ER_IROM3$$Limit;
    extern uint32_t Load$$ER_IROM3$$Limit;

    /* The safety-related FLASH CRC value. */
    fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
    {
        .ui16Start      = 0xA55AU,
        .ui32FlashStart = (uint32_t)__ROM_start__,
        .ui32FlashEnd   = (uint32_t)&Load$$ER_IROM3$$Limit,
        .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
        .ui16End        = 0x5AA5U
    };    

#else /* IAR + MCUXpresso */
    extern uint32_t m_wd_test_backup;   /* from Linker configuration file */
    const uint32_t c_wdBackupAddress = (uint32_t)&m_wd_test_backup;
    #define WATCHDOG_TEST_VARIABLES ((fs_wdog_test_t *) c_wdBackupAddress)

    extern uint32_t m_ram_test_backup; /* symbol from Linker configuration file */
    const uint32_t c_backupAddress = (uint32_t)&m_ram_test_backup;

    extern uint32_t m_pc_test_flag;   /* from Linker configuration file */
    const uint32_t u_programCounterTestFlag = (uint32_t)&m_pc_test_flag;
    #define PC_TEST_FLAG ((uint32_t  *)  u_programCounterTestFlag)

    extern uint32_t m_safety_error_code; /* from Linker configuration file */
    const uint32_t c_safetyErrorCodeAddress = (uint32_t)&m_safety_error_code;
    #define SAFETY_ERROR_CODE ((uint32_t *) c_safetyErrorCodeAddress)

    extern uint32_t m_stack_test_p_2;   /* symbol from Linker configuration file */
    extern uint32_t m_stack_test_p_3;   /* symbol from Linker configuration file */
    /* put values from extern symbols to const variables */
    const uint32_t c_stackTestFirstAddress = (uint32_t)&m_stack_test_p_2;
    const uint32_t c_stackTestSecondAddress = (uint32_t)&m_stack_test_p_3;

    #if defined(__IAR_SYSTEMS_ICC__) /* IAR */
        extern const uint32_t __checksum;  /* calculated by Linker */
    	const uint32_t        c_checksumStart @ "checksum_start_mark";
    	const uint32_t        c_checksumEnd   @ "checksum_end_mark";

        extern uint32_t __ROM_start__;  /* from Linker configuration file */
        const uint32_t c_romStart = (uint32_t)&__ROM_start__;
        extern uint32_t __ROM_end__;    /* from Linker configuration file */
        const uint32_t c_romEnd = (uint32_t)&__ROM_end__;
        extern uint32_t __RAM_start__;  /* from Linker configuration file */
        const uint32_t c_ramStart = (uint32_t)&__RAM_start__;
        extern uint32_t __RAM_end__;    /* from Linker configuration file */
        const uint32_t c_ramEnd = (uint32_t)&__RAM_end__;
    #else /* MCUXpresso */
        extern uint32_t crcPostbuild32; /* defined in main.c */
        extern uint32_t stack_test_block_size; /* from Linker command file */
        #define STACK_TEST_BLOCK_SIZE (uint32_t)&stack_test_block_size
        extern uint32_t ram_test_backup_size; /* from Linker command file */
        #define RAM_TEST_BACKUP_SIZE (uint32_t)&ram_test_backup_size

        extern uint32_t __RAM_start__; /* symbol from Linker command file */
        const uint32_t c_ramStart = (uint32_t)&__RAM_start__;

        extern uint32_t __RAM_end__; /* symbol from Linker command file */
        const uint32_t c_ramEnd = (uint32_t)&__RAM_end__;

        extern uint32_t __ROM_start__  ; /* symbol from Linker command file */
        const uint32_t c_romStart = (uint32_t)&__ROM_start__  ;

        extern uint32_t __ROM_end__  ; /* symbol from Linker command file */
        const uint32_t c_romEnd = (uint32_t)&__ROM_end__  ;
  
        /* Safety-related FLASH boundaries. */
        extern uint32_t m_safety_flash_end;

        /* The safety-related FLASH CRC value. */
        fs_crc_t c_sfsCRC __attribute__((used, section(".flshcrc"))) =
        {
            .ui16Start      = 0xA55AU,
            .ui32FlashStart = (uint32_t)&__ROM_start__,
            .ui32FlashEnd   = (uint32_t)&m_safety_flash_end,
            .ui32CRC        = (uint32_t)FS_CFG_FLASH_TST_CRC,
            .ui16End        = 0x5AA5U
        };
    #endif
#endif

fs_aio_test_t aio_Str;
const fs_aio_limits_t FS_ADC_Limits[FS_CFG_AIO_CHANNELS_CNT] = FS_CFG_AIO_CHANNELS_LIMITS_INIT;
const uint8_t FS_ADC_inputs[FS_CFG_AIO_CHANNELS_CNT]         = FS_CFG_AIO_CHANNELS_INIT;
const uint8_t FS_ADC_samples[FS_CFG_AIO_CHANNELS_CNT]        = FS_CFG_AIO_SAMPLES_INIT;

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
//#include "safety_test_items.h"

void SafetyWatchdogTest(safety_common_t *psSafetyCommon, wd_test_t *psSafetyWdTest)
{
#if WATCHDOG_ENABLED
    uint32_t counterLimitHigh;
    uint32_t counterLimitLow;
    uint32_t runTestConditionSRS0;
    uint32_t runTestConditionSRS1;
    uint32_t checkTestCondition;

    /* calculate counter limit values */
    psSafetyWdTest->wdTestTemp1     = (uint64_t)WATCHDOG_TIMEOUT_VALUE * (uint64_t)WD_REF_TIMER_CLOCK_FREQUENCY;
    psSafetyWdTest->wdTestExpected  = psSafetyWdTest->wdTestTemp1 / (uint32_t)WATCHDOG_CLOCK;
    psSafetyWdTest->wdTestTolerance = (psSafetyWdTest->wdTestExpected * (uint32_t)WD_TEST_TOLERANCE) / (uint32_t)100;
    psSafetyWdTest->wdTestLimitHigh = psSafetyWdTest->wdTestExpected + psSafetyWdTest->wdTestTolerance;
    psSafetyWdTest->wdTestLimitLow  = psSafetyWdTest->wdTestExpected - psSafetyWdTest->wdTestTolerance;
    counterLimitHigh                = psSafetyWdTest->wdTestLimitHigh;
    counterLimitLow                 = psSafetyWdTest->wdTestLimitLow;

    runTestConditionSRS0 = WD_RUN_TEST_CONDITION_SRS0;
    runTestConditionSRS1 = WD_RUN_TEST_CONDITION_SRS1;
    checkTestCondition   = WD_CHECK_TEST_CONDITION;

    /*Library variables initialization*/
    WATCHDOG_TEST_VARIABLES->RefTimerBase         = (uint32_t)WDOG_REF_TIMER_BASE;
    WATCHDOG_TEST_VARIABLES->WdogBase             = (uint32_t)USED_WDOG;
    WATCHDOG_TEST_VARIABLES->pResetDetectRegister = (uint32_t)(RESET_DETECT_REGISTER);
    WATCHDOG_TEST_VARIABLES->ResetDetectMask      = (uint32_t)RESET_DETECT_MASK;

#ifdef _MKE18F16_H_

    LPTMRInit();

    if (*(RESET_DETECT_REGISTER)&runTestConditionSRS0) /* If non WD reset */
    {
        FS_WDOG_Setup_LPTMR(WATCHDOG_TEST_VARIABLES, REFRESH_INDEX);
    }
    if (*(RESET_DETECT_REGISTER)&checkTestCondition) /* If WD reset */
    {
        FS_WDOG_Check(counterLimitHigh, counterLimitLow, WATCHDOG_RESETS_LIMIT, ENDLESS_LOOP_ENABLE,
                      WATCHDOG_TEST_VARIABLES, CLEAR_FLAG, REG_WIDE);
    }
#endif /* _MKE18F16_H_ */

#ifdef _K32W042S1M2_CM4_H_
    /* initialization of LPTMR that will be used for watchtog timeout check */

    if (SMC0->SRS & runTestConditionSRS0) /* If non WD reset */
    {
        LPTMRInit(LPTMR_USED, LPTMR_CLOCK_SIRC);
        FS_CM4_WDOG_Setup_K32W(WATCHDOG_TEST_VARIABLES, (uint32_t *)WDOG0, (uint32_t *)LPTMR0);
    }
    if (SMC0->SRS & checkTestCondition) /* If WD reset */
    {
        FS_CM4_WDOG_Check_K32W(counterLimitHigh, counterLimitLow, WATCHDOG_RESETS_LIMIT, ENDLESS_LOOP_ENABLE,
                               WATCHDOG_TEST_VARIABLES);
    }
#endif /* _K32W042S1M2_CM4_H_ */

#ifndef _MKE18F16_H_
#ifndef _K32W042S1M2_CM4_H_

    LPTMRInit();

    if ((*(RESET_DETECT_REGISTER)&runTestConditionSRS0) |
        (RCM->SRS1 &
         runTestConditionSRS1)) /* if non WD reset --- because of debugging--- in real it must be only after POR reset*/
    {
        *SAFETY_ERROR_CODE = 0; /* clean the safety error code flag */
        FS_WDOG_Setup_LPTMR(WATCHDOG_TEST_VARIABLES, REFRESH_INDEX);
    }
    if (*(RESET_DETECT_REGISTER)&checkTestCondition) /* if WD reset --- because of debugging--- in real it should be
                                                        after every non-POR reset*/
    {
        FS_WDOG_Check(counterLimitHigh, counterLimitLow, WATCHDOG_RESETS_LIMIT, ENDLESS_LOOP_ENABLE,
                      WATCHDOG_TEST_VARIABLES, CLEAR_FLAG, REG_WIDE);
    }

#endif /* #ifndef */
#endif /* #ifndef */

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
void SafetyClockTestInit(safety_common_t *psSafetyCommon, fs_clock_test_t *psSafetyClockTest)
{
    psSafetyCommon->mcgirclkFreq         = LPTMR_CLOCK_FREQUENCY;
    psSafetyClockTest->clockTestExpected = psSafetyCommon->mcgirclkFreq / (uint32_t)ISR_FREQUENCY;
    psSafetyClockTest->clockTestTolerance =
        (psSafetyClockTest->clockTestExpected * (uint32_t)CLOCK_TEST_TOLERANCE) / (uint32_t)100;
    psSafetyClockTest->clockTestLimitHigh =
        psSafetyClockTest->clockTestExpected + psSafetyClockTest->clockTestTolerance;
    psSafetyClockTest->clockTestLimitLow = psSafetyClockTest->clockTestExpected - psSafetyClockTest->clockTestTolerance;
    psSafetyClockTest->clockTestStart    = 0; /* clock test result will be processed after the first interrupt occurs */

    FS_CLK_Init((uint32_t *)&psSafetyClockTest->clockTestContext);

    /* Initialization of timer */
    LPTMRInit();
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
void SafetyClockTestIsr(fs_clock_test_t *psSafetyClockTest)
{
    FS_CLK_LPTMR((fs_lptmr_t *)LPTMR0, (uint32_t *)&psSafetyClockTest->clockTestContext);

    if (psSafetyClockTest->clockTestStart != 2)
        psSafetyClockTest->clockTestStart += 1; /* to prevent checking of result before execution */
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
void SafetyClockTestCheck(safety_common_t *psSafetyCommon, fs_clock_test_t *psSafetyClockTest)
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
void SafetyFlashTestInit(fs_flash_runtime_test_parameters_t *psFlashCrc,
                         fs_flash_configuration_parameters_t *psFlashConfig,
                         fs_flash_dcp_state_t *psFlashDCPState)
{
#if USE_PCC
    PCC->CLKCFG[PCC_CRC_INDEX] = PCC_CLKCFG_CGC(1); /* enable clock to CRC module */
#endif
#if USE_SIM
    SIM->SCGC6 |= SIM_SCGC6_CRC_MASK; /* enable clock to CRC module */
#endif

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    psFlashConfig->startAddress = (uint32_t)&c_checksumStart;
    psFlashConfig->endAddress =
        4 + (uint32_t)&c_checksumEnd; /*We must test also last adress, due to this reason +4 in IAR*/
    psFlashConfig->checksum = __checksum;

#else /* KEIL + MCUXpresso */
    psFlashConfig->startAddress             = c_sfsCRC.ui32FlashStart;
    psFlashConfig->endAddress               = c_sfsCRC.ui32FlashEnd;
    psFlashConfig->checksum                 = (uint16_t)((c_sfsCRC.ui32CRC) & 0x0000FFFF);

#if USE_CRC16
    psFlashConfig->checksum                 = 0x0000FFFF & crcPostbuild32;
#endif

#endif

    psFlashConfig->size               = psFlashConfig->endAddress - psFlashConfig->startAddress;
    psFlashConfig->startConditionSeed = (uint32_t)FLASH_TEST_CONDITION_SEED;
    psFlashConfig->blockSize          = FLASH_TEST_BLOCK_SIZE;

    psFlashCrc->actualAddress = psFlashConfig->startAddress;       /* start address */
    psFlashCrc->partCrc       = psFlashConfig->startConditionSeed; /* initial seed */
    psFlashCrc->blockSize =
        (psFlashConfig->size < psFlashConfig->blockSize) ? psFlashConfig->size : psFlashConfig->blockSize;
}

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
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon,
                               fs_flash_configuration_parameters_t *psFlashConfig,
                               fs_flash_dcp_state_t *psFlashDCPState)
{
    psSafetyCommon->FLASH_test_result = FS_CM4_CM7_FLASH_HW16(psFlashConfig->startAddress, psFlashConfig->size,
                                                              CRC_BASE, psFlashConfig->startConditionSeed);
    if (psSafetyCommon->FLASH_test_result != psFlashConfig->checksum)
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

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
                            fs_flash_runtime_test_parameters_t *psFlashCrc,
                            fs_flash_configuration_parameters_t *psFlashConfig,
                            fs_flash_dcp_state_t *psFlashDCPState)
{
    /* CRC calculation for a given block of Flash memory */
    psFlashCrc->partCrc =
        FS_CM4_CM7_FLASH_HW16(psFlashCrc->actualAddress, psFlashCrc->blockSize, CRC_BASE, psFlashCrc->partCrc);
    if (FS_FLASH_FAIL == SafetyFlashTestHandling(psFlashCrc, psFlashConfig))
    {
        psSafetyCommon->safetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Handling of flash test when used in runtime.
 *
 *          The function updates the flash test variables, when flash is tested in sequence.
 *
 * @param   __checksum    - Constant that is calculated by Linker and stored in Flash.
 * @param   psFlashCrc    - The pointer of the Flash CRC structure.
 * @param   psFlashConfig - The pointer of the Safety Flash test configuration structure.
 *
 * @return  Result of the flash test: FS_FLASH_FAIL or FS_FLASH_PASS
 */
uint32_t SafetyFlashTestHandling(fs_flash_runtime_test_parameters_t *psFlashCrc,
                                 fs_flash_configuration_parameters_t *psFlashConfig)
{
    uint32_t status = FS_FLASH_PROGRESS;

    psFlashCrc->actualAddress += psFlashCrc->blockSize; /* set the actual address for testing */

    if (psFlashCrc->actualAddress == psFlashConfig->endAddress)
    {
        if (psFlashCrc->partCrc == psFlashConfig->checksum) /* checksum must be same as calculated in post-build */
        {
            status = FS_FLASH_PASS;
            runtime_test_finish_flag |=
                INVARIABLE_RUNTIME_PASSED; /*Variable for development run test, useless for final application */
        }
        else
        {
            status = FS_FLASH_FAIL;
        }

        psFlashCrc->partCrc       = psFlashConfig->startConditionSeed; /* set start seed as input for CRC calculation */
        psFlashCrc->actualAddress = psFlashConfig->startAddress;       /* set start address */
        psFlashCrc->blockSize     = psFlashConfig->blockSize;          /* size of block for CRC testing */
    }

    if ((psFlashCrc->actualAddress + psFlashCrc->blockSize) >= psFlashConfig->endAddress)
    {
        psFlashCrc->blockSize = psFlashConfig->endAddress - psFlashCrc->actualAddress;
    }

    return status;
}

/*!
 * @brief   Initialization of Safety RAM test.
 *
 *          Inits the RAM test variables
 *
 * @param   psSafetyRamTest     - The pointer of the RAM test structure.
 * @param   pui32SafetyRamStart - The pointer of the RAM test start address.
 * @param   pui32SafetyRamEnd   - The pointer of the RAM test end address.
 *
 * @return  None
 */
void SafetyRamTestInit(fs_ram_test_t *psSafetyRamTest, uint32_t *pSafetyRamStart, uint32_t *pSafetyRamEnd)
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
void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, fs_ram_test_t *psSafetyRamTest)
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
void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, fs_ram_test_t *psSafetyRamTest)
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
 *          This function uses two addresses: first is defined in linker file (iec60730b_cm4_cm7_pc_object.o),
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
#ifdef _K32W042S1M2_CM4_H_
    psSafetyCommon->CPU_special_test_result = FS_CM4_CM7_CPU_Special8PriorityLevels();
#else
    psSafetyCommon->CPU_special_test_result = FS_CM4_CM7_CPU_Special();
#endif /* _K32W042S1M2_CM4_H_ */
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

#ifdef _K32W042S1M2_CM4_H_
    psSafetyCommon->CPU_special_test_result = FS_CM4_CM7_CPU_Special8PriorityLevels();
#else
    psSafetyCommon->CPU_special_test_result = FS_CM4_CM7_CPU_Special();
#endif /* _K32W042S1M2_CM4_H_ */
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
 * @brief  SafetyDIOTestInit
 *
 *         Check if every item of input array has valid pin definition.
 *         It also fills the pcr variable with appropriate address (pin control register address), which is used in DIO
 * test.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   *pTestItems    - Array of pointers to DIO test items (pin definitions)
 *
 * @return None
 */
void SafetyDIOTestInit(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestItems[])
{
    uint32_t st = 0;

    /* Check every item */
    for (uint16_t i = 0; pTestItems[i] != 0; i++)
    {
        /* Check pin number */
        if (pTestItems[i]->pinNum > 31)
        {
            st = 1; // Error - invalid pin number
        }

        /* Check pin direction */
        if (pTestItems[i]->pinDir != PIN_DIRECTION_IN && pTestItems[i]->pinDir != PIN_DIRECTION_OUT)
            st = 1;

        /* Check pin mux */
        if (pTestItems[i]->pinMux > 7)
            st = 1;

        /* Error check */
        if (st)
        {
            psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }
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
void SafetyDigitalOutputTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin)
{
    PortInit(pTestedPin->gpio, pTestedPin->pcr, pTestedPin->pinNum, PIN_DIRECTION_OUT, PIN_MUX_GPIO);

    psSafetyCommon->DIO_output_test_result = FS_DIO_Output(pTestedPin, DIO_WAIT_CYCLE);
    if ((psSafetyCommon->DIO_output_test_result) == FS_FAIL_DIO)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortInit(pTestedPin->gpio, pTestedPin->pcr, pTestedPin->pinNum, pTestedPin->pinDir, pTestedPin->pinMux);
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
                                           fs_dio_test_t *pTestedPin,
                                           fs_dio_test_t *pAdjPin,
                                           uint32_t PinValue)

{
    PortInit(pTestedPin->gpio, pTestedPin->pcr, pTestedPin->pinNum, PIN_DIRECTION_IN, PIN_MUX_GPIO);
    PortInit(pAdjPin->gpio, pTestedPin->pcr, pAdjPin->pinNum, PIN_DIRECTION_OUT, PIN_MUX_GPIO);

    psSafetyCommon->DIO_short_test_result = FS_DIO_ShortToAdjSet(pTestedPin, pAdjPin, PinValue, DIO_BACKUP);
    if ((psSafetyCommon->DIO_short_test_result) == FS_FAIL_DIO)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* if needed, place some delay loop here */
    for (int32_t i = 0; i < 100; i++)
    {
        __asm("nop");
    }

    psSafetyCommon->DIO_input_test_result = FS_DIO_InputExt(pTestedPin, pAdjPin, PinValue, DIO_BACKUP);
    if ((psSafetyCommon->DIO_input_test_result) == FS_FAIL_DIO)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortInit(pTestedPin->gpio, pTestedPin->pcr, pTestedPin->pinNum, pTestedPin->pinDir, pTestedPin->pinMux);
    PortInit(pAdjPin->gpio, pTestedPin->pcr, pAdjPin->pinNum, pAdjPin->pinDir, pAdjPin->pinMux);
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
                                              fs_dio_test_t *pTestedPin,
                                              uint8_t polarity)
{
    PortInit(pTestedPin->gpio, pTestedPin->pcr, pTestedPin->pinNum, PIN_DIRECTION_IN, PIN_MUX_GPIO);

    psSafetyCommon->DIO_short_test_result = FS_DIO_ShortToSupplySet(pTestedPin, polarity, DIO_BACKUP);
    if ((psSafetyCommon->DIO_short_test_result) == FS_FAIL_DIO)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* if needed, place some delay loop here */
    for (int32_t i = 0; i < 100; i++)
    {
        __asm("nop");
    }

    psSafetyCommon->DIO_input_test_result = FS_DIO_InputExt(pTestedPin, pTestedPin, polarity, DIO_BACKUP);
    if ((psSafetyCommon->DIO_input_test_result) == FS_FAIL_DIO)
    {
        psSafetyCommon->safetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortInit(pTestedPin->gpio, pTestedPin->pcr, pTestedPin->pinNum, pTestedPin->pinDir, pTestedPin->pinMux);
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
#ifdef _MKV46F16_H_
    FS_AIO_InputInit_CYCLIC(&aio_Str, (fs_aio_limits_t *)FS_ADC_Limits, (uint8_t *)FS_ADC_inputs,
                            (uint8_t *)FS_ADC_samples, FS_CFG_AIO_CHANNELS_CNT);
#elif _K32W042S1M2_CM4_H_
    FS_AIO_InputInit_K3S(&aio_Str, (fs_aio_limits_t *)FS_ADC_Limits, (uint8_t *)FS_ADC_inputs, FS_CFG_AIO_CHANNELS_CNT,
                         ADC_COMMAND_BUFFER);
#else
    FS_AIO_InputInit(&aio_Str, (fs_aio_limits_t *)FS_ADC_Limits, (uint8_t *)FS_ADC_inputs, FS_CFG_AIO_CHANNELS_CNT);
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
void SafetyAnalogTest(safety_common_t *psSafetyCommon)
{
#ifdef _MKE18F16_H_
    psSafetyCommon->AIO_test_result = FS_AIO_InputCheck_KE(&aio_Str, (fs_aio_ke_t *)TESTED_ADC);
    switch (psSafetyCommon->AIO_test_result)
    {
        case FS_AIO_START: /* state START means that everything is ready to trigger the conversion */
            FS_AIO_InputSet_KE(&aio_Str, (fs_aio_ke_t *)TESTED_ADC);
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
#endif /* _MKE18F16_H_ */

#ifdef _MKV46F16_H_
    psSafetyCommon->AIO_test_result = FS_AIO_InputCheck_CYCLIC(&aio_Str, (fs_aio_cyclic_t *)TESTED_ADC);

    switch (psSafetyCommon->AIO_test_result)
    {
        case FS_AIO_START:
        { /* state START means that everything is ready to trigger the conversion */
            FS_AIO_InputSet_CYCLIC(&aio_Str, (fs_aio_cyclic_t *)TESTED_ADC);
            TESTED_ADC->CTRL1 |= ADC_CTRL1_START0_MASK;
        } /* ADCA start conversion */
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

#endif /* _MKV46F16_H_ */

#ifdef _K32W042S1M2_CM4_H_
    uint32_t adcResFifo;

    if (ADC0->STAT & ADC_STAT_RDY_MASK) /* if RDY flag is valid, read RESFIFO register */
    {
        adcResFifo = ADC0->RESFIFO;
    }
    /* call the check function */
    psSafetyCommon->AIO_test_result = FS_AIO_InputCheck_K3S(&aio_Str, (uint32_t)TESTED_ADC, adcResFifo);

    switch (psSafetyCommon->AIO_test_result)
    {
        case FS_AIO_START: /* state START means that everything is ready to trigger the conversion */
            FS_AIO_InputSet_K3S(&aio_Str, (uint32_t)TESTED_ADC);
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
#endif /* _K32W042S1M2_CM4_H_ */

#ifndef _MKE18F16_H_
#ifndef _MKV46F16_H_
#ifndef _K32W042S1M2_CM4_H_

    psSafetyCommon->AIO_test_result = FS_AIO_InputCheck(&aio_Str, (fs_aio_t *)TESTED_ADC);

    switch (psSafetyCommon->AIO_test_result)
    {
        case FS_AIO_START: /* state START means that everything is ready to trigger the conversion */
            FS_AIO_InputSet(&aio_Str, (fs_aio_t *)TESTED_ADC);
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

#endif /* _K32W042S1M2_CM4_H_ */
#endif /* _MKV46F16_H_ */
#endif /* _MKE18F16_H_ */
}

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
void SafetyIsrFunction(safety_common_t *psSafetyCommon,
                       fs_ram_test_t *psSafetyRamTest,
                       fs_ram_test_t *psSafetyRamStackTest)
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
