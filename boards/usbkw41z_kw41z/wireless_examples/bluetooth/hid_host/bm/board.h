/*
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_gpio.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* The board name */
#define MANUFACTURER_NAME               "NXP"
#define BOARD_NAME                      "USB-KW41Z"


#define CORE_CLOCK_FREQ 47972352U

/* Connectivity */
#ifndef APP_SERIAL_INTERFACE_TYPE
#define APP_SERIAL_INTERFACE_TYPE (gSerialMgrLpuart_c)
#endif

#ifndef APP_SERIAL_INTERFACE_INSTANCE
#define APP_SERIAL_INTERFACE_INSTANCE (0)
#endif

#ifndef APP_SERIAL_INTERFACE_SPEED
#define APP_SERIAL_INTERFACE_SPEED (115200)
#endif

/* EEPROM type used */
#ifndef gEepromType_d
#define gEepromType_d gEepromDevice_InternalFlash_c
#endif

/* DCDC driver default mode */
#ifndef APP_DCDC_MODE
#define APP_DCDC_MODE    (gDCDC_Mode_Buck_c)
#endif
   
/* Vbat monitor Interval in Ms */
#ifndef APP_DCDC_VBAT_MONITOR_INTERVAL
#define APP_DCDC_VBAT_MONITOR_INTERVAL    (50000)
#endif
   
/* 1<<x to enable PINx as wakup source in LLWU */
#define BOARD_LLWU_PIN_ENABLE_BITMAP ( 1<<12 | 1<<13 )


#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

void hardware_init(void);

/* Function to initialize ADC on board configuration. */
void BOARD_InitAdc(void);

/* Function to initialize DCDC on board configuration. */
void BOARD_DCDCInit(void);

/* Function to read battery level on board configuration. */
uint8_t BOARD_GetBatteryLevel(void);

/* Function to read potentiometer level on board configuration. */
uint16_t BOARD_GetPotentiometerLevel(void);

/* Function to read temperature level on board configuration. */
int32_t BOARD_GetTemperature(void);

/* Function to install callbacks for actions before and after entering low power. */
extern void BOARD_InstallLowPowerCallbacks(void);

/* Function called by low power module prior to entering low power. */
extern void BOARD_EnterLowPowerCb(void);

/* Function called by low power module after exiting low power. */
extern void BOARD_ExitLowPowerCb(void);

/* Function called by the BLE connection manager to generate PER MCU keys */
extern void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen);

/* Functions used to determine the frequency of a module's input clock. */
uint32_t BOARD_GetLpuartClock(uint32_t instance);
uint32_t BOARD_GetTpmClock(uint32_t instance);
uint32_t BOARD_GetSpiClock(uint32_t instance);
uint32_t BOARD_GetI2cClock(uint32_t instance);
void BOARD_BLPEtoBLPI(void);
void BOARD_BLPItoBLPE(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __BOARD_H__ */
