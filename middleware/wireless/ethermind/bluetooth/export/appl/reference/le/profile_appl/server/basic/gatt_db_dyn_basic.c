
/**
 *  \file gatt_db_dyn_basic.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_basic.h"

#if (defined ATT && defined BASIC)
#ifdef GATT_DB_DYNAMIC
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
/* Service Instances */
UINT16 GATT_SER_BASIC_GAP_INST;
UINT16 GATT_SER_BASIC_GATT_INST;
UINT16 GATT_SER_BASIC_BATTERY_INST;
UINT16 GATT_SER_BASIC_DEV_INFO_INST;
UINT16 GATT_SER_BASIC_TEST_SERV_A_INST;
UINT16 GATT_SER_BASIC_TEST_SERV_B_INST;

/** GAP Service Characteristic Instances */
UINT16 GATT_CHAR_BASIC_DEV_NAME_INST;
UINT16 GATT_CHAR_BASIC_APPEARANCE_INST;

/** GATT Service Characteristic Instances */
UINT16 GATT_CHAR_BASIC_SER_CHNGD_INST;
UINT16 GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST;
UINT16 GATT_CHAR_BASIC_DATABASE_HASH_INST;
UINT16 GATT_CHAR_BASIC_SERVER_SUPPORTED_INST;

/** Battery Service Characteristic Instances */
UINT16 GATT_CHAR_BASIC_BATTERY_LVL_INST;

/** Device Info Service Characteristic Instances */
UINT16 GATT_CHAR_BASIC_MAN_NAME_INST;
UINT16 GATT_CHAR_BASIC_MODEL_NO_INST;
UINT16 GATT_CHAR_BASIC_SL_NO_INST;
UINT16 GATT_CHAR_BASIC_FW_REV_INST;
UINT16 GATT_CHAR_BASIC_HW_REV_INST;
UINT16 GATT_CHAR_BASIC_SW_REV_INST;
UINT16 GATT_CHAR_BASIC_SYS_ID_INST;
UINT16 GATT_CHAR_BASIC_REG_CERT_DATA_INST;
UINT16 GATT_CHAR_BASIC_PNP_ID_INST;

/** Test Service-A Characteristic Instance */
UINT16 GATT_CHAR_BASIC_TEST_CHAR_A1_INST;

/** Test Service-B Characteristic Instance */
UINT16 GATT_CHAR_BASIC_TEST_CHAR_B1_INST;
UINT16 GATT_CHAR_BASIC_TEST_CHAR_B2_INST;

/**
 * GATT Service Client Supported Features Characteristic Value.
 *
 * NOTE:
 * - This characteristic will be maintained Peer Specific
 * - Application needs to handle access related to each peer instance.
 * - Value maintained at the application is not persistently stored
 *   inside by the stack. Application to handle this.
 *
 * Setting the below bits support to be "RESET"
 * 1. Robust Caching
 * 2. EATT Support
 * 3. Multiple Handle Value Notification Handling
 */
UCHAR appl_basic_csfc_value[BT_MAX_DEVICE_QUEUE_SIZE];

/**
 * GATT Service: Service Changed CCCD Value.
 */
UINT16 appl_basic_serv_chngd_cccd[BT_MAX_DEVICE_QUEUE_SIZE];

/**
 * GATT Service: Maintains the change aware state of each
 * Client device.
 */
UCHAR appl_basic_change_aware_state[BT_MAX_DEVICE_QUEUE_SIZE];

/**
 * GATT Service: Maintains the ATT Request counter of each
 * Client device.
 */
UCHAR appl_basic_request_counter[BT_MAX_DEVICE_QUEUE_SIZE];

/* --------------------------------------------- Static Global Variables */
/** GATT DB Hash Value */
DECL_STATIC UCHAR appl_basic_gatt_db_hash_value[GATT_DB_HASH_VALUE_LEN];
/**
 * GATT DB Server Supported Features Characteristic Value.
 *
 * NOTE:
 * Setting the below bits support to be "RESET"
 * - No EATT Supported by this current application.
 */
