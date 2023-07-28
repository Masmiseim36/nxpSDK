
/**
 *  \file gatt_db_dyn_hid.c
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_hid.h"

#if (defined ATT && defined HID)
#ifdef GATT_DB_DYNAMIC
/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */
/* Service Instances */
UINT16 GATT_SER_HID_GATT_INST;
UINT16 GATT_SER_HID_GAP_INST;
UINT16 GATT_SER_HID_DEV_INFO_INST;
UINT16 GATT_SER_HID_BATTERY_INST;
UINT16 GATT_SER_HID_KEYBOARD_INST;
UINT16 GATT_SER_HID_MOUSE_INST;
UINT16 GATT_SER_HID_SCAN_PARAM_INST;

/* Characteristic Instances */
UINT16 GATT_CHAR_HID_BATTERY_LVL_INST;

/* 1st instance of HID characteristics */
UINT16 GATT_CHAR_HID_KEYBOARD_PROTO_MODE_INST;
UINT16 GATT_CHAR_HID_KEYBOARD_REPORT_MAP_INST;
UINT16 GATT_CHAR_HID_KEYBOARD_REPORT_INST0;
UINT16 GATT_CHAR_HID_KEYBOARD_REPORT_INST1;
UINT16 GATT_CHAR_HID_KEYBOARD_REPORT_INST2;
UINT16 GATT_CHAR_HID_KEYBOARD_CNTRL_PNT_INST;
UINT16 GATT_CHAR_HID_KEYBOARD_HID_INFO_INST;
UINT16 GATT_CHAR_HID_KEYBOARD_KBD_IN_INST;
UINT16 GATT_CHAR_HID_KEYBOARD_KBD_OUT_INST;

/* 2nd instance of HID characteristics */
UINT16 GATT_CHAR_HID_MOUSE_PROTO_MODE_INST;
UINT16 GATT_CHAR_HID_MOUSE_REPORT_MAP_INST;
UINT16 GATT_CHAR_HID_MOUSE_REPORT_INST0;
UINT16 GATT_CHAR_HID_MOUSE_REPORT_INST1;
UINT16 GATT_CHAR_HID_MOUSE_REPORT_INST2;
UINT16 GATT_CHAR_HID_MOUSE_CNTRL_PNT_INST;
UINT16 GATT_CHAR_HID_MOUSE_HID_INFO_INST;
UINT16 GATT_CHAR_HID_MOUSE_IN_INST;

UINT16 GATT_CHAR_HID_SCAN_INTRVL_WNDW_INST;
UINT16 GATT_CHAR_HID_SCAN_REFRESH_INST;

/* --------------------------------------------- Static Global Variables */
/* HID Characteristic Values related globals */
static UCHAR hid_rprtmap[] =
                      {
                          0x05U, 0x01U, 0x09U, 0x06U, 0xA1U, 0x01U, 0x05U, 0x07U,
                          0x19U, 0xE0U, 0x29U, 0xE7U, 0x15U, 0x00U, 0x25U, 0x01U,
                          0x75U, 0x01U, 0x95U, 0x08U, 0x81U, 0x02U, 0x95U, 0x01U,
                          0x75U, 0x08U, 0x81U, 0x03U, 0x95U, 0x05U, 0x75U, 0x01U,
                          0x05U, 0x08U, 0x19U, 0x01U, 0x29U, 0x05U, 0x91U, 0x02U,
                          0x95U, 0x01U, 0x75U, 0x03U, 0x91U, 0x03U, 0x95U, 0x06U,
                          0x75U, 0x08U, 0x15U, 0x00U, 0x25U, 0x65U, 0x05U, 0x07U,
                          0x19U, 0x00U, 0x29U, 0x65U, 0x81U, 0x00U, 0xC0U
                      };
static UCHAR protocol_mode            = 0x01U;
static UCHAR input_hid_rprt[]         = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
static UCHAR output_hid_rprt[]        = {0x00U, 0x00U, 0x00U};
static UCHAR feature_hid_rprt[]       = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
static UCHAR hid_info[]               = {0x13U, 0x02U, 0x40U, 0x01U};
static UCHAR boot_mouse_input_rprt[]  = {0x00U, 0x00U, 0x00U};
static UCHAR boot_kbd_input_rprt[]    = {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};
static UCHAR boot_kbd_output_rprt[]   = {0x00U, 0x00U, 0x00U};

/* --------------------------------------------- Functions */

