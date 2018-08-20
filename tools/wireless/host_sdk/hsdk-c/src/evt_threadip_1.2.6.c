/* Source file generated from ThreadIP_1.2.6.xml */
/*
 * The Clear BSD License
 * Copyright 2018 NXP
 * All rights reserved.
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
/*==================================================================================================
Include Files
==================================================================================================*/
#include "cmd_threadip_1.2.6.h"

/*==================================================================================================
Private Prototypes
==================================================================================================*/
#if SOCKET_ENABLE
static memStatus_t Load_SocketCreateConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketShutdownConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketBindConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketSendToConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketReceiveConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketReceiveFromConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketConnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketListenConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketAcceptConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketSetOptionConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SocketGetOptionConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* SOCKET_ENABLE */

#if MAC_ENABLE
static memStatus_t Load_MAC_MacFilteringAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MAC_MacFilteringRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MAC_MacFilteringEnableConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MAC_MacFilteringGetTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* MAC_ENABLE */

#if THREADMGMT_ENABLE
static memStatus_t Load_THR_SetDeviceConfigConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_NwkScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_CreateNwkConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_JoinConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_FactoryResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_CpuResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_CpuResetIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_DisconnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_AttachConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_PromoteAsRouterConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_EventNwkScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_EventNwkCreateConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_EventNwkJoinConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_EventNwkJoinSelectParentsConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_EventGeneralConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_EventNwkCommissioningIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_CommissioningDiagnosticIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_MgmtDiagnosticGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_DiagTestGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_MgmtDiagnosticResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_MgmtReadMemoryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_MgmtWriteMemoryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_SetManualSlaacIIDConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_SendProactiveAddrNotifConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_NwkDiscoveryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_NwkDiscoveryStopConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_SearchNwkWithAnounceConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_MgmtDiagnosticGetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_DiagTestGetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_MgmtDiagnosticDiagTestEventIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_MgmtDiagnosticResetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* THREADMGMT_ENABLE */

