/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _KEYSTORE_PUF_H_
#define _KEYSTORE_PUF_H_

#include "bootloader_common.h"
#include "fsl_common.h"
#include "fsl_puf.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Key type of the key store. */
/*
 * Note: key_type_t should be compatible with the PUF key store for i.MXRT 3 digits series and LPC series.
 */
enum _key_type
{
    kKeyType_Invalid = 0x0u,
    // kKeyType_HashCryptSRK = 0x01u,
    kKeyType_OtfadKEK = 0x02u,
    // kKeyType_FirmwareUpdateKey0 = 0x03u,
    // kKeyType_FirmwareUpdateKey1 = 0x04u,
    // kKeyType_FirmwareUpdateKey2 = 0x05u,
    // kKeyType_FirmwareUpdateKey3 = 0x06u,
    // kKeyType_EncryptedRegionKey0 = 0x07u,
    // kKeyType_EncryptedRegionKey1 = 0x08u,
    // kKeyType_EncryptedRegionKey2 = 0x09u,
    // kKeyType_EncryptedRegionKey3 = 0x0Au,
    // kKeyType_UserKey = 0x0Bu,
    // kKeyType_UDS = 0x0Cu,
};

#if defined(BL_FEATURE_KEY_STORE_MAX_KEY_SIZE)
#if defined(PUF_MAX_KEY_CODE_SIZE)
/*
 * Note: Key store only generate encryption key, maxium key size is 256 bit. So override the definition in the PUF
 * driver, and set the maxium key code size to 52 bytes.
 */
#undef PUF_MAX_KEY_CODE_SIZE
#endif
/*! @brief Max byte count of the PUF key code. */
#define PUF_MAX_KEY_CODE_SIZE (PUF_GET_KEY_CODE_SIZE_FOR_KEY_SIZE(BL_FEATURE_KEY_STORE_MAX_KEY_SIZE))
#endif

/*! @brief PUF activation code structure. */
typedef struct _activation_code
{
    uint8_t code[PUF_ACTIVATION_CODE_SIZE]; /*!< The activation code. */
} activation_code_t;

/*! @brief PUF key code header structure. */
typedef union _key_code_header
{
    struct
    {
        uint32_t size : 6;  /*!< The size of the key. */
        uint32_t rsv0 : 10; /*!< Reserved. */
        uint32_t index : 4; /*!< The index of the key. */
        uint32_t rsv1 : 4;  /*!< Reserved. */
        uint32_t type : 2;  /*!< The type of the key, generate key or user key. */
        uint32_t rsv2 : 6;  /*!< Reserved. */
    } B;
    uint32_t U;
} key_code_header_t;

/*! @brief PUF key code structure. */
typedef struct _key_code
{
    key_code_header_t header;                                        /*!< The header of the key code. */
    uint8_t code[PUF_MAX_KEY_CODE_SIZE - sizeof(key_code_header_t)]; /*!< The key code. */
} key_code_t;

/*! @brief PUF key code instance. */
typedef struct _key_item
{
    uint32_t type;      /*!< The type of the key code, refer to _key_type. */
    key_code_t keyCode; /*!< The key code. */
} key_item_t;

/*! @brief PUF key store structure. */
typedef struct _key_store
{
    uint32_t header;            /*!< The header of the key store. */
    uint32_t version;           /*!< The key store version. */
    uint32_t flag;              /*!< The flags of the key store. */
    uint32_t reserved;          /*!< Reserved. Fixed to be zero. */
    activation_code_t actvCode; /*!< The activation code. Must be word aligned. */
    key_item_t otfadKey;        /*!< The key code for OTFAD. Must be word aligned.*/
} key_store_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

    /*!
     * @brief Initialize key store
     *
     * This function initializes key store.
     *
     * @retval kStatus_Fail Operation is failed.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_init(void);

    /*!
     * @brief Denitialize key store
     *
     * This function cleans data in the key store, and disables PUF.
     *
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_deinit(void);

    /*!
     * @brief Perform prepare operations for a new key store
     *
     * This function initializes a new key store, and get the PUF activation code.
     *
     * @retval kStatus_Fail Operation is failed.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_enroll(void);

    /*!
     * @brief Set the software/hardware key.
     *
     * This function send the specific key data to PUF and get the corresponding key code.
     *
     * @param key Buffer containing the key data.
     * @param size The size in bytes of the key.
     * @param type The type of the specific key.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Fail Operation is failed.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_set_key(uint8_t *key, uint32_t size, uint32_t type);

    /*!
     * @brief Get the software key.
     *
     * This function restores the software key from the key code.
     *
     * @param key Buffer to put the key.
     * @param size Return the key size if @param key is NULL, otherwise contain the buffer size of the @param key.
     * @param type The type of the specific key.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Fail Operation is failed.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_get_sw_key(uint8_t *key, uint32_t *size, uint32_t type);

    /*!
     * @brief Get the hardware key.
     *
     * This function restores the hardware key from the key code, and sends it to the corresponding engine.
     *
     * @param type The type of the specific key.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Fail Operation is failed.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_get_hw_key(uint32_t type);

    /*!
     * @brief Write the key store to a nonvolatile memory.
     *
     * This function writes the key store to a specific memory.
     *
     * @param memoryId The memoyr ID of the dest memory.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Fail Operation is failed.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_write_nonvolatile(uint32_t memoryId);

    /*!
     * @brief Read the key store from a nonvolatile memory.
     *
     * This function reads the key store from a specific memory.
     *
     * @param memoryId The memoyr ID of the source memory.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Fail Operation is failed.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_read_nonvolatile(uint32_t memoryId);

    /*!
     * @brief Set the key store.
     *
     * This function override the key store with the specific data.
     *
     * @param store Buffer contains the new key store.
     * @param size The size in bytes of the new key store.
     * @retval kStatus_Fail The new key store is invalid.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_set(uint8_t *store, uint32_t size);

    /*!
     * @brief Get the key store.
     *
     * This function returns the key store base address and size.
     *
     * @param store Return the key store base address.
     * @param size Return the size in bytes of the key store.
     * @retval kStatus_Fail Key store is not available.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_get(uint8_t **store, uint32_t *size);

    /*!
     * @brief Get the key store base address in the external memory.
     *
     * Note: For XIP external memory, this function should return the XIP address, not the offset of the external
     * memory.
     *
     * @param memoryId The memoyr ID of the dest memory.
     * @param offset Return the key store base address.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Success Operate successfully.
     */
    status_t get_key_store_nonvolatile_address(uint32_t memoryId, uint32_t *offset);

    /*!
     * @brief Get the key slot for the specific hardware key.
     *
     * @param slot Return the slot of the hardware key.
     * @param type The type of the specific key.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_get_hw_key_slot(uint32_t *slot, uint32_t type);

    /*! @brief Get the key size rang of the specifi key type.
     *
     * @param type The type of the specific key.
     * @param maxKeySize Return the max byte count of the specific key.
     * @param minKeySize Return the min byte count of the specific key.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Success Operate successfully.
     */
    status_t key_store_get_hw_key_range(uint32_t type, uint32_t *maxKeySize, uint32_t *minKeySize);

    /*!
     * @brief Change the key sequence to match the device-specific hardware.
     *
     * @param slot Return the slot of the hardware key.
     * @param key The user key.
     * @param size The key size of the @param key.
     * @param type The type of the specific key.
     * @retval kStatus_InvalidArgument Input argument is invalid.
     * @retval kStatus_Success Operate successfully.
     */
    void key_store_switch_sequence(uint8_t *key, uint32_t size, uint32_t type);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _KEYSTORE_PUF_H_ */
