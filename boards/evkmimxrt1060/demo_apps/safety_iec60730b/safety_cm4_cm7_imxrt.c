/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* SPDX-License-Identifier: NXP Proprietary
* 
*
*******************************************************************************/

#include "safety_cm4_cm7_imxrt.h"

#if defined(__CC_ARM)/* For ARM(KEIL) compiler */
#include "linker_config.h"
#endif

#if defined(__IAR_SYSTEMS_ICC__)
#pragma section =  ".safety_ram"
#pragma section =  ".checksum"
#pragma location = ".checksum"
#endif

/*******************************************************************************
* Variables
******************************************************************************/
#if defined(__IAR_SYSTEMS_ICC__)
extern const uint32_t __checksum;  /* calculated by Linker */
const uint32_t        c_ui32ChecksumStart @ "checksum_start_mark";
const uint32_t        c_ui32ChecksumEnd   @ "checksum_end_mark";
#endif

#if defined(__GNUC__)
extern uint32_t ui32CrcPostbuild; /* defined in main.c */
extern uint32_t stack_test_block_size; /* from Linker command file */
#define STACK_TEST_BLOCK_SIZE (uint32_t)&stack_test_block_size
extern uint32_t ram_test_backup_size; /* from Linker command file */
#define RAM_TEST_BACKUP_SIZE (uint32_t)&ram_test_backup_size
#endif

#if defined(__GNUC__) || defined(__IAR_SYSTEMS_ICC__)
extern uint32_t m_wd_test_backup;   /* from Linker command file */
const uint32_t c_ui32WdBackupAddress = (uint32_t)&m_wd_test_backup;
#define WATCHDOG_TEST_VARIABLES ((WD_Test_Str  *)  c_ui32WdBackupAddress)

extern uint32_t m_pc_test_flag;   /* from Linker command file */
const uint32_t u_ui32ProgramCounterTestFlag = (uint32_t)&m_pc_test_flag;
#define PC_TEST_FLAG ((uint32_t  *)  u_ui32ProgramCounterTestFlag)

extern uint32_t m_safety_error_code; /* from Linker command file */
const uint32_t c_ui32SafetyErrorCodeAddress = (uint32_t)&m_safety_error_code;
#define SAFETY_ERROR_CODE ((uint32_t *) c_ui32SafetyErrorCodeAddress)

extern uint32_t m_ram_test_backup; /* symbol from Linker command file */
const uint32_t c_ui32BackupAddress = (uint32_t)&m_ram_test_backup;

extern uint32_t m_stack_test_p_2;   /* symbol from Linker command file */
extern uint32_t m_stack_test_p_3;   /* symbol from Linker command file */

/* put values from extern symbols to const variables */
const uint32_t c_ui32StackTestFirstAddress = (uint32_t)&m_stack_test_p_2;
const uint32_t c_ui32StackTestSecondAddress = (uint32_t)&m_stack_test_p_3;
#endif

#if defined(__CC_ARM)/* For ARM(KEIL) compiler */
extern uint32_t ui32CrcPostbuild; /* defined in main.c */
const uint32_t c_ui32WdBackupAddress = (uint32_t)m_wd_test_backup;
#define WATCHDOG_TEST_VARIABLES ((WD_Test_Str  *)  c_ui32WdBackupAddress)

const uint32_t u_ui32ProgramCounterTestFlag = (uint32_t)m_pc_test_flag;
#define PC_TEST_FLAG ((uint32_t  *)  u_ui32ProgramCounterTestFlag)

const uint32_t c_ui32SafetyErrorCodeAddress = (uint32_t)m_safety_error_code;
#define SAFETY_ERROR_CODE ((uint32_t *) c_ui32SafetyErrorCodeAddress)

const uint32_t c_ui32BackupAddress = (uint32_t)m_ram_test_backup;

/* put values from extern symbols to const variables */
const uint32_t c_ui32StackTestFirstAddress = (uint32_t)m_stack_test_p_2;
const uint32_t c_ui32StackTestSecondAddress = (uint32_t)m_stack_test_p_3;
#endif

