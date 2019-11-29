/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       Diagnostics.c
\brief      Diagnostics Cluster
==============================================================================*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#include "zcl.h"
#include "Diagnostics.h"
#include "zcl_options.h"

#if defined CPU_MKW41Z512VHT4
#include "TimersManager.h"
#include "MemManager.h"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
PRIVATE teZCL_Status eCLD_DiagnosticsCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance);

#if defined DIAGNOSTICS_CLIENT && defined DIAGNOSTICS_PTP
PRIVATE  teZCL_Status eCLD_DiagnosticsHandleDelayResponseCommand(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint8                       u8CommandIdentifier);

PUBLIC teZCL_Status eCLD_DiagnosticsDelayCommandSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Diagnostics_CommandPayload   *pCommand);
#endif

#if defined DIAGNOSTICS_SERVER && defined DIAGNOSTICS_PTP
PRIVATE  teZCL_Status eCLD_DiagnosticsHandleDelayRequestCommand(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint8                       u8CommandIdentifier);

PUBLIC teZCL_Status eCLD_DiagnosticsDelayResponseCommandSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Diagnostics_DelayResponsePayload *psPayload);
#endif

/****************************************************************************/
/***        Exported Function Variables                                            ***/
/****************************************************************************/
#if ((defined JENNIC_CHIP_FAMILY) &&  ((JENNIC_CHIP_FAMILY == JN516x) || (JENNIC_CHIP_FAMILY == JN517x)))
extern void MAC_vInitBbcTxTries(void);
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
#ifdef CLD_DIAGNOSTICS_ATTR_ID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT
extern uint16 u16AvApsMessageTxRetry;
#endif
#ifdef CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_LQI
extern uint8 u8LastLqi;
#endif
#ifdef CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_RSSI
extern uint8 u8LastRssi;
#endif
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
const tsZCL_AttributeDefinition asCLD_DiagnosticsClusterAttributeDefinitions[] = {
#ifdef DIAGNOSTICS_SERVER
            /* Hardware Information attribute set attribute ID's (ZCL Spec r6 3.15.2.2.1) */
    #ifdef CLD_DIAGNOSTICS_ATTR_ID_NUMBER_OF_RESETS
            {E_CLD_DIAGNOSTICS_ATTR_ID_NUMBER_OF_RESETS,          E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16NumberOfResets),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_PERSISTENT_MEMORY_WRITES
            {E_CLD_DIAGNOSTICS_ATTR_ID_PERSISTENT_MEMORY_WRITES,  E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16PersistentMemoryWrites),0},
    #endif

            /* Stack/Network Information attribute set attribute ID's (ZCL Spec r6 3.15.2.2.2) */
    #ifdef CLD_DIAGNOSTICS_ATTR_ID_MAC_RX_BCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_MAC_RX_BCAST,              E_ZCL_AF_RD,                E_ZCL_UINT32,   (uint32)(&((tsCLD_Diagnostics*)(0))->u32MacRxBcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_BCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_BCAST,              E_ZCL_AF_RD,                E_ZCL_UINT32,   (uint32)(&((tsCLD_Diagnostics*)(0))->u32MacTxBcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_MAC_RX_UCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_MAC_RX_UCAST,              E_ZCL_AF_RD,                E_ZCL_UINT32,   (uint32)(&((tsCLD_Diagnostics*)(0))->u32MacRxUcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_UCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_UCAST,              E_ZCL_AF_RD,                E_ZCL_UINT32,   (uint32)(&((tsCLD_Diagnostics*)(0))->u32MacTxUcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_UCAST_RETRY
            {E_CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_UCAST_RETRY,        E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16MacTxUcastRetry),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_UCAST_FAIL
            {E_CLD_DIAGNOSTICS_ATTR_ID_MAC_TX_UCAST_FAIL,         E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16MacTxUcastFail),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_RX_BCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_RX_BCAST,              E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ApsRxBcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_TX_BCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_TX_BCAST,              E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ApsTxBcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_RX_UCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_RX_UCAST,              E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ApsRxUcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_TX_UCAST_SUCCESS
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_TX_UCAST_SUCCESS,      E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ApsTxUcastSuccess),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_TX_UCAST_RETRY
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_TX_UCAST_RETRY,         E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ApsTxUcastRetry),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_TX_UCAST_FAIL
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_TX_UCAST_FAIL,          E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ApsTxUcastFail),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_ROUTE_DISC_INITIATED
            {E_CLD_DIAGNOSTICS_ATTR_ID_ROUTE_DISC_INITIATED,       E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16RouteDiscInitiated),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_NEIGHBOR_ADDED
            {E_CLD_DIAGNOSTICS_ATTR_ID_NEIGHBOR_ADDED,             E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16NeighborAdded),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_NEIGHBOR_REMOVED
            {E_CLD_DIAGNOSTICS_ATTR_ID_NEIGHBOR_REMOVED,           E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16NeighborRemoved),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_NEIGHBOR_STALE
            {E_CLD_DIAGNOSTICS_ATTR_ID_NEIGHBOR_STALE,             E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16NeighborStale),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_JOIN_INDICATION
            {E_CLD_DIAGNOSTICS_ATTR_ID_JOIN_INDICATION,            E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16JoinIndication),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_CHILD_MOVED
            {E_CLD_DIAGNOSTICS_ATTR_ID_CHILD_MOVED,                E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ChildMoved),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_NWK_FC_FAILURE
            {E_CLD_DIAGNOSTICS_ATTR_ID_NWK_FC_FAILURE,             E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16NWKFCFailure),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_FC_FAILURE
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_FC_FAILURE,             E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16APSFCFailure),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_UNAUTHORIZED_KEY
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_UNAUTHORIZED_KEY,       E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16APSUnauthorizedKey),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_NWK_DECRYPT_FAILURE
            {E_CLD_DIAGNOSTICS_ATTR_ID_NWK_DECRYPT_FAILURE,        E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16NWKDecryptFailure),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_APS_DECRYPT_FAILURE
            {E_CLD_DIAGNOSTICS_ATTR_ID_APS_DECRYPT_FAILURE,        E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16APSDecryptFailure),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_PACKET_BUFFER_ALLOCATE_FAILURE
            {E_CLD_DIAGNOSTICS_ATTR_ID_PACKET_BUFFER_ALLOCATE_FAILURE,E_ZCL_AF_RD,             E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16PacketBufferAllocateFailure),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_RELAYED_UCAST
            {E_CLD_DIAGNOSTICS_ATTR_ID_RELAYED_UCAST,              E_ZCL_AF_RD,                E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16RelayedUcast),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_PHY_TO_MAC_QUEUE_LIMIT_REACHED
            {E_CLD_DIAGNOSTICS_ATTR_ID_PHY_TO_MAC_QUEUE_LIMIT_REACHED,E_ZCL_AF_RD,             E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16PhyToMACQueueLimitReached),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_PACKET_VALIDATE_DROP_COUNT
            {E_CLD_DIAGNOSTICS_ATTR_ID_PACKET_VALIDATE_DROP_COUNT,  E_ZCL_AF_RD,               E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16PacketValidateDropCount),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT
            {E_CLD_DIAGNOSTICS_ATTR_ID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT,  E_ZCL_AF_RD,   E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16AverageMACRetryPerAPSMessageSent),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_LQI
            {E_CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_LQI,            E_ZCL_AF_RD,               E_ZCL_UINT8,   (uint32)(&((tsCLD_Diagnostics*)(0))->u8LastMessageLQI),0},
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_RSSI
            {E_CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_RSSI,            E_ZCL_AF_RD,              E_ZCL_INT8,   (uint32)(&((tsCLD_Diagnostics*)(0))->i8LastMessageRSSI),0},
    #endif
