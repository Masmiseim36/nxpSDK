
/**
 *  \file ethal_fops.c
 *
 *  This file contains all OS Abstraction functions for file
 *  and directory operations.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "ethal_fops.h"

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
#if EM_FOPS_FILE_SYNC_IN_IDLE
typedef struct _fops_file_write_buffer_note
{
    uint8_t buffer[EM_FOPS_FILE_OBJ_BUFFER_SIZE];
    uint32_t bufferLength;
} fops_file_write_buffer_node_t;

typedef struct _fops_file_handle
{
    FIL fileHandle;
    struct _fops_file_handle *next;
    size_t writeSize;
    fops_file_write_buffer_node_t *busyBuffer;
    OSA_MSGQ_HANDLE_DEFINE(fullMsgqBuffer, EM_FOPS_FILE_OBJ_BUFFER_COUNT, sizeof(fops_file_write_buffer_node_t *));
    osa_msgq_handle_t fullMsgq;
    uint32_t bufferLength;
    uint32_t lastWroteTime;
    uint32_t index;
    uint8_t closing;
} fops_file_handle_t;

typedef struct _fops_file_handle_list
{
    struct _fops_file_handle *next;
} fops_file_handle_list_t;

static void fops_idle(osa_task_param_t arg);
static OSA_TASK_DEFINE(fops_idle, EM_FOPS_FILE_SYNC_TASK_PRIORITY, 1U, EM_FOPS_FILE_SYNC_TASK_STACK_SIZE, 0U);
static OSA_SEMAPHORE_HANDLE_DEFINE(s_fopsSemaphore);
static osa_semaphore_handle_t s_fopsSemaphoreHandle;
static OSA_TASK_HANDLE_DEFINE(s_fopsIdleTask);
static osa_task_handle_t s_fopsIdleTaskHandle;
static fops_file_handle_list_t s_fopsFileObjList;
static OSA_MUTEX_HANDLE_DEFINE(s_fopsFileObjListLockMutex);
static osa_mutex_handle_t s_fopsFileObjListLock;
static OSA_MSGQ_HANDLE_DEFINE(s_emptyMsgqBuffer, EM_FOPS_FILE_OBJ_BUFFER_COUNT, sizeof(fops_file_write_buffer_node_t *));
static osa_msgq_handle_t s_emptyMsgq;
static fops_file_write_buffer_node_t s_writeBufferNodes[EM_FOPS_FILE_OBJ_BUFFER_COUNT];
#else
typedef struct _fops_file_handle
{
    FIL fileHandle;
} fops_file_handle_t;
#endif

static void fops_display_file_info(FILINFO *fileInfo);
static void fops_list_directory (CHAR *path);
#define fops_echo    printf

/* ----------------------------------------- static Functions */
#if EM_FOPS_FILE_SYNC_IN_IDLE
static void fops_file_write_buffer_queue_init(void)
{
    fops_file_write_buffer_node_t *node;
    uint32_t index = 0U;
    uint32_t regMask = 0U;
    osa_status_t ret = KOSA_StatusSuccess;

    OSA_EnterCritical(&regMask);
    if (NULL == s_fopsFileObjListLock)
    {
        ret = OSA_MutexCreate((osa_mutex_handle_t)s_fopsFileObjListLockMutex);
        if (KOSA_StatusSuccess == ret)
        {
            s_fopsFileObjListLock = (osa_mutex_handle_t)s_fopsFileObjListLockMutex;
        }
    }
    OSA_ExitCritical(regMask);

    assert(KOSA_StatusSuccess == ret);

    if (NULL == s_emptyMsgq)
    {
        ret = OSA_MsgQCreate((osa_msgq_handle_t)s_emptyMsgqBuffer, EM_FOPS_FILE_OBJ_BUFFER_COUNT, sizeof(fops_file_write_buffer_node_t *));
        if (KOSA_StatusSuccess == ret)
        {
            s_emptyMsgq = (osa_msgq_handle_t)s_emptyMsgqBuffer;
        }

        if (NULL != s_emptyMsgq)
        {
            for (index = 0U;index < EM_FOPS_FILE_OBJ_BUFFER_COUNT;index++)
            {
                node = &s_writeBufferNodes[index];
                ret = OSA_MsgQPut(s_emptyMsgq, &node);
            }
        }
        assert(KOSA_StatusSuccess == ret);
        (void)ret;
    }
}
#endif

/* ----------------------------------------- Functions */
/**
 *  \fn EM_fops_get_current_directory
 *
 *  \brief To get the current working directory.
 *
 *  \param [in] current_directory
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_get_current_directory
           (
               /* OUT */  UCHAR  *current_directory
           )
{
    FRESULT ret;
    EM_RESULT retval;

    /* NULL Check */
    if(NULL == current_directory)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    retval = EM_SUCCESS;
#if FF_FS_RPATH >= 2
    ret = f_getcwd((TCHAR*)current_directory, EM_FOPS_MAX_DIRECTORY_SIZE);
#else
        ret = FR_NOT_ENABLED;
#endif
    if (0 != ret)
    {
        retval = EM_FOPS_ERR_GET_CURRECT_DIRECTORY;
    }

    return retval;
}