API_RESULT appl_hid_add_gaps(void)
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
        "[HID APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Store the Service Handle in the Global */
        GATT_SER_HID_GAP_INST  = service_handle;

        /* Device Name */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_DEVICE_NAME_CHARACTERISTIC;

        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val         = (UCHAR *)"Mindtree-HID";
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
            "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }

        /* Appearance */
        {
            UCHAR appearance_value[2U] = { 0x00U, 0x00U };

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
                "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
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

static API_RESULT appl_add_sccd
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
    desc_uuid.uuid.uuid_16 = GATT_SERVER_CONFIG;

    perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);

    desc_value.val         = cccd_value;
    desc_value.len         = 2U;
     desc_value.actual_len = desc_value.len;

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

API_RESULT appl_hid_add_gatts(void)
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
        "[HID APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        /* Save GATT Service Instance for future reference */
        GATT_SER_HID_GATT_INST  = service_handle;

        char_uuid.uuid_format   = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16  = GATT_SERVICE_CHANGED_CHARACTERISTIC;

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
            "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
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

API_RESULT appl_hid_add_bas(void)
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
        "[HID APPL] BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        UCHAR bat_level;

        /* Save Battery Service Instance for future reference */
        GATT_SER_HID_BATTERY_INST  = service_handle;

        char_uuid.uuid_format      = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16     = GATT_BATTERY_LEVEL_CHARACTERISTIC;

        perm                       = GATT_DB_PERM_READ;
        property                   = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);

        /* 100% */
        bat_level                  = (UCHAR)APPL_BATTERY_PERCENT;
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
            "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Battery Level Characteristic Instance for future reference */
            GATT_CHAR_HID_BATTERY_LVL_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            if (API_SUCCESS != retval)
            {
                APPL_TRC(
                "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
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

/**
 * The is a reference sample of DIS with few associated characteristics
 * added here along with other Sevice that comprise of the HID.
 * Currently the Characteristics Added to DIS are:
 * 1. Manufacturer Name
 * 2. Model Number
 *
 * All other DIS specific characteristics are not part of this application.
 */
API_RESULT appl_hid_add_dis(void)
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
        "[HID APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);

        /* return retval; */
    }
    else
    {
        /* Save Device Info Service Instance for future reference */
        GATT_SER_HID_DEV_INFO_INST  = service_handle;

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
                "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
        }

        /* Model Number */
        {
            /* HID-1.0.0 */
            UCHAR model_number[]   = { 'H', 'I', 'D', '-', '1', '.', '0', '.', '0' };
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
                "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
                retval);
            }
        }
    }

    return retval;
}

API_RESULT appl_hid_add_hid_keyboard(void)
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
    service_info.uuid.uuid.uuid_16 = GATT_HID_SERVICE;
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
        "[HID APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        GATT_SER_HID_KEYBOARD_INST      = service_handle;

        /* HIDProtocolMode */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_PROTO_MODE_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
        char_value.val         = &protocol_mode;
        char_value.len         = sizeof(protocol_mode);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Protocol Mode */
            GATT_CHAR_HID_KEYBOARD_PROTO_MODE_INST = char_handle;
        }

        /* HIDReportMap */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_MAP_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val         = hid_rprtmap;
        char_value.len         = sizeof(hid_rprtmap);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Report Map */
            GATT_CHAR_HID_KEYBOARD_REPORT_MAP_INST = char_handle;

            /* Add Profile defined descriptor */
            retval = appl_add_hid_report_map_hld_desp(service_handle, char_handle);
        }

        /* Input HIDReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
        char_value.val         = input_hid_rprt;
        char_value.len         = sizeof(input_hid_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Input Report Map */
            GATT_CHAR_HID_KEYBOARD_REPORT_INST0 = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            /* Add Profile defined descriptor */
            retval = appl_add_ip_report_ref_hld_desp(service_handle, char_handle);
        }

        /* Output HIDReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
        char_value.val         = output_hid_rprt;
        char_value.len         = sizeof(output_hid_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Output HID Report */
            GATT_CHAR_HID_KEYBOARD_REPORT_INST1 = char_handle;

            /* Add Profile defined descriptor */
            retval = appl_add_op_report_ref_hld_desp(service_handle, char_handle);
        }

        /* Feature HIDReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY);
        char_value.val         = feature_hid_rprt;
        char_value.len         = sizeof(feature_hid_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Feature HID Report Map */
            GATT_CHAR_HID_KEYBOARD_REPORT_INST2 = char_handle;

            /* Add Profile defined descriptor */
            retval = appl_add_feature_report_ref_hld_desp(service_handle, char_handle);
        }

        /* HIDControlPoint */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_CP_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_NONE;
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID control point */
            GATT_CHAR_HID_KEYBOARD_CNTRL_PNT_INST = char_handle;
        }

        /* HIDInformation */
        /* Mindtree Limited */
        char_uuid.uuid_format   = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16  = GATT_HID_INFO_CHARACTERISTIC;
        perm                    = GATT_DB_PERM_READ;
        property                = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val          = hid_info;
        char_value.len          = sizeof(hid_info);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Information */
            GATT_CHAR_HID_KEYBOARD_HID_INFO_INST = char_handle;
        }

        /* BootKeyboardInputReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_BOOT_KYBRD_IN_RPT_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
        char_value.val         = boot_kbd_input_rprt;
        char_value.len         = sizeof(boot_kbd_input_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* HID Boot Keyboard Input Report */
            GATT_CHAR_HID_KEYBOARD_KBD_IN_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);
        }

        /* BootKeyboardOutputReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_BOOT_KYBRD_OP_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE | GATT_DB_PERM_NONE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
        char_value.val         = boot_kbd_output_rprt;
        char_value.len         = sizeof(boot_kbd_output_rprt);
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
            "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
             retval);
        }
        else
        {
            /* Save HID Boot Keyboard Output Report */
            GATT_CHAR_HID_KEYBOARD_KBD_OUT_INST = char_handle;
        }
    }

    return API_SUCCESS;
}