#endif    
            {E_CLD_GLOBAL_ATTR_ID_CLUSTER_REVISION,                 (E_ZCL_AF_RD|E_ZCL_AF_GA), E_ZCL_UINT16,   (uint32)(&((tsCLD_Diagnostics*)(0))->u16ClusterRevision),0},   /* Mandatory  */
 };

tsZCL_ClusterDefinition sCLD_Diagnostics = {
        GENERAL_CLUSTER_ID_DIAGNOSTICS,
        FALSE,
        E_ZCL_SECURITY_NETWORK,
        (sizeof(asCLD_DiagnosticsClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition)),
        (tsZCL_AttributeDefinition*)asCLD_DiagnosticsClusterAttributeDefinitions,
        NULL
};

uint8 au8DiagnosticsClusterAttributeControlBits[(sizeof(asCLD_DiagnosticsClusterAttributeDefinitions) / sizeof(tsZCL_AttributeDefinition))];

#if defined DIAGNOSTICS_SERVER && defined DIAGNOSTICS_PTP
/* Variable to store request latency when receiving multicast delay requests */
zint32 savedRequestLatency = 0;
zuint8 savedSequenceNumber = 0;
#endif
/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsCreateDiagnostics
 **
 ** DESCRIPTION:
 ** Creates a diagnostic cluster
 **
 ** PARAMETERS:                 Name                        Usage
 ** tsZCL_ClusterInstance    *psClusterInstance             Cluster structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_DiagnosticsCreateDiagnostics(
                tsZCL_ClusterInstance              *psClusterInstance,
                bool_t                              bIsServer,
                tsZCL_ClusterDefinition            *psClusterDefinition,
                void                               *pvEndPointSharedStructPtr,
                uint8                              *pu8AttributeControlBits)
{
    #ifdef STRICT_PARAM_CHECK 
        /* Parameter check */
        if((psClusterInstance==NULL) ||
           (psClusterDefinition==NULL))
        {
            return E_ZCL_ERR_PARAMETER_NULL;
        }
    #endif

    /* Create an instance of a diagnostics cluster */
    vZCL_InitializeClusterInstance(
                           psClusterInstance, 
                           bIsServer,
                           psClusterDefinition,
                           pvEndPointSharedStructPtr,
                           pu8AttributeControlBits,
                           NULL,
                           eCLD_DiagnosticsCommandHandler);

    if(psClusterInstance->pvEndPointSharedStructPtr != NULL)
    {
        /* Initialise attribute defaults */
        ((tsCLD_Diagnostics*)psClusterInstance->pvEndPointSharedStructPtr)->u16ClusterRevision = CLD_DIAGNOSTICS_CLUSTER_REVISION ;
    }
#if ((defined JENNIC_CHIP_FAMILY) &&  ((JENNIC_CHIP_FAMILY == JN516x) || (JENNIC_CHIP_FAMILY == JN517x)))
    /* Initialise attribute defaults */
    /* All the deafult values are zero for this cluster hence no need to initialise explicly - save some code */
#ifndef JENNIC_MAC_MiniMacShim

    /*Initialise Stack/NWK to provide the attribute values */
    MAC_vInitBbcTxTries();
#endif
#endif
    return E_ZCL_SUCCESS;
}