DECL_STATIC UCHAR appl_basic_ssfc_value = 0x00;

/* --------------------------------------------- Functions */

/**
 * Initialization function for Basic Dynamic Databases
 */
void appl_basic_dyn_db_init(void)
{
    UINT32 i;

    /* Initialize */
    GATT_SER_BASIC_GAP_INST               = 0xFFFF;
    GATT_SER_BASIC_GATT_INST              = 0xFFFF;
    GATT_SER_BASIC_BATTERY_INST           = 0xFFFF;
    GATT_SER_BASIC_DEV_INFO_INST          = 0xFFFF;

    GATT_CHAR_BASIC_DEV_NAME_INST         = 0xFFFF;
    GATT_CHAR_BASIC_APPEARANCE_INST       = 0xFFFF;

    GATT_CHAR_BASIC_SER_CHNGD_INST        = 0xFFFF;
    GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST = 0xFFFF;
    GATT_CHAR_BASIC_DATABASE_HASH_INST    = 0xFFFF;
    GATT_CHAR_BASIC_SERVER_SUPPORTED_INST = 0xFFFF;

    GATT_CHAR_BASIC_BATTERY_LVL_INST      = 0xFFFF;

    GATT_CHAR_BASIC_MAN_NAME_INST         = 0xFFFF;
    GATT_CHAR_BASIC_MODEL_NO_INST         = 0xFFFF;
    GATT_CHAR_BASIC_SL_NO_INST            = 0xFFFF;
    GATT_CHAR_BASIC_FW_REV_INST           = 0xFFFF;
    GATT_CHAR_BASIC_HW_REV_INST           = 0xFFFF;
    GATT_CHAR_BASIC_SW_REV_INST           = 0xFFFF;
    GATT_CHAR_BASIC_SYS_ID_INST           = 0xFFFF;
    GATT_CHAR_BASIC_REG_CERT_DATA_INST    = 0xFFFF;
    GATT_CHAR_BASIC_PNP_ID_INST           = 0xFFFF;

    appl_basic_ssfc_value                 = 0x00;

    BT_mem_set
    (
        appl_basic_gatt_db_hash_value,
        0x0,
        sizeof(appl_basic_gatt_db_hash_value)
    );

    for (i = 0; i < BT_MAX_DEVICE_QUEUE_SIZE; i++)
    {
        appl_basic_reset_serv_chngd_cccd((DEVICE_HANDLE)i);
    }

    for (i = 0; i < BT_MAX_DEVICE_QUEUE_SIZE; i++)
    {
        appl_basic_reset_csfc_value((DEVICE_HANDLE)i);
    }

    for (i = 0; i < BT_MAX_DEVICE_QUEUE_SIZE; i++)
    {
        /* Default state is Change Aware */
        appl_basic_change_aware_state[i] = BT_TRUE;
    }

    for (i = 0; i < BT_MAX_DEVICE_QUEUE_SIZE; i++)
    {
        /* Default state is 0 */
        appl_basic_request_counter[i] = 0;
    }
}

/**
 * This is to be called only for Device Handle's which are
 * not corresponding to Bonded Devices.
 */
void appl_basic_reset_serv_chngd_cccd(DEVICE_HANDLE dq_handle)
{
   appl_basic_serv_chngd_cccd[dq_handle] = 0x0000;
}

/**
 * This is to be called only for Device Handle's which are
 * not corresponding to Bonded Devices.
 */
void appl_basic_reset_csfc_value(DEVICE_HANDLE dq_handle)
{
    appl_basic_csfc_value[dq_handle] = 0x00;
}

