
/**
 *  \file gatt_db_stat_ipspn.c
 *  GATT Databse.
 */

/**
 *  GATT DATABASE QUICK REFERENCE TABLE:
 *  Abbreviations used for Permission:
 *      Rd  = Read
 *      WwR = Write Without Response
 *      Wr  = Write
 *      Swr  = Signed Write
 *      Ntf = Notification
 *      Ind = Indication
 *
 *  Handle  |      ATT_Type      |    Permission    |  ATT_VALUE
 *  ========+====================+==================+=====================
 *  0x0001  |       0x2800       | Rd               | 0x1800
 *  --------+--------------------+------------------+---------------------
 *  0x0002  |       0x2803       | Rd               | 0x02, 0x0003, 0x2A00
 *  --------+--------------------+------------------+---------------------
 *  0x0003  |       0x2A00       | Rd               | MindtreeHealthThermometer
 *  --------+--------------------+------------------+---------------------
 *  0x0004  |       0x2803       | Rd               | 0x02, 0x0005, 0x2A01
 *  --------+--------------------+------------------+---------------------
 *  0x0005  |       0x2A01       | Rd               | 768.0
 *  ========+====================+==================+=====================
 *  0x0006  |       0x2800       | Rd               | 0x1801
 *  --------+--------------------+------------------+---------------------
 *  0x0007  |       0x2803       | Rd               | 0x20, 0x0008, 0x2A05
 *  --------+--------------------+------------------+---------------------
 *  0x0008  |       0x2A05       | Ind              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0009  |       0x2902       | Rd, Wr           | 0x0000
 *  ========+====================+==================+=====================
 *  0x000A  |       0x2800       | Rd               | 0x180F
 *  --------+--------------------+------------------+---------------------
 *  0x000B  |       0x2803       | Rd               | 0x12, 0x000C, 0x2A19
 *  --------+--------------------+------------------+---------------------
 *  0x000C  |       0x2A19       | Rd, Ntf          | 100.0
 *  --------+--------------------+------------------+---------------------
 *  0x000D  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x000E  |       0x2904       | Rd               | VALUE
 *  ========+====================+==================+=====================
 *  0x000F  |       0x2800       | Rd               | 0x1820
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "BT_gatt_db_api.h"
#include "gatt_db.h"
#include "gatt_defines.h"

#ifdef ATT
#ifndef GATT_DB_DYNAMIC
#if ((defined IPSPN) && (defined GATT_DB))

/* ----------------------------------------- Global Variables */
#ifndef GATT_DB_HAVE_STATIC_CONFIGURATION
DECL_CONST UCHAR  gatt_service_count = GATT_SERVICE_COUNT;
DECL_CONST UCHAR  gatt_characteristic_count = GATT_CHARACTERISTIC_COUNT;
DECL_CONST UCHAR  gatt_db_max_type_count = GATT_DB_MAX_TYPE_COUNT;
DECL_CONST UCHAR  gatt_db_peer_val_arr_size = GATT_DB_PEER_VALUE_ARRAY_SIZE;

#ifdef STORAGE_RETENTION_SUPPORT
DECL_CONST UINT16 gatt_value_array_size = GATT_VALUE_ARRAY_SIZE;
DECL_CONST UINT16 gatt_db_max_peer_config = GATT_DB_MAX_PEER_CONFIGURATION;
#endif /* STORAGE_RETENTION_SUPPORT */
#endif /* GATT_DB_HAVE_STATIC_CONFIGURATION */

DECL_CONST UCHAR gatt_const_uuid_arr[GATT_UUID_ARRAY_SIZE] =
{
    /* 0 - Primary Service UUID */
    0x00U, 0x28U,

    /* 2 - Secondary Service UUID */
    0x01U, 0x28U,

    /* 4 - Include UUID */
    0x02U, 0x28U,

    /* 6 - Characteristic UUID */
    0x03U, 0x28U,

    /* 8 - Characteristic Extended Properties UUID */
    0x00U, 0x29U,

    /* 10 - Characteristic User Description UUID */
    0x01U, 0x29U,

    /* 12 - Client Configuration UUID */
    0x02U, 0x29U,

    /* 14 - Server Configuration UUID */
    0x03U, 0x29U,

    /* 16 - Characteristic Format UUID */
    0x04U, 0x29U,

    /* 18 - Characteristic Aggregate Format UUID */
    0x05U, 0x29U,

    /* 20 - GAP Service UUID */
    0x00U, 0x18U,

    /* 22 - DeviceName Characteristic UUID */
    0x00U, 0x2AU,

    /* 24 - Appearance Characteristic UUID */
    0x01U, 0x2AU,

    /* 26 - GATT Service UUID */
    0x01U, 0x18U,

    /* 28 - Service Changed Characteristic UUID */
    0x05U, 0x2AU,

    /* 30 - Battery Service UUID */
    0x0FU, 0x18U,

    /* 32 - BatteryLevel Characteristic UUID */
    0x19U, 0x2AU,

    /* 34 - IPSP Service UUID */
    0x20U, 0x18U,

};


