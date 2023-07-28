/*
 * Copyright 2019-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include "key_store_puf.h"
#include "memory/memory.h"

#if BL_FEATURE_KEY_STORE_PUF

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The header tag of the puf key store. */
#define KEY_STORE_HEADER (0x4b505546) /* ASICII code for 'K', 'P', 'U' and 'F'*/

/*! @brief The version tag of the puf key store. */
#define KEY_STORE_VERSION (MAKE_VERSION(1, 1, 0))

/*! @brief The base address of PUF used for key store. */
#define KEY_STORE_PUF (KEY_MANAGER__PUF)

/*! @brief The key index used for PUF key store. */
#define KEY_STORE_PUF_HW_KEY_INDEX (kPUF_KeyIndex_00)

/*! @brief The key store alignment. Must be word aligned. */
#define KEY_STORE_ALIGNMENT (4)

/*! @brief The state of the PUF. */
typedef enum _puf_state
{
    kPUF_State_Init =
        0, /*!< Initial state after power up, including Init_Error, Init_Cold and Init_Warm. Must be zero. */
           /*
            * Note: it is not necessary to distinguish the three state, Error, Cold and Warm. In fact, there is no register for
            * software to get current PUF state. The value in PUF ALLOW register will show current state indirectly. PUF driver
            * will check it before execute the command
            */
    kPUF_State_Initialized = 1, /*!< Initialized state, including Error, Cold and Warm. */
    kPUF_State_Enrolled = 2,    /*!< Enrolled state, only set key command is allowed. */
    kPUF_State_Started = 3,     /*!< Started state, set&get key command are allowed. */
} puf_state_t;

/*! @brief PUF key store context structure. */
typedef struct _key_store_context
{
    puf_config_t config; /*!< The PUF configuration. */
    puf_state_t state;   /*!< The PUF state. */
} key_store_context_t;

/*******************************************************************************
 * Prototype
 ******************************************************************************/
/*!
 * @brief Setup the PUF with the activation code.
 *
 * @retval kStatus_Fail Operation is failed.
 * @retval kStatus_Success Operate successfully.
 */
static status_t key_store_start(void);
/*!
 * @brief Check if the specific key type is a hardware key.
 *
 * @param type The type of the specific key.
 * @retval True Hardware key type.
 * @retval False Software key type or invalid key type.
 */
static inline bool key_store_is_hw_key(uint32_t type);
/*!
 * @brief Check if the specific key type is a software key.
 * @param type The type of the specific key.
 * @retval True Software key type.
 * @retval False hardware key type or invalid key type.
 */
static inline bool key_store_is_sw_key(uint32_t type);
/*!
 * @brief Check the key store is valid or not.
 *
 * @param store Key store pointer.
 * @retval True Valid key store.
 * @retval False Invalid key store.
 */
static bool key_store_is_valid(key_store_t *store);
/*!
 * @brief Get the key size from the key code.
 *
 * @param size Return the key size.
 * @param type Key tyep.
 * @retval kStatus_InvalidArgument Key type is not valid.
 * @retval kStatus_Fail Key code is not valid.
 * @retval kStatus_Success Operate successfully.
 */
static status_t key_store_get_key_size(uint32_t *size, uint32_t type);
/*******************************************************************************
 * Variables
 ******************************************************************************/
/*! @brief PUF key store. */
BL_ALIGN(KEY_STORE_ALIGNMENT) static key_store_t s_keyStore = { 0 };

/*! @brief PUF key store context. */
static key_store_context_t s_keyStoreContext = { 0 };
/*******************************************************************************
 * Codes
 ******************************************************************************/
status_t key_store_init(void)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Initialize PUF. */
        PUF_GetDefaultConfig(&s_keyStoreContext.config);
        status = PUF_Init(KEY_STORE_PUF, &s_keyStoreContext.config);
        if (status != kStatus_Success)
        {
            break;
        }

        /* Initialization is success, PUF is switched to Code state. */
        s_keyStoreContext.state = kPUF_State_Initialized;
    } while (0);

    return status;
}

