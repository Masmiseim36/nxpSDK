/*
*                     Copyright 2019, NXP
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
* ISO10373 PCD Application interface.
*
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef INTFS_NFCRDLIBEX11_ISO10373_PCD_H_
#define INTFS_NFCRDLIBEX11_ISO10373_PCD_H_
#include <ph_Status.h>

#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define ISO10373_PCD_TASK_STACK              (2000/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define ISO10373_PCD_TASK_STACK              (2000/4)
#else /*  __PN74XXXX__*/
#define ISO10373_PCD_TASK_STACK              (2000)
#endif /*  __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define ISO10373_PCD_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define ISO10373_PCD_TASK_STACK                0x20000
#define ISO10373_PCD_TASK_PRIO                 0
#endif /* PH_OSAL_LINUX */

#endif /* INTFS_NFCRDLIBEX11_ISO10373_PCD_H_ */