/**
 *  \fn EM_fops_get_file_attributes
 *
 *  \brief To get the file attributes of a file/directory.
 *
 *  \par Description:
 *  This function is used to find out the attributes of a file/directory.
 *  For ex: is file/directory readonly etc.
 *
 *  \param [in] file
 *
 *  \param [out] file_attribute
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_get_file_attributes
           (
               /* IN */   UCHAR   * object_name,
               /* OUT */  UINT32  * file_attribute
           )
{
    FRESULT ret;
    EM_RESULT retval;
    FILINFO fileInfo;

    /* NULL Check */
    if((NULL == object_name) || (NULL == file_attribute))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    retval = EM_SUCCESS;
    *file_attribute   = 0U;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    EM_mem_set(&fileInfo, 0, sizeof(FILINFO));

    /* Get the Attributes of the file/directory */
    ret = f_stat((TCHAR*)object_name, &fileInfo);

    if(0 != ret)
    {
        retval = EM_FOPS_ERR_GET_FILE_ATTRIBUTES;
    }
    else
    {
        /* Check if it is Folder */
       if (0U != (fileInfo.fattrib & AM_DIR))
       {
           /* Set the bit for Folder */
           EM_FOPS_SET_BIT(*file_attribute, EM_FOPS_MASK_FOLDER);
       }

       /* Check if it is Readonly Folder */
       if (0U != (fileInfo.fattrib & AM_RDO))
       {
           /* Set the bit for Readonly Folder */
           EM_FOPS_SET_BIT(*file_attribute, EM_FOPS_MASK_READONLY);
       }
    }

    return retval;
}


/**
 *  \fn EM_fops_set_file_attributes
 *
 *  \brief To set the file attributes of a file/directory.
 *
 *  \par Description:
 *  This function is used to set the attributes of a file/directory.
 *
 *  \param [in] file
 *
 *  \param [in] file_attribute
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_set_file_attributes
           (
               /* IN */  UCHAR   * object_name,
               /* IN */  UINT32    file_attribute
           )
{
    BYTE attr;

    attr = (((UINT8)file_attribute & 0xFFU) == 0x01U)? AM_RDO: 0U;

    (void) f_chmod(_T((TCHAR *)object_name), attr, AM_RDO);
    return EM_FAILURE;
}

/**
 *  \fn EM_fops_access_first
 *
 *  \brief To find the first object (file/folder) in a directory.
 *
 *  \par Description:
 *  This function is used find and access the first object (file/folder)
 *  in a given directory path.
 *
 *  \param [in] dir Directory path to be used
 *  \param [in] pattern File name pattern to be matched if any
 *  \param [in] object Object handle for use in EM_fops_access_next for
 *              subsequent searches
 *  \param [in] info Information of the File/Folder object
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
EM_RESULT EM_fops_access_first
           (
               /* IN */   UCHAR   * dir,
               /* IN */   UCHAR   * pattern,
               /* OUT */  EM_fops_object_handle   * object,
               /* OUT */  EM_FOPS_FILINFO   * info
           )
{
    FRESULT ret;
    EM_RESULT retval;
    FILINFO fi;

    retval = EM_SUCCESS;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    EM_mem_set(&fi, 0, sizeof(FILINFO));

    ret = f_opendir(object, (const TCHAR*)dir);
    if(0 != ret)
    {
        retval = EM_FOPS_ERR_GET_FILE_ATTRIBUTES;
    }
    else
    {
        ret = f_readdir(object, &fi);
        if ((ret) || (!fi.fname[0U]))
        {
            retval = EM_FOPS_ERR_GET_FILE_ATTRIBUTES;
        }
        else
        {
            EM_str_copy(info->fname, fi.fname);
            info->flsize = fi.fsize;
            info->fdyear = (uint32_t)((fi.fdate >> 9U) + 1980U);
            info->fdmonth = (uint32_t)((fi.fdate >> 5U) & 0x000Fu);
            info->fdday = (uint32_t)(fi.fdate & 0x001Fu);
            info->fthour = (uint32_t)((fi.ftime >> 11U) & 0x0000001Fu);
            info->ftmin = (uint32_t)((fi.ftime >> 5U) & 0x0000003Fu);
            info->ftsec = (uint32_t)(fi.ftime & 0x0000001Fu);

            info->fattrib = fi.fattrib;
        }
    }

    return retval;
}

/**
 *  \fn EM_fops_access_next
 *
 *  \brief To find the next object (file/folder) in a directory.
 *
 *  \par Description:
 *  This function is used find and access the next object (file/folder)
 *  in a given directory.
 *
 *  \param [in] object Object handle returned by EM_fops_access_first for
 *              subsequent searches
 *  \param [in] info Information of the File/Folder object
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
EM_RESULT EM_fops_access_next
           (
               /* IN */  EM_fops_object_handle  * object,
               /* OUT */ EM_FOPS_FILINFO   * info
           )
{
    FRESULT ret;
    EM_RESULT retval;
    FILINFO fi;

    retval = EM_SUCCESS;

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    EM_mem_set(&fi, 0, sizeof(FILINFO));

    ret = f_readdir(object, &fi);
    if ((ret) || (!fi.fname[0U]))
    {
        retval = EM_FOPS_ERR_GET_FILE_ATTRIBUTES;
    }
    else
    {
        EM_str_copy(info->fname, fi.fname);
        info->flsize = fi.fsize;
        info->fdyear = (uint32_t)((fi.fdate >> 9U) + 1980U);
        info->fdmonth = (uint32_t)((fi.fdate >> 5U) & 0x000Fu);
        info->fdday = (uint32_t)(fi.fdate & 0x001Fu);
        info->fthour = (uint32_t)((fi.ftime >> 11U) & 0x0000001Fu);
        info->ftmin = (uint32_t)((fi.ftime >> 5U) & 0x0000003Fu);
        info->ftsec = (uint32_t)(fi.ftime & 0x0000001Fu);

        info->fattrib = fi.fattrib;
    }

    return retval;
}

