
/**
 *  \file gatt_db_dyn_pass.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_pass.h"

#if (defined ATT && defined PASS)
#ifdef GATT_DB_DYNAMIC
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
/* Service Instances */
UINT16 GATT_SER_PASS_GAP_INST;
UINT16 GATT_SER_PASS_GATT_INST;
UINT16 GATT_SER_PASS_BATTERY_INST;
UINT16 GATT_SER_PASS_PHN_ALRT_STATUS_INST;

/* Characteristic Instances */
UINT16 GATT_CHAR_PASS_BATTERY_LVL_INST;
UINT16 GATT_CHAR_PASS_ALRT_STATUS_INST;
UINT16 GATT_CHAR_PASS_RING_SET_INST;
UINT16 GATT_CHAR_PASS_RING_CTRL_PNT_INST;

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- Functions */
API_RESULT appl_pass_add_gaps(void)
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
            "[PASS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
            retval);
    }
    else
    {
        /* Store the Service Handle in the Global */
        GATT_SER_PASS_GAP_INST = service_handle;
        /* Device Name */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_DEVICE_NAME_CHARACTERISTIC;

        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val         = (UCHAR *)"Mindtree-PASS";
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
                "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
        }

        /* Appearance */
        {
            /* Generic Thermometer = 0x0300 */
            UCHAR appearance_value[2U] = { 0x00U, 0x03U };

            char_uuid.uuid_format     = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16    = GATT_APPEARANCE_CHARACTERISTIC;

            perm                      = GATT_DB_PERM_READ;
            property                  = GATT_DB_CHAR_READ_PROPERTY;

            char_value.val            = appearance_value;
            char_value.len            = sizeof(appearance_value);
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
                    "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
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
    API_RESULT           retval;

    GATT_DB_UUID_TYPE    desc_uuid;
    UINT16               perm;
    ATT_VALUE            desc_value;

    UCHAR cccd_value[2U]    = { 0x00U, 0x00U };

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
API_RESULT appl_pass_add_gatts(void)
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
            "[PASS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
            retval);
    }
    else
    {
        /* Store the Service Handle in the Global */
        GATT_SER_PASS_GATT_INST  = service_handle;
        char_uuid.uuid_format    = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16   = GATT_SERVICE_CHANGED_CHARACTERISTIC;

        perm                    = GATT_DB_PERM_NONE;
        property                = GATT_DB_CHAR_INDICATE_PROPERTY;

        char_value.val          = NULL;
        char_value.len          = 0U;
        char_value.actual_len   = char_value.len;

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
                "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
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

API_RESULT appl_pass_add_bas(void)
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
            "[PASS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
            retval);
    }
    else
    {
        UCHAR bat_level;

        /* Save Battery Service Instance for future reference */
        GATT_SER_PASS_BATTERY_INST = service_handle;

        char_uuid.uuid_format      = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16     = GATT_BATTERY_LEVEL_CHARACTERISTIC;

        perm                       = GATT_DB_PERM_READ;
        property                   = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);

        /* 100% */
        bat_level                  = 100U;
        char_value.val             = &bat_level;
        char_value.len             = sizeof(bat_level);
        char_value.actual_len      = char_value.len;

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
                "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
        }
        else
        {
            /* Save Battery Level Characteristic Instance for future reference */
            GATT_CHAR_PASS_BATTERY_LVL_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                    "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
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

API_RESULT appl_pass_add_pass(void)
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
    service_info.uuid.uuid.uuid_16 = GATT_PHONE_ALERT_STATUS_SERVICE;
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
            "[PASS APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
            retval);

        /* return retval; */
    }
    else
    {
        /* Save PASS Service Instance for future reference */
        GATT_SER_PASS_PHN_ALRT_STATUS_INST = service_handle;

        {
            /* AlertStatus */
            UCHAR alert_status     = 1U;

            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_ALERT_STATUS_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY;

            char_value.val         = &alert_status;
            char_value.len         = 1U;
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
                    "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                    retval);
            }
            else
            {
                /* Save AlertStatus Characteristic Instance for future reference */
                GATT_CHAR_PASS_ALRT_STATUS_INST = char_handle;
            }

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);
        }

        {
            /*  RingerSetting Characteristic */
            UCHAR ringer_setting   = 1U;

            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_RINGER_SETTING_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_READ;
            property               = GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY;

            char_value.val         = &ringer_setting;
            char_value.len         = 1U;
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
                    "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                    retval);
            }
            else
            {
                /* Save RingerSetting Characteristic Instance for future reference */
                GATT_CHAR_PASS_RING_SET_INST = char_handle;
            }

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);
        }

        {
            /* RingerControlPoint */
            char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
            char_uuid.uuid.uuid_16 = GATT_RINGER_CONTROL_POINT_CHARACTERISTIC;

            perm                   = GATT_DB_PERM_WRITE;
            property               = GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY;

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
                    "[PASS APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                    retval);
            }
            else
            {
                /* Save RingerControlPoint Characteristic Instance for future reference */
                GATT_CHAR_PASS_RING_CTRL_PNT_INST = char_handle;
            }
        }
    }
    return retval;

}
#endif /* GATT_DB_DYNAMIC */
#endif /* (defined ATT && defined PASS) */
