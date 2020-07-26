/*
*                     Copyright 2016-2017,2019 NXP
*
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
* Interface file of Operating System Abstraction Layer for FreeRTOS.
* $Author$
* $Revision$
* $Date$
*/

#ifndef PHOSAL_FREERTOS_H
#define PHOSAL_FREERTOS_H

#include <FreeRTOS.h>

#define PHOSAL_FREERTOS_ALL_EVENTS      0x00FFFFFF

#define PHOSAL_MAX_DELAY      portMAX_DELAY

#if( configSUPPORT_STATIC_ALLOCATION == 1 )
    #define PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */


/**
 * xPortIsInsideInterrupt is required by FreeRTOS OSAL wrapper. FreeRTOS port provides xPortIsInsideInterrupt definition for
 * Cortex-M3, M4 only. For cortex-M0 xPortIsInsideInterrupt is implemented here.
 */
#if (defined(CORE_M0) || defined(__PN74XXXX__) || defined (__PN73XXXX__))

/*Interrupt Control and State Register*/
#define PHOSAL_NVIC_INT_CTRL        ( (volatile uint32_t *) 0xe000ed04 )
#define PHOSAL_NVIC_VECTACTIVE      (0x0000003FU)

#define xPortIsInsideInterrupt()    (((*(PHOSAL_NVIC_INT_CTRL) & PHOSAL_NVIC_VECTACTIVE ) == 0)? pdFALSE : pdTRUE)

#endif /* (defined(CORE_M0) || defined(__PN74XXXX__) || defined (__PN73XXXX__)) */



#endif /* PHOSAL_FREERTOS_H */
