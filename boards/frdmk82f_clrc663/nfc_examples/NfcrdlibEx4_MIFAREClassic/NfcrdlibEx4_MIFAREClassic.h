/*
*                     Copyright 2016-2019, NXP
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

#ifndef INTFS_NFCRDLIBEX4_MIFARECLASSIC_H_
#define INTFS_NFCRDLIBEX4_MIFARECLASSIC_H_
#include <ph_Status.h>

#ifdef PH_OSAL_FREERTOS

#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define MIFARECLASSIC_TASK_STACK              (1200/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define MIFARECLASSIC_TASK_STACK              (1200/4)
#else /* __PN74XXXX__*/
#define MIFARECLASSIC_TASK_STACK              1200
#endif /* __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */

#define MIFARECLASSIC_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define MIFARECLASSIC_TASK_STACK                0x20000
#define MIFARECLASSIC_TASK_PRIO                 0
#endif /* PH_OSAL_LINUX */


#endif /* INTFS_NFCRDLIBEX4_MIFARECLASSIC_H_ */
