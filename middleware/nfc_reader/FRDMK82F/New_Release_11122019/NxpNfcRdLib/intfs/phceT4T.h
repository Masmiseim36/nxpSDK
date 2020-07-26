/*
*         Copyright (c), NXP Semiconductors Bangalore / India
*
*                     (C)NXP Semiconductors
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
* particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Card Emulation component for Type 4A Tag.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef PHCET4T_H
#define PHCET4T_H

#include <ph_Status.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef NXPBUILD__PHCE_T4T_SW

#include <phOsal.h>
/** \defgroup phceT4T_Sw Component : Software
* @{
*/
/**
 * ID for this layer (software implementation for card emulation of Type 4 Tag).
 *
 * \ref phceT4T_Sw_DataParams::wId is initialized with this ID in the
 * initialization function \ref phceT4T_Sw_Init.
 * */
#define PHCE_T4T_SW_ID                    0x01U

/**
 * Maximum number of proprietary file supported.
 *
 * This defines the maximum count of proprietary file supported inside the T4T
 * NDEF Application. By default maximum proprietary file count is sent to 1.
 * User can change this value to support more proprietary files.
 * #NXPBUILD__PHCE_T4T_PROPRIETARY shall be enabled in build configuration to
 * support proprietary files.
 * */
#define PHCE_T4T_MAX_PROPRIETARY_FILE     0x01U



/* HCE */
#define E_PH_OSAL_EVT_RXDATA_AVAILABLE   (1U << 1U)     /**< Event posted by Reader library thread to the application thread once the ISO 18092 packet is received and processed. */
#define E_PH_OSAL_EVT_TXDATA_AVAILABLE   (1U << 2U)     /**< Event posted by application thread so that the Reader library thread takes the data and sends over ISO 18092. */
#define E_PH_OSAL_EVT_ERROR              (1U << 4U)     /**< Event posted by the Reader library thread, in case application should use /ref phOsal__PostwithDetail to knows the error occurred. */

/**< Event posted by response time extension timer ISR. Reader library thread listens to this event and sends the RTOx packet. */
#ifndef E_PH_OSAL_EVT_RTO_TIMEOUT
#    define E_PH_OSAL_EVT_RTO_TIMEOUT         (1U << 3U)
#endif


