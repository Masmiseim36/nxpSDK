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
* Generic Simple NDEF Exchange Protocol( SNEP) Component of Reader Library Framework.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  KK: Generated 05. Mar 2014
*
*/

#ifndef PHNPSNEP_H
#define PHNPSNEP_H

#include <ph_Status.h>
#include <phlnLlcp.h>

#ifdef __cplusplus
extern "C" {
#endif    /* __cplusplus */

#ifdef NXPBUILD__PHNP_SNEP_SW

/** \defgroup phnpSnep_Sw Component : Software
* \brief Implementation of NFC Forum Simple NDEF Exchange Protocol (SNEP) 1.0
* @{
*/

#define PHNP_SNEP_SW_ID               0x01U                      /**< ID for SNEP SW component. */

/**
* \brief Defines SNEP Server Type
*/
typedef enum /* phnpSnep_Server_type */
{
    phnpSnep_Default_Server    = 0x01,                          /**< Default NFC Forum SNEP server. */
    phnpSnep_NonDefault_Server                                  /**< Proprietary SNEP server. */
} phnpSnep_Server_type_t;

/**
*\brief SNEP parameter structure
*/
typedef struct /* phnpSnep_Sw_DataParams */
{
    uint16_t               wId;                                 /**< Layer ID for this component, NEVER MODIFY!. */
    void                 * plnLlcpDataParams;                   /**< LLCP Data Parameters structure pointer. */
    void                 * psSocket;                            /**< LLCP transport Socket structure pointer. */
    uint8_t                bSnepVersion;                        /**< SNEP protocol version supported by Implementation. */
    phnpSnep_Server_type_t eServerType;                         /**< Initialized Server Type.*/
} phnpSnep_Sw_DataParams_t;


/**
* \brief Initialize the SNEP component.
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Parameter structure size is invalid.
*/
phStatus_t phnpSnep_Sw_Init(
                            phnpSnep_Sw_DataParams_t * pDataParams,        /**< [In] Pointer to this layer's parameter structure. */
                            uint16_t wSizeOfDataParams,                    /**< [In] Specifies the size of the data parameter structure. */
                            void * plnLlcpDataparams,                      /**< [In] Pointer to LLCP parameters. */
                            void * psSocket                                /**< [In] Pointer to LLCP transport Socket. */
                            );
/** @} */
#endif /* NXPBUILD__PHNP_SNEP_SW */

#ifdef NXPBUILD__PHNP_SNEP

/** \defgroup phnpSnep NFCForum-SNEP
* \brief This component implements the NFC Forum Simple NDEF Exchange Protocol (SNEP) version 1.0.
*
* <strong>Design Limitation:</strong> SNEP implementation will only allow to initialize maximum of one default SNEP server
* and one non-default SNEP server using \ref phnpSnep_ServerInit API.
*
* This layer follows a multi-threaded RTOS based design using one \ref phlnLlcp_Activate
* "reader library task" and one SNEP Server/Client application task.
* Reader Library Task calls LLCP API's to establish Link Connection on which SNEP Server/Client Task can send SNEP packets.
*
* A pictorial representation of reader library task and SNEP Server application task
* used for SNEP Exchange is shown below.
*
* \image html SNEP_Server_Sequence.png
*
* A pictorial representation of reader library task and SNEP Client application task
* used for SNEP Exchange is shown below.
*
* \image html SNEP_Client_Sequence.png
* @{
*/

/**
* \name SNEP specific error codes.
* \note Do not change the macro values.
*/

/*@{*/
/**
* Status indicating that Received Invalid Request to Server and LLCP Disconnection is done and further communication is not possible over this Link.
* If this error is returned then a new LLC Session needs to be created by Server for further communication.
*/
#define PH_ERR_SNEP_FAILED              0x81U

/**
* Status indicating that Client GET Request Failed as Response sent by Server is other than SUCCESS Response.
*/
#define PH_ERR_SNEP_GET_FAILED          0x82U

/**
* Status indicating that received Client Request contains Invalid Version number.
*/
#define PH_ERR_INVALID_VERSION          0x83U
/*@}*/

/**
 * SNEP frame header size.
 */
#define PHNP_SNEP_HEADER_SIZE           0x06U

/**
*  \brief SNEP Request Codes
* The \ref phnpSnep_ServerListen API returns with one of the following request codes. These are passed to \ref phnpSnep_ServerSendResponse API
* received from Client to Server.
*/
#if 0
typedef enum /* phnpSnep_Request_Codes */ {
    PHNP_SNEP_REQ_CONT        = 0x00,                                      /**< Continue sending remaining fragments. */
    PHNP_SNEP_REQ_GET         = 0x01,                                      /**< Return an NDEF message. */
    PHNP_SNEP_REQ_PUT         = 0x02,                                      /**< Accept an NDEF message. */
    PHNP_SNEP_REQ_REJECT      = 0x7F                                       /**< Client indicates not to send remaining fragments using this request. */
} phnpSnep_Request_Codes_t;
#endif
#define PHNP_SNEP_REQ_CONT        0x00U                                      /**< Continue sending remaining fragments. */
#define PHNP_SNEP_REQ_GET         0x01U                                      /**< Return an NDEF message. */
#define PHNP_SNEP_REQ_PUT         0x02U                                      /**< Accept an NDEF message. */
#define PHNP_SNEP_REQ_REJECT      0x7FU                                       /**< Client indicates not to send remaining fragments using this request. */
/* Removal of generic interfaces */
#ifdef NXPRDLIB_REM_GEN_INTFS
#include "../comps/phnpSnep/src/Sw/phnpSnep_Sw.h"

#define phnpSnep_ClientInit( pDataParams, eServerType,pNonDefaultUri, pbSnepRxBuffer, dwSnepRxBufferSize) \
        phnpSnep_Sw_ClientInit((phnpSnep_Sw_DataParams_t *) pDataParams, eServerType,pNonDefaultUri, pbSnepRxBuffer, dwSnepRxBufferSize)

#define phnpSnep_ServerInit(pDataParams, eServerType,pNonDefaultUri, pbSnepRxBuffer, dwSnepRxBufferSize) \
        phnpSnep_Sw_ServerInit((phnpSnep_Sw_DataParams_t *) pDataParams, eServerType,pNonDefaultUri, pbSnepRxBuffer, dwSnepRxBufferSize)

#define phnpSnep_Put( pDataParams, pTxBuffer, dwTxBufferSize) \
        phnpSnep_Sw_Put((phnpSnep_Sw_DataParams_t *) pDataParams, pTxBuffer, dwTxBufferSize)

#define phnpSnep_Get( pDataParams, pNdefMsg, dwNdefMsgLen,pRxBuffer, pdwRxLen, dwAppBufSize) \
        phnpSnep_Sw_Get((phnpSnep_Sw_DataParams_t *) pDataParams, pNdefMsg, dwNdefMsgLen,pRxBuffer, pdwRxLen, dwAppBufSize)

#define phnpSnep_ServerListen( pDataParams, dwGetReqBuffSize,pGetReqData, dwGetReqLength, pClientReq) \
        phnpSnep_Sw_ServerListen((phnpSnep_Sw_DataParams_t *) pDataParams, dwGetReqBuffSize,pGetReqData, dwGetReqLength, pClientReq)

#define phnpSnep_ServerSendResponse( pDataParams, bClientReq,pRespDataForGet, dwRespDataLen, dwPutBuffSize, pPutData, pdwPutDataLen) \
        phnpSnep_Sw_ServerSendResponse((phnpSnep_Sw_DataParams_t *) pDataParams, bClientReq,pRespDataForGet, dwRespDataLen, dwPutBuffSize, pPutData, pdwPutDataLen)

#define phnpSnep_ClientDeInit( pDataParams) \
        phnpSnep_Sw_ClientDeInit((phnpSnep_Sw_DataParams_t *) pDataParams)

#define phnpSnep_ServerDeInit( pDataParams) \
        phnpSnep_Sw_ServerDeInit((phnpSnep_Sw_DataParams_t *) pDataParams)

#else

/**
* \brief Interface to create and configure SNEP Client.
* This function creates and configures a SNEP Client over LLCP.
* This has to be called first in a client session/Task after \ref phlnLlcp_WaitForActivation.
* If the Server Type is Default Server than pNonDefaultUri parameter is Ignored.
*
* \return Status code
* \retval #PH_ERR_SUCCESS                 Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS     Invalid Component ID.
* \retval Other                           Depending on implementation and underlying component.
*/
phStatus_t phnpSnep_ClientInit(
                               void * pDataParams,                         /**< [In] Pointer to this layer's parameter structure. */
                               phnpSnep_Server_type_t eServerType,         /**< [In] Type of the SNEP Server to which Client has to connect. */
                               uint8_t *pNonDefaultUri,                    /**< [In] Pointer of Uniform Resource Identifier (URI) of Non-Default Server.
                                                                                     NULL terminated URI string should be provided. */
                               uint8_t *pbSnepRxBuffer,                    /**< [In] Pointer to Client Receive Buffer. */
                               uint32_t dwSnepRxBufferSize                 /**< [In] Size of Client Receive Buffer. */
                               );

/**
* \brief Interface to create and configure SNEP Server.
* This function creates and configures a SNEP Server using eServerType parameter over LLCP.
* If the Server Type is Default Server than pNonDefaultUri parameter is Ignored.
*
* This API will be called first in a server session/task after \ref phlnLlcp_WaitForActivation.
* \return Status code
* \retval #PH_ERR_SUCCESS                 Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS     Invalid Component ID.
* \retval Other                           Depending on implementation and underlying component.
*/
phStatus_t phnpSnep_ServerInit(
                               void * pDataParams,                         /**< [In] Pointer to this layer's parameter structure. */
                               phnpSnep_Server_type_t eServerType,         /**< [In] Type of the SNEP Server. */
                               uint8_t *pNonDefaultUri,                    /**< [In] Pointer of Uniform Resource Identifier (URI) of Non-Default Server.
                                                                                     NULL terminated URI string should be provided. */
                               uint8_t *pbSnepRxBuffer,                    /**< [In] Pointer to Server Receive Buffer. */
                               uint32_t dwSnepRxBufferSize                 /**< [In] Size of Server Receive Buffer. */
                               );


/**
* \brief Performs the SNEP PUT Request operation.
*
* This function has to be called after the \ref phnpSnep_ClientInit is done.
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_PROTOCOL_ERROR        Did not receive valid response from Server.
* \retval Other                         Depending on underlying component.
*/
phStatus_t phnpSnep_Put(
                        void * pDataParams,                                /**< [In] Pointer to this layer's parameter structure. */
                        uint8_t * pTxBuffer,                               /**< [In] Pointer of Buffer to be transmitted. */
                        uint32_t dwTxBufferSize                            /**< [In] Length of Buffer to be transmitted. */
                        );

/**
* \brief Performs the SNEP GET Request operation.
* This API should not be called if initialized in phnpSnep_NonDefault_Server. Also this API returns #PH_ERR_SNEP_GET_FAILED\n
* if the remote SNEP Server is Default Server.\n
* This function has to be called only after the \ref phnpSnep_ClientInit is done.
*
* NOTE: Sending Client GET Request in fragments is not supported by this API due to which length of NDEF Message (dwNdefMsgLen) in
* GET Request cannot be more than Remote MIU. If length of NDEF Message is more than remote MIU then #PH_ERR_INVALID_PARAMETER
* error will be returned by this API.
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_PROTOCOL_ERROR        Did not receive valid Response from Server.
* \retval #PH_ERR_BUFFER_OVERFLOW       If Server Response length is more than the specified dwAppBufSize length.
* \retval #PH_ERR_SNEP_GET_FAILED       Received Error response from Remote Server. Client should De-Init and start new session.
* \retval Other                         Depending on implementation and underlying component.
*/
phStatus_t phnpSnep_Get(
                        void * pDataParams,                                /**< [In] Pointer to this layer's parameter structure. */
                        uint8_t *pNdefMsg,                                 /**< [In] Pointer to the NDEF message in the Get Request to be sent to Server. */
                        uint32_t dwNdefMsgLen,                             /**< [In] Length of the NDEF message in the Get Request to be sent to Server. */
                        uint8_t *pRxBuffer,                                /**< [Out] Pointer to the Application Rx Buffer to receive data. */
                        uint32_t *pdwRxLen,                                /**< [Out] Number of bytes received from the Server. */
                        uint32_t dwAppBufSize                              /**< [In]  Maximum acceptable data from Server, normally size of pRxBuffer.*/
                        );

/**
* \brief Used to receive SENP Request from the Remote Client.
* This function Listens for SNEP Client Request and parses the Request type and returns to Application in pbClientReq parameter.
*
* Parameter Usage:\n
* If the Initialized Server Type is Default Server then 'dwGetReqBuffSize', 'pGetReqData', 'dwGetReqLength' parameters are ignored.
*
* This function is immediately called after \ref phnpSnep_ServerInit.
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_INVALID_VERSION       Received Client Request with Invalid Version.
* \retval #PH_ERR_UNSUPPORTED_COMMAND   Received unsupported Request from Client.
* \retval #PH_ERR_BUFFER_OVERFLOW       Message in GET cannot fit in Application Buffer.
* \retval #PH_ERR_SNEP_FAILED           Received Invalid Request, Server session will be closed and new session needs to be created.
* \retval Other                         Depending on implementation and underlying component.
*/
phStatus_t phnpSnep_ServerListen(
                                 void * pDataParams,                       /**< [In] Pointer to this layer's parameter structure. */
                                 uint32_t   dwGetReqBuffSize,              /**< [In] Size of Buffer to store received NDEF Message in Get Request. */
                                 uint8_t  * pGetReqData,                   /**< [Out] Pointer to the application Buffer to store received Message in Get Request. */
                                 uint32_t * dwGetReqLength,                /**< [Out] Received data length in Message in GET Request from Client. */
                                 uint8_t  * pClientReq                     /**< [Out] Pointer to the Type of received Client Request \ref phnpSnep_Request_Codes. */
                                 );

/**
* \brief Used to send Response to the SNEP Request delivered by \ref phnpSnep_ServerListen.
* This function will receive in chunks if more data to be received in case of PUT Request\n
* and copies it to 'dwPutBuffSize' Buffer and performs the required SNEP Server Response.\n
* This function will send SNEP Success Response for GET Request in chunks based on Remote MIU.\n
*
* Parameter Usage:\n
* 1. If bClientReq is PUT request, then 'pPutData', 'dwPutBuffSize' and 'pdwPutDataLen' have to be provided by the caller where PUT data will be copied to.\n
* 2. If the SNEP PUT header indicates that the client is putting more data than the dwBufferSize, then Excess Data response is sent to the client.\n
* 3. If bClientReq is GET request, then Message pointed by pRespDataForGet of dwRespDataLen length will be sent as Response to the client.\n
* If no Data needs to be sent in response for GET Request then dwRespDataLen should be zero. \n
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. \ref phnpSnep_ServerInit()\n
* 2. \ref phnpSnep_ServerListen()\n
*
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval #PH_ERR_INVALID_PARAMETER     Invalid input parameters.
* \retval #PH_ERR_BUFFER_OVERFLOW       Message in PUT cannot be fitted in Application Buffer.
* \retval #PH_ERR_PROTOCOL_ERROR        Received wrong SNEP Request from Client.
*/
phStatus_t phnpSnep_ServerSendResponse(
                                       void * pDataParams,                 /**< [In] Pointer to this layer's parameter structure. */
                                       uint8_t bClientReq,                 /**< [In] Type of received Client Request. Should only be PHNP_SNEP_REQ_GET or PHNP_SNEP_REQ_PUT. */
                                       uint8_t *pRespDataForGet,           /**< [In] Pointer to application provided data to handle GET Request from Client. */
                                       uint32_t dwRespDataLen,             /**< [In] Data length to be transmitted to Client for GET Request. */
                                       uint32_t dwPutBuffSize,             /**< [In] Size of the Data buffer given by the application to hold the received PUT Request Data. */
                                       uint8_t *pPutData,                  /**< [Out] Pointer to Data Buffer given by the application to hold the received PUT Request Data from client. */
                                       uint32_t *pdwPutDataLen             /**< [Out] Actual length of PUT Request Data received from the Client. */
                                       );

/**
* \brief SNEP Client De-Initialization.
* This function De-Initializes the SNEP Client session.
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. \ref phnpSnep_ClientInit()\n
* 2. \ref phnpSnep_Put() or \ref phnpSnep_Get()\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval Other                         Depending on implementation and underlying component.
*/
phStatus_t phnpSnep_ClientDeInit(
                                 void * pDataParams                        /**< [In] Pointer to this layer's parameter structure. */
                                 );


/**
* \brief SNEP Server De-Initialization.
* This function De-Initializes the SNEP Server session.
*
* <em>Sequence of functions that needs to be called prior to this API are as below:</em>\n
* 1. \ref phnpSnep_ServerInit()\n
* 2. \ref phnpSnep_ServerListen()\n
* 3. \ref phnpSnep_ServerSendResponse()\n
* \return Status code
* \retval #PH_ERR_SUCCESS               Operation successful.
* \retval #PH_ERR_INVALID_DATA_PARAMS   Invalid Component ID.
* \retval Other                         Depending on implementation and underlying component.
*/
phStatus_t phnpSnep_ServerDeInit(
                                 void * pDataParams                        /**< [In] Pointer to this layer's parameter structure. */
                                 );
/** @} */

#endif /* NXPRDLIB_REM_GEN_INTFS */

#endif /* NXPBUILD__PHNP_SNEP */

#ifdef __cplusplus
} /* Extern C */
#endif

#endif /* PHNPSNEP_H */
