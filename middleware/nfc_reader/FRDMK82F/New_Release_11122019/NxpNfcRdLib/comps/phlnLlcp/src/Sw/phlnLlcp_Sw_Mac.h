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
* Software MAC Layer of LLCP Link layer Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 18. Feb 2015
*
*/

#ifndef PHLNLLCP_MAC_H
#define PHLNLLCP_MAC_H

/* *****************************************************************************************************************
*   Includes
* ***************************************************************************************************************** */
#include "phlnLlcp.h"

/* *****************************************************************************************************************
* Types/Structure Declarations
* ***************************************************************************************************************** */
#define PHLN_LLCP_ALLOWED_FRAMESIZE              0x03U   /**< FSL value for max. framesize of 254 Bytes. */
#define PHLN_LLCP_RWT_MIN_US                     302U    /**< Minimum response waiting time as per ISO/IEC 18092:2004(E) section 12.5.1.2.1. */

/* *****************************************************************************************************************
*   Function Prototypes
* ***************************************************************************************************************** */

/**
*
* @param[in] dwDevType either PHLN_LLCP_INITIATOR OR PHLN_LLCP_TARGET
*/
phStatus_t phlnLlcp_Sw_MacInit(phlnLlcp_Sw_DataParams_t * pDataParams, uint8_t bConfig, uint16_t *wSymmTime, uint16_t *wLtoTime);

void phlnLlcp_Sw_MacHAL_ShutDown(void);

phStatus_t phlnLlcp_MacTransmit(uint16_t wFrameOpt, uint8_t* pTxBuffer, uint16_t wTxLength);

phStatus_t phlnLlcp_MacReceive(uint16_t wFrameOpt, uint8_t **ppRxBuffer, uint16_t *pRxLength);

phStatus_t phlnLlcp_MacSetConfig(uint16_t wConfig, uint16_t wValue);

phStatus_t phlnLlcp_MacGetConfig(uint16_t wConfig, uint16_t* pwValue);

phStatus_t phlnLlcp_MacDeactivation(phlnLlcp_Sw_DataParams_t * pDataParams);

#endif /* PHLNLLCP_MAC_H */