/**
* \brief NFC Type 4A Tag card emulation parameter structure
*/
typedef struct phceT4T_Sw_DataParams
{
    /**
     * ID for this component, not to be modified by user.
     *
     * This is initialized with #PHCE_T4T_SW_ID in the initialization
     * function \ref phceT4T_Sw_Init. This is used for validating the data
     * parameter structure and to make sure that APIs are called after
     * initialization.
     * */
    uint16_t wId;

    /**
     * Status Word (SW1 and SW2) as per ISO 7816-4 (Example: 0x9000 indicates
     * SUCCESS).
     *
     * This will be updated after successful processing of received C-APDU
     * (Command Application Protocol Data Unit). This Status Word is send as
     * the last 2 bytes of Response Application Protocol Data Unit (R-APDU).
     * */
    uint16_t wStatusWord;

    /**
     * Lc field of received C-APDU (Command Application Protocol Data Unit).
     *
     * This indicates the length of command data field. This parameter is
     * updated and used only for UPDATE BINARY command.
     * */
    uint16_t wLc;

    /**
     * Size of application buffer.
     *
     * Holds the size of application buffer (phceT4T_Sw_DataParams::pAppBuffer)
     * provided in the initialization function \ref phceT4T_Sw_Init. Application
     * buffer size should be at least equal to the HAL receive buffer.
     * */
    uint16_t wAppBufferSize;

    /**
     * Application buffer.
     *
     * Holds the application buffer provided in the initialization function
     * \ref phceT4T_Sw_Init. This buffer is used to copy and keep the received
     * data to avoid data loss in case of WTX (Waiting Time eXtension).
     * */
    uint8_t *pAppBuffer;

    /**
     * NDEF file pointer.
     *
     * Holds the pointer to application provided NDEF file. Application can set
     * the NDEF file using \ref phceT4T_SetElementaryFile with file type as
     * #PHCE_T4T_FILE_NDEF.
     * */
    uint8_t *pNdefFile;

    /**
     * NDEF file size.
     *
     * Holds the size of application provided NDEF file. Application can set
     * the NDEF file and file size using \ref phceT4T_SetElementaryFile with
     * file type as #PHCE_T4T_FILE_NDEF.
     * */
    uint32_t dwNdefFileSize;

    /**
     * CC (Capability Container) file pointer.
     *
     * Holds the pointer to application provided CC file. Application can set
     * the CC file using \ref phceT4T_SetElementaryFile with file type as
     * #PHCE_T4T_FILE_CC.
     * */
    uint8_t *pCcFile;

    /**
     * CC (Capability Container) file size.
     *
     * Holds the size of application provided CC file. Application can set
     * the CC file and file size using \ref phceT4T_SetElementaryFile with
     * file type as #PHCE_T4T_FILE_CC.
     * */
    uint16_t wCcFileSize;

    /**
     * CC (Capability Container) file content length.
     *
     * Holds the content length of application provided CC file. Application can
     * set the CC file and content length using \ref phceT4T_SetElementaryFile
     * with file type as #PHCE_T4T_FILE_CC. Content length specifies the length
     * of valid content inside file. For CC file the valid content length values
     * supported by \ref phceT4T_SetElementaryFile are:
     *  - 0x00 - No valid content. Then CE T4T layer will initialize CC with
     *           default values.
     *  - 0x0F - Content length till one normal NDEF TLV (8 bytes long) is
     *           specified.
     *  - 0x11 - Content length till one extended NDEF TLV (10 bytes long) is
     *           specified.
     *  - Others - If proprietary TLVs are specified in CC, the CC content
     *             length should include it too.
     * */
    uint16_t wCcContentLen;

    /**
     * Pointer to currently selected file.
     *
     * Holds the pointer to the currently selected file. This will be updated
     * after processing a valid FILE SELECT command. The pointer to the file
     * selected by the SELECT command is updated here.
     * */
    uint8_t *pSelectedFile;

    /**
     * Size of currently selected file.
     *
     * Holds the size of currently selected file. This will be updated after
     * processing a valid FILE SELECT command. The size of file selected by the
     * SELECT command is updated here.
     * */
    uint32_t dwFileSize;

    /**
     * Offset for selected file.
     *
     * Holds the offset of currently selected file. This will be updated after
     * processing a valid UPDATE BINARY command. The offset of file specified in
     * the UPDATE BINARY command is updated here. After updating the file this
     * will be updated with the file offset till where update happened
     * successfully.
     * */
    uint32_t dwFileOffset;

    /**
     * ID of currently selected file.
     *
     * Holds the ID of the currently selected file. This will be updated after
     * processing a valid FILE SELECT command. The ID of file selected by the
     * SELECT command is updated here.
     * */
    uint16_t wSelectedFileId;

    /**
     * Write access of currently selected file.
     *
     * Holds the write access of the currently selected file. This will be
     * updated after processing a valid FILE SELECT command. The write access
     * of file selected by the SELECT command is updated here.
     * */
    uint8_t bFileWriteAccess;

    /**
     * Current \ref states "state of tag".
     *
     * The supported values of tag state are:
     *  - #PHCE_T4T_STATE_NONE
     *  - #PHCE_T4T_STATE_CC_FILE_CREATED
     *  - #PHCE_T4T_STATE_NDEF_FILE_CREATED
     *  - #PHCE_T4T_STATE_NDEF_APP_SELECTED
     *  - #PHCE_T4T_STATE_FILE_SELECTED
     *  - #PHCE_T4T_STATE_FILE_READ
     *  - #PHCE_T4T_STATE_FILE_UPDATE
     * */
    uint8_t bTagState;

    /**
     * Pointer to ISO 14443p4mC PAL data parameter structure.
     *
     * Holds the pointer to ISO 14443p4mC PAL data parameter structure specified
     * by application in the initialization function \ref phceT4T_Sw_Init.
     * */
    void * pPalI14443p4mCDataParams;

    /**
     * Shared data buffer.
     *
     * Used to share data between reader library thread and application thread
     * in multi-threaded environment. Pointer to the data to be send to another
     * thread is copied to this variable and shared.
     * */
    uint8_t *pSharedData;

    /**
     * Length of shared data in phceT4T_Sw_DataParams::pSharedData.
     *
     * Used to share data between reader library thread and application thread
     * in multi-threaded environment. Length of data to be send to another
     * thread is copied to this variable and shared.
     * */
    uint16_t wSharedDataLen;

    /**
     * Indicates the data \ref options "received option".
     *
     * This specifies if the received data is complete or chaining. The
     * supported options are:
     *  - #PHCE_T4T_RXDEFAULT
     *  - #PHCE_T4T_RXCHAINING_BUFFER_FIRST
     *  - #PHCE_T4T_RXCHAINING_BUFFER_CONT
     *  - #PHCE_T4T_RXCHAINING_BUFFER_LAST
     *
     *  If UPDATE BINARY and/or proprietary commands are handled by application
     *  callback, then value of this option is send to application callback
     *  through wRxOption parameter of \ref phceT4T_AppCallback_t.
     * */
    uint8_t bRxOption;

    /**
     * Indicates support for proprietary commands.
     *
     * By default the proprietary command support is disabled. If proprietary
     * command support is disabled then unsupported command status word is send
     * back to reader.
     *
     * Use \ref phceT4T_SetConfig with #PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY
     * option to enable/disable (#PH_ON or #PH_OFF) support for proprietary
     * commands.
     * */
    uint8_t bSupportProprietaryCmd;

    /**
     * Indicates handling of UPDATE BINARY command by \ref phceT4T_Activate and
     * \ref phceT4T_ProcessCmd.
     *
     * By default the handling of UPDATE BINARY by \ref phceT4T_Activate and
     * \ref phceT4T_ProcessCmd is disabled (#PH_OFF). If UPDATE BINARY is
     * handled, WTX can not be handled. If application don't want handling of
     * UPDATE BINARY using a separate application thread (like in single
     * threaded environment with no RTOS), application can enable (#PH_ON)
     * this option.
     *
     * Use \ref phceT4T_SetConfig with #PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY
     * option to enable/disable (#PH_ON or #PH_OFF) this option.
     * */
    uint8_t bHandleUpdateCmd;

    /**
     * Used to share the exit status (error code) between reader library thread
     * and application thread in multi-threaded environment.
     *
     * This contains the return status from lower layers or error occurred in
     * this layer when \ref phceT4T_Activate exits.
     * */
    uint16_t wExitStatus;

#ifdef NXPBUILD__PHCE_T4T_PROPRIETARY
    /**
     * Number of proprietary files present.
     *
     * Application can set proprietary file using \ref phceT4T_SetElementaryFile
     * with file type as #PHCE_T4T_FILE_PROPRIETARY. The maximum number of
     * proprietary files that can be created is limited by
     * #PHCE_T4T_MAX_PROPRIETARY_FILE.
     *
     * NOTE: To use proprietary file feature, #NXPBUILD__PHCE_T4T_PROPRIETARY
     * should be enabled in build.
     * */
    uint8_t bProprietaryFileCount;

    /**
     * Structure to hold details of proprietary file(s).
     *
     * \b Note: <em>To use proprietary file feature,
     * #NXPBUILD__PHCE_T4T_PROPRIETARY should be enabled in build.</em>
     * */
    struct phceT4T_Sw_ProprietaryFile
    {
        /**
         * Proprietary file pointer.
         *
         * Holds the pointer to application provided proprietary file.
         * Application can set proprietary files using
         * \ref phceT4T_SetElementaryFile with file type as
         * #PHCE_T4T_FILE_PROPRIETARY.
         * */
        uint8_t *pFile;

        /**
         * Proprietary file size.
         *
         * Holds the size of application provided proprietary file. Application
         * can set proprietary files using \ref phceT4T_SetElementaryFile with
         * file type as #PHCE_T4T_FILE_PROPRIETARY.
         * */
        uint16_t wFileSize;

        /**
         * Proprietary file content length (length of valid content in file).
         *
         * Holds the content length of application provided proprietary file.
         * Application can set proprietary files using
         * \ref phceT4T_SetElementaryFile with file type as
         * #PHCE_T4T_FILE_PROPRIETARY.
         * */
        uint16_t wContentenLen;
    }asProprietaryFile[PHCE_T4T_MAX_PROPRIETARY_FILE];
#endif /* NXPBUILD__PHCE_T4T_PROPRIETARY */

    phOsal_EventObj_t T4TEventObj;                            /**< Event Object. */
}phceT4T_Sw_DataParams_t;