/**
 *  \fn EM_fops_access_close
 *
 *  \brief To close the object handle.
 *
 *  \par Description:
 *  This function is used close the object and references created.
 *
 *  \param [in] object Object handle returned by EM_fops_access_first for
 *              subsequent searches
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
EM_RESULT EM_fops_access_close
           (
               /* IN */  EM_fops_object_handle   * object
           )
{
    if (NULL == object)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    (void) f_closedir(object);

    return EM_SUCCESS;
}

/**
 *  \fn EM_fops_set_path_forward
 *
 *  \brief To change the current working directory forward.
 *
 *  \param [in] folder_name
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_set_path_forward
           (
               /* IN */  UCHAR *folder_name
           )
{
    FRESULT ret;
    EM_RESULT retval;

    /* NULL Check */
    if(NULL == folder_name)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    retval = EM_SUCCESS;
#if (FF_FS_RPATH >= 2)
    ret = f_chdir((TCHAR*)folder_name);
#else
        ret = FR_NOT_ENABLED;
#endif

    if (0 != ret)
    {
        retval = EM_FOPS_ERR_SET_PATH_FORWARD;
    }

    return retval;
}


/**
 *  \fn EM_fops_set_path_backward
 *
 *  \brief To change the current working directory backward.
 *
 *  \param [in] folder_name
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_set_path_backward( void )
{
    FRESULT ret;
    EM_RESULT retval;

    retval = EM_SUCCESS;
#if (FF_FS_RPATH >= 2)
    ret = f_chdir(_T(".."));
#else
        ret = FR_NOT_ENABLED;
#endif

    if (0 != ret)
    {
        retval = EM_FOPS_ERR_SET_PATH_BACKWARD;
    }

    return retval;
}



/**
 *  \fn EM_fops_create_folder
 *
 *  \brief To Create folder/directory
 *
 *  \param [in] folder_name
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_create_folder
           (
               /* IN */  UCHAR * folder_name
           )
{
    FRESULT ret;
    EM_RESULT retval;

    /* NULL Check */
    if(NULL == folder_name)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    retval = EM_SUCCESS;
    ret = f_mkdir((TCHAR*)folder_name);

    if ((ret) && (ret != FR_EXIST))
    {
        retval = EM_FOPS_ERR_CREATE_FOLDER;
    }

    return retval;
}



/**
 *  \fn EM_fops_file_open
 *
 *  \brief To open file
 *
 *  \param [in] file_name
 *
 *  \param [in] mode
 *
 *  \param [in] file_handle
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_file_open
           (
               /* IN */  UCHAR                * file_name,
               /* IN */  UCHAR                * mode,
               /* OUT */ EM_fops_file_handle  *file_handle
           )
{
    FRESULT ret;
    EM_RESULT retval;
    UCHAR type;
    fops_file_handle_t * fhandle;

    /* NULL Check */
    if ((NULL == file_name) || (NULL == mode) || (NULL == file_handle))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }
#if EM_FOPS_FILE_SYNC_IN_IDLE
    fops_file_write_buffer_queue_init();
#endif
    /* Allocate memory for the handle */
    fhandle = (fops_file_handle_t *)EM_alloc_mem(sizeof(fops_file_handle_t));
    if (NULL == fhandle)
    {
        return EM_FOPS_MEMORY_ALLOCATION_FAILED;
    }

    (void) memset(fhandle, 0U, sizeof(*fhandle));
    *file_handle = NULL;
    retval = EM_SUCCESS;
    type = (EM_str_cmp(mode, "wb"))? FA_READ: (FA_WRITE | FA_CREATE_ALWAYS);
    ret = f_open(&fhandle->fileHandle, _T((TCHAR*)file_name), type);

    if ((ret) && (ret != FR_EXIST))
    {
        EM_free_mem(fhandle);
        retval = EM_FOPS_ERR_FILE_OPEN;
    }
    else
    {
        *file_handle = (EM_fops_file_handle)fhandle;
    }
    return retval;
}



