/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* SPDX-License-Identifier: NXP Proprietary
* 
*
*******************************************************************************/

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

#define IEC60730B_ST_FLASH_PASS         0x0        /* Flash test pass return */
#define IEC60730B_ST_FLASH_FAIL         0x00000301 /* Flash test fail return */
#define IEC60730B_ST_FLASH_PROGRESS     0x00000302 /* Flash test inprogress  */

#define IEC60730B_CFG_PC_ADDR0 	        0x1FFFFFF8 /* #1 test address for Program counter test */
#define IEC60730B_CFG_PC_ADDR1 	        0x20000006 /* #2 test address for Program counter test */

/*! @brief Safety tests */
typedef struct _safety_common
{
    uint32_t ui32FastIsrSafetySwitch;
    uint32_t ui32SafetyErrors;
    uint32_t IEC60730B_clock_test_result;
    uint32_t IEC60730B_flash_test_result;
    uint32_t IEC60730B_ram_test_result;
    uint32_t IEC60730B_pc_test_result;
    uint32_t IEC60730B_cpu_primask_test_result;
    uint32_t IEC60730B_cpu_sp_main_test_result;
    uint32_t IEC60730B_cpu_reg_test_result;
    uint32_t IEC60730B_cpu_non_stacked_test_result;
    uint32_t IEC60730B_cpu_float_test_1_result;
    uint32_t IEC60730B_cpu_float_test_2_result;
    uint32_t IEC60730B_cpu_control_test_result;
    uint32_t IEC60730B_cpu_sp_process_test_result;
    uint32_t IEC60730B_cpu_special_test_result;
    uint32_t IEC60730B_stack_test_result;
    uint32_t IEC60730B_dio_input_test_result;
    uint32_t IEC60730B_dio_output_test_result;
    uint32_t IEC60730B_dio_short_test_result;
    uint32_t IEC60730B_aio_test_result;
    uint32_t ui32CpuClkFreq;
    uint32_t ui32McgirclkFreq;
    uint32_t ui32LpoFreq;
} safety_common_t;

/*! @brief Safety Watchdog test */
typedef struct _wd_test
{
    uint64_t ui64WdTestTemp1;
    uint32_t ui32WdTestExpected;
    uint32_t ui32WdTestTolerance;
    uint32_t ui32WdTestLptmrCnt;
    uint32_t ui32WdTestLimitHigh;
    uint32_t ui32WdTestLimitLow;
    uint32_t ui32WatchdogResets;
    uint32_t ui32WatchdogTimeoutCheck;
    uint16_t ui16WatchdogRefreshRatio;
} wd_test_t;

/*! @brief Safety Clock test */
typedef struct _clock_test
{
    uint32_t ui32ClockTestContext;
    uint32_t ui32ClockTestTolerance;
    uint32_t ui32ClockTestExpected;
    uint32_t ui32ClockTestLimitHigh;
    uint32_t ui32ClockTestLimitLow;
    uint32_t ui32SystickReloadValue;
    uint16_t ui16ClockTestStart;
} clock_test_t;

/*! @brief Safety RAM test */
typedef struct _ram_test
{
    uint32_t ui32RamTestStartAddress;
    uint32_t ui32RamTestEndAddress;
    uint32_t ui32BlockSize;
    uint32_t ui32ActualAddress;
    uint32_t ui32DefaultBlockSize;
    uint32_t ui32BackupAddress;
} ram_test_t;

/*! @brief Safety Flash test runtime */
typedef struct _flash_runtime_test_parameters
{
    uint32_t ui32BlockSize;         /* size of tested block */
    uint32_t ui32ActualAddress;     /* actual start address for crc module */
    uint32_t ui32PartCrc;           /* seed in begin, particular crc result in process, crc result in final*/
} flash_runtime_test_parameters_t;

/*! @brief Safety Flash test parameters */
typedef struct _flash_configuration_parameters
{
    uint32_t ui32Iec60730bStartConditionSeed;
    uint32_t ui32Iec60730bStartAddress;
    uint32_t ui32Iec60730bEndAddress;
    uint32_t ui32Iec60730bSize;
    uint32_t ui32Iec60730bBlockSize;
    uint32_t ui32Iec60730bChecksum;
} flash_configuration_parameters_t;

/*! @brief Safety Program Counter test */
typedef struct _pc_test
{
    uint32_t ui32AddrVal0;
    uint32_t ui32AddrVal1;
    uint32_t ui32AddrVal2;
    uint32_t ui32AddrVal3;
    uint32_t ui32AddrVal4;
    uint32_t ui32AddrVal5;
    uint32_t ui32AddrVal6;
    uint32_t ui32AddrVal7;
} pc_test_t;

/*! @brief Safety memory for Program Counter test */
typedef struct _pc_test_memory
{
    uint32_t ui32Address0; /* 0x1FFFFFF8*/
    uint32_t ui32Address1; /* 0x1FFFFFFC*/
    uint32_t ui32Address2; /* 0x20000000*/
    uint32_t ui32Address3; /* 0x20000004*/
    uint32_t ui32Address4; /* 0x20000008*/
    uint32_t ui32Address5; /* 0x2000000C*/
    uint32_t ui32Address6; /* 0x20000010*/
    uint32_t ui32Address7; /* 0x20000014*/
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
void SafetyWatchdogTest(wd_test_t *psSafetyWdTest);
void SafetyWatchdogRuntimeRefresh(wd_test_t *psSafetyWdTest);
void SafetyClockTestInit(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest);
void SafetyClockTestIsr(clock_test_t *psSafetyClockTest);
void SafetyClockTestCheck(safety_common_t *psSafetyCommon, clock_test_t *psSafetyClockTest);
void SafetyFlashTestInit(flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, flash_dcp_state_t *psFlashDCPState);
void SafetyFlashAfterResetTest(safety_common_t *psSafetyCommon, flash_configuration_parameters_t *psFlashConfig, flash_dcp_state_t *psFlashDCPState);
void SafetyFlashRuntimeTest(safety_common_t *psSafetyCommon, flash_runtime_test_parameters_t *psFlashCrc, flash_configuration_parameters_t *psFlashConfig, flash_dcp_state_t *psFlashDCPState);
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

void SafetyDigitalOutputTestRT(safety_common_t *psSafetyCommon, dio_test_rt_t *pTestedPin);
void SafetyDigitalInputOutput_ShortSupplyTestRT(safety_common_t *psSafetyCommon, dio_test_rt_t* pTestedPin, uint8_t polarity);
void SafetyDigitalInputOutput_ShortAdjTestRT(safety_common_t *psSafetyCommon, dio_test_rt_t* pTestedPin, dio_test_rt_t* pAdjPin, unsigned long PinValue);

void SafetyIsrFunction(safety_common_t *psSafetyCommon, ram_test_t *psSafetyRamTest, ram_test_t *psSafetyRamStackTest);
void SafetyErrorHandling(safety_common_t *psSafetyCommon);
void InitPackets(flash_dcp_state_t *psFlashDCPState);

#ifdef __cplusplus
}
#endif

#endif /* _SAFETY_CM4_CM7_IMXRT_H_ */
