/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FWK_KEYSTORAGE_H__
#define __FWK_KEYSTORAGE_H__

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/**
 * \brief Key storage management
 *
 */

/**
 * \brief Task delay time
 *
 * Delay the task by KS_OPEN_WAIT_LOOP_MS ms in order to leave time to write in flash the buffers in the write queue and
 * free space
 */
#ifndef KS_OPEN_WAIT_LOOP_MS
#define KS_OPEN_WAIT_LOOP_MS 1
#endif

/**
 * \brief Maximum number of attempts to open the buffer
 *
 * Try to open a buffer multiple times until it succeeds or reaches KS_OPEN_WAIT_LOOP_MAX_ITERATIONS_N number of
 * iteration. If FC_Open(...) fails, the buffer can't be opened with the requested size.
 */
#ifndef KS_OPEN_WAIT_LOOP_MAX_ITERATIONS_N
#define KS_OPEN_WAIT_LOOP_MAX_ITERATIONS_N 100
#endif

/**
 * \enum ks_error_t
 * \brief Define error type for KeyStorage
 *
 * \var KS_ERROR_NONE: no error
 * \var KS_ERROR_BUF_TOO_SMALL: there is not enough space to memorize a new key
 * \var KS_ERROR_KEY_NOT_FOUND: the key was not found in the buffer
 */
typedef enum
{
    KS_ERROR_NONE          = 0,
    KS_ERROR_BUF_TOO_SMALL = 1,
    KS_ERROR_KEY_NOT_FOUND = 2,
} ks_error_t;

/**
 * \brief Structure for KeyStorage configuration
 *
 * \param size : size in bytes of the ram memory section dedicated to the KS configuration. If the memory is not
 * pre-allocated this size will be used for the dynamic allocation in KS_Init()
 * \param KS_Name : name of the KS configuration
 * \param mem_p : pointer to the pre-allocated buffer in memory. If NULL, a dynamic allocation will be
 * done in the KS_Init() function
 */
typedef struct ks_config_s
{
    int         size;
    const char *KS_name;
    uint8_t *   mem_p;
} ks_config_t;

/**
 * \brief Register new key storage configuration
 *
 * \param ks_config_p [in] configuration structure containing scratch RAM area for context, folder name
 * \return void* ks_handle_p: handle to context for this configuration
 */
void *KS_Init(ks_config_t *ks_config_p);

/**
 * \brief Reset all files for the current configuration
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \return int : 0 if successful, -1 in case of error
 */
int KS_Reset(void *ks_handle_p);

/**
 * \brief Release the allocated resources
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \return int : 0 if successful, -1 in case of error
 */
int KS_DeInit(void *ks_handle_p);

/**
 * \brief Add a new integer key to the buffer
 *
 * If the same key is already stored, a new copy will be added.
 * If there is no buffer available, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS wait for buffer to
 * be free again. If the opening of the buffer still fails after waiting, there is not enough space to store the new key
 * and an error is returned.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key [in] new integer key to store
 * \param key_namespace [in] namespace of the key to identify the right file in which to store it
 * \param value [in] pointer to the new value to store
 * \param val_size [in] length of the value in bytes
 * \return ks_error_t status: KS_ERROR_NONE if successful, KS_ERROR_BUF_TOO_SMALL in case of error
 */
ks_error_t KS_AddKeyInt(void *ks_handle_p, int key, char *key_namespace, void *value, int val_sz);

/**
 * \brief Get integer key from the buffer
 *
 * If the same key is stored multiple times in the buffer, only the first match will be reported.
 * If there is not enough space to open the buffer, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS to
 * wait for buffer to be free again. If the opening of the buffer still fails after waiting, an error is returned.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key [in] int key to search
 * \param key_namespace [in] namespace of the key to identify the right file in which it is stored
 * \param value_p [out] pointer to the value if found
 * \param buf_sz [in] max buffer size in bytes
 * \param val_sz [out] real read size in bytes
 * \return ks_error_t status: KS_ERROR_NONE if successful, KS_ERROR_KEY_NOT_FOUND in case of error
 */
ks_error_t KS_GetKeyInt(void *ks_handle_p, int key, char *key_namespace, void *value_p, int buf_sz, int *val_sz);

/**
 * \brief Add a new string key to the buffer
 *
 * If the same key is already stored, a new copy will be added.
 * If there is no buffer available, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS to wait for buffer to
 * be free again. If the opening of the buffer still fails after waiting, there is not enough space to store the new key
 * and an error is returned.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key_buf [in] new string key to store
 * \param key_len [in] length of the key in bytes including end char \0
 * \param key_namespace [in] namespace of the key to identify the right file in which to store it
 * \param value [in] pointer to the new value to store
 * \param val_size [in] length of the value in bytes
 * \return ks_error_t status: KS_ERROR_NONE if successful, KS_ERROR_BUF_TOO_SMALL in case of error
 */