/**
 * \brief Initialize Type 4A card emulation layer.
 * \anchor initialization
 *
 * This should be called before using any of the card emulation APIs to do
 * the initialization of card emulation component. This configures the T4T card
 * emulation layer with default configurations. Refer \ref configurations
 * "configuration options" for supported configurations and their default
 * values.
 *
 * @param[in] pDataParams    Pointer to T4T card emulation data parameter
 * structure \ref phceT4T_Sw_DataParams_t.
 *
 * @param[in] wSizeOfDataParams    Specifies the size of data parameter
 * structure \ref phceT4T_Sw_DataParams_t.
 *
 * @param[in] pPalI14443p4mCDataParams    Pointer to the ISO 14443p4mC data
 * parameter structure \ref phpalI14443p4mC_Sw_DataParams_t.
 *
 * @param[in] pAppBuffer    Application buffer to be give by application where
 * the command received from external reader will be copied to. This is required
 * if application handles UPDATE BINARY or proprietary commands in application
 * thread (multi-threaded environment) otherwise this can be set to NULL (in
 * single threaded environment with no RTOS).
 *
 * @param[in] wAppBufferSize    Size of Application buffer (pAppBuffer). If
 * pAppBuffer is set to NULL, then this should be set to zero.
 *
 * \return Status code
 * \retval #PH_ERR_SUCCESS Operation successful.
 * \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. Size of data
 *         parameter structure \ref phceT4T_Sw_DataParams_t specified in
 *         wSizeOfDataParams is wrong.
 */
phStatus_t phceT4T_Sw_Init(
    phceT4T_Sw_DataParams_t *pDataParams,
    uint16_t wSizeOfDataParams,
    void *pPalI14443p4mCDataParams,
    uint8_t *pAppBuffer,
    uint16_t wAppBufferSize
    );
/** @} */
#endif /* NXPBUILD__PHCE_T4T_SW */

#ifdef NXPBUILD__PHCE_T4T

/** \defgroup phceT4T NFC Forum Type 4A Card Emulation
* \brief This component implements the card emulation of NFC Forum Type 4A Tag.
*
* Implementation is based on NFC Forum Type 4 Tag Operation Specification
* version 2.0. SELECT, READ BINARY and UPDATE BINARY commands are supported as
* per NFC Type 4 Tag Operation Specification.
*
* This layer follows a multi-threaded RTOS based design using one
* \ref phceT4T_Activate "reader library thread" and one
* \ref phceT4T_AppProcessCmd "application thread". Application thread along with
* \ref phceT4T_AppCallback_t "application callback" can be used for handling of
* UPDATE BINARY and proprietary commands.
*
* A pictorial representation of reader library thread and application thread
* used for card emulation is shown below. Refer \ref phceT4T_SetElementaryFile
* "function documentation" for more information.
*
* \image html hce_combined_threads.png
*
* As UPDATE BINARY and proprietary commands are handled by the
* \ref phceT4T_AppProcessCmd in application thread, application shall use some
* synchronization mechanism (like mutex or semaphore) as shown in the diagram to
* synchronize entry to and exit from \ref phceT4T_Activate and
* \ref phceT4T_AppProcessCmd.
*
* Waiting Time eXtension (WTX) is handled internally by \ref phceT4T_Activate in
* reader library thread. The reader library thread should be of high priority
* than the application thread for proper handling of WTX. In non RTOS single
* threaded environment this layer can be used with limited features, i.e. with
* no support for WTX.
* @{
*/

/**
 * CC (Capability Container) file ID as per NFC Forum Specification.
 * */
#define PHCE_T4T_CC_FILE_ID               0xE103U

/**
* \name Tag States
* \anchor states
*/
/*@{*/
/**
 * Initial state.
 *
 * State after \ref initialization "Initialization" of this layer.
 * */
#define PHCE_T4T_STATE_NONE               0x00U

/**
 * CC (Capability container) file created.
 *
 * State after creation of CC (Capability Container) file #PHCE_T4T_FILE_CC
 * using \ref phceT4T_SetElementaryFile.
 * */
#define PHCE_T4T_STATE_CC_FILE_CREATED    0x01U

/**
 * NDEF file created.
 *
 * State after creation of NDEF file #PHCE_T4T_FILE_NDEF using
 * \ref phceT4T_SetElementaryFile.
 * */
#define PHCE_T4T_STATE_NDEF_FILE_CREATED  0x02U

/**
 * NDEF application selected.
 *
 * State after receiving a valid SELECT request to select NFC NDEF application.
 * */
#define PHCE_T4T_STATE_NDEF_APP_SELECTED  0x03U

/**
 * File selected.
 *
 * State after receiving a valid SELECT request to select a file present inside
 * the NDEF application.
 * */
#define PHCE_T4T_STATE_FILE_SELECTED      0x04U

/**
 * File read.
 *
 * State after receiving a valid READ BINARY request.
 * */
#define PHCE_T4T_STATE_FILE_READ          0x05U

/**
 * File update.
 *
 * State after receiving a valid UPDATE BINARY request.
 * */
#define PHCE_T4T_STATE_FILE_UPDATE        0x06U

/**
 * Proprietary command received.
 *
 * State after receiving a proprietary command (command with unsupported
 * instruction byte).
 * */
#define PHCE_T4T_STATE_PROPRIETARY        0x07U
/*@}*/

/**
* \name Receive Options
* \anchor options
* \brief These options are given as input to \ref phceT4T_ProcessCmd and
* \ref appcallback "application callback" (if used) to indicate if the received
* data (C-APDU) given to it for processing is partial or complete.
*
* The data (C-APDU) to be processed is received using
* \ref phpalI14443p4mC_Receive.
*/
/*@{*/
/**
 * Default option.
 *
 * Option specified in \ref phceT4T_ProcessCmd or \ref appcallback
 * "application callback" when \ref phpalI14443p4mC_Receive returns
 * #PH_ERR_SUCCESS and the whole received data (complete C-APDU) is given for
 * processing.
 * */
#define PHCE_T4T_RXDEFAULT                   0x00U

/**
 * First part of partial receive data.
 *
 * Option specified in \ref phceT4T_ProcessCmd or \ref appcallback
 * "application callback" when \ref phpalI14443p4mC_Receive returns with
 * #PH_ERR_SUCCESS_CHAINING for first time and this partial received data
 * (first part of C-APDU) is given for processing. This option indicates that
 * the received data is not complete and only first part of data is given
 * for processing.
 * */
