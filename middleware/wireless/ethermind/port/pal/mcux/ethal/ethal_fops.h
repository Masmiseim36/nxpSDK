
/**
 *  \file ethal_fops.h
 *
 *  This file has all definitions of constants and declarations APIs
 *  for File Operations.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_ETHAL_FOPS_
#define _H_ETHAL_FOPS_

/* ------------------------- Header File Inclusion */
/* EtherMind Common Header Files */
#include "EM_os.h"
#include "ff.h"

/* ------------------------- Debug Macros */

/* ------------------------- Global Definitions */
/* Platform type for File handle */
typedef FIL* EM_fops_file_handle;

/* Platform type for Object handle */
typedef DIR EM_fops_object_handle;

/* Below changes required to test LC3 encoding-decoding without overwriting the output file's
 * initial data with few last samples of the file.
 * For more detail, please see Bugzilla 974 RCA
 */
#ifdef LC3_TEST
#define EM_FOPS_FILE_SYNC_IN_IDLE 0
#endif

/* when EM_FOPS_FILE_SYNC_IN_IDLE is enable
 * 1. when one file is written firstly, the file need be closed before reading it.
 * 2.If there is file reading in other tasks that is executing in parallel with file sync idle task, FF_FS_REENTRANT need be enabled.
 */
#ifndef EM_FOPS_FILE_SYNC_IN_IDLE
#define EM_FOPS_FILE_SYNC_IN_IDLE 1U
#endif

#if EM_FOPS_FILE_SYNC_IN_IDLE

#ifndef EM_FOPS_FILE_SYNC_TASK_PRIORITY
#define EM_FOPS_FILE_SYNC_TASK_PRIORITY (configMAX_PRIORITIES - 4)
#endif

#ifndef EM_FOPS_FILE_SYNC_TASK_STACK_SIZE
#define EM_FOPS_FILE_SYNC_TASK_STACK_SIZE 1024U
#endif

#ifndef EM_FOPS_FILE_SYNC_TASK_LOG
#define EM_FOPS_FILE_SYNC_TASK_LOG 0U
#endif

/*
 * SYNC the write file if the file was written last befor
 * EM_FOPS_FILE_OBJ_SYNC_TIMEOUT (unit ms)
 */
#ifndef EM_FOPS_FILE_OBJ_SYNC_TIMEOUT
#define EM_FOPS_FILE_OBJ_SYNC_TIMEOUT (3000U)
#endif

#ifndef EM_FOPS_FILE_OBJ_BUFFER_SIZE
#define EM_FOPS_FILE_OBJ_BUFFER_SIZE (FF_MAX_SS)
#endif

#ifndef EM_FOPS_FILE_OBJ_BUFFER_COUNT
#define EM_FOPS_FILE_OBJ_BUFFER_COUNT (8U)
#endif

#endif

/** Bitmap mask for File Attributes */
#define EM_FOPS_MASK_READONLY                 0x00000001U
#define EM_FOPS_MASK_HIDDEN                   0x00000002U
#define EM_FOPS_MASK_SYSTEM                   0x00000004U
#define EM_FOPS_MASK_FOLDER                   0x00000010U
#define EM_FOPS_MASK_ARCHIVE                  0x00000020U

/** File Seek Positions */
#define EM_FOPS_SEEK_SET                      0U
#define EM_FOPS_SEEK_CUR                      1U
#define EM_FOPS_SEEK_END                      2U

#define EM_FOPS_BASE                          "1:"
#define EM_FOPS_PATH_SEP                      "/"
#define EM_FOPS_MAX_DIRECTORY_SIZE            256U
#define EM_FOPS_MAX_FN_SIZE                   256U

/** FOPS Error */
#define EM_FOPS_ERR_ID                          0xF000U

#define EM_FOPS_MUTEX_INIT_FAILED               \
        (0x0001U | EM_FOPS_ERR_ID)
#define EM_FOPS_COND_INIT_FAILED                \
        (0x0002U | EM_FOPS_ERR_ID)
#define EM_FOPS_MUTEX_LOCK_FAILED               \
        (0x0003U | EM_FOPS_ERR_ID)
#define EM_FOPS_MUTEX_UNLOCK_FAILED             \
        (0x0004U | EM_FOPS_ERR_ID)
#define EM_FOPS_MEMORY_ALLOCATION_FAILED        \
        (0x0005U | EM_FOPS_ERR_ID)

#define EM_FOPS_ERR_GET_CURRECT_DIRECTORY       (0x0011U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_GET_FILE_ATTRIBUTES         (0x0012U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_SET_PATH_FORWARD            (0x0013U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_SET_PATH_BACKWARD           (0x0014U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_CREATE_FOLDER               (0x0015U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_OPEN                   (0x0016U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_WRITE                  (0x0017U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_READ                   (0x0018U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FOLDER_DELETE               (0x0019U | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_DELETE                 (0x001AU | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_NOT_FOUND              (0x001BU | EM_FOPS_ERR_ID)
#define EM_FOPS_INVALID_PARAMETER_VALUE         (0x001CU | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_SEEK_FAILED            (0x001DU | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_COPY                   (0x001EU | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_FILE_MOVE                   (0x001FU | EM_FOPS_ERR_ID)
#define EM_FOPS_ERR_SET_FILE_ATTRIBUTES         (0x0020U | EM_FOPS_ERR_ID)

/* ------------------------- Macros */
/** Create File path with seperator */
#define EM_FOPS_PATH_JOIN(a,b)                a EM_FOPS_PATH_SEP b