#if defined DIAGNOSTICS_SERVER
/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsUpdate
 **
 ** DESCRIPTION:
 ** Updates the the state of an Diagnostic cluster
 **
 ** PARAMETERS:                 Name                        Usage
 ** uint8                       u8SourceEndPointId          Source EP Id
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_DiagnosticsUpdate(uint8 u8SourceEndPointId)
{
    teZCL_Status eStatus;
    tsZCL_EndPointDefinition *psEndPointDefinition;
    tsCLD_DiagnosticsCustomDataStructure *psCustomDataStructure;
#if defined( CLD_DIAGNOSTICS_ATTR_ID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT) || defined (CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_LQI)
    tsCLD_Diagnostics *psSharedStruct;
#endif
    tsZCL_ClusterInstance *psClusterInstance;/* Find pointers to cluster */
    eStatus = eZCL_FindCluster(GENERAL_CLUSTER_ID_DIAGNOSTICS, u8SourceEndPointId, TRUE, &psEndPointDefinition, &psClusterInstance, (void*)&psCustomDataStructure);
    if(eStatus != E_ZCL_SUCCESS)
    {
        return eStatus;
    }

    /* get EP mutex */
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif
    
    #if defined( CLD_DIAGNOSTICS_ATTR_ID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT) || defined (CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_LQI)
         psSharedStruct = (tsCLD_Diagnostics*)psClusterInstance->pvEndPointSharedStructPtr;
    #endif
    /*
     * Get all the Attributes Updated from the stack/nwk layer
     */
    #ifdef CLD_DIAGNOSTICS_ATTR_ID_AVERAGE_MAC_RETRY_PER_APS_MESSAGE_SENT
        psSharedStruct->u16AverageMACRetryPerAPSMessageSent=u16AvApsMessageTxRetry;
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_LQI
        psSharedStruct->u8LastMessageLQI = u8LastLqi;
    #endif

    #ifdef CLD_DIAGNOSTICS_ATTR_ID_LAST_MESSAGE_RSSI
        psSharedStruct->i8LastMessageRSSI = u8LastRssi;
    #endif

     /* release EP mutex */
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif

    return(E_ZCL_SUCCESS);
}
#endif

/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsCommandSend
 **
 ** DESCRIPTION:
 ** Send a Diagnostics command
 **
 ** PARAMETERS:                 Name                            Usage
 ** uint8                       u8SourceEndPointId              Source EP Id
 ** uint8                       u8DestinationEndPointId         Destination EP Id
 ** tsZCL_Address               *psDestinationAddress           Pointer to destination address
 ** uint8                       *pu8TransactionSequenceNumber   Pointer to transaction sequence number
 ** teCLD_Diagnostics_Command   *pCommand                       Pointer to Command structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_DiagnosticsCommandSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Diagnostics_CommandPayload   *pCommand)
{
    teZCL_Status status = E_ZCL_ERR_ATTRIBUTE_TYPE_UNSUPPORTED;

    switch(pCommand->cmdId)
    {
#if defined DIAGNOSTICS_CLIENT && defined DIAGNOSTICS_PTP
        case E_CLD_DIAGNOSTICS_CMD_DELAY_REQUEST:
        case E_CLD_DIAGNOSTICS_CMD_DELAY_RESULTS:
            status =  eCLD_DiagnosticsDelayCommandSend(u8SourceEndPointId,
                                                              u8DestinationEndPointId,
                                                              psDestinationAddress,
                                                              pu8TransactionSequenceNumber,
                                                              pCommand);
            break;
#endif

         default:
            break;
    }    

    return status;
}

#if defined DIAGNOSTICS_CLIENT && defined DIAGNOSTICS_PTP
/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsDelayCommandSend
 **
 ** DESCRIPTION:
 ** Sends a Diagnostics Delay command
 **
 ** PARAMETERS:                 Name                            Usage
 ** uint8                       u8SourceEndPointId              Source EP Id
 ** uint8                       u8DestinationEndPointId         Destination EP Id
 ** tsZCL_Address               *psDestinationAddress           Pointer to destination address
 ** uint8                       *pu8TransactionSequenceNumber   Pointer to transaction sequence number
 ** teCLD_Diagnostics_Command   *pCommand                       Pointer to Command structure
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_DiagnosticsDelayCommandSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Diagnostics_CommandPayload   *pCommand)
{
    teZCL_Status status = E_ZCL_ERR_PARAMETER_RANGE;

    if(pCommand->cmdId == E_CLD_DIAGNOSTICS_CMD_DELAY_REQUEST)
    {
        tsCLD_Diagnostics_DelayRequestPayload sRequest = {0};
        tsZCL_TxPayloadItem asPayloadDefinition[] = {
                    {1,                        E_ZCL_UINT32,  &sRequest.u32TxRequestTimestamp},
                    {1,                        E_ZCL_UINT8,   &sRequest.sequenceNumber},
                    {1,                        E_ZCL_UINT8,   &sRequest.payloadSize},
                    {pCommand->payloadSize,    E_ZCL_UINT8,   pCommand->payload},
                                                      };
#if defined CPU_MKW41Z512VHT4
        /* Get TX request timestamp */
        sRequest.u32TxRequestTimestamp = (uint32_t)(TMR_GetTimestamp() / 1000);
#endif
        sRequest.sequenceNumber = pCommand->sequenceNumber;
        sRequest.payloadSize = pCommand->payloadSize;
        sRequest.payload = pCommand->payload;

        status =  eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_DIAGNOSTICS,
                                  FALSE,
                                  pCommand->cmdId,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );
    }
    else if(pCommand->cmdId == E_CLD_DIAGNOSTICS_CMD_DELAY_RESULTS)
    {
        status =  eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_DIAGNOSTICS,
                                  FALSE,
                                  pCommand->cmdId,
                                  pu8TransactionSequenceNumber,
                                  NULL,
                                  FALSE,
                                  0,
                                  0);
    }

    return status;
}
#endif

#if defined DIAGNOSTICS_SERVER && defined DIAGNOSTICS_PTP
/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsDelayResponseCommandSend
 **
 ** DESCRIPTION:
 ** Sends a Diagnostics Delay response command
 **
 ** PARAMETERS:                              Name                            Usage
 ** uint8                                    u8SourceEndPointId              Source EP Id
 ** uint8                                    u8DestinationEndPointId         Destination EP Id
 ** tsZCL_Address                            *psDestinationAddress           Pointer to destination address
 ** uint8                                    *pu8TransactionSequenceNumber   Pointer to transaction sequence number
 ** tsCLD_Diagnostics_DelayResponsePayload   *psPayload                      Pointer to payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_DiagnosticsDelayResponseCommandSend(
                uint8           u8SourceEndPointId,
                uint8           u8DestinationEndPointId,
                tsZCL_Address   *psDestinationAddress,
                uint8           *pu8TransactionSequenceNumber,
                tsCLD_Diagnostics_DelayResponsePayload *psPayload)
{
    tsZCL_TxPayloadItem asPayloadDefinition[] = {
                {1,                         E_ZCL_INT32,   &psPayload->i32RequestLatency},
                {1,                         E_ZCL_UINT32,  &psPayload->u32TxResponseTimestamp},
                {1,                         E_ZCL_UINT8,   &psPayload->sequenceNumber},
                {1,                         E_ZCL_UINT8,   &psPayload->payloadSize},
                {psPayload->payloadSize,    E_ZCL_UINT8,   psPayload->payload},
                                                  };

#if defined CPU_MKW41Z512VHT4
    psPayload->u32TxResponseTimestamp = (uint32_t)(TMR_GetTimestamp() / 1000);
#endif
    return eZCL_CustomCommandSend(u8SourceEndPointId,
                                  u8DestinationEndPointId,
                                  psDestinationAddress,
                                  GENERAL_CLUSTER_ID_DIAGNOSTICS,
                                  TRUE,
                                  E_CLD_DIAGNOSTICS_CMD_DELAY_RESPONSE,
                                  pu8TransactionSequenceNumber,
                                  asPayloadDefinition,
                                  FALSE,
                                  0,
                                  sizeof(asPayloadDefinition) / sizeof(tsZCL_TxPayloadItem)
                                 );
}
#endif

