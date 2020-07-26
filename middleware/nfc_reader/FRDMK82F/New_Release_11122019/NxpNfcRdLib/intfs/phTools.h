/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
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
* Generic Tools Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  CHu: Generated 19. May 2009
*
*/

#ifndef PHTOOLS_H
#define PHTOOLS_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

/** \defgroup phTools Tools
*
* \brief This component implements tools that are necessary for many different software modules
* @{
*/

/**
* \name ToolsConfigs
*/
/*@{*/
#define PH_TOOLS_CRC16_PRESET_ISO14443A 0x6363U     /**< CRC16 Preset for ISO14443-A. */
#define PH_TOOLS_CRC16_PRESET_ISO14443B 0xFFFFU     /**< CRC16 Preset for ISO14443-B. */
#define PH_TOOLS_CRC16_PRESET_FELICA    0x0000U     /**< CRC16 Preset for FeliCa. */
#define PH_TOOLS_CRC16_POLY_ISO14443    0x8408U     /**< CRC16 Polynom for ISO14443. */
#define PH_TOOLS_CRC32_PRESET_DF8       0xFFFFFFFFU /**< CRC32 Preset for MIFARE DESFire. */
#define PH_TOOLS_CRC32_POLY_DF8         0xEDB88320U /**< CRC32 Polynom for MIFARE DESFire. */
#define PH_TOOLS_CRC8_PRESET_EPC        0xFFU       /**< Preset value for EPC CRC-8 generation. */
#define PH_TOOLS_CRC8_PRESET_UID        0xFDU       /**< Preset value for UID CRC-8 generation. */
#define PH_TOOLS_CRC8_POLY_EPCUID       0x1DU       /**< Polynomial for EPC/UID CRC-8 generation: x^8 + x^4 + x^3 + x^2 + 1 (MSB first). */
#define PH_TOOLS_CRC16_PRESET_EPCUID    0xFFFFU     /**< Preset value for EPC/UID CRC-16 generation. */
#define PH_TOOLS_CRC16_POLY_EPCUID      0x1021U     /**< Polynomial for EPC/UID CRC-16 generation: x^16 + x^12 + x^5 + 1 (MSB first). */
#define PH_TOOLS_CRC5_PRESET_I18000P3   0x09U       /**< Preset value for ISO18000-3 CRC-5 generation. */
#define PH_TOOLS_CRC5_POLY_I18000P3     0x09U       /**< Polynomial for ISO18000-3 CRC-5 generation: x^5 + x^3 + 1 (LSB first). */
#define PH_TOOLS_CRC16_PRESET_I18000P3  0xFFFFU     /**< Preset value for ISO18000-3 CRC-16 generation. */
#define PH_TOOLS_CRC16_POLY_I18000P3    0x8408U     /**< Polynomial for ISO18000-3 CRC-16 generation: x^16 + x^12 + x^5 + 1 (LSB first). */
#define PH_TOOLS_CRC16_POLY_FELICA      0x1021U     /**< Polynomial for FeliCa CRC-16 generation: x^16 + x^12 + x^5 + 1 (MSB first). */
#define PH_TOOLS_PARITY_OPTION_EVEN         0x00U   /**< Encode/Decode even parity bits. */
#define PH_TOOLS_PARITY_OPTION_ODD          0x01U   /**< Encode/Decode odd parity bits. */
#define PH_TOOLS_CRC16_POLY_PN7462AU_TESTOS    0x1021U /**< Polynomial for TestOS CRC-16 generation: x^16 + x^12 + x^5 + 1 (MSB first). */

#define PH_TOOLS_CRC_OPTION_DEFAULT         0x00U   /**< Default CRC operation. */
#define PH_TOOLS_CRC_OPTION_OUPUT_INVERTED  0x01U   /**< Inverts the bits after calculation. */
#define PH_TOOLS_CRC_OPTION_MSB_FIRST       0x02U   /**< Calculates CRC with MSB(it) first. */
#define PH_TOOLS_CRC_OPTION_BITWISE         0x04U   /**< Bitwise CRC, \c wDataLength represents the Bitlength. */
#define PH_TOOLS_CRC_OPTION_MASK            0x07U   /**< Mask of valid option bits. */
/*@}*/