/**
 *  \fn EM_fops_file_write
 *
 *  \brief To open file
 *
 *  \param [in] buffer
 *
 *  \param [in] buf_length
 *
 *  \param [in] file_handle
 *
 *  \param [in] bytes_written
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_file_write
           (
               /* IN */   UCHAR               * buffer,
               /* IN */   UINT16                buf_length,
               /* IN */   EM_fops_file_handle   file_handle,
               /* OUT */  UINT16              * bytes_written
           )
{
#if EM_FOPS_FILE_SYNC_IN_IDLE
    static uint8_t initialized = 0U;
#else
    FRESULT ret;
#endif
    EM_RESULT retval;
    fops_file_handle_t * fhandle;
    UINT actual = 0U;
#if EM_FOPS_FILE_SYNC_IN_IDLE
    fops_file_handle_t * fHandleP;
    fops_file_handle_t * fHandleQ;
    uint32_t regMask = 0U;
    osa_status_t ret;
    uint32_t length;

    if (NULL == s_emptyMsgq)
    {
        return EM_FOPS_ERR_FILE_WRITE;
    }

    if (0U == initialized)
    {
        OSA_EnterCritical(&regMask);
        initialized = 1U;

        if (NULL == s_fopsSemaphoreHandle)
        {
            if (KOSA_StatusSuccess == OSA_SemaphoreCreate((osa_semaphore_handle_t)s_fopsSemaphore, 0U))
            {
                s_fopsSemaphoreHandle = (osa_semaphore_handle_t)s_fopsSemaphore;
            }
            s_fopsFileObjList.next = NULL;
        }

        if ((NULL != s_fopsSemaphoreHandle) && (NULL != s_fopsFileObjListLock) && (NULL == s_fopsIdleTaskHandle))
        {
            if (KOSA_StatusSuccess == OSA_TaskCreate((osa_task_handle_t)s_fopsIdleTask, OSA_TASK(fops_idle), NULL))
            {
                s_fopsIdleTaskHandle = (osa_task_handle_t)s_fopsIdleTask;
            }
        }
        OSA_ExitCritical(regMask);
    }
#endif

    /* NULL Check */
    if ((NULL == file_handle) || (NULL == buffer) || (0U == buf_length))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    fhandle = (fops_file_handle_t *)file_handle;

    retval = EM_SUCCESS;
#if EM_FOPS_FILE_SYNC_IN_IDLE

    if (0U != fhandle->closing)
    {
        return EM_FOPS_ERR_FILE_WRITE;
    }

    if (NULL == fhandle->fullMsgq)
    {
        ret = OSA_MsgQCreate((osa_msgq_handle_t)fhandle->fullMsgqBuffer, EM_FOPS_FILE_OBJ_BUFFER_COUNT, sizeof(fops_file_write_buffer_node_t *));
        if (KOSA_StatusSuccess == ret)
        {
            fhandle->fullMsgq = (osa_msgq_handle_t)fhandle->fullMsgqBuffer;
        }
    }

    if ((NULL == s_fopsSemaphoreHandle) || (NULL == s_fopsFileObjListLock) || (NULL == s_fopsIdleTaskHandle) || (NULL == fhandle->fullMsgq))
    {
        return EM_FOPS_ERR_FILE_WRITE;
    }

    fhandle->writeSize += buf_length;
    fhandle->lastWroteTime = OSA_TimeGetMsec();

    actual = buf_length;

    fhandle->index ++;
    length = 0U;
    while (buf_length > 0u)
    {
        if (NULL == fhandle->busyBuffer)
        {
            while (KOSA_StatusSuccess != OSA_MsgQGet(s_emptyMsgq, &fhandle->busyBuffer, osaWaitForever_c))
            {
            }
            fhandle->busyBuffer->bufferLength = 0u;
        }
        length = MIN(buf_length, EM_FOPS_FILE_OBJ_BUFFER_SIZE);
        length = MIN(length, (EM_FOPS_FILE_OBJ_BUFFER_SIZE - fhandle->busyBuffer->bufferLength));

        (void) memcpy(&fhandle->busyBuffer->buffer[fhandle->busyBuffer->bufferLength], buffer, length);
        buffer += length;
        buf_length -= length;
        fhandle->busyBuffer->bufferLength += length;

        if (fhandle->busyBuffer->bufferLength >= EM_FOPS_FILE_OBJ_BUFFER_SIZE)
        {
            while (KOSA_StatusSuccess != OSA_MsgQPut(fhandle->fullMsgq, &fhandle->busyBuffer))
            {
            }
            fhandle->busyBuffer = NULL;
        }
    }

    (void) OSA_MutexLock(s_fopsFileObjListLock, osaWaitForever_c);

    fHandleP = (fops_file_handle_t *)s_fopsFileObjList.next;
    fHandleQ = NULL;
    while (NULL != fHandleP)
    {
        if (fhandle == fHandleP)
        {
            break;
        }
        fHandleQ = fHandleP;
        fHandleP = fHandleP->next;
    }
    if (NULL == fHandleP)
    {
        if (NULL == fHandleQ)
        {
            s_fopsFileObjList.next = fhandle;
        }
        else
        {
            fHandleQ->next = fhandle;
        }
        fhandle->next = NULL;
    }
    (void) OSA_MutexUnlock(s_fopsFileObjListLock);
    (void)OSA_SemaphorePost(s_fopsSemaphoreHandle);
#else
    ret = f_write((FIL *)&fhandle->fileHandle, buffer, buf_length, &actual);
    if ((ret) || (actual != buf_length))
    {
        retval = EM_FOPS_ERR_FILE_WRITE;
    }
    else
    {
        ret = f_sync((FIL *)&fhandle->fileHandle);
        if (0 != ret)
        {
            retval = EM_FOPS_ERR_FILE_WRITE;
        }
    }
#endif

    *bytes_written = actual;

    return retval;
}


/**
 *  \fn EM_fops_file_read
 *
 *  \brief To open file
 *
 *  \param [in] buffer
 *
 *  \param [in] buf_length
 *
 *  \param [in] file_handle
 *
 *  \param [in] bytes_read
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_file_read
           (
               /* IN */   UCHAR               * buffer,
               /* IN */   UINT16                buf_length,
               /* IN */   EM_fops_file_handle   file_handle,
               /* OUT */  UINT16              * bytes_read
           )
{
    fops_file_handle_t *fhandle;
    FRESULT ret;
    EM_RESULT retval;
    UINT actual;

    /* NULL Check */
    if ((NULL == file_handle) || (NULL == buffer) || (0U == buf_length))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    fhandle = (fops_file_handle_t *)file_handle;
    retval = EM_SUCCESS;
    ret = f_read((FIL *)&fhandle->fileHandle, buffer, buf_length, &actual);
    if ((ret) || (actual != buf_length))
    {
        retval = EM_FOPS_ERR_FILE_READ;
    }

    *bytes_read = (UINT16)actual;

    return retval;
}