/** To set/get/reset a Bit in File Operations Bitmaps */
#define EM_FOPS_SET_BIT(bitmap, mask)         (bitmap) |= (mask)
#define EM_FOPS_GET_BIT(bitmap, mask)         (((bitmap) & (mask)) == (mask))
#define EM_FOPS_RESET_BIT(bitmap, mask)       (bitmap) ^= (mask)

#define EM_fops_file_print(fd,...)            (void)f_printf((fd), __VA_ARGS__)

/* -------------------------------------------- Data Structures */
typedef struct _EM_FOPS_FILINFO
{
    CHAR    fname[EM_FOPS_MAX_FN_SIZE];
    UINT32  fhsize;
    UINT32  flsize;

    UINT32  fdyear;
    UINT32  fdmonth;
    UINT32  fdday;

    UINT32  fthour;
    UINT32  ftmin;
    UINT32  ftsec;

    UINT16  fattrib;
} EM_FOPS_FILINFO;

/* ------------------------- Function Declarations */

EM_RESULT EM_fops_get_current_directory
           (
               /* OUT */  UCHAR  * current_directory
           );

EM_RESULT EM_fops_get_file_attributes
           (
               /* IN */   UCHAR   * object_name,
               /* OUT */  UINT32  * file_attribute
           );

EM_RESULT EM_fops_set_file_attributes
           (
               /* IN */   UCHAR   * object_name,
               /* IN */   UINT32    file_attribute
           );

EM_RESULT EM_fops_access_first
           (
               /* IN */   UCHAR   * dir,
               /* IN */   UCHAR   * pattern,
               /* OUT */  EM_fops_object_handle   * object,
               /* OUT */  EM_FOPS_FILINFO   * info
           );

EM_RESULT EM_fops_access_next
           (
               /* IN */  EM_fops_object_handle   * object,
               /* OUT */ EM_FOPS_FILINFO   * info
           );

EM_RESULT EM_fops_access_close
           (
               /* IN */  EM_fops_object_handle   * object
           );

EM_RESULT EM_fops_set_path_forward
           (
               /* IN */  UCHAR *folder_name
           );

EM_RESULT EM_fops_set_path_backward( void );

EM_RESULT EM_fops_create_folder
           (
               /* IN */  UCHAR * folder_name
           );

EM_RESULT EM_fops_file_open
           (
               /* IN */  UCHAR                * file_name,
               /* IN */  UCHAR                * mode,
               /* OUT */ EM_fops_file_handle  *file_handle
           );

EM_RESULT EM_fops_file_write
           (
               /* IN */   UCHAR               * buffer,
               /* IN */   UINT16                buf_length,
               /* IN */   EM_fops_file_handle   file_handle,
               /* OUT */  UINT16              * bytes_written
           );

EM_RESULT EM_fops_file_read
           (
               /* IN */   UCHAR               * buffer,
               /* IN */   UINT16                buf_length,
               /* IN */   EM_fops_file_handle   file_handle,
               /* OUT */  UINT16              * bytes_read
           );

#if 0
EM_RESULT EM_fops_file_print
          (
              /* IN */   EM_fops_file_handle   file_handle,
              /* IN */   CHAR * fmt,
              /* IN */   ...
          );
#endif /* 0 */

EM_RESULT EM_fops_file_put
          (
              /* IN */   EM_fops_file_handle   file_handle,
              /* IN */   UCHAR               * buffer,
              /* INOUT */   UINT16           * buf_length
          );

EM_RESULT EM_fops_file_get
          (
              /* IN */   EM_fops_file_handle   file_handle,
              /* IN */   UCHAR               * buffer,
              /* INOUT */   UINT16           * buf_length
          );

EM_RESULT EM_fops_file_get_formatted
          (
              /* IN */    EM_fops_file_handle   file_handle,
              /* IN */    CHAR                * format,
              /* INOUT */ void                * parameter,
              /* INOUT */ UINT16              * length
          );

EM_RESULT EM_fops_file_close
           (
               /* IN */  EM_fops_file_handle  file_handle
           );

EM_RESULT EM_fops_object_delete
           (
               /* IN */  UCHAR * object_name
           );

EM_RESULT EM_fops_file_size
           (
               /* IN */  EM_fops_file_handle   file_handle,
               /* OUT */ UINT32              * file_size
           );

EM_RESULT EM_fops_file_seek
           (
               /* IN */  EM_fops_file_handle   file_handle,
               /* IN */  INT32                 offset,
               /* IN */  INT32                 whence
           );

EM_RESULT EM_fops_file_copy
           (
               /* IN */  UCHAR * existing_file_name,
               /* IN */  UCHAR * new_file_name,
               /* IN */  UCHAR   fail_if_exists
           );

EM_RESULT EM_fops_file_move
           (
               /* IN */  UCHAR * existing_file_name,
               /* IN */  UCHAR * new_file_name
           );

EM_RESULT EM_vfops_create_object_name
           (
               /* IN */  UCHAR * path,
               /* IN */  UCHAR * object,
               /* OUT */ UCHAR * obj_name
           );

EM_RESULT EM_vfops_set_path_backward
           (
               /* OUT */ UCHAR *path
           );

EM_RESULT EM_vfops_set_path_forward
           (
               /* INOUT */  UCHAR * path,
               /* IN */     UCHAR * folder
           );

void EM_fops_list_directory (CHAR * path);

#endif /* _H_ETHAL_FOPS_ */

