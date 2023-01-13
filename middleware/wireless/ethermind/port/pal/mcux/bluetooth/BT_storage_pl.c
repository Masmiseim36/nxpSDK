
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

#include "littlefs_pl.h"

#ifdef BT_STORAGE

#define CONFIG_NVM_SIZE (6U * 1024U)

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */
#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
DECL_STATIC lfs_t * lfs;

/* Storage File Handle array */
DECL_STATIC lfs_file_t * fp[STORAGE_NUM_TYPES];
DECL_STATIC lfs_file_t lfs_file[STORAGE_NUM_TYPES];
#endif /* CONFIG_BT_SETTINGS */

/* Storage File Name array */
#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
DECL_STATIC UCHAR * fn[STORAGE_NUM_TYPES] =
{
    (UCHAR *)"btps.db",
#ifdef STORAGE_RETENTION_SUPPORT
    (UCHAR *)"btrn.db",
#endif /* STORAGE_RETENTION_SUPPORT */
};
#endif

#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
DECL_STATIC lfs_t * lfs;
#endif /* CONFIG_BT_SETTINGS */

/* if CONFIG_BT_SETTINGS is not enable, disable STORAGE_IDLE_TASK_SYNC_ENABLE */
#if !((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
#if defined STORAGE_IDLE_TASK_SYNC_ENABLE
#undef STORAGE_IDLE_TASK_SYNC_ENABLE
#define STORAGE_IDLE_TASK_SYNC_ENABLE (0)
#endif
#endif

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
static OSA_TASK_HANDLE_DEFINE(s_nvmIdleTask);
static osa_task_handle_t s_nvmIdleTaskHandle;
static void storage_idle_task(osa_task_param_t arg);
static OSA_TASK_DEFINE(storage_idle_task, STORAGE_IDLE_TASK_PRIORITY, 1, STORAGE_IDLE_TASK_STACK_SIZE, 0);

static bool s_nvLoadData;

static osa_semaphore_handle_t g_nvWriteBack;
static OSA_SEMAPHORE_HANDLE_DEFINE(g_nvWriteBackHandle);
static volatile bool g_nvWriteBackState[STORAGE_NUM_TYPES];
#endif

#if ((((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS)) &&\
      ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))) ||\
     (!((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))))
/* Case1:when CONFIG_BT_SETTINGS is not enable, NvmSaveBuf and ssign are used to save data from mindtree stack
   Case2: when both CONFIG_BT_SETTINGS and STORAGE_IDLE_TASK_SYNC_ENABLE are enable, NvmSaveBuf and ssign are used to cache data from mindtree stack.
*/
DECL_STATIC UCHAR NvmSaveBuf[CONFIG_NVM_SIZE];
DECL_STATIC UINT16 nv_offset;
#if (STORAGE_SKEY_SIZE != 0)
/* Storage Signature Key array */
DECL_STATIC UCHAR ssign[STORAGE_NUM_TYPES][STORAGE_SKEY_SIZE] =
	{ {'E', 'T', 'H', 'E', 'R', 'M', 'I', 'N', 'D', 'P', 'S'} };
#endif /* (STORAGE_SKEY_SIZE != 0) */
#endif
/* --------------------------------------------- Functions */

#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
static void storage_idle_task(osa_task_param_t arg)
{
#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
    int err;
#endif
    osa_status_t ret;
    while (1)
    {
        ret = OSA_SemaphoreWait(g_nvWriteBack, osaWaitForever_c);
        if (KOSA_StatusSuccess == ret)
        {
            for (int i= 0;i < STORAGE_NUM_TYPES;i++)
            {
                if (true == g_nvWriteBackState[i])
                {
                    g_nvWriteBackState[i] = false;

#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
                    err = lfs_file_open (lfs, &lfs_file[i], (CHAR *)fn[i], LFS_O_WRONLY | LFS_O_CREAT);
                    if (err >= 0)
                    {
                        err = lfs_file_write (lfs, &lfs_file[i], NvmSaveBuf, CONFIG_NVM_SIZE);
                        assert(err >= 0);
                        (void)lfs_file_close(lfs, &lfs_file[i]);
                    }
#endif /* CONFIG_BT_SETTINGS */
                }
            }
        }
    }
}
#endif /* STORAGE_IDLE_TASK_SYNC_ENABLE */
#endif /* CONFIG_BT_SETTINGS */

