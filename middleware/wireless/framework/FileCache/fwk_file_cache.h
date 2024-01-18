/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FWK_FILECACHE_H__
#define __FWK_FILECACHE_H__

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * \brief RAM buffer management in fileSystem
 *
 * This FileSystem extension provides a RAM buffer management associated with the File System. This is targeted to have
 * a low RAM memory footprint based on a dynamic internal allocation of the RAM buffer, the memory pool being provided
 * at initialization. The Module also supports multi threading for complex applications.
 *
 * Basically, it provides the Ability to :
 * - Register a context configuration with pre allocated scratch RAM memory for buffer pool
 * - Allocate a RAM buffer from the scratch memory and synchronize the data with a file in the file system (Open API)
 * - Allow the caller to do read, write onto the buffer without restriction
 * - Write the updated data into the file system and free the allocated buffer (Close API). The write can be done
 * immediatly, or save into a write queue, the data being written in the file system when FC_Process() is called,
 * typically from Idle task
 * - Protect from concurrent access the writing of the buffer into the file system
 *
 */

/**
 * \brief Deschedule Time delay if FC_Open() called during buffer write to Flash, Scheduler disabled if 0
 *
 * When a buffer is being written to a file in file system, If FC_Open() to the same file is called, this MACRO provides
 * the time for the function to deschedule so it gives time for the buffer to be written into flash, the value of the
 * MACRO provides the value passed to OSA_TimeDelay() as argument for this purpose. If the macro is defined to 0, the
 * scheduler will be disabled during the write to file system period so no other FC_Open() could happen
 * */
#ifndef FC_EnableTaskDelayKeepSchedulerEnabled_d
#define FC_EnableTaskDelayKeepSchedulerEnabled_d 0
#endif

/**
 * \brief Task delay time
 *
 * Delay the task by FC_DEINIT_WAIT_LOOP_MS ms in order to leave time to write in flash the buffers that are still
 * allocated in the scratch ram section
 */
#ifndef FC_WAIT_LOOP_MS
#define FC_WAIT_LOOP_MS 10
#endif

/**
 * \brief Maximum number of attempts to free the scratch ram section during the deinitialization before the function
 * return an error status
 */
#ifndef FC_DEINIT_WAIT_LOOP_MAX_ITERATIONS_N
#define FC_DEINIT_WAIT_LOOP_MAX_ITERATIONS_N 100
#endif

/**
 * \brief Defines the maximum size of the file name inside the buffer descriptor structure
 */
#ifndef FC_FILE_NAME_SIZE_MAX
#define FC_FILE_NAME_SIZE_MAX 16
#endif

/**
 * \brief Definitions for flags argument provided to FC_Close()
 */
#define FC_CLOSE_ONLY                 (0)
#define FC_CLOSE_AND_SAVE_IMMEDIATELY (1)
#define FC_CLOSE_AND_SAVE_TO_QUEUE    (2)

#define FC_FILE_CACHE_SIZE \
    256 /** used by file system as file cache in RAM, one per configuration, located in scratch area */
#define FC_CONTEXT_SIZE                                                                  \
    (20 + FC_FILE_CACHE_SIZE) /** used by file system as context and file cache, one per \
                           configuration, located in scratch area */
#define FC_FILE_DESC_SIZE                                                                                              \
    (12 + FC_FILE_NAME_SIZE_MAX) /** used by file system as file descriptor in RAM, one per buffer, located in scratch \
       area */

/**
 * \brief Provides the RAM buffer management to file system configuration when calling FC_InitConfig()
 */
typedef struct fc_config_s
{
    const char *folder_name;   // for future use, not supported for now
    void *      scracth_ram_p; /* Pointer to an allocated RAM space for buffer pool */
    uint16_t scracth_ram_sz; /* Size of the Allocated RAM space , should be ((( <your_buffer_size> + FC_FILE_DESC_SIZE)
                              * <your_buffer_number> ) + FC_CONTEXT_SIZE) */
} fc_config_t;

/**
 * \brief Register new buffer configuration
 *
 * \param config : configuration structure containing scratch RAM area for context, file descriptor and buffer
 * management
 * \return void* fc_context_p : handle to context for this configuration, to be used for FC_Open() API
 */
void *FC_InitConfig(const fc_config_t *config);

/**
 * \brief Deinitialize the FileCache configuration for the specified context
 *
 * Clear the scratch RAM section.
 *
 * \param fc_context_p [in] context handle returned by FC_InitConfig()
 * \return int : 0 if successful, -1 in case of error
 */
int FC_DeInit(void *fc_context_p);