status_t key_store_deinit(void)
{
    /* Clear the sensitive data in the key store. */
    memset(&s_keyStore, 0, sizeof(s_keyStore));
    /* Clear the key store context. */
    memset(&s_keyStoreContext, 0, sizeof(s_keyStoreContext));

    /* Avoid calling PUF_Deinit without PUF_Init executed by previous operation. The clock might be gated. */
    if (s_keyStoreContext.state != kPUF_State_Init)
    {
        /* De-initialize PUF. */
        PUF_Deinit(KEY_STORE_PUF, &s_keyStoreContext.config);
    }

    return kStatus_Success;
}

status_t key_store_enroll(void)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Key store and PUF engine are not initialized, perform initialization firstly. */
        if (s_keyStoreContext.state == kPUF_State_Init)
        {
            status = key_store_init();
            if (status != kStatus_Success)
            {
                break;
            }
        }

        /* Enroll operation can be executed only when PUF is at Cold state*/
        if (s_keyStoreContext.state != kPUF_State_Initialized)
        {
            status = kStatus_Fail;
            break;
        }

        /* A new key store will be generated, clear the key store. */
        memset(&s_keyStore, 0, sizeof(s_keyStore));

        /* Execute PUF Enroll command. */
        status = PUF_Enroll(KEY_STORE_PUF, (uint8_t *)&s_keyStore.actvCode, sizeof(s_keyStore.actvCode));
        if (status != kStatus_Success)
        {
            break;
        }

        /* Fill the key store data. */
        s_keyStore.header = KEY_STORE_HEADER;
        s_keyStore.version = KEY_STORE_VERSION;
        s_keyStore.flag = 0;
        s_keyStore.reserved = 0;

        /* After Enroll, PUF is switched to Enrolled state. */
        s_keyStoreContext.state = kPUF_State_Enrolled;
    } while (0);

    return status;
}

status_t key_store_set_key(uint8_t *key, uint32_t size, uint32_t type)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Check if key size is valid. */
        uint32_t maxHwKeySize = 0, minHwKeySize = 0;
        if (key_store_is_hw_key(type))
        {
            key_store_get_hw_key_range(type, &maxHwKeySize, &minHwKeySize);
        }
        else
        {
#if defined(BL_FEATURE_KEY_STORE_MAX_KEY_SIZE)
            maxHwKeySize = BL_FEATURE_KEY_STORE_MAX_KEY_SIZE;
#else
            maxHwKeySize = kPUF_KeySizeMax;
#endif
            minHwKeySize = kPUF_KeySizeMin;
        }

        if ((size < minHwKeySize) || (size > maxHwKeySize))
        {
            status = kStatus_InvalidArgument;
            break;
        }

        /* If PUF is not at Enrolled state or Started state, it has to be switched to Enrolled state or Started state
         * before executing Set Key command */
        if ((s_keyStoreContext.state != kPUF_State_Enrolled) && (s_keyStoreContext.state != kPUF_State_Started))
        {
            /* If key store is available, execute Start command, and goto Started state. Otherwise, return failure. Call
             * key_store_enroll() or key_store_set() before execute this function. */
            status = key_store_start();
            if (status != kStatus_Success)
            {
                break;
            }
        }

        /* Check if Set Key command is permitted. */
        if ((s_keyStoreContext.state == kPUF_State_Enrolled) || (s_keyStoreContext.state == kPUF_State_Started))
        {
            if (key != NULL)
            {
                /* switch the key sequnce to match the specific hardware. */
                key_store_switch_sequence(key, size, type);
            }

            switch (type)
            {
                case kKeyType_OtfadKEK:
                    if (key != NULL)
                    {
                        status = PUF_SetUserKey(KEY_STORE_PUF, KEY_STORE_PUF_HW_KEY_INDEX, key, size,
                                                (uint8_t *)&s_keyStore.otfadKey.keyCode,
                                                sizeof(s_keyStore.otfadKey.keyCode));
                    }
                    else
                    {
                        status = PUF_SetIntrinsicKey(KEY_STORE_PUF, KEY_STORE_PUF_HW_KEY_INDEX, size,
                                                     (uint8_t *)&s_keyStore.otfadKey.keyCode,
                                                     sizeof(s_keyStore.otfadKey.keyCode));
                    }

                    if (status != kStatus_Success)
                    {
                        /* Clear the senstive data if the operation is failed. */
                        memset(&s_keyStore.otfadKey, 0, sizeof(s_keyStore.otfadKey));
                    }
                    else
                    {
                        /* Mark the key code valid*/
                        s_keyStore.otfadKey.type = type;
                    }
                    break;
                default:
                    status = kStatus_InvalidArgument;
                    break;
            }
        }
        else
        {
            /* Set Key command is not permitted at current PUF state. Return failure. */
            status = kStatus_Fail;
            break;
        }
    } while (0);

    return status;
}

status_t key_store_get_sw_key(uint8_t *key, uint32_t *size, uint32_t type)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Check if argument is valid. */
        if (size == NULL)
        {
            status = kStatus_InvalidArgument;
            break;
        }

        /* Check if the key type is valid. */
        if (!key_store_is_sw_key(type))
        {
            status = kStatus_InvalidArgument;
            break;
        }

        /* If key buffer is a NULL pointer, return the key size back. */
        if (key == NULL)
        {
            status = key_store_get_key_size(size, type);
            break;
        }

        /* If PUF is not at Started state, it has to be switched to Started state before executing Get Key command */
        if (s_keyStoreContext.state != kPUF_State_Started)
        {
            status = key_store_start();
            if (status != kStatus_Success)
            {
                break;
            }
        }

        /* Check if Set Key command is permitted. */
        if (s_keyStoreContext.state == kPUF_State_Started)
        {
            switch (type)
            {
#if 0
/* Code example. Currently, no software key. */
                case kKeyType_OtfadKEK:
                    if (*size < s_keyStore.otfadKey.keyCode.header.B.size)
                    {
                        /* Key buffer is too small for the key. */
                        status = kStatus_Fail;
                        break;
                    }
                    if (s_keyStore.otfadKey.type != kKeyType_OtfadKEK)
                    {
                        /* Key code is not available. */
                        status = kStatus_Fail;
                        break;
                    }
                    status = PUF_GetKey(KEY_STORE_PUF, (uint8_t *)&s_keyStore.otfadKey.keyCode, sizeof(s_keyStore.otfadKey.keyCode), key, *size);
                    if (status != kStatus_Success)
                    {
                        /* Clear the senstive data if the operation is failed. */
                        memset(key, 0, *size);
                    }
                    break;
#endif
                default:
                    status = kStatus_InvalidArgument;
                    break;
            }
        }
        else
        {
            /* Get Key command is not permitted at current PUF state. Return failure. */
            status = kStatus_Fail;
            break;
        }
    } while (0);

    return status;
}

status_t key_store_write_nonvolatile(uint32_t memoryId)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Check if the key store is valid. */
        if (!key_store_is_valid(&s_keyStore))
        {
            break;
        }
        /* Get the position of the key store in the non-volatile memory. */
        uint32_t address = 0;
        status = get_key_store_nonvolatile_address(memoryId, &address);
        if (status != kStatus_Success)
        {
            break;
        }
        /* Write key store. */
        status = mem_write(address, sizeof(s_keyStore), (uint8_t *)&s_keyStore, memoryId);
        if (status != kStatus_Success)
        {
            break;
        }
        /* Flush data in the write buffer to the non-volatile memory. */
        status = mem_flush();
        if (status != kStatus_Success)
        {
            break;
        }

    } while (0);

    return status;
}

