/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "fsl_debug_console.h"
#include "fsl_mu.h"
#include "board.h"
#include "app.h"
#include "mu_ipc_shm_client.h"
#include "ipc_shm_client.h"

__WEAK int obj_write_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData);
__WEAK int obj_read_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData);
__WEAK int Cia402_status_machine_trans(uint8_t axis, uint8_t trans_id, struct param_t *g_param);
__WEAK int Cia402_tran_action_quary(uint8_t axis);
__WEAK int motor_slow_task(int axis, struct pdo_to_motor_t *pdo_m, struct pdo_to_esc_t *pdo_e, struct param_t *param);

struct duplex_channel obj_access;
struct duplex_channel pdo_comm;
struct duplex_channel func_call;
struct param_t *g_param;

struct ipc_shm_t *ipc_shm;

void MU_ipc_shm_client_init()
{
	while (!GET_IPC_SHM_MEM_BASE_STATUS());
	ipc_shm = (struct ipc_shm_t *)MU_ReceiveMsgNonBlocking(IPC_SHM_MU, MU_CHN_IPC_SHM_BASE_REG_INDEX);
	ipc_shm_client_sdc_find(&obj_access, "obj_access");
	ipc_shm_client_pdc_find(&pdo_comm, "pdo_comm");
	ipc_shm_client_sdc_find(&func_call, "func_call");
	g_param = ipc_shm_param_find("param"); 
	PDO_RECEIVE_INT_ENABLE();
	CALLBACK_INT_ENABLE();
	OBJ_ACCESS_INT_ENABLE();
	EnableIRQ(IPC_SHM_MU_IRQn);
}

void IPC_SHM_MU_IRQHandler(void)
{
	uint32_t flag = 0;
	flag = MU_GetStatusFlags(IPC_SHM_MU);
	/* PDO input */
	if ((flag & MU_INT_PDO_COMM_FLAG) == MU_INT_PDO_COMM_FLAG)
	{
		process_handler();
		CLR_PDO_RECEIVE_FLAG();
	}

	/*IPC_SHM call back request has been received on motor control side */
	if ((flag & MU_INT_FUNC_CALL_FLAG) == MU_INT_FUNC_CALL_FLAG )
	{
		func_call_handler();
		CLR_CALLBACK_FLAG();
	}

	/*IPC_SHM objiect access request has been received by motor control side */
	if ((flag & MU_INT_OBJ_ACCESS_FLAG) == MU_INT_OBJ_ACCESS_FLAG)
	{
 		obj_handler();
		CLR_OBJ_ACCESS_FLAG();
	}

	SDK_ISR_EXIT_BARRIER;
}

int obj_write_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
	return 0;
}

int obj_read_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
	return 0;
}

void obj_handler()
{
	struct obj_access_t obj;
	int len, l, ret;
	len = ipc_shm_client_sdc_read(&obj_access, &obj, sizeof(obj));
	if ( len >= 5 ) {
		if (obj.opt == OBJ_READ_OPT) {
			l = obj_read_callback(obj.index, obj.subIndex, 16, obj.argv);
			if (l < 0) {
				obj.opt = OBJ_READ_ERR;
				obj.len = 0;
			} else {
				obj.opt = OBJ_READ_RES;
				obj.len = l;
			}
		} else if (obj.opt == OBJ_WRITE_OPT && len >= obj.len + 5) {
			ret = obj_write_callback(obj.index, obj.subIndex, obj.len, obj.argv);
			obj.len = 0;
			if (ret < 0) {
				obj.opt = OBJ_WRITE_ERR;
			} else {
				obj.opt = OBJ_WRITE_RES;
			}
		} else {
			obj.opt = OBJ_ACCESS_ERR;
			obj.len = 0;
		}
	} else {
		obj.opt = OBJ_ACCESS_ERR;
		obj.len = 0;
	}
	ipc_shm_client_sdc_write(&obj_access, &obj, obj.len + 5);

}

int motor_slow_task(int axis, struct pdo_to_motor_t *pdo_m, struct pdo_to_esc_t *pdo_e, struct param_t *param)
{
	return 0;
}

void process_handler()
{
	int i;
	struct pdo_to_motor_t *pdo_m = PDO_read_start();
	struct pdo_to_esc_t *pdo_e = PDO_write_start();
	for (i = 0; i < AXIS_MAX; i++) {
		motor_slow_task(i, &pdo_m[i], &pdo_e[i], g_param);
	}
	PDO_read_end();
	PDO_write_end();
	MU_TriggerInterrupts(IPC_SHM_MU, MU_INT_PDO_COMM_INDEX);
}

int Cia402_status_machine_trans(uint8_t axis, uint8_t trans_id, struct param_t *g_param)
{
	return 0;
}

int Cia402_tran_action_quary(uint8_t axis)
{
	return 0;
}

int func_call_handler()
{
	struct func_call_t call;
	int len, ret = 0;
	len = ipc_shm_client_sdc_read(&func_call, &call, sizeof(call));
	if (len >=2 && len >= call.argc + 1) {
		if (call.func_id == FUNC_STATE_MACHINE_TRAN) {
			ret = Cia402_status_machine_trans(call.argv[0], call.argv[1], g_param);
		} else if (call.func_id == FUNC_TRAN_ACTION_QUARY) {
			ret = Cia402_tran_action_quary(call.argv[0]);
		} else {
			ret = -1;
		}
	} else {
		ret = -1;
	}
	ipc_shm_client_sdc_write(&func_call, &ret, sizeof(ret));
	return ret;
}