extern dcp_channels_t g_dcpSafetyChannel;

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
void SafetyWatchdogTest(wd_test_t *psSafetyWdTest)
{
#ifdef WATCHDOG_ON
    uint32_t ui32CounterLimitHigh;
    uint32_t ui32CounterLimitLow;
    uint32_t ui32RunTestCondition;
    uint32_t ui32CheckTestCondition;
  
    /* calculate counter limit values */
    psSafetyWdTest->ui64WdTestTemp1 = (uint64_t)WATCHDOG_TIMEOUT_VALUE * (uint64_t)WD_REF_TIMER_CLOCK_FREQUENCY;
    psSafetyWdTest->ui32WdTestExpected = psSafetyWdTest->ui64WdTestTemp1 / (uint32_t)WATCHDOG_CLOCK;
    psSafetyWdTest->ui32WdTestTolerance = (psSafetyWdTest->ui32WdTestExpected * (uint32_t)WD_TEST_TOLERANCE) /(uint32_t)100 ;
    psSafetyWdTest->ui32WdTestLimitHigh = psSafetyWdTest->ui32WdTestExpected + psSafetyWdTest->ui32WdTestTolerance;
    psSafetyWdTest->ui32WdTestLimitLow = psSafetyWdTest->ui32WdTestExpected - psSafetyWdTest->ui32WdTestTolerance;    
    ui32CounterLimitHigh = psSafetyWdTest->ui32WdTestLimitHigh;
    ui32CounterLimitLow = psSafetyWdTest->ui32WdTestLimitLow;
    
    ui32RunTestCondition = WD_RUN_TEST_CONDITION;
    ui32CheckTestCondition = WD_CHECK_TEST_CONDITION;

    /* Init timer */
    GPT1_Init(WD_GPT1_SRC, WD_GPT1_CMP, WD_GPT1_DIV);
    
    if (SRC->SRSR & ui32RunTestCondition) /* If non WD reset */
    {   
        /* Clear tested flags */
        SRC->SRSR = ui32RunTestCondition;

        IEC60730B_CM4_CM7_watchdog_test_setup_RT(WATCHDOG_TEST_VARIABLES);
    }

    if (SRC->SRSR & ui32CheckTestCondition)   /* If WD reset */
    {
        IEC60730B_CM4_CM7_watchdog_test_check_RT(ui32CounterLimitHigh, ui32CounterLimitLow, WATCHDOG_RESETS_LIMIT, ENDLESS_LOOP_ENABLE, WATCHDOG_TEST_VARIABLES);

        /* Clear WDOG3 flag */
        SRC->SRSR = SRC_SRSR_WDOG3_RST_B_MASK;
    }
    
#endif /* WATCHDOG_ON */
    
    psSafetyWdTest->ui32WatchdogResets = WATCHDOG_TEST_VARIABLES->resets;
    psSafetyWdTest->ui32WatchdogTimeoutCheck = WATCHDOG_TEST_VARIABLES->counter;
    psSafetyWdTest->ui16WatchdogRefreshRatio = 0;
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
    psSafetyWdTest->ui16WatchdogRefreshRatio++;
    if (psSafetyWdTest->ui16WatchdogRefreshRatio == WATCHDOG_REFRESH_RATIO)
    {
#ifdef WATCHDOG_ON
        Watchdog_refresh; /* refreshing the watchdog */
#endif
        psSafetyWdTest->ui16WatchdogRefreshRatio = 0;
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
    psSafetyCommon->ui32McgirclkFreq = CLOCK_GPT1_FREQ;
    psSafetyClockTest->ui32ClockTestExpected = psSafetyCommon->ui32McgirclkFreq / (uint32_t)ISR_FREQUENCY;
    psSafetyClockTest->ui32ClockTestTolerance = (psSafetyClockTest->ui32ClockTestExpected * (uint32_t)CLOCK_TEST_TOLERANCE) /(uint32_t)100;
    psSafetyClockTest->ui32ClockTestLimitHigh = psSafetyClockTest->ui32ClockTestExpected + psSafetyClockTest->ui32ClockTestTolerance;
    psSafetyClockTest->ui32ClockTestLimitLow  = psSafetyClockTest->ui32ClockTestExpected - psSafetyClockTest->ui32ClockTestTolerance;
    psSafetyClockTest->ui16ClockTestStart = 0; /* clock test result will be processed after the first interrupt occurs */

    IEC60730B_CM4_CM7_CLK_SYNC_Init((unsigned long *)&psSafetyClockTest->ui32ClockTestContext);
    
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
    IEC60730B_CM4_CM7_CLK_SYNC_GPT_Isr((unsigned long *)GPT1, (unsigned long *)&psSafetyClockTest->ui32ClockTestContext);

    if(psSafetyClockTest->ui16ClockTestStart != 2)
    psSafetyClockTest->ui16ClockTestStart += 1;  /* to prevent checking of result before execution */
}

/*!
* @brief   Clock test check function.
*
*          This function can be called from any place of application.
*          It calls the IEC60730B_CM4_CM7_CLK_Check function from the IEC60730 library
*          In case of incorrect clock test result, it updates the ui32SafetyErrors variable accordingly.
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
    if (psSafetyClockTest->ui16ClockTestStart == 2)  /* condition is valid after the first Systick interrupt */
    {
        psSafetyCommon->IEC60730B_clock_test_result = IEC60730B_CM4_CM7_CLK_Check(psSafetyClockTest->ui32ClockTestContext, psSafetyClockTest->ui32ClockTestLimitLow, psSafetyClockTest->ui32ClockTestLimitHigh);
        if(psSafetyCommon->IEC60730B_clock_test_result == IEC60730B_ST_CLK_FAIL)
        {
            psSafetyCommon->ui32SafetyErrors |= CLOCK_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }
    }
}