#define PHCE_T4T_RXCHAINING_BUFFER_FIRST     0x01U

/**
 * Subsequent part of partial receive data.
 *
 * Option specified in \ref phceT4T_ProcessCmd or \ref appcallback
 * "application callback" when \ref phpalI14443p4mC_Receive returns with
 * #PH_ERR_SUCCESS_CHAINING for second time or more and this partial received
 * data (subsequent part of C-APDU) is given for processing. This option
 * indicates that the received data is not complete and subsequent part of
 * data is given for processing.
 * */
#define PHCE_T4T_RXCHAINING_BUFFER_CONT      0x02U

/**
 * Last chunk of receive data.
 *
 * Option specified in \ref phceT4T_ProcessCmd or \ref appcallback
 * "application callback" when \ref phpalI14443p4mC_Receive returns with
 * #PH_ERR_SUCCESS after a #PH_ERR_SUCCESS_CHAINING in previous receive and this
 * partial received data is given for processing. This option indicates that it
 * is last part of partial receive data.
 * */
#define PHCE_T4T_RXCHAINING_BUFFER_LAST      0x03U
/*@}*/

/**
* \name Return Status
* \brief Custom return status codes of T4T card emulation layer.
*/
/*@{*/
/**
 * Failure.
 *
 * This status code is returned by \ref phceT4T_ProcessCmd when an unsupported
 * C-APDU (Command Application Protocol Data Unit) is given to it for
 * processing. Supported C-APDUs are SELECT, READ BINARY, UPDATE BINARY and
 * PROPRIETARY (commands with unsupported instruction byte).
 * */
#define PH_CE_T4T_FAILURE                ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x00U)

/**
 * SELECT command received.
 *
 * This status code is returned by \ref phceT4T_ProcessCmd when a valid SELECT
 * command is given to it for processing and the file is successfully selected.
 * */
#define PH_CE_T4T_SELECT                 ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x01U)

/**
 * READ BINARY command received.
 *
 * This status code is returned by \ref phceT4T_ProcessCmd when a valid READ
 * BINARY command is given to it for processing and the file is successfully
 * read.
 * */
#define PH_CE_T4T_READ_BINARY            ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x02U)

/**
 * UPDATE BINARY command received.
 *
 * This status code is returned by \ref phceT4T_ProcessCmd when a valid UPDATE
 * BINARY command is given to it for processing. The file will be updated by
 * \ref phceT4T_ProcessCmd if #PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY is enabled
 * else application shall update the file using \ref phceT4T_AppProcessCmd or
 * using application callback from application thread.
 * */
#define PH_CE_T4T_UPDATE_BINARY          ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x03U)

/**
 * Proprietary command received.
 *
 * This status code is returned by \ref phceT4T_ProcessCmd when a proprietary
 * command (command with unsupported instruction byte) is given to it for
 * processing. If #PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY is enabled then
 * proprietary command will be sent to application and application can handle it
 * using callback, else unsupported instruction is returned as status to reader.
 * */
#define PH_CE_T4T_PROPRIETARY            ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x04U)

/**
 * Tag in invalid state to execute the requested operation.
 *
 * This status code is returned by \ref phceT4T_GetSelectedFileInfo,
 * \ref phceT4T_SetConfig and \ref phceT4T_GetConfig when card emulation layer
 * is not in a valid state to execute these APIs.
 * */
#define PH_CE_T4T_INVALID_STATE          ((phStatus_t)PH_ERR_CUSTOM_BEGIN + 0x05U)
/*@}*/

/**
* \name File Types
* \brief File types to be specified in \ref phceT4T_SetElementaryFile.
*/
/*@{*/
/**
 * Capability Container (CC) File. This is used in
 * \ref phceT4T_SetElementaryFile to set the CC file.
 * */
#define PHCE_T4T_FILE_CC            0x00U

/**
 * NFC Data Exchange Format (NDEF) File.  This is used in
 * \ref phceT4T_SetElementaryFile to set the NDEF file.
 * */
#define PHCE_T4T_FILE_NDEF          0x01U

/**
 * Proprietary File. This is used in \ref phceT4T_SetElementaryFile to set a
 * proprietary file.
 * */
#define PHCE_T4T_FILE_PROPRIETARY   0x02U
/*@}*/

/**
* \name Configuration Types
* \anchor configurations
* \brief These options shall be used along with \ref phceT4T_SetConfig and
* \ref phceT4T_GetConfig to configure Type 4A card emulation layer.
*/
/*@{*/
/**
 * Enable/Disable handling of UPDATE BINARY inside card emulation layer.
 *
 * Default value is #PH_OFF (disabled). Supported values are #PH_ON and #PH_OFF.
 * If enabled, the UPDATE BINARY command is processed and file is updated by the
 * \ref phceT4T_ProcessCmd itself. If disabled, UPDATE BINARY command is only
 * validated and data to be written to file is given to application thread (i.e.
 * \ref phceT4T_AppProcessCmd) to update the file.
 * */
#define PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY               0x00U

/**
 * Enable/Disable support for proprietary command.
 *
 * Default value is #PH_OFF (disabled). Supported values are #PH_ON and #PH_OFF.
 * If disabled, unsupported instruction status word is sent back to reader. If
 * enabled, application can handle proprietary commands in application callback
 * \ref phceT4T_AppCallback_t of \ref phceT4T_AppProcessCmd.
 * */
#define PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY               0x01U

/**
 * Set/Get NDEF file ID.
 *
 * Valid range is between 0001h to E101h, E104h to 3EFFh, 3F01h to 3FFEh and
 * 4000h to FFFEh. NDEF file ID can be updated only after
 * \ref phceT4T_SetElementaryFile is executed for NDEF file #PHCE_T4T_FILE_NDEF.
 * */
#define PHCE_T4T_CONFIG_NDEF_FILE_ID                      0x02U

/**
 * Set/Get NDEF file read access value.
 *
 * Default is 0x00 (unrestricted read access). Only 0x00 is the supported value
 * as per NFC Forum Type 4 Tag specification. Other values are RFU.
 *
 * This can be configured only after executing \ref phceT4T_SetElementaryFile
 * for CC file #PHCE_T4T_FILE_NDEF.
 * */