/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsCommandHandler
 **
 ** DESCRIPTION:
 ** Handles received diagnostics commands
 **
 ** PARAMETERS:                 Name                     Usage
 ** ZPS_tsAfEvent               *pZPSevent               Pointer to ZPS event structure
 ** tsZCL_EndPointDefinition    *psEndPointDefinition    Pointer to end point definition
 ** tsZCL_ClusterInstance       *psClusterInstance       Pointer to cluster instance
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC  teZCL_Status eCLD_DiagnosticsCommandHandler(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance)
{
    tsZCL_HeaderParams sZCL_HeaderParams;
    teZCL_Status eStatus = E_ZCL_SUCCESS;
    // further error checking can only be done once we have interrogated the ZCL payload
    u16ZCL_ReadCommandHeader(pZPSevent->uEvent.sApsDataIndEvent.hAPduInst,
                             &sZCL_HeaderParams);

{
    // get EP mutex
    #ifndef COOPERATIVE
        eZCL_GetMutex(psEndPointDefinition);
    #endif

    switch(sZCL_HeaderParams.u8CommandIdentifier)
    {
#if defined DIAGNOSTICS_SERVER && defined DIAGNOSTICS_PTP
    case E_CLD_DIAGNOSTICS_CMD_DELAY_REQUEST:
    case E_CLD_DIAGNOSTICS_CMD_DELAY_RESULTS:
        eStatus = eCLD_DiagnosticsHandleDelayRequestCommand(pZPSevent, psEndPointDefinition, psClusterInstance, sZCL_HeaderParams.u8CommandIdentifier);
        break;
#endif

#if defined DIAGNOSTICS_CLIENT && defined DIAGNOSTICS_PTP
    case E_CLD_DIAGNOSTICS_CMD_DELAY_RESPONSE:
        eStatus = eCLD_DiagnosticsHandleDelayResponseCommand(pZPSevent, psEndPointDefinition, psClusterInstance, sZCL_HeaderParams.u8CommandIdentifier);
        break;
#endif

    default:
        // unlock
        #ifndef COOPERATIVE
            eZCL_ReleaseMutex(psEndPointDefinition);
        #endif

        eStatus = E_ZCL_ERR_CUSTOM_COMMAND_HANDLER_NULL_OR_RETURNED_ERROR;
        break;
    }

    // unlock
    #ifndef COOPERATIVE
        eZCL_ReleaseMutex(psEndPointDefinition);
    #endif
}

    // delete the i/p buffer on return
    return eStatus;
}

#if defined DIAGNOSTICS_SERVER && defined DIAGNOSTICS_PTP
/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsCommandDelayRequestReceive
 **
 ** DESCRIPTION:
 ** Receives Diagnostics Delay request command and parses the payload
 **
 ** PARAMETERS:                               Name                          Usage
 ** ZPS_tsAfEvent                            *pZPSevent                     Zigbee stack event structure
 ** uint8                                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Diagnostics_DelayRequestPayload    *psPayload                     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_DiagnosticsCommandDelayRequestReceive(
                ZPS_tsAfEvent               *pZPSevent,
                uint8                       *pu8TransactionSequenceNumber,
                tsCLD_Diagnostics_DelayRequestPayload *psPayload)
{
    uint8_t *pPayloadLength = pZPSevent->uEvent.sApsDataIndEvent.hAPduInst->au8Storage + 0x08;
    uint16 u16ActualQuantity;
    teZCL_Status status = E_ZCL_FAIL;

    psPayload->payloadSize = *pPayloadLength;
    
    if(psPayload->payloadSize != 0)
    {
#if defined CPU_MKW41Z512VHT4
        psPayload->payload = (zuint8 *)MEM_BufferAlloc(psPayload->payloadSize);
#endif
        tsZCL_RxPayloadItem asPayloadDefinition[] = {
                {1,                        &u16ActualQuantity, E_ZCL_UINT32,    &psPayload->u32TxRequestTimestamp},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,     &psPayload->sequenceNumber},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,     &psPayload->payloadSize},
                {psPayload->payloadSize,   &u16ActualQuantity, E_ZCL_UINT8,     psPayload->payload},
                                                     };

        status =  eZCL_CustomCommandReceive(pZPSevent,
                                         pu8TransactionSequenceNumber,
                                         asPayloadDefinition,
                                         sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                         E_ZCL_ACCEPT_EXACT);
    }
    else
    {
        tsZCL_RxPayloadItem asNoPayloadDefinition[] = {
                {1,                        &u16ActualQuantity, E_ZCL_UINT32,    &psPayload->u32TxRequestTimestamp},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,     &psPayload->sequenceNumber},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,     &psPayload->payloadSize},
                                                     };

        status =  eZCL_CustomCommandReceive(pZPSevent,
                                         pu8TransactionSequenceNumber,
                                         asNoPayloadDefinition,
                                         sizeof(asNoPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                         E_ZCL_ACCEPT_EXACT);
    }

    return status;
}

