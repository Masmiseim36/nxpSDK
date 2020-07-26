/*
 * NfcrdlibEx7_EMVCo_Polling.h
 *
 *  Created on: May 19, 2016
 *      Author: nxa05442
 */

#ifndef INTFS_NFCRDLIBEX7_EMVCO_POLLING_H_
#define INTFS_NFCRDLIBEX7_EMVCO_POLLING_H_

#include <ph_Status.h>

#define PRETTY_PRINTING                                         /**< Enable pretty printing */
#define MiN_VALID_DATA_SIZE                     6
#define PHAC_EMVCO_MAX_BUFFSIZE               600               /**< Maximum buffer size for Emvco. */
//#define RUN_TEST_SUIT

typedef enum{
    eEmdRes_EOT = 0x70,
    eEmdRes_SW_0 = 0x90,
    eEmdRes_SW_1 = 0x00,
}eEmvcoRespByte;

#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define EMVCO_POLLING_TASK_STACK              (2000/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define EMVCO_POLLING_TASK_STACK              (2000/4)
#else /*  __PN74XXXX__*/
#define EMVCO_POLLING_TASK_STACK              (2000)
#endif /*  __PN74XXXX__*/
#endif /*  PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION*/
#define EMVCO_POLLING_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define EMVCO_POLLING_TASK_STACK              0x20000
#define EMVCO_POLLING_TASK_PRIO               0
#endif /* PH_OSAL_LINUX */


#endif /* INTFS_NFCRDLIBEX7_EMVCO_POLLING_H_ */