#define PHCE_T4T_CONFIG_NDEF_FILE_READ_ACCESS             0x03U

/**
 * Set/Get NDEF file write access value.
 *
 * Default is 0x00 (unrestricted write access). Supported values are 0x00 (write
 * access) and 0xFF (no write access, read only). Other values are RFU as per
 * NFC Forum Type 4 Tag Specification.
 *
 * This can be configured only after executing \ref phceT4T_SetElementaryFile
 * for CC file #PHCE_T4T_FILE_NDEF.
 * */
#define PHCE_T4T_CONFIG_NDEF_FILE_WRITE_ACCESS            0x04U

/**
 * Set/Get value of MLe (Maximum data that can be read in one Read Binary
 * command).
 *
 * Default is 0xFFFF. Supported values are from 0x000F - 0xFFFF. Maximum read
 * data length of 0xFFFF is supported only with extended APDUs (Application
 * protocol data units). For short APDUs a maximum read length of 0xFF is
 * supported. This can be configured only after executing
 * \ref phceT4T_SetElementaryFile for CC file #PHCE_T4T_FILE_CC.
 *
 * For more info on short and extended APDUs refer ISO 7816-4 2005, Section 5.
 * */
#define PHCE_T4T_CONFIG_MLE                               0x05U

/**
 * Set/Get value of MLc (Maximum data that can be written in one Update Binary
 * command).
 *
 * Default is 0xFFFF. Supported values are from 0x0001 - 0xFFFF. Maximum write
 * data length of 0xFFFF is supported only with extended APDUs (Application
 * protocol data units). For short APDUs a maximum write data length of 0xFF is
 * supported. This can be configured only after executing
 * \ref phceT4T_SetElementaryFile for CC file #PHCE_T4T_FILE_CC.
 *
 * For more info on short and extended APDUs refer ISO 7816-4 2005, Section 5.
 * */
#define PHCE_T4T_CONFIG_MLC                               0x06U
/*@}*/

/**
* \anchor appcallback
* \brief Application callback to handle proprietary commands and UPDATE BINARY.
* This should be implemented by the user if proprietary means are needed to
* write to NDEF file and/or proprietary file(s) which may be on EEPROM or to
* handle proprietary commands.
*
* Application callback, if implemented shall be specified when calling \ref
* phceT4T_AppProcessCmd from application thread. Otherwise a default
* implementation as provided in \ref phceT4T_AppProcessCmd will execute.
*
* For UPDATE BINARY command, data to be written to file and length of data to
* be written to file is given to callback in pRxData and wRxDataLen
* respectively. For proprietary commands whole C-APDU (Command application
* Protocol Data Unit) received from reader is given to callback.
*
* R-APDU (Response Application Protocol Data Unit) to be send back to reader
* should be updated in ppTxData (data field of R-APDU) and pStatusWord (SW1 and
* SW2 of R-APDU).
*
* Application callback can get the details of currently selected file using
* \ref phceT4T_GetSelectedFileInfo.
*
* @param[in] bState    Tag State indicating received command type
* (#PHCE_T4T_STATE_FILE_UPDATE or #PHCE_T4T_STATE_PROPRIETARY).
*
* @param[in] wRxOption    \ref options "Receive option" indicating if received
* data (pRxData) is partial or complete.
*
* @param[in] pRxData    Data received from reader library thread.
*
* @param[in] wRxDataLen    Length of received data (length of pRxData).
*
* @param[out] pStatusWord    Status Word to be sent (as part of R-APDU). For
* example set pStatusWord as 0x9000 when requested command is successfully
* completed.
*
* @param[out] ppTxData    Data to be transmitted (as part of R-APDU). If only
* status word (pStatusWord) needs to be send as response to reader, set this
* to NULL. For UPDATE BINARY command this should be set to NULL.
*
* @param[out] pTxDataLen    Length of data to be transmitted. Set this to 0 if
* data to be transmitted (ppTxData) is set to NULL.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INTERNAL_ERROR for all failures.
*
* \b Usage:
*
* <b>Application Callback (Sample Code):</b>
*
\code
    phStatus_t AppCallback(
                           uint8_t bState,
                           uint16_t wRxOption,
                           uint8_t *pRxData,
                           uint16_t wRxDataLen,
                           uint16_t *pStatusWord,
                           uint8_t **ppTxData,
                           uint16_t *pTxDataLen
                           )
    {
        phStatus_t status;
        uint16_t wSelectedFileId;
        uint8_t *pSelectedFile;
        uint32_t dwSelectedFileSize;
        uint8_t bWriteAccess;
        uint32_t dwFileOffset;

        / / UPDATE BINARY command handling
        if(bState == PHCE_T4T_STATE_FILE_UPDATE)
        {
            / / Get Selected file info.
            status = phceT4T_GetSelectedFileInfo(
                &sceT4T,
                &wSelectedFileId,
                &pSelectedFile,
                &dwSelectedFileSize,
                &bWriteAccess,
                &dwFileOffset);

            if((status & PH_ERR_MASK) == PH_ERR_SUCCESS)
            {
                / / Update file. If specific methods are needed for writing file,
                / / use it here.
                (void)memcpy(&pSelectedFile[dwFileOffset], pRxData, wRxDataLen);

                / / File update success, update out parameters.
                *pStatusWord = 0x9000;
                *ppTxData = NULL;
                *pTxDataLen = 0;
            }
            else / / This condition shall never come.
            {
                / / Could not update file.
                printf("Application Thread: Getting file Info failed...\n");
                *pStatusWord = 0x6A82;
                *ppTxData = NULL;
                *pTxDataLen = 0;

                / / Return same error back.
                return (status & PH_ERR_MASK);
            }
        }

        / / Proprietary command handling.
        else
        {
         / / DEFAULT: No proprietary support.
            *pStatusWord = 0x6D00; / / Instruction code not supported.
            *ppTxData = NULL;
            *pTxDataLen = 0;
        }

        return PH_ERR_SUCCESS;
    }
\endcode
*/
typedef phStatus_t(*phceT4T_AppCallback_t)(
    uint8_t bState,
    uint16_t wRxOption,
    uint8_t *pRxData,
    uint16_t wRxDataLen,
    uint16_t *pStatusWord,
    uint8_t **ppTxData,
    uint16_t *pTxDataLen
    );

#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phceT4T/src/Sw/phceT4T_Sw.h"

