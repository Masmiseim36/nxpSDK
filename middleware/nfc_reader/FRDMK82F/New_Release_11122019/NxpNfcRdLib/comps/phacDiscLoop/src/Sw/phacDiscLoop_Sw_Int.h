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
* This File contains all support functions that's needed by Discovery Loop
* for performing activities defined by NFC Forum
* as part of the Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  PC: Generated 23. Aug 2012
*
*/
#ifndef PHACDICLOOP_SW_INT_H
#define PHACDICLOOP_SW_INT_H

#include <ph_Status.h>
#include <phNxpNfcRdLib_Config.h>

#ifdef NXPBUILD__PHAC_DISCLOOP_SW

#define PHAC_DISCLOOP_POLL_MAX_NFC_TECHS_SUPPORTED            0x05U              /**< Maximum NFC Technologies support in Poll Mode. */
#define PHAC_DISCLOOP_ACT_POLL_MAX_TECHS_SUPPORTED            0x03U              /**< Maximum NFC Technologies support in Active Poll Mode. */

/* Time extension to support EMVCO Requirement
* Emvco: case_id TB001
* measured delay between WUPA sequence and WUPB sequence
* during polling is less than t_P_MIN = 5100 microsecs */
#define PHAC_DISCLOOP_DELTA_TP_MIN                            200U
/**
 * Single technology detected.
 *
 * This status is used in passive poll mode when a single NFC or proprietary
 * technology is detected during technology detection stage.
 * */
#define PHAC_DISCLOOP_TECH_DETECTED                         0x0087U

/**
 * Single device/card resolved.
 *
 * This status is used in passive poll mode when a single NFC or proprietary
 * card/device is detected during technology detection stage and the same got
 * resolved in collision resolution phase.
 * */
#define PHAC_DISCLOOP_DEVICE_RESOLVED                       0x0089U

typedef phStatus_t (*pphacDiscLoop_Sw_DetTechs) (phacDiscLoop_Sw_DataParams_t *pDataParams );
typedef phStatus_t (*pphacDiscLoop_Sw_Reslns)(phacDiscLoop_Sw_DataParams_t *pDataParams);
typedef phStatus_t (*pphacDiscLoop_Sw_DeviceActivate)(phacDiscLoop_Sw_DataParams_t *pDataParams, uint8_t bIndex);

phStatus_t phacDiscLoop_Sw_Int_ListenMode(
    phacDiscLoop_Sw_DataParams_t *pDataParams
    );

phStatus_t phacDiscLoop_Sw_Int_Stop(
    phacDiscLoop_Sw_DataParams_t *pDataParams
    );

phStatus_t phacDiscLoop_Sw_Int_PollMode(
    phacDiscLoop_Sw_DataParams_t *pDataParams
    );

phStatus_t phacDiscLoop_Sw_Int_EmvcoRemovalProcedure(
    phacDiscLoop_Sw_DataParams_t * pDataParams
    );

phStatus_t phacDiscLoop_Sw_Int_ActivePollMode(
    phacDiscLoop_Sw_DataParams_t *pDataParams
    );

phStatus_t phacDiscLoop_Sw_Int_TechDetectActivity(
    phacDiscLoop_Sw_DataParams_t *pDataParams,
    uint8_t bPasPollBailOut,
    uint8_t bDetectConfig,
    uint8_t* pNumOfTechsDetect
    );

phStatus_t phacDiscLoop_Sw_Int_EmvcoTechDetectActivity(
    phacDiscLoop_Sw_DataParams_t *pDataParams,
    uint8_t bDetectConfig,
    uint8_t* pNumOfTechsDetect
    );

phStatus_t phacDiscLoop_Sw_Int_ColsnReslnActivity(
    phacDiscLoop_Sw_DataParams_t *pDataParams,
    uint8_t bTechType
    );

phStatus_t phacDiscLoop_Sw_Int_ActivateDevice(
    phacDiscLoop_Sw_DataParams_t * pDataParams,
    uint8_t bTechType,
    uint8_t bTagIndex
    );

phStatus_t phacDiscLoop_Sw_Int_RfcaOff_FieldOn(
    phacDiscLoop_Sw_DataParams_t * pDataParams
    );

phStatus_t phacDiscLoop_Sw_Int_FieldOn(
    phacDiscLoop_Sw_DataParams_t * pDataParams
    );

uint8_t phacDiscLoop_Sw_Int_IsValidPollStatus(
    phStatus_t wStatus
    );
#endif /* NXPBUILD__PHAC_DISCLOOP_SW */
#endif /* PHACDICLOOP_H */