#if THREADUTILS_ENABLE
static memStatus_t Load_THR_SetNwkIdTimeoutConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_SetThresholdConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GetNeighborInfoConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GetChildrenTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GetNeighborTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GetRoutingTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GetAttrConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_SetAttrConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GetThreadIpAddrConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GetParentConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_ChildUpdateToParentConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_LeaderRemoveRouterIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_GenerateAllKeysConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_SwitchKeyKeyConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_BrPrefixAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_BrPrefixGetTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_BrPrefixRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_BrServiceRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_BrServiceAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_BrPrefixSyncConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_BrPrefixRemoveAllConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_EidToRlocMapResponse(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_THR_IdentifyConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* THREADUTILS_ENABLE */

#if IPUTILS_ENABLE
static memStatus_t Load_NWKU_IfconfigBindConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_IfconfigAllResponse(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_PingConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_EchoUDPConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_CoapMsgReceivedIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_DnsResponseReceivedIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_CoapRegisterConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_CoapCreateInstanceConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_CoapSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_DnsSendRequestConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_McastGroupManageConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_McastGroupShowResponse(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_RoutesManageConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_NWKU_RoutesShowResponse(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* IPUTILS_ENABLE */

#if MESHCOP_ENABLE
static memStatus_t Load_MESHCOP_StartCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_StartNativeCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_StopCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_AddExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_GetExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_RemoveExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_RemoveAllExpectedJoinersConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_SyncSteeringDataConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_SetCommissionerCredentialConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmNwkFormConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtCommissionerGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtActiveGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtPendingGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtCommissionerSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtActiveSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtPendingSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtSendPanIdQueryConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtPanIdConflictConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtSendEdScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtEdReportConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MESHCOP_MgmtSendAnnounceBeginConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* MESHCOP_ENABLE */

#if DTLS_ENABLE
static memStatus_t Load_DTLSOpenConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DTLSCloseConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DTLSClosePeerConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DTLSConnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DTLSClientConnectedConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DTLSSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DTLSReceiveConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* DTLS_ENABLE */

#if OTHERUTILS_ENABLE
static memStatus_t Load_FSCIGetUniqueIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_FSCIGetMcuIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_FSCIGetSwVersionsConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_Sniffer_MacSetPIBAttributeConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_MAC_PromiscuousRxIndication(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SerialTun_LinkIndicationConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_SerialTun_IPPacketReceivedConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_AspSetPowerLevelConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_AspGetPowerLevelConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
static memStatus_t Load_DBGConfirm(thrEvtContainer_t *container, uint8_t *pPayload);
#endif  /* OTHERUTILS_ENABLE */

/*==================================================================================================
Private global variables declarations
==================================================================================================*/
static const thrEvtHandler_t evtHandlerTbl[] =
{
#if SOCKET_ENABLE
	{SocketCreateConfirm_FSCI_ID, Load_SocketCreateConfirm},
	{SocketShutdownConfirm_FSCI_ID, Load_SocketShutdownConfirm},
	{SocketBindConfirm_FSCI_ID, Load_SocketBindConfirm},
	{SocketSendConfirm_FSCI_ID, Load_SocketSendConfirm},
	{SocketSendToConfirm_FSCI_ID, Load_SocketSendToConfirm},
	{SocketReceiveConfirm_FSCI_ID, Load_SocketReceiveConfirm},
	{SocketReceiveFromConfirm_FSCI_ID, Load_SocketReceiveFromConfirm},
	{SocketConnectConfirm_FSCI_ID, Load_SocketConnectConfirm},
	{SocketListenConfirm_FSCI_ID, Load_SocketListenConfirm},
	{SocketAcceptConfirm_FSCI_ID, Load_SocketAcceptConfirm},
	{SocketSetOptionConfirm_FSCI_ID, Load_SocketSetOptionConfirm},
	{SocketGetOptionConfirm_FSCI_ID, Load_SocketGetOptionConfirm},
#endif  /* SOCKET_ENABLE */

#if MAC_ENABLE
	{MAC_MacFilteringAddEntryConfirm_FSCI_ID, Load_MAC_MacFilteringAddEntryConfirm},
	{MAC_MacFilteringRemoveEntryConfirm_FSCI_ID, Load_MAC_MacFilteringRemoveEntryConfirm},
	{MAC_MacFilteringEnableConfirm_FSCI_ID, Load_MAC_MacFilteringEnableConfirm},
	{MAC_MacFilteringGetTableConfirm_FSCI_ID, Load_MAC_MacFilteringGetTableConfirm},
#endif  /* MAC_ENABLE */

#if THREADMGMT_ENABLE
	{THR_SetDeviceConfigConfirm_FSCI_ID, Load_THR_SetDeviceConfigConfirm},
	{THR_NwkScanConfirm_FSCI_ID, Load_THR_NwkScanConfirm},
	{THR_CreateNwkConfirm_FSCI_ID, Load_THR_CreateNwkConfirm},
	{THR_JoinConfirm_FSCI_ID, Load_THR_JoinConfirm},
	{THR_FactoryResetConfirm_FSCI_ID, Load_THR_FactoryResetConfirm},
	{THR_CpuResetConfirm_FSCI_ID, Load_THR_CpuResetConfirm},
	{THR_CpuResetIndication_FSCI_ID, Load_THR_CpuResetIndication},
	{THR_DisconnectConfirm_FSCI_ID, Load_THR_DisconnectConfirm},
	{THR_AttachConfirm_FSCI_ID, Load_THR_AttachConfirm},
	{THR_PromoteAsRouterConfirm_FSCI_ID, Load_THR_PromoteAsRouterConfirm},
	{THR_EventNwkScanConfirm_FSCI_ID, Load_THR_EventNwkScanConfirm},
	{THR_EventNwkCreateConfirm_FSCI_ID, Load_THR_EventNwkCreateConfirm},
	{THR_EventNwkJoinConfirm_FSCI_ID, Load_THR_EventNwkJoinConfirm},
	{THR_EventNwkJoinSelectParentsConfirm_FSCI_ID, Load_THR_EventNwkJoinSelectParentsConfirm},
	{THR_EventGeneralConfirm_FSCI_ID, Load_THR_EventGeneralConfirm},
	{THR_EventNwkCommissioningIndication_FSCI_ID, Load_THR_EventNwkCommissioningIndication},
	{THR_CommissioningDiagnosticIndication_FSCI_ID, Load_THR_CommissioningDiagnosticIndication},
	{THR_MgmtDiagnosticGetConfirm_FSCI_ID, Load_THR_MgmtDiagnosticGetConfirm},
	{THR_DiagTestGetConfirm_FSCI_ID, Load_THR_DiagTestGetConfirm},
	{THR_MgmtDiagnosticResetConfirm_FSCI_ID, Load_THR_MgmtDiagnosticResetConfirm},
	{THR_MgmtReadMemoryConfirm_FSCI_ID, Load_THR_MgmtReadMemoryConfirm},
	{THR_MgmtWriteMemoryConfirm_FSCI_ID, Load_THR_MgmtWriteMemoryConfirm},
	{THR_SetManualSlaacIIDConfirm_FSCI_ID, Load_THR_SetManualSlaacIIDConfirm},
	{THR_SendProactiveAddrNotifConfirm_FSCI_ID, Load_THR_SendProactiveAddrNotifConfirm},
	{THR_NwkDiscoveryConfirm_FSCI_ID, Load_THR_NwkDiscoveryConfirm},
	{THR_NwkDiscoveryStopConfirm_FSCI_ID, Load_THR_NwkDiscoveryStopConfirm},
	{THR_SearchNwkWithAnounceConfirm_FSCI_ID, Load_THR_SearchNwkWithAnounceConfirm},
	{THR_MgmtDiagnosticGetRspIndication_FSCI_ID, Load_THR_MgmtDiagnosticGetRspIndication},
	{THR_DiagTestGetRspIndication_FSCI_ID, Load_THR_DiagTestGetRspIndication},
	{THR_MgmtDiagnosticDiagTestEventIndication_FSCI_ID, Load_THR_MgmtDiagnosticDiagTestEventIndication},
	{THR_MgmtDiagnosticResetRspIndication_FSCI_ID, Load_THR_MgmtDiagnosticResetRspIndication},
#endif  /* THREADMGMT_ENABLE */

#if THREADUTILS_ENABLE
	{THR_SetNwkIdTimeoutConfirm_FSCI_ID, Load_THR_SetNwkIdTimeoutConfirm},
	{THR_SetThresholdConfirm_FSCI_ID, Load_THR_SetThresholdConfirm},
	{THR_GetNeighborInfoConfirm_FSCI_ID, Load_THR_GetNeighborInfoConfirm},
	{THR_GetChildrenTableConfirm_FSCI_ID, Load_THR_GetChildrenTableConfirm},
	{THR_GetNeighborTableConfirm_FSCI_ID, Load_THR_GetNeighborTableConfirm},
	{THR_GetRoutingTableConfirm_FSCI_ID, Load_THR_GetRoutingTableConfirm},
	{THR_GetAttrConfirm_FSCI_ID, Load_THR_GetAttrConfirm},
	{THR_SetAttrConfirm_FSCI_ID, Load_THR_SetAttrConfirm},
	{THR_GetThreadIpAddrConfirm_FSCI_ID, Load_THR_GetThreadIpAddrConfirm},
	{THR_GetParentConfirm_FSCI_ID, Load_THR_GetParentConfirm},
	{THR_ChildUpdateToParentConfirm_FSCI_ID, Load_THR_ChildUpdateToParentConfirm},
	{THR_LeaderRemoveRouterIdConfirm_FSCI_ID, Load_THR_LeaderRemoveRouterIdConfirm},
	{THR_GenerateAllKeysConfirm_FSCI_ID, Load_THR_GenerateAllKeysConfirm},
	{THR_SwitchKeyKeyConfirm_FSCI_ID, Load_THR_SwitchKeyKeyConfirm},
	{THR_BrPrefixAddEntryConfirm_FSCI_ID, Load_THR_BrPrefixAddEntryConfirm},
	{THR_BrPrefixGetTableConfirm_FSCI_ID, Load_THR_BrPrefixGetTableConfirm},
	{THR_BrPrefixRemoveEntryConfirm_FSCI_ID, Load_THR_BrPrefixRemoveEntryConfirm},
	{THR_BrServiceRemoveEntryConfirm_FSCI_ID, Load_THR_BrServiceRemoveEntryConfirm},
	{THR_BrServiceAddEntryConfirm_FSCI_ID, Load_THR_BrServiceAddEntryConfirm},
	{THR_BrPrefixSyncConfirm_FSCI_ID, Load_THR_BrPrefixSyncConfirm},
	{THR_BrPrefixRemoveAllConfirm_FSCI_ID, Load_THR_BrPrefixRemoveAllConfirm},
	{NWKU_EidToRlocMapResponse_FSCI_ID, Load_NWKU_EidToRlocMapResponse},
	{THR_IdentifyConfirm_FSCI_ID, Load_THR_IdentifyConfirm},
#endif  /* THREADUTILS_ENABLE */

#if IPUTILS_ENABLE
	{NWKU_IfconfigBindConfirm_FSCI_ID, Load_NWKU_IfconfigBindConfirm},
	{NWKU_IfconfigAllResponse_FSCI_ID, Load_NWKU_IfconfigAllResponse},
	{NWKU_PingConfirm_FSCI_ID, Load_NWKU_PingConfirm},
	{NWKU_EchoUDPConfirm_FSCI_ID, Load_NWKU_EchoUDPConfirm},
	{NWKU_CoapMsgReceivedIndication_FSCI_ID, Load_NWKU_CoapMsgReceivedIndication},
	{NWKU_DnsResponseReceivedIndication_FSCI_ID, Load_NWKU_DnsResponseReceivedIndication},
	{NWKU_CoapRegisterConfirm_FSCI_ID, Load_NWKU_CoapRegisterConfirm},
	{NWKU_CoapCreateInstanceConfirm_FSCI_ID, Load_NWKU_CoapCreateInstanceConfirm},
	{NWKU_CoapSendConfirm_FSCI_ID, Load_NWKU_CoapSendConfirm},
	{NWKU_DnsSendRequestConfirm_FSCI_ID, Load_NWKU_DnsSendRequestConfirm},
	{NWKU_McastGroupManageConfirm_FSCI_ID, Load_NWKU_McastGroupManageConfirm},
	{NWKU_McastGroupShowResponse_FSCI_ID, Load_NWKU_McastGroupShowResponse},
	{NWKU_RoutesManageConfirm_FSCI_ID, Load_NWKU_RoutesManageConfirm},
	{NWKU_RoutesShowResponse_FSCI_ID, Load_NWKU_RoutesShowResponse},
#endif  /* IPUTILS_ENABLE */

#if MESHCOP_ENABLE
	{MESHCOP_StartCommissionerConfirm_FSCI_ID, Load_MESHCOP_StartCommissionerConfirm},
	{MESHCOP_StartNativeCommissionerConfirm_FSCI_ID, Load_MESHCOP_StartNativeCommissionerConfirm},
	{MESHCOP_StopCommissionerConfirm_FSCI_ID, Load_MESHCOP_StopCommissionerConfirm},
	{MESHCOP_AddExpectedJoinerConfirm_FSCI_ID, Load_MESHCOP_AddExpectedJoinerConfirm},
	{MESHCOP_GetExpectedJoinerConfirm_FSCI_ID, Load_MESHCOP_GetExpectedJoinerConfirm},
	{MESHCOP_RemoveExpectedJoinerConfirm_FSCI_ID, Load_MESHCOP_RemoveExpectedJoinerConfirm},
	{MESHCOP_RemoveAllExpectedJoinersConfirm_FSCI_ID, Load_MESHCOP_RemoveAllExpectedJoinersConfirm},
	{MESHCOP_SyncSteeringDataConfirm_FSCI_ID, Load_MESHCOP_SyncSteeringDataConfirm},
	{MESHCOP_MgmtSetConfirm_FSCI_ID, Load_MESHCOP_MgmtSetConfirm},
	{MESHCOP_MgmtGetConfirm_FSCI_ID, Load_MESHCOP_MgmtGetConfirm},
	{MESHCOP_SetCommissionerCredentialConfirm_FSCI_ID, Load_MESHCOP_SetCommissionerCredentialConfirm},
	{MESHCOP_MgmNwkFormConfirm_FSCI_ID, Load_MESHCOP_MgmNwkFormConfirm},
	{MESHCOP_MgmtCommissionerGetConfirm_FSCI_ID, Load_MESHCOP_MgmtCommissionerGetConfirm},
	{MESHCOP_MgmtActiveGetConfirm_FSCI_ID, Load_MESHCOP_MgmtActiveGetConfirm},
	{MESHCOP_MgmtPendingGetConfirm_FSCI_ID, Load_MESHCOP_MgmtPendingGetConfirm},
	{MESHCOP_MgmtCommissionerSetConfirm_FSCI_ID, Load_MESHCOP_MgmtCommissionerSetConfirm},
	{MESHCOP_MgmtActiveSetConfirm_FSCI_ID, Load_MESHCOP_MgmtActiveSetConfirm},
	{MESHCOP_MgmtPendingSetConfirm_FSCI_ID, Load_MESHCOP_MgmtPendingSetConfirm},
	{MESHCOP_MgmtSendPanIdQueryConfirm_FSCI_ID, Load_MESHCOP_MgmtSendPanIdQueryConfirm},
	{MESHCOP_MgmtPanIdConflictConfirm_FSCI_ID, Load_MESHCOP_MgmtPanIdConflictConfirm},
	{MESHCOP_MgmtSendEdScanConfirm_FSCI_ID, Load_MESHCOP_MgmtSendEdScanConfirm},
	{MESHCOP_MgmtEdReportConfirm_FSCI_ID, Load_MESHCOP_MgmtEdReportConfirm},
	{MESHCOP_MgmtSendAnnounceBeginConfirm_FSCI_ID, Load_MESHCOP_MgmtSendAnnounceBeginConfirm},
#endif  /* MESHCOP_ENABLE */

#if DTLS_ENABLE
	{DTLSOpenConfirm_FSCI_ID, Load_DTLSOpenConfirm},
	{DTLSCloseConfirm_FSCI_ID, Load_DTLSCloseConfirm},
	{DTLSClosePeerConfirm_FSCI_ID, Load_DTLSClosePeerConfirm},
	{DTLSConnectConfirm_FSCI_ID, Load_DTLSConnectConfirm},
	{DTLSClientConnectedConfirm_FSCI_ID, Load_DTLSClientConnectedConfirm},
	{DTLSSendConfirm_FSCI_ID, Load_DTLSSendConfirm},
	{DTLSReceiveConfirm_FSCI_ID, Load_DTLSReceiveConfirm},
#endif  /* DTLS_ENABLE */

#if OTHERUTILS_ENABLE
	{FSCIGetUniqueIdConfirm_FSCI_ID, Load_FSCIGetUniqueIdConfirm},
	{FSCIGetMcuIdConfirm_FSCI_ID, Load_FSCIGetMcuIdConfirm},
	{FSCIGetSwVersionsConfirm_FSCI_ID, Load_FSCIGetSwVersionsConfirm},
	{Sniffer_MacSetPIBAttributeConfirm_FSCI_ID, Load_Sniffer_MacSetPIBAttributeConfirm},
	{MAC_PromiscuousRxIndication_FSCI_ID, Load_MAC_PromiscuousRxIndication},
	{SerialTun_LinkIndicationConfirm_FSCI_ID, Load_SerialTun_LinkIndicationConfirm},
	{SerialTun_IPPacketReceivedConfirm_FSCI_ID, Load_SerialTun_IPPacketReceivedConfirm},
	{AspSetPowerLevelConfirm_FSCI_ID, Load_AspSetPowerLevelConfirm},
	{AspGetPowerLevelConfirm_FSCI_ID, Load_AspGetPowerLevelConfirm},
	{DBGConfirm_FSCI_ID, Load_DBGConfirm},
#endif  /* OTHERUTILS_ENABLE */
};

/*==================================================================================================
Public Functions
==================================================================================================*/
#if SOCKET_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketCreateConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket create request.
***************************************************************************************************/
static memStatus_t Load_SocketCreateConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketCreateConfirm_t *evt = &(container->Data.SocketCreateConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketCreateConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketCreateConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketShutdownConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket shutdown request.
***************************************************************************************************/
static memStatus_t Load_SocketShutdownConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketShutdownConfirm_t *evt = &(container->Data.SocketShutdownConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketShutdownConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketShutdownConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketBindConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket bind request.
***************************************************************************************************/
static memStatus_t Load_SocketBindConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketBindConfirm_t *evt = &(container->Data.SocketBindConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketBindConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketBindConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket send request.
***************************************************************************************************/
static memStatus_t Load_SocketSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketSendConfirm_t *evt = &(container->Data.SocketSendConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketSendConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketSendConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketSendToConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket send request.
***************************************************************************************************/
static memStatus_t Load_SocketSendToConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketSendToConfirm_t *evt = &(container->Data.SocketSendToConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketSendToConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketSendToConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketReceiveConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket receive request.
***************************************************************************************************/
static memStatus_t Load_SocketReceiveConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketReceiveConfirm_t *evt = &(container->Data.SocketReceiveConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = SocketReceiveConfirm_FSCI_ID;

	FLib_MemCpy(&(evt->Size), pPayload + idx, sizeof(evt->Size)); idx += sizeof(evt->Size);

	if (evt->Size > 0)
	{
		evt->Data = MEM_BufferAlloc(evt->Size);

		if (!evt->Data)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Data = NULL;
	}

	FLib_MemCpy(evt->Data, pPayload + idx, evt->Size); idx += evt->Size;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketReceiveFromConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket receive request.
***************************************************************************************************/
static memStatus_t Load_SocketReceiveFromConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketReceiveFromConfirm_t *evt = &(container->Data.SocketReceiveFromConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = SocketReceiveFromConfirm_FSCI_ID;

	evt->Status = (SocketReceiveFromConfirm_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(evt->RemoteIpAddress, pPayload + idx, sizeof(evt->RemoteIpAddress)); idx += sizeof(evt->RemoteIpAddress);
	FLib_MemCpy(&(evt->RemotePort), pPayload + idx, sizeof(evt->RemotePort)); idx += sizeof(evt->RemotePort);
	FLib_MemCpy(&(evt->Size), pPayload + idx, sizeof(evt->Size)); idx += sizeof(evt->Size);

	if (evt->Size > 0)
	{
		evt->Data = MEM_BufferAlloc(evt->Size);

		if (!evt->Data)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Data = NULL;
	}

	FLib_MemCpy(evt->Data, pPayload + idx, evt->Size); idx += evt->Size;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketConnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket connect request.
***************************************************************************************************/
static memStatus_t Load_SocketConnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketConnectConfirm_t *evt = &(container->Data.SocketConnectConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketConnectConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketConnectConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketListenConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket listen request.
***************************************************************************************************/
static memStatus_t Load_SocketListenConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketListenConfirm_t *evt = &(container->Data.SocketListenConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketListenConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketListenConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketAcceptConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket accept request.
***************************************************************************************************/
static memStatus_t Load_SocketAcceptConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketAcceptConfirm_t *evt = &(container->Data.SocketAcceptConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketAcceptConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketAcceptConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketSetOptionConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the socket set option request.
***************************************************************************************************/
static memStatus_t Load_SocketSetOptionConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketSetOptionConfirm_t *evt = &(container->Data.SocketSetOptionConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketSetOptionConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketSetOptionConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SocketGetOptionConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get socket option.
***************************************************************************************************/
static memStatus_t Load_SocketGetOptionConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SocketGetOptionConfirm_t *evt = &(container->Data.SocketGetOptionConfirm);

	/* Store (OG, OC) in ID */
	container->id = SocketGetOptionConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SocketGetOptionConfirm_t));

	return MEM_SUCCESS_c;
}

#endif  /* SOCKET_ENABLE */

#if MAC_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_MAC_MacFilteringAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the addition of a new entry in the Mac filtering List table.
***************************************************************************************************/
static memStatus_t Load_MAC_MacFilteringAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MAC_MacFilteringAddEntryConfirm_t *evt = &(container->Data.MAC_MacFilteringAddEntryConfirm);

	/* Store (OG, OC) in ID */
	container->id = MAC_MacFilteringAddEntryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MAC_MacFilteringAddEntryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MAC_MacFilteringRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Remove Mac filtering list entry confirmation.
***************************************************************************************************/
static memStatus_t Load_MAC_MacFilteringRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MAC_MacFilteringRemoveEntryConfirm_t *evt = &(container->Data.MAC_MacFilteringRemoveEntryConfirm);

	/* Store (OG, OC) in ID */
	container->id = MAC_MacFilteringRemoveEntryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MAC_MacFilteringRemoveEntryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MAC_MacFilteringEnableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Enable Mac filtering list table confirmation.
***************************************************************************************************/
static memStatus_t Load_MAC_MacFilteringEnableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MAC_MacFilteringEnableConfirm_t *evt = &(container->Data.MAC_MacFilteringEnableConfirm);

	/* Store (OG, OC) in ID */
	container->id = MAC_MacFilteringEnableConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MAC_MacFilteringEnableConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MAC_MacFilteringGetTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get Mac filtering list table response.
***************************************************************************************************/
static memStatus_t Load_MAC_MacFilteringGetTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MAC_MacFilteringGetTableConfirm_t *evt = &(container->Data.MAC_MacFilteringGetTableConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MAC_MacFilteringGetTableConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	evt->NoOfElements = pPayload[idx]; idx++;

	if (evt->NoOfElements > 0)
	{
		evt->MacFilteringEntries = MEM_BufferAlloc(evt->NoOfElements * sizeof(evt->MacFilteringEntries[0]));

		if (!evt->MacFilteringEntries)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->MacFilteringEntries = NULL;
	}


	for (uint32_t i = 0; i < evt->NoOfElements; i++)
	{
		FLib_MemCpy(&(evt->MacFilteringEntries[i].ExtendedAddress), pPayload + idx, sizeof(evt->MacFilteringEntries[i].ExtendedAddress)); idx += sizeof(evt->MacFilteringEntries[i].ExtendedAddress);
		FLib_MemCpy(&(evt->MacFilteringEntries[i].ShortAddress), pPayload + idx, sizeof(evt->MacFilteringEntries[i].ShortAddress)); idx += sizeof(evt->MacFilteringEntries[i].ShortAddress);
		evt->MacFilteringEntries[i].LinkIndicator = pPayload[idx]; idx++;
		evt->MacFilteringEntries[i].BlockedNeighbor = (bool_t)pPayload[idx]; idx++;
	}

	return MEM_SUCCESS_c;
}

#endif  /* MAC_ENABLE */

#if THREADMGMT_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SetDeviceConfigConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Set device configuration confirm
***************************************************************************************************/
static memStatus_t Load_THR_SetDeviceConfigConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SetDeviceConfigConfirm_t *evt = &(container->Data.THR_SetDeviceConfigConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SetDeviceConfigConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SetDeviceConfigConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_NwkScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the scan network request.
***************************************************************************************************/
static memStatus_t Load_THR_NwkScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_NwkScanConfirm_t *evt = &(container->Data.THR_NwkScanConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_NwkScanConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_NwkScanConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_CreateNwkConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the create network request.
***************************************************************************************************/
static memStatus_t Load_THR_CreateNwkConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_CreateNwkConfirm_t *evt = &(container->Data.THR_CreateNwkConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_CreateNwkConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_CreateNwkConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_JoinConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the join network request.
***************************************************************************************************/
static memStatus_t Load_THR_JoinConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_JoinConfirm_t *evt = &(container->Data.THR_JoinConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_JoinConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_JoinConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_FactoryResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Reset the device to factory default
***************************************************************************************************/
static memStatus_t Load_THR_FactoryResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_FactoryResetConfirm_t *evt = &(container->Data.THR_FactoryResetConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_FactoryResetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_FactoryResetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_CpuResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the device reset.
***************************************************************************************************/
static memStatus_t Load_THR_CpuResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_CpuResetConfirm_t *evt = &(container->Data.THR_CpuResetConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_CpuResetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_CpuResetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_CpuResetIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	THR_CPUReset indication
***************************************************************************************************/
static memStatus_t Load_THR_CpuResetIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_CpuResetIndication_t *evt = &(container->Data.THR_CpuResetIndication);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_CpuResetIndication_FSCI_ID;

	evt->Status = (THR_CpuResetIndication_Status_t)pPayload[idx]; idx++;

	switch (evt->Status)
	{
		case THR_CpuResetIndication_Status_ResetCpuSuccess:
			evt->ResetCpuPayload.ResetCpuSuccess.BoardNameLen = pPayload[idx]; idx++;

			if (evt->ResetCpuPayload.ResetCpuSuccess.BoardNameLen > 0)
			{
				evt->ResetCpuPayload.ResetCpuSuccess.BoardName = MEM_BufferAlloc(evt->ResetCpuPayload.ResetCpuSuccess.BoardNameLen);

				if (!evt->ResetCpuPayload.ResetCpuSuccess.BoardName)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->ResetCpuPayload.ResetCpuSuccess.BoardName = NULL;
			}

			FLib_MemCpy(evt->ResetCpuPayload.ResetCpuSuccess.BoardName, pPayload + idx, evt->ResetCpuPayload.ResetCpuSuccess.BoardNameLen); idx += evt->ResetCpuPayload.ResetCpuSuccess.BoardNameLen;
			FLib_MemCpy(evt->ResetCpuPayload.ResetCpuSuccess.UniqueMcuId, pPayload + idx, sizeof(evt->ResetCpuPayload.ResetCpuSuccess.UniqueMcuId)); idx += sizeof(evt->ResetCpuPayload.ResetCpuSuccess.UniqueMcuId);
			FLib_MemCpy(&(evt->ResetCpuPayload.ResetCpuSuccess.StackVersionStruct.StackVendorOUI), pPayload + idx, 3); idx += 3;
			FLib_MemCpy(&(evt->ResetCpuPayload.ResetCpuSuccess.StackVersionStruct.StackVersion), pPayload + idx, 3); idx += 3;
			evt->ResetCpuPayload.ResetCpuSuccess.SwVersionLen = pPayload[idx]; idx++;

			if (evt->ResetCpuPayload.ResetCpuSuccess.SwVersionLen > 0)
			{
				evt->ResetCpuPayload.ResetCpuSuccess.SwVersion = MEM_BufferAlloc(evt->ResetCpuPayload.ResetCpuSuccess.SwVersionLen);

				if (!evt->ResetCpuPayload.ResetCpuSuccess.SwVersion)
				{
					MEM_BufferFree(evt->ResetCpuPayload.ResetCpuSuccess.BoardName);
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->ResetCpuPayload.ResetCpuSuccess.SwVersion = NULL;
			}

			FLib_MemCpy(evt->ResetCpuPayload.ResetCpuSuccess.SwVersion, pPayload + idx, evt->ResetCpuPayload.ResetCpuSuccess.SwVersionLen); idx += evt->ResetCpuPayload.ResetCpuSuccess.SwVersionLen;
			break;

		case THR_CpuResetIndication_Status_ResetCpuPending:
			FLib_MemCpy(&(evt->ResetCpuPayload.ResetCpuPending_TimeoutMs), pPayload + idx, sizeof(evt->ResetCpuPayload.ResetCpuPending_TimeoutMs)); idx += sizeof(evt->ResetCpuPayload.ResetCpuPending_TimeoutMs);
			break;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_DisconnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the THR_Disconnect.Request.
***************************************************************************************************/
static memStatus_t Load_THR_DisconnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_DisconnectConfirm_t *evt = &(container->Data.THR_DisconnectConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_DisconnectConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_DisconnectConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_AttachConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the THR_Attach.Request.
***************************************************************************************************/
static memStatus_t Load_THR_AttachConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_AttachConfirm_t *evt = &(container->Data.THR_AttachConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_AttachConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_AttachConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_PromoteAsRouterConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the THR_PromoteAsRouter.Request
***************************************************************************************************/
static memStatus_t Load_THR_PromoteAsRouterConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_PromoteAsRouterConfirm_t *evt = &(container->Data.THR_PromoteAsRouterConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_PromoteAsRouterConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_PromoteAsRouterConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_EventNwkScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the network scan event
***************************************************************************************************/
static memStatus_t Load_THR_EventNwkScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_EventNwkScanConfirm_t *evt = &(container->Data.THR_EventNwkScanConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_EventNwkScanConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->EventStatus), pPayload + idx, 2); idx += 2;
	FLib_MemCpy(&(evt->DataSize), pPayload + idx, sizeof(evt->DataSize)); idx += sizeof(evt->DataSize);
	FLib_MemCpy(&(evt->ScanChannelMask), pPayload + idx, sizeof(evt->ScanChannelMask)); idx += sizeof(evt->ScanChannelMask);
	evt->ScanType = (THR_EventNwkScanConfirm_ScanType_t)pPayload[idx]; idx++;
	evt->ScanDuration = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->maxThrNwkToDiscover), pPayload + idx, sizeof(evt->maxThrNwkToDiscover)); idx += sizeof(evt->maxThrNwkToDiscover);

	switch (evt->ScanType)
	{
		case THR_EventNwkScanConfirm_ScanType_EnergyDetect:
			evt->Data.EnergyDetect.EnergyDetectEntries = pPayload[idx]; idx++;

			if (evt->Data.EnergyDetect.EnergyDetectEntries > 0)
			{
				evt->Data.EnergyDetect.EnergyDetectList = MEM_BufferAlloc(evt->Data.EnergyDetect.EnergyDetectEntries);

				if (!evt->Data.EnergyDetect.EnergyDetectList)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Data.EnergyDetect.EnergyDetectList = NULL;
			}

			FLib_MemCpy(evt->Data.EnergyDetect.EnergyDetectList, pPayload + idx, evt->Data.EnergyDetect.EnergyDetectEntries); idx += evt->Data.EnergyDetect.EnergyDetectEntries;
			break;

		case THR_EventNwkScanConfirm_ScanType_ActiveScan:
			evt->Data.ActiveScan.NwkDiscoveryEntries = pPayload[idx]; idx++;

			if (evt->Data.ActiveScan.NwkDiscoveryEntries > 0)
			{
				evt->Data.ActiveScan.NwkDiscoveryList = MEM_BufferAlloc(evt->Data.ActiveScan.NwkDiscoveryEntries * sizeof(evt->Data.ActiveScan.NwkDiscoveryList[0]));

				if (!evt->Data.ActiveScan.NwkDiscoveryList)
				{
					MEM_BufferFree(evt->Data.EnergyDetect.EnergyDetectList);
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Data.ActiveScan.NwkDiscoveryList = NULL;
			}


			for (uint32_t i = 0; i < evt->Data.ActiveScan.NwkDiscoveryEntries; i++)
			{
				FLib_MemCpy(&(evt->Data.ActiveScan.NwkDiscoveryList[i].NumOfRcvdBeacons), pPayload + idx, sizeof(evt->Data.ActiveScan.NwkDiscoveryList[i].NumOfRcvdBeacons)); idx += sizeof(evt->Data.ActiveScan.NwkDiscoveryList[i].NumOfRcvdBeacons);
				FLib_MemCpy(&(evt->Data.ActiveScan.NwkDiscoveryList[i].PanId), pPayload + idx, sizeof(evt->Data.ActiveScan.NwkDiscoveryList[i].PanId)); idx += sizeof(evt->Data.ActiveScan.NwkDiscoveryList[i].PanId);
				evt->Data.ActiveScan.NwkDiscoveryList[i].Channel = pPayload[idx]; idx++;
				evt->Data.ActiveScan.NwkDiscoveryList[i].Reserved = pPayload[idx]; idx++;
			}
			break;

		case THR_EventNwkScanConfirm_ScanType_EnergyDetectAndActiveScan:
			evt->Data.EnergyDetectAndActiveScan.EnergyDetectEntries = pPayload[idx]; idx++;

			if (evt->Data.EnergyDetectAndActiveScan.EnergyDetectEntries > 0)
			{
				evt->Data.EnergyDetectAndActiveScan.EnergyDetectList = MEM_BufferAlloc(evt->Data.EnergyDetectAndActiveScan.EnergyDetectEntries);

				if (!evt->Data.EnergyDetectAndActiveScan.EnergyDetectList)
				{
					MEM_BufferFree(evt->Data.ActiveScan.NwkDiscoveryList);
					MEM_BufferFree(evt->Data.EnergyDetect.EnergyDetectList);
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Data.EnergyDetectAndActiveScan.EnergyDetectList = NULL;
			}

			FLib_MemCpy(evt->Data.EnergyDetectAndActiveScan.EnergyDetectList, pPayload + idx, evt->Data.EnergyDetectAndActiveScan.EnergyDetectEntries); idx += evt->Data.EnergyDetectAndActiveScan.EnergyDetectEntries;
			evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryEntries = pPayload[idx]; idx++;

			if (evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryEntries > 0)
			{
				evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList = MEM_BufferAlloc(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryEntries * sizeof(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[0]));

				if (!evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList)
				{
					MEM_BufferFree(evt->Data.EnergyDetectAndActiveScan.EnergyDetectList);
					MEM_BufferFree(evt->Data.ActiveScan.NwkDiscoveryList);
					MEM_BufferFree(evt->Data.EnergyDetect.EnergyDetectList);
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList = NULL;
			}


			for (uint32_t i = 0; i < evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryEntries; i++)
			{
				FLib_MemCpy(&(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].NumOfRcvdBeacons), pPayload + idx, sizeof(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].NumOfRcvdBeacons)); idx += sizeof(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].NumOfRcvdBeacons);
				FLib_MemCpy(&(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].PanId), pPayload + idx, sizeof(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].PanId)); idx += sizeof(evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].PanId);
				evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].Channel = pPayload[idx]; idx++;
				evt->Data.EnergyDetectAndActiveScan.NwkDiscoveryList[i].Reserved = pPayload[idx]; idx++;
			}
			break;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_EventNwkCreateConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the network create event
***************************************************************************************************/
static memStatus_t Load_THR_EventNwkCreateConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_EventNwkCreateConfirm_t *evt = &(container->Data.THR_EventNwkCreateConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_EventNwkCreateConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->EventStatus), pPayload + idx, 2); idx += 2;
	FLib_MemCpy(&(evt->DataSize), pPayload + idx, sizeof(evt->DataSize)); idx += sizeof(evt->DataSize);

	if (evt->DataSize > 0)
	{
		evt->Data = MEM_BufferAlloc(evt->DataSize);

		if (!evt->Data)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Data = NULL;
	}

	FLib_MemCpy(evt->Data, pPayload + idx, evt->DataSize); idx += evt->DataSize;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_EventNwkJoinConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the network join event
***************************************************************************************************/
static memStatus_t Load_THR_EventNwkJoinConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_EventNwkJoinConfirm_t *evt = &(container->Data.THR_EventNwkJoinConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_EventNwkJoinConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->EventStatus), pPayload + idx, 2); idx += 2;
	FLib_MemCpy(&(evt->DataSize), pPayload + idx, sizeof(evt->DataSize)); idx += sizeof(evt->DataSize);

	if (evt->DataSize > 0)
	{
		evt->Data = MEM_BufferAlloc(evt->DataSize);

		if (!evt->Data)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Data = NULL;
	}

	FLib_MemCpy(evt->Data, pPayload + idx, evt->DataSize); idx += evt->DataSize;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_EventNwkJoinSelectParentsConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the network join event
***************************************************************************************************/
static memStatus_t Load_THR_EventNwkJoinSelectParentsConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_EventNwkJoinSelectParentsConfirm_t *evt = &(container->Data.THR_EventNwkJoinSelectParentsConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_EventNwkJoinSelectParentsConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->EventStatus), pPayload + idx, 2); idx += 2;
	FLib_MemCpy(&(evt->DataSize), pPayload + idx, sizeof(evt->DataSize)); idx += sizeof(evt->DataSize);

	if (evt->DataSize > 0)
	{
		evt->Data = MEM_BufferAlloc(evt->DataSize);

		if (!evt->Data)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Data = NULL;
	}

	FLib_MemCpy(evt->Data, pPayload + idx, evt->DataSize); idx += evt->DataSize;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_EventGeneralConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the network join event
***************************************************************************************************/
static memStatus_t Load_THR_EventGeneralConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_EventGeneralConfirm_t *evt = &(container->Data.THR_EventGeneralConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_EventGeneralConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->EventStatus), pPayload + idx, 2); idx += 2;
	FLib_MemCpy(&(evt->DataSize), pPayload + idx, sizeof(evt->DataSize)); idx += sizeof(evt->DataSize);

	if (evt->DataSize > 0)
	{
		evt->Data = MEM_BufferAlloc(evt->DataSize);

		if (!evt->Data)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Data = NULL;
	}

	FLib_MemCpy(evt->Data, pPayload + idx, evt->DataSize); idx += evt->DataSize;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_EventNwkCommissioningIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Commissioning Events
***************************************************************************************************/
static memStatus_t Load_THR_EventNwkCommissioningIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_EventNwkCommissioningIndication_t *evt = &(container->Data.THR_EventNwkCommissioningIndication);

	/* Store (OG, OC) in ID */
	container->id = THR_EventNwkCommissioningIndication_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_EventNwkCommissioningIndication_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_CommissioningDiagnosticIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Commissioning diagnostic messages
***************************************************************************************************/
static memStatus_t Load_THR_CommissioningDiagnosticIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_CommissioningDiagnosticIndication_t *evt = &(container->Data.THR_CommissioningDiagnosticIndication);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_CommissioningDiagnosticIndication_FSCI_ID;

	evt->Direction = (THR_CommissioningDiagnosticIndication_Direction_t)pPayload[idx]; idx++;
	evt->Type = (THR_CommissioningDiagnosticIndication_Type_t)pPayload[idx]; idx++;
	FLib_MemCpy(evt->EUI, pPayload + idx, 8); idx += 8;
	evt->TlvsLength = pPayload[idx]; idx++;

	if (evt->TlvsLength > 0)
	{
		evt->TlvsBytes = MEM_BufferAlloc(evt->TlvsLength);

		if (!evt->TlvsBytes)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->TlvsBytes = NULL;
	}

	FLib_MemCpy(evt->TlvsBytes, pPayload + idx, evt->TlvsLength); idx += evt->TlvsLength;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_MgmtDiagnosticGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation to network diagnostic get request.
***************************************************************************************************/
static memStatus_t Load_THR_MgmtDiagnosticGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_MgmtDiagnosticGetConfirm_t *evt = &(container->Data.THR_MgmtDiagnosticGetConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_MgmtDiagnosticGetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_MgmtDiagnosticGetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_DiagTestGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation to network diagnostic get request.
***************************************************************************************************/
static memStatus_t Load_THR_DiagTestGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_DiagTestGetConfirm_t *evt = &(container->Data.THR_DiagTestGetConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_DiagTestGetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_DiagTestGetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_MgmtDiagnosticResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation to network diagnostic reset request.
***************************************************************************************************/
static memStatus_t Load_THR_MgmtDiagnosticResetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_MgmtDiagnosticResetConfirm_t *evt = &(container->Data.THR_MgmtDiagnosticResetConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_MgmtDiagnosticResetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_MgmtDiagnosticResetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_MgmtReadMemoryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Memory content corresponding to the THR_MgmtReadMemory.Request.
***************************************************************************************************/
static memStatus_t Load_THR_MgmtReadMemoryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_MgmtReadMemoryConfirm_t *evt = &(container->Data.THR_MgmtReadMemoryConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_MgmtReadMemoryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_MgmtReadMemoryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_MgmtWriteMemoryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirm of the THR_MgmtWriteMemory.Request command. Is always success.
***************************************************************************************************/
static memStatus_t Load_THR_MgmtWriteMemoryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_MgmtWriteMemoryConfirm_t *evt = &(container->Data.THR_MgmtWriteMemoryConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_MgmtWriteMemoryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_MgmtWriteMemoryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SetManualSlaacIIDConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the THR_SetManualSlaacIID.Request.
***************************************************************************************************/
static memStatus_t Load_THR_SetManualSlaacIIDConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SetManualSlaacIIDConfirm_t *evt = &(container->Data.THR_SetManualSlaacIIDConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SetManualSlaacIIDConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SetManualSlaacIIDConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SendProactiveAddrNotifConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	THR_SendProactiveAddrNotif confirmation
***************************************************************************************************/
static memStatus_t Load_THR_SendProactiveAddrNotifConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SendProactiveAddrNotifConfirm_t *evt = &(container->Data.THR_SendProactiveAddrNotifConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SendProactiveAddrNotifConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SendProactiveAddrNotifConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_NwkDiscoveryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the nwk discovery request.
***************************************************************************************************/
static memStatus_t Load_THR_NwkDiscoveryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_NwkDiscoveryConfirm_t *evt = &(container->Data.THR_NwkDiscoveryConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_NwkDiscoveryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_NwkDiscoveryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_NwkDiscoveryStopConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the nwk discovery stop request.
***************************************************************************************************/
static memStatus_t Load_THR_NwkDiscoveryStopConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_NwkDiscoveryStopConfirm_t *evt = &(container->Data.THR_NwkDiscoveryStopConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_NwkDiscoveryStopConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_NwkDiscoveryStopConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SearchNwkWithAnounceConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the search for nwk with announce.
***************************************************************************************************/
static memStatus_t Load_THR_SearchNwkWithAnounceConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SearchNwkWithAnounceConfirm_t *evt = &(container->Data.THR_SearchNwkWithAnounceConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SearchNwkWithAnounceConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SearchNwkWithAnounceConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_MgmtDiagnosticGetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The response to nwk diagnostic get request.
***************************************************************************************************/
static memStatus_t Load_THR_MgmtDiagnosticGetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_MgmtDiagnosticGetRspIndication_t *evt = &(container->Data.THR_MgmtDiagnosticGetRspIndication);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_MgmtDiagnosticGetRspIndication_FSCI_ID;

	evt->Status = (THR_MgmtDiagnosticGetRspIndication_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->CoapMsgId), pPayload + idx, sizeof(evt->CoapMsgId)); idx += sizeof(evt->CoapMsgId);
	FLib_MemCpy(&(evt->DataLen), pPayload + idx, sizeof(evt->DataLen)); idx += sizeof(evt->DataLen);

	if (evt->DataLen > 0)
	{
		evt->Payload = MEM_BufferAlloc(evt->DataLen * sizeof(evt->Payload[0]));

		if (!evt->Payload)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Payload = NULL;
	}


	for (uint32_t i = 0; i < evt->DataLen; i++)
	{
		evt->Payload[i].TlvId = (THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_t)pPayload[idx]; idx++;
		evt->Payload[i].TlvLength = pPayload[idx]; idx++;

		switch (evt->Payload[i].TlvId)
		{
			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_SourceAddr:
				FLib_MemCpy(&(evt->Payload[i].TlvData.SourceAddr), pPayload + idx, sizeof(evt->Payload[i].TlvData.SourceAddr)); idx += sizeof(evt->Payload[i].TlvData.SourceAddr);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_ShortAddr:
				FLib_MemCpy(&(evt->Payload[i].TlvData.ShortAddr), pPayload + idx, sizeof(evt->Payload[i].TlvData.ShortAddr)); idx += sizeof(evt->Payload[i].TlvData.ShortAddr);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Mode:
				evt->Payload[i].TlvData.Mode = pPayload[idx]; idx++;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Timeout:
				FLib_MemCpy(&(evt->Payload[i].TlvData.Timeout), pPayload + idx, sizeof(evt->Payload[i].TlvData.Timeout)); idx += sizeof(evt->Payload[i].TlvData.Timeout);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_LinkQuality:
				evt->Payload[i].TlvData.LinkQuality.MaxChildCount = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LinkQuality.ChildCount = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LinkQuality.cLinkQuality3 = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LinkQuality.cLinkQuality2 = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LinkQuality.cLinkQuality1 = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LinkQuality.LeaderCost = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LinkQuality.IdSequence = pPayload[idx]; idx++;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_RoutingTable:

				if (evt->Payload[i].TlvLength > 0)
				{
					evt->Payload[i].TlvData.RoutingTable = MEM_BufferAlloc(evt->Payload[i].TlvLength);

					if (!evt->Payload[i].TlvData.RoutingTable)
					{
						MEM_BufferFree(evt->Payload);
						return MEM_ALLOC_ERROR_c;
					}

				}
				else
				{
					evt->Payload[i].TlvData.RoutingTable = NULL;
				}

				FLib_MemCpy(evt->Payload[i].TlvData.RoutingTable, pPayload + idx, evt->Payload[i].TlvLength); idx += evt->Payload[i].TlvLength;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_LeaderData:
				FLib_MemCpy(&(evt->Payload[i].TlvData.LeaderData.PartitionId), pPayload + idx, sizeof(evt->Payload[i].TlvData.LeaderData.PartitionId)); idx += sizeof(evt->Payload[i].TlvData.LeaderData.PartitionId);
				evt->Payload[i].TlvData.LeaderData.Weighting = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LeaderData.DataVersion = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LeaderData.StableVersion = pPayload[idx]; idx++;
				evt->Payload[i].TlvData.LeaderData.LeaderId = pPayload[idx]; idx++;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_NwkData:

				if (evt->Payload[i].TlvLength > 0)
				{
					evt->Payload[i].TlvData.NwkData = MEM_BufferAlloc(evt->Payload[i].TlvLength);

					if (!evt->Payload[i].TlvData.NwkData)
					{
						MEM_BufferFree(evt->Payload);
						return MEM_ALLOC_ERROR_c;
					}

				}
				else
				{
					evt->Payload[i].TlvData.NwkData = NULL;
				}

				FLib_MemCpy(evt->Payload[i].TlvData.NwkData, pPayload + idx, evt->Payload[i].TlvLength); idx += evt->Payload[i].TlvLength;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Ip6AddrList:

				if (evt->Payload[i].TlvLength > 0)
				{
					evt->Payload[i].TlvData.Ip6AddrList = MEM_BufferAlloc(evt->Payload[i].TlvLength);

					if (!evt->Payload[i].TlvData.Ip6AddrList)
					{
						MEM_BufferFree(evt->Payload);
						return MEM_ALLOC_ERROR_c;
					}

				}
				else
				{
					evt->Payload[i].TlvData.Ip6AddrList = NULL;
				}

				FLib_MemCpy(evt->Payload[i].TlvData.Ip6AddrList, pPayload + idx, evt->Payload[i].TlvLength); idx += evt->Payload[i].TlvLength;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_MacCounters:
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifInUnknownProtos), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifInUnknownProtos)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifInUnknownProtos);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifInErrors), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifInErrors)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifInErrors);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifOutErrors), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifOutErrors)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifOutErrors);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifInUcastPkts), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifInUcastPkts)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifInUcastPkts);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifInBroadcastPkts), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifInBroadcastPkts)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifInBroadcastPkts);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifInDiscards), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifInDiscards)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifInDiscards);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifOutUcastPkts), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifOutUcastPkts)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifOutUcastPkts);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifOutBroadcastPkts), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifOutBroadcastPkts)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifOutBroadcastPkts);
				FLib_MemCpy(&(evt->Payload[i].TlvData.MacCounters.ifOutDiscards), pPayload + idx, sizeof(evt->Payload[i].TlvData.MacCounters.ifOutDiscards)); idx += sizeof(evt->Payload[i].TlvData.MacCounters.ifOutDiscards);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_BatteryLevel:
				evt->Payload[i].TlvData.BatteryLevel = pPayload[idx]; idx++;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_SupplyVoltage:
				FLib_MemCpy(&(evt->Payload[i].TlvData.SupplyVoltage), pPayload + idx, sizeof(evt->Payload[i].TlvData.SupplyVoltage)); idx += sizeof(evt->Payload[i].TlvData.SupplyVoltage);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_ChildTable:

				if (evt->Payload[i].TlvLength > 0)
				{
					evt->Payload[i].TlvData.ChildTable = MEM_BufferAlloc(evt->Payload[i].TlvLength);

					if (!evt->Payload[i].TlvData.ChildTable)
					{
						MEM_BufferFree(evt->Payload);
						return MEM_ALLOC_ERROR_c;
					}

				}
				else
				{
					evt->Payload[i].TlvData.ChildTable = NULL;
				}

				FLib_MemCpy(evt->Payload[i].TlvData.ChildTable, pPayload + idx, evt->Payload[i].TlvLength); idx += evt->Payload[i].TlvLength;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_ChannelPages:
				evt->Payload[i].TlvData.ChannelPages = pPayload[idx]; idx++;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_Mac6lowPanNvmDataCount:
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_Mac6lowPanNvmDataCount.NvmDataSetId), pPayload + idx, 2); idx += 2;
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_Mac6lowPanNvmDataCount.DataSetCount), pPayload + idx, sizeof(evt->Payload[i].TlvData.Fsl_Mac6lowPanNvmDataCount.DataSetCount)); idx += sizeof(evt->Payload[i].TlvData.Fsl_Mac6lowPanNvmDataCount.DataSetCount);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_NetworkNvmDataCount_c:

				if (evt->Payload[i].TlvLength > 0)
				{
					evt->Payload[i].TlvData.Fsl_NetworkNvmDataCount_c = MEM_BufferAlloc(evt->Payload[i].TlvLength);

					if (!evt->Payload[i].TlvData.Fsl_NetworkNvmDataCount_c)
					{
						MEM_BufferFree(evt->Payload);
						return MEM_ALLOC_ERROR_c;
					}

				}
				else
				{
					evt->Payload[i].TlvData.Fsl_NetworkNvmDataCount_c = NULL;
				}

				FLib_MemCpy(evt->Payload[i].TlvData.Fsl_NetworkNvmDataCount_c, pPayload + idx, evt->Payload[i].TlvLength); idx += evt->Payload[i].TlvLength;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_SecurityNvmDataCount_c:
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_SecurityNvmDataCount_c.NvmDataSetId), pPayload + idx, 2); idx += 2;
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_SecurityNvmDataCount_c.DataSetCount), pPayload + idx, sizeof(evt->Payload[i].TlvData.Fsl_SecurityNvmDataCount_c.DataSetCount)); idx += sizeof(evt->Payload[i].TlvData.Fsl_SecurityNvmDataCount_c.DataSetCount);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_FunctionalNvmDataCount_c:
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_FunctionalNvmDataCount_c.NvmDataSetId), pPayload + idx, 2); idx += 2;
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_FunctionalNvmDataCount_c.DataSetCount), pPayload + idx, sizeof(evt->Payload[i].TlvData.Fsl_FunctionalNvmDataCount_c.DataSetCount)); idx += sizeof(evt->Payload[i].TlvData.Fsl_FunctionalNvmDataCount_c.DataSetCount);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_BoardName_c:

				if (evt->Payload[i].TlvLength > 0)
				{
					evt->Payload[i].TlvData.Fsl_BoardName_c = MEM_BufferAlloc(evt->Payload[i].TlvLength);

					if (!evt->Payload[i].TlvData.Fsl_BoardName_c)
					{
						MEM_BufferFree(evt->Payload);
						return MEM_ALLOC_ERROR_c;
					}

				}
				else
				{
					evt->Payload[i].TlvData.Fsl_BoardName_c = NULL;
				}

				FLib_MemCpy(evt->Payload[i].TlvData.Fsl_BoardName_c, pPayload + idx, evt->Payload[i].TlvLength); idx += evt->Payload[i].TlvLength;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_UniqueMcuId_c:
				FLib_MemCpy(evt->Payload[i].TlvData.Fsl_UniqueMcuId_c, pPayload + idx, sizeof(evt->Payload[i].TlvData.Fsl_UniqueMcuId_c)); idx += sizeof(evt->Payload[i].TlvData.Fsl_UniqueMcuId_c);
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_StackVersion_c:
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_StackVersion_c.StackVendorOUI), pPayload + idx, 3); idx += 3;
				FLib_MemCpy(&(evt->Payload[i].TlvData.Fsl_StackVersion_c.StackVersion), pPayload + idx, 3); idx += 3;
				break;

			case THR_MgmtDiagnosticGetRspIndication_Payload_TlvId_Fsl_SoftwareVersion_c:

				if (evt->Payload[i].TlvLength > 0)
				{
					evt->Payload[i].TlvData.Fsl_SoftwareVersion_c = MEM_BufferAlloc(evt->Payload[i].TlvLength);

					if (!evt->Payload[i].TlvData.Fsl_SoftwareVersion_c)
					{
						MEM_BufferFree(evt->Payload);
						return MEM_ALLOC_ERROR_c;
					}

				}
				else
				{
					evt->Payload[i].TlvData.Fsl_SoftwareVersion_c = NULL;
				}

				FLib_MemCpy(evt->Payload[i].TlvData.Fsl_SoftwareVersion_c, pPayload + idx, evt->Payload[i].TlvLength); idx += evt->Payload[i].TlvLength;
				break;
		}
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_DiagTestGetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The response to large network diagnostic get request.
***************************************************************************************************/
static memStatus_t Load_THR_DiagTestGetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_DiagTestGetRspIndication_t *evt = &(container->Data.THR_DiagTestGetRspIndication);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_DiagTestGetRspIndication_FSCI_ID;

	evt->Status = (THR_DiagTestGetRspIndication_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->CoapMsgId), pPayload + idx, sizeof(evt->CoapMsgId)); idx += sizeof(evt->CoapMsgId);
	FLib_MemCpy(&(evt->DataLen), pPayload + idx, sizeof(evt->DataLen)); idx += sizeof(evt->DataLen);
	evt->Payload.TlvId = (THR_DiagTestGetRspIndication_Payload_TlvId_t)pPayload[idx]; idx++;

	switch (evt->Payload.TlvId)
	{
		case THR_DiagTestGetRspIndication_Payload_TlvId_ColdFactoryReset:
			FLib_MemCpy(&(evt->Payload.TLVPayload.ColdFactoryReset), pPayload + idx, sizeof(evt->Payload.TLVPayload.ColdFactoryReset)); idx += sizeof(evt->Payload.TLVPayload.ColdFactoryReset);
			break;

		case THR_DiagTestGetRspIndication_Payload_TlvId_WarmCPUReset:
			FLib_MemCpy(&(evt->Payload.TLVPayload.WarmCPUReset), pPayload + idx, sizeof(evt->Payload.TLVPayload.WarmCPUReset)); idx += sizeof(evt->Payload.TLVPayload.WarmCPUReset);
			break;

		case THR_DiagTestGetRspIndication_Payload_TlvId_Data:
			FLib_MemCpy(&(evt->Payload.TLVPayload.Data.ReqLatency), pPayload + idx, sizeof(evt->Payload.TLVPayload.Data.ReqLatency)); idx += sizeof(evt->Payload.TLVPayload.Data.ReqLatency);
			FLib_MemCpy(&(evt->Payload.TLVPayload.Data.RspLatency), pPayload + idx, sizeof(evt->Payload.TLVPayload.Data.RspLatency)); idx += sizeof(evt->Payload.TLVPayload.Data.RspLatency);
			FLib_MemCpy(&(evt->Payload.TLVPayload.Data.Offset), pPayload + idx, sizeof(evt->Payload.TLVPayload.Data.Offset)); idx += sizeof(evt->Payload.TLVPayload.Data.Offset);
			evt->Payload.TLVPayload.Data.SequenceNumber = pPayload[idx]; idx++;
			evt->Payload.TLVPayload.Data.PayloadSize = pPayload[idx]; idx++;

			if (evt->Payload.TLVPayload.Data.PayloadSize > 0)
			{
				evt->Payload.TLVPayload.Data.Payload = MEM_BufferAlloc(evt->Payload.TLVPayload.Data.PayloadSize);

				if (!evt->Payload.TLVPayload.Data.Payload)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Payload.TLVPayload.Data.Payload = NULL;
			}

			FLib_MemCpy(evt->Payload.TLVPayload.Data.Payload, pPayload + idx, evt->Payload.TLVPayload.Data.PayloadSize); idx += evt->Payload.TLVPayload.Data.PayloadSize;
			break;

		case THR_DiagTestGetRspIndication_Payload_TlvId_Results:
			FLib_MemCpy(&(evt->Payload.TLVPayload.Results.ReqLatency), pPayload + idx, sizeof(evt->Payload.TLVPayload.Results.ReqLatency)); idx += sizeof(evt->Payload.TLVPayload.Results.ReqLatency);
			FLib_MemCpy(&(evt->Payload.TLVPayload.Results.RspLatency), pPayload + idx, sizeof(evt->Payload.TLVPayload.Results.RspLatency)); idx += sizeof(evt->Payload.TLVPayload.Results.RspLatency);
			FLib_MemCpy(&(evt->Payload.TLVPayload.Results.Offset), pPayload + idx, sizeof(evt->Payload.TLVPayload.Results.Offset)); idx += sizeof(evt->Payload.TLVPayload.Results.Offset);
			evt->Payload.TLVPayload.Results.SequenceNumber = pPayload[idx]; idx++;
			break;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_MgmtDiagnosticDiagTestEventIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The event generated by a multicast large network diagnostic get request.
***************************************************************************************************/
static memStatus_t Load_THR_MgmtDiagnosticDiagTestEventIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_MgmtDiagnosticDiagTestEventIndication_t *evt = &(container->Data.THR_MgmtDiagnosticDiagTestEventIndication);

	/* Store (OG, OC) in ID */
	container->id = THR_MgmtDiagnosticDiagTestEventIndication_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_MgmtDiagnosticDiagTestEventIndication_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_MgmtDiagnosticResetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The response to Network diagnostic reset request.
***************************************************************************************************/
static memStatus_t Load_THR_MgmtDiagnosticResetRspIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_MgmtDiagnosticResetRspIndication_t *evt = &(container->Data.THR_MgmtDiagnosticResetRspIndication);

	/* Store (OG, OC) in ID */
	container->id = THR_MgmtDiagnosticResetRspIndication_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_MgmtDiagnosticResetRspIndication_t));

	return MEM_SUCCESS_c;
}

