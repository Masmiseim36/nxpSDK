/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_NTAG_H
#define APP_NTAG_H

/*!
\file       app_ntag.h
\brief      app NTAG header file.
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include "EmbeddedTypes.h"
#include "FunctionLib.h"
#include "GPIO_Adapter.h"
#include "TimersManager.h"
#include "MemManager.h"
#include "fsl_os_abstraction.h"
#include "pin_mux.h"
#include "nfc_device.h"
#include "ntag_driver.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* I2C master base address */
#define NTAG_I2C_MASTER_BASEADDR  I2C1
/* I2C master clock source */
#define I2C_MASTER_CLK_SRC 	  I2C1_CLK_SRC

/* NTAG factory format support */
#ifndef APP_NTAG_SUPPORT_FACTORY_FORMAT_d
#define APP_NTAG_SUPPORT_FACTORY_FORMAT_d    0
#endif

/* NTAG full user memory read support */
#ifndef APP_NTAG_SUPPORT_FULL_MEM_READ_d
#define APP_NTAG_SUPPORT_FULL_MEM_READ_d    0
#endif

/* APP NTAG is running on ZigBee Coordinator */
#ifndef APP_NTAG_DEVICE_IS_COORD_d
#define APP_NTAG_DEVICE_IS_COORD_d    1
#endif

/* APP NTAG is running on ZigBee End Device (ZED) */
#ifndef APP_NTAG_DEVICE_IS_ZED_d
#define APP_NTAG_DEVICE_IS_ZED_d    0
#endif

#ifdef APP_NTAG_I2C_PLUS
/* Some extra checks */
#if (APP_NTAG_DEVICE_IS_ZED_d + APP_NTAG_DEVICE_IS_COORD_d > 1)
#error "The NTAG application cannot support both ZED and Coordinator at the same time"
#endif
#if (APP_NTAG_DEVICE_IS_ZED_d + APP_NTAG_DEVICE_IS_COORD_d == 0)
#warning "The NTAG I2C support is enabled, but the Zigbee device type is not specified in config.h"
#endif
#endif /* APP_NTAG_I2C_PLUS */

/* Field Detect interrupt support (mandatory enabled on coordinator) */
#if APP_NTAG_DEVICE_IS_COORD_d
#define APP_NTAG_FD_INTERRUPT_ENABLE_d    1 /* MANDATORY enabled on coordinator */
#elif APP_NTAG_DEVICE_IS_ZED_d
#define APP_NTAG_FD_INTERRUPT_ENABLE_d    0 /* ZED doesn't need FD interrupt */
#else
#define APP_NTAG_FD_INTERRUPT_ENABLE_d    0 /* disabled */
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/* APP NTAG events */
typedef enum
{
  NTAG_READY_c,
  NTAG_COMM_ERROR_c,
  NTAG_ICODE_VALID_c,
  NTAG_ICODE_INVALID_c
} APP_NTAG_Events_t;

/* Application callback function pointer type definition */
typedef void (*ptrAppCbk)(APP_NTAG_Events_t event);

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
/*! -------------------------------------------------------------------------
 * \brief Initialize the NTAG
 * \param[in] -
 * \return TRUE if init was successfully, FALSE otherwise
 *---------------------------------------------------------------------------*/
bool_t APP_NTAG_Init(void);

/*! -------------------------------------------------------------------------
 * \brief Retrieves the NTAG handle
 * \param[in] -
 * \return the NTAG handle
 *---------------------------------------------------------------------------*/
NFC_HANDLE_T APP_NTAG_GetHandle(void);

/*! -------------------------------------------------------------------------
 * \brief Registers a user callback
 * \param[in] the callback funtion to be registered and called
 * \return -
 *---------------------------------------------------------------------------*/
void APP_NTAG_RegisterCallback(ptrAppCbk cbk);

/*! -------------------------------------------------------------------------
 * \brief Retrieves the Install Code (iCode)
 * \param[in] -
 * \return the iCode
 *---------------------------------------------------------------------------*/
uint8_t* APP_NTAG_GetInstallCode(void);

/*! -------------------------------------------------------------------------
 * \brief Retrieves the device Extended Address
 * \param[in] -
 * \return pointer to Extended Address
 *---------------------------------------------------------------------------*/
uint64_t APP_NTAG_GetDeviceExtAddress(void);

/*! -------------------------------------------------------------------------
 * \brief NTAG full memory read (for debugging purposes)
 * \param[in] ntag_handle - handle to NTAG
 * \param[in] pData - pointer where the
 * \return -
 *---------------------------------------------------------------------------*/
void APP_NTAG_ReadMemoryFull(NFC_HANDLE_T ntag_handle, uint8_t* pData);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif /* APP_NTAG_H */