/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsHandleDelayRequestCommand
 **
 ** DESCRIPTION:
 ** Handles Diagnostics Delay request command
 **
 ** PARAMETERS:               Name                      Usage
 ** ZPS_tsAfEvent            *pZPSevent                 Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition      EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance         Cluster structure
 ** uint8                     u8CommandIdentifier       Command Id
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PRIVATE  teZCL_Status eCLD_DiagnosticsHandleDelayRequestCommand(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint8                       u8CommandIdentifier)
{
    teZCL_Status eStatus;
    uint8 u8TransactionSequenceNumber;

    /* Payload to be sent back */
    tsCLD_Diagnostics_DelayResponsePayload rPayload = {0};
    bool toBeTransmited = FALSE;
    /* Received payload */
    tsCLD_Diagnostics_DelayRequestPayload sPayload = {0};

    if(u8CommandIdentifier == E_CLD_DIAGNOSTICS_CMD_DELAY_REQUEST)
    {
        /* Receive the command */
        eStatus = eCLD_DiagnosticsCommandDelayRequestReceive(pZPSevent,
                                    &u8TransactionSequenceNumber,
                                    &sPayload);

        if(eStatus != E_ZCL_SUCCESS)
        {
            return(E_ZCL_FAIL);
        }

#if defined CPU_MKW41Z512VHT4
        /* Get RX request timestamp and calculate the request latency, signed value */
        rPayload.i32RequestLatency = (zint32)((uint32_t)(TMR_GetTimestamp() / 1000) - sPayload.u32TxRequestTimestamp);
#endif
        /* Get sequence number and return it to client */
        rPayload.sequenceNumber = sPayload.sequenceNumber;
        
        /* Get payload and return it to client */
        if((sPayload.payloadSize != 0) && (sPayload.payload != NULL))
        {
            rPayload.payloadSize = sPayload.payloadSize;
            rPayload.payload = sPayload.payload;
        }
        
        /* For delay request received on unicast short or extended address */
        if(((pZPSevent->uEvent.sApsDataIndEvent.u8DstAddrMode == E_ZCL_AM_SHORT) &&
            (pZPSevent->uEvent.sApsDataIndEvent.uDstAddress.u16Addr < 0xfff8)) ||
            (pZPSevent->uEvent.sApsDataIndEvent.u8DstAddrMode == E_ZCL_AM_IEEE))
        {
            toBeTransmited = TRUE;
        }
        /* For delay request received on broadcast short address */
        else if((pZPSevent->uEvent.sApsDataIndEvent.uDstAddress.u16Addr >= 0xfffb) &&
                (pZPSevent->uEvent.sApsDataIndEvent.u8DstAddrMode == E_ZCL_AM_BROADCAST))
        {
            /* Save values of request latency and sequence number, to be returned to client with results command sent unicast */
            savedRequestLatency = rPayload.i32RequestLatency;
            savedSequenceNumber = rPayload.sequenceNumber;
            eStatus = E_ZCL_SUCCESS;
        }
        /* For reserved addresses (fff8-fffa) and other cases */
        else
        {
            eStatus = E_ZCL_FAIL;
        }
    }
    else if(u8CommandIdentifier == E_CLD_DIAGNOSTICS_CMD_DELAY_RESULTS)
    {
        /* For delay results response received on unicast short address */
        if((pZPSevent->uEvent.sApsDataIndEvent.u8DstAddrMode == E_ZCL_AM_SHORT) &&
         (pZPSevent->uEvent.sApsDataIndEvent.uDstAddress.u16Addr < 0xfffb))
        {
            rPayload.i32RequestLatency = savedRequestLatency;
            rPayload.sequenceNumber = savedSequenceNumber;
            toBeTransmited = TRUE;
        }
    }

    if(toBeTransmited)
    {
        tsZCL_Address   sDestinationAddress = {0};
        /* Set the destination for the returned package to the client */
        sDestinationAddress.eAddressMode = pZPSevent->uEvent.sApsDataIndEvent.u8SrcAddrMode;
        sDestinationAddress.uAddress.u16DestinationAddress = pZPSevent->uEvent.sApsDataIndEvent.uSrcAddress.u16Addr;

        eStatus = eCLD_DiagnosticsDelayResponseCommandSend(pZPSevent->uEvent.sApsDataIndEvent.u8SrcEndpoint,
                                                           pZPSevent->uEvent.sApsDataIndEvent.u8DstEndpoint,
                                                           &sDestinationAddress, &u8TransactionSequenceNumber,
                                                           &rPayload);
    }

    if(sPayload.payload != NULL)
    {
#if defined CPU_MKW41Z512VHT4
        MEM_BufferFree((void *)sPayload.payload);
#endif
    }

    if(rPayload.payload != NULL)
    {
#if defined CPU_MKW41Z512VHT4
        MEM_BufferFree((void *)rPayload.payload);
#endif
    }

    return eStatus;
}
#endif