/**
* \name Q Configs
*/
/*@{*/
#define PH_TOOLS_Q_MAX_ELEMENTS             10U      /* Indicates maximum elements in the Q pool */
#define PH_TOOLS_Q_RESERVED_ELEMENTS        2U       /* Indicates reserved elements in the Q pool for high priority tasks. */

#if (PH_TOOLS_Q_MAX_ELEMENTS > 32U)
#   error "PH_TOOLS_Q_MAX_ELEMENTS can have a max value of 32."
#endif /* PH_TOOLS_Q_MAX_ELEMENTS > 32 */
/*@}*/

/**
* \brief Type of Queue Message.
*/
#if 0
typedef enum{
    PH_TOOLS_Q_DATA_TO_BE_SENT = 0x01,  /**< Message needs to be sent. */
    PH_TOOLS_Q_RX_DATA                  /**< Message is received. */
} phTools_Q_Type_t;
#endif

#define PH_TOOLS_Q_DATA_TO_BE_SENT 		0x01U
#define PH_TOOLS_Q_RX_DATA				PH_TOOLS_Q_DATA_TO_BE_SENT + 1U


/**
* \brief Queue structure of Message Queue.
*/
typedef struct phTools_Q
{
    void    * pSender;                /**< Sender of the message. As this is void pointer pointing to the sender's context,
                                           bSenderType determines the actual type of the sender to allow proper casting. */
    uint8_t *pbData;                  /**< Content of the message, type of the content is given using bType. */
    uint32_t dwLength;                /**< Length of the message. */
    uint8_t bLlcpBuf[6];              /**< Buffer used to store LLCP formatted PDUs that needs to be sent while processing Message Queue. */
    uint8_t bLlcpData;                /**< This variable is used to decide if LLCP framed data should be sent or data from application. */
    struct phTools_Q *pNext;          /**< Pointer to next element in the list/queue. */
    uint16_t wFrameOpt;               /**< Frame Option can take #PH_TRANSMIT_DEFAULT, #PH_TRANSMIT_BUFFER_FIRST,
                                           #PH_TRANSMIT_BUFFER_CONT, or #PH_TRANSMIT_BUFFER_LAST. */
    uint8_t bSenderType;              /**< Type of the sender, used for proper casting of pSender. */
    uint8_t bType;           		  /**< Message content type. It can be either data to be sent/received data. */
} phTools_Q_t;

/**
* \brief Calculate even or odd parity.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phTools_EncodeParity(
                                uint8_t bOption,                /**< [In] Parity option; e.g. #PH_TOOLS_PARITY_OPTION_EVEN. */
                                uint8_t * pInBuffer,            /**< [In] Array to input data. */
                                uint16_t wInBufferLength,       /**< [In] Length of input data in bytes. */
                                uint8_t bInBufferBits,          /**< [In] Number of valid bits in last byte of pInBuffer. */
                                uint16_t wOutBufferSize,        /**< [In] Size of the output buffer. */
                                uint8_t * pOutBuffer,           /**< [Out] Output buffer. */
                                uint16_t * pOutBufferLength,    /**< [Out] Number of valid bytes in pOutBuffer. */
                                uint8_t * pOutBufferBits        /**< [Out] Number of valid bits in last byte of pOutBuffer. */
                                );

