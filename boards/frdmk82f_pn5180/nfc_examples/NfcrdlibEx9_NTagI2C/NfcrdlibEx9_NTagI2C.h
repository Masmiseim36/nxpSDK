/*
 * NfcrdlibEx9_NTagI2C.h
 *
 *  Created on: May 19, 2016
 *      Author: nxa05442
 */

#ifndef INTFS_NFCRDLIBEX9_NTAGI2C_H_
#define INTFS_NFCRDLIBEX9_NTAGI2C_H_
#include <ph_Status.h>

#define DATA_BUFFER_LEN         16         /* Buffer length */

/* Tag Version info macros */
#define NTAG_I2C_1K_VERSION     0x13       /* Tag Version: 1k */
#define NTAG_I2C_2K_VERSION     0x15       /* Tag Version: 2k */

#define PTHRU_ON_OFF            (1 << 6)   /* NC_REG Pass-through bit */

#ifdef PH_OSAL_FREERTOS
#ifdef PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION
#define NTAGI2C_TASK_STACK              (1200/4)
#else /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#ifdef __PN74XXXX__
#define NTAGI2C_TASK_STACK              (1200/4)
#else /*  __PN74XXXX__*/
#define NTAGI2C_TASK_STACK              (1200)
#endif /*  __PN74XXXX__*/
#endif /* PHOSAL_FREERTOS_STATIC_MEM_ALLOCATION */
#define NTAGI2C_TASK_PRIO                4
#endif /* PH_OSAL_FREERTOS */

#ifdef PH_OSAL_LINUX
#define NTAGI2C_TASK_STACK                0x20000
#define NTAGI2C_TASK_PRIO                 0
#endif /* PH_OSAL_LINUX */


#endif /* INTFS_NFCRDLIBEX9_NTAGI2C_H_ */