#if defined DIAGNOSTICS_CLIENT && defined DIAGNOSTICS_PTP
/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsCommandDelayResponseReceive
 **
 ** DESCRIPTION:
 ** Receives Diagnostics Delay response command and parses the payload
 **
 ** PARAMETERS:                               Name                          Usage
 ** ZPS_tsAfEvent                            *pZPSevent                     Zigbee stack event structure
 ** uint8                                    *pu8TransactionSequenceNumber  Sequence number Pointer
 ** tsCLD_Diagnostics_DelayRequestPayload    *psPayload                     Payload
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PUBLIC teZCL_Status eCLD_DiagnosticsCommandDelayResponseReceive(
                ZPS_tsAfEvent               *pZPSevent,
                uint8                       *pu8TransactionSequenceNumber,
                tsCLD_Diagnostics_DelayResponsePayload *psPayload)
{
    uint8_t *pPayloadLength = pZPSevent->uEvent.sApsDataIndEvent.hAPduInst->au8Storage + 0x0C;
    uint16 u16ActualQuantity;
    teZCL_Status status = E_ZCL_FAIL;
    
    psPayload->payloadSize = *pPayloadLength;

    if(psPayload->payloadSize != 0)
    {
#if defined CPU_MKW41Z512VHT4
        psPayload->payload = (zuint8 *)MEM_BufferAlloc(psPayload->payloadSize);
#endif
        tsZCL_RxPayloadItem asPayloadDefinition[] = {
                {1,                        &u16ActualQuantity, E_ZCL_INT32,    &psPayload->i32RequestLatency},
                {1,                        &u16ActualQuantity, E_ZCL_UINT32,   &psPayload->u32TxResponseTimestamp},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,    &psPayload->sequenceNumber},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,    &psPayload->payloadSize},
                {psPayload->payloadSize,   &u16ActualQuantity, E_ZCL_UINT8,    psPayload->payload},
                                                     };

        status = eZCL_CustomCommandReceive(pZPSevent,
                                         pu8TransactionSequenceNumber,
                                         asPayloadDefinition,
                                         sizeof(asPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                         E_ZCL_ACCEPT_EXACT);
    }
    else
    {
        tsZCL_RxPayloadItem asNoPayloadDefinition[] = {
                {1,                        &u16ActualQuantity, E_ZCL_INT32,    &psPayload->i32RequestLatency},
                {1,                        &u16ActualQuantity, E_ZCL_UINT32,   &psPayload->u32TxResponseTimestamp},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,    &psPayload->sequenceNumber},
                {1,                        &u16ActualQuantity, E_ZCL_UINT8,    &psPayload->payloadSize},
                                                     };

        status = eZCL_CustomCommandReceive(pZPSevent,
                                         pu8TransactionSequenceNumber,
                                         asNoPayloadDefinition,
                                         sizeof(asNoPayloadDefinition) / sizeof(tsZCL_RxPayloadItem),
                                         E_ZCL_ACCEPT_EXACT);
    }

    return status;
}