/**
* \brief Verify and Remove even or odd parity.
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phTools_DecodeParity(
                                uint8_t bOption,                /**< [In] Parity option; e.g. #PH_TOOLS_PARITY_OPTION_EVEN. */
                                uint8_t * pInBuffer,            /**< [In] Array to input data. */
                                uint16_t wInBufferLength,       /**< [In] Length of input data in bytes. */
                                uint8_t bInBufferBits,          /**< [In] Number of valid bits in last byte of pInBuffer. */
                                uint16_t wOutBufferSize,        /**< [In] Size of the output buffer. */
                                uint8_t * pOutBuffer,           /**< [Out] Output buffer. */
                                uint16_t * pOutBufferLength,    /**< [Out] Number of valid bytes in pOutBuffer. */
                                uint8_t * pOutBufferBits        /**< [Out] Number of valid bits in last byte of pOutBuffer. */
                                );

/**
* \brief Calculate a CRC 5
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phTools_CalculateCrc5(
                                 uint8_t bOption,       /**< [In] Specifies whether the output should be inverted or not. */
                                 uint8_t bPreset,       /**< [In] Preset used for CRC calculation, e.g. #PH_TOOLS_CRC5_PRESET_I18000P3. */
                                 uint8_t bPolynom,      /**< [In] Polynom used for reduction, e.g. #PH_TOOLS_CRC5_POLY_I18000P3. */
                                 uint8_t * pData,       /**< [In] Array to input data. */
                                 uint16_t wDataLength,  /**< [In] Length of input data. */
                                 uint8_t * pCrc         /**< [Out] Resulting CRC. */
                                 );

/**
* \brief Calculate a CRC 8
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phTools_CalculateCrc8(
                                 uint8_t bOption,       /**< [In] Specifies whether the output should be inverted or not. */
                                 uint8_t bPreset,       /**< [In] Preset used for CRC calculation, e.g. #PH_TOOLS_CRC8_PRESET_UID. */
                                 uint8_t bPolynom,      /**< [In] Polynom used for reduction, e.g. #PH_TOOLS_CRC8_POLY_EPCUID. */
                                 uint8_t * pData,       /**< [In] Array to input data. */
                                 uint16_t wDataLength,  /**< [In] Length of input data. */
                                 uint8_t * pCrc         /**< [Out] Resulting CRC. */
                                 );

/**
* \brief Calculate a CRC 16
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phTools_CalculateCrc16(
                                  uint8_t bOption,      /**< [In] Specifies whether the output should be inverted or not. */
                                  uint16_t wPreset,     /**< [In] Preset used for CRC calculation, e.g. #PH_TOOLS_CRC16_PRESET_ISO14443A. */
                                  uint16_t wPolynom,    /**< [In] Polynom used for reduction, e.g. #PH_TOOLS_CRC16_POLY_ISO14443. */
                                  uint8_t * pData,      /**< [In] Array to input data. */
                                  uint16_t wDataLength, /**< [In] Length of input data. */
                                  uint16_t * pCrc       /**< [Out] Resulting CRC. */
                                  );

/**
* \brief Calculate a CRC 32
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phTools_CalculateCrc32(
                                  uint8_t bOption,      /**< [In] Specifies whether the output should be inverted or not. */
                                  uint32_t dwPreset,    /**< [In] Preset used for CRC calculation, e.g. #PH_TOOLS_CRC32_PRESET_DF8. */
                                  uint32_t dwPolynom,   /**< [In] Polynom used for reduction, e.g. #PH_TOOLS_CRC32_POLY_DF8. */
                                  uint8_t * pData,      /**< [In] Array to input data. */
                                  uint16_t wDataLength, /**< [In] Length of input data. */
                                  uint32_t * pCrc       /**< [Out] Resulting CRC. */
                                  );

/**
* \brief Calculate a CRC_B
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
*/
phStatus_t phTools_ComputeCrc_B(
                                uint8_t *pData,        /**< [In] Array to input data. */
                                uint32_t dwLength,     /**< [In] Length of input data. */
                                uint8_t *pCrc          /**< [Out] Resulting Two byte CRC. */
                                );

