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
#include "mu_ipc_shm_host.h"
#include "ecatappl.h"

__WEAK void PDO_InputMapping_CallBack();

void IPC_SHM_MU_IRQHandler(void)
{
	uint32_t flag = 0;
	flag = MU_GetStatusFlags(IPC_SHM_MU);
	/* PDO to ESC */
	if ((flag & MU_INT_PDO_COMM_FLAG) == MU_INT_PDO_COMM_FLAG)
	{
		PDO_InputMapping_CallBack();
		MU_ClearGeneralPurposeStatusFlags(IPC_SHM_MU, MU_INT_PDO_COMM_GENERAL_INDEX);
	}
	SDK_ISR_EXIT_BARRIER;
}

struct duplex_channel obj_access;
struct duplex_channel pdo_comm;
struct duplex_channel func_call;
struct param_t *g_param;

int MU_ipc_shm_init(uint32_t ipc_shm_base, uint32_t ipc_shm_size)
{
	int ret = 0;
	if ((ret = ipc_shm_host_init(ipc_shm_base, ipc_shm_size)) < 0) {
		return ret;
	}

	if ((ret = ipc_shm_sdc_create("func_call", sizeof(struct func_call_t), sizeof(struct func_resp_t), &func_call)) < 0) {
		return ret;
	}
	
	if ((ret = ipc_shm_sdc_create("obj_access", sizeof(struct obj_access_t), sizeof(struct obj_access_t), &obj_access)) < 0) {
		return ret;
	}

	if ((ret = ipc_shm_pdc_create("pdo_comm", sizeof(struct pdo_to_motor_t) * AXIS_MAX, sizeof(struct pdo_to_esc_t) * AXIS_MAX, &pdo_comm)) < 0) {
		return ret;
	}
	
	if ((ret = ipc_shm_param_create("param", sizeof(struct param_t))) < 0) {
		return ret;
	}
	g_param =  ipc_shm_get_param_pointer(ret);
	MU_send_ipc_shm_base(ipc_shm_base);
	PDO_RECEIVE_INT_ENABLE();
	return ret;
}

int CiA402_statemachine_transition(uint8_t axis, uint8_t trans_id)
{
	struct func_call_t call;
	struct func_resp_t resp;
	int len;
	call.func_id = FUNC_STATE_MACHINE_TRAN;
	call.argc = 2;
	call.argv[0] = axis;
	call.argv[1] = trans_id;
	ipc_shm_host_sdc_send(&func_call, &call, 4);
	CALLBACK_TRIGGER();

	while ((len = ipc_shm_host_sdc_recv(&func_call, &resp, sizeof(struct func_resp_t))) == 0);
	return len < 0 ? -1 : resp.ret;
}

int CiA402_transition_action_query(uint8_t axis)
{
	struct func_call_t call;
	struct func_resp_t resp;
	int len;
	call.func_id = FUNC_TRAN_ACTION_QUARY;
	call.argc = 1;
	call.argv[0] = axis;
	ipc_shm_host_sdc_send(&func_call, &call, 3);
	CALLBACK_TRIGGER();

	while ((len = ipc_shm_host_sdc_recv(&func_call, &resp, sizeof(struct func_resp_t))) != 0);
	return len < 0 ? -1 : resp.ret;

}

int CiA402_object_write(uint16_t index, uint8_t subIndex, void *value, uint8_t len)
{
	struct obj_access_t obj;
	int ret, l;
	obj.index = index;
	obj.subIndex = subIndex;
	obj.opt = OBJ_WRITE_OPT;
	memcpy(obj.argv, value, len);
	obj.len = len;
	ipc_shm_host_sdc_send(&obj_access, &obj, len + 5);
	OBJ_ACCESS_TRIGGER();

	while ((l = ipc_shm_host_sdc_recv(&obj_access,  &obj, sizeof(struct obj_access_t))) == 0);
	if (l < 5) {
		return -1;
	}

	if (obj.opt == OBJ_WRITE_ERR) {
		ret = -1;
	} else {
		ret = 0;
	}

	return  ret;
}

int CiA402_object_read(uint16_t index, uint8_t subIndex, void *value, uint8_t len)
{
	struct obj_access_t obj;
	int l;
	obj.index = index;
	obj.subIndex = subIndex;
	obj.opt = OBJ_READ_OPT;
	obj.len = len;
	ipc_shm_host_sdc_send(&obj_access, &obj, 5);
	OBJ_ACCESS_TRIGGER();

	while ((l = ipc_shm_host_sdc_recv(&obj_access, &obj, sizeof(struct obj_access_t))) == 0);
	if (l < 5) {
		return -1;
	}
	memcpy(value, obj.argv, obj.len);
	return obj.len;
}

