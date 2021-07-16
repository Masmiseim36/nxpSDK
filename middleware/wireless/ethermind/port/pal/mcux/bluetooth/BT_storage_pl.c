
/**
 *  \file BT_storage_pl.c
 *
 *  This file contains the implementation for all platform specific
 *  extensions for persistant storage module.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "BT_storage.h"
#include "BT_storage_pl.h"
#include "NVM_Interface.h"
#include "nvm_adapter.h"

#ifdef BT_STORAGE

#define CONFIG_NVM_SIZE (2U * 1024U)

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

#if (STORAGE_SKEY_SIZE != 0)
/* Storage Signature Key array */
DECL_STATIC UCHAR ssign[STORAGE_NUM_TYPES][STORAGE_SKEY_SIZE] =
	{ {'E', 'T', 'H', 'E', 'R', 'M', 'I', 'N', 'D', 'P', 'S'} };
#endif /* (STORAGE_SKEY_SIZE != 0) */

static int BT_StorageNvmBackendInit(void);

#if defined(__IAR_SYSTEMS_ICC__)
_Pragma("location=\"NVM_TABLE\"") __root \
    NVM_DataEntry_t NvDataTable;
NVM_ADAPTER_TABLE(nvm_adapter_table_t BtNvAdapterDataTable[]) =
{
    {
        &NvDataTable,
        1,
        BT_StorageNvmBackendInit
    }
};
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
NVM_DataEntry_t NvDataTable __attribute__((section ("NVM_TABLE"), used));
static NVM_ADAPTER_TABLE(nvm_adapter_table_t NvAdapterDataTable[]) =
{
    {
        &NvDataTable,
        1U,
        BT_StorageNvmBackendInit
    }
};
#elif defined(__GNUC__)
NVM_DataEntry_t NvDataTable __attribute__((section (".NVM_TABLE"), used));
static NVM_ADAPTER_TABLE(nvm_adapter_table_t NvAdapterDataTable[]) =
{
    {
        &NvDataTable,
        1U,
        BT_StorageNvmBackendInit
    }
};
#else
#error "Must define NvDataTable"
#endif


DECL_STATIC UCHAR NvmSaveBuf[CONFIG_NVM_SIZE];
DECL_STATIC UINT16 nv_offset;

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
static OSA_TASK_HANDLE_DEFINE(s_nvmIdleTask);
static osa_task_handle_t s_nvmIdleTaskHandle;
static void storage_idle_task(osa_task_param_t arg);
static OSA_TASK_DEFINE(storage_idle_task, STORAGE_IDLE_TASK_PRIORITY, 1U, STORAGE_IDLE_TASK_STACK_SIZE, 0U);
#endif
/* --------------------------------------------- Functions */

static int BT_StorageNvmBackendInit(void)
{
    NvDataTable.pData = NvmSaveBuf;
    NvDataTable.ElementSize = CONFIG_NVM_SIZE;
    NvDataTable.ElementsCount = 1U;
    NvDataTable.DataEntryID = 0xf101U;
    NvDataTable.DataEntryType = gNVM_MirroredInRam_c;
    return 0U;
}

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
static void storage_idle_task(osa_task_param_t arg)
{
    BT_LOOP_FOREVER()
    {
        NvIdle();
    }
}
#endif

void storage_bt_init_pl (void)
{
    (BT_IGNORE_RETURN_VALUE) NVM_AdapterInit();
#if defined(__IAR_SYSTEMS_ICC__)
    (void)BtNvAdapterDataTable[0U];
#elif (defined(__CC_ARM) || defined(__ARMCC_VERSION))
    (void)NvAdapterDataTable[0U];
#elif defined(__GNUC__)
    (void)NvAdapterDataTable[0U];
#endif
#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
    if (s_nvmIdleTaskHandle == NULL)
    {
        if (KOSA_StatusSuccess == OSA_TaskCreate((osa_task_handle_t)s_nvmIdleTask, OSA_TASK(storage_idle_task), NULL))
        {
            s_nvmIdleTaskHandle = (osa_task_handle_t)s_nvmIdleTask;
        }
        else
        {
            printf("fail to create NVM idle sync task\n");
        }
    }
#endif
}

