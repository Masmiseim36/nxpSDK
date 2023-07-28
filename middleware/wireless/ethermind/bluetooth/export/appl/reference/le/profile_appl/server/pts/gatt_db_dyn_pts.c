
/**
 *  \file gatt_db_dyn_pts.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/*--------------------------------------------- Header File Inclusion */
#include "appl_pts.h"

#if (defined ATT && defined PTS)
#ifdef GATT_DB_DYNAMIC
/*--------------------------------------------- External Global Variables */

/*--------------------------------------------- Exported Global Variables */

/* Service Instances */
UINT16 GATT_SER_PTS_GAP_INST;
UINT16 GATT_SER_PTS_GATT_INST;
UINT16 GATT_SER_PTS_BATTERY_INST;
UINT16 GATT_SER_PTS_DEV_INFO_INST;
UINT16 GATT_SER_PTS_1_DUMMY_INST;
UINT16 GATT_SER_PTS_2_DUMMY_INST;
UINT16 GATT_SER_PTS_3_DUMMY_INST;
UINT16 GATT_SER_PTS_4_DUMMY_INST;
UINT16 GATT_SER_PTS_5_DUMMY_INST;
UINT16 GATT_SER_PTS_6_DUMMY_INST;
UINT16 GATT_SER_PTS_7_DUMMY_INST;
UINT16 GATT_SER_PTS_8_DUMMY_INST;
UINT16 GATT_SER_PTS_9_DUMMY_INST;

/* Characteristic Instances */
UINT16 GATT_CHAR_PTS_DEV_NAME_INST;
UINT16 GATT_CHAR_PTS_APPEARANCE_INST;
UINT16 GATT_CHAR_PTS_PPCP_INST;
UINT16 GATT_CHAR_PTS_CAR_INST;
UINT16 GATT_CHAR_PTS_RPA_ONLY_INST;
UINT16 GATT_CHAR_PTS_SER_CHNGD_INST;
UINT16 GATT_CHAR_PTS_CLIENT_SUPPORTED_INST;
UINT16 GATT_CHAR_PTS_DATABASE_HASH_INST;
UINT16 GATT_CHAR_PTS_SERVER_SUPPORTED_INST;

UINT16 GATT_CHAR_PTS_BATTERY_LVL_INST;

UINT16 GATT_CHAR_PTS_MAN_NAME_INST;
UINT16 GATT_CHAR_PTS_MODEL_NO_INST;
UINT16 GATT_CHAR_PTS_SL_NO_INST;
UINT16 GATT_CHAR_PTS_FW_REV_INST;
UINT16 GATT_CHAR_PTS_HW_REV_INST;
UINT16 GATT_CHAR_PTS_SW_REV_INST;
UINT16 GATT_CHAR_PTS_SYS_ID_INST;
UINT16 GATT_CHAR_PTS_REG_CERT_DATA_INST;
UINT16 GATT_CHAR_PTS_PNP_ID_INST;

UINT16 GATT_CHAR_PTS_1_DUMMY_INST;
UINT16 GATT_CHAR_PTS_2_DUMMY_INST;
UINT16 GATT_CHAR_PTS_3_DUMMY_INST;
UINT16 GATT_CHAR_PTS_4_DUMMY_INST;
UINT16 GATT_CHAR_PTS_5_DUMMY_INST;
UINT16 GATT_CHAR_PTS_6_DUMMY_INST;
UINT16 GATT_CHAR_PTS_7_DUMMY_INST;
UINT16 GATT_CHAR_PTS_8_DUMMY_INST;
UINT16 GATT_CHAR_PTS_9_DUMMY_INST;
UINT16 GATT_CHAR_PTS_10_DUMMY_INST;
UINT16 GATT_CHAR_PTS_11_DUMMY_INST;
UINT16 GATT_CHAR_PTS_12_DUMMY_INST;
UINT16 GATT_CHAR_PTS_13_DUMMY_INST;
UINT16 GATT_CHAR_PTS_14_DUMMY_INST;
UINT16 GATT_CHAR_PTS_15_DUMMY_INST;
UINT16 GATT_CHAR_PTS_16_DUMMY_INST;
UINT16 GATT_CHAR_PTS_17_DUMMY_INST;
UINT16 GATT_CHAR_PTS_18_DUMMY_INST;
UINT16 GATT_CHAR_PTS_19_DUMMY_INST;
UINT16 GATT_CHAR_PTS_20_DUMMY_INST;
UINT16 GATT_CHAR_PTS_21_DUMMY_INST;
UINT16 GATT_CHAR_PTS_22_DUMMY_INST;
UINT16 GATT_CHAR_PTS_23_DUMMY_INST;