#if 0
/**
 *  \fn EM_fops_file_print
 *
 *  \brief To print the formatted text to the file.
 *
 *  \par Description:
 *  This function is used to print formatted text to file like fprintf.
 *
 *  \param [in] file_handle File handle to be printed
 *  \param [in] fmt Format string
 *  \param [in] ... List of Identifers
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
EM_RESULT EM_fops_file_print
          (
              /* IN */   EM_fops_file_handle   file_handle,
              /* IN */   CHAR * fmt,
              /* IN */   ...
          )
{
    va_list parg;

    /* NULL Check */
    if (NULL == file_handle)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    va_start(parg, fmt);
    f_printf(file_handle, fmt, parg);
    va_end(parg);

    return EM_SUCCESS;
}
#endif /* 0 */

/**
 *  \fn EM_fops_file_put
 *
 *  \brief To print the string to the file.
 *
 *  \par Description:
 *  This function is used to print string to file like fputs.
 *
 *  \param [in] file_handle File handle to be printed
 *  \param [in] buffer Pointer to the string
 *  \param [in] buf_length Pointer Length of string. Actual printed can
 *              be returned if required
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
EM_RESULT EM_fops_file_put
          (
              /* IN */   EM_fops_file_handle   file_handle,
              /* IN */   UCHAR               * buffer,
              /* IN */   UINT16              * buf_length
          )
{
    /* NULL Check */
    if ((NULL == buffer) || (NULL == file_handle))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    (void) f_puts((TCHAR*)buffer, file_handle);

    return EM_SUCCESS;
}

/**
 *  \fn EM_fops_file_get
 *
 *  \brief To scan string from the file.
 *
 *  \par Description:
 *  This function is used to scan string of given length from file like fgets.
 *
 *  \param [in] file_handle File handle to be printed
 *  \param [in] buffer Pointer to get the string
 *  \param [in] buf_length Pointer Length of string. Actual scanned can
 *              be returned if required
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
EM_RESULT EM_fops_file_get
          (
              /* IN */    EM_fops_file_handle   file_handle,
              /* IN */    UCHAR               * buffer,
              /* INOUT */ UINT16              * buf_length
          )
{
    TCHAR * buf;
    EM_RESULT retval;

    /* NULL Check */
    if ((NULL == buffer) || (NULL == buf_length) || (NULL == file_handle))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    buf = f_gets((TCHAR*)buffer, *buf_length, file_handle);
    retval = (NULL == buf) ? EM_FAILURE : EM_SUCCESS;

    return retval;
}


/**
 *  \fn EM_fops_file_get_formatted
 *
 *  \brief To scan string from the file.
 *
 *  \par Description:
 *  This function is used to scan string of given length from file like fgets.
 *
 *  \param [in] file_handle File handle to be printed
 *  \param [in] buffer Pointer to get the string
 *  \param [in] buf_length Pointer Length of string. Actual scanned can
 *              be returned if required
 *  \param [inout] buffer Pointer to formatted paramter
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 */
EM_RESULT EM_fops_file_get_formatted
          (
              /* IN */    EM_fops_file_handle   file_handle,
              /* IN */    CHAR                * format,
              /* INOUT */ void                * parameter,
              /* INOUT */ UINT16              * length
          )
{
    TCHAR * buf;
    TCHAR buffer[32U];
    CHAR * rbuf;
    EM_RESULT retval;
    UINT16 rlen, i;

    /* NULL Check */
    if ((NULL == format) || (NULL == parameter))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    /* If length is Not NULL it is a string read */
    if (NULL != length)
    {
        rbuf = ((CHAR *)parameter);
        rlen = (*length);
    }
    else
    {
        rbuf = buffer;
        rlen = sizeof(buffer);
    }

    /* fgets(buffer, *buf_length, file_handle); */
    buf = f_gets((TCHAR*)rbuf, rlen, file_handle);

    if (NULL != buf)
    {
        rlen = EM_str_len(rbuf);
        for (i = 0U; i < rlen; i++)
        {
            if (('\r' == rbuf[i]) || ('\n' == rbuf[i]))
            {
                rbuf[i] = '\0';
                break;
            }
        }

        if (NULL == length)
        {
            (void) sscanf(rbuf, format, parameter);
        }

        retval = EM_SUCCESS;
    }
    else
    {
        retval = EM_FAILURE;
    }

    return retval;
}

#if EM_FOPS_FILE_SYNC_IN_IDLE
static void EM_fops_file_flush_pending_write
(
    /* IN */  EM_fops_file_handle  file_handle
)
{
    fops_file_handle_t *fhandle;
    fops_file_write_buffer_node_t *node;
    uint32_t regMask;
    FRESULT ret;

    /* NULL Check */
    if (NULL == file_handle)
    {
        return;
    }

    fhandle = (fops_file_handle_t *)file_handle;

    (void) OSA_MutexLock(s_fopsFileObjListLock, osaWaitForever_c);
    if ((NULL != fhandle) && (NULL != fhandle->fullMsgq))
    {
#if EM_FOPS_FILE_SYNC_TASK_LOG
        PRINTF("Sync(%d) S(%d)", fhandle->index, OSA_TimeGetMsec());
#endif
        while (KOSA_StatusSuccess == OSA_MsgQGet(fhandle->fullMsgq, &node, 0U))
        {
            UINT bytes_written = 0u;
            ret = f_write((FIL *)&fhandle->fileHandle, node->buffer, node->bufferLength, (UINT *)&bytes_written);
            if ((ret) || (bytes_written != node->bufferLength))
            {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                PRINTF("x");
#endif
            }
            else
            {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                PRINTF(".");
#endif
            }
            node->bufferLength = 0u;
            (void)OSA_MsgQPut(s_emptyMsgq, &node);
        };

        OSA_EnterCritical(&regMask);
        node = fhandle->busyBuffer;
        fhandle->busyBuffer = NULL;
        OSA_ExitCritical(regMask);

        if (NULL != node)
        {
            UINT bytes_written = 0u;
            ret = f_write((FIL *)&fhandle->fileHandle, node->buffer, node->bufferLength, (UINT *)&bytes_written);
            if ((ret) || (bytes_written != node->bufferLength))
            {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                PRINTF("x");
#endif
            }
            else
            {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                PRINTF("-");
#endif
            }
            node->bufferLength = 0u;
            (void)OSA_MsgQPut(s_emptyMsgq, &node);
        }
    }
    (void) OSA_MutexUnlock(s_fopsFileObjListLock);
}
#endif

