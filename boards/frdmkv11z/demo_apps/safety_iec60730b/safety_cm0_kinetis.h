/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_CM0_KINETIS_H_
#define _SAFETY_CM0_KINETIS_H_

#include "safety_config.h"

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
#define CPU_REGISTERS_ERROR               0x80  /* CPU registers test fault flag */
#define CPU_NONSTACKED_ERROR             0x100  /* non-stacked CPU test fault flag */
#define CPU_CONTROL_ERROR                0x200  /* CONTROL register test fault flag */
#define DIO_TEST_ERROR                   0x400  /* DIO test fault flag */
#define AIO_TEST_ERROR                   0x800  /* AIO test fault flag */
#define TSI_TEST_ERROR                  0x1000  /* TSI test fault flag */

#define FS_FLASH_PASS         0x0        /* Flash test pass return */
#define FS_FLASH_FAIL         0x00000301 /* Flash test fail return */
#define FS_FLASH_PROGRESS     0x00000302

#define SAFETY_CFG_PC_ADDR0 	        0x1FFFFFF8 /* #1 test address for Program counter test */
#define SAFETY_CFG_PC_ADDR1 	        0x20000006 /* #2 test address for Program counter test */

/*! @brief Safety tests */
typedef struct _safety_common
{
 uint32_t fastIsrSafetySwitch;
 uint32_t safetyErrors;
 uint32_t WDOG_test_result;
 uint32_t CLOCK_test_result;
 uint32_t FLASH_test_result;
 uint32_t RAM_test_result;
 uint32_t PC_test_result;
 uint32_t CPU_primask_test_result;
 uint32_t CPU_reg_test_result;
 uint32_t CPU_non_stacked_test_result;
 uint32_t CPU_control_test_result;
 uint32_t STACK_test_result;
 uint32_t DIO_input_test_result;
 uint32_t DIO_output_test_result;
 uint32_t DIO_short_test_result;
 uint32_t AIO_test_result;
 uint32_t TSI_test_result;
 uint32_t cpuClkFreq;
 uint32_t mcgirclkFreq;
 uint32_t lpoFreq;
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
 uint16_t checksum16;
 } flash_configuration_parameters_t;

/*! @brief Safety Program Counter test */
typedef struct _pc_test
{
 uint32_t addrVal0;
 uint32_t addrVal1;
 uint32_t addrVal2;
 uint32_t addrVal3;
 uint32_t addrVal4;
 uint32_t addrVal5;
 uint32_t addrVal6;
 uint32_t addrVal7;
} pc_test_t;

/*! @brief Safety memory for Program Counter test */
typedef struct _pc_test_memory
{
 uint32_t address0; /* 0x1FFFFFF8*/
 uint32_t address1; /* 0x1FFFFFFC*/
 uint32_t address2; /* 0x20000000*/
 uint32_t address3; /* 0x20000004*/
 uint32_t address4; /* 0x20000008*/
 uint32_t address5; /* 0x2000000C*/
 uint32_t address6; /* 0x20000010*/
 uint32_t address7; /* 0x20000014*/
} pc_test_memory_t;

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
void SafetyWatchdogTest(safety_common_t *psSafetyCommon, wd_test_t *psSafetyWdTest);
void SafetyWatchdogRuntimeRefresh(wd_test_t *psSafetyWdTest);
void SafetyClockTestInit(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest);
void SafetyClockTestIsr(clock_test_t *psSafetyClockTest);
void SafetyClockTestCheck(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest);
void SafetyFlashTestInit(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig);
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, flash_configuration_parameters_t *psFlashConfig);
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon, flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig);
uint32_t SafetyFlashTestHandling(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig);
void SafetyRamTestInit(ram_test_t *psSafetyRamTest, uint32_t *pSafetyRamStart, uint32_t *pSafetyRamEnd);
void SafetyRamAfterResetTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest);
void SafetyRamRuntimeTest(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest);
void SafetyPcTestInit(pc_test_t *psSafetyPcTest, pc_test_memory_t *psSafetyPcMemory);
void SafetyPcTest(safety_common_t *psSafetyCommon, uint32_t pattern);
void SafetyCpuAfterResetTest(safety_common_t *psSafetyCommon);
void SafetyCpuIsrTest(safety_common_t *psSafetyCommon);
void SafetyCpuBackgroundTest(safety_common_t *psSafetyCommon);
void SafetyStackTestInit(void);
void SafetyStackTest(safety_common_t *psSafetyCommon);

void SafetyDIOTestInit(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestItems[]);
#ifdef _MKE02Z4_H_
void SafetyDigitalInputTest(safety_common_t *psSafetyCommon, fs_dio_test_t* pTestedPin);
#endif
void SafetyDigitalOutputTest(safety_common_t *psSafetyCommon, fs_dio_test_t* pTestedPin);
void SafetyDigitalInputOutput_ShortSupplyTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin, uint8_t polarity );
void SafetyDigitalInputOutput_ShortAdjTest(safety_common_t *psSafetyCommon, fs_dio_test_t *pTestedPin, fs_dio_test_t *pAdjPin, uint32_t PinValue);


void SafetyAnalogTestInitialization(void);
void SafetyAnalogTest(safety_common_t *psSafetyCommon);


void tsi_port_clock_enable(void);
uint32_t SafetyTsiChanelTest(safety_common_t *psSafetyCommon, fs_tsi_t*pObj);

void SafetyIsrFunction(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest, ram_test_t *psSafetyRamStackTest);
void SafetyErrorHandling(safety_common_t *psSafetyCommon);


#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_CM0_KINETIS_H_ */