void storage_bt_init_pl (void)
{
#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
    UCHAR i;
#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
    osa_status_t ret;
#endif

#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
    for (i = 0; i < STORAGE_NUM_TYPES; i++)
    {
        fp[i] = NULL;
    }

    lfs = lfs_pl_init();

    assert(NULL != lfs);
#endif /* CONFIG_BT_SETTINGS */

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
    if (NULL == g_nvWriteBack)
    {
        ret = OSA_SemaphoreCreate((osa_semaphore_handle_t)g_nvWriteBackHandle, 0);
        if (KOSA_StatusSuccess == ret)
        {
            g_nvWriteBack = (osa_semaphore_handle_t)g_nvWriteBackHandle;
        }
        assert(KOSA_StatusSuccess == ret);
    }
    if (NULL == s_nvmIdleTaskHandle)
    {
        ret = OSA_TaskCreate((osa_task_handle_t)s_nvmIdleTask, OSA_TASK(storage_idle_task), NULL);
        if (KOSA_StatusSuccess == ret)
        {
            s_nvmIdleTaskHandle = (osa_task_handle_t)s_nvmIdleTask;
        }
        assert(KOSA_StatusSuccess == ret);
    }
    s_nvLoadData = true;
#endif

#endif /* CONFIG_BT_SETTINGS */
}

void storage_bt_shutdown_pl (void)
{
#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
    UCHAR i;

    for (i = 0; i < STORAGE_NUM_TYPES; i++)
    {
        if (NULL != fp[i])
        {
            lfs_file_close (lfs, fp[i]);
            fp[i] = NULL;
        }
    }
#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
    s_nvLoadData = false;
#endif
#endif /* CONFIG_BT_SETTINGS */
}

API_RESULT storage_open_pl (UCHAR type, UCHAR mode)
{
#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
    int err;

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
    nv_offset = 0;
    if (true == s_nvLoadData)
    {
        s_nvLoadData = false;
        err = lfs_file_open (lfs, &lfs_file[type], (CHAR *)fn[type], LFS_O_RDONLY);
        if (err < 0)
        {
            return API_FAILURE;
        }
        err = lfs_file_read (lfs, &lfs_file[type], NvmSaveBuf, CONFIG_NVM_SIZE);

        (void)lfs_file_close(lfs, &lfs_file[type]);

        if (err < 0)
        {
            return API_FAILURE;
        }
    }
#else
    int rw;

    if (NULL != fp[type])
    {
        (void)lfs_file_close(lfs, fp[type]);
        fp[type] = NULL;
    }
    /* Set the file access mode */
    rw = (int)((STORAGE_OPEN_MODE_WRITE == mode)? (LFS_O_WRONLY | LFS_O_CREAT): LFS_O_RDONLY);

    err = lfs_file_open (lfs, &lfs_file[type], (CHAR *)fn[type], rw);

    if (err < 0)
    {
        return API_FAILURE;
    }

    fp[type] = &lfs_file[type];

#endif
#else
    nv_offset = 0;
#endif /* CONFIG_BT_SETTINGS */

    return API_SUCCESS;
}