void appl_basic_updt_serv_chngd_cccd(void)
{
    UINT32         i;
    ATT_VALUE      value;
    GATT_DB_HANDLE scv_db_hndl;
    API_RESULT     retval;

    /* Initialize with Service Change related values */
    scv_db_hndl.service_id = (UCHAR)GATT_SER_BASIC_GATT_INST;
    scv_db_hndl.char_id    = (UCHAR)GATT_CHAR_BASIC_SER_CHNGD_INST;

    for (i = 0; i < BT_MAX_DEVICE_QUEUE_SIZE; i++)
    {
        scv_db_hndl.device_id = (DEVICE_HANDLE)i;

        /* Get the CCCD Value Reference */
        retval = BT_gatt_db_get_char_cli_cnfg (&scv_db_hndl, &value);

        if (API_SUCCESS == retval)
        {
            /* PACK the locally maintained CCCD value to this reference */
            BT_PACK_LE_2_BYTE
            (
                value.val,
                &appl_basic_serv_chngd_cccd[i]
            );
        }
    }
}

void appl_basic_updt_csfc_value(void)
{
    UINT32         i;
    ATT_VALUE      value;
    GATT_DB_HANDLE csfc_db_hndl;

    /* Initialize with CSFC related values */
    csfc_db_hndl.service_id = (UCHAR)GATT_SER_BASIC_GATT_INST;
    csfc_db_hndl.char_id    = (UCHAR)GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST;

    for (i = 0; i < BT_MAX_DEVICE_QUEUE_SIZE; i++)
    {
        csfc_db_hndl.device_id = (DEVICE_HANDLE)i;

        /* Update the Value from the locally maintained list */
        value.val         = &appl_basic_csfc_value[i];
        value.len         = 1U;
        value.actual_len  = value.len;

        /**
         * Update the Value of the CSFC based on what is
         * stored in the application.
         * Currently nothing is done here. Its a dummy function.
         * This is handled when handling the Read Response.
         */
    }
}

API_RESULT appl_basic_add_gaps(void)
{
    API_RESULT           retval;
    GATT_DB_SERVICE_INFO service_info;
    GATT_DB_UUID_TYPE    char_uuid;
    UINT16               num_attr_handles;
    UINT16               service_handle;
    UINT16               perm;
    UINT16               property;
    ATT_VALUE            char_value;
    UINT16               char_handle;

    service_info.is_primary        = BT_TRUE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = GATT_GAP_SERVICE;
    service_info.link_req          = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
    service_info.sec_req           = GATT_DB_SER_NO_SECURITY_PROPERTY;

    num_attr_handles = 30U;

    retval = BT_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[BASIC APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save GAP Service Instance for future reference */
        GATT_SER_BASIC_GAP_INST      = service_handle;

        /* Device Name */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_DEVICE_NAME_CHARACTERISTIC;

        perm                  = GATT_DB_PERM_READ;
        property              = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val        = (UCHAR *)"MindtreeBasicGattDB";
        char_value.len        = (UINT16)BT_str_len(char_value.val);
        char_value.actual_len = char_value.len;

        retval = BT_gatt_db_add_characteristic
                 (
                     service_handle,
                     &char_uuid,
                     perm,
                     property,
                     &char_value,
                     &char_handle
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_BASIC_DEV_NAME_INST = char_handle;
        }

        /* Appearance */
        {
            /* Generic Thermometer = 0x0300 */
            UCHAR appearance_value[2U] = { 0x00U, 0x03U };

            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_APPEARANCE_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = appearance_value;
            char_value.len         = sizeof(appearance_value);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_APPEARANCE_INST = char_handle;
            }
        }
    }

    return retval;
}

static API_RESULT appl_add_cccd
                  (
                      UINT16    service_handle,
                      UINT16    char_handle
                  )
{
    API_RESULT        retval;
    GATT_DB_UUID_TYPE desc_uuid;
    UINT16            perm;
    ATT_VALUE         desc_value;
    UCHAR             cccd_value[2U] = { 0x00U, 0x00U };

    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = GATT_CLIENT_CONFIG;
    perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
    desc_value.val         = cccd_value;
    desc_value.len         = 2U;
    desc_value.actual_len  = desc_value.len;

    /* Add descriptor CCCD */
    retval = BT_gatt_db_add_characteristic_descriptor
             (
                 service_handle,
                 char_handle,
                 &desc_uuid,
                 perm,
                 &desc_value
             );

    return retval;
}