/**
 *  \fn EM_fops_file_close
 *
 *  \brief To open file
 *
 *  \param [in] file_handle
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_file_close
           (
               /* IN */  EM_fops_file_handle  file_handle
           )
{
    fops_file_handle_t *fhandle;
#if EM_FOPS_FILE_SYNC_IN_IDLE
    fops_file_handle_t * fHandleP;
    fops_file_handle_t * fHandleQ;
    uint32_t regMask = 0U;
#endif
    FRESULT ret;
    EM_RESULT retval;

    /* NULL Check */
    if (NULL == file_handle)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    fhandle = (fops_file_handle_t *)file_handle;

#if EM_FOPS_FILE_SYNC_IN_IDLE

    OSA_EnterCritical(&regMask);
    fhandle->closing = 1U;
    OSA_ExitCritical(regMask);
    EM_fops_file_flush_pending_write(file_handle);

    if (NULL != s_fopsFileObjListLock)
    {
        (void) OSA_MutexLock(s_fopsFileObjListLock, osaWaitForever_c);
    }
    fHandleP = (fops_file_handle_t *)s_fopsFileObjList.next;
    fHandleQ = NULL;
    while (NULL != fHandleP)
    {
        if (fhandle == fHandleP)
        {
            break;
        }
        fHandleQ = fHandleP;
        fHandleP = fHandleP->next;
    }
    if (NULL != fHandleP)
    {
        if (NULL == fHandleQ)
        {
            s_fopsFileObjList.next = fHandleP->next;
        }
        else
        {
            fHandleQ->next = fhandle->next;
        }
        fhandle->next = NULL;
    }

    if (NULL != s_fopsFileObjListLock)
    {
        (void) OSA_MutexUnlock(s_fopsFileObjListLock);
    }
#endif

    retval = EM_SUCCESS;
    ret = f_close((FIL *)&fhandle->fileHandle);
    if (0 != ret)
    {
        retval = EM_FAILURE;
    }
    else
    {
        EM_free_mem(fhandle);
    }

    return retval;
}


/**
 *  \fn EM_fops_object_delete
 *
 *  \brief To open file
 *
 *  \param [in] file_name
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_object_delete
           (
               /* IN */  UCHAR * object_name
           )
{
    FRESULT ret;
    EM_RESULT retval;

    /* NULL Check */
    if(NULL == object_name)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    retval = EM_SUCCESS;
    ret = f_unlink((TCHAR *)object_name);

    if (0 != ret)
    {
        retval = EM_FAILURE;
    }

    return retval;
}

/**
 *  \fn EM_fops_file_size
 *
 *  \brief To find size of a file
 *
 *  \param [in] file_name
 *
 *  \param [out] file_size
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
EM_RESULT EM_fops_file_size
           (
               /* IN */  EM_fops_file_handle   file_handle,
               /* OUT */ UINT32              * file_size
           )
{
    fops_file_handle_t *fhandle;

    /* NULL Check */
    if ((NULL == file_handle) || (NULL == file_size))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    fhandle = (fops_file_handle_t *)file_handle;

    *file_size = f_size((FIL *)&fhandle->fileHandle);

    return EM_SUCCESS;
}

/**
 *  \fn EM_fops_file_seek
 *
 *  \brief To set the file position
 *
 *  \param [in] file_handle
 *
 *  \param [in] offset
 *
 *  \param [in] whence
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 */
EM_RESULT EM_fops_file_seek
           (
               /* IN */  EM_fops_file_handle   file_handle,
               /* IN */  INT32                 offset,
               /* IN */  INT32                 whence
           )
{
    fops_file_handle_t *fhandle;
    FRESULT ret;
    EM_RESULT retval;

    /* NULL Check */
    if (NULL == file_handle)
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    fhandle = (fops_file_handle_t *)file_handle;

    retval = EM_SUCCESS;
    ret = f_lseek ((FIL *)&fhandle->fileHandle, (FSIZE_t)offset);

    if (0 != ret)
    {
        retval = EM_FOPS_ERR_FILE_SEEK_FAILED;
    }

    return retval;
}