/****************************************************************************
 **
 ** NAME:       eCLD_DiagnosticsHandleDelayResponseCommand
 **
 ** DESCRIPTION:
 ** Handles Diagnostics Delay response command
 **
 ** PARAMETERS:               Name                      Usage
 ** ZPS_tsAfEvent            *pZPSevent                 Zigbee stack event structure
 ** tsZCL_EndPointDefinition *psEndPointDefinition      EP structure
 ** tsZCL_ClusterInstance    *psClusterInstance         Cluster structure
 ** uint8                     u8CommandIdentifier       Command Id
 **
 ** RETURN:
 ** teZCL_Status
 **
 ****************************************************************************/
PRIVATE teZCL_Status eCLD_DiagnosticsHandleDelayResponseCommand(
                    ZPS_tsAfEvent               *pZPSevent,
                    tsZCL_EndPointDefinition    *psEndPointDefinition,
                    tsZCL_ClusterInstance       *psClusterInstance,
                    uint8                       u8CommandIdentifier)
{
    teZCL_Status eStatus;
    uint8 u8TransactionSequenceNumber;
    tsZCL_CallBackEvent sZCL_CallBackEvent;
    tsCLD_DiagnosticsCallBackMessage sCallBackMessage = {0};
    tsCLD_Diagnostics_DelayResponsePayload rPayload = {0};
    zint32 iTemp = 0;

    /* Receive the command */
    eStatus = eCLD_DiagnosticsCommandDelayResponseReceive(pZPSevent,
                                &u8TransactionSequenceNumber,
                                &rPayload);

    if(eStatus != E_ZCL_SUCCESS)
    {
        return(E_ZCL_FAIL);
    }

    /* Calculate response latency, offset and put information in the package that is returned to the application */
    /* Return information is unsigned, so transform value from signed to unsigned */
    if(rPayload.i32RequestLatency < 0)
    {
        sCallBackMessage.u32RequestLatency = (zuint32)(0xFFFFFFFF - rPayload.i32RequestLatency);
    }
    else
    {
        sCallBackMessage.u32RequestLatency = (zuint32)rPayload.i32RequestLatency;
    }

#if defined CPU_MKW41Z512VHT4
    iTemp = (zint32)(rPayload.u32TxResponseTimestamp - (uint32_t)(TMR_GetTimestamp() / 1000));
#endif
    if(iTemp < 0)
    {
        sCallBackMessage.u32ResponseLatency = (zuint32)(0xFFFFFFFF - iTemp);
    }
    else
    {
        sCallBackMessage.u32ResponseLatency = (zuint32)iTemp;
    }
    
    if(sCallBackMessage.u32RequestLatency >= sCallBackMessage.u32ResponseLatency)
    {
        sCallBackMessage.u32Offset = (sCallBackMessage.u32RequestLatency - sCallBackMessage.u32ResponseLatency)/2;
    }
    else
    {
        sCallBackMessage.u32Offset = (sCallBackMessage.u32ResponseLatency - sCallBackMessage.u32RequestLatency)/2;
    }

    /* Include the user defined sequence number */
    sCallBackMessage.sequenceNumber = rPayload.sequenceNumber; 

    /* Return payload to upper layer */
    if((rPayload.payloadSize != 0) && (rPayload.payload != NULL))
    {
        sCallBackMessage.payloadSize = rPayload.payloadSize;
        sCallBackMessage.payload = rPayload.payload;
    }

    /* Generate a callback to let the app know the results */
    sCallBackMessage.u8CommandId            = u8CommandIdentifier;

    sZCL_CallBackEvent.u8EndPoint           = psEndPointDefinition->u8EndPointNumber;
    sZCL_CallBackEvent.psClusterInstance    = psClusterInstance;
    sZCL_CallBackEvent.pZPSevent            = pZPSevent;
    sZCL_CallBackEvent.eEventType           = E_ZCL_CBET_CLUSTER_CUSTOM;
    sZCL_CallBackEvent.uMessage.sClusterCustomMessage.u16ClusterId = GENERAL_CLUSTER_ID_DIAGNOSTICS;
    sZCL_CallBackEvent.uMessage.sClusterCustomMessage.pvCustomData = (void*)&sCallBackMessage;
    psEndPointDefinition->pCallBackFunctions(&sZCL_CallBackEvent);

    if(sCallBackMessage.payload != NULL)
    {
#if defined CPU_MKW41Z512VHT4
        MEM_BufferFree((void *)sCallBackMessage.payload);
#endif
    }

    if(rPayload.payload != NULL)
    {
#if defined CPU_MKW41Z512VHT4
        MEM_BufferFree((void *)rPayload.payload);
#endif
    }

    return eStatus;
}
#endif

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