/**
* \brief The function gets the Version of the Firmware of DUT.
* \return Status code
* \retval #PH_ERR_SUCCESS Command successfully executed.
* \retval #PH_ERR_PROTOCOL_ERROR Invalid frame or unexpected receive data length or command code mismatch.
*/
phStatus_t phTools_GetVersion(
                              uint8_t * pProductVer,          /**< [OUT] Product Version. */
                              uint16_t * pMajor,              /**< [OUT] Major Version. */
                              uint8_t * pMinor,               /**< [OUT] Minor Version. */
                              uint16_t * pPatch_Dev,          /**< [OUT] Development/Patch Version. */
                              uint8_t * pVersionString,       /**< [OUT] Version String. */
                              uint16_t * pVersionStringLen    /**< [OUT] Version String Length. */
                              );

/**
* \brief To initialize the Queue manager.
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_RESOURCE_ERROR        Message Queue is unavailable or OSAL returned error.
*/
phStatus_t phTools_Q_Init(void);

/**
* \brief To get a empty Queue from a Free Pool.
* Returns a pointer to an empty Queue if available or returns NULL.
*
* \b bPriority can be one of:\n
* \li #PH_ON
* \li #PH_OFF
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
*/
phTools_Q_t* phTools_Q_Get(
                           uint32_t dwBlockTime,              /**< [IN] The time in ticks to wait for a empty Queue to become available. */
                           uint8_t  bPriority                 /**< [IN] Used to indicate if Queue is required by high priority thread or not. */
                           );

/**
* \brief Queue a message to last of the Message send queue list.
*
* \b wFrameOpt can be one of:\n
* \li #PH_TRANSMIT_DEFAULT
*
* \b wFrameOpt can be combined with:\n
* \li #PH_TRANSMIT_BUFFERED_BIT
* \li #PH_TRANSMIT_LEAVE_BUFFER_BIT
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
*/
phStatus_t phTools_Q_Send(
                          phTools_Q_t* psMsgQueue,            /**< [IN] Pointer to the Message Queue to be Queued. */
                          uint32_t dwBlockTime,               /**< [IN] The time in ticks to wait for a empty Queue to become available. */
                          uint16_t wFrameOpt                  /**< [IN] MAC/HAL Buffering option. */
                          );

/**
* \brief Queue a message to front of the Message send queue list.
*
* \b wFrameOpt can be one of:\n
* \li #PH_TRANSMIT_DEFAULT
*
* \b wFrameOpt can be combined with:\n
* \li #PH_TRANSMIT_BUFFERED_BIT
* \li #PH_TRANSMIT_LEAVE_BUFFER_BIT
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
*/
phStatus_t phTools_Q_SendFront(
                               phTools_Q_t* psMsgQueue,       /**< [IN] Pointer to the Message Queue to be Queued. */
                               uint32_t dwBlockTime,          /**< [IN] The time in ticks to wait for a empty Queue to become available. */
                               uint16_t wFrameOpt             /**< [IN] MAC/HAL Buffering option. */
                               );

/**
* \brief To get a filled message queue from the send queue list.
* Usually used by the Message Queue Manager to retrieve the queued messages waiting to be processed.
*/
phTools_Q_t* phTools_Q_Receive(
                               uint32_t dwBlockTime          /**< [IN] The time in ticks to wait for a empty Queue to become available. */
                               );

/**
* \brief Returns a freed message queue back to the free message pool.
* Usually used by the Message Queue Manager to return the queue to the Free pool.
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_RESOURCE_ERROR        OSAL returned error.
*/
phStatus_t phTools_Q_Release(
                             phTools_Q_t *psMsgQueue,         /**< [IN] Pointer to the Message Queue to be Queued. */
                             uint32_t dwBlockTime             /**< [IN] The time in ticks to wait for a empty Queue to become available. */
                             );

/**
* \brief To release the memory resources used by the Message queue manager.
*/
void phTools_Q_DeInit(void);



/** @}
* end of phTools group
*/

#ifdef __cplusplus
}/*Extern C*/
#endif

#endif /* PHTOOLS_H */
