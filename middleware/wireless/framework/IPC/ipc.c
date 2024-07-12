/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*! *********************************************************************************
 * Include
 ********************************************************************************** */
#include "board.h"
#include "fsl_adapter_rpmsg.h"
#include "fsl_os_abstraction.h"
#include "FunctionLib.h"
#include "ipc.h"

/*! *********************************************************************************
 * Private definitions
 ********************************************************************************** */

/*! *********************************************************************************
 * Private memory declarations
 ********************************************************************************** */
OSA_SEMAPHORE_HANDLE_DEFINE(gIpcDataBufferingSem);
ipcTransportInterface_t   mIpcInterface    = NULL;
static hal_rpmsg_handle_t s_ipcRpmsgHandle = NULL;

/*! *********************************************************************************
 * Private functions prototypes
 ********************************************************************************** */

static hal_rpmsg_return_status_t Ipc_RxCallBack(void *param, uint8_t *data, uint32_t len);

/*! *********************************************************************************
 * Public functions
 ********************************************************************************** */
void Ipc_Init(hal_rpmsg_handle_t        ipcRpmsgHandle,
              const hal_rpmsg_config_t *ipcRpmsgConfig,
              ipcTransportInterface_t   interface)
{
    osa_status_t       osa_status;
    hal_rpmsg_status_t ipc_rpmsg_status;

    osa_status = OSA_SemaphoreCreate(gIpcDataBufferingSem, 0);
    assert(osa_status == KOSA_StatusSuccess);
    (void)osa_status;
    mIpcInterface    = NULL;
    s_ipcRpmsgHandle = ipcRpmsgHandle;
    do
    {
        /* Init RPMSG */
        ipc_rpmsg_status = HAL_RpmsgInit((hal_rpmsg_handle_t)ipcRpmsgHandle, (hal_rpmsg_config_t *)ipcRpmsgConfig);
        if (kStatus_HAL_RpmsgSuccess != ipc_rpmsg_status)
        {
            break;
        }
        ipc_rpmsg_status = HAL_RpmsgInstallRxCallback((hal_rpmsg_handle_t)ipcRpmsgHandle, Ipc_RxCallBack, NULL);
        if (kStatus_HAL_RpmsgSuccess != ipc_rpmsg_status)
        {
            break;
        }
        mIpcInterface = interface;
    } while (false);
    if (NULL == mIpcInterface)
    {
#ifndef KW45B41Z83_NBU_SERIES
        assert(mIpcInterface != NULL);
#endif
    }
}

int Ipc_SendPacket(void *pPacket, uint16_t packetSize)
{
    int ret = 0;

    if (HAL_RpmsgSend(s_ipcRpmsgHandle, pPacket, packetSize) != kStatus_HAL_RpmsgSuccess)
    {
        ret = -1;
    }

    return ret;
}

/*! *********************************************************************************
 * Private functions
 ********************************************************************************** */
static hal_rpmsg_return_status_t Ipc_RxCallBack(void *param, uint8_t *data, uint32_t len)
{
    /* Send packet to registered interface */
    mIpcInterface(data, len);

    return kStatus_HAL_RL_RELEASE;
}
