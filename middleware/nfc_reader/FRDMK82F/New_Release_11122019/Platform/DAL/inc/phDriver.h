/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Generic phDriver Component of Reader Library Framework.
* $Author$
* $Revision$
* $Date$
*
* History:
*  RS: Generated 24. Jan 2017
*
*/


#ifndef PHDRIVER_H
#define PHDRIVER_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */


/** \defgroup phDriver Driver Abstraction Layer (DAL)
*
* \brief This component implements hardware drivers that are necessary for RdLib software modules
* @{
*/

#ifdef linux
#include <sys/types.h>
#endif

#if defined (PH_TYPEDEFS_H)
#else /*PH_TYPEDEFS_H*/

#if defined(__GNUC__ ) /* Toolchain with StdInt */
#    include <stdint.h>
#elif defined(__ICCARM__)
#   include "intrinsics.h"
#   include <stdint.h>
#elif defined(__CC_ARM)
#   include <stdint.h>
#elif defined(_WIN32)
#   include <stdint.h>
#endif


/** \name Floating-Point Types
*/
/*@{*/
#ifndef __float32_t_defined
/**
* \brief 32 bit floating point
*/
typedef float float32_t;
#endif
/*@}*/

#endif /*PH_TYPEDEFS_H*/

#ifndef  PH_STATUS_H
/**
* \brief phStatus_t is a signed short value, using the positive range.
*
* High byte: Category (group) Identifier.\n
* Low byte : Error Specifier.
*/
typedef uint16_t phStatus_t;

#define PH_COMP_DRIVER              0xF100U /**< DRIVER component code. */
#define PH_COMP_MASK                0xFF00U /**< Component Mask for status code and component ID. */
#define PH_COMPID_MASK              0x00FFU /**< ID Mask for component ID. */
#define PH_ERR_MASK                 0x00FFU /**< Error Mask for status code. */
#define PH_COMP_GENERIC             0x0000U /**< Generic Component Code. */

#ifndef NULL
#    define NULL 0
#endif
#endif


#include "phbalReg.h"
#include "phDriver_Gpio.h"
#include "phDriver_Timer.h"

/********************************************************************************
 * Critical Section Management API's
 *******************************************************************************/

/**
 * \brief Enter Critical section.
 */
void phDriver_EnterCriticalSection(void);

/**
 * \brief Exit Critical section.
 */
void phDriver_ExitCriticalSection(void);

/*!
  \brief Generic Status codes

  All functions within the phDriver use these macro values as return codes.
*/
#define PH_DRIVER_SUCCESS                 0x0000U /**< Function executed successfully. */
#define PH_DRIVER_TIMEOUT                 0x0001U /**< No reply received, e.g. PICC removal. */
#define PH_DRIVER_ABORTED                 0x0012U /**< Used when HAL ShutDown is called. */
#define PH_DRIVER_ERROR                   0x0080U /**< Invalid Parameter, buffer overflow  or other configuration error. */
#define PH_DRIVER_FAILURE                 0x0081U /**< Failed to perform the requested operation. */

/** @}
* end of phDriver Driver Abstraction Layer (DAL)
*/

#ifdef __cplusplus
}/*Extern C*/
#endif

#endif /* PHDRIVER_H */
