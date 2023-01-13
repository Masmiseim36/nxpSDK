
/**
 *  \file smp_pl.c
 *
 *  This file contains the implementation for all platform specific
 *  extensions for Security Manager Protocol module.
 *
 *  The code here manages the Rank awarded to Device Database entries
 *  whenever one is "accessed" - either on completion of ACL establishment
 *  or Pairing.
 *
 *  The most recently used/accessed device is the one with Rank = 1.
 *  The least recently used/accessed device is the one with Rank = Max.
 *
 *  The API functions defined here can be modified to any extent, including
 *  the name & prototype of the functions, as these functions are called
 *  by applications only.
 *
 *  On the other hand, the internal functions (functions not starting with
 *  "BT_") are used by SMP core. Content of these functions can be modified
 *  to implement a different logic for storing ranks (or, for completely
 *  different use all together), however the function names and prototypes
 *  cannot be changed.
 *
 *  Important points to note:
 *  SMP core calls the internal functions for a Device Database entry which
 *  is guaranteed to be marked "USED". Hence, Ranks should be awarded and/or
 *  managed only for Device Database entries which are marked "USED" only.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */


/* ----------------------------------------- Header File Inclusion */
#include "smp_internal.h"
#include "smp_extern.h"

#ifdef SMP_HAVE_TBX_PL_ENC
#include "aes_cmac_tp_ext.h"
#endif /* SMP_HAVE_TBX_PL_ENC */

#ifdef SMP

/* Compilation switch to generate Constant SMP LESC OOB data */
/* NOTE : The oob data thus generated will be:
 *  OOB - 0x61, 0x35, 0x83, 0x15, 0xDD, 0xB0, 0xFE, 0xB4,
 *        0xFB, 0x0E, 0x61, 0x09, 0x11, 0x8D, 0x68, 0xC6
 */
#ifdef SMP_HAVE_TBX_PL_ECDH
/* #define SMP_LESC_CONST_OOB_VAL_SUPPORT */
#endif /* SMP_HAVE_TBX_PL_ECDH */

/* ----------------------------------------- External Global Variables */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
/** TODO: provide proper compilation flags to all types of IO_CAP and oob. */

/** Note : All the byte stream values are stored in Little endian order */

/** Input/Output capability of the device */
DECL_STATIC UCHAR smp_io_capability_pl;

/**
 * Key distribution flag
 *      Bit 0: Local Encryption information
 *      Bit 1: Local Identity information
 *      Bit 2: Local Signature Key
 *      Bit 3: Reserved
 *      Bit 4: Remote Encryption information
 *      Bit 5: Remote Identity information
 *      Bit 6: Remote Signature Key
 *      Bit 7: Reserved
 */
DECL_STATIC UCHAR smp_key_distribution_pl;

/** Max Supported encryption key size */
DECL_STATIC UCHAR smp_encrytion_key_size_pl;

/** Platform security requirement */
DECL_STATIC UCHAR smp_mitm_policy_pl;

#ifdef SMP_LESC
/** Platform pairing mode requirement */
DECL_STATIC UCHAR smp_lesc_policy_pl;

/** Platform pairing KeyPress Notification Flag */
DECL_STATIC UCHAR smp_lesc_key_press_ntf_pl;
#endif /* SMP_LESC */

#ifdef SMP_HAVE_OOB_SUPPORT
/** Platform device list */
DECL_STATIC SMP_DEVICE_PL smp_pl_device_list[SMP_PL_DEVICE_LIST_SIZE];

#ifdef SMP_LESC
/* LESC OOB Data */
DECL_STATIC SMP_LESC_OOB_DATA_PL smp_lesc_oob_data;

/* LESC OOB data generation complete handler */
DECL_STATIC SMP_LESC_OOB_GEN_COMPLETE_CB smp_lesc_oob_gen_handler;
#endif /* SMP_LESC */
#endif /* SMP_HAVE_OOB_SUPPORT */

#ifndef ENABLE_BACI_INF
/** Key Distribution Information */
DECL_STATIC SMP_KEY_DIST key_distribution_info_pl =
{
    /* LTK */
    {0xD6U, 0x93U, 0xE8U, 0xA4U, 0x23U, 0x55U, 0x48U, 0x99U,
     0x1DU, 0x77U, 0x61U, 0xE6U, 0x63U, 0x2BU, 0x10U, 0x8EU},

    /* EDIV and RAND*/
    {0x99U, 0x1FU, 0x26U, 0x1EU, 0xF6U, 0x09U, 0x97U, 0x2EU, 0xADU, 0x7EU},

    /* IRK */
    {0x0AU, 0x2DU, 0xF4U, 0x65U, 0xE3U, 0xBDU, 0x7BU, 0x49U,
     0x1EU, 0xB4U, 0xC0U, 0x95U, 0x95U, 0x13U, 0x46U, 0x73U},

    /* Type and BD Addr */
    {0x00U, 0x00U, 0xC0U, 0xFFU, 0xEEU, 0xDEU, 0xDAU},

    /* CSRK */
    {0x90U, 0xD5U, 0x06U, 0x95U, 0x92U, 0xEDU, 0x91U, 0xD7U,
     0xA8U, 0x9EU, 0x2CU, 0xDCU, 0x4AU, 0x93U, 0x5BU, 0xF9U}
};

#ifdef SMP_PL_ENABLE_KEY_GENERATION

#define KEY_STATE_IN_LTK_GEN     0x01U
#define KEY_STATE_IN_CSRK_GEN    0x02U
#define KEY_STATE_IN_IRK_GEN     0x04U
#define KEY_STATE_IN_DHK_GEN     0x08U
#define KEY_STATE_IN_EDIV_GEN    0x10U

DECL_STATIC UCHAR er_pl[16U] = {0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U,
                                0x08U, 0x09U, 0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU, 0x0FU};

DECL_STATIC UCHAR ir_pl[16U] = {0x0FU, 0x0EU, 0x0DU, 0x0CU, 0x0BU, 0x0AU, 0x09U, 0x08U,
                                0x07U, 0x06U, 0x05U, 0x04U, 0x03U, 0x02U, 0x01U, 0x00U};

DECL_STATIC UINT16 div_pl;

DECL_STATIC UCHAR dhk_pl[16U];
DECL_STATIC UCHAR * rnd_pl = &key_distribution_info_pl.mid_info[2U];

DECL_STATIC UINT16 ediv_pl;
DECL_STATIC UINT16 div_y;

DECL_STATIC UCHAR * ltk_pl = key_distribution_info_pl.enc_info;
DECL_STATIC UCHAR * csrk_pl = key_distribution_info_pl.sign_info;
DECL_STATIC UCHAR * irk_pl = key_distribution_info_pl.id_info;

DECL_STATIC UCHAR key_gen_state = 0x00U;
DECL_STATIC UCHAR auto_generate = 0x00U;

DECL_STATIC API_RESULT smp_func_d1 (UCHAR * k, UINT16 d, UINT16 r);
DECL_STATIC API_RESULT smp_func_dm (UCHAR * k, UCHAR * r);

#endif /* SMP_PL_ENABLE_KEY_GENERATION */

#endif /* ENABLE_BACI_INF */

/** Rank counter */
SMP_DEVICE_ATTR_PL smp_rank_count_pl;

#ifdef SMP_LESC
#ifdef SMP_HAVE_TBX_PL_ECDH

SMP_PL_ECDH_PRIV_KEY smp_ecdh_priv_key_pl;
SMP_PL_ECDH_DHKEY    smp_ecdh_dhkey_pl;
UCHAR                smp_ecdh_pub_key_pl[SMP_LESC_PUBLIC_KEY_SIZE];

#ifdef SMP_LESC_CONST_OOB_VAL_SUPPORT
DECL_CONST DECL_STATIC UCHAR smp_ecdh_oob_priv_key_pl[] =
{
    0x56U, 0x9AU, 0x68U, 0xF7U, 0xB9U, 0xAAU, 0x83U, 0x58U,
    0xB7U, 0xABU, 0x56U, 0x34U, 0x1EU, 0xD8U, 0x3BU, 0x67U,
    0x3CU, 0xE6U, 0xA5U, 0xF6U, 0xF9U, 0xF3U, 0xDDU, 0x20U,
    0x6BU, 0xB2U, 0x2DU, 0x76U, 0x31U, 0x78U, 0x10U, 0x54U
};

DECL_CONST DECL_STATIC UCHAR smp_ecdh_oob_pub_key_pl[] =
{
    0xF2U, 0x00U, 0xD3U, 0x55U, 0xE9U, 0x8BU, 0x7FU, 0xF8U,
    0x7AU, 0x01U, 0xCDU, 0xE2U, 0x2FU, 0x78U, 0x0EU, 0x14U,
    0xC8U, 0x35U, 0xC3U, 0x3FU, 0x76U, 0x02U, 0xC0U, 0x76U,
    0x4EU, 0x60U, 0xB3U, 0x1CU, 0xF3U, 0xF7U, 0x67U, 0x7DU,
    0xCBU, 0x21U, 0x92U, 0x10U, 0x73U, 0x34U, 0x48U, 0x88U,
    0x00U, 0x62U, 0xE1U, 0xF3U, 0xF5U, 0x8EU, 0xF1U, 0x7BU,
    0x19U, 0x8CU, 0x67U, 0xC6U, 0xD4U, 0x0BU, 0xD6U, 0xFCU,
    0x7DU, 0x6EU, 0xABU, 0x37U, 0xBAU, 0x5DU, 0x71U, 0x09U
};
#endif /* SMP_LESC_CONST_OOB_VAL_SUPPORT */
#endif /* SMP_HAVE_TBX_PL_ECDH */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
/* LESC LK LTK generation Data struct */
DECL_STATIC SMP_LESC_LK_LTK_GEN_PL smp_lesc_txp_keys;

/* LESC LK LTK key generation complete handler */
DECL_STATIC SMP_LESC_LK_LTK_GEN_COMPLETE_CB smp_lesc_lk_ltk_gen_handler;
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

/** Platform mode for Generated Passkey Updation */
DECL_STATIC UCHAR smp_passkey_revamp_mode_pl;

/* ----------------------------------------- Functions */

/**
 * \brief To make platform initializations for SMP
 *
 * \par Description:
 *      This internal platform API is used to make platform
 *      specific initializations during the initialization of
 *      internal SMP module
 *
 * \param void
 *
 * \return void
 */
void smp_init_pl (void)
{
#ifdef SMP_HAVE_OOB_SUPPORT
    UCHAR i;

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
    for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
    i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
    {
        /* Initialize the device entuty */
        BT_mem_set (&smp_pl_device_list[i], 0x00, sizeof (SMP_DEVICE_PL));
    }

#ifdef SMP_LESC
    /* Initialize state to IDLE */
    SMP_LESC_OOB_SET_STATE(&smp_lesc_oob_data, SMP_OOB_STATE_IDLE);
#endif /* SMP_LESC */
#endif /* SMP_HAVE_OOB_SUPPORT */

    /* Initialize platform constants */
    smp_io_capability_pl = SMP_DEFAULT_IO_CAPABILITY;
    smp_key_distribution_pl = 0x77U;
    smp_encrytion_key_size_pl = SMP_MAX_ENCRYPTION_KEY_SIZE_PL;

    /* Initialize platform rank count */
    smp_rank_count_pl = 0U;

    /* Initialize to strict MITM requirements by platform */
    smp_mitm_policy_pl = SMP_PL_SEC_MITM_STRICT;

#ifdef SMP_LESC
    /* Initialize to Best Effort LESC requirements by platform */
    smp_lesc_policy_pl = SMP_PL_LESC_BEST_EFFORT;

    /* Initiatlize KeyPress notification to Reset by platform */
    smp_lesc_key_press_ntf_pl = SMP_PL_LESC_KEY_PRESS_NTF_RESET;
#endif /* SMP_LESC */

    /* Initializing Passkey Updation Mode */
    smp_passkey_revamp_mode_pl = 0x01U;
}