/**
 * \brief Allocate a new RAM buffer and copy data from file in File system
 *
 * A new buffer is allocated from the scratch RAM memory provided during FC_InitConfig(). The requested Size of the
 * buffer is given by buffer_length argument.
 *
 * If the associated file given by buffer name exists in file system, The buffer will be filled with the file content
 * from the File system. Maximum copied data size is given by buffer_length argument even if the File size in File
 * system is larger. If the file size in file system is smaller, the full data of the file will be copied in the ram
 * buffer and the user could append more data in the buffer.
 *
 * If the File does not exist, an empty file will be created in filesystem and the buffer of size buffer_length will  be
 * allocated. However, on function return, the buffer_length will be 0 to indicate no data has been filled into the
 * buffer
 *
 * On FC_Open() function return, If the function does not return NULL, the buffer_addr argument provides the address to
 * the new allocated buffer and buffer_length argument provides the number of bytes that the buffer contains. The value
 * is smaller or equal to the requested Size.
 * The buffer descriptor handle provided by the returned value shall then be used to close the buffer when this last is
 * no longer used and shall be written into flash. See FC_Close() API. After a successfull FC_Open() call, the buffer
 * refered by the buffer descriptor is exclusively owned by the caller. The caller is allowed to do any read/write in
 * the buffer whose size is always the requested size value passed on FC_Open().
 *
 * If there is no buffer left in the scratch RAM memory, or if the size of the remaining buffer is lower than the
 * requested size, The function will return NULL and buffer_length will provide the size of the free buffer left (0 if
 * no buffer left). The caller would need to recall the function with the buffer_length returned value to have it
 * successful (if different from 0)
 *
 * If the buffer is in the write queue when calling FC_Open(), the buffer will be removed from the write queue and
 * provided back to the caller provided that the requested size is less than or equal to the buffer size. If the buffer
 * is being written into the File system when calling FC_Open(), if FC_EnableTaskDelayKeepSchedulerEnabled_d is
 * different from 0, The function will deschedule by calling OSA_TimeDelay() function until the Buffer write into Flash
 * is completed. if FC_EnableTaskDelayKeepSchedulerEnabled_d is 0, The write process into the file system has the
 * scheduler disabled to prevent this situation to happen. However, if the caller needs to have scheduler still running
 * to execute higher priority task, FC_EnableTaskDelayKeepSchedulerEnabled_d shall be set.
 *
 * The flag argument is reserved for future used for optimization (example of potential optimization : If the buffer is
 * not planned to be written, the buffer adress could be directly the address in Flash so no RAM buffer will be used for
 * RAM saving optimization).
 *
 * \param fc_context_p [in] context handle returned by  FC_InitConfig()
 * \param buffer_name [in]
 * \param buffer_addr [out] provide the buffer address. the buffer will be filled from buffer_name file if the file
 * exists in the filesystem
 * \param buffer_length [in/out] provide the requested buffer size for buffer_name, get the real read size for
 * buffer_name file in flash
 * \param flag [in] for future used
 * \return void* fc_hdl : buffer descriptor handle to be used for closing the file FC_Close()
 */
void *FC_Open(
    void *fc_context_p, const char *buffer_name, uint8_t **buffer_addr, uint16_t *buffer_length, uint16_t flag);

/**
 * \brief Get the effective useful size of the file
 *
 * It checks also if the file is in the write list and get the size of its payload.
 *
 * \param buffer_name [in]
 * \param payload_size [out] : size of the usefull payload in the buffer
 * \return int : 0 if successful, -1 in case of error
 */
int FC_GetFileSize(const char *buffer_name, uint16_t *payload_size);

/**
 * \brief Close the buffer : write to Filesystem and free the RAM buffer
 *
 * Depending of the flag argument, the buffer will be :
 * - write into File system immediatly : FC_CLOSE_AND_SAVE_IMMEDIATELY
 * - add buffer into write queue : FC_CLOSE_AND_SAVE_TO_QUEUE
 * - no write to File System : FC_CLOSE_ONLY
 *
 * After the above step, the buffer is then free and added by into the scratch RAM memory
 *
 * \param fc_hdl : buffer descriptor handle to the buffer
 * \param payload_size : size of the usefull payload in the buffer to write into the file system
 * \param flag : FC_CLOSE_ONLY, FC_CLOSE_AND_SAVE_IMMEDIATELY or FC_CLOSE_AND_SAVE_TO_QUEUE
 * \return int always return 0
 */
int FC_Close(void *fc_hdl, uint16_t payload_size, uint16_t flag);

/**
 * \brief process one buffer from the write queue and save it into the Filesystem
 *
 * FC_Process() will perfom only one buffer write to file system. In order to empty the queue, this function shall
 * be called until it returns 0
 *
 * This function shall be typically called from Idle task
 *
 * \return int : Number of buffer written into Filesystem (1 by design)
 *      0 if write queue is empty.
 */
int FC_Process(void);

/**
 * \brief Empty the buffer : delete the content of the buffer whether it is in the write queue or in flash
 *
 * \param buffer_name [in]: name of the buffer
 * \return int : 0 if successful, -1 in case of error
 */
int FC_Delete(const char *buffer_name);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif