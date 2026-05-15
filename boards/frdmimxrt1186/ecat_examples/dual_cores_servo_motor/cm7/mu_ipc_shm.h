/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MU_IPC_SHM_H_
#define _MU_IPC_SHM_H_

#include "cc.h"
#include "app.h"
#include "ipc_shm.h"
#define AXIS_MAX   2

/* Channel for IPC_SHM base address transaction */
#define MU_CHN_IPC_SHM_BASE_REG_INDEX           kMU_MsgReg0
#define MU_CHN_IPC_SHM_BASE_SEND_EMPTY          kMU_Tx0EmptyFlag
#define MU_CHN_IPC_SHM_BASE_RECE_FULL           kMU_Rx0FullFlag
#define MU_CHN_IPC_SHM_BASE_TX_EMPTY_INT_EN     kMU_Tx0EmptyInterruptEnable
#define MU_CHN_IPC_SHM_BASE_RX_FULL_INT_EN      kMU_Rx0FullInterruptEnable

/* Interrupter for funcation callback */
#define MU_INT_FUNC_CALL_INDEX               kMU_GenInt1InterruptTrigger
#define MU_INT_FUNC_CALL_FLAG                kMU_GenInt1Flag
#define MU_INT_FUNC_CALL_EN                  kMU_GenInt1InterruptEnable
#define MU_INT_FUNC_CALL_GENERAL_INDEX       kMU_GeneralPurposeInterrupt1

/* Interrupter for Obj update */
#define MU_INT_OBJ_ACCESS_INDEX             kMU_GenInt2InterruptTrigger
#define MU_INT_OBJ_ACCESS_FLAG              kMU_GenInt2Flag
#define MU_INT_OBJ_ACCESS_EN                kMU_GenInt2InterruptEnable
#define MU_INT_OBJ_ACCESS_GENERAL_INDEX     kMU_GeneralPurposeInterrupt2

/* Interrupter for PDO communication */
#define MU_INT_PDO_COMM_INDEX               kMU_GenInt0InterruptTrigger
#define MU_INT_PDO_COMM_FLAG                kMU_GenInt0Flag
#define MU_INT_PDO_COMM_EN                  kMU_GenInt0InterruptEnable
#define MU_INT_PDO_COMM_GENERAL_INDEX       kMU_GeneralPurposeInterrupt0

#define MU_send_ipc_shm_base(base)          MU_SendMsgNonBlocking(IPC_SHM_MU, MU_CHN_IPC_SHM_BASE_REG_INDEX, base)

#define PDO_TRIGGER()                       MU_TriggerGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX)
#define CALLBACK_TRIGGER()                  MU_TriggerGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_FUNC_CALL_GENERAL_INDEX)
#define OBJ_ACCESS_TRIGGER()                MU_TriggerGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_OBJ_ACCESS_GENERAL_INDEX)

#define GET_PDO_RECEIVE_FLAG()              (MU_GetGeneralPurposeStatusFlags(IPC_SHM_MU) & MU_INT_PDO_COMM_GENERAL_INDEX)
#define CLR_PDO_RECEIVE_FLAG()              MU_ClearGeneralPurposeStatusFlags(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX)


#define GET_CALLBACK_FLAG()             (MU_GetGeneralPurposeStatusFlags(IPC_SHM_MU) & MU_INT_FUNC_CALL_GENERAL_INDEX)
#define CLR_CALLBACK_FLAG()             MU_ClearGeneralPurposeStatusFlags(IPC_SHM_MU, MU_INT_FUNC_CALL_GENERAL_INDEX)

#define GET_OBJ_ACCESS_FLAG()           (MU_GetGeneralPurposeStatusFlags(IPC_SHM_MU) & MU_INT_OBJ_ACCESS_GENERAL_INDEX)
#define CLR_OBJ_ACCESS_FLAG()           MU_ClearGeneralPurposeStatusFlags(IPC_SHM_MU, MU_INT_OBJ_ACCESS_GENERAL_INDEX)

