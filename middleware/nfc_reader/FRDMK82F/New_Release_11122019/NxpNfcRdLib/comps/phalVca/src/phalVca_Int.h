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
* Internal functions of Virtual Card Architecture Application Component.
* $Author: nxp79567 $
* $Revision: 1833 $ (v06.01.00)
* $Date: 2016-05-24 12:21:08 +0530 (Tue, 24 May 2016) $
*
* History:
*  CHu: Generated 31. August 2009
*
*/

#ifndef PHALVCA_INT_H
#define PHALVCA_INT_H

#include <ph_Status.h>

/** \addtogroup ph_Private
* @{
*/

/**
* \name Virtual Card Architecture Commands
*/
/*@{*/
#define PHAL_VCA_CMD_PPC            0xF0U    /**< VCA Prepare Proximity Check command code. */
#define PHAL_VCA_CMD_PC             0xF2U    /**< VCA Proximity Check command code. */
#define PHAL_VCA_CMD_VPC            0xFDU    /**< VCA Verify Proximity Check command code. */
#define PHAL_VCA_CMD_VCS            0x42U    /**< VCA Virtual Card Support command code. */
#define PHAL_VCA_CMD_VCSL           0x4BU    /**< VCA Virtual Card Support Last command code. */
#define PHAL_VCA_CMD_SVC            0x40U    /**< VCA Select Virtual Card command code. */
#define PHAL_VCA_CMD_ISOSVC         0xA4U    /**< IsoSelect Virtual Card command code .*/
#define PHAL_VCA_CMD_ISOEXT_AUTH    0x82U    /**< IsoExternalAuthenticate Virtual Card command code. */
#define PHAL_VCA_CMD_DVC            0x48U    /**< VCA DeselectVC command code. */
/*@}*/

/**
* \name Virtual Card Architecture Response Codes
*/
/*@{*/
#define PHAL_VCA_RESP_ACK_ISO4          0x90U   /*< VCA ACK (in ISO14443-4 mode). */
#define PHAL_VCA_RESP_ERR_CMD_INVALID   0x0BU   /*< VCA Invalid Command Error. */
#define PHAL_VCA_RESP_ERR_FORMAT        0x0CU   /*< VCA Format Error. */
#define PHAL_VCA_RESP_ERR_GEN           0x0FU   /*< VCA Generic Error. */
#define PHAL_VCA_RESP_ERR_CMD_OVERFLOW  0x07U   /*< VCA CMD Overflow Error. */
#define PHAL_VCA_RESP_ERR_COMMAND_ABORTED 0xCAU  /*< VCA Command Aborted Error. */
/*@}*/

/**
* \name ISO 7816 VCA return Codes
*/
/*@{*/
#define PHAL_VCA_ISO7816_SUCCESS                        0x9000U /**< Correct execution. */
#define PHAL_VCA_ISO7816_PC_SUCCESS                     0x9190U /**< Correct execution for PC commands. */
#define PHAL_VCA_ISO7816_ERR_WRONG_LENGTH               0x6700U /**< Wrong length. */
#define PHAL_VCA_ISO7816_ERR_INVALID_APPLN              0x6A82U /**< Application / file not found. */
#define PHAL_VCA_ISO7816_ERR_WRONG_PARAMS               0x6A86U /**< Wrong parameters P1 and/or P2. */
#define PHAL_VCA_ISO7816_ERR_WRONG_LC                   0x6A87U /**< Lc inconsistent with P1/p2. */
#define PHAL_VCA_ISO7816_ERR_WRONG_LE                   0x6C00U /**< Wrong Le. */
#define PHAL_VCA_ISO7816_ERR_NO_PRECISE_DIAGNOSTICS     0x6F00U /**< No precise diagnostics. */
#define PHAL_VCA_ISO7816_ERR_EOF_REACHED                0x6282U /**< End of File reached. */
#define PHAL_VCA_ISO7816_ERR_FILE_ACCESS                0x6982U /**< File access not allowed. */
#define PHAL_VCA_ISO7816_ERR_FILE_EMPTY                 0x6985U /**< File empty or access conditions not satisfied. */
#define PHAL_VCA_ISO7816_ERR_FILE_NOT_FOUND             0x6A82U /**< File not found. */
#define PHAL_VCA_ISO7816_ERR_MEMORY_FAILURE             0x6581U /**< Memory failure (unsuccessful update). */
#define PHAL_VCA_ISO7816_ERR_INCORRECT_PARAMS           0x6B00U /**< Wrong parameter p1 or p2. READ RECORDS. */
#define PHAL_VCA_ISO7816_ERR_WRONG_CLA                  0x6E00U /**< Wrong Class byte. */
#define PHAL_VCA_ISO7816_ERR_UNSUPPORTED_INS            0x6D00U /**< Instruction not supported. */
#define PHAL_VCA_ERR_DF_7816_GEN_ERROR                  ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 13U) /**< VCA ISO 7816 Generic error. Check Additional Info. */
/*@}*/