#ifdef SMP_HAVE_OOB_SUPPORT
/**
 * \brief Add device to platform database
 *
 * \par Description:
 *      This function adds the given device to the platform database
 *
 * \param [in] bd_addr  Peer bluetooth device address
 *
 * \return API_SUCCESS or Error Code stating the failure reason
 */
API_RESULT BT_smp_add_device_pl (/* IN */ BT_DEVICE_ADDR * bd_addr)
{
    UCHAR i;
    API_RESULT retval;

    /* Init */
    retval = API_FAILURE;

    /* Check for a free entity */
#if (SMP_PL_DEVICE_LIST_SIZE > 1)
    for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
    i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
    {
        if (0x00U == smp_pl_device_list[i].used)
        {
            BT_COPY_BD_ADDR_AND_TYPE (&smp_pl_device_list[i].bd_addr, bd_addr);

            smp_pl_device_list[i].used = 0x01U;

            retval = API_SUCCESS; /* return API_SUCCESS; */

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
            break;
#endif /* SMP_PL_DEVICE_LIST_SIZE */
        }
    }

    return retval;
}


/**
 * \brief Delete device from platform database
 *
 * \par Description:
 *      This function removes the given device from the platform database
 *
 * \param [in] bd_addr  Peer bluetooth device address
 *
 * \return API_SUCCESS or Error Code stating the failure reason
 */
API_RESULT BT_smp_remove_device_pl (/* IN */ BT_DEVICE_ADDR * bd_addr)
{
    UCHAR i;
    API_RESULT retval;

    /* Init */
    retval = API_FAILURE;

    /* Look up platform database to check for the device */
#if (SMP_PL_DEVICE_LIST_SIZE > 1)
    for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
    i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
    {
        if (BT_TRUE ==
            BT_COMPARE_BD_ADDR_AND_TYPE (bd_addr, &smp_pl_device_list[i].bd_addr))
        {
            smp_pl_device_list[i].used = 0x00U;

            retval = API_SUCCESS; /* return API_SUCCESS; */

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
            break;
#endif /* SMP_PL_DEVICE_LIST_SIZE */
        }
    }

    return retval;
}
#endif /* SMP_HAVE_OOB_SUPPORT */

/**
 * \brief To get local platform capabilities
 *
 * \par Description:
 *      This API retrieves local platform IO capabilities,
 *      key distribution setting and maximum encryption key size.
 *
 * \param [out] iocap  Pointer to IO Capability supported
 * \param [out] keys  Pointer to key distribution setting flag
 * \param [out] ekey_size  Pointer to max. encryption size
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_local_capability_pl
           (
               UCHAR  * iocap,
               UCHAR  * keys,
               UCHAR  * ekey_size
           )
{
    *iocap = smp_io_capability_pl;
    *keys = smp_key_distribution_pl;
    *ekey_size = smp_encrytion_key_size_pl;

#ifdef BT_SECURITY_VU_VALIDATION
    if (BT_SECURITY_VU_BLURTOOTH == BT_security_vu_get())
    {
        *ekey_size = (smp_encrytion_key_size_pl - 2U);
    }
#endif /* BT_SECURITY_VU_VALIDATION */

    return API_SUCCESS;
}


#ifdef SMP_HAVE_OOB_SUPPORT
/**
 * \brief To get OOB availability for a peer device
 *
 * \par Description:
 *      This API retrieves OOB presence flag for the given peer device.
 *      This is used by SMP, during authentication procedure.
 *
 * \param [in] di  Peer device for which the OOB availability to be check.
 * \param [out] oob  Pointer to OOB data presence flag.
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
void smp_get_peer_oob_availability_pl
     (
         /* IN */  SMP_BD_HANDLE * di,
         /* OUT */ UCHAR         * oob
     )
{
    API_RESULT retval;
    BT_DEVICE_ADDR bd_addr;
    UCHAR i;

    /* Initialize OOB flag */
    *oob = 0x00U;

    /* Get the device address for the given peer handle */
    retval = BT_smp_get_bd_addr (di, &bd_addr);

    if (API_SUCCESS == retval)
    {
        /* Look up platform database to check OOB support for the device */
#if (SMP_PL_DEVICE_LIST_SIZE > 1)
        for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
        i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
        {
            if ((0x01U == smp_pl_device_list[i].used) &&
                (BT_TRUE ==
                BT_COMPARE_BD_ADDR_AND_TYPE(&bd_addr, &smp_pl_device_list[i].bd_addr)))
            {
                /* Return the OOB availability present */
                *oob = smp_pl_device_list[i].oob_flag;

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
                break; /* return; */
#endif /* #if (SMP_PL_DEVICE_LIST_SIZE > 1) */
            }
        }
    }
}

#ifdef SMP_SAVE_REMOTE_IOCAP
API_RESULT BT_smp_get_remote_iocaps_pl
           (
               /* IN */  BT_DEVICE_ADDR * bd_addr,
               /* OUT */ SMP_IOCAPS     * iocaps
           )
{
    API_RESULT retval;
    SMP_BD_HANDLE bd_handle;
    UCHAR di;

    /* Initialize OOB flag */
    BT_mem_set(iocaps, 0x00, sizeof(SMP_IOCAPS));

    /* Get the device address for the given peer handle */
    retval = BT_smp_get_bd_handle(bd_addr, &bd_handle);

    if (API_SUCCESS == retval)
    {
        /* Lock SMP */
        smp_lock();

        /* Search device index */
        di = smp_search_device(&bd_handle, SMP_L2CAP_INVALID_SIG_ID);

        /* If device not found in database */
        if (SMP_MAX_DEVICES == di)
        {
            SMP_ERR(
            "[SMP] Device not found in the device database.\n");

            /* Return error */
            retval = SMP_NO_DEVICE_ENTITY;
        }
        else
        {
            SMP_TRC(
            "[SMP] Device found in SMP DB, Get the IOCaps.\n");

            *iocaps = smp_devices[di].db.iocaps;
        }

        /* Unlock */
        smp_unlock();
    }

    return retval;
}
#endif /* SMP_SAVE_REMOTE_IOCAP */

/**
 * \brief To retrieve the OOB Temporary Key of a peer device.
 *
 * \par Description:
 *      This API retrieves OOB Temporary Key of a peer device.
 *      This is used by SMP, during authentication procedure.
 *
 * \param [in] di  Peer device for which the OOB Temporary Key is requested.
 * \param [out] temp_key  Pointer to hold 16 Octet, OOB Temporary Key
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT smp_get_oob_tk_pl
           (
               /* IN */  SMP_BD_HANDLE * di,
               /* OUT */ UCHAR         * temp_key
           )
{
    API_RESULT retval;
    BT_DEVICE_ADDR bd_addr;
    UCHAR i;

    if (NULL == temp_key)
    {
        retval = SMP_INVALID_PARAMETERS; /* return SMP_INVALID_PARAMETERS; */
    }
    else
    {
        /* Get the device address for the given peer handle */
        retval = BT_smp_get_bd_addr(di, &bd_addr);
        if (API_SUCCESS == retval)
        {
            /* Init */
            retval = API_FAILURE;

            /* Look up platform database to check OOB support for the device */
#if (SMP_PL_DEVICE_LIST_SIZE > 1)
            for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
            i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
            {
                if ((0x01U == smp_pl_device_list[i].used) &&
                    (BT_TRUE ==
                    BT_COMPARE_BD_ADDR_AND_TYPE(&bd_addr, &smp_pl_device_list[i].bd_addr)))
                {
                    /* Return the OOB Temp Key */
                    BT_mem_copy(temp_key, smp_pl_device_list[i].tk, SMP_OOB_TEMP_KEY_SIZE);

                    retval = API_SUCCESS;

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
                    break;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
                }
            }
        }
    }

    return retval;
}

#ifdef SMP_LESC
void smp_get_local_oob_public_key_pl( /* OUT */ UCHAR * l_pub_key)
{
    if (NULL != l_pub_key)
    {
        BT_mem_copy(l_pub_key,smp_lesc_oob_data.pkey,SMP_OOB_PUBLIC_KEY_SIZE);
    }
}

void smp_get_lesc_oob_l_rand_pl( /* OUT */ UCHAR * l_rand)
{
    if (NULL != l_rand)
    {
        BT_mem_copy(l_rand,smp_lesc_oob_data.rand,SMP_OOB_RANDOM_VAL_SIZE);
    }
}

void smp_get_lesc_oob_l_cnf_val_pl( /* OUT */ UCHAR * l_cnf_val)
{
    if (NULL != l_cnf_val)
    {
        BT_mem_copy(l_cnf_val,smp_lesc_oob_data.cnf_val,SMP_OOB_CONFIRM_VAL_SIZE);
    }
}

API_RESULT smp_get_lesc_oob_pl
           (
               /* IN */  SMP_BD_HANDLE * di,
               /* OUT */ UCHAR         * r_conf_val,
               /* OUT */ UCHAR         * r_rand
           )
{
    API_RESULT retval;
    BT_DEVICE_ADDR bd_addr;
    UCHAR i;

    if ((NULL == r_conf_val) && (NULL == r_rand))
    {
        retval = SMP_INVALID_PARAMETERS; /* return SMP_INVALID_PARAMETERS; */
    }
    else
    {
        /* Get the device address for the given peer handle */
        retval = BT_smp_get_bd_addr(di, &bd_addr);
        if (API_SUCCESS == retval)
        {
            /* Init */
            retval = API_FAILURE;

            /* Look up platform database to check OOB support for the device */
#if (SMP_PL_DEVICE_LIST_SIZE > 1)
            for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
            i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
            {
                if ((0x01U == smp_pl_device_list[i].used) &&
                    (BT_TRUE ==
                    BT_COMPARE_BD_ADDR_AND_TYPE(&bd_addr, &smp_pl_device_list[i].bd_addr)))
                {
                    /* Return the Remote LESC OOB Confirm Value if not NULL */
                    if (NULL != r_conf_val)
                    {
                        BT_mem_copy(r_conf_val, smp_pl_device_list[i].cnf_val, SMP_OOB_CONFIRM_VAL_SIZE);
                    }

                    /* Return the Remote LESC OOB RAND Value if not NULL */
                    if (NULL != r_rand)
                    {
                        BT_mem_copy(r_rand, smp_pl_device_list[i].rand, SMP_OOB_RANDOM_VAL_SIZE);
                    }

                    retval = API_SUCCESS;

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
                    break;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
                }
            }
        }
    }

    return retval; /* API_FAILURE; */
}
#endif /* SMP_LESC */

#endif /* SMP_HAVE_OOB_SUPPORT */


#ifndef ENABLE_BACI_INF
/**
 * \brief To get local keys for exchange
 *
 * \par Description:
 *      This API retrieves local keys to be exchanged with peer.
 *
 * \param [out] key_info  Pointer to \ref SMP_KEY_DIST
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_key_exchange_info_pl(SMP_KEY_DIST ** key_info)
{
    *key_info = &key_distribution_info_pl;

    return API_SUCCESS;
}
#endif /* ENABLE_BACI_INF */