static API_RESULT appl_add_presentation_format
                  (
                      UINT16    service_handle,
                      UINT16    char_handle,
                      UCHAR   * presentation_format,
                      UINT16    format_length
                  )
{
    API_RESULT        retval;
    GATT_DB_UUID_TYPE desc_uuid;
    UINT16            perm;
    ATT_VALUE         desc_value;

    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = GATT_FORMAT;
    perm                   = 0U;
    desc_value.val         = presentation_format;
    desc_value.len         = format_length;
    desc_value.actual_len  = desc_value.len;

    /* Add Presentation Format descriptor */
    retval = BT_gatt_db_add_characteristic_descriptor
             (
                 service_handle,
                 char_handle,
                 &desc_uuid,
                 perm,
                 &desc_value
             );

    return retval;
}

API_RESULT appl_basic_add_gatts(void)
{
    API_RESULT           retval;
    GATT_DB_SERVICE_INFO service_info;
    UINT16               num_attr_handles;
    UINT16               service_handle;
    GATT_DB_UUID_TYPE    char_uuid;
    UINT16               perm;
    UINT16               property;
    ATT_VALUE            char_value;
    UINT16               char_handle;

    service_info.is_primary        = BT_TRUE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = GATT_GATT_SERVICE;
    service_info.link_req          = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
    service_info.sec_req           = GATT_DB_SER_NO_SECURITY_PROPERTY;

    num_attr_handles = 30U;

    retval = BT_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[BASIC APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save GATT Service Instance for future reference */
        GATT_SER_BASIC_GATT_INST = service_handle;

        /* Adding GATT Service Changed Characteristic */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_SERVICE_CHANGED_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_NONE;
        property               = GATT_DB_CHAR_INDICATE_PROPERTY;
        char_value.val         = NULL;
        char_value.len         = 0U;
        char_value.actual_len  = char_value.len;

        retval = BT_gatt_db_add_characteristic
                 (
                     service_handle,
                     &char_uuid,
                     perm,
                     property,
                     &char_value,
                     &char_handle
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_BASIC_SER_CHNGD_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: Service Changed CCCD Add failed. Result: 0x%04X\n",
                retval);
            }
        }

        /* Adding GATT Client Supported Features Characteristics */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_CLIENT_SUPP_FEATURES_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = (GATT_DB_CHAR_READ_PROPERTY |
                                  GATT_DB_CHAR_WRITE_PROPERTY);
        char_value.val         = NULL;
        char_value.len         = 0x00U;
        char_value.actual_len  = char_value.len;

        retval = BT_gatt_db_add_characteristic
                 (
                     service_handle,
                     &char_uuid,
                     perm,
                     property,
                     &char_value,
                     &char_handle
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST = char_handle;
        }

        /* Adding GATT Database Hash Characteristics */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_DATABASE_HASH_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val         = &appl_basic_gatt_db_hash_value[0];
        char_value.len         = sizeof(appl_basic_gatt_db_hash_value);
        char_value.actual_len  = char_value.len;

        retval = BT_gatt_db_add_characteristic
                 (
                     service_handle,
                     &char_uuid,
                     perm,
                     property,
                     &char_value,
                     &char_handle
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_BASIC_DATABASE_HASH_INST = char_handle;
        }

        /* Adding GATT Server Supported Features Characteristics */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_SERVER_SUPP_FEATURES_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val         = &appl_basic_ssfc_value;
        char_value.len         = sizeof(appl_basic_ssfc_value);
        char_value.actual_len  = char_value.len;

        retval = BT_gatt_db_add_characteristic
                 (
                     service_handle,
                     &char_uuid,
                     perm,
                     property,
                     &char_value,
                     &char_handle
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_BASIC_SERVER_SUPPORTED_INST = char_handle;
        }
    }

    return retval;
}

API_RESULT appl_basic_add_bas(void)
{
    API_RESULT           retval;
    GATT_DB_SERVICE_INFO service_info;
    UINT16               num_attr_handles;
    UINT16               service_handle;
    GATT_DB_UUID_TYPE    char_uuid;
    UINT16               perm;
    UINT16               property;
    ATT_VALUE            char_value;
    UINT16               char_handle;

    service_info.is_primary        = BT_TRUE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = GATT_BATTERY_SERVICE;
    service_info.link_req          = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
    service_info.sec_req           = GATT_DB_SER_NO_SECURITY_PROPERTY;

    num_attr_handles = 30U;

    retval = BT_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[BASIC APPL]: Device Info Service Add failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        UCHAR bat_level;

        /* Save Battery Service Instance for future reference */
        GATT_SER_BASIC_BATTERY_INST = service_handle;

        char_uuid.uuid_format     = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16    = GATT_BATTERY_LEVEL_CHARACTERISTIC;
        perm                      = GATT_DB_PERM_READ;
        property                  = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
        /* 100% */
        bat_level                 = (UCHAR)APPL_BATTERY_PERCENT;
        char_value.val            = &bat_level;
        char_value.len            = sizeof(bat_level);
        char_value.actual_len     = char_value.len;

        retval = BT_gatt_db_add_characteristic
                 (
                     service_handle,
                     &char_uuid,
                     perm,
                     property,
                     &char_value,
                     &char_handle
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Battery Level Characteristic Instance for future reference */
            GATT_CHAR_BASIC_BATTERY_LVL_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: Battery Level CCCD add failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                /* Presentation Format for BatteryLevel */
                /**
                 * Characteristic Presentation Format for BatteryLevel
                 * - Format            (1 octet)
                 * - Exponent          (1 octet)
                 * - Unit              (2 octet)
                 * - Name Space        (1 octet)
                 * - Description       (2 octet)
                 */
                UCHAR presentation_format[] = { 0x04U, 0x00U, 0xADU, 0x27U, 0x01U, 0x00U, 0x00U };

                /* Add Presentation Format */
                retval = appl_add_presentation_format
                         (
                             service_handle,
                             char_handle,
                             presentation_format,
                             sizeof(presentation_format)
                         );

                if (API_SUCCESS != retval)
                {
                     APPL_TRC(
                     "[BASIC APPL]: Battery CPF Desc Add failed. Result: 0x%04X\n",
                     retval);
                }
            }
        }
    }

    return retval;
}

API_RESULT appl_basic_add_dis(void)
{
    API_RESULT           retval;
    GATT_DB_SERVICE_INFO service_info;
    UINT16               num_attr_handles;
    UINT16               service_handle;
    GATT_DB_UUID_TYPE    char_uuid;
    UINT16               perm;
    UINT16               property;
    ATT_VALUE            char_value;
    UINT16               char_handle;

    service_info.is_primary        = BT_TRUE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = GATT_DEVICE_INFO_SERVICE;
    service_info.link_req          = GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
    service_info.sec_req           = GATT_DB_SER_NO_SECURITY_PROPERTY;

    num_attr_handles = 30U;

    retval = BT_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[BASIC APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);

        /* return retval; */
    }
    else
    {
        /* Save Device Info Service Instance for future reference */
        GATT_SER_BASIC_DEV_INFO_INST  = service_handle;

        /* ManufacturerName */
        {
            char_uuid.uuid_format     = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16    = GATT_MANUFACTURER_NAME_CHARACTERISTIC;

            perm                      = GATT_DB_PERM_READ;
            property                  = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val            = appl_manufacturer_name_ext;
            char_value.len            = sizeof(APPL_MANUFACTURER_NAME);
            char_value.actual_len     = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_MAN_NAME_INST = char_handle;
            }
        }

        /* Model Number */
        {
            /* GATT-DB-1.0.0 */
            UCHAR model_number[]   = { 'G', 'A', 'T', 'T', '-', 'D', 'B', '-', '1', '.', '0', '.', '0' };
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_MODEL_NUMBER_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = model_number;
            char_value.len         = sizeof(model_number);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_MODEL_NO_INST = char_handle;
            }
        }

        /* SerialNumber */
        {
            /* 1.0.0 */
            UCHAR serial_number[]  = { '1', '.', '0', '.', '0' };
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_SERIAL_NUMBER_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = serial_number;
            char_value.len         = sizeof(serial_number);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_SL_NO_INST = char_handle;
            }
        }

        /* FirmwareRevision */
        {
            /* 1.0.0 */
            UCHAR fw_revision[]    = { '1', '.', '0', '.', '0' };
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_FIRMWARE_REV_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = fw_revision;
            char_value.len         = sizeof(fw_revision);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_FW_REV_INST = char_handle;
            }
        }

        /* HardwareRevision */
        {
            /* 1.0.0 */
            UCHAR hw_revision[]    = { '1', '.', '0', '.', '0' };
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_HARDWARE_REV_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = hw_revision;
            char_value.len         = sizeof(hw_revision);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_HW_REV_INST = char_handle;
            }
        }

        /* SoftwareRevision */
        {
            /* 1.0.0 */
            UCHAR sw_revision[]    = { '1', '.', '0', '.', '0' };
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_SOFTWARE_REV_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = sw_revision;
            char_value.len         = sizeof(sw_revision);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_SW_REV_INST = char_handle;
            }
        }

        /* SystemId */
        {
            /* 11223344 */
            UCHAR sys_id[]         = { '1', '1', '2', '2', '3', '3', '4', '4' };
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_SYSTEM_ID_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = sys_id;
            char_value.len         = sizeof(sys_id);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_SYS_ID_INST = char_handle;
            }
        }

        /* RegCertDataList */
        {
            /* 11223344 */
            UCHAR cert[] = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
                            0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
                            0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U };
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_REG_CERT_DATA_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = cert;
            char_value.len         = sizeof(cert);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_REG_CERT_DATA_INST = char_handle;
            }
        }

        /* PnPID */
        {
            /* Vendor: 0x0025[NXP], Product: 0x0000, Version: 0x0001 */
            UCHAR pnp_id[]         = {0x01U, 0x25U, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U};
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_PNP_ID_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = pnp_id;
            char_value.len         = sizeof(pnp_id);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_BASIC_PNP_ID_INST = char_handle;
            }
        }
    }

    return retval;
}

/**
 * Test Service A:
 * This is a test service with following service properties
 *  - Secondary Service
 *  - Visible on LE only Transport
 *  - Security Requirements: Level 1, Mode 2, KeySz 16
 *  - Supports Data Signing Characteristic
 */
API_RESULT appl_basic_add_test_serv_a(void)
{
    API_RESULT           retval;
    GATT_DB_SERVICE_INFO service_info;
    GATT_DB_UUID_TYPE    char_uuid;
    UINT16               num_attr_handles;
    UINT16               service_handle;
    UINT16               perm;
    UINT16               property;
    ATT_VALUE            char_value;
    UINT16               char_handle;

    APPL_TRC (
    "[BASIC APPL]: >> appl_basic_add_test_serv_a\n");

    service_info.is_primary        = BT_FALSE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = APPL_BASIC_TEST_SERV_A_UUID;
    service_info.link_req          = GATT_DB_SER_SUPPORT_LE_LINK_TYPE;
    service_info.sec_req           = GATT_DB_SER_NO_SECURITY_PROPERTY;
    
    num_attr_handles = 30;

    retval = BT_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[BASIC APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Default Value */
        UCHAR dummy_char_val_a1[] = {0x00, 0x00};

        /* Save Test Service-A Instance for future reference */
        GATT_SER_BASIC_TEST_SERV_A_INST  = service_handle;

        /* Char A1 */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = APPL_BASIC_TEST_CHAR_A1_UUID;

        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY |
                                  GATT_DB_CHAR_SIGNED_WRITE_PROPERTY);
        char_value.val         = dummy_char_val_a1;
        char_value.len         = (UINT16)sizeof(dummy_char_val_a1);
        char_value.actual_len  = char_value.len;

        retval = BT_gatt_db_add_characteristic
                 (
                     service_handle,
                     &char_uuid,
                     perm,
                     property,
                     &char_value,
                     &char_handle
                 );

        if (API_SUCCESS != retval)
        {
            APPL_TRC(
            "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save the Characteristic ID */
            GATT_CHAR_BASIC_TEST_CHAR_A1_INST = char_handle;
        }
    }

    APPL_TRC (
    "[BASIC APPL]: << appl_basic_add_test_serv_a, retval 0x%04X\n", retval);

    return retval;
}