/** GATT DB Hash Value */
UCHAR appl_pts_gatt_db_hash_value[GATT_DB_HASH_VALUE_LEN];

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT appl_pts_add_gaps(void)
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
        "[PTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save GAP Service Instance for future reference */
        GATT_SER_PTS_GAP_INST      = service_handle;

        /* Device Name */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_DEVICE_NAME_CHARACTERISTIC;

        perm = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_VARIABLE_LENGTH_AUX_PROPERTY);

        char_value.val         = (UCHAR *)"Mindtree-PTS";
        char_value.len         = (UINT16)BT_str_len(char_value.val);
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
            "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_PTS_DEV_NAME_INST = char_handle;
        }

        /* Appearance */
        {
            /* Generic Thermometer = 0x0300 */
            UCHAR appearance_value[2U] = { 0x00U, 0x03U };

            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_APPEARANCE_CHARACTERISTIC;

            perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
            property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_FIXED_LENGTH_PROPERTY);

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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_APPEARANCE_INST = char_handle;
            }
        }

        /* Peripheral Preferred Connection Parameters Characteristic */
        {
            UCHAR pref_conn_param[] = { 0x40U, 0x00U, 0x40U, 0x00U, 0x00U, 0x00U, 0x80U, 0x0CU };
    
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_PRFRRD_CNXN_PARAM_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val         = pref_conn_param;
            char_value.len         = sizeof(pref_conn_param);
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_PPCP_INST = char_handle;
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
    UCHAR             cccd_value[2] = { 0x00U, 0x00U };

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

API_RESULT appl_pts_add_gatts(void)
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
        "[PTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save GATT Service Instance for future reference */
        GATT_SER_PTS_GATT_INST = service_handle;

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
            "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_PTS_SER_CHNGD_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[PTS APPL]: appl_add_cccd() failed. Result: 0x%04X\n",
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
            "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_PTS_CLIENT_SUPPORTED_INST = char_handle;
        }

        /* Adding GATT Database Hash Characteristics */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_DATABASE_HASH_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val         = appl_pts_gatt_db_hash_value;
        char_value.len         = sizeof(appl_pts_gatt_db_hash_value);
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
            "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_PTS_DATABASE_HASH_INST = char_handle;
        }

        /* Adding GATT Server Supported Features Characteristics */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_SERVER_SUPP_FEATURES_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
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
            "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            GATT_CHAR_PTS_SERVER_SUPPORTED_INST = char_handle;
        }
    }

    return retval;
}

API_RESULT appl_pts_add_bas(void)
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
        "[PTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        UCHAR bat_level;

        /* Save Battery Service Instance for future reference */
        GATT_SER_PTS_BATTERY_INST = service_handle;

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
            "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Battery Level Characteristic Instance for future reference */
            GATT_CHAR_PTS_BATTERY_LVL_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[PTS APPL]: appl_add_cccd() failed. Result: 0x%04X\n",
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
                     "[PTS APPL]: appl_add_presentation_format() failed. Result: 0x%04X\n",
                     retval);
                }
            }
        }
    }

    return retval;
}


API_RESULT appl_pts_add_dis(void)
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
        "[PTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save Device Info Service Instance for future reference */
        GATT_SER_PTS_DEV_INFO_INST  = service_handle;

        /* ManufacturerName */
        {
            /* Mindtree Limited */
            UCHAR manufacturer_name[] = { 'M', 'i', 'n', 'd', 't', 'r', 'e', 'e', ' ', 'L', 'i', 'm', 'i', 't', 'e', 'd' };
            char_uuid.uuid_format     = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16    = GATT_MANUFACTURER_NAME_CHARACTERISTIC;

            perm                      = GATT_DB_PERM_READ;
            property                  = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val            = manufacturer_name;
            char_value.len            = sizeof(manufacturer_name);
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_MAN_NAME_INST = char_handle;
            }
        }

        /* Model Number */
        {
            /* PTS-X.X.X */
            UCHAR model_number[]   = { 'P', 'T', 'S', '-', 'X', '.', 'X', '.', 'X' };
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_MODEL_NO_INST = char_handle;
            }
        }

        /* SerialNumber */
        {
            /* X.X.X */
            UCHAR serial_number[]  = { 'X', '.', 'X', '.', 'X' };
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_SL_NO_INST = char_handle;
            }
        }

        /* FirmwareRevision */
        {
            /* X.X.X */
            UCHAR fw_revision[]    = { 'X', '.', 'X', '.', 'X' };
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_FW_REV_INST = char_handle;
            }
        }

        /* HardwareRevision */
        {
            /* X.X.X */
            UCHAR hw_revision[]    = { 'X', '.', 'X', '.', 'X' };
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_HW_REV_INST = char_handle;
            }
        }

        /* SoftwareRevision */
        {
            /* X.X.X */
            UCHAR sw_revision[]    = { 'X', '.', 'X', '.', 'X' };
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_SW_REV_INST = char_handle;
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_SYS_ID_INST = char_handle;
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_REG_CERT_DATA_INST = char_handle;
            }
        }

        /* PnPID */
        {
            /* Vendor: 0x006A[Mindtree], Product: 0x014D, Version: 0x100 */
            UCHAR pnp_id[]         = {0x01U, 0x6AU, 0x00U, 0x4DU, 0x01U, 0x00U, 0x01U};
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
                "[PTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                GATT_CHAR_PTS_PNP_ID_INST = char_handle;
            }
        }
    }

    return retval;
}

#endif /* GATT_DB_DYNAMIC */
#endif /* (defined ATT && defined PTS) */