#ifdef SMP_HAVE_TBX_PL_ENC

API_RESULT BT_smp_128B_encrypt_pl (UCHAR * key, UCHAR * data, UCHAR * encout)
{
    return smp_tbx_128B_encrypt(key,data,encout);
}

void smp_assign_encrypt_cmd_counter_pl (UCHAR * cmd_cntr)
{
    DECL_STATIC UCHAR smp_encrypt_cmd_counter_pl = 0x00;

    /* On the buff is not null */
    if(NULL != cmd_cntr)
    {
        /* Copy encrypt counter */
        *cmd_cntr = smp_encrypt_cmd_counter_pl;
    }

    (0xFF == smp_encrypt_cmd_counter_pl) ? smp_encrypt_cmd_counter_pl = 0 :\
                                           smp_encrypt_cmd_counter_pl++ ;
}

/**
 *  \fn BT_smp_encrypt_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param key
 *  \param data
 *  \param size
 *  \param eout
 *
 *  \return
 */
API_RESULT BT_smp_encrypt_pl
           (
               UCHAR  * key,
               UCHAR  * text,
               UINT16   len,
               UCHAR  * cmd_counter
           )
{
#if 0
    API_RESULT retval;
    UCHAR counter;

    /* Invoke controller to encrypt the data */
    retval = BT_hci_le_encrypt
             (
                 key,
                 text,
                 &counter
             );

    if (API_SUCCESS == retval)
    {
        /* Set Retval to Pending to wait for event */
        retval = SMP_OPERATION_IN_PROGRESS;
    }

    return retval;
#else
    void * ctx;
    static UCHAR enc_out[0x10U];

    /* SMP Update Command Counter */
    smp_assign_encrypt_cmd_counter_pl(cmd_counter);

    /* Convert the incoming Little Endian input to Big Endian */
    smp_reverse_octet_stream_pl (key, 0x10U);
    smp_reverse_octet_stream_pl (text, 0x10U);

    ctx = aes_encrypt_init (key, 0x10U);
    aes_encrypt (ctx, text, enc_out);
    aes_encrypt_deinit (ctx);

    /* Convert the outgoing Big Endian output to Little Endian */
    smp_reverse_octet_stream_pl (enc_out, 0x10U);
    smp_reverse_octet_stream_pl (key, 0x10U);
    smp_reverse_octet_stream_pl (text, 0x10U);

    /* Enqueue to write Task */
#ifdef SMP_HAVE_TBX_CMD_WT_BH
    return smp_enqueue_cmd_op_to_wt(enc_out,0x00U,0x10U,SMP_TBX_ENCRYPT_CMD,*cmd_counter);
#else /* SMP_HAVE_TBX_CMD_WT_BH */
    return API_FAILURE;
#endif /* SMP_HAVE_TBX_CMD_WT_BH */
#endif /* 0 */
}

void smp_reverse_octet_stream_pl(UCHAR * buffer, UINT16 buffer_len)
{
    UINT16 index;
    UCHAR  temp;

    for (index = 0U; index < (buffer_len/2U); index ++)
    {
        temp = buffer[index];
        buffer[index] = buffer[buffer_len - (index + 1U)];
        buffer[buffer_len - (index + 1U)] = temp;
    }
}
#endif /* SMP_HAVE_TBX_PL_ENC */

#ifdef SMP_HAVE_TBX_PL_RAND
/**
 *  \fn BT_smp_rand_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param n_rand
 *  \param size
 *
 *  \return
 */
API_RESULT BT_smp_rand_pl (UCHAR * n_rand, UINT16 size)
{
    API_RESULT retval;

    /* Invoke controller to generate random number */
    retval = BT_hci_le_rand();
    if (API_SUCCESS == retval)
    {
        /* Set Retval to Pending to wait for event */
        retval = SMP_OPERATION_IN_PROGRESS;
    }

    return retval;
}
#endif /* SMP_HAVE_TBX_PL_RAND */

#ifdef SMP_LESC
#ifdef SMP_HAVE_TBX_PL_ECDH
API_RESULT smp_gen_public_key_pl (void)
{
#ifdef SMP_LESC_CONST_OOB_VAL_SUPPORT
    /* Populate the Private and Public Key with the CONST values */
    BT_mem_copy(smp_ecdh_priv_key_pl, smp_ecdh_oob_priv_key_pl, sizeof(smp_ecdh_oob_priv_key_pl));
    BT_mem_copy(smp_ecdh_pub_key_pl, smp_ecdh_oob_pub_key_pl, sizeof(smp_ecdh_oob_pub_key_pl));

#else /* SMP_LESC_CONST_OOB_VAL_SUPPORT */

    UCHAR ret;

    /* Temporary public key structure */
    SMP_PL_ECDH_PUB_KEY pub_key;

    BT_mem_set (pub_key.x, 0, SMP_LESC_PUBLIC_KEY_X_SIZE);
    BT_mem_set (pub_key.y, 0, SMP_LESC_PUBLIC_KEY_Y_SIZE);

    ssp_init();

    BT_LOOP_FOREVER()
    {
       ret = ssp_get_ecdh_keypair(&smp_ecdh_priv_key_pl, &pub_key);

       if(0U != ret)
       {
            break;
       }
    }

    /* Copy the Local Public Key from the Temporary Structure */
    BT_mem_copy(smp_ecdh_pub_key_pl, &pub_key.x, SMP_LESC_PUBLIC_KEY_X_SIZE);
    BT_mem_copy(&smp_ecdh_pub_key_pl[SMP_LESC_PUBLIC_KEY_X_SIZE], &pub_key.y, SMP_LESC_PUBLIC_KEY_Y_SIZE);

    ssp_shutdown();
#endif /* SMP_LESC_CONST_OOB_VAL_SUPPORT */

    SMP_TRC(
    "[SMP-PL] Generated ECDH Private Key is: \n");

    SMP_debug_dump_bytes(smp_ecdh_priv_key_pl,sizeof(smp_ecdh_priv_key_pl));

    SMP_TRC(
    "[SMP-PL] Generated ECDH Public Key X&Y coordinate is: \n");

    SMP_debug_dump_bytes(smp_ecdh_pub_key_pl,sizeof(smp_ecdh_pub_key_pl));

    /* Enqueue the Generated Public Key to the Write Tasak */
#ifdef SMP_HAVE_TBX_CMD_WT_BH
    return smp_enqueue_cmd_op_to_wt
           (
               smp_ecdh_pub_key_pl,
               0x00U,
               SMP_LESC_PUBLIC_KEY_SIZE,
               SMP_TBX_READ_L_P256_PUB_KEY_CMD,
               0x00U
            );
#else /* SMP_HAVE_TBX_CMD_WT_BH */
    return API_FAILURE;
#endif /* SMP_HAVE_TBX_CMD_WT_BH */
}

API_RESULT smp_gen_dh_key_pl (UCHAR * r_pub_key)
{
    UCHAR ret;
    SMP_PL_ECDH_PUB_KEY peer_ecdh_pub_key;

    /* Initialize ECDH Module */
    ssp_init();

    /* Populate the temporary Public Key Structure */
    BT_mem_copy(&peer_ecdh_pub_key.x,r_pub_key, SMP_LESC_PUBLIC_KEY_X_SIZE);
    BT_mem_copy(&peer_ecdh_pub_key.y,&r_pub_key[SMP_LESC_PUBLIC_KEY_X_SIZE], SMP_LESC_PUBLIC_KEY_Y_SIZE);

    BT_LOOP_FOREVER()
    {
       ret = ssp_get_dhkey(&smp_ecdh_priv_key_pl, &peer_ecdh_pub_key, &smp_ecdh_dhkey_pl);

       if(0U != ret)
       {
            break;
       }
    }

    ssp_shutdown();

    /* If ret = 1 assign status as 0x00, else status is Invalid HCI Command Parameters */
    ret = (((1U == ret) ? (UCHAR)API_SUCCESS : (UCHAR)HC_INVALID_HCI_COMMAND_PARAMETERS));

    SMP_TRC(
    "[SMP-PL] Generated DHKey with retval 0x%02X: \n", ret);

    SMP_debug_dump_bytes(smp_ecdh_dhkey_pl,sizeof(smp_ecdh_dhkey_pl));

    /* Enqueue the Generated ECDH Key to the Write Tasak */
#ifdef SMP_HAVE_TBX_CMD_WT_BH
    return smp_enqueue_cmd_op_to_wt
           (
               smp_ecdh_dhkey_pl,
               ret,
               SMP_LESC_DHKEY_SIZE,
               SMP_TBX_GEN_DHKEY_CMD,
               0x00U
            );
#else /* SMP_HAVE_TBX_CMD_WT_BH */
    return API_FAILURE;
#endif /* SMP_HAVE_TBX_CMD_WT_BH */
}
#endif /* SMP_HAVE_TBX_PL_ECDH */
#endif /* SMP_LESC */

#ifndef ENABLE_BACI_INF
#ifdef SMP_SUPPORT_UTIL_APIS
/**
 * \brief To mark a device untrusted
 *
 * \par Description:
 *      This API marks the device specified by the handle as untrusted.
 *
 * \param [in] bd_handle  SMP BD Handle of peer device
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_mark_device_untrusted_pl (/* IN */ SMP_BD_HANDLE * bd_handle)
{
    UCHAR di;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Validate parameters */
    if(0U == (SMP_IS_VALID_BD_HANDLE(bd_handle)))
    {
        SMP_ERR(
        "[SMP] Invalid paramter.\n");

        /* Return error */
        retval = SMP_INVALID_PARAMETERS; /* return SMP_INVALID_PARAMETERS; */
    }
    else
    {
        SMP_TRC(
        "[SMP] Mark device as untrusted. BD Handle = %02X\n", *bd_handle);

        /* Lock SMP */
        smp_lock();

        /* Search device index */
        di = smp_search_device (bd_handle, SMP_L2CAP_INVALID_SIG_ID);

        /* If device not found in database */
        if(SMP_MAX_DEVICES == di)
        {
            SMP_ERR(
            "[SMP] Device not found in the device database.\n");

            /* Unlock SMP */
            /* smp_unlock(); */

            /* Return error */
            retval = SMP_NO_DEVICE_ENTITY; /* return SMP_NO_DEVICE_ENTITY; */
        }
        else
        {
            if (SMP_ENTITY_BOND_ON !=
                SMP_GET_BONDING_STATE(smp_devices[di].dev_attr))
            {
                SMP_ERR(
                "[SMP] Device not trusted.\n");

                /* Unlock SMP */
                /* smp_unlock(); */

                /* Return error */
                retval = SMP_INCORRECT_STATE; /* return SMP_INCORRECT_STATE; */
            }

            /* Disallowing Bond deletion when link is ENCRYPTED */
            else if (SMP_ENTITY_ENC_ON == SMP_GET_ENCRYPTION_STATE(smp_devices[di].dev_attr))
            {
                SMP_ERR(
                "[SMP] Devices already share an encrypted Link.\n");

                /* Unlock SMP */
                /* smp_unlock(); */

                /* Return error */
                retval = SMP_INCORRECT_STATE; /* return SMP_INCORRECT_STATE; */
            }
            else
            {
                /* Mark device untrusted */
                SMP_SET_BONDING_OFF(smp_devices[di].dev_attr);

                /* Set device valid flag */
                (BT_IGNORE_RETURN_VALUE)smp_set_device_validity(di);

                /* If not connected, Reset the device entity from SMP and other Modules */
                if (SMP_ENTITY_CONNECTED != SMP_GET_ENTITY_CONNECTION(smp_devices[di].state))
                {
                    smp_reset_device_entity(di, BT_TRUE);
                }
                else
                {
                    /* Update platform delete */
                    smp_update_device_attr_pl(SMP_DEVICE_ATTR_PL_DELETE, di);
                }
            }
        }

        /* Unlock SMP */
        smp_unlock();
    }

    /* Return Success */
    return retval;
}
#endif /* SMP_SUPPORT_UTIL_APIS */

#ifdef SMP_SLAVE
/**
 * \brief To get the long term key
 *
 * \par Description:
 *      This API retrieves the long term key for a given combination of RAND and
 *      EDIV.
 *
 * \param [in] rnd  8 octet Random Number
 * \param [in] ediv  Encrypted Diversifier
 * \param [out] ltk  Pointer to long term key to be retrieved
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_long_term_key_pl
           (
               /* IN */  UCHAR  * rnd,
               /* IN */  UINT16   ediv,
               /* OUT */ UCHAR  * ltk
           )
{
#ifdef SMP_PL_ENABLE_KEY_GENERATION
    UINT16 div;
    API_RESULT retval;

    /* Init */
    retval = API_FAILURE;

    /* Get the DIV from EDIV */
    div = div_y ^ ediv;

    /* Check for Rand and Div mapping */
    if ((div == div_pl) &&
        (0 == (BT_mem_cmp(rnd, rnd_pl, SMP_RAND_64B_SIZE))))
    {
        /* Copy the Encryption Key pointer */
        BT_mem_copy(ltk, key_distribution_info_pl.enc_info, SMP_LTK_SIZE);

        retval = API_SUCCESS; /* return API_SUCCESS; */
    }

    return retval; /*  return API_FAILURE; */
#else /* SMP_PL_ENABLE_KEY_GENERATION */

    BT_IGNORE_UNUSED_PARAM(rnd);
    BT_IGNORE_UNUSED_PARAM(ediv);

    /* NOTE: EDIV and RAND mapping is not validated */
    /* Copy the Encryption Key pointer */
    BT_mem_copy(ltk, key_distribution_info_pl.enc_info, SMP_LTK_SIZE);

    return API_SUCCESS;
#endif /* SMP_PL_ENABLE_KEY_GENERATION */
}
#endif /* SMP_SLAVE */
#endif /* ENABLE_BACI_INF */

/**
 * \brief To set local IO capability
 *
 * \par Description:
 *      This API sets the local platform IO capability that will be used during
 *      SMP authentication procedure.
 *
 * \param [in] io_cap  IO Capability as in \ref smp_pl_iocaps
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_io_cap_pl (/* IN */ UCHAR io_cap)
{
    API_RESULT retval;

    SMP_TRC(
    "[SMP] Recived Set IO Cap. io_cap = 0x%02X\n",
    io_cap);

#if (SMP_DEFAULT_IO_CAPABILITY != SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT)
#ifndef SMP_NO_PARAM_CHECK
    /* On invalid IO Cap value */
    if(io_cap > SMP_IO_CAPABILITY_KEYBOARD_DISPLAY)
    {
        SMP_ERR(
        "[SMP] Invalid paramter.\n");

        /* Return error */
        retval = SMP_INVALID_PARAMETERS;
    }
    else
#endif /* SMP_NO_PARAM_CHECK */
    {
        /* Set IO Cap flag */
        smp_io_capability_pl = io_cap;

        /* Return Success */
        retval = API_SUCCESS;
    }
#else /* (SMP_DEFAULT_IO_CAPABILITY != SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT) */
    if (SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT != io_cap)
    {
        SMP_ERR(
        "[SMP] Compile Time IOcap Default set to SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT\n");

        SMP_ERR(
        "[SMP] Attempt to change IOcap Value to 0x%02X!!\n",
        io_cap);

        retval = API_FAILURE;
    }
    else
    {
        retval = API_SUCCESS;
    }
#endif /* (SMP_DEFAULT_IO_CAPABILITY != SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT) */

    return retval;
}


/**
 * \brief To set local and remote key distribution support
 *
 * \par Description:
 *      This API sets the local and remote keys to be exchanged, that will be
 *      used during SMP authentication procedure.
 *
 * \param [in] key_dist  Key distribution setting flag
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_key_distribution_flag_pl (/* IN */ UCHAR key_dist)
{
    SMP_TRC(
    "[SMP] Received Set Key distribution flag. key_distro = 0x%02X\n",
    key_dist);

    /* Set key distribution flag */
    smp_key_distribution_pl = key_dist;

    /* Return success */
    return API_SUCCESS;
}


/**
 * \brief To set platform Max Encryption key size
 *
 * \par Description:
 *     This API sets the max encryption key size of local platform.
 *
 * \param [in] key_size  Max Encryption size
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_max_encryption_key_size_pl (/* IN */ UCHAR key_size)
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    SMP_TRC(
    "[SMP] Recived Set Max Encryption Key size. key_distro = 0x%02X\n",
    key_size);

#ifndef SMP_NO_PARAM_CHECK
    if ((SMP_MIN_ENCRYPTION_KEY_SIZE > key_size) ||
        (SMP_MAX_ENCRYPTION_KEY_SIZE < key_size))
    {
        SMP_ERR(
        "[SMP] Invalid paramter.\n");

        /* Return error */
        retval = SMP_INVALID_PARAMETERS; /* return SMP_INVALID_PARAMETERS; */
    }
#endif /* SMP_NO_PARAM_CHECK */

    /* Set encryption key size */
    smp_encrytion_key_size_pl = key_size;

    /* Return Success */
    return retval;
}


/**
 * \brief To get platform Max Encryption key size
 *
 * \par Description:
 *     This API retrieves the max encryption key size of local platform.
 *
 * \return Platform Max Encryption key size
 */
UCHAR BT_smp_get_max_encryption_key_size_pl (void)
{
    return smp_encrytion_key_size_pl;
}


#ifdef SMP_HAVE_OOB_SUPPORT
/**
 * \brief To set the platform OOB data
 *
 * \par Description:
 *      This API sets local platform OOB presence flag and data.
 *
 * \param [in] bd_addr  Peer device for which the OOB configuration is to be used
 * \param [in] oob_flag  OOB data presence flag
 * \param [in] oob  Pointer to the structure \ref SMP_OOB_DATA
 * \param [in] blob  OOB blob data
 * \param [in,out] blob_len  OOB blob data length.
 *                         Actual blob length stored will be returned.
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 *
 * \note Before calling this API to set the OOB data, application must
 *       add the device to the platform database by calling
 *       BT_smp_add_device_pl().
 */
API_RESULT BT_smp_set_oob_data_pl
           (
               /* IN */    BT_DEVICE_ADDR * bd_addr,
               /* IN */    UCHAR            oob_flag,
               /* IN */    SMP_OOB_DATA   * oob,
               /* IN */    UCHAR          * blob,
               /* INOUT */ UCHAR          * blob_len
           )
{
    UCHAR i;
    API_RESULT retval;

    /* Init */
    retval = API_FAILURE;

    SMP_TRC(
    "[SMP] Recived Set OOB data. oob_flag = 0x%02X\n",
    oob_flag);

    /* If OOB data available and data pointer is NULL or invalid flag */
    if ((NULL == bd_addr) || ((0x01U == oob_flag) && (NULL == oob)) || (oob_flag > 0x01U))
    {
        SMP_ERR(
        "[SMP] Invalid paramter.\n");

        /* Return error */
        retval = SMP_INVALID_PARAMETERS; /*  return SMP_INVALID_PARAMETERS; */
    }
    else
    {
        /* Look up platform database to get the device id */
#if (SMP_PL_DEVICE_LIST_SIZE > 1)
        for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
        i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
        {
            if ((0x01U == smp_pl_device_list[i].used) &&
                (BT_TRUE ==
                 BT_COMPARE_BD_ADDR_AND_TYPE (bd_addr, &smp_pl_device_list[i].bd_addr)))
            {
                /* Set the OOB flag */
                smp_pl_device_list[i].oob_flag = oob_flag;

                /* Copy the Temp Key if OOB data is set */
                if (0x01U == oob_flag)
                {
#ifdef SMP_LESC
                    BT_mem_copy
                    (
                        smp_pl_device_list[i].cnf_val,
                        oob->lesc_cnf_val,
                        SMP_OOB_CONFIRM_VAL_SIZE
                    );

                    BT_mem_copy
                    (
                        smp_pl_device_list[i].rand,
                        oob->lesc_rand,
                        SMP_OOB_RANDOM_VAL_SIZE
                    );
#endif /* SMP_LESC */
                    BT_mem_copy
                    (
                        smp_pl_device_list[i].tk,
                        oob->temp_key,
                        SMP_OOB_TEMP_KEY_SIZE
                    );
                }

                if ((NULL != blob) && (NULL != blob_len))
                {
                    /* Copy the given blob data to the max limit available */
                    *blob_len = (SMP_OOB_BLOB_DATA_SIZE < (*blob_len))?
                                 SMP_OOB_BLOB_DATA_SIZE: (*blob_len);
                    BT_mem_copy
                    (
                        smp_pl_device_list[i].blob,
                        blob,
                        *blob_len
                    );
                }

                retval = API_SUCCESS; /* return API_SUCCESS; */

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
                break;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
            }
        }
    }

    return retval;
}


/**
 * \brief To get the platform OOB data
 *
 * \par Description:
 *      This API retrieves peer device OOB information.
 *
 * \param [in] bd_addr  Peer device for which the OOB configuration is to be retrieved
 * \param [out] oob_flag  OOB data presence flag
 * \param [out] oob  Pointer to the structure \ref SMP_OOB_DATA
 * \param [out] blob  OOB blob data
 * \param [in,out] blob_len  OOB blob data length.
 *                         Actual blob length copied will be returned.
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_get_oob_data_pl
           (
               /* IN */    BT_DEVICE_ADDR * bd_addr,
               /* OUT */   UCHAR          * oob_flag,
               /* OUT */   SMP_OOB_DATA   * oob,
               /* OUT */   UCHAR          * blob,
               /* INOUT */ UCHAR          * blob_len
           )
{
    UCHAR i;
    API_RESULT retval;

    /* Init */
    retval = API_FAILURE;

    /* If OOB data available and data pointer is NULL or invalid flag */
    if ((NULL == bd_addr) || (NULL == oob))
    {
        SMP_ERR(
        "[SMP] Invalid paramter.\n");

        /* Return error */
        retval = SMP_INVALID_PARAMETERS; /* return SMP_INVALID_PARAMETERS; */
    }
    else
    {
        /*
         * Look up platform database to check OOB support for the device
         * and get the data
         */
#if (SMP_PL_DEVICE_LIST_SIZE > 1)
        for (i = 0U; i < SMP_PL_DEVICE_LIST_SIZE; i++)
#else /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
        i = 0;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
        {
            if ((0x01U == smp_pl_device_list[i].used) &&
                (BT_TRUE ==
                 BT_COMPARE_BD_ADDR_AND_TYPE (bd_addr, &smp_pl_device_list[i].bd_addr)))
            {
                /* Get the OOB flag */
                *oob_flag = smp_pl_device_list[i].oob_flag;

                /* Get the Temp Key if OOB data is available */
                if (0x01U == (*oob_flag))
                {
#ifdef SMP_LESC
                    BT_mem_copy (oob->lesc_rand, smp_pl_device_list[i].rand, SMP_OOB_RANDOM_VAL_SIZE);
                    BT_mem_copy (oob->temp_key, smp_pl_device_list[i].cnf_val, SMP_OOB_CONFIRM_VAL_SIZE);
#endif /* SMP_LESC */
                    BT_mem_copy (oob->temp_key, smp_pl_device_list[i].tk, SMP_OOB_TEMP_KEY_SIZE);
                }

                if ((NULL != blob) && (NULL != blob_len))
                {
                    /* Copy oob data from database */
                    *blob_len = (SMP_OOB_BLOB_DATA_SIZE < (*blob_len))?
                                 SMP_OOB_BLOB_DATA_SIZE: (*blob_len);
                    BT_mem_copy
                    (
                        blob,
                        smp_pl_device_list[i].blob,
                        *blob_len
                    );
                }

                retval = API_SUCCESS; /* return API_SUCCESS; */

#if (SMP_PL_DEVICE_LIST_SIZE > 1)
                break;
#endif /* (SMP_PL_DEVICE_LIST_SIZE > 1) */
            }
        }
    }

    return retval;
}
#endif /* SMP_HAVE_OOB_SUPPORT */