API_RESULT appl_hid_add_hid_mouse(void)
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
    service_info.uuid.uuid.uuid_16 = GATT_HID_SERVICE;
    service_info.link_req = GATT_DB_SER_SUPPORT_LE_LINK_TYPE;
    service_info.sec_req = GATT_DB_SER_NO_SECURITY_PROPERTY;

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
        "[HID APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        GATT_SER_HID_MOUSE_INST = service_handle;

        /* HIDProtocolMode */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_PROTO_MODE_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
        char_value.val         = &protocol_mode;
        char_value.len         = sizeof(protocol_mode);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Protocol Mode */
            GATT_CHAR_HID_MOUSE_PROTO_MODE_INST = char_handle;
        }

        /* HIDReportMap */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_MAP_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val         = hid_rprtmap;
        char_value.len         = sizeof(hid_rprtmap);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Report Map */
            GATT_CHAR_HID_MOUSE_REPORT_MAP_INST = char_handle;

            /* Add Profile defined descriptor */
            retval = appl_add_hid_report_map_hld_desp(service_handle, char_handle);
        }

        /* Input HIDReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
        char_value.val         = input_hid_rprt;
        char_value.len         = sizeof(input_hid_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Input Report Map */
            GATT_CHAR_HID_MOUSE_REPORT_INST0 = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);

            /* Add Profile defined descriptor */
            retval = appl_add_ip_report_ref_hld_desp(service_handle, char_handle);
        }

        /* Output HIDReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY);
        char_value.val         = output_hid_rprt;
        char_value.len         = sizeof(output_hid_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Output HID Report */
            GATT_CHAR_HID_MOUSE_REPORT_INST1 = char_handle;

            /* Add Profile defined descriptor */
            retval = appl_add_op_report_ref_hld_desp(service_handle, char_handle);
        }

        /* Feature HIDReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_RPT_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY);
        char_value.val         = feature_hid_rprt;
        char_value.len         = sizeof(feature_hid_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Feature HID Report Map */
            GATT_CHAR_HID_MOUSE_REPORT_INST2 = char_handle;

            /* Add Profile defined descriptor */
            retval = appl_add_feature_report_ref_hld_desp(service_handle, char_handle);
        }

        /* HIDControlPoint */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_HID_CP_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_NONE;
        property               = GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY;
        char_value.val         = NULL;
        char_value.len         = 0;
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID control point */
            GATT_CHAR_HID_MOUSE_CNTRL_PNT_INST = char_handle;
        }

        /* HIDInformation */
        /* Mindtree Limited */
        char_uuid.uuid_format   = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16  = GATT_HID_INFO_CHARACTERISTIC;
        perm                    = GATT_DB_PERM_READ;
        property                = GATT_DB_CHAR_READ_PROPERTY;
        char_value.val          = hid_info;
        char_value.len          = sizeof(hid_info);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Information */
            GATT_CHAR_HID_MOUSE_HID_INFO_INST = char_handle;
        }

        /* BootMouseInputReport */
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_MOUSE_IP_CHARACTERISTIC;
        perm                   = (GATT_DB_PERM_READ | GATT_DB_PERM_WRITE);
        property               = GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_WRITE_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY;
        char_value.val         = boot_mouse_input_rprt;
        char_value.len         = sizeof(boot_mouse_input_rprt);
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
            "[ HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save HID Boot Mouse Input Report */
            GATT_CHAR_HID_MOUSE_IN_INST = char_handle;

            /* Add CCCD */
            retval = appl_add_cccd(service_handle, char_handle);
        }
    }

    return API_SUCCESS;
}

API_RESULT appl_add_scps(void)
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
    UCHAR scan_interval_window[] = {0x40U, 0x00U, 0x10U, 0x00U};
    UCHAR scan_refresh = 0x00U;

    service_info.is_primary        = BT_TRUE;
    service_info.uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    service_info.uuid.uuid.uuid_16 = GATT_SCAN_PARAM_SERVICE;
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
        "[HID APPL]: BT_gatt_db_add_service() failed. Result: 0x%04X\n",
        retval);
    }
    else
    {
        GATT_SER_HID_SCAN_PARAM_INST  = service_handle;

        /* ScanIntervalWindow*/
        /* Mindtree Limited */
        char_uuid.uuid_format         = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16        = GATT_SCAN_WINDOW_CHARACTERISTIC;
        perm                          = GATT_DB_PERM_NONE;
        property                      = GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY;
        char_value.val                = scan_interval_window;
        char_value.len                = sizeof(scan_interval_window);
        char_value.actual_len         = char_value.len;

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
            "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Save Scan Interval Window */
            GATT_CHAR_HID_SCAN_INTRVL_WNDW_INST = char_handle;
        }

        /* ScanRefresh*/
        /* Mindtree Limited */
        char_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
        char_uuid.uuid.uuid_16 = GATT_SCAN_REFRESH_CHARACTERISTIC;
        perm                   = GATT_DB_PERM_READ;
        property               = (GATT_DB_CHAR_READ_PROPERTY | GATT_DB_CHAR_NOTIFY_PROPERTY);
        char_value.val         = &scan_refresh;
        char_value.len         = sizeof(scan_refresh);
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
            "[HID APPL]: BT_gatt_db_add_characteristic() failed. Result: 0x%04X\n",
            retval);
        }
        else
        {
            /* Scan Refresh Instance */
            GATT_CHAR_HID_SCAN_REFRESH_INST = char_handle;
        }

        /* Add CCCD */
        retval = appl_add_cccd(service_handle, char_handle);
    }

    return API_SUCCESS;
}

API_RESULT appl_add_ip_report_ref_hld_desp
       (
           UINT16    service_handle,
           UINT16    char_handle
       )
{
    API_RESULT        retval;
    GATT_DB_UUID_TYPE desc_uuid;
    UINT16            perm;
    ATT_VALUE         desc_value;

    /* Valid Range Descriptor   */
    UCHAR value[]          = { 0x00U, 0x01U};

    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = GATT_IP_OP_FEATURE_REPORT_REF;

    perm                   = GATT_DB_PERM_READ;

    desc_value.val         = value;
    desc_value.len         = sizeof(value);
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

API_RESULT appl_add_op_report_ref_hld_desp
           (
               UINT16    service_handle,
               UINT16    char_handle
           )
{
    API_RESULT        retval;
    GATT_DB_UUID_TYPE desc_uuid;
    UINT16            perm;
    ATT_VALUE         desc_value;

    /* Valid Range Descriptor  */
    UCHAR value[]          = { 0x00U, 0x02U };

    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = GATT_IP_OP_FEATURE_REPORT_REF;

    perm                   = GATT_DB_PERM_READ;

    desc_value.val         = value;
    desc_value.len         = sizeof(value);
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

API_RESULT appl_add_feature_report_ref_hld_desp
           (
               UINT16    service_handle,
               UINT16    char_handle
           )
{
    API_RESULT        retval;
    GATT_DB_UUID_TYPE desc_uuid;
    UINT16            perm;
    ATT_VALUE         desc_value;

    /* Valid Range Descriptor   */
    UCHAR value[]          = { 0x00U, 0x03U };

    desc_uuid.uuid_format  = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16 = GATT_IP_OP_FEATURE_REPORT_REF;

    perm                   = GATT_DB_PERM_READ;

    desc_value.val         = value;
    desc_value.len         = sizeof(value);
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

API_RESULT appl_add_hid_report_map_hld_desp
           (
               UINT16    service_handle,
               UINT16    char_handle
           )
{
    API_RESULT        retval;
    GATT_DB_UUID_TYPE desc_uuid;
    UINT16            perm;
    ATT_VALUE         desc_value;

    /* Valid Range Descriptor   */
    UCHAR value[]           = { 0x00U, 0x00U };

    desc_uuid.uuid_format   = ATT_16_BIT_UUID_FORMAT;
    desc_uuid.uuid.uuid_16  = GATT_EXTERNAL_REPORT_REF;

    perm                    = GATT_DB_PERM_READ;

    desc_value.val          = value;
    desc_value.len          = sizeof(value);
    desc_value.actual_len   = desc_value.len;

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

#endif /* GATT_DB_DYNAMIC */
#endif /* (defined ATT && defined HID) */