/**
 *  \fn EM_fops_file_copy
 *
 *  \brief To copy file
 *
 *  \param [in] existing_file_name  Name of an existing file.
 *
 *  \param [in] new_file_name  Name of the new file.
 *
 *  \param [in] fail_if_exists If this parameter is EM_TRUE and
 *                             the new file specified by new_file_name
 *                             already exists, the function fails.
 *                             If this parameter is EM_FALSE and
 *                             the new file already exists,
 *                             the function overwrites the existing file
 *                             and succeeds.
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_file_copy
           (
               /* IN */  UCHAR * existing_file_name,
               /* IN */  UCHAR * new_file_name,
               /* IN */  UCHAR   fail_if_exists
           )
{
    FRESULT ret;
    EM_RESULT retval;
    UCHAR buffer[512U];
    UINT bytes;
    fops_file_handle_t *ifhandle, *ofhandle;

    /* NULL Check */
    if ((NULL == existing_file_name) || (NULL == new_file_name))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    retval = EM_FOPS_ERR_FILE_COPY;
    ifhandle = NULL;
    ofhandle = NULL;

    /* Allocate memory for the handle */
    ifhandle = (fops_file_handle_t *)EM_alloc_mem(sizeof(fops_file_handle_t));
    if (NULL == ifhandle)
    {
        return EM_FOPS_MEMORY_ALLOCATION_FAILED;
    }

    ofhandle = (fops_file_handle_t *)EM_alloc_mem(sizeof(fops_file_handle_t));
    if (NULL == ofhandle)
    {
        EM_free_mem(ifhandle);
        return EM_FOPS_MEMORY_ALLOCATION_FAILED;
    }

    ret = f_open(&ifhandle->fileHandle, _T((TCHAR*)existing_file_name), (FA_READ));
    if (0 == ret)
    {
        ret = f_open(&ofhandle->fileHandle, _T((TCHAR*)new_file_name), (FA_WRITE | FA_CREATE_ALWAYS));
        if ((!ret) || ((ret == FR_EXIST) && (!fail_if_exists)))
        {
            while(!f_eof(&ifhandle->fileHandle))
            {
                ret = f_read(&ifhandle->fileHandle, buffer, sizeof(buffer), &bytes);
                if (0 == ret)
                {
                    ret = f_write(&ofhandle->fileHandle, buffer, bytes, &bytes);
                    if (0 == ret)
                    {
                        retval = EM_SUCCESS;
                    }
                }
            }
            (void) f_close(&ofhandle->fileHandle);
        }
        (void) f_close(&ifhandle->fileHandle);
    }

    EM_free_mem(ifhandle);
    EM_free_mem(ofhandle);

    return retval;
}

/**
 *  \fn EM_fops_file_move
 *
 *  \brief To move file
 *
 *  \param [in] existing_file_name  Name of an existing file.
 *
 *  \param [in] new_file_name  Name of the new file.
 *
 *  \return EM_RESULT: EM_SUCCESS on success otherwise an error code
 *                      describing the cause of failure.
 *
 *  \if PSEUDO_CODE
 *  <B> Pseudo Code </B>
 *  \code
 *
 *  \endcode
 *  \endif
 */
EM_RESULT EM_fops_file_move
           (
               /* IN */  UCHAR * existing_file_name,
               /* IN */  UCHAR * new_file_name
           )
{
    FRESULT ret;
    EM_RESULT retval;

    /* NULL Check */
    if ((NULL == existing_file_name) || (NULL == new_file_name))
    {
        return EM_FOPS_INVALID_PARAMETER_VALUE;
    }

    retval = EM_SUCCESS;
    ret = f_rename((TCHAR*)existing_file_name, (TCHAR*)new_file_name);
    if (0 != ret)
    {
        retval = EM_FOPS_ERR_FILE_MOVE;
    }

    return retval;
}


/**
 *  \fn EM_vfops_create_object_name
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param path
 *  \param object
 *  \param obj_name
 *
 *  \return None
 */
EM_RESULT EM_vfops_create_object_name
           (
               /* IN */  UCHAR * path,
               /* IN */  UCHAR * object,
               /* OUT */ UCHAR * obj_name
           )
{
    if ((NULL == path) || (NULL == object) || (NULL == obj_name))
    {
        return EM_FAILURE;
    }

    /* Append the path of the new directory */
    EM_str_copy (obj_name, path);
    EM_str_cat (obj_name, EM_FOPS_PATH_SEP);
    EM_str_cat (obj_name, object);

    return EM_SUCCESS;
}

/**
 *  \fn EM_vfops_set_path_backward
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param path
 *
 *  \return None
 */

EM_RESULT EM_vfops_set_path_backward
           (
               /* OUT */ UCHAR *path
           )
{
    int i;
    UCHAR *sep = (UCHAR *)EM_FOPS_PATH_SEP;

    if (NULL == path)
    {
        return EM_FAILURE;
    }

    /* Get the length of the current path */
    i = EM_str_len (path);

    /* Ignore the first '\' */
    i--;

    /* search till the '\' from back */

    while (0 != i)
    {
        if (*sep == path[i])
        {
            path[i] = '\0';
            break;
        }
        i--;
    }

    return EM_SUCCESS;
}


/**
 *  \fn EM_vfops_set_path_forward
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param path
 *  \param folder
 *
 *  \return None
 */
EM_RESULT EM_vfops_set_path_forward
           (
               /* INOUT */  UCHAR * path,
               /* IN */     UCHAR * folder
           )
{
    EM_RESULT retval;
    UINT32 file_attr;

    if ((NULL == path) || (NULL == folder))
    {
        return EM_FAILURE;
    }

    /* Append the path of the new directory */
    EM_str_cat (path, EM_FOPS_PATH_SEP);
    EM_str_cat (path, folder);

    /* Check if the directory exists or not */
    retval = EM_fops_get_file_attributes (path, &file_attr);

    if ((EM_SUCCESS != retval) || !(file_attr & EM_FOPS_MASK_FOLDER))
    {
        /* its either not a valid object or its not a folder */
        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void) EM_vfops_set_path_backward (path);
    }

    return retval;
}

void EM_fops_list_directory (CHAR * path)
{
    fops_list_directory(path);
}