ks_error_t KS_AddKeyString(void *ks_handle_p, char *key_buf, int key_len, char *key_namespace, void *value, int val_sz);

/**
 * \brief Get string key from the buffer
 *
 * If the same key is stored multiple times in the buffer, only the first match will be reported.
 * If there is not enough space to open the buffer, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS to
 * wait for buffer to be free again. If the opening of the buffer still fails after waiting, an error is returned.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key_buf [in] string key to search
 * \param key_len [in] length of the key in bytes including end char \0
 * \param key_namespace [in] namespace of the key to identify the right file in which it is stored
 * \param value_p [out] pointer to the value if found
 * \param buf_sz [in] max buffer size in bytes
 * \param val_sz [out] real read size in bytes
 * \return ks_error_t status: KS_ERROR_NONE if successful, KS_ERROR_KEY_NOT_FOUND in case of error
 */
ks_error_t KS_GetKeyString(
    void *ks_handle_p, char *key_buf, int key_len, char *key_namespace, void *value_p, int buf_sz, int *val_sz);

/**
 * \brief Replace the value associated to the specified integer key or eventually add a new integer key
 *
 * If the key is already present once or multiple times in the buffer, all the entries associated to it will be deleted
 * and a new entry will be added at the end of the buffer.
 * If the key is not present in the buffer, it will be added.
 * If there is not enough space to open the buffer, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS to
 * wait for buffer to be free again. Return an error if the buffer is too small to replace the value with one of a
 * bigger size.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key [in] integer key to set
 * \param key_namespace [in] namespace of the key to identify the right file in which it is stored
 * \param value [in] pointer to the new value to set
 * \param val_size [in] length of the value in bytes
 * \return ks_error_t status: KS_ERROR_NONE if the replacement was successful, KS_ERROR_BUF_TOO_SMALL in case of error
 */
ks_error_t KS_SetKeyInt(void *ks_handle_p, int key, char *key_namespace, void *value_p, int val_sz);

/**
 * \brief Delete all the entries associated to the specified integer key
 *
 * If there is not enough space to open the buffer, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS to
 * wait for buffer to be free again. If the opening of the buffer still fails after waiting, an error is returned.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key [in] integer key to delete
 * \param key_namespace [in] namespace of the key to identify the right file in which it is stored
 * \return ks_error_t status: KS_ERROR_NONE if successful, KS_ERROR_KEY_NOT_FOUND in case of error
 */
ks_error_t KS_DeleteKeyInt(void *ks_handle_p, int key, char *key_namespace);

/**
 * \brief Replace the value associated to the specified string key or eventually add a new string key
 *
 * If the key is already present once or multiple times in the buffer, all the entries associated to it will be deleted
 * and a new entry will be added at the end of the buffer.
 * If the key is not present in the buffer, it will be added.
 * If there is not enough space to open the buffer, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS to
 * wait for buffer to be free again. Return an error if the buffer is too small to replace the value with one of a
 * bigger size.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key_buf [in] string key to set
 * \param key_len [in] length of the key in bytes including end char \0
 * \param key_namespace [in] namespace of the key to identify the right file in which it is stored
 * \param value [in] pointer to the new value to set
 * \param val_size [in] length of the value in bytes
 * \return ks_error_t status: KS_ERROR_NONE if the replacement was successful, KS_ERROR_BUF_TOO_SMALL in case of error
 */
ks_error_t KS_SetKeyString(
    void *ks_handle_p, char *key_buf, int key_len, char *key_namespace, void *value_p, int val_sz);

/**
 * \brief Delete all the entries associated to the specified string key
 *
 * If there is not enough space to open the buffer, the task is delayed by defining the macro KS_OPEN_WAIT_LOOP_MS to
 * wait for buffer to be free again. If the opening of the buffer still fails after waiting, an error is returned.
 *
 * \param ks_handle_p [in] context handle returned by KS_Init()
 * \param key_buf [in] string key to delete
 * \param key_len [in] length of the key in bytes including end char \0
 * \param key_namespace [in] namespace of the key to identify the right file in which it is stored
 * \return ks_error_t status: KS_ERROR_NONE if successful, KS_ERROR_KEY_NOT_FOUND in case of error
 */
ks_error_t KS_DeleteKeyString(void *ks_handle_p, char *key_buf, int key_len, char *key_namespace);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif