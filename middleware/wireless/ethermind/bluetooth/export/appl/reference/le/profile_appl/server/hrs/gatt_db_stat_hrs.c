
/**
 *  \file gatt_db_stat_hrs.c
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
 *  0x0003  |       0x2A00       | Rd               | MindtreeHeartRateBelt
 *  --------+--------------------+------------------+---------------------
 *  0x0004  |       0x2803       | Rd               | 0x02, 0x0005, 0x2A01
 *  --------+--------------------+------------------+---------------------
 *  0x0005  |       0x2A01       | Rd               | 833.0
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
 *  0x000F  |       0x2800       | Rd               | 0x180A
 *  --------+--------------------+------------------+---------------------
 *  0x0010  |       0x2803       | Rd               | 0x02, 0x0011, 0x2A29
 *  --------+--------------------+------------------+---------------------
 *  0x0011  |       0x2A29       | Rd               | Mindtree Limited.
 *  --------+--------------------+------------------+---------------------
 *  0x0012  |       0x2803       | Rd               | 0x02, 0x0013, 0x2A24
 *  --------+--------------------+------------------+---------------------
 *  0x0013  |       0x2A24       | Rd               | HRS-1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0014  |       0x2803       | Rd               | 0x02, 0x0015, 0x2A25
 *  --------+--------------------+------------------+---------------------
 *  0x0015  |       0x2A25       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0016  |       0x2803       | Rd               | 0x02, 0x0017, 0x2A26
 *  --------+--------------------+------------------+---------------------
 *  0x0017  |       0x2A26       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0018  |       0x2803       | Rd               | 0x02, 0x0019, 0x2A27
 *  --------+--------------------+------------------+---------------------
 *  0x0019  |       0x2A27       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x001A  |       0x2803       | Rd               | 0x02, 0x001B, 0x2A28
 *  --------+--------------------+------------------+---------------------
 *  0x001B  |       0x2A28       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x001C  |       0x2803       | Rd               | 0x02, 0x001D, 0x2A23
 *  --------+--------------------+------------------+---------------------
 *  0x001D  |       0x2A23       | Rd               | 11223344.0
 *  --------+--------------------+------------------+---------------------
 *  0x001E  |       0x2803       | Rd               | 0x02, 0x001F, 0x2A2A
 *  --------+--------------------+------------------+---------------------
 *  0x001F  |       0x2A2A       | Rd               | 0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0020  |       0x2803       | Rd               | 0x02, 0x0021, 0x2A50
 *  --------+--------------------+------------------+---------------------
 *  0x0021  |       0x2A50       | Rd               | 0
 *  ========+====================+==================+=====================
 *  0x0022  |       0x2800       | Rd               | 0x180D
 *  --------+--------------------+------------------+---------------------
 *  0x0023  |       0x2803       | Rd               | 0x10, 0x0024, 0x2A37
 *  --------+--------------------+------------------+---------------------
 *  0x0024  |       0x2A37       | Ntf              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0025  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0026  |       0x2803       | Rd               | 0x02, 0x0027, 0x2A38
 *  --------+--------------------+------------------+---------------------
 *  0x0027  |       0x2A38       | Rd               | 0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0028  |       0x2803       | Rd               | 0x08, 0x0029, 0x2A39
 *  --------+--------------------+------------------+---------------------
 *  0x0029  |       0x2A39       | Wr               | 0.0
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
#if ((defined BT_HRS) && (defined GATT_DB))

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

    /* 34 - DeviceInformation Service UUID */
    0x0AU, 0x18U,

    /* 36 - ManufacturerName Characteristic UUID */
    0x29U, 0x2AU,

    /* 38 - ModelNumber Characteristic UUID */
    0x24U, 0x2AU,

    /* 40 - SerialNumber Characteristic UUID */
    0x25U, 0x2AU,

    /* 42 - FirmwareRevision Characteristic UUID */
    0x26U, 0x2AU,

    /* 44 - HardwareRevision Characteristic UUID */
    0x27U, 0x2AU,

    /* 46 - SoftwareRevision Characteristic UUID */
    0x28U, 0x2AU,

    /* 48 - SystemId Characteristic UUID */
    0x23U, 0x2AU,

    /* 50 - RegCertDataList Characteristic UUID */
    0x2AU, 0x2AU,

    /* 52 - PnPID Characteristic UUID */
    0x50U, 0x2AU,

    /* 54 - Heart Rate Service UUID */
    0x0DU, 0x18U,

    /* 56 - HeartRateMeasurement Characteristic UUID */
    0x37U, 0x2AU,

    /* 58 - BodySensorLocation Characteristic UUID */
    0x38U, 0x2AU,

    /* 60 - HeartRateControlPoint Characteristic UUID */
    0x39U, 0x2AU,

};


UCHAR gatt_value_arr[GATT_VALUE_ARRAY_SIZE] =
{
    /* 0 - BatteryLevel(3); Default: 100.0 */
    0x64U,

    /* 1 - BodySensorLocation(14); Default: 0.0 */
    0x00U,
};


DECL_CONST UCHAR gatt_const_value_arr[GATT_CONST_VALUE_ARRAY_SIZE] =
{

    /* 0 - DeviceName(0); Property: 0x02, Value Handle: 0x0003, UUID: 0x2A00 */
    0x02U, 0x03U, 0x00U, 0x00U, 0x2AU,

    /* 5 - DeviceName(0); Default: MindtreeHeartRateBelt */
    0x4DU, 0x69U, 0x6EU, 0x64U, 0x74U, 0x72U, 0x65U, 0x65U, 0x48U, 0x65U, 0x61U, 0x72U,
    0x74U, 0x52U, 0x61U, 0x74U, 0x65U, 0x42U, 0x65U, 0x6CU, 0x74U,

    /* 26 - Appearance(1); Property: 0x02, Value Handle: 0x0005, UUID: 0x2A01 */
    0x02U, 0x05U, 0x00U, 0x01U, 0x2AU,

    /* 31 - Appearance(1); Default: 833.0 */
    0x03U, 0x41U,

    /* 33 - Service Changed(2); Property: 0x20, Value Handle: 0x0008, UUID: 0x2A05 */
    0x20U, 0x08U, 0x00U, 0x05U, 0x2AU,

    /* 38 - BatteryLevel(3); Property: 0x12, Value Handle: 0x000C, UUID: 0x2A19 */
    0x12U, 0x0CU, 0x00U, 0x19U, 0x2AU,

    /* 43 - Presentation Fromat for BatteryLevel */
    /**
     * Characteristic Presentation Format for BatteryLevel
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x04U, 0x00U, 0xADU, 0x27U, 0x01U, 0x00U, 0x00U,

    /* 50 - ManufacturerName(4); Property: 0x02, Value Handle: 0x0011, UUID: 0x2A29 */
    0x02U, 0x11U, 0x00U, 0x29U, 0x2AU,

    /* 55 - ManufacturerName(4); Default: Mindtree Limited. */
    0x4DU, 0x69U, 0x6EU, 0x64U, 0x74U, 0x72U, 0x65U, 0x65U, 0x20U, 0x4CU, 0x69U, 0x6DU,
    0x69U, 0x74U, 0x65U, 0x64U, 0x2EU,

    /* 72 - ModelNumber(5); Property: 0x02, Value Handle: 0x0013, UUID: 0x2A24 */
    0x02U, 0x13U, 0x00U, 0x24U, 0x2AU,

    /* 77 - ModelNumber(5); Default: HRS-1.0.0 */
    0x48U, 0x52U, 0x53U, 0x2DU, 0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 86 - SerialNumber(6); Property: 0x02, Value Handle: 0x0015, UUID: 0x2A25 */
    0x02U, 0x15U, 0x00U, 0x25U, 0x2AU,

    /* 91 - SerialNumber(6); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 96 - FirmwareRevision(7); Property: 0x02, Value Handle: 0x0017, UUID: 0x2A26 */
    0x02U, 0x17U, 0x00U, 0x26U, 0x2AU,

    /* 101 - FirmwareRevision(7); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 106 - HardwareRevision(8); Property: 0x02, Value Handle: 0x0019, UUID: 0x2A27 */
    0x02U, 0x19U, 0x00U, 0x27U, 0x2AU,

    /* 111 - HardwareRevision(8); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 116 - SoftwareRevision(9); Property: 0x02, Value Handle: 0x001B, UUID: 0x2A28 */
    0x02U, 0x1BU, 0x00U, 0x28U, 0x2AU,

    /* 121 - SoftwareRevision(9); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 126 - SystemId(10); Property: 0x02, Value Handle: 0x001D, UUID: 0x2A23 */
    0x02U, 0x1DU, 0x00U, 0x23U, 0x2AU,

    /* 131 - SystemId(10); Default: 11223344.0 */
    0x31U, 0x31U, 0x32U, 0x32U, 0x33U, 0x33U, 0x34U, 0x34U,

    /* 139 - RegCertDataList(11); Property: 0x02, Value Handle: 0x001F, UUID: 0x2A2A */
    0x02U, 0x1FU, 0x00U, 0x2AU, 0x2AU,

    /* 144 - RegCertDataList(11); Default: 0.0 */
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    /* 176 - PnPID(12); Property: 0x02, Value Handle: 0x0021, UUID: 0x2A50 */
    0x02U, 0x21U, 0x00U, 0x50U, 0x2AU,

    /* 181 - PnPID(12); Default: 0 */
    0x01U, 0x25U, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U,

    /* 188 - HeartRateMeasurement(13); Property: 0x10, Value Handle: 0x0024, UUID: 0x2A37 */
    0x10U, 0x24U, 0x00U, 0x37U, 0x2AU,

    /* 193 - BodySensorLocation(14); Property: 0x02, Value Handle: 0x0027, UUID: 0x2A38 */
    0x02U, 0x27U, 0x00U, 0x38U, 0x2AU,

    /* 198 - HeartRateControlPoint(15); Property: 0x08, Value Handle: 0x0029, UUID: 0x2A39 */
    0x08U, 0x29U, 0x00U, 0x39U, 0x2AU,
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

    /* 4 - Client Configuration for HeartRateMeasurement */
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
        34U,

    },
    {
        /* UUID Offset */
        6U,

        /* First Occurence for Type */
        2U,

        /* Last Occurence for Type */
        40U,

    },
    {
        /* UUID Offset */
        12U,

        /* First Occurence for Type */
        9U,

        /* Last Occurence for Type */
        37U,

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
    {
        /* UUID Offset */
        36U,

        /* First Occurence for Type */
        17U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        38U,

        /* First Occurence for Type */
        19U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        40U,

        /* First Occurence for Type */
        21U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        42U,

        /* First Occurence for Type */
        23U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        44U,

        /* First Occurence for Type */
        25U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        46U,

        /* First Occurence for Type */
        27U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        48U,

        /* First Occurence for Type */
        29U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        50U,

        /* First Occurence for Type */
        31U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        52U,

        /* First Occurence for Type */
        33U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        54U,

        /* First Occurence for Type */
        34U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        56U,

        /* First Occurence for Type */
        36U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        58U,

        /* First Occurence for Type */
        39U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        60U,

        /* First Occurence for Type */
        41U,

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
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        21U,

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
        (UCHAR *)(&gatt_const_value_arr[26U]),
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
        (UCHAR *)(&gatt_const_value_arr[31U]),
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
        (UCHAR *)(&gatt_const_value_arr[33U]),
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
        16U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[38U]),
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
        37U,

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
        (UCHAR *)(&gatt_const_value_arr[43U]),
    },

    /* Handle - 0x000F */
    /* 3 - DeviceInformation - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        34U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[34U]),
    },

    /* Handle - 0x0010 */
    /* 4 - ManufacturerName - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        18U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[50U]),
    },

    /* Handle - 0x0011 */
    /* 4 - ManufacturerName - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        sizeof(APPL_MANUFACTURER_NAME),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        36U,

        /* Value */
        (UCHAR *)(&appl_manufacturer_name_ext),
    },

    /* Handle - 0x0012 */
    /* 5 - ModelNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        20U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[72U]),
    },

    /* Handle - 0x0013 */
    /* 5 - ModelNumber - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        9U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        38U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[77U]),
    },

    /* Handle - 0x0014 */
    /* 6 - SerialNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        22U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[86U]),
    },

    /* Handle - 0x0015 */
    /* 6 - SerialNumber - Characteristic Value */
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
        40U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[91U]),
    },

    /* Handle - 0x0016 */
    /* 7 - FirmwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        24U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[96U]),
    },

    /* Handle - 0x0017 */
    /* 7 - FirmwareRevision - Characteristic Value */
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
        42U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[101U]),
    },

    /* Handle - 0x0018 */
    /* 8 - HardwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        26U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[106U]),
    },

    /* Handle - 0x0019 */
    /* 8 - HardwareRevision - Characteristic Value */
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
        44U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[111U]),
    },

    /* Handle - 0x001A */
    /* 9 - SoftwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        28U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[116U]),
    },

    /* Handle - 0x001B */
    /* 9 - SoftwareRevision - Characteristic Value */
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
        46U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[121U]),
    },

    /* Handle - 0x001C */
    /* 10 - SystemId - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        30U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[126U]),
    },

    /* Handle - 0x001D */
    /* 10 - SystemId - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        8U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        48U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[131U]),
    },

    /* Handle - 0x001E */
    /* 11 - RegCertDataList - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        32U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[139U]),
    },

    /* Handle - 0x001F */
    /* 11 - RegCertDataList - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        32U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        50U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[144U]),
    },

    /* Handle - 0x0020 */
    /* 12 - PnPID - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        35U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[176U]),
    },

    /* Handle - 0x0021 */
    /* 12 - PnPID - Characteristic Value */
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
        52U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[181U]),
    },

    /* Handle - 0x0022 */
    /* 4 - Heart Rate - Service Declaration */
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
        (UCHAR *)(&gatt_const_uuid_arr[54U]),
    },

    /* Handle - 0x0023 */
    /* 13 - HeartRateMeasurement - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        38U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[188U]),
    },

    /* Handle - 0x0024 */
    /* 13 - HeartRateMeasurement - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length */
        0U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        56U,

        /* Value */
        NULL,
    },

    /* Handle - 0x0025 */
    /* 13 - HeartRateMeasurement - CCD */
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
        &gatt_db_peer_specific_val_arr[4U],
    },

    /* Handle - 0x0026 */
    /* 14 - BodySensorLocation - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        40U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[193U]),
    },

    /* Handle - 0x0027 */
    /* 14 - BodySensorLocation - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        58U,

        /* Value */
        &gatt_value_arr[1U],
    },

    /* Handle - 0x0028 */
    /* 15 - HeartRateControlPoint - Characteristic Declaration */
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
        (UCHAR *)(&gatt_const_value_arr[198U]),
    },

    /* Handle - 0x0029 */
    /* 15 - HeartRateControlPoint - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length */
        0U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        60U,

        /* Value */
        NULL,
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

    /* 4 - ManufacturerName */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0010U,

        /* Characteristic Service Index */
        3U,
    },

    /* 5 - ModelNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0012U,

        /* Characteristic Service Index */
        3U,
    },

    /* 6 - SerialNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0014U,

        /* Characteristic Service Index */
        3U,
    },

    /* 7 - FirmwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0016U,

        /* Characteristic Service Index */
        3U,
    },

    /* 8 - HardwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0018U,

        /* Characteristic Service Index */
        3U,
    },

    /* 9 - SoftwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x001AU,

        /* Characteristic Service Index */
        3U,
    },

    /* 10 - SystemId */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x001CU,

        /* Characteristic Service Index */
        3U,
    },

    /* 11 - RegCertDataList */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x001EU,

        /* Characteristic Service Index */
        3U,
    },

    /* 12 - PnPID */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0020U,

        /* Characteristic Service Index */
        3U,
    },

    /* 13 - HeartRateMeasurement */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0023U,

        /* Characteristic Service Index */
        4U,
    },

    /* 14 - BodySensorLocation */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0026U,

        /* Characteristic Service Index */
        4U,
    },

    /* 15 - HeartRateControlPoint */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0028U,

        /* Characteristic Service Index */
        4U,
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

    /* 3 - DeviceInformation */
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
        0x0021U,

        /* Characteristic Start Index */
        4U,

        /* Characteristic End Index */
        12U
    },

    /* 4 - Heart Rate */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_LE_LINK_TYPE,

        /* Service Start Handle */
        0x0022U,

        /* Service End Handle */
        0x0029U,

        /* Characteristic Start Index */
        13U,

        /* Characteristic End Index */
        15U
    },
};


#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_hrs_gatt_db_register(void)
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

#endif /* ((defined BT_HRS) && (defined GATT_DB)) */
#endif /* GATT_DB_DYNAMIC */
#endif /* ATT */
