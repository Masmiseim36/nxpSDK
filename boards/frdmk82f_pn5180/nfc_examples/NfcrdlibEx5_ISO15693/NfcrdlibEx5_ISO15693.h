/*
 * NfcrdlibEx5_ISO15693.h
 *
 *  Created on: May 23, 2016
 *      Author: nxa05442
 */

#ifndef INTFS_NFCRDLIBEX5_ISO15693_H_
#define INTFS_NFCRDLIBEX5_ISO15693_H_
#include <ph_Status.h>


#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define ISO15693_DEMO_TASK_STACK              (1200/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define ISO15693_DEMO_TASK_STACK              (1200/4)
#else /* __PN74XXXX__*/
#define ISO15693_DEMO_TASK_STACK              (1200)
#endif /* __PN74XXXX__*/
#endif
#define ISO15693_DEMO_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define ISO15693_DEMO_TASK_STACK                0x20000
#define ISO15693_DEMO_TASK_PRIO                 0
#endif /* PH_OSAL_LINUX */



#endif /* INTFS_NFCRDLIBEX5_ISO15693_H_ */