/**
 * Test Service B:
 * This is a test service with following service properties
 *  - Primary Service
 *  - Visible on LE only Transport
 *  - No Particular Security Requirements
 *  - Includes the Test Service A
 *  - Supports Characteristic Value beyond regular MTU
 */
API_RESULT appl_basic_add_test_serv_b(void)
{
    API_RESULT           retval;
    GATT_DB_SERVICE_INFO service_info;
    GATT_DB_UUID_TYPE    char_uuid;
    UINT16               num_attr_handles;
    UINT16               service_handle;
    UINT16               perm;
    UINT16               property;
    ATT_VALUE            char_value;
    UINT16               char_handle;

    APPL_TRC (
    "[BASIC APPL]: >> appl_basic_add_test_serv_b\n");

    service_info.is_primary        = BT_TRUE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = APPL_BASIC_TEST_SERV_B_UUID;
    service_info.link_req          = GATT_DB_SER_SUPPORT_LE_LINK_TYPE;
    service_info.sec_req           = GATT_DB_SER_NO_SECURITY_PROPERTY;

    num_attr_handles = 30;

    retval = BT_gatt_db_add_service
             (
                 &service_info,
                 num_attr_handles,
                 &service_handle
             );

    if (API_SUCCESS != retval)
    {
        APPL_TRC(
        "[BASIC APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Default Value - 25 Bytes */
        UCHAR dummy_char_val_b1[] = {0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                     0x00, 0x00, 0x00, 0x00,
                                     0x00};
        UCHAR dummy_char_val_b2[] = {0x00, 0x00};

        /* Save Test Service-B Instance for future reference */
        GATT_SER_BASIC_TEST_SERV_B_INST  = service_handle;

        /* Add Included Service here */
        retval = BT_gatt_db_add_included_service
                 (
                     GATT_SER_BASIC_TEST_SERV_B_INST,
                     GATT_SER_BASIC_TEST_SERV_A_INST
                 );

        if (API_SUCCESS == retval)
        {
            /* Char B1 */
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = APPL_BASIC_TEST_CHAR_B1_UUID;

            perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
            property               = (GATT_DB_CHAR_READ_PROPERTY |
                                      GATT_DB_CHAR_WRITE_PROPERTY);
            char_value.val         = dummy_char_val_b1;
            char_value.len         = (UINT16)sizeof(dummy_char_val_b1);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                /* Save the Characteristic ID */
                GATT_CHAR_BASIC_TEST_CHAR_B1_INST = char_handle;
            }

            /* Char B2 */
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = APPL_BASIC_TEST_CHAR_B2_UUID;

            perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
            property               = (GATT_DB_CHAR_READ_PROPERTY |
                                      GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
            char_value.val         = dummy_char_val_b2;
            char_value.len         = (UINT16)sizeof(dummy_char_val_b2);
            char_value.actual_len  = char_value.len;

            retval = BT_gatt_db_add_characteristic
                     (
                         service_handle,
                         &char_uuid,
                         perm,
                         property,
                         &char_value,
                         &char_handle
                     );

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[BASIC APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                /* Save the Characteristic ID */
                GATT_CHAR_BASIC_TEST_CHAR_B2_INST = char_handle;
            }
        }
    }

    APPL_TRC (
    "[BASIC APPL]: << appl_basic_add_test_serv_b, retval 0x%04X\n", retval);

    return retval;
}

#endif /* GATT_DB_DYNAMIC */
#endif /* (defined ATT && defined BASIC) */

