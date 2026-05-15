/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MU_IPC_SHM__HOST_H___
#define __MU_IPC_SHM__HOST_H___

#include "mu_ipc_shm.h"
#include "ipc_shm_host.h"
#include "fsl_mu.h"

extern struct duplex_channel obj_access;
extern struct duplex_channel pdo_comm;
extern struct duplex_channel func_call;
extern struct param_t *g_param;

int MU_ipc_shm_init(uint32_t ipc_shm_base, uint32_t ipc_shm_size);
int CiA402_statemachine_transition(uint8_t axis, uint8_t trans_id);
int CiA402_transition_action_query(uint8_t axis);
int CiA402_object_write(uint16_t index, uint8_t subIndex, void *value, uint8_t len);
int CiA402_object_read(uint16_t index, uint8_t subIndex, void *value, uint8_t len);

#define PDO_write_start()     ipc_shm_host_pdc_send_pre(&pdo_comm)
#define PDO_write_end()       ipc_shm_host_pdc_send(&pdo_comm)
#define PDO_read_start()      ipc_shm_host_pdc_recv(&pdo_comm)
#define PDO_read_end()        ipc_shm_host_pdc_recv_post(&pdo_comm)

//#define MU_send_ipc_shm_base(base) 	MU_SendMsgNonBlocking(IPC_SHM_MU, MU_CHN_IPC_SHM_BASE_REG_INDEX, base)
//
//#define PDO_TRIGGER()					MU_TriggerGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX) 
//#define CALLBACK_TRIGGER()				MU_TriggerGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_FUNC_CALL_INDEX) 
//#define OBJ_ACCESS_TRIGGER()			MU_TriggerGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_OBJ_ACCESS_INDEX) 
//
//#define GET_PDO_RECEIVE_FLAG()		(MU_GetGeneralPurposeStatusFlags(IPC_SHM_MU) & MU_INT_PDO_COMM_GENERAL_INDEX) 
//#define CLR_PDO_RECEIVE_FLAG()		MU_ClearGeneralPurposeStatusFlags(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX)
//
//
//#define GET_CALLBACK_FLAG()			(MU_GetGeneralPurposeStatusFlags(IPC_SHM_MU) & MU_INT_FUNC_CALL_GENERAL_INDEX) 
//#define CLR_CALLBACK_FLAG()			MU_ClearGeneralPurposeStatusFlags(IPC_SHM_MU, MU_INT_FUNC_CALL_GENERAL_INDEX)
//
//#define GET_OBJ_ACCESS_FLAG()		(MU_GetGeneralPurposeStatusFlags(IPC_SHM_MU) & MU_INT_OBJ_ACCESS_GENERAL_INDEX) 
//#define CLR_OBJ_ACCESS_FLAG()		MU_ClearGeneralPurposeStatusFlags(IPC_SHM_MU, MU_INT_OBJ_ACCESS_GENERAL_INDEX)
//
//#define IPC_SHM_BASE_RECV_INT_DISABLE() 	MU_DisableInterrupts(IPC_SHM_MU, MU_CHN_IPC_SHM_BASE_RX_FULL_INT_EN)
//#define IPC_SHM_BASE_RECV_INT_ENABLE() 		MU_EnableInterrupts(IPC_SHM_MU, MU_CHN_IPC_SHM_BASE_RX_FULL_INT_EN)
//
//#define PDO_RECEIVE_INT_DISABLE() 	MU_DisableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX)
//#define PDO_RECEIVE_INT_ENABLE() 	MU_EnableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX)
//
//#define CALLBACK_INT_ENABLE()		MU_EnableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_FUNC_CALL_GENERAL_INDEX)
//#define CALLBACK_INT_DISABLE()   	MU_DisableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_FUNC_CALL_GENERAL_INDEX)
//
//#define OBJ_ACCESS_INT_ENABLE() 	MU_EnableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_OBJ_ACCESS_GENERAL_INDEX)
//#define OBJ_ACCESS_INT_DISABLE()    MU_DisableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_OBJ_ACCESS_GENERAL_INDEX)
//
//#define GET_IPC_SHM_MEM_BASE_STATUS() (MU_GetStatusFlags(IPC_SHM_MU) & MU_CHN_IPC_SHM_BASE_RECE_FULL) 

#define CiA402_transition1(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_1)
#define CiA402_transition2(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_2)
#define CiA402_transition3(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_3)
#define CiA402_transition4(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_4)
#define CiA402_transition5(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_5)
#define CiA402_transition6(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_6)
#define CiA402_transition7(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_7)
#define CiA402_transition8(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_8)
#define CiA402_transition9(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_9)
#define CiA402_transition10(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_10)
#define CiA402_transition11(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_11)
#define CiA402_transition12(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_12)
#define CiA402_transition13(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_13)
#define CiA402_transition14(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_14)
#define CiA402_transition15(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_15)
#define CiA402_transition16(axis)  CiA402_statemachine_transition(axis, FUNC_TRAN_16)

#endif
