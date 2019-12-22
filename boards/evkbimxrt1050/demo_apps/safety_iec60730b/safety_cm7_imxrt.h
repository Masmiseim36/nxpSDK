/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CM4_CM7_IMXRT_H_
#define _SAFETY_CM4_CM7_IMXRT_H_

#include "safety_config.h"
#include "iec60730b.h"

/*******************************************************************************
* Definitions
******************************************************************************/

#define CPU_TEST_ERROR                     0x1  /* CPU test fault flag */
#define FLASH_TEST_ERROR                   0x2  /* Flash test fault flag */
#define RAM_TEST_ERROR                     0x4  /* RAM test fault flag */
#define STACK_TEST_ERROR                   0x8  /* Stack test fault flag */
#define CLOCK_TEST_ERROR                  0x10  /* Clock test fault flag */
#define PC_TEST_ERROR                     0x20  /* Program counter test fault flag */
#define CPU_PRIMASK_ERROR                 0x40  /* PRIMASK test fault flag */
#define CPU_SP_MAIN_ERROR                 0x80  /* main SP test fault flag */
#define CPU_REGISTERS_ERROR              0x100  /* CPU registers test fault flag */
#define CPU_NONSTACKED_ERROR             0x200  /* non-stacked CPU test fault flag */
#define CPU_FLOAT_1_ERROR                0x400  /* floating point registers test fault flag */
#define CPU_FLOAT_2_ERROR                0x800  /* floating point registers test fault flag */
#define CPU_CONTROL_ERROR               0x1000  /* CONTROL register test fault flag */
#define CPU_SP_PROCESS_ERROR            0x2000  /* process SP test fault flag */
#define CPU_SPECIAL_ERROR               0x4000  /* special CPU registers test fault flag */
#define DIO_TEST_ERROR                  0x8000  /* DIO test fault flag */
#define AIO_TEST_ERROR                 0x10000  /* AIO test fault flag */

#define FS_FLASH_PASS         0x0        /* Flash test pass return */
#define FS_FLASH_FAIL         0x00000301 /* Flash test fail return */
#define FS_FLASH_PROGRESS     0x00000302 /* Flash test inprogress  */

#define IEC60730B_CFG_PC_ADDR0 	        0x1FFFFFF8 /* #1 test address for Program counter test */
#define IEC60730B_CFG_PC_ADDR1 	        0x20000006 /* #2 test address for Program counter test */

/*! @brief Safety tests */
typedef struct _safety_common
{
    uint32_t fastIsrSafetySwitch;
    uint32_t safetyErrors;
    uint32_t CLOCK_test_result;
    uint32_t FLASH_test_result;
    uint32_t RAM_test_result;
    uint32_t PC_test_result;
    uint32_t CPU_primask_test_result;
    uint32_t CPU_sp_main_test_result;
    uint32_t CPU_reg_test_result;
    uint32_t CPU_non_stacked_test_result;
    uint32_t CPU_control_test_result;
    uint32_t CPU_sp_process_test_result;
    uint32_t CPU_special_test_result;
    uint32_t CPU_float_test_1_result;
    uint32_t CPU_float_test_2_result;
    uint32_t STACK_test_result;
    uint32_t DIO_input_test_result;
    uint32_t DIO_output_test_result;
    uint32_t DIO_short_test_result;
    uint32_t refClkFreq;
} safety_common_t;

/*! @brief Safety Watchdog test */
typedef struct _wd_test
{
    uint64_t wdTestTemp1;
    uint32_t wdTestExpected;
    uint32_t wdTestTolerance;
    uint32_t wdTestLptmrCnt;
    uint32_t wdTestLimitHigh;
    uint32_t wdTestLimitLow;
    uint32_t watchdogResets;
    uint32_t watchdogTimeoutCheck;
    uint16_t watchdogRefreshRatio;
} wd_test_t;

/*! @brief Safety Clock test */
typedef struct _clock_test
{
    uint32_t clockTestContext;
    uint32_t clockTestTolerance;
    uint32_t clockTestExpected;
    uint32_t clockTestLimitHigh;
    uint32_t clockTestLimitLow;
    uint32_t systickReloadValue;
    uint16_t clockTestStart;
} clock_test_t;

/*! @brief Safety RAM test */
typedef struct _ram_test
{
    uint32_t ramTestStartAddress;
    uint32_t ramTestEndAddress;
    uint32_t blockSize;
    uint32_t actualAddress;
    uint32_t defaultBlockSize;
    uint32_t backupAddress;
} ram_test_t;

/*! @brief Safety Flash test runtime */
typedef struct _flash_runtime_test_parameters
{
    uint32_t blockSize;         /* size of tested block */
    uint32_t actualAddress;     /* actual start address for crc module */
    uint32_t partCrc;           /* seed in begin, particular crc result in process, crc result in final*/
} flash_runtime_test_parameters_t;

/*! @brief Safety Flash test parameters */
typedef struct _flash_configuration_parameters
{
    uint32_t startConditionSeed;
    uint32_t startAddress;
    uint32_t endAddress;
    uint32_t size;
    uint32_t blockSize;
    uint32_t checksum;
} flash_configuration_parameters_t;

#ifdef __cplusplus
extern "C" {
#endif

/*!
* @name Safety Class B tests handling functions
* @{
*/

/*******************************************************************************
* API
******************************************************************************/
void SafetyWatchdogTest(wd_test_t *psSafetyWdTest);
void SafetyWatchdogRuntimeRefresh(wd_test_t *psSafetyWdTest);
void SafetyClockTestInit(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest);
void SafetyClockTestIsr(clock_test_t *psSafetyClockTest);
void SafetyClockTestCheck(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest);
void SafetyFlashTestInit(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, fs_flash_dcp_state_t *psFlashDCPState);
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, flash_configuration_parameters_t *psFlashConfig, fs_flash_dcp_state_t *psFlashDCPState);
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon, flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, fs_flash_dcp_state_t *psFlashDCPState);
uint32_t SafetyFlashTestHandling( flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig);
void SafetyRamTestInit(ram_test_t *psSafetyRamTest, uint32_t *pui32SafetyRamStart, uint32_t *pui32SafetyRamEnd);
void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest);
void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest);
void SafetyPcTest(safety_common_t *psSafetyCommon, uint32_t pattern);
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon);
void SafetyCpuIsrTest(safety_common_t *psSafetyCommon);
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon);
void SafetyStackTestInit(void);
void SafetyStackTest(safety_common_t *psSafetyCommon);

void SafetyDIOTestInit(safety_common_t *psSafetyCommon, fs_dio_test_rt_t *pTestItems[]);
void SafetyDigitalOutputTest(safety_common_t *psSafetyCommon, fs_dio_test_rt_t *pTestedPin);
void SafetyDigitalInputOutput_ShortSupplyTest(safety_common_t *psSafetyCommon, fs_dio_test_rt_t *pTestedPin, uint8_t polarity);
void SafetyDigitalInputOutput_ShortAdjTest(safety_common_t *psSafetyCommon, fs_dio_test_rt_t *pTestedPin, fs_dio_test_rt_t *pAdjPin, uint32_t PinValue);

void SafetyIsrFunction(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest, ram_test_t *psSafetyRamStackTest);
void SafetyErrorHandling(safety_common_t *psSafetyCommon);
void InitPackets(fs_flash_dcp_state_t *psFlashDCPState);

#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_CM4_CM7_IMXRT_H_ */