#define PDO_RECEIVE_INT_ENABLE()        MU_EnableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX)

#define CALLBACK_INT_ENABLE()           MU_EnableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_FUNC_CALL_GENERAL_INDEX)
#define CALLBACK_INT_DISABLE()          MU_DisableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_FUNC_CALL_GENERAL_INDEX)

#define OBJ_ACCESS_INT_ENABLE()         MU_EnableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_OBJ_ACCESS_GENERAL_INDEX)
#define OBJ_ACCESS_INT_DISABLE()        MU_DisableGeneralPurposeInterrupts(IPC_SHM_MU, MU_INT_OBJ_ACCESS_GENERAL_INDEX)

#define GET_IPC_SHM_MEM_BASE_STATUS()   (MU_GetStatusFlags(IPC_SHM_MU) & MU_CHN_IPC_SHM_BASE_RECE_FULL)

#define IPC_SHM_BASE_RECV_INT_DISABLE() 	MU_DisableInterrupts(IPC_SHM_MU, MU_CHN_IPC_SHM_BASE_RX_FULL_INT_EN)
#define IPC_SHM_BASE_RECV_INT_ENABLE() 		MU_EnableInterrupts(IPC_SHM_MU, MU_CHN_IPC_SHM_BASE_RX_FULL_INT_EN)

#define FUNC_TRAN_1    0x01
#define FUNC_TRAN_2    0x02
#define FUNC_TRAN_3    0x03
#define FUNC_TRAN_4    0x04
#define FUNC_TRAN_5    0x05
#define FUNC_TRAN_6    0x06
#define FUNC_TRAN_7    0x07
#define FUNC_TRAN_8    0x08
#define FUNC_TRAN_9    0x09
#define FUNC_TRAN_10    0x0A
#define FUNC_TRAN_11    0x0B
#define FUNC_TRAN_12    0x0C
#define FUNC_TRAN_13    0x0D
#define FUNC_TRAN_14    0x0E
#define FUNC_TRAN_15    0x0F
#define FUNC_TRAN_16    0x10

#define FUNC_STATE_MACHINE_TRAN 0x00
#define FUNC_TRAN_ACTION_QUARY  0x1

#define OBJ_WRITE_OPT     0x01
#define OBJ_WRITE_RES     0x81
#define OBJ_WRITE_ERR     0xF1
#define OBJ_READ_OPT      0x02
#define OBJ_READ_RES      0x82
#define OBJ_READ_ERR      0xF2
#define OBJ_ACCESS_ERR    0xF0 

STRUCT_PACKED
struct func_call_t {
	uint8_t func_id;
	uint8_t argc;
	uint8_t argv[16]; 
} STRUCT_UNPACKED;

STRUCT_PACKED
struct func_resp_t {
	int32_t ret;
} STRUCT_UNPACKED;

STRUCT_PACKED
struct obj_access_t {
	uint16_t index;
	uint8_t subIndex;
	uint8_t len;
	uint8_t opt;
	uint8_t argv[16]; 
} STRUCT_UNPACKED;

STRUCT_PACKED
struct pdo_to_motor_t {
	uint16_t Control;
	int32_t Target_pos;
	int32_t Target_vel;
} STRUCT_UNPACKED;

STRUCT_PACKED
struct pdo_to_esc_t {
	uint16_t Status;
	int32_t Actual_pos;
	int32_t Actual_vel;
} STRUCT_UNPACKED;

STRUCT_PACKED
struct axis_param_t {
	uint16_t pending_option_code;
	uint8_t axis_is_active;
	uint8_t brake_applied;
	uint8_t lowlevel_power_applied;
	uint8_t highlevel_power_applied;
	uint8_t axis_function_enabled;
	uint8_t configuration_allowed;
} STRUCT_UNPACKED;

STRUCT_PACKED
struct param_t{
	uint32_t u32CycleTime;
	struct axis_param_t axis[AXIS_MAX];
} STRUCT_UNPACKED;

#endif