#define PHAL_VCA_IID_SIZE       16U  /**< Size of the Installation Identifier. */
#define PHAL_VCA_CMD_SIZE       20U  /**< Size of the Virtual Card Command */
#define PHAL_VCA_PC_RND_LEN     7U   /**< Size of the Proximity Check Random numbers. */
#define PHAL_VCA_PC_RND_LEN_NEW 8U   /**< Size of the New Proximity Check Random numbers. */
#define PHAL_VCA_AUTH_RND_LEN   32U  /**< Size of Iso Select random challenge. */
#define PHAL_VCA_MAXWRAPPEDAPDU_SIZE   0x37u   /**< Maximum size of wrapped APDU 55 bytes. */
/**
* \brief Evaluate if the received response is erroneus.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_ComputeErrorResponse(
        uint16_t wNumBytesReceived,     /**< [In] Number of bytes received from the card. */
        uint8_t bStatus                 /**< [In] Status byte received from the card. */
    );

/**
* \brief Evaluate if the received response is erroneus.This is used if the error code needs to be set to VC param structure
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_ComputeErrorResponse_Extended(
        void * pDataParams,             /**< [In] Pointer to this layer's parameter structure. */
        uint16_t wStatus                /**< [In] Status byte received from the card. */
    );

/**
* \brief Perform a Prepare Proximity Check command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_PrepareProximityCheck(
        void * pPalMifareDataParams     /**< [In] Pointer to a palMifare component context. */
    );

#ifdef NXPBUILD__PHAL_VCA_SW
/**
* \brief Perform a Prepare Proximity Check new command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_PrepareProximityCheckNew(
        phalVca_Sw_DataParams_t * pDataParams,
        uint8_t *pOption,
        uint8_t *pPubRespTime,
        uint8_t *pPPS
    );
#endif /* NXPBUILD__PHAL_VCA_SW */

/**
* \brief Perform a Proximity Check command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_ProximityCheck(
        void * pPalMifareDataParams,  /**< [In] Pointer to a palMifare component context. */
        uint8_t bNumSteps,            /**< [In] Number of steps for the PC (1..7). */
        uint8_t * pRndC,              /**< [In] RndC[7]. */
        uint8_t * pRndRC              /**< [Out] RndRC[14]. */
    );

#ifdef NXPBUILD__PHAL_VCA_SW
/**
* \brief Perform a Proximity Check command.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_ProximityCheckNew(
        phalVca_Sw_DataParams_t * pDataParams,  /**< [In] Pointer to a palMifare component context. */
        uint8_t bNumSteps,                      /**< [In] Number of steps for the PC (1..7). */
        uint8_t * pPubRespTime,                 /**< [In] Published response time. */
        uint8_t * pRndC,                        /**< [In] RndC[7]. */
        uint8_t * pRndRC                        /**< [Out] RndRC[14]. */
    );

/**
* \brief Command to frame the Data complaint to ISO-7816 wrapped mode for Proximity Check commands
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval Other Depending on implementation and underlying component.
*/
phStatus_t phalVca_Int_SendISOWrappedCmd(
        phalVca_Sw_DataParams_t * pDataParams,
        uint8_t * pSendBuff,
        uint8_t  bLc,
        uint8_t ** pResponse,
        uint16_t * pRxlen
    );
#endif /* NXPBUILD__PHAL_VCA_SW */

/** @}
* end of ph_Private group
*/

#endif /* PHALVCA_INT_H */
