
/**
 *  \file gatt_db_dyn_hts.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_hts.h"

#if (defined ATT && defined HTS)
#ifdef GATT_DB_DYNAMIC
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
/* Service Instances */
UINT16 GATT_SER_HTS_GAP_INST;
UINT16 GATT_SER_HTS_GATT_INST;
UINT16 GATT_SER_HTS_BATTERY_INST;
UINT16 GATT_SER_HTS_DEV_INFO_INST;
UINT16 GATT_SER_HTS_HEALTH_THERM_INST;

/* Characteristic Instances */
UINT16 GATT_CHAR_HTS_BATTERY_LVL_INST;
UINT16 GATT_CHAR_HTS_TEMP_MSRMNT_INST;
UINT16 GATT_CHAR_HTS_INTRM_TEMP_INST;
UINT16 GATT_CHAR_HTS_MSRMT_INTERVAL_INST;
UINT16 GATT_DB_MAX_ATTRIBUTES;
/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */

API_RESULT appl_hts_add_gaps(void)
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
        "[HTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save GAP Service Instance for future reference */
        GATT_SER_HTS_GAP_INST      = service_handle;

        /* Device Name */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_DEVICE_NAME_CHARACTERISTIC;

        perm                  = GATT_DB_PERM_READ;
        property              = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val        = (UCHAR *)"Mindtree-HTS";
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
            "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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

static API_RESULT appl_add_measurement_interval_char_hld_desc
                  (
                      UINT16    service_handle,
                      UINT16    char_handle
                  )
{
    API_RESULT        retval;
    GATT_DB_UUID_TYPE desc_uuid;
    UINT16            perm;
    ATT_VALUE         desc_value;
    UCHAR             range_value[4U];
    UINT16            range_min, range_max;

    range_min              = APPL_MSRMT_INTRVL_DEFAULT_RANGE_MIN;
    range_max              = APPL_MSRMT_INTRVL_DEFAULT_RANGE_MAX;
    BT_PACK_LE_2_BYTE(&range_value[0U], &range_min);
    BT_PACK_LE_2_BYTE(&range_value[2U], &range_max);

    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = GATT_VALID_RANGE;
    perm                   = GATT_DB_PERM_READ;
    desc_value.val         = range_value;
    desc_value.len         = sizeof(range_value);
    desc_value.actual_len  = desc_value.len;

    /* Add descriptor HLD */
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

API_RESULT appl_hts_add_gatts(void)
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
        "[HTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save GATT Service Instance for future reference */
        GATT_SER_HTS_GATT_INST = service_handle;

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
            "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);
        }
    }

    return retval;
}

API_RESULT appl_hts_add_bas(void)
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
        "[HTS ADEV_INFO_INST_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        UCHAR bat_level;

        /* Save Battery Service Instance for future reference */
        GATT_SER_HTS_BATTERY_INST = service_handle;

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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Battery Level Characteristic Instance for future reference */
            GATT_CHAR_HTS_BATTERY_LVL_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
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
            }
        }
    }

    return retval;
}

API_RESULT appl_hts_add_dis(void)
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
        "[HTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);

        /* return retval; */
    }
    else
    {
        /* Save Device Info Service Instance for future reference */
        GATT_SER_HTS_DEV_INFO_INST  = service_handle;

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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
        }

        /* Model Number */
        {
            /* HTS-1.0.0 */
            UCHAR model_number[]   = { 'H', 'T', 'S', '-', '1', '.', '0', '.', '0' };
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
        }
    }

    return retval;
}

API_RESULT appl_hts_add_hts(void)
{
    API_RESULT           retval = API_FAILURE;
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
    service_info.uuid.uuid.uuid_16 = GATT_HEALTH_THERMOMETER_SERVICE;
    service_info.link_req          = GATT_DB_SER_SUPPORT_LE_LINK_TYPE;
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
        "[HTS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);

        /* return retval; */
    }
    else
    {
        /* Save HTS Service Instance for future reference */
        GATT_SER_HTS_HEALTH_THERM_INST = service_handle;

        /* TemperatureMeasurement */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_TEMPERATURE_MSMNT_CHARACTERISTIC;
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
            "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save TemperatureMeasurement Characteristic Instance for future reference */
            GATT_CHAR_HTS_TEMP_MSRMNT_INST = char_handle;
        }

        /* Add CCCD */
        retval = appl_add_cccd(service_handle, char_handle);

        /* IntermediateTemperature */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_INTERMEDIATE_TEMP_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_NONE;
        property               = GATT_DB_CHAR_NOTIFY_PROPERTY;
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
            "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save IntermediateTemperature Characteristic Instance for future reference */
            GATT_CHAR_HTS_INTRM_TEMP_INST = char_handle;
        }

        /* Add CCCD */
        retval = appl_add_cccd(service_handle, char_handle);

        {
            /* TemperatureType */
            UCHAR temp_type;
            temp_type = 0x01U;

            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_TEMPERATURE_TYPE_CHARACTERISTIC;
            perm                   = GATT_DB_PERM_NONE;
            property               = GATT_DB_CHAR_READ_PROPERTY;
            char_value.val         = &temp_type;
            char_value.len         = sizeof(temp_type);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
        }

        {
            /* Measurement Interval */
            UCHAR  msrmnt_interval[2U];
            UINT16 interval_val;

            /* Setting the measurement interval to default value */
            interval_val           = APPL_TEMPERATURE_MEASUREMENT_INTERVAL;
            BT_PACK_LE_2_BYTE(msrmnt_interval, &interval_val);

            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_MSMNT_INTERVAL_CHARATACTERISTIC;
            perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
            property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_INDICATE_PROPERTY);
            char_value.val         = msrmnt_interval;
            char_value.len         = sizeof(msrmnt_interval);
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
                "[HTS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
            else
            {
                /* Save Measurement Interval Characteristic Instance for future reference */
                GATT_CHAR_HTS_MSRMT_INTERVAL_INST = char_handle;
            }
        }
        /* Add CCCD */
        retval = appl_add_cccd(service_handle, char_handle);

        /* Add HLD */
        retval = appl_add_measurement_interval_char_hld_desc(service_handle, char_handle);
    }

    return retval;
}
#endif /* GATT_DB_DYNAMIC */
#endif /* (defined ATT && defined HTS) */