/**
 *  \fn BT_smp_set_mitm_policy_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param policy
 *
 *  \return None
 */
void BT_smp_set_mitm_policy_pl (UCHAR policy)
{
    smp_mitm_policy_pl = policy;
}

/**
 *  \fn BT_smp_get_mitm_policy_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param void
 *
 *  \return Policy
 */
UCHAR BT_smp_get_mitm_policy_pl(void)
{
    return smp_mitm_policy_pl;
}

#ifdef SMP_LESC
/**
 *  \fn BT_smp_set_lesc_policy_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param policy
 *
 *  \return None
 */
void BT_smp_set_lesc_policy_pl (UCHAR policy)
{
    smp_lesc_policy_pl = policy;
}

/**
 *  \fn BT_smp_get_lesc_policy_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param policy
 *
 *  \return None
 */
UCHAR BT_smp_get_lesc_policy_pl(void)
{
    return smp_lesc_policy_pl;
}

/**
 *  \fn BT_smp_set_key_press_ntf_pl
 *
 *  \brief
 *
 *  \Description
 *
 *
 *  \param policy
 *
 *  \return None
 */
void BT_smp_set_key_press_ntf_pl (UCHAR flag)
{
    smp_lesc_key_press_ntf_pl = flag;
}
#endif /* SMP_LESC */

/**
 * \brief To purge internal device list
 *
 * \par Description:
 *      This internal platform API is used to purge and manage the internal
 *      device list. This API is called when SMP device database is full
 *      and there is no space to add new device.
 *
 * \param [out] free_index  Index of freed device in database
 *
 * \return API_RESULT:
 *         - API_SUCCESS: If successful.
 *         - Error Codes: An error code describing the cause of failure.
 */
API_RESULT smp_purge_device_list_pl (UCHAR *free_index)
{
    UCHAR max_rank_device = SMP_MAX_DEVICES;
    SMP_DEVICE_ATTR_PL max_rank = 0U;
    UCHAR di;
    API_RESULT retval;

    SMP_TRC(
    "[SMP] Purging the device list to free a device database entity.\n");

    for(di = 0U; di < SMP_MAX_DEVICES; di++)
    {
        /* Store max ranked device index */
        if((SMP_ENTITY_VALID_TRUSTED == SMP_GET_ENTITY_VALIDITY(smp_devices[di].state)) &&
            (SMP_L2CAP_INVALID_SIG_ID == smp_devices[di].l2cap_sid) &&
            (smp_devices[di].db.device_attr_pl > max_rank))
        {
            SMP_TRC(
            "[SMP] Found Max ranked device. di = %u\n", di);

            /* Copy max rank */
            max_rank = smp_devices[di].db.device_attr_pl;

            /* Copy max ranked device index */
            max_rank_device = di;
        }
    }

    /* If deletable max ranked device found */
    if(SMP_MAX_DEVICES != max_rank_device)
    {
        SMP_TRC(
        "[SMP] Deleting device entity. di = %u\n", max_rank_device);

        /* Mark device untrusted */
        SMP_SET_BONDING_OFF(smp_devices[max_rank_device].dev_attr);

        /* Free device entity */
        smp_reset_device_entity(max_rank_device, BT_TRUE);

        retval = API_SUCCESS;
    }
    else
    {
        SMP_ERR(
        "[SMP-PL] FAILED to Purge Device List for Highest Rank!\n");

        retval = SMP_DEVICE_ENTRY_PURGE_FAILED;
    }

    /* Use the max ranked device */
    *free_index = max_rank_device;

    return retval;
}


/**
 * \brief To update the platform specific device attrbute.
 *
 * \par Description:
 *      This is internal platform API to update/delte platform specific device.
 *      This API is called whenever there connection/reconnection or
 *      pairing/authentication or disconnection.
 *
 * \param [in] operation  The operation to be performed on the device.
 *        - \ref SMP_DEVICE_ATTR_PL_DELETE To deleted platform specific
 *               device attribute.
 *        - \ref SMP_DEVICE_ATTR_PL_CONNECTION_COMPLETE To indicate connection
 *               is been establish to remote device and needs update platform
 *               specifci attribute.
 *        - \ref SMP_DEVICE_ATTR_PL_AUTHENTICATION_COMPLETE To indicate
 *               successful authentication of a Link.
 *
 * \param [in] di  The remote device database index.
 *
 * \return void
 */
void smp_update_device_attr_pl (UCHAR operation, UCHAR di)
{
    UCHAR i;
    SMP_DEVICE_ATTR_PL rank;

#ifdef SMP_STORAGE
    UCHAR update;
#endif /* SMP_STORAGE */

    SMP_TRC(
    "[SMP] Operation 0x%02X on Device Index %u, Rank %u\n",
    operation, (UCHAR) di, (UCHAR) smp_devices[di].db.device_attr_pl);

#ifdef SMP_STORAGE
    /* Set update to false by default */
    update = BT_FALSE;
#endif /* SMP_STORAGE */

    switch (operation)
    {
    case SMP_DEVICE_ATTR_PL_DELETE:
        /*
         *  Deletion of Rank is handled only if the Device has
         *  been provided with a valid Rank before.
         */
        if (0U != smp_devices[di].db.device_attr_pl)
        {
            /* Save current Rank of the Device */
            rank = smp_devices[di].db.device_attr_pl;

            /* Reset Device's Rank */
            smp_devices[di].db.device_attr_pl = 0U;

            /* Decrement Ranks of Devices holding Ranks above this Device */
            for (i = 0U; i < SMP_MAX_DEVICES; i ++)
            {
                if ((i == di) ||
                    (SMP_ENTITY_VALID_TRUSTED != SMP_GET_ENTITY_VALIDITY(smp_devices[i].state)))
                {
                    continue;
                }

                if (smp_devices[i].db.device_attr_pl > rank)
                {
                    smp_devices[i].db.device_attr_pl --;
                }
            }

            /* Decrement Rank Count offered so far */
            smp_rank_count_pl --;

#ifdef SMP_STORAGE
            /* Indicate to update */
            update = BT_TRUE;
#endif /* SMP_STORAGE */
        }

        break;

    case SMP_DEVICE_ATTR_PL_CONNECTION_COMPLETE:        /* Fall Through */
    case SMP_DEVICE_ATTR_PL_AUTHENTICATION_COMPLETE:
#ifdef SMP_STORAGE
        /* Indicate to update */
        update = BT_TRUE;
#endif /* SMP_STORAGE */

        /*
         *  If there is no Rank awarded to the Device so far, award
         *  it a Rank provisionally so that it is put in the end
         *  of the Rank list.
         */
        if (0U == smp_devices[di].db.device_attr_pl)
        {
            /* Increment Rank Count offered so far */
            smp_rank_count_pl ++;

            /* The Device is being added in the end of Rank List */
            smp_devices[di].db.device_attr_pl = smp_rank_count_pl;
        }

        /*
         *  If the Device already at the top of Rank List (Rank = 1),
         *  then nothing needs to be done - the Device continues to
         *  hold Rank 1.
         */
        rank = 1U;
        if (1U != smp_devices[di].db.device_attr_pl)
        {
            /* Save current Rank of the Device */
            rank = smp_devices[di].db.device_attr_pl;

            /* Move the Device at the top of Rank List */
            smp_devices[di].db.device_attr_pl = 1U;

            /* Update Rank of Devices appearing below by 1 */
            for (i = 0U; i < SMP_MAX_DEVICES; i ++)
            {
                if ((i == di) ||
                    (SMP_ENTITY_VALID_TRUSTED != SMP_GET_ENTITY_VALIDITY(smp_devices[i].state)))
                {
                    continue;
                }

                if (smp_devices[i].db.device_attr_pl < rank)
                {
                    smp_devices[i].db.device_attr_pl ++;
                }
            }
        }
        break;

    default: /* Do nothing */
        SMP_TRC("Invalid Operation\n");
        break;
    }

#ifdef SMP_STORAGE
    /**
     *  At this point Rank for the Devices have been updated. The updated
     *  Ranks now must be stored in the Persistent Storage, to avoid
     *  unwanted loss of information in case of crash/hang.
     */
    if (BT_TRUE == update)
    {
        UINT16            storage_type;

        /* Unlock SMP */
        smp_unlock_void();

        if (SMP_DEVICE_ATTR_PL_AUTHENTICATION_COMPLETE == operation)
        {
            storage_type = STORAGE_EVENT_AUTH_UPDATE;
        }
        else
        {
            storage_type = STORAGE_EVENT_OTHERS;
        }

        /* Update Persistent Storage */
        (BT_IGNORE_RETURN_VALUE) BT_storage_store_db(STORAGE_TYPE_PERSISTENT, storage_type);

        /* Lock SMP */
        smp_lock_void();
    }
#endif /* SMP_STORAGE */

}


/**
 * \brief To check for security requirement
 *
 * \par Description:
 *      This internal platform API is used to assert for required security for
 *      a link during SMP authentication procedure.
 *
 * \param [in] auth_req  Required Authentication, as per auth_req format in spec
 * \param [in] auth_poss  Possible Authentication, as per auth_req format in spec
 * \param [in] assoc  IOCAP Association model possible
 *
 * \return API_SUCCESS if requirement met, API_FAILURE if requirement failed to
 * meet
 *
 * \note Currently, implementation does checking only for MITM
 */
API_RESULT smp_check_pairing_security_pl
           (
               UCHAR auth_req,
               UCHAR auth_poss,
               UCHAR assoc
           )
{
    BT_IGNORE_UNUSED_PARAM(auth_poss);

#ifndef SMP_LESC
    /*
     * If Auth Required has MITM and IOCAP association
     * would support JUST WORKS(0x00), and if the platform policy is strict
     * it's requirement failure, else requirement met.
     */
    return ((auth_req & SMP_AUTHREQ_MITM_BITMASK) &&
            (0x00U == assoc))?
            ((SMP_PL_SEC_MITM_STRICT == smp_mitm_policy_pl)?
             (API_FAILURE): (API_SUCCESS)): (API_SUCCESS);
#else /* SMP_LESC */
    return ((SMP_TRUE != SMP_AUTHREQ_IS_LESC(auth_req)) && ((SMP_PL_LESC_STRICT ==
           smp_lesc_policy_pl)))? (API_FAILURE) :\
           ((auth_req & SMP_AUTHREQ_MITM_BITMASK) &&
           (0x00U == assoc))?
           ((SMP_PL_SEC_MITM_STRICT == smp_mitm_policy_pl)?
           (API_FAILURE): (API_SUCCESS)): (API_SUCCESS);
#endif /* SMP_LESC */
}

#ifdef SMP_LESC
API_RESULT smp_get_lesc_key_ntf_pl(void)
{
    return (SMP_PL_LESC_KEY_PRESS_NTF_SET == smp_lesc_key_press_ntf_pl) ?
           API_SUCCESS:API_FAILURE;
}
#endif /* SMP_LESC */

/**
 * \brief To set local Identity Address
 *
 * \par Description:
 *      This API sets the local Identity Address that will be used during
 *      SMP authentication Key Exchange procedure.
 *
 * \param [in] bd_addr Pointer to Identity Address to be set  \ref SMP_BD_ADDR
 *
 * \return API_SUCCESS if Successful else an Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_set_local_identity_addr(/* IN */ SMP_BD_ADDR * bd_addr)
{
    SMP_TRC(
    "[SMP] Received Set Local ID Address :- " BT_DEVICE_ADDR_FRMT_SPECIFIER "\n",
    BT_DEVICE_ADDR_PRINT_STR(bd_addr));

    /* Copy Identity Address */
    BT_COPY_BD_ADDR
    (
        &key_distribution_info_pl.id_addr_info[1U],
        &bd_addr->addr
    );

    /* Copy Identity Address Type */
    BT_COPY_TYPE
    (
        key_distribution_info_pl.id_addr_info[0U],
        bd_addr->type
    );

    return API_SUCCESS;
}


#ifdef HOST_RESOLVE_PVT_ADDR
/**
 *  \brief To resolve the connected device if Resolvable Private Address
 *
 *  \Description
 *  Default implementation makes use of SMP resolution procedure if only
 *  storage of remote keys in SMP is enabled by not defining SMP_NO_STORED_KEYS
 *  flag.
 *  This can be updated to make use of application resolution procedure
 *  in case when SMP_NO_STORED_KEYS is defined or in case when application
 *  desires to use its own remote database.
 *
 *  Post the resolution procedure from the application with all devices
 *  in database in a chained manner, the application shall
 *  notify the procedure result using the BT_smp_notify_address_resolution API.
 *
 *  \param [in] bdaddr  Peer device address to be resolved that has connected
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT smp_resolve_connection_address_pl
           (
               BT_DEVICE_ADDR * bdaddr,
               SMP_PVT_ADDR_RESOLV_COMPLETE_CB handler
           )
{
#ifndef SMP_NO_STORED_KEYS
    return BT_smp_resolve_device (bdaddr->addr, handler);
#else /* SMP_NO_STORED_KEYS */
    return SMP_PROCEDURE_FAILED;
#endif /* SMP_NO_STORED_KEYS */
}
#endif /* HOST_RESOLVE_PVT_ADDR */

#ifdef SMP_PL_ENABLE_KEY_GENERATION
/**
 * \brief To generate the SMP platform keys
 *
 * \par Description:
 *      This utility platform API helps to generate different SMP keys upon
 *      giving a diversifier value, that may be specific to a peer device.
 *
 * \param [in] div  Diversifier specific to a device
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_keys_pl (UINT16 div)
{
    div_pl = div;
    auto_generate = 0x01U;
    return BT_smp_generate_ediv_pl();
}

/**
 * \brief To generate the SMP Encrypted Diversifier
 *
 * \par Description:
 *      This utility platform API helps to generate Encrypted Diversifier.
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_ediv_pl (void)
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Validate state to check if no other operation id on */
    if (0x00U != key_gen_state)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Set state */
        key_gen_state = KEY_STATE_IN_EDIV_GEN;

        /* Generate the RAND */
#ifdef SMP_HAVE_TBX_PL_RAND
        smp_tbx_generate_rand(rnd_pl, SMP_RAND_64B_SIZE);
#else /* SMP_HAVE_TBX_PL_RAND */
        smp_tbx_generate_rand();
#endif /* SMP_HAVE_TBX_PL_RAND */
    }

    return retval;
}


/**
 * \brief To generate the SMP Encryption key
 *
 * \par Description:
 *      This utility platform API helps to generate Encryption key
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_ltk_pl (void)
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Validate state to check if no other operation id on */
    if (0x00U != key_gen_state)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Set state */
        key_gen_state = KEY_STATE_IN_LTK_GEN;

        /* LTK = d1(ER, DIV, 0) */
        smp_func_d1(er_pl, div_pl, 0x00U);
    }

    return retval;
}

/**
 * \brief To generate the SMP signature key
 *
 * \par Description:
 *      This utility platform API helps to generate signature key
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_csrk_pl (void)
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Validate state to check if no other operation id on */
    if (0x00U != key_gen_state)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Set state */
        key_gen_state = KEY_STATE_IN_CSRK_GEN;

        /* CSRK = d1(ER, DIV, 1) */
        smp_func_d1(er_pl, div_pl, 0x01U);
    }

    return retval;
}


/**
 * \brief To generate the SMP Identity key
 *
 * \par Description:
 *      This utility platform API helps to generate identity resolving key
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_irk_pl (void)
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Validate state to check if no other operation id on */
    if (0x00U != key_gen_state)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Set state */
        key_gen_state = KEY_STATE_IN_IRK_GEN;

        /* IRK = d1(IR, 1, 0) */
        smp_func_d1(ir_pl, 0x01U, 0x00U);
    }

    return retval;
}


/**
 * \brief To generate the SMP DHK
 *
 * \par Description:
 *      This utility platform API helps to generate DHK
 *
 * \return API_SUCCESS if Successful else and Error code describing
 * cause of failure.
 */
API_RESULT BT_smp_generate_dhk_pl (void)
{
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    /* Validate state to check if no other operation id on */
    if (0x00U != key_gen_state)
    {
        retval = API_FAILURE; /* return API_FAILURE; */
    }
    else
    {
        /* Set state */
        key_gen_state = KEY_STATE_IN_DHK_GEN;

        /* DHK = d1(IR, 3, 0) */
        smp_func_d1(ir_pl, 0x03U, 0x00U);
    }

    return retval;
}


/**
 *  \fn smp_func_d1
 *
 *  \brief Diversifying function
 *
 *  \Description
 *
 *
 *  \param k
 *  \param d
 *  \param r
 *
 *  \return
 */
DECL_STATIC API_RESULT smp_func_d1 (UCHAR * k, UINT16 d, UINT16 r)
{
    UCHAR _d[16U];
    UCHAR count;

    /* _d = padding (96 bits) || r || d */
    BT_mem_set (_d, 0x00, 16U);
    _d[0U] = (UCHAR)d;
    _d[1U] = (UCHAR)(d >> 8U);
    _d[2U] = (UCHAR)r;
    _d[3U] = (UCHAR)(r >> 8U);

    /* d1(k, d, r) = e(k, _d) */

    /* Call to Encrypt */
    smp_tbx_128B_encrypt(k, _d, &count);

    return API_SUCCESS;
}


/**
 *  \fn smp_func_dm
 *
 *  \brief DIV Mask generation function
 *
 *  \Description
 *
 *
 *  \param k
 *  \param r
 *
 *  \return
 */
DECL_STATIC API_RESULT smp_func_dm (UCHAR * k, UCHAR * r)
{
    UCHAR _r[16U];
    UCHAR count;

    /* _r = padding (64 bits) || r */
    BT_mem_set (_r, 0x00, 16U);
    BT_mem_copy (_r, r, 8U);

    /* dm(k, r) = e(k, _r) mod 2^16 */
    smp_tbx_128B_encrypt(k, _r, &count);

    return API_SUCCESS;
}

/**
 * \brief Rand Complete handler
 *
 * \par Description:
 *      This handles the RAND complete HCI event.
 *
 * \param [in] data  Random Number
 * \param [in] datalen  Random Number size
 *
 * \return void
 */
void smp_rand_complete_pl (UCHAR * data, UINT16 datalen)
{
    /* Validate state */
    if (0U != (KEY_STATE_IN_EDIV_GEN & key_gen_state))
    {
        /* Update the RAND for Master Identification */
        BT_mem_copy (rnd_pl, data, SMP_RAND_64B_SIZE);

        /* Set state */
        key_gen_state |= KEY_STATE_IN_DHK_GEN;

        /* DHK = d1(IR, 3, 0) */
        smp_func_d1 (ir_pl, 0x03U, 0x00U);
    }
}

/**
 * \brief Encryption Complete handler
 *
 * \par Description:
 *      This handles the Encryption complete HCI event.
 *
 * \param [in] data  Encrypted data
 * \param [in] datalen  Encrypted data size
 *
 * \return void
 */
void smp_encryption_complete_pl (UCHAR * data, UINT16 datalen)
{
    /* Validate state */
    if (KEY_STATE_IN_DHK_GEN & key_gen_state)
    {
        /* DHK Generated */
        SMP_TRC (
        "DHK Generated:\n");
        SMP_debug_dump_bytes (data, datalen);

        /* Store the DHK */
        BT_mem_copy (dhk_pl, data, datalen);

        /* Reset state bit */
        key_gen_state &= (UCHAR)(~KEY_STATE_IN_DHK_GEN);

        if (0U != (KEY_STATE_IN_EDIV_GEN & key_gen_state))
        {
            smp_func_dm (dhk_pl, rnd_pl);
        }
    }
    else if (KEY_STATE_IN_EDIV_GEN & key_gen_state)
    {
        /* DM E Generated */
        SMP_TRC (
        "DM E Generated:\n");
        SMP_debug_dump_bytes (data, datalen);

        /* Reset state bit */
        key_gen_state &= (UCHAR)(~KEY_STATE_IN_EDIV_GEN);

        /* div_y = (data) mod (2^16) - take the 16 least significant bits */
        BT_UNPACK_LE_2_BYTE(&div_y, data);

        /* ediv_pl = (div_y) xor (div_pl) */
        ediv_pl = div_y ^ div_pl;
        BT_PACK_LE_2_BYTE(key_distribution_info_pl.mid_info, &ediv_pl);

        if (0U != auto_generate)
        {
            BT_smp_generate_ltk_pl();
        }
    }
    else if (KEY_STATE_IN_LTK_GEN & key_gen_state)
    {
        /* LTK Generated */
        SMP_TRC (
        "LTK Generated:\n");
        SMP_debug_dump_bytes (data, datalen);

        /* Store the LTK */
        BT_mem_copy (ltk_pl, data, datalen);

        /* Reset state bit */
        key_gen_state &= (UCHAR)(~KEY_STATE_IN_LTK_GEN);

        if (0U != auto_generate)
        {
            /*
             * Enable when needed to generate CSRK.
             * Here it is generated once and single sign counter maintained
             */
            /* BT_smp_generate_csrk_pl(); */

            auto_generate = 0x00U;
        }
    }
    else if (KEY_STATE_IN_CSRK_GEN & key_gen_state)
    {
        /* CSRK Generated */
        SMP_TRC (
        "CSRK Generated:\n");
        SMP_debug_dump_bytes (data, datalen);

        /* Store the CSRK */
        BT_mem_copy (csrk_pl, data, datalen);

        /* Reset state bit */
        key_gen_state &= (UCHAR)(~KEY_STATE_IN_CSRK_GEN);

        if (0U != auto_generate)
        {
            /*
             * Enable when needed to generate IRK.
             * Normally this is done only once for a device
             */
            /* BT_smp_generate_irk_pl(); */

            auto_generate = 0x00U;
        }
    }
    else if (KEY_STATE_IN_IRK_GEN & key_gen_state)
    {
        /* IRK Generated */
        SMP_TRC (
        "IRK Generated:\n");
        SMP_debug_dump_bytes (data, datalen);

        /* Store the IRK */
        BT_mem_copy (irk_pl, data, datalen);

        /* Reset state bit */
        key_gen_state &= (UCHAR)(~KEY_STATE_IN_IRK_GEN);

        if (0U != auto_generate)
        {
            auto_generate = 0x00U;
        }
    }
    else
    {
        /* MISRA C-2012 Rule 15.7 */
    }
}
#endif /* SMP_PL_ENABLE_KEY_GENERATION */

