/*! *********************************************************************************
* \defgroup PhyMessages PHY Messages
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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