/*!
* @brief   Initialization of DCP packet to zeros.
*
* @param   psFlashDCPState - struct which contains packet per channel.
*
* @return  None
*/
void InitPackets(flash_dcp_state_t *psFlashDCPState)
{
    for(uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->ui32Iec60730bCH0Packet[size] = 0;
    }
    for(uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->ui32Iec60730bCH1Packet[size] = 0;
    }
    for(uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->ui32Iec60730bCH2Packet[size] = 0;
    }
    for(uint8_t size = 0; size < 8; size++)
    {
        psFlashDCPState->ui32Iec60730bCH3Packet[size] = 0;
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
void SafetyFlashTestInit(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, flash_dcp_state_t *psFlashDCPState)
{
#if defined(__IAR_SYSTEMS_ICC__)
    psFlashConfig->ui32Iec60730bStartAddress = (uint32_t)&c_ui32ChecksumStart;
    psFlashConfig->ui32Iec60730bEndAddress = 4 + (uint32_t)&c_ui32ChecksumEnd; /*We must test also last adress, due to this reason +4 in IAR*/
    psFlashConfig->ui32Iec60730bChecksum = __checksum;
#endif
#if defined(__GNUC__) || defined(__CC_ARM)
    psFlashConfig->ui32Iec60730bStartAddress = (uint32_t)FLASH_TEST_START_ADDRESS;
    psFlashConfig->ui32Iec60730bEndAddress = (uint32_t)FLASH_TEST_END_ADDRESS;
    psFlashConfig->ui32Iec60730bChecksum = ui32CrcPostbuild;
#endif

    psFlashConfig->ui32Iec60730bSize = psFlashConfig->ui32Iec60730bEndAddress - psFlashConfig->ui32Iec60730bStartAddress;
    psFlashConfig->ui32Iec60730bStartConditionSeed = (uint32_t)FLASH_TEST_CONDITION_SEED;
    psFlashConfig->ui32Iec60730bBlockSize = FLASH_TEST_BLOCK_SIZE;

    psFlashCrc->ui32ActualAddress  = psFlashConfig->ui32Iec60730bStartAddress;    /* start address */
    psFlashCrc->ui32PartCrc = psFlashConfig->ui32Iec60730bStartConditionSeed;    /* initial seed */
    psFlashCrc->ui32BlockSize = (psFlashConfig->ui32Iec60730bSize < psFlashConfig->ui32Iec60730bBlockSize) ? psFlashConfig->ui32Iec60730bSize : psFlashConfig->ui32Iec60730bBlockSize;
    
    /* Init DCP channels states and results */
    psFlashDCPState->ui32Iec60730bCH0State = IEC60730B_ST_DCP_AVAILABLE;
    psFlashDCPState->ui32Iec60730bCH0Result = 0;
    psFlashDCPState->ui32Iec60730bCH1State = IEC60730B_ST_DCP_AVAILABLE;
    psFlashDCPState->ui32Iec60730bCH1Result = 0;
    psFlashDCPState->ui32Iec60730bCH2State = IEC60730B_ST_DCP_AVAILABLE;
    psFlashDCPState->ui32Iec60730bCH2Result = 0;
    psFlashDCPState->ui32Iec60730bCH3State = IEC60730B_ST_DCP_AVAILABLE;
    psFlashDCPState->ui32Iec60730bCH3Result = 0;
    InitPackets(psFlashDCPState);
    g_dcpSafetyChannel = DCP_CH3;
}

/*!
* @brief   After-reset Flash test.
*
*          This function calls the flash test function from IEC60730 library.
*          Safety-related part of the flash is tested at once.
*          In case of incorrect flash test result, it updates the ui32SafetyErrors variable accordingly.
*
* @param   psSafetyCommon - The pointer of the Common Safety structure
* @param   psFlashConfig  - The pointer of the Safety Flash test configuration structure.
*
* @return  None
*/
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, flash_configuration_parameters_t *psFlashConfig, flash_dcp_state_t *psFlashDCPState)
{
    /* CRC calculation of SAFETY_FLASH_BLOCK (channel should be available after reset) */
    do
    {
        IEC60730B_CM7_Flash_HWTest_DCP(psFlashConfig->ui32Iec60730bStartAddress, psFlashConfig->ui32Iec60730bSize,
                                       psFlashConfig->ui32Iec60730bStartConditionSeed, g_dcpSafetyChannel, psFlashDCPState, 0);
    }
    while (psFlashDCPState->ui32Iec60730bCH3State == IEC60730B_ST_DCP_BUSY);
    
    /* Store the result */
    psSafetyCommon->IEC60730B_flash_test_result = psFlashDCPState->ui32Iec60730bCH3Result;
    
    /* Check if result equals precomputed CRC value */
    if(psSafetyCommon->IEC60730B_flash_test_result != psFlashConfig->ui32Iec60730bChecksum)
    {
        psSafetyCommon->ui32SafetyErrors |= FLASH_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }   
}

/*!
* @brief   Runtime Flash test.
*
*          This function calls the flash test function from IEC60730 library.
*          Safety-related part of the flash is tested in sequence.
*          Calls SafetyFlashTestHandling function.
*          In case of incorrect flash test result, it updates the ui32SafetyErrors variable accordingly.
*          A node of program flow check is placed here.
*
* @param   psSafetyCommon          - The pointer of the Common Safety structure
* @param   psFlashCrc              - The pointer of the Flash CRC structure.
* @param   psFlashConfig           - The pointer of the Safety Flash test configuration structure.
* @param   psSafetyProgramFlowTest - The pointer of the Program flow test structure
*
* @return  None
*/
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon, flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, flash_dcp_state_t *psFlashDCPState)
{
    /* Start CRC calculation for a given block of Flash memory */
    IEC60730B_CM7_Flash_HWTest_DCP(psFlashCrc->ui32ActualAddress, psFlashCrc->ui32BlockSize,
                               psFlashCrc->ui32PartCrc, g_dcpSafetyChannel, psFlashDCPState, 0);          

    /* Check if calculation finished */
    if (psFlashDCPState->ui32Iec60730bCH3State == IEC60730B_ST_DCP_AVAILABLE)
    {
        /* Store partial result */
        psFlashCrc->ui32PartCrc = psFlashDCPState->ui32Iec60730bCH3Result;
      
        /* Handle result */
        if(SafetyFlashTestHandling(psFlashCrc, psFlashConfig) == IEC60730B_ST_FLASH_FAIL)
        {
            psSafetyCommon->ui32SafetyErrors |= FLASH_TEST_ERROR;
            SafetyErrorHandling(psSafetyCommon);
        }
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
* @return  Result of the flash test: IEC60730B_ST_FLASH_FAIL or IEC60730B_ST_FLASH_PASS
*/
uint32_t SafetyFlashTestHandling(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig)
{
    uint32_t ui32Status;
	ui32Status = IEC60730B_ST_FLASH_PROGRESS;

	psFlashCrc->ui32ActualAddress += psFlashCrc->ui32BlockSize;    /* set the actual address for testing */

    if(psFlashCrc->ui32ActualAddress == psFlashConfig->ui32Iec60730bEndAddress)
    {
        if(psFlashCrc->ui32PartCrc == psFlashConfig->ui32Iec60730bChecksum)      /* checksum must be same as calculated in post-build */
        {
        	ui32Status = IEC60730B_ST_FLASH_PASS;
        }
        else
        {
        	ui32Status = IEC60730B_ST_FLASH_FAIL;
        }

        psFlashCrc->ui32PartCrc = psFlashConfig->ui32Iec60730bStartConditionSeed;   /* set start seed as input for CRC calculation */
        psFlashCrc->ui32ActualAddress = psFlashConfig->ui32Iec60730bStartAddress;   /* set start address */
        psFlashCrc->ui32BlockSize = psFlashConfig->ui32Iec60730bBlockSize;          /* size of block for CRC testing */
    }

    if((psFlashCrc->ui32ActualAddress + psFlashCrc->ui32BlockSize) >= psFlashConfig->ui32Iec60730bEndAddress)
    {
    	psFlashCrc->ui32BlockSize = psFlashConfig->ui32Iec60730bEndAddress - psFlashCrc->ui32ActualAddress;

    }

    return (ui32Status);
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
void SafetyRamTestInit(ram_test_t *psSafetyRamTest, uint32_t *pui32SafetyRamStart, uint32_t *pui32SafetyRamEnd)
{
    psSafetyRamTest->ui32RamTestStartAddress = (uint32_t)pui32SafetyRamStart;
    psSafetyRamTest->ui32RamTestEndAddress =   (uint32_t)pui32SafetyRamEnd;
    psSafetyRamTest->ui32DefaultBlockSize =  RAM_TEST_BACKUP_SIZE;
    psSafetyRamTest->ui32BlockSize = RAM_TEST_BLOCK_SIZE;
    psSafetyRamTest->ui32ActualAddress = psSafetyRamTest->ui32RamTestStartAddress;

#if defined(__CC_ARM)  /* For ARM(KEIL) compiler */
    psSafetyRamTest->ui32BackupAddress = (uint32_t)m_ram_test_backup;
#else /*For IAR and MCUx compiler*/
    psSafetyRamTest->ui32BackupAddress = (uint32_t)&m_ram_test_backup;
#endif
}

/*!
* @brief   After-reset RAM test.
*
*          This function calls the RAM test function from IEC60730 library.
*          Safety-related part of the RAM is tested at once.
*          In case of incorrect RAM test result, it updates the ui32SafetyErrors variable accordingly.
*
* @param   psSafetyCommon  - The pointer of the Common Safety structure
* @param   psSafetyRamTest - The pointer of the Safety RAM test structure.
*
* @return  None
*/
void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->IEC60730B_ram_test_result = IEC60730B_CM4_CM7_RAM_AfterResetTest(psSafetyRamTest->ui32RamTestStartAddress, \
                                                psSafetyRamTest->ui32RamTestEndAddress,   \
                                                psSafetyRamTest->ui32DefaultBlockSize,    \
                                                psSafetyRamTest->ui32BackupAddress,       \
                                                IEC60730B_CM4_CM7_RAM_SegmentMarchC);
    if(psSafetyCommon->IEC60730B_ram_test_result == IEC60730B_ST_RAM_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
* @brief   Runtime RAM test.
*
*          This function calls the RAM test function from IEC60730 library.
*          Safety-related part of the RAM is tested in sequence.
*          Calls SafetyFlashTestHandling function.
*          In case of incorrect RAM test result, it updates the ui32SafetyErrors variable accordingly.
*
* @param   psSafetyCommon  - The pointer of the Common Safety structure
* @param   psSafetyRamTest - The pointer of the Safety RAM test structure.
*
* @return  None
*/
void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest)
{
    psSafetyCommon->IEC60730B_ram_test_result = IEC60730B_CM4_CM7_RAM_RuntimeTest(psSafetyRamTest->ui32RamTestStartAddress, \
        psSafetyRamTest->ui32RamTestEndAddress,   \
            (unsigned long *)&psSafetyRamTest->ui32ActualAddress,      \
                psSafetyRamTest->ui32BlockSize,           \
                    psSafetyRamTest->ui32BackupAddress,       \
                        IEC60730B_CM4_CM7_RAM_SegmentMarchX);

    if(psSafetyCommon->IEC60730B_ram_test_result == IEC60730B_ST_RAM_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= RAM_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
* @brief   Program counter test.
*
*          This function uses two addresses: first is defined in linker file (IEC60730_B_CM4_CM7_pc_object.o),
*          second address comes as function argument (must be RAM address).
*          Both addresses must be defined by the developer and suitable to test all of the possible PC bits.
*          This test can�t be interrupted.
*          In case of incorrect PC test result, it updates the ui32SafetyErrors variable accordingly.
*
* @param   psSafetyCommon - The pointer of the Common Safety structure
* @param   pattern        - RAM address, it can vary with multiple function calls
*
* @return  None
*/
void SafetyPcTest(safety_common_t *psSafetyCommon, uint32_t pattern)
{
    psSafetyCommon->IEC60730B_pc_test_result = IEC60730B_CM7_PC_Test(pattern, IEC60730B_PC_object, (unsigned long *)PC_TEST_FLAG);
    if(psSafetyCommon->IEC60730B_pc_test_result == IEC60730B_ST_PC_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= PC_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
* @brief   After-reset CPU registers test.
*
*          This function calls the CPU test functions from IEC60730 library.
*          All the registers are tested at once.
*          In case of incorrect flash test result, it updates the ui32SafetyErrors variable accordingly.
*          See IEC60730 library documentation for CPU errors handling !
*
* @param   psSafetyCommon - The pointer of the Common Safety structure
*
* @return  None
*/
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon)
{
    /* stacked CPU registers */
    psSafetyCommon->IEC60730B_cpu_reg_test_result = IEC60730B_CM4_CM7_CPU_RegisterTest();
    if(psSafetyCommon->IEC60730B_cpu_reg_test_result == IEC60730B_ST_CPU_REGISTER_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* non-stacked CPU registers */
    psSafetyCommon->IEC60730B_cpu_non_stacked_test_result = IEC60730B_CM4_CM7_CPU_NonStackedRegisterTest();
    if(psSafetyCommon->IEC60730B_cpu_non_stacked_test_result == IEC60730B_ST_CPU_NONSTACKED_REGISTER_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* CONTROL */
#if __FPU_PRESENT
    psSafetyCommon->IEC60730B_cpu_control_test_result = IEC60730B_CM4_CM7_CPU_ControlTest_fpu();
#else
    psSafetyCommon->IEC60730B_cpu_control_test_result = IEC60730B_CM4_CM7_CPU_ControlTest();
#endif

    if(psSafetyCommon->IEC60730B_cpu_control_test_result == IEC60730B_ST_CPU_CONTROL_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_CONTROL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* SP main */
    psSafetyCommon->IEC60730B_cpu_sp_main_test_result = IEC60730B_CM4_CM7_CPU_SPmainTest();
    if(psSafetyCommon->IEC60730B_cpu_sp_main_test_result == IEC60730B_ST_CPU_SP_MAIN_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_SP_MAIN_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* SP process */
    psSafetyCommon->IEC60730B_cpu_sp_process_test_result = IEC60730B_CM4_CM7_CPU_SPprocessTest();
    if(psSafetyCommon->IEC60730B_cpu_sp_process_test_result == IEC60730B_ST_CPU_SP_PROCESS_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_SP_PROCESS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* PRIMASK */
    psSafetyCommon->IEC60730B_cpu_primask_test_result = IEC60730B_CM4_CM7_CPU_PrimaskTest();
    if(psSafetyCommon->IEC60730B_cpu_primask_test_result == IEC60730B_ST_CPU_PRIMASK_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* Special CPU registers  */ 
    psSafetyCommon->IEC60730B_cpu_special_test_result = IEC60730B_CM4_CM7_CPU_SpecialTest();
    if(psSafetyCommon->IEC60730B_cpu_special_test_result == IEC60730B_ST_CPU_SPECIAL_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* group 1 of FPU registers */
#if __FPU_PRESENT
    psSafetyCommon->IEC60730B_cpu_float_test_1_result =IEC60730B_CM4_CM7_CPU_FloatTest1();
    if(psSafetyCommon->IEC60730B_cpu_float_test_1_result == IEC60730B_ST_CPU_FLOAT_1_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
    /* group 2 of FPU registers */
    psSafetyCommon->IEC60730B_cpu_float_test_2_result =IEC60730B_CM4_CM7_CPU_FloatTest2();
    if(psSafetyCommon->IEC60730B_cpu_float_test_2_result == IEC60730B_ST_CPU_FLOAT_2_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_FLOAT_2_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#endif
}

/*!
* @brief   Uninterruptible test of CPU registers.
*
*          This function calls the CPU test functions from IEC60730 library.
*          The function must be called from an interrupt with highest priority.
*          In case of incorrect flash test result, it updates the ui32SafetyErrors variable accordingly.
*          See IEC60730 library documentation for CPU errors handling !
*
* @param   psSafetyCommon - The pointer of the Common Safety structure
*
* @return  None
*/
void SafetyCpuIsrTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->IEC60730B_cpu_primask_test_result = IEC60730B_CM4_CM7_CPU_PrimaskTest();
    if(psSafetyCommon->IEC60730B_cpu_primask_test_result == IEC60730B_ST_CPU_PRIMASK_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_PRIMASK_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->IEC60730B_cpu_sp_main_test_result = IEC60730B_CM4_CM7_CPU_SPmainTest();
    if(psSafetyCommon->IEC60730B_cpu_sp_main_test_result == IEC60730B_ST_CPU_SP_MAIN_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_SP_MAIN_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
  
    psSafetyCommon->IEC60730B_cpu_special_test_result = IEC60730B_CM4_CM7_CPU_SpecialTest();
    if(psSafetyCommon->IEC60730B_cpu_special_test_result == IEC60730B_ST_CPU_SPECIAL_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_SPECIAL_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
* @brief   Interruptible test of CPU registers.
*
*          This function calls the CPU test functions from IEC60730 library.
*          The function can be called from the background loop.
*          In case of incorrect flash test result, it updates the ui32SafetyErrors variable accordingly.
*          See IEC60730 library documentation for CPU errors handling !
*
* @param   psSafetyCommon - The pointer of the Common Safety structure
*
* @return  None
*/
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->IEC60730B_cpu_reg_test_result = IEC60730B_CM4_CM7_CPU_RegisterTest();
    if(psSafetyCommon->IEC60730B_cpu_reg_test_result == IEC60730B_ST_CPU_REGISTER_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_REGISTERS_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->IEC60730B_cpu_non_stacked_test_result = IEC60730B_CM4_CM7_CPU_NonStackedRegisterTest();
    if(psSafetyCommon->IEC60730B_cpu_non_stacked_test_result == IEC60730B_ST_CPU_NONSTACKED_REGISTER_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_NONSTACKED_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
#if __FPU_PRESENT
    psSafetyCommon->IEC60730B_cpu_float_test_1_result =IEC60730B_CM4_CM7_CPU_FloatTest1();
    if(psSafetyCommon->IEC60730B_cpu_float_test_1_result == IEC60730B_ST_CPU_FLOAT_1_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_FLOAT_1_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    psSafetyCommon->IEC60730B_cpu_float_test_2_result =IEC60730B_CM4_CM7_CPU_FloatTest2();
    if(psSafetyCommon->IEC60730B_cpu_float_test_2_result == IEC60730B_ST_CPU_FLOAT_2_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= CPU_FLOAT_2_ERROR;
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
    IEC60730B_CM4_CM7_Stack_Init(STACK_TEST_PATTERN, c_ui32StackTestFirstAddress, c_ui32StackTestSecondAddress, STACK_TEST_BLOCK_SIZE);
}


/*!
* @brief   Stack test.
*
*          This function calls the STACK test function from IEC60730 library
*          Stack is tested for underflow and overflow condition.
*          In case of incorrect Stack test result, it updates the ui32SafetyErrors variable accordingly.
*
* @param   psSafetyCommon - The pointer of the Common Safety structure
*
* @return  None
*/
void SafetyStackTest(safety_common_t *psSafetyCommon)
{
    psSafetyCommon->IEC60730B_stack_test_result =IEC60730B_CM4_CM7_Stack_Test(STACK_TEST_PATTERN, c_ui32StackTestFirstAddress, c_ui32StackTestSecondAddress, STACK_TEST_BLOCK_SIZE);
    if(psSafetyCommon->IEC60730B_stack_test_result == IEC60730B_ST_STACK_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= STACK_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
}

/*!
 * @brief   Digital Input/Output test.
 *
 *          This function calls output test functions from IEC60730 library
 *          In case of incorrect test result, it updates the ui32SafetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   *pTestedPin    - The pointer to the DIO test item structure (pin definition)
 *
 * @return  None
 */
void SafetyDigitalOutputTestRT(safety_common_t *psSafetyCommon, dio_test_rt_t *pTestedPin)
{      
    /* Setup port for testing */
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, PIN_DIRECTION_OUT);
    
    /* Output test */
    psSafetyCommon->IEC60730B_dio_output_test_result = IEC60730B_CM4_CM7_DIO_OutputTest_RT(pTestedPin, DIO_WAIT_CYCLE);

    /* Error check */
    if(psSafetyCommon->IEC60730B_dio_output_test_result == IEC60730B_ST_DIO_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }  
  
    /* Restore pin settings */
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, pTestedPin->pinDir);
}

/*!
 * @brief   Digital Input/Output Short to Adjacent pins test
 *
 *          This function calls digital io short test SET and GET functions from IEC60730 library
 *          In case of incorrect test conditions, it updates the ui32SafetyErrors variable accordingly.
 *
 * @param   psSafetyCommon - The pointer of the Common Safety structure
 * @param   *pTestedPin    - The pointer to the DIO test item structure (pin definition)
 * @param   *pAdjPin       - The pointer of the DIO test item structure for Adjacent pin (pin definition)
 * @param   PinValue       - logical 1 or logical 0 will be set on the tested pin  
 *
 * @return  None
 */
void SafetyDigitalInputOutput_ShortAdjTestRT(safety_common_t *psSafetyCommon, dio_test_rt_t *pTestedPin, dio_test_rt_t *pAdjPin, unsigned long PinValue)

{
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, PIN_DIRECTION_IN);
    PortSetup(pAdjPin->gpio,    pAdjPin->pinNum,    PIN_DIRECTION_OUT);

    psSafetyCommon->IEC60730B_dio_short_test_result = IEC60730B_CM4_CM7_DIO_ShortToAdjTest_Set_RT(pTestedPin, pAdjPin, PinValue, DIO_BACKUP);
    if((psSafetyCommon->IEC60730B_dio_short_test_result) == IEC60730B_ST_DIO_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }
      
    /* if needed, place some delay loop here */
    for(int32_t i=0; i<7000; i++)
    {
        __asm("nop");
    }
    
    psSafetyCommon->IEC60730B_dio_short_test_result = IEC60730B_CM4_CM7_DIO_InputTest_Ext_RT(pTestedPin, pAdjPin, PinValue, DIO_BACKUP);    
    if((psSafetyCommon->IEC60730B_dio_short_test_result) == IEC60730B_ST_DIO_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, pTestedPin->pinDir);
    PortSetup(pAdjPin->gpio,    pAdjPin->pinNum,    pAdjPin->pinDir);
}

/*!
 * @brief   Digital Input/Output Short to Supply test.
 *
 *          This function calls digital io short test SET and GET functions from IEC60730 library
 *          In case of incorrect test conditions, it updates the ui32SafetyErrors variable accordingly.
 *
 * @param   psSafetyCommon   - The pointer of the Common Safety structure
 * @param   *pTestedPin      - The pointer to the DIO test item structure (pin definition)
 * @param   polarity         - macro DIO_SHORT_TO_VDD_TEST or DIO_SHORT_TO_GND_TEST
 *
 * @return  None
 */
void SafetyDigitalInputOutput_ShortSupplyTestRT(safety_common_t *psSafetyCommon, dio_test_rt_t *pTestedPin, uint8_t polarity)
{
    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, PIN_DIRECTION_IN);

    psSafetyCommon->IEC60730B_dio_short_test_result = IEC60730B_CM4_CM7_DIO_ShortToSupplyTest_Set_RT(pTestedPin, polarity, DIO_BACKUP);
    if((psSafetyCommon->IEC60730B_dio_short_test_result) == IEC60730B_ST_DIO_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    /* if needed, place some delay loop here */
    for(int32_t i=0; i<7000; i++)
    {
        __asm("nop");
    }

    psSafetyCommon->IEC60730B_dio_short_test_result = IEC60730B_CM4_CM7_DIO_InputTest_Ext_RT(pTestedPin, pTestedPin, polarity, DIO_BACKUP);
    if((psSafetyCommon->IEC60730B_dio_short_test_result) == IEC60730B_ST_DIO_FAIL)
    {
        psSafetyCommon->ui32SafetyErrors |= DIO_TEST_ERROR;
        SafetyErrorHandling(psSafetyCommon);
    }

    PortSetup(pTestedPin->gpio, pTestedPin->pinNum, pTestedPin->pinDir);  
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
void SafetyIsrFunction(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest, ram_test_t *psSafetyRamStackTest)
{
    switch(psSafetyCommon->ui32FastIsrSafetySwitch){
    case 0:   /* CPU registers test that cannot be interrupted */
        SafetyCpuIsrTest(psSafetyCommon);
        break;
    case 1:   /* Program counter test */
        #ifdef PC_TEST_ON
          SafetyPcTest(psSafetyCommon, PC_TEST_PATTERN);
        #endif
        break;
    case 2:   /* RAM March test for safety related RAM space */
        SafetyRamRuntimeTest(psSafetyCommon, psSafetyRamTest);
        break;
    case 3:   /* RAM March test for memory occupied by the Stack */
        SafetyRamRuntimeTest(psSafetyCommon, psSafetyRamStackTest);
        break;
    default:
        __asm("nop");
        break;
    }

    psSafetyCommon->ui32FastIsrSafetySwitch++;
    if(psSafetyCommon->ui32FastIsrSafetySwitch ==4)
        psSafetyCommon->ui32FastIsrSafetySwitch = 0;
}

/*!
* @brief   Handling with a safety error.
*
*          This function stores the code of recognized safety error into the dedicated RAM memory that is deleted only after POR.
*          If SAFETY_ERROR_ACTION macro is defined, interrupts are disabled and function waits for watchdog reset.
*
* @param   psSafetyCommon - The pointer of the Common Safety structure
*
* @return  None
*/
void SafetyErrorHandling(safety_common_t *psSafetyCommon)
{
    *SAFETY_ERROR_CODE = psSafetyCommon->ui32SafetyErrors;
#ifdef SAFETY_ERROR_ACTION
    __asm("CPSID i"); /* disable interrupts */
    while(1);
#endif
}