#endif  /* THREADMGMT_ENABLE */

#if THREADUTILS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SetNwkIdTimeoutConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Set network ID timeout confirm
***************************************************************************************************/
static memStatus_t Load_THR_SetNwkIdTimeoutConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SetNwkIdTimeoutConfirm_t *evt = &(container->Data.THR_SetNwkIdTimeoutConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SetNwkIdTimeoutConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SetNwkIdTimeoutConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SetThresholdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Thread set threshold confirm
***************************************************************************************************/
static memStatus_t Load_THR_SetThresholdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SetThresholdConfirm_t *evt = &(container->Data.THR_SetThresholdConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SetThresholdConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SetThresholdConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GetNeighborInfoConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get detailed info about neighbor response.
***************************************************************************************************/
static memStatus_t Load_THR_GetNeighborInfoConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GetNeighborInfoConfirm_t *evt = &(container->Data.THR_GetNeighborInfoConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_GetNeighborInfoConfirm_FSCI_ID;

	evt->Status = (THR_GetNeighborInfoConfirm_Status_t)pPayload[idx]; idx++;

	switch (evt->Status)
	{
		case THR_GetNeighborInfoConfirm_Status_Success:
			FLib_MemCpy(&(evt->NeighborInfo.Success.ExtendedAddress), pPayload + idx, sizeof(evt->NeighborInfo.Success.ExtendedAddress)); idx += sizeof(evt->NeighborInfo.Success.ExtendedAddress);
			FLib_MemCpy(&(evt->NeighborInfo.Success.ShortAddress), pPayload + idx, sizeof(evt->NeighborInfo.Success.ShortAddress)); idx += sizeof(evt->NeighborInfo.Success.ShortAddress);
			FLib_MemCpy(&(evt->NeighborInfo.Success.LastCommTime), pPayload + idx, sizeof(evt->NeighborInfo.Success.LastCommTime)); idx += sizeof(evt->NeighborInfo.Success.LastCommTime);
			evt->NeighborInfo.Success.InRSSI = pPayload[idx]; idx++;
			FLib_MemCpy(&(evt->NeighborInfo.Success.Timeoutsec), pPayload + idx, sizeof(evt->NeighborInfo.Success.Timeoutsec)); idx += sizeof(evt->NeighborInfo.Success.Timeoutsec);
			break;

		case THR_GetNeighborInfoConfirm_Status_NeighborNotFound:
			break;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GetChildrenTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get neighbor table response.
***************************************************************************************************/
static memStatus_t Load_THR_GetChildrenTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GetChildrenTableConfirm_t *evt = &(container->Data.THR_GetChildrenTableConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_GetChildrenTableConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	evt->NoOfElements = pPayload[idx]; idx++;

	if (evt->NoOfElements > 0)
	{
		evt->NeighborEntries = MEM_BufferAlloc(evt->NoOfElements * sizeof(evt->NeighborEntries[0]));

		if (!evt->NeighborEntries)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->NeighborEntries = NULL;
	}


	for (uint32_t i = 0; i < evt->NoOfElements; i++)
	{
		FLib_MemCpy(&(evt->NeighborEntries[i].ExtendedAddress), pPayload + idx, sizeof(evt->NeighborEntries[i].ExtendedAddress)); idx += sizeof(evt->NeighborEntries[i].ExtendedAddress);
		FLib_MemCpy(&(evt->NeighborEntries[i].ShortAddress), pPayload + idx, sizeof(evt->NeighborEntries[i].ShortAddress)); idx += sizeof(evt->NeighborEntries[i].ShortAddress);
		FLib_MemCpy(&(evt->NeighborEntries[i].LastCommTime), pPayload + idx, sizeof(evt->NeighborEntries[i].LastCommTime)); idx += sizeof(evt->NeighborEntries[i].LastCommTime);
		evt->NeighborEntries[i].LastRSSI = pPayload[idx]; idx++;
		FLib_MemCpy(&(evt->NeighborEntries[i].Timeout), pPayload + idx, sizeof(evt->NeighborEntries[i].Timeout)); idx += sizeof(evt->NeighborEntries[i].Timeout);
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GetNeighborTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get neighbor table response.
***************************************************************************************************/
static memStatus_t Load_THR_GetNeighborTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GetNeighborTableConfirm_t *evt = &(container->Data.THR_GetNeighborTableConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_GetNeighborTableConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	evt->NoOfElements = pPayload[idx]; idx++;

	if (evt->NoOfElements > 0)
	{
		evt->NeighborEntries = MEM_BufferAlloc(evt->NoOfElements * sizeof(evt->NeighborEntries[0]));

		if (!evt->NeighborEntries)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->NeighborEntries = NULL;
	}


	for (uint32_t i = 0; i < evt->NoOfElements; i++)
	{
		FLib_MemCpy(&(evt->NeighborEntries[i].ExtendedAddress), pPayload + idx, sizeof(evt->NeighborEntries[i].ExtendedAddress)); idx += sizeof(evt->NeighborEntries[i].ExtendedAddress);
		FLib_MemCpy(&(evt->NeighborEntries[i].ShortAddress), pPayload + idx, sizeof(evt->NeighborEntries[i].ShortAddress)); idx += sizeof(evt->NeighborEntries[i].ShortAddress);
		FLib_MemCpy(&(evt->NeighborEntries[i].LastCommTime), pPayload + idx, sizeof(evt->NeighborEntries[i].LastCommTime)); idx += sizeof(evt->NeighborEntries[i].LastCommTime);
		evt->NeighborEntries[i].LastRSSI = pPayload[idx]; idx++;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GetRoutingTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get routing table response.
***************************************************************************************************/
static memStatus_t Load_THR_GetRoutingTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GetRoutingTableConfirm_t *evt = &(container->Data.THR_GetRoutingTableConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_GetRoutingTableConfirm_FSCI_ID;

	evt->NoOfElements = pPayload[idx]; idx++;
	evt->IdSequenceNb = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->RouterIDMask), pPayload + idx, sizeof(evt->RouterIDMask)); idx += sizeof(evt->RouterIDMask);

	if (evt->NoOfElements > 0)
	{
		evt->RoutingEntries = MEM_BufferAlloc(evt->NoOfElements * sizeof(evt->RoutingEntries[0]));

		if (!evt->RoutingEntries)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->RoutingEntries = NULL;
	}


	for (uint32_t i = 0; i < evt->NoOfElements; i++)
	{
		evt->RoutingEntries[i].RouterID = pPayload[idx]; idx++;
		FLib_MemCpy(&(evt->RoutingEntries[i].ShortAddress), pPayload + idx, sizeof(evt->RoutingEntries[i].ShortAddress)); idx += sizeof(evt->RoutingEntries[i].ShortAddress);
		FLib_MemCpy(&(evt->RoutingEntries[i].NextHop), pPayload + idx, sizeof(evt->RoutingEntries[i].NextHop)); idx += sizeof(evt->RoutingEntries[i].NextHop);
		evt->RoutingEntries[i].Cost = pPayload[idx]; idx++;
		evt->RoutingEntries[i].nIn = pPayload[idx]; idx++;
		evt->RoutingEntries[i].nOut = pPayload[idx]; idx++;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GetAttrConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get attribute response
***************************************************************************************************/
static memStatus_t Load_THR_GetAttrConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GetAttrConfirm_t *evt = &(container->Data.THR_GetAttrConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_GetAttrConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	evt->AttributeId = (THR_GetAttrConfirm_AttributeId_t)pPayload[idx]; idx++;
	evt->Index = pPayload[idx]; idx++;
	evt->Status = (THR_GetAttrConfirm_Status_t)pPayload[idx]; idx++;
	evt->AttrSize = pPayload[idx]; idx++;

	switch (evt->AttributeId)
	{
		case THR_GetAttrConfirm_AttributeId_RandomExtendedAddr:
			FLib_MemCpy(&(evt->AttributeValue.RandomExtendedAddr), pPayload + idx, sizeof(evt->AttributeValue.RandomExtendedAddr)); idx += sizeof(evt->AttributeValue.RandomExtendedAddr);
			break;

		case THR_GetAttrConfirm_AttributeId_ShortAddress:
			FLib_MemCpy(&(evt->AttributeValue.ShortAddress), pPayload + idx, sizeof(evt->AttributeValue.ShortAddress)); idx += sizeof(evt->AttributeValue.ShortAddress);
			break;

		case THR_GetAttrConfirm_AttributeId_ScanChannelMask:
			FLib_MemCpy(&(evt->AttributeValue.ScanChannelMask), pPayload + idx, sizeof(evt->AttributeValue.ScanChannelMask)); idx += sizeof(evt->AttributeValue.ScanChannelMask);
			break;

		case THR_GetAttrConfirm_AttributeId_ScanDuration:
			FLib_MemCpy(&(evt->AttributeValue.ScanDuration), pPayload + idx, sizeof(evt->AttributeValue.ScanDuration)); idx += sizeof(evt->AttributeValue.ScanDuration);
			break;

		case THR_GetAttrConfirm_AttributeId_Channel:
			evt->AttributeValue.Channel = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_ShortPanId:
			FLib_MemCpy(&(evt->AttributeValue.ShortPanId), pPayload + idx, sizeof(evt->AttributeValue.ShortPanId)); idx += sizeof(evt->AttributeValue.ShortPanId);
			break;

		case THR_GetAttrConfirm_AttributeId_ExtendedPanId:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.ExtendedPanId = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.ExtendedPanId)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.ExtendedPanId = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.ExtendedPanId, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_PermitJoin:
			evt->AttributeValue.PermitJoin = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_RxOnIdle:
			evt->AttributeValue.RxOnIdle = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_SedPollInterval:
			FLib_MemCpy(&(evt->AttributeValue.SedPollInterval), pPayload + idx, sizeof(evt->AttributeValue.SedPollInterval)); idx += sizeof(evt->AttributeValue.SedPollInterval);
			break;

		case THR_GetAttrConfirm_AttributeId_UniqueExtendedAddress:
			evt->AttributeValue.UniqueExtendedAddress = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_VendorName:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.VendorName = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.VendorName)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.VendorName = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.VendorName, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_ModelName:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.ModelName = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.ModelName)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.ModelName = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.ModelName, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_SwVersion:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.SwVersion = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.SwVersion)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.SwVersion = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.SwVersion, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_StackVersion:
			break;

		case THR_GetAttrConfirm_AttributeId_NwkCapabilities:
			evt->AttributeValue.NwkCapabilities = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_NwkName:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.NwkName = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.NwkName)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.NwkName = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.NwkName, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_DeviceType:
			evt->AttributeValue.DeviceType = (THR_GetAttrConfirm_AttributeValue_DeviceType_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_IsDevConnected:
			evt->AttributeValue.IsDevConnected = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_IsDevCommissioned:
			evt->AttributeValue.IsDevCommissioned = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_PartitionId:
			FLib_MemCpy(&(evt->AttributeValue.PartitionId), pPayload + idx, sizeof(evt->AttributeValue.PartitionId)); idx += sizeof(evt->AttributeValue.PartitionId);
			break;

		case THR_GetAttrConfirm_AttributeId_DeviceRole:
			evt->AttributeValue.DeviceRole = (THR_GetAttrConfirm_AttributeValue_DeviceRole_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_Security_NwkMasterKey:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.Security_NwkMasterKey = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.Security_NwkMasterKey)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.Security_NwkMasterKey = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.Security_NwkMasterKey, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_Security_NwkKeySeq:
			FLib_MemCpy(&(evt->AttributeValue.Security_NwkKeySeq), pPayload + idx, sizeof(evt->AttributeValue.Security_NwkKeySeq)); idx += sizeof(evt->AttributeValue.Security_NwkKeySeq);
			break;

		case THR_GetAttrConfirm_AttributeId_Security_PSKc:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.Security_PSKc = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.Security_PSKc)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.Security_PSKc = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.Security_PSKc, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_Security_PSKd:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.Security_PSKd = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.Security_PSKd)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.Security_PSKd = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.Security_PSKd, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_VendorData:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.VendorData = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.VendorData)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.VendorData = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.VendorData, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_MLPrefix:
			FLib_MemCpy(evt->AttributeValue.MLPrefix.PrefixData, pPayload + idx, 16); idx += 16;
			evt->AttributeValue.MLPrefix.PrefixLength = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_MacFilteringEntry:
			FLib_MemCpy(&(evt->AttributeValue.MacFilteringEntry.ExtendedAddress), pPayload + idx, sizeof(evt->AttributeValue.MacFilteringEntry.ExtendedAddress)); idx += sizeof(evt->AttributeValue.MacFilteringEntry.ExtendedAddress);
			FLib_MemCpy(&(evt->AttributeValue.MacFilteringEntry.ShortAddress), pPayload + idx, sizeof(evt->AttributeValue.MacFilteringEntry.ShortAddress)); idx += sizeof(evt->AttributeValue.MacFilteringEntry.ShortAddress);
			evt->AttributeValue.MacFilteringEntry.LinkIndicator = pPayload[idx]; idx++;
			evt->AttributeValue.MacFilteringEntry.BlockNeighbor = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_Security_KeyRotationInterval:
			FLib_MemCpy(&(evt->AttributeValue.Security_KeyRotationInterval), pPayload + idx, sizeof(evt->AttributeValue.Security_KeyRotationInterval)); idx += sizeof(evt->AttributeValue.Security_KeyRotationInterval);
			break;

		case THR_GetAttrConfirm_AttributeId_ChildAddrMask:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.ChildAddrMask = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.ChildAddrMask)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.ChildAddrMask = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.ChildAddrMask, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_ChildSEDTimeout:
			FLib_MemCpy(&(evt->AttributeValue.ChildSEDTimeout), pPayload + idx, sizeof(evt->AttributeValue.ChildSEDTimeout)); idx += sizeof(evt->AttributeValue.ChildSEDTimeout);
			break;

		case THR_GetAttrConfirm_AttributeId_ChildEDTimeout:
			FLib_MemCpy(&(evt->AttributeValue.ChildEDTimeout), pPayload + idx, sizeof(evt->AttributeValue.ChildEDTimeout)); idx += sizeof(evt->AttributeValue.ChildEDTimeout);
			break;

		case THR_GetAttrConfirm_AttributeId_EndDevice_ChildEDReqFullNwkData:
			evt->AttributeValue.EndDevice_ChildEDReqFullNwkData = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_EndDevice_IsFastPollEnabled:
			evt->AttributeValue.EndDevice_IsFastPollEnabled = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_SleepyEndDevice_FastPollInterval:
			FLib_MemCpy(&(evt->AttributeValue.SleepyEndDevice_FastPollInterval), pPayload + idx, sizeof(evt->AttributeValue.SleepyEndDevice_FastPollInterval)); idx += sizeof(evt->AttributeValue.SleepyEndDevice_FastPollInterval);
			break;

		case THR_GetAttrConfirm_AttributeId_JoinLqiThreshold:
			evt->AttributeValue.JoinLqiThreshold = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_ProvisioningURL:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.ProvisioningURL = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.ProvisioningURL)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.ProvisioningURL = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.ProvisioningURL, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_SelectBestChannelEDThreshold:
			evt->AttributeValue.SelectBestChannelEDThreshold = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_CommissionerMode:
			evt->AttributeValue.CommissionerMode = (THR_GetAttrConfirm_AttributeValue_CommissionerMode_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_BorderRouter_BrPrefixEntry:
			evt->AttributeValue.BorderRouter_BrPrefixEntry.prefixLength = pPayload[idx]; idx++;
			FLib_MemCpy(evt->AttributeValue.BorderRouter_BrPrefixEntry.PrefixValue, pPayload + idx, 16); idx += 16;
			evt->AttributeValue.BorderRouter_BrPrefixEntry.PrefixFlagsReserved = pPayload[idx]; idx++;
			evt->AttributeValue.BorderRouter_BrPrefixEntry.PrefixFlags = pPayload[idx]; idx++;
			FLib_MemCpy(&(evt->AttributeValue.BorderRouter_BrPrefixEntry.prefixLifetime), pPayload + idx, sizeof(evt->AttributeValue.BorderRouter_BrPrefixEntry.prefixLifetime)); idx += sizeof(evt->AttributeValue.BorderRouter_BrPrefixEntry.prefixLifetime);
			evt->AttributeValue.BorderRouter_BrPrefixEntry.prefixAdvertised = (bool_t)pPayload[idx]; idx++;
			evt->AttributeValue.BorderRouter_BrPrefixEntry.ExternalRouteFlags = pPayload[idx]; idx++;
			FLib_MemCpy(&(evt->AttributeValue.BorderRouter_BrPrefixEntry.ExternalRouteLifetime), pPayload + idx, sizeof(evt->AttributeValue.BorderRouter_BrPrefixEntry.ExternalRouteLifetime)); idx += sizeof(evt->AttributeValue.BorderRouter_BrPrefixEntry.ExternalRouteLifetime);
			evt->AttributeValue.BorderRouter_BrPrefixEntry.ExternalRouteAdvertised = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_SteeringData:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.SteeringData = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.SteeringData)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.SteeringData = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.SteeringData, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_Security_KeySwitchGuardTime:
			FLib_MemCpy(&(evt->AttributeValue.Security_KeySwitchGuardTime), pPayload + idx, sizeof(evt->AttributeValue.Security_KeySwitchGuardTime)); idx += sizeof(evt->AttributeValue.Security_KeySwitchGuardTime);
			break;

		case THR_GetAttrConfirm_AttributeId_ParentHoldTime:
			FLib_MemCpy(&(evt->AttributeValue.ParentHoldTime), pPayload + idx, sizeof(evt->AttributeValue.ParentHoldTime)); idx += sizeof(evt->AttributeValue.ParentHoldTime);
			break;

		case THR_GetAttrConfirm_AttributeId_Security_Policy:
			evt->AttributeValue.Security_Policy = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_NVM_RestoreAutoStart:
			evt->AttributeValue.NVM_RestoreAutoStart = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_NVM_Restore:
			evt->AttributeValue.NVM_Restore = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_SlaacPolicy:
			evt->AttributeValue.SlaacPolicy = (THR_GetAttrConfirm_AttributeValue_SlaacPolicy_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_IeeeExtendedAddr:
			FLib_MemCpy(&(evt->AttributeValue.IeeeExtendedAddr), pPayload + idx, sizeof(evt->AttributeValue.IeeeExtendedAddr)); idx += sizeof(evt->AttributeValue.IeeeExtendedAddr);
			break;

		case THR_GetAttrConfirm_AttributeId_LeaderWeight:
			evt->AttributeValue.LeaderWeight = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_HashIeeeAddr:
			FLib_MemCpy(&(evt->AttributeValue.HashIeeeAddr), pPayload + idx, sizeof(evt->AttributeValue.HashIeeeAddr)); idx += sizeof(evt->AttributeValue.HashIeeeAddr);
			break;

		case THR_GetAttrConfirm_AttributeId_BorderRouter_BrGlobalOnMeshPrefix:
			FLib_MemCpy(evt->AttributeValue.BorderRouter_BrGlobalOnMeshPrefix.PrefixData, pPayload + idx, 16); idx += 16;
			evt->AttributeValue.BorderRouter_BrGlobalOnMeshPrefix.PrefixLength = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_BorderRouter_BrDefaultRouteOnMeshPrefix:
			evt->AttributeValue.BorderRouter_BrDefaultRouteOnMeshPrefix = (bool_t)pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_BorderRouter_BrExternalIfPrefix:
			FLib_MemCpy(evt->AttributeValue.BorderRouter_BrExternalIfPrefix.PrefixData, pPayload + idx, 16); idx += 16;
			evt->AttributeValue.BorderRouter_BrExternalIfPrefix.PrefixLength = pPayload[idx]; idx++;
			break;

		case THR_GetAttrConfirm_AttributeId_ActiveTimestamp:
			FLib_MemCpy(evt->AttributeValue.ActiveTimestamp.ActiveSeconds, pPayload + idx, 6); idx += 6;
			FLib_MemCpy(evt->AttributeValue.ActiveTimestamp.ActiveTicks, pPayload + idx, 2); idx += 2;
			break;

		case THR_GetAttrConfirm_AttributeId_PendingChannel:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingChannelMask:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingXpanId:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingMLprefix:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingNwkMasterKey:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingNwkName:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingPanId:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingPSK:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingSecurityPolicy:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingNwkKeyRotationInterval:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingDelayTimer:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingActiveTimestamp:
			break;

		case THR_GetAttrConfirm_AttributeId_PendingTimestamp:
			break;

		case THR_GetAttrConfirm_AttributeId_CommissionerId:

			if (evt->AttrSize > 0)
			{
				evt->AttributeValue.CommissionerID = MEM_BufferAlloc(evt->AttrSize);

				if (!evt->AttributeValue.CommissionerID)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->AttributeValue.CommissionerID = NULL;
			}

			FLib_MemCpy(evt->AttributeValue.CommissionerID, pPayload + idx, evt->AttrSize); idx += evt->AttrSize;
			break;

		case THR_GetAttrConfirm_AttributeId_JoinerPort:
			FLib_MemCpy(&(evt->AttributeValue.JoinerPort), pPayload + idx, sizeof(evt->AttributeValue.JoinerPort)); idx += sizeof(evt->AttributeValue.JoinerPort);
			break;

		case THR_GetAttrConfirm_AttributeId_CommissionerUdpPort:
			FLib_MemCpy(&(evt->AttributeValue.CommissionerUdpPort), pPayload + idx, sizeof(evt->AttributeValue.CommissionerUdpPort)); idx += sizeof(evt->AttributeValue.CommissionerUdpPort);
			break;

		case THR_GetAttrConfirm_AttributeId_DiscoveryReqMacTxOptions:
			break;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SetAttrConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Set attribute confirm
***************************************************************************************************/
static memStatus_t Load_THR_SetAttrConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SetAttrConfirm_t *evt = &(container->Data.THR_SetAttrConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SetAttrConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SetAttrConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GetThreadIpAddrConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get thread IP addr confirm
***************************************************************************************************/
static memStatus_t Load_THR_GetThreadIpAddrConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GetThreadIpAddrConfirm_t *evt = &(container->Data.THR_GetThreadIpAddrConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_GetThreadIpAddrConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	evt->Status = (THR_GetThreadIpAddrConfirm_Status_t)pPayload[idx]; idx++;
	evt->AddressType = pPayload[idx]; idx++;
	evt->NoOfIpAddr = pPayload[idx]; idx++;

	if (evt->NoOfIpAddr > 0)
	{
		evt->AddressList = MEM_BufferAlloc(evt->NoOfIpAddr * 16);

		if (!evt->AddressList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->AddressList = NULL;
	}

	FLib_MemCpy(evt->AddressList, pPayload + idx, evt->NoOfIpAddr * 16); idx += evt->NoOfIpAddr * 16;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GetParentConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Get parent confirm
***************************************************************************************************/
static memStatus_t Load_THR_GetParentConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GetParentConfirm_t *evt = &(container->Data.THR_GetParentConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_GetParentConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_GetParentConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_ChildUpdateToParentConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the THR_ChildUpdateToParent.Request.
***************************************************************************************************/
static memStatus_t Load_THR_ChildUpdateToParentConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_ChildUpdateToParentConfirm_t *evt = &(container->Data.THR_ChildUpdateToParentConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_ChildUpdateToParentConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_ChildUpdateToParentConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_LeaderRemoveRouterIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of remove router id request.
***************************************************************************************************/
static memStatus_t Load_THR_LeaderRemoveRouterIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_LeaderRemoveRouterIdConfirm_t *evt = &(container->Data.THR_LeaderRemoveRouterIdConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_LeaderRemoveRouterIdConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_LeaderRemoveRouterIdConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_GenerateAllKeysConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of THR_GenerateAllKeys.Request.
***************************************************************************************************/
static memStatus_t Load_THR_GenerateAllKeysConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_GenerateAllKeysConfirm_t *evt = &(container->Data.THR_GenerateAllKeysConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_GenerateAllKeysConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_GenerateAllKeysConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_SwitchKeyKeyConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of ThrSwitchKey request.
***************************************************************************************************/
static memStatus_t Load_THR_SwitchKeyKeyConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_SwitchKeyKeyConfirm_t *evt = &(container->Data.THR_SwitchKeyKeyConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_SwitchKeyKeyConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_SwitchKeyKeyConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_BrPrefixAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of THR_BrPrefixAddEntry request.
***************************************************************************************************/
static memStatus_t Load_THR_BrPrefixAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_BrPrefixAddEntryConfirm_t *evt = &(container->Data.THR_BrPrefixAddEntryConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_BrPrefixAddEntryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_BrPrefixAddEntryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_BrPrefixGetTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	THR_BrPrefixGetTable response.
***************************************************************************************************/
static memStatus_t Load_THR_BrPrefixGetTableConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_BrPrefixGetTableConfirm_t *evt = &(container->Data.THR_BrPrefixGetTableConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = THR_BrPrefixGetTableConfirm_FSCI_ID;

	evt->InstanceId = pPayload[idx]; idx++;
	evt->NoOfElements = pPayload[idx]; idx++;

	if (evt->NoOfElements > 0)
	{
		evt->BrPrefixEntries = MEM_BufferAlloc(evt->NoOfElements * sizeof(evt->BrPrefixEntries[0]));

		if (!evt->BrPrefixEntries)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->BrPrefixEntries = NULL;
	}


	for (uint32_t i = 0; i < evt->NoOfElements; i++)
	{
		evt->BrPrefixEntries[i].prefixLength = pPayload[idx]; idx++;
		FLib_MemCpy(evt->BrPrefixEntries[i].PrefixValue, pPayload + idx, 16); idx += 16;
		evt->BrPrefixEntries[i].PrefixFlagsReserved = pPayload[idx]; idx++;
		evt->BrPrefixEntries[i].PrefixFlags = pPayload[idx]; idx++;
		FLib_MemCpy(&(evt->BrPrefixEntries[i].prefixLifetime), pPayload + idx, sizeof(evt->BrPrefixEntries[i].prefixLifetime)); idx += sizeof(evt->BrPrefixEntries[i].prefixLifetime);
		evt->BrPrefixEntries[i].prefixAdvertised = (bool_t)pPayload[idx]; idx++;
		evt->BrPrefixEntries[i].ExternalRouteFlags = pPayload[idx]; idx++;
		FLib_MemCpy(&(evt->BrPrefixEntries[i].ExternalRouteLifetime), pPayload + idx, sizeof(evt->BrPrefixEntries[i].ExternalRouteLifetime)); idx += sizeof(evt->BrPrefixEntries[i].ExternalRouteLifetime);
		evt->BrPrefixEntries[i].ExternalRouteAdvertised = (bool_t)pPayload[idx]; idx++;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_BrPrefixRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of THR_BrPrefixRemoveEntry request.
***************************************************************************************************/
static memStatus_t Load_THR_BrPrefixRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_BrPrefixRemoveEntryConfirm_t *evt = &(container->Data.THR_BrPrefixRemoveEntryConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_BrPrefixRemoveEntryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_BrPrefixRemoveEntryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_BrServiceRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of THR_BrServiceRemoveEntry request.
***************************************************************************************************/
static memStatus_t Load_THR_BrServiceRemoveEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_BrServiceRemoveEntryConfirm_t *evt = &(container->Data.THR_BrServiceRemoveEntryConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_BrServiceRemoveEntryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_BrServiceRemoveEntryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_BrServiceAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of THR_BrServiceAddEntry request.
***************************************************************************************************/
static memStatus_t Load_THR_BrServiceAddEntryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_BrServiceAddEntryConfirm_t *evt = &(container->Data.THR_BrServiceAddEntryConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_BrServiceAddEntryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_BrServiceAddEntryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_BrPrefixSyncConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of THR_BrPrefixSync request.
***************************************************************************************************/
static memStatus_t Load_THR_BrPrefixSyncConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_BrPrefixSyncConfirm_t *evt = &(container->Data.THR_BrPrefixSyncConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_BrPrefixSyncConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_BrPrefixSyncConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_BrPrefixRemoveAllConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of THR_BrPrefixRemoveAll request.
***************************************************************************************************/
static memStatus_t Load_THR_BrPrefixRemoveAllConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_BrPrefixRemoveAllConfirm_t *evt = &(container->Data.THR_BrPrefixRemoveAllConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_BrPrefixRemoveAllConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_BrPrefixRemoveAllConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_EidToRlocMapResponse(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Response to EidToRlocMap request.
***************************************************************************************************/
static memStatus_t Load_NWKU_EidToRlocMapResponse(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_EidToRlocMapResponse_t *evt = &(container->Data.NWKU_EidToRlocMapResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = NWKU_EidToRlocMapResponse_FSCI_ID;

	evt->NumberOfEntries = pPayload[idx]; idx++;

	if (evt->NumberOfEntries > 0)
	{
		evt->CacheEntries = MEM_BufferAlloc(evt->NumberOfEntries * sizeof(evt->CacheEntries[0]));

		if (!evt->CacheEntries)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->CacheEntries = NULL;
	}


	for (uint32_t i = 0; i < evt->NumberOfEntries; i++)
	{
		FLib_MemCpy(evt->CacheEntries[i].EID, pPayload + idx, 16); idx += 16;
		FLib_MemCpy(evt->CacheEntries[i].RLOC, pPayload + idx, 2); idx += 2;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_THR_IdentifyConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the board identification command.
***************************************************************************************************/
static memStatus_t Load_THR_IdentifyConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	THR_IdentifyConfirm_t *evt = &(container->Data.THR_IdentifyConfirm);

	/* Store (OG, OC) in ID */
	container->id = THR_IdentifyConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(THR_IdentifyConfirm_t));

	return MEM_SUCCESS_c;
}

#endif  /* THREADUTILS_ENABLE */

#if IPUTILS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_IfconfigBindConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation of the bind to interface request.
***************************************************************************************************/
static memStatus_t Load_NWKU_IfconfigBindConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_IfconfigBindConfirm_t *evt = &(container->Data.NWKU_IfconfigBindConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_IfconfigBindConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_IfconfigBindConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_IfconfigAllResponse(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Response to IfconfigAll request.
***************************************************************************************************/
static memStatus_t Load_NWKU_IfconfigAllResponse(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_IfconfigAllResponse_t *evt = &(container->Data.NWKU_IfconfigAllResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = NWKU_IfconfigAllResponse_FSCI_ID;

	evt->CountInterfaces = pPayload[idx]; idx++;

	if (evt->CountInterfaces > 0)
	{
		evt->InterfaceIDs = MEM_BufferAlloc(evt->CountInterfaces * sizeof(evt->InterfaceIDs[0]));

		if (!evt->InterfaceIDs)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->InterfaceIDs = NULL;
	}


	for (uint32_t i = 0; i < evt->CountInterfaces; i++)
	{
		evt->InterfaceIDs[i].InterfaceID = pPayload[idx]; idx++;
		evt->InterfaceIDs[i].CountIpAddresses = pPayload[idx]; idx++;

		if (evt->InterfaceIDs[i].CountIpAddresses > 0)
		{
			evt->InterfaceIDs[i].Addresses = MEM_BufferAlloc(evt->InterfaceIDs[i].CountIpAddresses * 16);

			if (!evt->InterfaceIDs[i].Addresses)
			{
				MEM_BufferFree(evt->InterfaceIDs);
				return MEM_ALLOC_ERROR_c;
			}

		}
		else
		{
			evt->InterfaceIDs[i].Addresses = NULL;
		}

		FLib_MemCpy(evt->InterfaceIDs[i].Addresses, pPayload + idx, evt->InterfaceIDs[i].CountIpAddresses * 16); idx += evt->InterfaceIDs[i].CountIpAddresses * 16;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_PingConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The response to ping request.
***************************************************************************************************/
static memStatus_t Load_NWKU_PingConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_PingConfirm_t *evt = &(container->Data.NWKU_PingConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_PingConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_PingConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_EchoUDPConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The response to echo UDP request.
***************************************************************************************************/
static memStatus_t Load_NWKU_EchoUDPConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_EchoUDPConfirm_t *evt = &(container->Data.NWKU_EchoUDPConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_EchoUDPConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_EchoUDPConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_CoapMsgReceivedIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The notification for receiving a CoAP message
***************************************************************************************************/
static memStatus_t Load_NWKU_CoapMsgReceivedIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_CoapMsgReceivedIndication_t *evt = &(container->Data.NWKU_CoapMsgReceivedIndication);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = NWKU_CoapMsgReceivedIndication_FSCI_ID;

	evt->Status = (NWKU_CoapMsgReceivedIndication_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(evt->RemoteIpAddress, pPayload + idx, sizeof(evt->RemoteIpAddress)); idx += sizeof(evt->RemoteIpAddress);
	FLib_MemCpy(&(evt->UDPPort), pPayload + idx, sizeof(evt->UDPPort)); idx += sizeof(evt->UDPPort);
	evt->RequestType = (NWKU_CoapMsgReceivedIndication_RequestType_t)pPayload[idx]; idx++;
	evt->MessageType = (NWKU_CoapMsgReceivedIndication_MessageType_t)pPayload[idx]; idx++;
	FLib_MemCpy(evt->URIpath, pPayload + idx, 30); idx += 30;
	evt->PayloadLength = pPayload[idx]; idx++;

	if (evt->PayloadLength > 0)
	{
		evt->Payload = MEM_BufferAlloc(evt->PayloadLength);

		if (!evt->Payload)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Payload = NULL;
	}

	FLib_MemCpy(evt->Payload, pPayload + idx, evt->PayloadLength); idx += evt->PayloadLength;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_DnsResponseReceivedIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The notification for receiving a CoAP message
***************************************************************************************************/
static memStatus_t Load_NWKU_DnsResponseReceivedIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_DnsResponseReceivedIndication_t *evt = &(container->Data.NWKU_DnsResponseReceivedIndication);

	/* Store (OG, OC) in ID */
	container->id = NWKU_DnsResponseReceivedIndication_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_DnsResponseReceivedIndication_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_CoapRegisterConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation for registering CoAP URI-path.
***************************************************************************************************/
static memStatus_t Load_NWKU_CoapRegisterConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_CoapRegisterConfirm_t *evt = &(container->Data.NWKU_CoapRegisterConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_CoapRegisterConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_CoapRegisterConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_CoapCreateInstanceConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation for createing a CoAP instance.
***************************************************************************************************/
static memStatus_t Load_NWKU_CoapCreateInstanceConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_CoapCreateInstanceConfirm_t *evt = &(container->Data.NWKU_CoapCreateInstanceConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_CoapCreateInstanceConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_CoapCreateInstanceConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_CoapSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation for sending CoAP message.
***************************************************************************************************/
static memStatus_t Load_NWKU_CoapSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_CoapSendConfirm_t *evt = &(container->Data.NWKU_CoapSendConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_CoapSendConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_CoapSendConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_DnsSendRequestConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation for sending a DNS request message.
***************************************************************************************************/
static memStatus_t Load_NWKU_DnsSendRequestConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_DnsSendRequestConfirm_t *evt = &(container->Data.NWKU_DnsSendRequestConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_DnsSendRequestConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_DnsSendRequestConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_McastGroupManageConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation for sending a McastGroupManage request message.
***************************************************************************************************/
static memStatus_t Load_NWKU_McastGroupManageConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_McastGroupManageConfirm_t *evt = &(container->Data.NWKU_McastGroupManageConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_McastGroupManageConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_McastGroupManageConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_McastGroupShowResponse(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Response to McastGroupShow request.
***************************************************************************************************/
static memStatus_t Load_NWKU_McastGroupShowResponse(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_McastGroupShowResponse_t *evt = &(container->Data.NWKU_McastGroupShowResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = NWKU_McastGroupShowResponse_FSCI_ID;

	evt->CountIpAddresses = pPayload[idx]; idx++;

	if (evt->CountIpAddresses > 0)
	{
		evt->Addresses = MEM_BufferAlloc(evt->CountIpAddresses * 16);

		if (!evt->Addresses)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Addresses = NULL;
	}

	FLib_MemCpy(evt->Addresses, pPayload + idx, evt->CountIpAddresses * 16); idx += evt->CountIpAddresses * 16;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_RoutesManageConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	The confirmation for sending a RoutesManage request message.
***************************************************************************************************/
static memStatus_t Load_NWKU_RoutesManageConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_RoutesManageConfirm_t *evt = &(container->Data.NWKU_RoutesManageConfirm);

	/* Store (OG, OC) in ID */
	container->id = NWKU_RoutesManageConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(NWKU_RoutesManageConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_NWKU_RoutesShowResponse(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Response to RoutesShow request.
***************************************************************************************************/
static memStatus_t Load_NWKU_RoutesShowResponse(thrEvtContainer_t *container, uint8_t *pPayload)
{
	NWKU_RoutesShowResponse_t *evt = &(container->Data.NWKU_RoutesShowResponse);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = NWKU_RoutesShowResponse_FSCI_ID;

	evt->CountRoutes = pPayload[idx]; idx++;

	if (evt->CountRoutes > 0)
	{
		evt->Route = MEM_BufferAlloc(evt->CountRoutes * sizeof(evt->Route[0]));

		if (!evt->Route)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Route = NULL;
	}


	for (uint32_t i = 0; i < evt->CountRoutes; i++)
	{
		FLib_MemCpy(evt->Route[i].Destination, pPayload + idx, 16); idx += 16;
		FLib_MemCpy(evt->Route[i].NextHop, pPayload + idx, 16); idx += 16;
		evt->Route[i].PrefixLength = pPayload[idx]; idx++;
		evt->Route[i].RouteMetric = pPayload[idx]; idx++;
		evt->Route[i].InstanceId = pPayload[idx]; idx++;
	}

	return MEM_SUCCESS_c;
}

#endif  /* IPUTILS_ENABLE */

#if MESHCOP_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_StartCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for starting a Commissioner
***************************************************************************************************/
static memStatus_t Load_MESHCOP_StartCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_StartCommissionerConfirm_t *evt = &(container->Data.MESHCOP_StartCommissionerConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_StartCommissionerConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_StartCommissionerConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_StartNativeCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for starting a Native Commissioner
***************************************************************************************************/
static memStatus_t Load_MESHCOP_StartNativeCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_StartNativeCommissionerConfirm_t *evt = &(container->Data.MESHCOP_StartNativeCommissionerConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_StartNativeCommissionerConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_StartNativeCommissionerConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_StopCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for stopping a Commissioner
***************************************************************************************************/
static memStatus_t Load_MESHCOP_StopCommissionerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_StopCommissionerConfirm_t *evt = &(container->Data.MESHCOP_StopCommissionerConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_StopCommissionerConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_StopCommissionerConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_AddExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for adding a Joiner in the Commissioner list
***************************************************************************************************/
static memStatus_t Load_MESHCOP_AddExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_AddExpectedJoinerConfirm_t *evt = &(container->Data.MESHCOP_AddExpectedJoinerConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_AddExpectedJoinerConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_AddExpectedJoinerConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_GetExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for getting information about Joiner from the Commissioner list
***************************************************************************************************/
static memStatus_t Load_MESHCOP_GetExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_GetExpectedJoinerConfirm_t *evt = &(container->Data.MESHCOP_GetExpectedJoinerConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_GetExpectedJoinerConfirm_FSCI_ID;

	evt->Status = (MESHCOP_GetExpectedJoinerConfirm_Status_t)pPayload[idx]; idx++;
	evt->Selected = (MESHCOP_GetExpectedJoinerConfirm_Selected_t)pPayload[idx]; idx++;
	evt->PSKdSize = pPayload[idx]; idx++;

	if (evt->PSKdSize > 0)
	{
		evt->PSKd = MEM_BufferAlloc(evt->PSKdSize);

		if (!evt->PSKd)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->PSKd = NULL;
	}

	FLib_MemCpy(evt->PSKd, pPayload + idx, evt->PSKdSize); idx += evt->PSKdSize;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_RemoveExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for removing a Joiner from the Commissioner list
***************************************************************************************************/
static memStatus_t Load_MESHCOP_RemoveExpectedJoinerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_RemoveExpectedJoinerConfirm_t *evt = &(container->Data.MESHCOP_RemoveExpectedJoinerConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_RemoveExpectedJoinerConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_RemoveExpectedJoinerConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_RemoveAllExpectedJoinersConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for removing all Joiners from the Commissioner's list
***************************************************************************************************/
static memStatus_t Load_MESHCOP_RemoveAllExpectedJoinersConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_RemoveAllExpectedJoinersConfirm_t *evt = &(container->Data.MESHCOP_RemoveAllExpectedJoinersConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_RemoveAllExpectedJoinersConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_RemoveAllExpectedJoinersConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_SyncSteeringDataConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for syncing the steering data
***************************************************************************************************/
static memStatus_t Load_MESHCOP_SyncSteeringDataConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_SyncSteeringDataConfirm_t *evt = &(container->Data.MESHCOP_SyncSteeringDataConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_SyncSteeringDataConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_SyncSteeringDataConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the Management Set request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtSetConfirm_t *evt = &(container->Data.MESHCOP_MgmtSetConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtSetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtSetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the Management Get request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtGetConfirm_t *evt = &(container->Data.MESHCOP_MgmtGetConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtGetConfirm_FSCI_ID;

	evt->Status = (MESHCOP_MgmtGetConfirm_Status_t)pPayload[idx]; idx++;
	evt->Type = (MESHCOP_MgmtGetConfirm_Type_t)pPayload[idx]; idx++;
	evt->Length = pPayload[idx]; idx++;

	switch (evt->Type)
	{
		case MESHCOP_MgmtGetConfirm_Type_Channel:
			evt->Value.Channel = pPayload[idx]; idx++;
			break;

		case MESHCOP_MgmtGetConfirm_Type_ChannelMask:

			if (evt->Length > 0)
			{
				evt->Value.ChannelMask = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.ChannelMask)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.ChannelMask = NULL;
			}

			FLib_MemCpy(evt->Value.ChannelMask, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_PanId:

			if (evt->Length > 0)
			{
				evt->Value.PanId = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.PanId)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.PanId = NULL;
			}

			FLib_MemCpy(evt->Value.PanId, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_XpanId:

			if (evt->Length > 0)
			{
				evt->Value.XpanId = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.XpanId)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.XpanId = NULL;
			}

			FLib_MemCpy(evt->Value.XpanId, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_NetworkName:

			if (evt->Length > 0)
			{
				evt->Value.NetworkName = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.NetworkName)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.NetworkName = NULL;
			}

			FLib_MemCpy(evt->Value.NetworkName, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_PSKc:

			if (evt->Length > 0)
			{
				evt->Value.PSKc = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.PSKc)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.PSKc = NULL;
			}

			FLib_MemCpy(evt->Value.PSKc, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_MasterKey:

			if (evt->Length > 0)
			{
				evt->Value.MasterKey = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.MasterKey)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.MasterKey = NULL;
			}

			FLib_MemCpy(evt->Value.MasterKey, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_KeySequence:

			if (evt->Length > 0)
			{
				evt->Value.KeySequence = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.KeySequence)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.KeySequence = NULL;
			}

			FLib_MemCpy(evt->Value.KeySequence, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_MeshLocalUla:

			if (evt->Length > 0)
			{
				evt->Value.MeshLocalUla = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.MeshLocalUla)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.MeshLocalUla = NULL;
			}

			FLib_MemCpy(evt->Value.MeshLocalUla, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_SteeringData:

			if (evt->Length > 0)
			{
				evt->Value.SteeringData = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.SteeringData)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.SteeringData = NULL;
			}

			FLib_MemCpy(evt->Value.SteeringData, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_BorderRouterLocator:

			if (evt->Length > 0)
			{
				evt->Value.BorderRouterLocator = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.BorderRouterLocator)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.BorderRouterLocator = NULL;
			}

			FLib_MemCpy(evt->Value.BorderRouterLocator, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_CommissionerID:

			if (evt->Length > 0)
			{
				evt->Value.CommissionerID = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.CommissionerID)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.CommissionerID = NULL;
			}

			FLib_MemCpy(evt->Value.CommissionerID, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_CommissionerSessionID:

			if (evt->Length > 0)
			{
				evt->Value.CommissionerSessionID = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.CommissionerSessionID)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.CommissionerSessionID = NULL;
			}

			FLib_MemCpy(evt->Value.CommissionerSessionID, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_SecurityPolicy:

			if (evt->Length > 0)
			{
				evt->Value.SecurityPolicy = MEM_BufferAlloc(evt->Length);

				if (!evt->Value.SecurityPolicy)
				{
					return MEM_ALLOC_ERROR_c;
				}

			}
			else
			{
				evt->Value.SecurityPolicy = NULL;
			}

			FLib_MemCpy(evt->Value.SecurityPolicy, pPayload + idx, evt->Length); idx += evt->Length;
			break;

		case MESHCOP_MgmtGetConfirm_Type_DelayTimer:
			break;

		case MESHCOP_MgmtGetConfirm_Type_ActiveTimestamp:
			break;

		case MESHCOP_MgmtGetConfirm_Type_PendingTimestamp:
			break;
	}

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_SetCommissionerCredentialConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for setting up a Native Commissioner credential
***************************************************************************************************/
static memStatus_t Load_MESHCOP_SetCommissionerCredentialConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_SetCommissionerCredentialConfirm_t *evt = &(container->Data.MESHCOP_SetCommissionerCredentialConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_SetCommissionerCredentialConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_SetCommissionerCredentialConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmNwkFormConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Network form confirmation message
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmNwkFormConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmNwkFormConfirm_t *evt = &(container->Data.MESHCOP_MgmNwkFormConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmNwkFormConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmNwkFormConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtCommissionerGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the Management Commissioner Get request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtCommissionerGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtCommissionerGetConfirm_t *evt = &(container->Data.MESHCOP_MgmtCommissionerGetConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtCommissionerGetConfirm_FSCI_ID;

	evt->Status = (MESHCOP_MgmtCommissionerGetConfirm_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Length), pPayload + idx, sizeof(evt->Length)); idx += sizeof(evt->Length);

	if (evt->Length > 0)
	{
		evt->TLVs = MEM_BufferAlloc(evt->Length);

		if (!evt->TLVs)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->TLVs = NULL;
	}

	FLib_MemCpy(evt->TLVs, pPayload + idx, evt->Length); idx += evt->Length;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtActiveGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the Management Active Get request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtActiveGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtActiveGetConfirm_t *evt = &(container->Data.MESHCOP_MgmtActiveGetConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtActiveGetConfirm_FSCI_ID;

	evt->Status = (MESHCOP_MgmtActiveGetConfirm_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Length), pPayload + idx, sizeof(evt->Length)); idx += sizeof(evt->Length);

	if (evt->Length > 0)
	{
		evt->TLVs = MEM_BufferAlloc(evt->Length);

		if (!evt->TLVs)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->TLVs = NULL;
	}

	FLib_MemCpy(evt->TLVs, pPayload + idx, evt->Length); idx += evt->Length;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtPendingGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the Management Pending Get request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtPendingGetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtPendingGetConfirm_t *evt = &(container->Data.MESHCOP_MgmtPendingGetConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtPendingGetConfirm_FSCI_ID;

	evt->Status = (MESHCOP_MgmtPendingGetConfirm_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Length), pPayload + idx, sizeof(evt->Length)); idx += sizeof(evt->Length);

	if (evt->Length > 0)
	{
		evt->TLVs = MEM_BufferAlloc(evt->Length);

		if (!evt->TLVs)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->TLVs = NULL;
	}

	FLib_MemCpy(evt->TLVs, pPayload + idx, evt->Length); idx += evt->Length;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtCommissionerSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the MESHCOP_MgmtCommissionerSet.Request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtCommissionerSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtCommissionerSetConfirm_t *evt = &(container->Data.MESHCOP_MgmtCommissionerSetConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtCommissionerSetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtCommissionerSetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtActiveSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the MESHCOP_MgmtActiveSet.Request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtActiveSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtActiveSetConfirm_t *evt = &(container->Data.MESHCOP_MgmtActiveSetConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtActiveSetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtActiveSetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtPendingSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the MESHCOP_MgmtPendingSet.Request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtPendingSetConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtPendingSetConfirm_t *evt = &(container->Data.MESHCOP_MgmtPendingSetConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtPendingSetConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtPendingSetConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtSendPanIdQueryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the Pan ID query
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtSendPanIdQueryConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtSendPanIdQueryConfirm_t *evt = &(container->Data.MESHCOP_MgmtSendPanIdQueryConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtSendPanIdQueryConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtSendPanIdQueryConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtPanIdConflictConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Pan ID conflict detected
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtPanIdConflictConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtPanIdConflictConfirm_t *evt = &(container->Data.MESHCOP_MgmtPanIdConflictConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtPanIdConflictConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtPanIdConflictConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtSendEdScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the ED scan
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtSendEdScanConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtSendEdScanConfirm_t *evt = &(container->Data.MESHCOP_MgmtSendEdScanConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtSendEdScanConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtSendEdScanConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtEdReportConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Energy detect report
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtEdReportConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtEdReportConfirm_t *evt = &(container->Data.MESHCOP_MgmtEdReportConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtEdReportConfirm_FSCI_ID;

	evt->Status = (MESHCOP_MgmtEdReportConfirm_Status_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->ScanChannelMask), pPayload + idx, sizeof(evt->ScanChannelMask)); idx += sizeof(evt->ScanChannelMask);
	evt->Length = pPayload[idx]; idx++;

	if (evt->Length > 0)
	{
		evt->EnergyList = MEM_BufferAlloc(evt->Length);

		if (!evt->EnergyList)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->EnergyList = NULL;
	}

	FLib_MemCpy(evt->EnergyList, pPayload + idx, evt->Length); idx += evt->Length;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MESHCOP_MgmtSendAnnounceBeginConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Confirmation for the MESHCOP_MgmtSendAnnounceBegin.Request
***************************************************************************************************/
static memStatus_t Load_MESHCOP_MgmtSendAnnounceBeginConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MESHCOP_MgmtSendAnnounceBeginConfirm_t *evt = &(container->Data.MESHCOP_MgmtSendAnnounceBeginConfirm);

	/* Store (OG, OC) in ID */
	container->id = MESHCOP_MgmtSendAnnounceBeginConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(MESHCOP_MgmtSendAnnounceBeginConfirm_t));

	return MEM_SUCCESS_c;
}

#endif  /* MESHCOP_ENABLE */

#if DTLS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_DTLSOpenConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DTLS-Open.Confirm
***************************************************************************************************/
static memStatus_t Load_DTLSOpenConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DTLSOpenConfirm_t *evt = &(container->Data.DTLSOpenConfirm);

	/* Store (OG, OC) in ID */
	container->id = DTLSOpenConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DTLSOpenConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DTLSCloseConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DTLS-Close.Confirm
***************************************************************************************************/
static memStatus_t Load_DTLSCloseConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DTLSCloseConfirm_t *evt = &(container->Data.DTLSCloseConfirm);

	/* Store (OG, OC) in ID */
	container->id = DTLSCloseConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DTLSCloseConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DTLSClosePeerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DTLS-ClosePeer.Confirm
***************************************************************************************************/
static memStatus_t Load_DTLSClosePeerConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DTLSClosePeerConfirm_t *evt = &(container->Data.DTLSClosePeerConfirm);

	/* Store (OG, OC) in ID */
	container->id = DTLSClosePeerConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DTLSClosePeerConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DTLSConnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DTLS-Connect.Confirm
***************************************************************************************************/
static memStatus_t Load_DTLSConnectConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DTLSConnectConfirm_t *evt = &(container->Data.DTLSConnectConfirm);

	/* Store (OG, OC) in ID */
	container->id = DTLSConnectConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DTLSConnectConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DTLSClientConnectedConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DTLS-ClientConnected.Confirm
***************************************************************************************************/
static memStatus_t Load_DTLSClientConnectedConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DTLSClientConnectedConfirm_t *evt = &(container->Data.DTLSClientConnectedConfirm);

	/* Store (OG, OC) in ID */
	container->id = DTLSClientConnectedConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DTLSClientConnectedConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DTLSSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DTLS-Send.Confirm
***************************************************************************************************/
static memStatus_t Load_DTLSSendConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DTLSSendConfirm_t *evt = &(container->Data.DTLSSendConfirm);

	/* Store (OG, OC) in ID */
	container->id = DTLSSendConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DTLSSendConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DTLSReceiveConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DTLS-Receive.Confirm
***************************************************************************************************/
static memStatus_t Load_DTLSReceiveConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DTLSReceiveConfirm_t *evt = &(container->Data.DTLSReceiveConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = DTLSReceiveConfirm_FSCI_ID;

	evt->PeerIndex = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->Size), pPayload + idx, sizeof(evt->Size)); idx += sizeof(evt->Size);

	if (evt->Size > 0)
	{
		evt->Data = MEM_BufferAlloc(evt->Size);

		if (!evt->Data)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->Data = NULL;
	}

	FLib_MemCpy(evt->Data, pPayload + idx, evt->Size); idx += evt->Size;

	return MEM_SUCCESS_c;
}

#endif  /* DTLS_ENABLE */

#if OTHERUTILS_ENABLE
/*!*************************************************************************************************
\fn		static memStatus_t Load_FSCIGetUniqueIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Response for Unique ID request
***************************************************************************************************/
static memStatus_t Load_FSCIGetUniqueIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	FSCIGetUniqueIdConfirm_t *evt = &(container->Data.FSCIGetUniqueIdConfirm);

	/* Store (OG, OC) in ID */
	container->id = FSCIGetUniqueIdConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(FSCIGetUniqueIdConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_FSCIGetMcuIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Response for MCU ID request
***************************************************************************************************/
static memStatus_t Load_FSCIGetMcuIdConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	FSCIGetMcuIdConfirm_t *evt = &(container->Data.FSCIGetMcuIdConfirm);

	/* Store (OG, OC) in ID */
	container->id = FSCIGetMcuIdConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(FSCIGetMcuIdConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_FSCIGetSwVersionsConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	FSCI-GetSwVersions.Confirm description
***************************************************************************************************/
static memStatus_t Load_FSCIGetSwVersionsConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	FSCIGetSwVersionsConfirm_t *evt = &(container->Data.FSCIGetSwVersionsConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = FSCIGetSwVersionsConfirm_FSCI_ID;

	evt->listSize = pPayload[idx]; idx++;

	if (evt->listSize > 0)
	{
		evt->SwVersions = MEM_BufferAlloc(evt->listSize * 6);

		if (!evt->SwVersions)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->SwVersions = NULL;
	}

	FLib_MemCpy(evt->SwVersions, pPayload + idx, evt->listSize * 6); idx += evt->listSize * 6;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_Sniffer_MacSetPIBAttributeConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Sniffer_MacSetPIBAttribute.Confirm description
***************************************************************************************************/
static memStatus_t Load_Sniffer_MacSetPIBAttributeConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	Sniffer_MacSetPIBAttributeConfirm_t *evt = &(container->Data.Sniffer_MacSetPIBAttributeConfirm);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = Sniffer_MacSetPIBAttributeConfirm_FSCI_ID;

	evt->Status = (Sniffer_MacSetPIBAttributeConfirm_Status_t)pPayload[idx]; idx++;
	evt->PIBAttribute = (Sniffer_MacSetPIBAttributeConfirm_PIBAttribute_t)pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->DataLength), pPayload + idx, sizeof(evt->DataLength)); idx += sizeof(evt->DataLength);

	if (evt->DataLength > 0)
	{
		evt->PIBAttributeValue = MEM_BufferAlloc(evt->DataLength);

		if (!evt->PIBAttributeValue)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->PIBAttributeValue = NULL;
	}

	FLib_MemCpy(evt->PIBAttributeValue, pPayload + idx, evt->DataLength); idx += evt->DataLength;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_MAC_PromiscuousRxIndication(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	MAC_PromiscuousRx.Indication Rx description
***************************************************************************************************/
static memStatus_t Load_MAC_PromiscuousRxIndication(thrEvtContainer_t *container, uint8_t *pPayload)
{
	MAC_PromiscuousRxIndication_t *evt = &(container->Data.MAC_PromiscuousRxIndication);

	uint32_t idx = 0;

	/* Store (OG, OC) in ID */
	container->id = MAC_PromiscuousRxIndication_FSCI_ID;

	evt->LinkQuality = pPayload[idx]; idx++;
	FLib_MemCpy(&(evt->TimeStamp), pPayload + idx, sizeof(evt->TimeStamp)); idx += sizeof(evt->TimeStamp);
	evt->msduLength = pPayload[idx]; idx++;

	if (evt->msduLength > 0)
	{
		evt->msdu = MEM_BufferAlloc(evt->msduLength);

		if (!evt->msdu)
		{
			return MEM_ALLOC_ERROR_c;
		}

	}
	else
	{
		evt->msdu = NULL;
	}

	FLib_MemCpy(evt->msdu, pPayload + idx, evt->msduLength); idx += evt->msduLength;

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SerialTun_LinkIndicationConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Inform the black-box of TUN/TAP link changes
***************************************************************************************************/
static memStatus_t Load_SerialTun_LinkIndicationConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SerialTun_LinkIndicationConfirm_t *evt = &(container->Data.SerialTun_LinkIndicationConfirm);

	/* Store (OG, OC) in ID */
	container->id = SerialTun_LinkIndicationConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SerialTun_LinkIndicationConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_SerialTun_IPPacketReceivedConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	Received an IP packet from the VTUN interface
***************************************************************************************************/
static memStatus_t Load_SerialTun_IPPacketReceivedConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	SerialTun_IPPacketReceivedConfirm_t *evt = &(container->Data.SerialTun_IPPacketReceivedConfirm);

	/* Store (OG, OC) in ID */
	container->id = SerialTun_IPPacketReceivedConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(SerialTun_IPPacketReceivedConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_AspSetPowerLevelConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	AspSetPowerLevel.Confirm description
***************************************************************************************************/
static memStatus_t Load_AspSetPowerLevelConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	AspSetPowerLevelConfirm_t *evt = &(container->Data.AspSetPowerLevelConfirm);

	/* Store (OG, OC) in ID */
	container->id = AspSetPowerLevelConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(AspSetPowerLevelConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_AspGetPowerLevelConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	AspGetPowerLevel.Confirm description
***************************************************************************************************/
static memStatus_t Load_AspGetPowerLevelConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	AspGetPowerLevelConfirm_t *evt = &(container->Data.AspGetPowerLevelConfirm);

	/* Store (OG, OC) in ID */
	container->id = AspGetPowerLevelConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(AspGetPowerLevelConfirm_t));

	return MEM_SUCCESS_c;
}

/*!*************************************************************************************************
\fn		static memStatus_t Load_DBGConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
\brief	DBG messages
***************************************************************************************************/
static memStatus_t Load_DBGConfirm(thrEvtContainer_t *container, uint8_t *pPayload)
{
	DBGConfirm_t *evt = &(container->Data.DBGConfirm);

	/* Store (OG, OC) in ID */
	container->id = DBGConfirm_FSCI_ID;

	FLib_MemCpy(evt, pPayload, sizeof(DBGConfirm_t));

	return MEM_SUCCESS_c;
}

#endif  /* OTHERUTILS_ENABLE */


void KHC_THREAD_RX_MsgHandler(void *pData, void *param, uint8_t fsciInterface)
{
	if (!pData || !param)
	{
		return;
	}

	fsciPacket_t *frame = (fsciPacket_t *)pData;
	thrEvtContainer_t *container = (thrEvtContainer_t *)param;
	uint8_t og = frame->opGroup;
	uint8_t oc = frame->opCode;
	uint8_t *pPayload = frame->data;
	uint16_t id = (og << 8) + oc, i;

	for (i = 0; i < sizeof(evtHandlerTbl) / sizeof(evtHandlerTbl[0]); i++)
	{
		if (evtHandlerTbl[i].id == id)
		{
			evtHandlerTbl[i].handlerFunc(container, pPayload);
			break;
		}
	}
}