#ifdef SMP_LESC
#ifdef SMP_HAVE_OOB_SUPPORT
API_RESULT BT_smp_generate_lesc_oob_local_data_pl
           (
               SMP_LESC_OOB_GEN_COMPLETE_CB handler
           )
{
    if (NULL != handler)
    {
        /* Store the oob generation complete handler */
        smp_lesc_oob_gen_handler = handler;
    }

    /* Initialize the state of smp_lesc_oob_data */
    SMP_LESC_OOB_SET_STATE(&smp_lesc_oob_data,SMP_OOB_STATE_RAND_GEN);

    /* Call Random Number Generation here */
    (BT_IGNORE_RETURN_VALUE) smp_tbx_generate_rand();

    return API_SUCCESS;
}

void smp_lesc_oob_handle_cmd_complete(UCHAR * data, UINT16 datalen)
{
    /* Check the state of the LESC OOB data structure */
    switch(smp_lesc_oob_data.state)
    {
        case SMP_OOB_STATE_RAND_GEN:
            /* Populate the data strucutre with the Random Number */
            /* Received Random number is  8 bytes from controller */
            BT_mem_set(smp_lesc_oob_data.rand,0x00,SMP_OOB_RANDOM_VAL_SIZE);

#ifndef SMP_LESC_CONST_OOB_VAL_SUPPORT

            BT_mem_copy(smp_lesc_oob_data.rand, data, ((datalen > SMP_OOB_RANDOM_VAL_SIZE) ? SMP_OOB_RANDOM_VAL_SIZE : datalen));
#else
            /*
             *  NOTE: If SMP_LESC_CONST_OOB_VAL_SUPPORT make the random
             *        number as all zeros.
             */
            BT_IGNORE_UNUSED_PARAM(datalen);
#endif /* SMP_LESC_CONST_OOB_VAL_SUPPORT */

            /* Change State to Public key generation */
            SMP_LESC_OOB_SET_STATE(&smp_lesc_oob_data,SMP_OOB_STATE_PUB_KEY_GEN);

            /* Invoke Public Key Generation */
            (BT_IGNORE_RETURN_VALUE) smp_tbx_read_l_p256_pub_key();
            break;

        case SMP_OOB_STATE_PUB_KEY_GEN:
            {
                UCHAR * plain_text;
                UINT16 plain_text_len = 0U;

                if (SMP_OOB_PUBLIC_KEY_SIZE != datalen)
                {
                    SMP_ERR("[SMP_PL] SMP_OOB_PUBLIC_KEY_SIZE != datalen\n");

                    /* Initialize state to IDLE */
                    SMP_LESC_OOB_SET_STATE(&smp_lesc_oob_data, SMP_OOB_STATE_IDLE);

                    break;
                }

                /* Populate the X co-ordinate of the Public Key*/
                BT_mem_copy(smp_lesc_oob_data.pkey,data,SMP_OOB_PUBLIC_KEY_SIZE);

                /* Change state to Confirm Value Generation */
                SMP_LESC_OOB_SET_STATE(&smp_lesc_oob_data,SMP_OOB_STATE_CONF_VAL_GEN);
                /* Invoke F4 function through AES CMAC */

                /* OOB F4() :
                 * Plain Text = Local Pub Key X | Local Pub Key X | 0
                 * Key        = Local Rand num
                 */

                /* Allocate Plain Text Array for Plain Text Length Size */
                plain_text = BT_alloc_mem(SMP_TBX_F4_PLAIN_TEXT_LEN);

                /* Populate PlainText here */
                BT_mem_set(&plain_text[plain_text_len],0x00,sizeof(UCHAR));
                plain_text_len += sizeof(UCHAR);
                BT_mem_copy(&plain_text[plain_text_len],smp_lesc_oob_data.pkey,SMP_OOB_PUBLIC_KEY_X_SIZE);
                plain_text_len += SMP_OOB_PUBLIC_KEY_X_SIZE;
                BT_mem_copy(&plain_text[plain_text_len],smp_lesc_oob_data.pkey,SMP_OOB_PUBLIC_KEY_X_SIZE);
                plain_text_len += SMP_OOB_PUBLIC_KEY_X_SIZE;

                /* Call the G2 Security function */
                (BT_IGNORE_RETURN_VALUE) smp_tbx_128_aes_cmac
                (
                    AES_CMAC_MAC_GENERATE,
                    SMP_AES_CMAC_LESC_CONF_VAL_GEN_OP,
                    plain_text,
                    plain_text_len,
                    smp_lesc_oob_data.rand,
                    SMP_AES_CMAC_MAC_SIZE,
                    smp_lesc_oob_data.cnf_val,
                    smp_lesc_oob_cnf_val_gen_complete,
                    (UCHAR)SMP_TRUE,
                    (UCHAR)SMP_FALSE
                );
            }
            break;

        default:
            /* Drop the Packet as LESC OOB is not in a state to process this */
            SMP_TRC("Invalid State\n");
            break;
    }
}

void smp_lesc_oob_cnf_val_gen_complete(void)
{
    if (SMP_OOB_STATE_CONF_VAL_GEN == smp_lesc_oob_data.state)
    {
        /* Free the Plain Text Allocated */
        BT_free_mem(smp_aes_context.cmac_context.data);

        /* Update AES CMAC context state */
        smp_aes_context.cmac_context.state = SMP_STATE_INVALID;

        /* Call the application Handler */
        smp_lesc_oob_gen_handler(&smp_lesc_oob_data);

        /* Change state to IDLE */
        SMP_LESC_OOB_SET_STATE(&smp_lesc_oob_data,SMP_OOB_STATE_IDLE);
    }
}
#endif /* SMP_HAVE_OOB_SUPPORT */

#ifdef SMP_LESC_CROSS_TXP_KEY_GEN
API_RESULT BT_smp_get_lk_ltk_pl
           (
               UCHAR                           mode,
               UCHAR                           * key,
               SMP_LESC_LK_LTK_GEN_COMPLETE_CB handler,
               UCHAR                           ct2
           )
{
    UCHAR  * plain_text;
    UCHAR  * enc_key;
    UINT16 plain_text_size;
    API_RESULT retval;

    /* Init */
    retval = API_SUCCESS;

    if (SMP_LESC_LK_LTK_IDLE != SMP_LESC_LK_LTK_GET_STATE(&smp_lesc_txp_keys))
    {
        SMP_ERR(
        "[SMP] Invalid State for Cross Transport Key Derivation.\n");

        retval = SMP_INCORRECT_STATE; /* return SMP_INCORRECT_STATE; */
    }
    else if (NULL == handler)
    {
        retval = API_FAILURE;
    }
    else
    {
        /* Store the Cross TXP Key generation complete handler */
        smp_lesc_lk_ltk_gen_handler = handler;

        plain_text = NULL;
        plain_text_size = 0U;
        enc_key = NULL;

        /* Invoke H6 function through AES CMAC */
        if (SMP_LESC_LK_FROM_LTK == mode)
        {
            if (0U == ct2)
            {
                SMP_TRC("[SMP] LTK to LK, CT2 = 0\n");

                /* Populate the global Cross Transport Key Gen Data struct */
                BT_mem_copy(smp_lesc_txp_keys.ltk, key, SMP_LTK_SIZE);

                enc_key = smp_lesc_txp_keys.ltk;

                /* Change state to Link Key Generation */
                SMP_LESC_LK_LTK_SET_STATE(&smp_lesc_txp_keys, SMP_LESC_TEMP1_GEN);

                /* Link key from Long Term Key i.e. LK from LTK H6() :
                 * Plain Text = "tmp1" i.e. smp_lesc_lk_tmp1
                 * Key        = incoming LTK
                 */
                 /* Populate PlainText here */
                plain_text = (UCHAR *)smp_lesc_lk_tmp1;
                plain_text_size = SMP_LESC_KEYID_SIZE;
            }
            else
            {
                SMP_TRC("[SMP] LTK to LK, CT2 = 1\n");

                /* Populate the global Cross Transport Key Gen Data struct */
                BT_mem_copy(smp_lesc_txp_keys.ltk, key, SMP_LTK_SIZE);

                enc_key = (UCHAR *)smp_lesc_lk_salt;

                /* Change state to Link Key Generation */
                SMP_LESC_LK_LTK_SET_STATE(&smp_lesc_txp_keys, SMP_LESC_TEMP1_GEN);

                /* Link key from Long Term Key i.e. LK from LTK H7() :
                * Plain Text = incoming LTK
                * Key        = SALT i.e. smp_lesc_lk_salt
                */
                /* Populate PlainText here */
                plain_text = (UCHAR *)smp_lesc_txp_keys.ltk;
                plain_text_size = SMP_LESC_KEY_SIZE;
            }
        }
        else if (SMP_LESC_LTK_FROM_LK == mode)
        {
            if (0U == ct2)
            {
                SMP_TRC("[SMP] LK to LTK, CT2 = 0\n");

                /* Populate the global Cross Transport Key Gen Data struct */
                BT_mem_copy(smp_lesc_txp_keys.lk, key, SMP_LK_SIZE);

                enc_key = smp_lesc_txp_keys.lk;

                /* Change state to Link Key Generation */
                SMP_LESC_LK_LTK_SET_STATE(&smp_lesc_txp_keys, SMP_LESC_TEMP2_GEN);

                /* Link key from Long Term Key i.e. LK from LTK H6() :
                 * Plain Text = "tmp2" i.e. smp_lesc_ltk_tmp2
                 * Key        = incoming LK
                 */
                 /* Populate PlainText here */
                plain_text = (UCHAR *)smp_lesc_ltk_tmp2;
                plain_text_size = SMP_LESC_KEYID_SIZE;
            }
            else
            {
                SMP_TRC("[SMP] LTK to LK, CT2 = 1\n");

                /* Populate the global Cross Transport Key Gen Data struct */
                BT_mem_copy(smp_lesc_txp_keys.lk, key, SMP_LTK_SIZE);

                enc_key = (UCHAR *)smp_lesc_ltk_salt;

                /* Change state to Link Key Generation */
                SMP_LESC_LK_LTK_SET_STATE(&smp_lesc_txp_keys, SMP_LESC_TEMP2_GEN);

                /* Link key from Long Term Key i.e. LTK from LK H7() :
                * Plain Text = incoming LK
                * Key        = SALT i.e. smp_lesc_ltk_salt
                */
                /* Populate PlainText here */
                plain_text = (UCHAR *)smp_lesc_txp_keys.lk;
                plain_text_size = SMP_LESC_KEY_SIZE;
            }
        }
        else
        {
            /* MISRA C-2012 Rule 15.7 */
        }

#ifdef SMP_DATA_DEBUG
        SMP_TRC (
        "[SMP] H6 Func 1st set data Text, Key:\n");

        SMP_debug_dump_bytes (plain_text, SMP_LESC_KEYID_SIZE);
        SMP_debug_dump_bytes (enc_key, SMP_RAND_128B_SIZE);
#endif /* SMP_DATA_DEBUG */

        if (NULL == enc_key)
        {
            retval = API_FAILURE; /* return API_FAILURE; */
        }
        else
        {
            /* Call the h6 Security function */
            retval = smp_tbx_128_aes_cmac
                     (
                         AES_CMAC_MAC_GENERATE,
                         SMP_TBX_H6_PLAIN_TEXT_LEN,
                         plain_text,
                         plain_text_size,
                         enc_key,
                         SMP_AES_CMAC_MAC_SIZE,
                         smp_lesc_txp_keys.temp,
                         smp_lesc_lk_ltk_complete,
                         (UCHAR)SMP_FALSE,
                         (UCHAR)SMP_FALSE
                     );
        }
    }

    return retval;
}