UCHAR gatt_value_arr[GATT_VALUE_ARRAY_SIZE] =
{
    /* 0 - BatteryLevel(3); Default: 100.0 */
    0x64U,
};


DECL_CONST UCHAR gatt_const_value_arr[GATT_CONST_VALUE_ARRAY_SIZE] =
{

    /* 0 - DeviceName(0); Property: 0x02, Value Handle: 0x0003, UUID: 0x2A00 */
    0x02U, 0x03U, 0x00U, 0x00U, 0x2AU,

    /* 5 - DeviceName(0); Default: MindtreeHealthThermometer */
    0x4DU, 0x69U, 0x6EU, 0x64U, 0x74U, 0x72U, 0x65U, 0x65U, 0x48U, 0x65U, 0x61U, 0x6CU,
    0x74U, 0x68U, 0x54U, 0x68U, 0x65U, 0x72U, 0x6DU, 0x6FU, 0x6DU, 0x65U, 0x74U, 0x65U,
    0x72U,

    /* 30 - Appearance(1); Property: 0x02, Value Handle: 0x0005, UUID: 0x2A01 */
    0x02U, 0x05U, 0x00U, 0x01U, 0x2AU,

    /* 35 - Appearance(1); Default: 768.0 */
    0x03U, 0x00U,

    /* 37 - Service Changed(2); Property: 0x20, Value Handle: 0x0008, UUID: 0x2A05 */
    0x20U, 0x08U, 0x00U, 0x05U, 0x2AU,

    /* 42 - BatteryLevel(3); Property: 0x12, Value Handle: 0x000C, UUID: 0x2A19 */
    0x12U, 0x0CU, 0x00U, 0x19U, 0x2AU,

    /* 47 - Presentation Fromat for BatteryLevel */
    /**
     * Characteristic Presentation Format for BatteryLevel
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x04U, 0x00U, 0xADU, 0x27U, 0x01U, 0x00U, 0x00U,
};


UCHAR gatt_db_peer_specific_val_arr[GATT_DB_MAX_PEER_CONFIGURATION];

DECL_CONST UCHAR gatt_db_const_peer_specific_val_arr[GATT_DB_PEER_VALUE_ARRAY_SIZE] =
{
    /* 0 - Client Configuration for Service Changed */
    /* Client Instance 0 */
    0x00U, 0x00U,

    /* 2 - Client Configuration for BatteryLevel */
    /* Client Instance 0 */
    0x00U, 0x00U,
};