#define phceT4T_SetElementaryFile(              \
                                  pDataParams,  \
                                  bFileType,    \
                                  pFile,        \
                                  wFileId,      \
                                  dwFileSize,   \
                                  dwContentLen) \
     phceT4T_Sw_SetElementaryFile(                                          \
                                  (phceT4T_Sw_DataParams_t *)pDataParams,   \
                                  bFileType,                                \
                                  pFile,                                    \
                                  wFileId,                                  \
                                  dwFileSize,                               \
                                  dwContentLen)

#define phceT4T_ProcessCmd(                 \
                           pDataParams,     \
                           wOption,         \
                           pRxData,         \
                           wRxDataLen,      \
                           pStatusWord,     \
                           ppTxData,        \
                           pTxDataLen)      \
        phceT4T_Sw_ProcessCmd(              \
                              (phceT4T_Sw_DataParams_t *)pDataParams,   \
                              wOption,                                  \
                              pRxData,                                  \
                              wRxDataLen,                               \
                              pStatusWord,                              \
                              ppTxData,                                 \
                              pTxDataLen)

#define phceT4T_Activate(pDataParams) \
        phceT4T_Sw_Activate((phceT4T_Sw_DataParams_t *)pDataParams)

#define phceT4T_AppProcessCmd(              \
                              pDataParams,  \
                              pAppCallback) \
        phceT4T_Sw_AppProcessCmd(                                           \
                                 (phceT4T_Sw_DataParams_t *)pDataParams,    \
                                 pAppCallback)

#define phceT4T_GetSelectedFileInfo(                \
                                    pDataParams,    \
                                    pFileId,        \
                                    ppFile,         \
                                    pFileSize,      \
                                    pWriteAccess,   \
                                    pFileOffset)    \
        phceT4T_Sw_GetSelectedFileInfo(                                        \
                                       (phceT4T_Sw_DataParams_t *)pDataParams, \
                                       pFileId,                                \
                                       ppFile,                                 \
                                       pFileSize,                              \
                                       pWriteAccess,                           \
                                       pFileOffset)

#define phceT4T_Reset(pDataParams) \
        phceT4T_Sw_Reset((phceT4T_Sw_DataParams_t *)pDataParams)

#define phceT4T_SetConfig(              \
                          pDataParams,  \
                          wConfig,      \
                          wValue)       \
        phceT4T_Sw_SetConfig(                                           \
                             (phceT4T_Sw_DataParams_t *)pDataParams,    \
                             wConfig,                                   \
                             wValue)

#define phceT4T_GetConfig(              \
                          pDataParams,  \
                          wConfig,      \
                          pValue)       \
        phceT4T_Sw_GetConfig(                                           \
                             (phceT4T_Sw_DataParams_t *)pDataParams,    \
                             wConfig,                                   \
                             pValue)
#else
/**
* \brief Set a supported Elementary File (EF).
* \anchor functions
*
* Supported EF types are:
*  - #PHCE_T4T_FILE_CC
*  - #PHCE_T4T_FILE_NDEF
*  - #PHCE_T4T_FILE_PROPRIETARY
*
* NDEF file (#PHCE_T4T_FILE_NDEF) and proprietary file
* (#PHCE_T4T_FILE_PROPRIETARY) can be on EEPROM. If writing to EEPROM needs
* special methods, then application should handle it using callback
* \ref phceT4T_AppCallback_t in \ref phceT4T_AppProcessCmd.
*
* Refer NFC Forum Type 4 Tag Operation Specification version 2.0, Section 5 for
* details of file size and file content for each file types.
*
* For CC file, if content length is set to 0, then CC file will be initialized
* with default values. If not set to 0, then content length should be a valid
* value including NDEF file TLV size and optional proprietary file TLV(s), if
* present.
*
* CC file and NDEF file are mandatory. CC file #PHCE_T4T_FILE_CC should be
* created first. Followed by NDEF file #PHCE_T4T_FILE_NDEF. Proprietary file(s)
* are optional.
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* @param[in] bFileType    Type of Elementary File. Supported file types are
* #PHCE_T4T_FILE_CC, #PHCE_T4T_FILE_NDEF and #PHCE_T4T_FILE_PROPRIETARY.
*
* @param[in] pFile    Pointer to the Elementary File. For example, elementary
* file can be a pointer to a buffer like aFile[1000].
*
* @param[in] wFileId    Elementary File ID. IDs as per NFC Forum T4T
* Specification 2.0 are supported. For CC file, the file ID should be set to the
* NFC Forum defined value of 0xE103. For NDEF and proprietary files the valid ID
* ranges are between 0001h to E101h, E104h to 3EFFh, 3F01h to 3FFEh and 4000h to
* FFFEh.
*
* @param[in] dwFileSize    Size of elementary file. For example, if aFile[1000]
* is set as the elementary file then dwFileSize should be set to 1000.
*
* @param[in] dwContentLen    Actual content length in elementary file. This
* specifies the length of actual valid content present in file. For example if
* aFile[1000] = {0x00, 0x02, 0x03, 0x00} is set as the elementary file then
* dwContentLen should be set to 4 and dwFileSize should be set to 1000.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_PARAMETER  Parameter value is invalid. An input
*         parameter is invalid.
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
*/
phStatus_t phceT4T_SetElementaryFile(
    void *pDataParams,
    uint8_t bFileType,
    uint8_t *pFile,
    uint16_t wFileId,
    uint32_t dwFileSize,
    uint32_t dwContentLen
    );

