/*! *********************************************************************************
* \defgroup PhyMessages PHY Messages
* @{
********************************************************************************** */
/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _PHY_MESSAGES_H
#define _PHY_MESSAGES_H

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! 802.15.4-2006 standard PHY PD and PLME API messages */
typedef enum
{
    gPdDataReq_c,           /*!< Data Request */
    gPdDataCnf_c,           /*!< Data Confirm */
    gPdDataInd_c,           /*!< Data Indication */

    gPdIndQueueInsertReq_c, /*!< Insert checksum into indirect table */
    gPdIndQueueInsertCnf_c, /*!< Unused */
    gPdIndQueueRemoveReq_c, /*!< Remove checksum from indirect table */

    gPlmeCcaReq_c,          /*!< Clear Channel Assessment request */
    gPlmeCcaCnf_c,          /*!< Clear Channel Assessment confirm */

    gPlmeEdReq_c,           /*!< Channel Energy Detect request */
    gPlmeEdCnf_c,           /*!< Channel Energy Detect Confirm */

    gPlmeSetTRxStateReq_c,  /*!< Set Transceiver State request */
    gPlmeSetTRxStateCnf_c,  /*!< Set Transceiver State confirm */

    gPlmeSetReq_c,          /*!< Set PHY PIB request */
    gPlmeSetCnf_c,          /*!< Unused */

    gPlmeGetReq_c,          /*!< Get PHY PIB request */
    gPlmeGetCnf_c,          /*!< Unused */

    gPlmeTimeoutInd_c,      /*!< PHY sequence time-out indication */

    gPlme_StartEventInd_c,  /*!< PHY sequence start indication */
    gPlme_SyncLossInd_c,    /*!< PHY synchronisation lost indication */
    gPlme_RxSfdDetectInd_c, /*!< Receiver SFD indication */
    gPlme_FilterFailInd_c,  /*!< Receiver Filtering Fail indication */
    gPlme_UnexpectedRadioResetInd_c, /*!< XCVR reset */
}phyMessageId_t;

#endif  /* _PHY_MESSAGES_H */
/*! *********************************************************************************
* @}
********************************************************************************** */