DECL_CONST GATT_ATTR_TYPE gatt_type_table[GATT_DB_MAX_TYPE_COUNT] =
{
    {
        /* UUID Offset */
        0U,

        /* First Occurence for Type */
        1U,

        /* Last Occurence for Type */
        15U,

    },
    {
        /* UUID Offset */
        2U,

        /* First Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        4U,

        /* First Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        6U,

        /* First Occurence for Type */
        2U,

        /* Last Occurence for Type */
        11U,

    },
    {
        /* UUID Offset */
        12U,

        /* First Occurence for Type */
        9U,

        /* Last Occurence for Type */
        13U,

    },
    {
        /* UUID Offset */
        16U,

        /* First Occurence for Type */
        14U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        20U,

        /* First Occurence for Type */
        1U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        22U,

        /* First Occurence for Type */
        3U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        24U,

        /* First Occurence for Type */
        5U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        26U,

        /* First Occurence for Type */
        6U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        28U,

        /* First Occurence for Type */
        8U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        30U,

        /* First Occurence for Type */
        10U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        32U,

        /* First Occurence for Type */
        12U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        34U,

        /* First Occurence for Type */
        15U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
};


DECL_CONST GATT_DB_DESC_DATA gatt_db_attr_table[GATT_DB_MAX_ATTRIBUTES] =
{
    /* Dummy */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* Value */
        NULL,
    },

    /* Handle - 0x0001 */
    /* 0 - GAP - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        6U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[20U]),
    },

    /* Handle - 0x0002 */
    /* 0 - DeviceName - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        4U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[0U]),
    },

    /* Handle - 0x0003 */
    /* 0 - DeviceName - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        25U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        22U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[5U]),
    },

    /* Handle - 0x0004 */
    /* 1 - Appearance - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        7U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[30U]),
    },

    /* Handle - 0x0005 */
    /* 1 - Appearance - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        24U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[35U]),
    },

    /* Handle - 0x0006 */
    /* 1 - GATT - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        10U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[26U]),
    },

    /* Handle - 0x0007 */
    /* 2 - Service Changed - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        11U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[37U]),
    },

    /* Handle - 0x0008 */
    /* 2 - Service Changed - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_INDICATE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length */
        0U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        28U,

        /* Value */
        NULL,
    },

    /* Handle - 0x0009 */
    /* 2 - Service Changed - CCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        13U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[0U],
    },

    /* Handle - 0x000A */
    /* 2 - Battery - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        15U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[30U]),
    },

    /* Handle - 0x000B */
    /* 3 - BatteryLevel - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[42U]),
    },

    /* Handle - 0x000C */
    /* 3 - BatteryLevel - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        32U,

        /* Value */
        &gatt_value_arr[0U],
    },

    /* Handle - 0x000D */
    /* 3 - BatteryLevel - CCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[2U],
    },

    /* Handle - 0x000E */
    /* Characteristic 3 - BatteryLevel - Presentation Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        16U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[47U]),
    },

    /* Handle - 0x000F */
    /* 3 - IPSP - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[34U]),
    },

};


DECL_CONST GATT_DB_CHARACERISTIC gatt_characteristic[GATT_CHARACTERISTIC_COUNT] =
{

    /* 0 - DeviceName */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0002U,

        /* Characteristic Service Index */
        0U,
    },

    /* 1 - Appearance */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0004U,

        /* Characteristic Service Index */
        0U,
    },

    /* 2 - Service Changed */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0007U,

        /* Characteristic Service Index */
        1U,
    },

    /* 3 - BatteryLevel */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4U,
        },

        /* Characteristic Start Handle */
        0x000BU,

        /* Characteristic Service Index */
        2U,
    },
};


DECL_CONST GATT_DB_SERVICE gatt_service[GATT_SERVICE_COUNT] =
{

    /* 0 - GAP */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0001U,

        /* Service End Handle */
        0x0005U,

        /* Characteristic Start Index */
        0U,

        /* Characteristic End Index */
        1U
    },

    /* 1 - GATT */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0006U,

        /* Service End Handle */
        0x0009U,

        /* Characteristic Start Index */
        2U,

        /* Characteristic End Index */
        2U
    },

    /* 2 - Battery */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x000AU,

        /* Service End Handle */
        0x000EU,

        /* Characteristic Start Index */
        3U,

        /* Characteristic End Index */
        3U
    },

    /* 3 - IPSP */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x000FU,

        /* Service End Handle */
        0x000FU,

        /* Characteristic Start Index */
        4U,

        /* Characteristic End Index */
        3U
    },
};

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_ipspn_gatt_db_register(void)
{
    GATT_DB_STRUCT   gatt_db;
    API_RESULT       retval;

    gatt_db.gatt_service = gatt_service;
    gatt_db.gatt_characteristic = gatt_characteristic;
    gatt_db.gatt_const_value_arr = gatt_const_value_arr;
    gatt_db.gatt_const_uuid_arr = gatt_const_uuid_arr;
    gatt_db.gatt_db_attr_table = gatt_db_attr_table;
    gatt_db.gatt_type_table = gatt_type_table;
    gatt_db.gatt_db_peer_specific_val_arr = gatt_db_peer_specific_val_arr;
    gatt_db.gatt_db_const_peer_specific_val_arr = gatt_db_const_peer_specific_val_arr;
    gatt_db.gatt_value_arr = gatt_value_arr;

    gatt_db.gatt_service_count = GATT_SERVICE_COUNT;
    gatt_db.gatt_characteristic_count = GATT_CHARACTERISTIC_COUNT;
    gatt_db.gatt_db_max_type_count = GATT_DB_MAX_TYPE_COUNT;
    gatt_db.gatt_db_peer_val_arr_size = GATT_DB_PEER_VALUE_ARRAY_SIZE;

#ifdef STORAGE_RETENTION_SUPPORT
    gatt_db.gatt_value_array_size = GATT_VALUE_ARRAY_SIZE;
    gatt_db.gatt_db_max_peer_config = GATT_DB_MAX_PEER_CONFIG;
#endif /* STORAGE_RETENTION_SUPPORT */

    retval = BT_gatt_db_register
             (
                 &gatt_db
             );
}
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

#endif /* ((defined IPSPN) && (defined GATT_DB)) */
#endif /* GATT_DB_DYNAMIC */
#endif /* ATT */
