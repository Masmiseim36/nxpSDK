/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"

#include "EmbeddedTypes.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "USBKW38"
#define MANUFACTURER_NAME               "NXP"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_LPUART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART0
#define BOARD_DEBUG_UART_INSTANCE 0U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_Osc0ErClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetOsc0ErClkFreq()
#define BOARD_UART_IRQ LPUART0_LPUART1_IRQn
/*TBD*/
#define BOARD_UART_IRQ_HANDLER LPUART0_LPUART1_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/* Vbat monitor Interval in Ms */
#ifndef APP_DCDC_VBAT_MONITOR_INTERVAL
#define APP_DCDC_VBAT_MONITOR_INTERVAL    (50000)
#endif

/* Connectivity */
#ifndef APP_SERIAL_INTERFACE_TYPE
#define APP_SERIAL_INTERFACE_TYPE (gSerialMgrLpuart_c)
#endif

/* EVAL use LPUART0 for HCI */
#ifndef APP_SERIAL_INTERFACE_INSTANCE
#define APP_SERIAL_INTERFACE_INSTANCE (0)
#endif

#ifndef APP_SERIAL_INTERFACE_SPEED
#define APP_SERIAL_INTERFACE_SPEED (115200)
#endif

#ifndef gStackTimerInstance_c
#define gStackTimerInstance_c           1
#endif
#ifndef gRedLedPwmTimerInstance_c
#define gRedLedPwmTimerInstance_c       0
#endif
#ifndef gGreenLedPwmTimerInstance_c
#define gGreenLedPwmTimerInstance_c     2
#endif
#ifndef gBlueLedPwmTimerInstance_c
#define gBlueLedPwmTimerInstance_c      2
#endif

#ifndef gRedLedPwmTimerChannel_c
#define gRedLedPwmTimerChannel_c        2
#endif
#ifndef gGreenLedPwmTimerChannel_c
#define gGreenLedPwmTimerChannel_c      1
#endif
#ifndef gBlueLedPwmTimerChannel_c
#define gBlueLedPwmTimerChannel_c       0
#endif
/* Index into the ledPins[] array */
#ifndef gRedLedIdx_c
#define gRedLedIdx_c                    0
#endif

/* Debug MACRO stubbed if not defined */
#ifndef BOARD_DBGLPIOSET
#define BOARD_DBGLPIOSET(__x, __y)
#endif

#ifndef BOARD_DBGINITSET
#define BOARD_DBGINITSET(__x, __y)
#endif

#ifndef BOARD_DBGINITDBGIO
#define BOARD_DBGINITDBGIO()
#endif

#ifndef BOARD_DBGTOGGLEDBGIO
#define BOARD_DBGTOGGLEDBGIO()
#endif

#ifndef BOARD_DBGSETDBGIOFORLOWPOWER
#define BOARD_DBGSETDBGIOFORLOWPOWER()
#endif

#ifndef BOARD_DBGCONFIGINIT
#define BOARD_DBGCONFIGINIT(__x)
#endif

#ifndef APP_DBG_LOG
#define APP_DBG_LOG(...)
#endif

#ifndef PWR_DBG_LOG
#define PWR_DBG_LOG(...)
#endif

#ifndef MEM_DBG_LOG
#define MEM_DBG_LOG(...)
#endif

#ifndef DBG_LOG_DUMP
#define DBG_LOG_DUMP(__x)
#endif



#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
void hardware_init(void);

/* Functions used to determine the frequency of a module's input clock. */
uint32_t BOARD_GetLpuartClock(uint32_t instance);
uint32_t BOARD_GetTpmClock(uint32_t instance);
uint32_t BOARD_GetSpiClock(uint32_t instance);
uint32_t BOARD_GetI2cClock(uint32_t instance);
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen);
void BOARD_GetMACAddr(uint8_t* aOutMacAddr5B);
void BOARD_DCDCInit(void);
void BOARD_InitDebugConsole(void);
void BOARD_InitAdc(void);
uint8_t BOARD_GetBatteryLevel(void);
uint16_t BOARD_GetPotentiometerLevel(void);
int32_t BOARD_GetTemperature(void);
uint16_t BOARD_GetAdcBatLvl(void);
uint16_t BOARD_GetAdcBgLvl(void);
void BOARD_BLPEtoBLPI(void);
void BOARD_BLPItoBLPE(void);

/* Function to install callbacks for actions before and after entering low power. */
extern void BOARD_InstallLowPowerCallbacks(void);

/* Function called by low power module prior to entering low power. */
extern void BOARD_EnterLowPowerCb(void);

/* Function called by low power module after exiting low power. */
extern void BOARD_ExitLowPowerCb(void);

/* Function to install callback for actions after exiting VLLS low power mode. */
extern void BOARD_InstallWarmbootCallback(void);

extern void BOARD_InitDebugConsole(void);
void BOARD_DeinitLPUART(void);
void BOARD_DeinitSPI(void);

extern uint8_t BOARD_GetXtal32MhzTrim(bool_t regRead);
extern void BOARD_SetXtal32MHzTrim(uint8_t trimValue, bool_t saveToHwParams);
extern uint8_t BOARD_GetRssiAdjustment(bool_t regRead);
extern void BOARD_SetRssiAdjustment(uint8_t newRssiAdjustmentValue, bool_t saveToHwParams);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
