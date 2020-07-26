/*
 * NfcrdlibEx1_BasicDiscoveryLoop.h
 *
 *  Created on: May 19, 2016
 *      Author: nxa05442
 */
#ifndef INTFS_NFCRDLIBEX1_BASICDISCOVERYLOOP_H_
#define INTFS_NFCRDLIBEX1_BASICDISCOVERYLOOP_H_

#include <ph_Status.h>

#if defined (NXPBUILD__PHHAL_HW_PN5180)   || \
    defined (NXPBUILD__PHHAL_HW_RC663)    || \
    defined (NXPBUILD__PHHAL_HW_PN7462AU)
#define	PH_EXAMPLE1_LPCD_ENABLE				/* If LPCD needs to be configured and used over HAL or over DiscLoop */
#endif

#define LISTEN_PHASE_TIME_MS              300                 /* Listen Phase TIME */

#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define BASIC_DISC_DEMO_TASK_STACK              (1600/4)
#else
#ifdef __PN74XXXX__
#define BASIC_DISC_DEMO_TASK_STACK              (1600/4)
#else /* __PN74XXXX__*/
#define BASIC_DISC_DEMO_TASK_STACK              (1600)
#endif /* __PN74XXXX__*/
#endif
#define BASIC_DISC_DEMO_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define BASIC_DISC_DEMO_TASK_STACK                  0x20000
#define BASIC_DISC_DEMO_TASK_PRIO                 0
#endif /* PH_OSAL_LINUX */

#endif /* INTFS_NFCRDLIBEX1_BASICDISCOVERYLOOP_H_ */
