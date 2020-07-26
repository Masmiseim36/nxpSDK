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
* Software LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 20. Feb 2015
*
*/

#ifndef PHLNLLCP_TIMERS_H
#define PHLNLLCP_TIMERS_H

/* *****************************************************************************************************************
*   Includes
* ***************************************************************************************************************** */
#include "phlnLlcp.h"

/* *****************************************************************************************************************
* MACROS/Defines
* ***************************************************************************************************************** */
#define PHLN_LLCP_TIMERS_SYM_ID     0x00001001UL
#define PHLN_LLCP_TIMERS_LTO_ID     0x00001002UL

#define PHLN_LLCP_LTO_DELTA_PERCENTAGE        30U               /**< Delta milli secs as guard time for sw overheads. */
/**
* Delta in percentage to be reduced from LTO for calculating SYMM timeout.
* PHLN_LLCP_SYMM_VALUE is a percentage of LTO value, LTO value is configured by the user.
*/
#define PHLN_LLCP_SYMM_DELTA_PERCENTAGE       (100U - PHLN_LLCP_SYMM_VALUE)

/* *****************************************************************************************************************
*   Extern Variables
* ***************************************************************************************************************** */

/* *****************************************************************************************************************
*   Function Prototypes
* ***************************************************************************************************************** */

typedef void (*phlnLlcpTimer_notify_function_t)(void * arg);

phStatus_t phlnLlcp_Timers_InitSym(phlnLlcp_Sw_DataParams_t * pDataParams, phlnLlcpTimer_notify_function_t pCB);
phStatus_t phlnLlcp_Timers_InitLto(phlnLlcp_Sw_DataParams_t * pDataParams, phlnLlcpTimer_notify_function_t pCB);
phStatus_t phlnLlcp_Timers_DeInitSym(phlnLlcp_Sw_DataParams_t * pDataParams);
phStatus_t phlnLlcp_Timers_DeInitLto(phlnLlcp_Sw_DataParams_t * pDataParams);

phStatus_t phlnLlcp_Timers_StopLto(phlnLlcp_Sw_DataParams_t * pDataParams);

void phlnLlcp_SymTimerCallback(void * arg);
void phlnLlcp_LtoTimerCallback(void * arg);

phStatus_t phlnLlcp_Timers_LtoStart(phlnLlcp_Sw_DataParams_t * pDataParams);
phStatus_t phlnLlcp_Timers_SymStart(phlnLlcp_Sw_DataParams_t * pDataParams);

/**
*phStatus_t phlnLlcp_Timers_SymStop(void);
*/
//#define phlnLlcp_Timers_SymStop()  phOsal_Timer_Stop(xSymHandle)
#define phlnLlcp_Timers_SymStop(pDataParams)  phOsal_TimerStop(&pDataParams->LlcpSymTimerObj.TimerHandle)

/**
*phStatus_t phlnLlcp_Timers_LtoStop(void);
*/
//#define phlnLlcp_Timers_LtoStop()  phOsal_Timer_Stop(xLtoHandle)
#define phlnLlcp_Timers_LtoStop(pDataParams)  phOsal_TimerStop(&pDataParams->LlcpLtoTimerObj.TimerHandle)

#endif /* PHLNLLCP_TIMERS_H */