static void fops_list_directory (CHAR *path)
{
    FRESULT fatfsCode = FR_OK;
    FILINFO fileInfo;
    DIR dir;
    uint8_t outputLabel = 0U;

#if FF_USE_LFN
    /* static uint8_t fileNameBuffer[FF_MAX_LFN]; */
    /* fileInfo.fname = fileNameBuffer; */
    fileInfo.fsize = FF_MAX_LFN;
#endif /* FF_USE_LFN */

    /* MISRA C-2012 Rule 9.1 | Coverity UNINIT */
    EM_mem_set(&dir, 0, sizeof(DIR));
    EM_mem_set(&fileInfo, 0, sizeof(FILINFO));

    fatfsCode = f_opendir(&dir, path);
    if (0 != fatfsCode)
    {
        fops_echo ("Failed to open directory for listing\n");
        return;
    }

    for(;;)
    {
        fatfsCode = f_readdir(&dir, &fileInfo);
        if ((fatfsCode) || (!fileInfo.fname[0U]))
        {
            break;
        }
        outputLabel = 1U;
        fops_display_file_info(&fileInfo);
    }

    if (0U == outputLabel)
    {
        fops_echo("\r\n");
    }

    return;
}

static void fops_display_file_info(FILINFO *fileInfo)
{
    char *fileName;
    (void)fileName;
    fileName = fileInfo->fname;
    /* note: if this file/directory don't have one attribute, '_' replace the attribute letter ('R' - readonly, 'H' - hide, 'S' - system) */
    fops_echo("    %s - %c%c%c - %s - %dBytes - %d-%d-%d %d:%d:%d\r\n", (fileInfo->fattrib & AM_DIR) ? "dir" : "fil",
             (fileInfo->fattrib & AM_RDO) ? 'R' : '_',
             (fileInfo->fattrib & AM_HID) ? 'H' : '_',
             (fileInfo->fattrib & AM_SYS) ? 'S' : '_',
             fileName,
             (fileInfo->fsize),
             (uint32_t)((fileInfo->fdate >> 9U) + 1980U) /* year */,
             (uint32_t)((fileInfo->fdate >> 5U) & 0x000Fu) /* month */,
             (uint32_t)(fileInfo->fdate & 0x001Fu) /* day */,
             (uint32_t)((fileInfo->ftime >> 11U) & 0x0000001Fu) /* hour */,
             (uint32_t)((fileInfo->ftime >> 5U) & 0x0000003Fu) /* minute */,
             (uint32_t)(fileInfo->ftime & 0x0000001Fu) /* second */
             );
}

#if EM_FOPS_FILE_SYNC_IN_IDLE
static void fops_idle(osa_task_param_t arg)
{
    FRESULT ret;
    fops_file_handle_t *fhandle;
    uint32_t regMask;
    osa_status_t osaRet;
    fops_file_write_buffer_node_t *node;
    uint8_t sync;
    (void)arg;

    for(;;)
    {
        osaRet = OSA_SemaphoreWait(s_fopsSemaphoreHandle, EM_FOPS_FILE_OBJ_SYNC_TIMEOUT);
        (void)osaRet;

        (void) OSA_MutexLock(s_fopsFileObjListLock, osaWaitForever_c);
        fhandle = s_fopsFileObjList.next;
        while ((NULL != fhandle) && (NULL != fhandle->fullMsgq))
        {
            sync = 0u;
#if EM_FOPS_FILE_SYNC_TASK_LOG
            PRINTF("Sync(%d) S(%d)", fhandle->index, OSA_TimeGetMsec());
#endif
            while (KOSA_StatusSuccess == OSA_MsgQGet(fhandle->fullMsgq, &node, 0U))
            {
                UINT bytes_written = 0u;
                ret = f_write((FIL *)&fhandle->fileHandle, node->buffer, node->bufferLength, (UINT *)&bytes_written);
                if ((ret) || (bytes_written != node->bufferLength))
                {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                    PRINTF("x");
#endif
                }
                else
                {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                    PRINTF(".");
#endif
                    sync = 1u;
                }
                node->bufferLength = 0u;
                (void)OSA_MsgQPut(s_emptyMsgq, &node);
            };

            if ((fhandle->lastWroteTime + EM_FOPS_FILE_OBJ_SYNC_TIMEOUT) <= OSA_TimeGetMsec())
            {
                OSA_EnterCritical(&regMask);
                node = fhandle->busyBuffer;
                fhandle->busyBuffer = NULL;
                OSA_ExitCritical(regMask);

                if (NULL != node)
                {
                    UINT bytes_written = 0u;
                    ret = f_write((FIL *)&fhandle->fileHandle, node->buffer, node->bufferLength, (UINT *)&bytes_written);
                    if ((ret) || (bytes_written != node->bufferLength))
                    {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                        PRINTF("x");
#endif
                    }
                    else
                    {
#if EM_FOPS_FILE_SYNC_TASK_LOG
                        PRINTF("-");
#endif
                        sync = 1u;
                    }
                    node->bufferLength = 0u;
                    (void)OSA_MsgQPut(s_emptyMsgq, &node);
                }
            }

            if (0u < sync)
            {
                ret = f_sync((FIL *)&fhandle->fileHandle);
                (void)ret;
#if EM_FOPS_FILE_SYNC_TASK_LOG
                PRINTF(" E(%d) w%d s%d\r\n", OSA_TimeGetMsec(), fhandle->writeSize, f_size((FIL *)&fhandle->fileHandle));
#endif
            }
            fhandle = fhandle->next;
        }
        (void) OSA_MutexUnlock(s_fopsFileObjListLock);
    }
}
#endif