API_RESULT storage_close_pl (UCHAR type, UCHAR mode)
{
    BT_IGNORE_UNUSED_PARAM(mode);
#if ((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS))
#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE))
    /* Notify idle task to write back */
    if (STORAGE_OPEN_MODE_WRITE == mode)
    {
        osa_status_t ret;
        g_nvWriteBackState[type] = true;

        ret = OSA_SemaphorePost(g_nvWriteBack);
        assert(KOSA_StatusSuccess == ret);
        (void)ret;
    }
#else
    if (NULL != fp[type])
    {
        (void)lfs_file_close(lfs, fp[type]);
        fp[type] = NULL;
    }
#endif
#endif /* CONFIG_BT_SETTINGS */

    return API_SUCCESS;
}

INT16 storage_write_pl (UCHAR type, void * buffer, UINT16 size)
{
    INT16 nbytes;

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE)) || \
    (!((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS)))
    nbytes = (INT16)size;
    if ((nv_offset + nbytes) < CONFIG_NVM_SIZE)
    {
        BT_mem_copy((NvmSaveBuf + nv_offset), buffer, size);
    }
    else
    {
        /* assert here, the buffer length CONFIG_NVM_SIZE is less than the
         * length of the write data.
         */
        assert(1 == 0);
    }
    nv_offset += size;
#else
    nbytes = 0;

    if (NULL != fp[type])
    {
        nbytes = (INT16)lfs_file_write (lfs, fp[type], buffer, size);
    }
#endif

    return nbytes;
}

INT16 storage_read_pl (UCHAR type, void * buffer, UINT16 size)
{
    INT16 nbytes;

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE)) || \
    (!((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS)))
    nbytes = (INT16)size;
    BT_mem_copy(buffer, (NvmSaveBuf + nv_offset), size);
    nv_offset += size;
#else
    nbytes = 0;

    if (NULL != fp[type])
    {
        nbytes = (INT16)lfs_file_read (lfs, fp[type], buffer, size);
    }
#endif

    return nbytes;
}

INT16 storage_write_signature_pl (UCHAR type)
{
#if (STORAGE_SKEY_SIZE != 0)
    INT16 nbytes;

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE)) || \
    (!((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS)))
    if (STORAGE_NUM_TYPES <= type)
    {
        return -1;
    }
    nbytes = STORAGE_SKEY_SIZE;
    BT_mem_copy(NvmSaveBuf, ssign[type], STORAGE_SKEY_SIZE);
    nv_offset += STORAGE_SKEY_SIZE;
#else
    nbytes = 0;

    if (NULL != fp[type])
    {
        nbytes = (INT16)lfs_file_write (lfs, fp[type], ssign[type], STORAGE_SKEY_SIZE);
    }
#endif

    return nbytes;
#else /* (STORAGE_SKEY_SIZE != 0) */
    BT_IGNORE_UNUSED_PARAM(type);

    return 0U;
#endif /* (STORAGE_SKEY_SIZE != 0) */
}

INT16 storage_read_signature_pl (UCHAR type)
{
#if (STORAGE_SKEY_SIZE != 0)
    INT16 nbytes;

#if ((defined STORAGE_IDLE_TASK_SYNC_ENABLE) && (STORAGE_IDLE_TASK_SYNC_ENABLE)) || \
    (!((defined(CONFIG_BT_SETTINGS)) && (CONFIG_BT_SETTINGS)))
    if (STORAGE_NUM_TYPES <= type)
    {
        return -1;
    }
    nbytes = STORAGE_SKEY_SIZE;
    nv_offset += STORAGE_SKEY_SIZE;
    if (BT_mem_cmp (ssign[type], NvmSaveBuf, STORAGE_SKEY_SIZE))
    {
        nbytes = -1;
    }
#else
    UCHAR sign[STORAGE_SKEY_SIZE];

    nbytes = 0;

    if (NULL != fp[type])
    {
        nbytes = lfs_file_read (lfs, fp[type], sign, STORAGE_SKEY_SIZE);

        if (BT_mem_cmp (ssign[type], sign, STORAGE_SKEY_SIZE))
        {
            return -1;
        }
    }
#endif

    return nbytes;
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