void storage_bt_shutdown_pl (void)
{
}

API_RESULT storage_open_pl (UCHAR type, UCHAR mode)
{
	/* Initialize the offset */
	nv_offset = 0U;

	/* If mode is read, load from the NVRAM */
	if (STORAGE_OPEN_MODE_READ == mode)
	{
        (BT_IGNORE_RETURN_VALUE) NvRestoreDataSet(NvmSaveBuf, false);
	}

	return API_SUCCESS;
}

API_RESULT storage_close_pl (UCHAR type, UCHAR mode)
{
	/* If mode is write, save to the NVRAM */
	if (STORAGE_OPEN_MODE_WRITE == mode)
	{
#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
        (BT_IGNORE_RETURN_VALUE) NvSaveOnIdle(NvmSaveBuf,  false);
#else
		NvSyncSave(NvmSaveBuf,  false);
#endif

		/*
		 *  TODO: Check! For the first time restore on power
		 *  cycle worked after having this here.
		 */
		/* NvRestoreDataSet(NvmSaveBuf, false); */
	}

	return API_SUCCESS;
}

INT16 storage_write_pl (UCHAR type, void * buffer, UINT16 size)
{
    BT_mem_copy((NvmSaveBuf + nv_offset), buffer, size);
    nv_offset += size;

    return size;
}

INT16 storage_read_pl (UCHAR type, void * buffer, UINT16 size)
{
    BT_mem_copy(buffer, (NvmSaveBuf + nv_offset), size);
    nv_offset += size;

    return size;
}

INT16 storage_write_signature_pl (UCHAR type)
{
#if (STORAGE_SKEY_SIZE != 0)
    if (STORAGE_NUM_TYPES <= type)
    {
        return -1;
    }

    BT_mem_copy(NvmSaveBuf, ssign[type], STORAGE_SKEY_SIZE);
    nv_offset += STORAGE_SKEY_SIZE;

    return STORAGE_SKEY_SIZE;
#else /* (STORAGE_SKEY_SIZE != 0) */
    BT_IGNORE_UNUSED_PARAM(type);

    return 0U;
#endif /* (STORAGE_SKEY_SIZE != 0) */
}

INT16 storage_read_signature_pl (UCHAR type)
{
#if (STORAGE_SKEY_SIZE != 0)
	INT16 ret;

    if (STORAGE_NUM_TYPES <= type)
    {
        return -1;
    }

	ret = STORAGE_SKEY_SIZE;
	if (0U != BT_mem_cmp(ssign[type], NvmSaveBuf, STORAGE_SKEY_SIZE))
	{
		ret = -1;
	}

	nv_offset += STORAGE_SKEY_SIZE;

    return ret;
#else /* (STORAGE_SKEY_SIZE != 0) */
    BT_IGNORE_UNUSED_PARAM(type);

    return 0U;
#endif /* (STORAGE_SKEY_SIZE != 0) */
}

#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
API_RESULT storage_check_consistency_pl
           (
               /* IN */ UCHAR type
           )
{
#ifdef BT_DUAL_MODE
    UINT32 sm_dq_entries;
    UINT32 smp_dq_entries;

    /**
     * Check device queue has some stray elements (due to inconsistent power cycle of the system etc.),
     * which are not present in the security databases.
     */

    /* Get the device queue valid entries bit fields from SM */
    sm_get_valid_dq_entries_pl(&sm_dq_entries);

    /* Get the device queue valid entries bit fields from SMP */
    smp_get_valid_dq_entries_pl(&smp_dq_entries);

    /* TBD: Check there is no overlap */

    /* Pass on the OR-ed bit fields to device queue for cleanup */
    device_queue_cleanup_pl(sm_dq_entries | smp_dq_entries);

#else
#endif /* BT_DUAL_MODE */

    return API_SUCCESS;
}
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

#endif /* BT_STORAGE */