/**
* \brief Activate T4T card emulation functionality.
*
* This shall be called from the reader library thread. This API calls \ref
* phpalI14443p4mC_Receive to receive data from reader and pass it to \ref
* phceT4T_ProcessCmd for processing the received C-APDU (Command Application
* Protocol Data Unit). After processing, it sends the R-APDU (Response
* Application Protocol Data Unit) to reader using \ref phpalI14443p4mC_Transmit.
*
* Based on the configuration options #PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY and
* #PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY, UPDATE BINARY and PROPRIETARY commands
* are handled by itself or given to application thread (i.e.
* \ref phceT4T_AppProcessCmd) for handling.
*
* Waiting Time eXtension (WTX) is handled internally by this function. This
* function can be used in single threaded non RTOS environment if UPDATE BINARY
* and proprietary commands are configured to be handled by this function itself
* (i.e. with no application thread present). WTX is not supported in single
* threaded environment.
*
* \ref phceT4T_Activate shall be called only after successful activation of ISO
* 14443-4 card layer \ref phpalI14443p4mC_Activate. This API exits only on error
* from lower layers, ABORT, external RF OFF or DESELECT.
*
* Application shall use some synchronization mechanism like mutex or semaphore
* to synchronize between application thread and reader library thread (i.e. to
* synchronize entry to and exit from \ref phceT4T_Activate and
* \ref phceT4T_AppProcessCmd).
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* \return Status code
* \retval #PH_ERR_SUCCESS_DESELECTED Tag DESELECTED. Tag got successfully
*         DESELECTED after receiving DESELECT request from the reader.
* \retval #PH_ERR_EXT_RF_ERROR External RF field is OFF.
* \retval #PH_ERR_ABORTED Aborted due to \ref phhalHw_AsyncAbort "HAL shutdown".
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
* \retval Other Error from underlying components.
*
* \b Usage:
*
* <b>Reader Library Thread (Block Diagram):</b>
*
* \image html hce_rdlib_thread.png
*
* <b>Reader Library Thread (Sample Code):</b>
*
\code
    void TReaderLibrary(void *pParams)
    {
        phStatus_t  status = 0;

        /-/ Set CC File.
        status = phceT4T_SetElementaryFile(
            &sceT4T,
            PHCE_T4T_FILE_CC,
            aCcFile,
            0xE103,
            sizeof(aCcFile),
            wCcFileContentLength);

        /-/ Set NDEF File.
        status = phceT4T_SetElementaryFile(
            &sceT4T,
            PHCE_T4T_FILE_NDEF,
            aNdefFile,
            wNdefFileId,
            sizeof(aNdefFile),
            wNdefFileContentLength);

        /-/ Loop always in listen mode.
        while(1)
        {
            /-/ Start discovery loop in listen mode
            status = phacDiscLoop_Run(
                &sDiscLoop,
                PHAC_DISCLOOP_ENTRY_POINT_LISTEN);
            if((status & PH_ERR_MASK) != PHAC_DISCLOOP_ACTIVATED_BY_PEER)
            {
                goto reset_tag;
            }

            /-/ Validate RATS and send ATS.
            status = phpalI14443p4mC_Activate(
                &spalI14443p4mC,
                pRxBuf,
                wRxBufLen,
                aAtsRes,
                wAtsLength);
            if((status & PH_ERR_MASK) != PH_ERR_SUCCESS)
            {
                goto reset_tag;
            }

            /-/ Trigger phceT4T_AppProcessCmd in Application thread to start.
            status = phOsal_Semaphore_Give(appstart);

            /-/ Activate HCE.
            status = phceT4T_Activate(&sceT4T);

            /-/ Wait for phceT4T_AppProcessCmd in Application thread to exit.
            status = phOsal_Semaphore_Take(appexit, 0xFFFFFFFF);

    reset_tag:
            /-/ Reset Reader library layers.
            (void)phpalI14443p4mC_ResetProtocol(&spalI14443p4mC);
            (void)phceT4T_Reset(&sceT4T);

            /-/ Clear all events from all sources.
            (void)phOsEventClear(pDataParams->Eventhandle, E_OS_EVENT_OPT_NONE, E_PH_OSAL_EVT_ALL, NULL);

        }
    }
\endcode
*/
phStatus_t phceT4T_Activate(
    void *pDataParams
    );

/**
* \brief Validate and process the received C-APDU (Command Application Protocol
* Data Unit).
*
* If RTOS is not used, then this API shall be called directly by the application
* in the only existing application task. In multi-threaded application or when
* \ref phceT4T_Activate is used by application, this API will be called
* internally by \ref phceT4T_Activate.
*
* Data received from \ref phpalI14443p4mC_Receive (C-APDU) shall be passed to
* this API for validation and processing. The out data parameters, pStatusWord
* and ppTxData (R-APDU) shall be send to reader using
* \ref phpalI14443p4mC_Transmit.
*
* SELECT and READ BINARY commands are validated and processed by this API.
* Proprietary commands (commands with unsupported instruction byte) are not
* processed and the whole C-APDU (Command Application Protocol Data Unit) is
* returned back in out parameter (ppTxData). For UPDATE BINARY command the file
* will be updated if #PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY is enabled else data
* to be written to file and data length is returned in out parameters ppTxData
* and pTxDataLen respectively.
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* @param[in] wOption     \ref options "Received Option". This indicates if the
* received data (pRxData) is complete or partial.
*
* @param[in] pRxData    Received data (C-APDU). Data (Command) received from
* reader side. Data can be received from reader using
* \ref phpalI14443p4mC_Receive and passed to this API for processing.
*
* @param[in] wRxDataLen    Received data length. Length of pRxData.
*
* @param[out] pStatusWord    Status Word. Contains status word SW1 and SW2 (last
* 2 bytes of R-APDU to be send back to reader). For example a value of 0x9000
* indicates that the received data (C-APDU) is valid and processed successfully.
* For a complete list of values refer ISO 7816-4 specification.
*
* @param[out] ppTxData    Data to be transmitted back to the reader. Contains
* the data field of R-APDU. This will be set to NULL if data field is not needed
* in R-APDU.
*
* @param[out] pTxDataLen    Length of data to be transmitted back to reader
* (length of R-APDU data field ppTxData). This will be set to 0 if ppTxData is
* NULL (i.e. no data field in R-APDU).
*
* \return Status code
* \retval #PH_CE_T4T_SELECT File selected.
* \retval #PH_CE_T4T_READ_BINARY File read.
* \retval #PH_CE_T4T_UPDATE_BINARY Update Binary received or file updated
*         (if #PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY is enabled).
* \retval #PH_CE_T4T_PROPRIETARY Proprietary C-APDU received.
* \retval #PH_CE_T4T_FAILURE Unsupported C-APDU / Error. Check status word
*         (pStatusWord) to get the actual type of error.
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
*/
phStatus_t phceT4T_ProcessCmd(
    void *pDataParams,
    uint16_t wOption,
    uint8_t *pRxData,
    uint16_t wRxDataLen,
    uint16_t *pStatusWord,
    uint8_t **ppTxData,
    uint16_t *pTxDataLen
    );