void smp_lesc_lk_ltk_complete (void)
{
    UCHAR switch_state;

    switch_state = SMP_LESC_LK_LTK_GET_STATE(&smp_lesc_txp_keys);

    /* Update AES CMAC context state */
    smp_aes_context.cmac_context.state = SMP_STATE_INVALID;

    /* Check the state of the LESC OOB data structure */
    switch(switch_state)
    {
        case SMP_LESC_TEMP1_GEN:
            {
#ifdef SMP_DATA_DEBUG
                SMP_TRC (
                "[SMP] H6 Func 2nd set data Text, Key:\n");

                SMP_debug_dump_bytes ((UCHAR *)smp_lesc_lk_lebr, SMP_LESC_KEYID_SIZE);
                SMP_debug_dump_bytes (smp_lesc_txp_keys.temp, SMP_RAND_128B_SIZE);
#endif /* SMP_DATA_DEBUG */

                /* Invoke H6 Again with the tmp1 AES-CMAC Output */
                (BT_IGNORE_RETURN_VALUE) smp_tbx_128_aes_cmac
                (
                    AES_CMAC_MAC_GENERATE,
                    SMP_AES_CMAC_LESC_LINK_KEY_CONV_OP,
                    (UCHAR *)smp_lesc_lk_lebr,
                    SMP_TBX_H6_PLAIN_TEXT_LEN,
                    smp_lesc_txp_keys.temp,
                    SMP_AES_CMAC_MAC_SIZE,
                    smp_lesc_txp_keys.lk,
                    smp_lesc_lk_ltk_complete,
                    (UCHAR)SMP_FALSE,
                    (UCHAR)SMP_FALSE
                );

                /* Change state to Link Key Generation */
                SMP_LESC_LK_LTK_SET_STATE(&smp_lesc_txp_keys,SMP_LESC_LK_GEN);
            }
            break;
        case SMP_LESC_TEMP2_GEN:
            {
#ifdef SMP_DATA_DEBUG
                SMP_TRC (
                "[SMP] H6 Func 2nd set data Text, Key:\n");

                SMP_debug_dump_bytes ((UCHAR *)smp_lesc_ltk_brle, SMP_LESC_KEYID_SIZE);
                SMP_debug_dump_bytes (smp_lesc_txp_keys.temp, SMP_RAND_128B_SIZE);
#endif /* SMP_DATA_DEBUG */
                /* Invoke H6 Again with the tmp2 AES-CMAC Output */
                (BT_IGNORE_RETURN_VALUE) smp_tbx_128_aes_cmac
                (
                    AES_CMAC_MAC_GENERATE,
                    SMP_AES_CMAC_LESC_LINK_KEY_CONV_OP,
                    (UCHAR *)smp_lesc_ltk_brle,
                    SMP_TBX_H6_PLAIN_TEXT_LEN,
                    smp_lesc_txp_keys.temp,
                    SMP_AES_CMAC_MAC_SIZE,
                    smp_lesc_txp_keys.ltk,
                    smp_lesc_lk_ltk_complete,
                    (UCHAR)SMP_FALSE,
                    (UCHAR)SMP_FALSE
                );

                /* Change state to Link Key Generation */
                SMP_LESC_LK_LTK_SET_STATE(&smp_lesc_txp_keys,SMP_LESC_LTK_GEN);
            }
            break;
        /* Fall Through */
        case SMP_LESC_LK_GEN:
        case SMP_LESC_LTK_GEN:
            {
                /* Change state to Link Key Generation */
                SMP_LESC_LK_LTK_SET_STATE(&smp_lesc_txp_keys,SMP_LESC_LK_LTK_IDLE);

                /* Print the output as LK- TODO post to appl thru Callback */
                smp_lesc_lk_ltk_gen_handler(&smp_lesc_txp_keys);
            }
            break;

        default:
            SMP_TRC("Invalid State\n");
            break;
    }
}
#endif /* SMP_LESC_CROSS_TXP_KEY_GEN */
#endif /* SMP_LESC */

#ifdef STORAGE_CHECK_CONSISTENCY_ON_RESTORE
/**
 *  \fn smp_get_valid_dq_entries_pl
 *
 *  \par Description:
 *  This function is called to get bit field representing valid
 *  device queue entries.
 *
 *  \param [out] dq_entries
 *         Bit field to represent device queue entries.
 *         1 : Valid device queue entry.
 *         0 : Unknown device queue entry (it could be associated with BR/EDR).
 *
 *  \return API_RESULT:
 *          - API_SUCCESS: If successful.
 *          - Error Codes: An error code describing the cause of failure.
 */
API_RESULT smp_get_valid_dq_entries_pl
           (
               /* OUT */ UINT32 *    dq_entries
           )
{
    *dq_entries = smp_dq_valid_entries;

    SMP_TRC(
    "[SMP] Valid DQ entries bit field 0x%08X.\n", smp_dq_valid_entries);

    return API_SUCCESS;
}
#endif /* STORAGE_CHECK_CONSISTENCY_ON_RESTORE */

/**
 *  \fn BT_smp_set_passkey_revamp_mode_pl
 *
 *  \par Description
 *  This function is called to set the passkey revamp mode to be used
 *  to enhance the passkey entropy as provided by the platform policy.
 *
 *  \param mode
 *
 *  \return API_RESULT:
 *          - API_SUCCESS: If successful.
 *          - Error Codes: An error code describing the cause of failure.
 */
API_RESULT BT_smp_set_passkey_revamp_mode_pl (/* IN */ UCHAR mode)
{
    API_RESULT retval;

    retval = API_SUCCESS;

    SMP_TRC (
    "[SMP]: >> BT_smp_set_passkey_revamp_mode_pl\n");

    /* Assigning application provided mode to Global */
    smp_passkey_revamp_mode_pl = mode;

    SMP_TRC (
    "[GAP]: << Setting Passkey Revamp Mode to 0x%02X\n",
    smp_passkey_revamp_mode_pl);

    SMP_TRC (
    "[GAP]: << BT_smp_set_passkey_revamp_mode_pl, retval 0x%04X\n", retval);

    return retval;
}

/**
 *  \fn smp_revamp_passkey_pl
 *
 *  \par Description:
 *  This platform call is provided as a platform hook for the internal stack functions
 *  to check if the PASSKEY generated by the stack needs to be updated.
 *
 *  \param [in,out] p_key_ptr : Pointer to the Passkey generated by the Stack.
 *  \param [in,out] p_key_len : Pointer to the length of Passkey generated by
 *                              the Stack.
 *
 *  \return API_RESULT:
 *          - API_SUCCESS: If successful.
 *          - Error Codes: An error code describing the cause of failure.
 *
 * NOTE: Currently, the stack truncates the Random Number generated to 2 Octets,
 *       because, 3 byte random numbers have the possibility of realizing a
 *       decimal number more than 999999, which is incorrect. According to the
 *       specificaion, 6 digit numeric passkeys shall only be used.
 */
API_RESULT smp_revamp_passkey_pl
           (
               /* INOUT */ UCHAR  * p_key_ptr,
               /* INOUT */ UINT16 * p_key_len
           )
{
    API_RESULT retval;
    UINT32     t_pkey_val;

    retval     = API_SUCCESS;
    t_pkey_val = 0x00000000U;

    SMP_TRC (
    "[SMP]: >> smp_revamp_passkey_pl\n");

    /* NULL Check the incoming parameters */
    if ((NULL == p_key_ptr) || (NULL == p_key_len))
    {
        SMP_ERR (
        "[SMP]: NULL pointer not accepted as param "
        "for smp_revamp_passkey_pl\n");

        retval = SMP_INVALID_PARAMETERS;
    }
    else
    {
        /* The Passkey data len shall be more than 3 octets */
        if (0x03U > (*p_key_len))
        {
            SMP_ERR (
            "[SMP]: Length of data less than 3 Octets not accepted "
            "for smp_revamp_passkey_pl\n");

            retval = SMP_INVALID_PARAMETERS;
        }
        else
        {
            /* Unpack 3 bytes of the passkey into the local */
            smp_unpack_3_byte_param (&t_pkey_val, p_key_ptr);

            SMP_TRC (
            "[SMP]: Stack generated Passkey Value is"
            "0x%02X [in HEX], %u [in Decimal]\n",
            t_pkey_val, t_pkey_val);

            /* Erase the exisiting data */
            BT_mem_set
            (
                p_key_ptr,
                0x0,
                (*p_key_len)
            );

            /**
             * The valid range of passkey is :
             * Decimal: 000000    - 999999
             * Hex    : 0x00000   - 0xF423F
             *
             * The platform can choose to have the following options
             * to increase the generated passkey entropy
             * 1. Modulus of the resultant passkey with 1000000
             * 2. Take the value to the nearly 2^19 bits.
             * 3. Anyother platform related logic can be added.
             */

            SMP_TRC (
            "[SMP]: Current passkey gen mode is 0x%02X\n",
            smp_passkey_revamp_mode_pl);

            switch (smp_passkey_revamp_mode_pl)
            {
                case 0x01U:
                    /* Modulus of 1000000 */
                    t_pkey_val = t_pkey_val % 1000000U;
                    (*p_key_len) = 3U;
                    break;

                case 0x02U:
                    /* Masking with 2^19 - 1 i.e. 0x7FFFF[524287] */
                    t_pkey_val &= 0x07FFFFU;
                    (*p_key_len) = 3U;
                    break;

                default:
                    /**
                     * Truncate length to 2 bytes, Passkey will range from
                     * 000000 - 065525
                     */
                    t_pkey_val &= 0x0000FFFFU;
                    (*p_key_len) = 2U;

                    break;
            }

            SMP_TRC (
            "[SMP]: Stack generated Passkey Value is"
            "0x%02X [in HEX], %u [in Decimal]\n",
            t_pkey_val, t_pkey_val);

            /* Pack 3 bytes of the passkey into the local */
            smp_pack_3_byte_param (p_key_ptr, &t_pkey_val);
        }
    }

    SMP_TRC (
    "[SMP]: << smp_revamp_passkey_pl, retval 0x%04X\n", retval);

    return retval;
}

#endif /* SMP */