status_t key_store_read_nonvolatile(uint32_t memoryId)
{
    status_t status = kStatus_Fail;

    do
    {
        /* Get the position of the key store in the non-volatile memory. */
        uint32_t address = 0;
        status = get_key_store_nonvolatile_address(memoryId, &address);
        if (status != kStatus_Success)
        {
            break;
        }
        /* Read key store. */
        status = mem_read(address, sizeof(s_keyStore), (uint8_t *)&s_keyStore, memoryId);
        if (status != kStatus_Success)
        {
            break;
        }
        /* Check if the key store is valid. */
        if (!key_store_is_valid(&s_keyStore))
        {
            memset(&s_keyStore, 0, sizeof(s_keyStore));
            status = kStatus_Fail;
            break;
        }

        if (s_keyStoreContext.state != kPUF_State_Initialized)
        {
            /* A now store is available, reset PUF to Cold state. */
            status = key_store_init();
            break;
        }

        status = kStatus_Success;
    } while (0);

    return status;
}

status_t key_store_get(uint8_t **store, uint32_t *size)
{
    if ((store == NULL) || (size == NULL))
    {
        return kStatus_InvalidArgument;
    }

    /* Check if the key store is available. */
    if (!key_store_is_valid(&s_keyStore))
    {
        return kStatus_Fail;
    }

    *store = (uint8_t *)&s_keyStore;
    *size = sizeof(s_keyStore);

    return kStatus_Success;
}

status_t key_store_set(uint8_t *store, uint32_t size)
{
    /* Check if the input data size matches key store size. */
    if (size != sizeof(s_keyStore))
    {
        return kStatus_InvalidArgument;
    }
    /* Check if the input data is a valid key store. */
    if (!key_store_is_valid((key_store_t *)store))
    {
        return kStatus_Fail;
    }

    /* Overwrite the key store variable. */
    memcpy((void *)&s_keyStore, store, size);

    if (s_keyStoreContext.state != kPUF_State_Initialized)
    {
        /* A now store is available, initialize/reset PUF to Cold state. */
        return key_store_init();
    }

    return kStatus_Success;
}

static status_t key_store_start(void)
{
    status_t status = kStatus_Fail;

    do
    {
        if (s_keyStoreContext.state == kPUF_State_Started)
        {
            /* PUF is already at Started State. Skip the following operations.*/
            status = kStatus_Success;
            break;
        }

        if (s_keyStoreContext.state == kPUF_State_Initialized)
        {
            if (key_store_is_valid(&s_keyStore))
            {
                /* If key store is available, execute Start command. */
                status = PUF_Start(KEY_STORE_PUF, (const uint8_t *)&s_keyStore.actvCode, sizeof(s_keyStore.actvCode));
                if (status != kStatus_Success)
                {
                    break;
                }
                /* After executing Start command, PUF is switched to Started state. */
                s_keyStoreContext.state = kPUF_State_Started;
                break;
            }
            else
            {
                /* Key store is not available, return failure. */
                status = kStatus_Fail;
                break;
            }
        }
        else
        {
            status = kStatus_Fail;
            break;
        }
    } while (0);

    return status;
}

static bool key_store_is_valid(key_store_t *store)
{
    /* Check header tag. */
    if (store->header != KEY_STORE_HEADER)
    {
        return false;
    }
    /* Key store code should be backward compatible. */
    if (store->version > KEY_STORE_VERSION)
    {
        return false;
    }

    return true;
}

static inline bool key_store_is_sw_key(uint32_t type)
{
    /* Currently, no software key is supported in key store. */
    return false;
}

static inline bool key_store_is_hw_key(uint32_t type)
{
    bool result = false;

    switch (type)
    {
        case kKeyType_OtfadKEK:
            result = true;
            break;
        default:
            result = false;
            break;
    }

    return result;
}

static inline status_t key_store_get_key_size(uint32_t *size, uint32_t type)
{
    status_t status = kStatus_Success;

    switch (type)
    {
        case kKeyType_OtfadKEK:
            if (s_keyStore.otfadKey.type != kKeyType_OtfadKEK)
            {
                /* Key Code is not available. */
                *size = 0;
                status = kStatus_Fail;
            }
            else
            {
                *size = s_keyStore.otfadKey.keyCode.header.B.size;
            }
            break;
        default:
            status = kStatus_InvalidArgument;
            break;
    }

    return status;
}

#endif // #if BL_FEATURE_KEY_STORE_PUF