/**
* \brief Application process command.
*
* This shall be called from the application thread when RTOS / multiple threads
* are used. Based on configuration options #PHCE_T4T_CONFIG_HANDLE_UPDATEBINARY
* and #PHCE_T4T_CONFIG_RECEIVE_PROPRIETARY, UPDATE BINARY and PROPRIETARY
* commands are handled by \ref phceT4T_Activate or by this API.
*
* If UPDATE BINARY needs some special write methods (i.e. if files are in
* EEPROM) or if some proprietary commands needs to be supported then application
* callback \ref phceT4T_AppCallback_t shall be provided for that. Else callback
* shall be set to NULL.
*
* This API exits only on error from lower layers, ABORT, external RF OFF or
* DESELECT. Application shall use some synchronization mechanism like mutex or
* semaphore to synchronize between application thread and reader library thread
* (i.e. to synchronize entry to and exit from \ref phceT4T_Activate and
* \ref phceT4T_AppProcessCmd).
*
* When \ref phceT4T_Activate returns in reader library thread, this API will
* return with same status in application thread.
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* @param[in] pAppCallback    \ref appcallback "Application callback".
* Application callback to handle UPDATE BINARY and proprietary commands. Can be
* set to NULL if not used.
*
* \return Status code
* \retval #PH_ERR_SUCCESS_DESELECTED Tag DESELECTED. Tag got successfully
*         DESELECTED after receiving DESELECT request from the reader.
* \retval #PH_ERR_EXT_RF_ERROR External RF field is OFF.
* \retval #PH_ERR_ABORTED Aborted due to \ref phhalHw_AsyncAbort "HAL shutdown".
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
* \retval Other Error from underlying components.
*
* \b Usage:
*
* <b>Application Thread (Block Diagram):</b>
*
* \image html hce_app_thread.png
*
* <b>Application Thread (Sample Code):</b>
\code
    void TApplication(void *pParams)
    {
        phStatus_t  status;

        while(1)
        {
            /-/ Wait for trigger from reader library thread to start.
            status = phOsal_Semaphore_Take(appstart, 0xFFFFFFFF);

            /-/ Start Application processing of HCE.
            status = phceT4T_AppProcessCmd(&sceT4T, AppCallback);

            /-/ Trigger reader lib thread about the phceT4T_AppProcessCmd exit.
            status = phOsal_Semaphore_Give(appexit);
        }
    }
\endcode
*/
phStatus_t phceT4T_AppProcessCmd(
    void *pDataParams,
    phceT4T_AppCallback_t pAppCallback
    );

/**
* \brief Get information about currently selected file.
*
* Application shall use this to get information about currently selected file
* when UPDATE BINARY and proprietary commands are handled by \ref appcallback
* "application callback".
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* @param[out] pFileId    ID of selected file. Used to pass back the ID of
* currently selected file.
*
* @param[out] ppFile    Pointer to selected file. Used to pass back the pointer
* to currently selected file.
*
* @param[out] pFileSize    Size of selected file. Used to pass back the size of
* currently selected file.
*
* @param[out] pWriteAccess    Write access of selected file. Used to pass back
* the write access of currently selected file. Only 0x00 and 0xFF are the
* supported vales. 0x00 - write access with out any restriction. 0xFF - no write
* access (read only file). All other values are RFU (not used).
*
* @param[out] pFileOffset    Used to pass back the offset of currently selected
* file from where it should be updated. This is valid only for UPDATE BINARY
* command. Application shall update the selected file (ppFile) starting from
* this offset (pFileOffset) for UPDATE BINARY command. When processing
* proprietary commands, value of this parameter shall be ignored as whole
* command (C-APDU) is processed by application callback itself.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
* \retval #PH_CE_T4T_INVALID_STATE Invalid State. Operation not supported in
*         current tag state (i.e. no files are in selected state).
*/
phStatus_t phceT4T_GetSelectedFileInfo(
    void *pDataParams,
    uint16_t *pFileId,
    uint8_t **ppFile,
    uint32_t *pFileSize,
    uint8_t *pWriteAccess,
    uint32_t *pFileOffset
    );

/**
* \brief Reset the tag's state variables.
*
* Application shall use this to reset variables and data parameters of tag after
* one cycle of operation or on error (i.e. after /ref phceT4T_Activate exists).
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
*/
phStatus_t phceT4T_Reset(
    void *pDataParams
    );

/**
* \brief Set value of the specified \ref configurations "configuration
* identifier".
*
* The SetConfig command is used to set values for the different configuration
* parameters of this layer.
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* @param[in] wConfig    \ref configurations "Configuration Identifier". Specify
* the configuration identifier to be set.
*
* @param[in] wValue    Configuration Value. Value to be set for the specified
* \ref configurations "configuration identifier" (wConfig).
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
* \retval #PH_ERR_INVALID_PARAMETER Parameter value or configuration option is
*         invalid.
* \retval #PH_CE_T4T_INVALID_STATE Invalid state. Operation not supported in
*         current tag state. Refer \ref configurations "configuration options"
*         to known when each configuration can be set.
*/
phStatus_t phceT4T_SetConfig(
    void *pDataParams,
    uint16_t wConfig,
    uint16_t wValue
    );

/**
* \brief Get value of the specified \ref configurations "configuration
* identifier".
*
* The GetConfig command is used to  retrieve values of different configuration
* parameters of this layer.
*
* @param[in] pDataParams    Pointer to this layer's parameter structure \ref
* phceT4T_Sw_DataParams_t.
*
* @param[in] wConfig    \ref configurations "Configuration Identifier". Specify
* the configuration identifier for which value to be retrieved.
*
* @param[out] pValue    Configuration Value. Used to pass back the value of the
* specified \ref configurations "configuration identifier" (wConfig).
*
* \return Status code
* \retval #PH_ERR_SUCCESS Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS Invalid data parameter. The provided data
*         parameter (pDataParams) is invalid or this layer is not
*         \ref initialization "initialized".
* \retval #PH_ERR_INVALID_PARAMETER Parameter value or configuration option
*         is invalid.
* \retval #PH_CE_T4T_INVALID_STATE Invalid state. Operation not supported in
*         current tag state. Refer \ref configurations "configuration options"
*         to known when each configuration value can be retrieved.
*/
phStatus_t phceT4T_GetConfig(
    void *pDataParams,
    uint16_t wConfig,
    uint16_t *pValue
    );

#endif /* NXPRDLIB_REM_GEN_INTFS */
/** @} */
#endif /* NXPBUILD__PHCE_T4T */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHCET4T_H */
