
/**
 *  \file gatt_db_stat_cscs.c
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
 *  0x0003  |       0x2A00       | Rd               | Mindtree CSC
 *  --------+--------------------+------------------+---------------------
 *  0x0004  |       0x2803       | Rd               | 0x02, 0x0005, 0x2A01
 *  --------+--------------------+------------------+---------------------
 *  0x0005  |       0x2A01       | Rd               | 0
 *  ========+====================+==================+=====================
 *  0x0006  |       0x2800       | Rd               | 0x1801
 *  --------+--------------------+------------------+---------------------
 *  0x0007  |       0x2803       | Rd               | 0x20, 0x0008, 0x2A05
 *  --------+--------------------+------------------+---------------------
 *  0x0008  |       0x2A05       | Ind              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0009  |       0x2902       | Rd, Wr           | 0x0000
 *  ========+====================+==================+=====================
 *  0x000A  |       0x2800       | Rd               | 0x180A
 *  --------+--------------------+------------------+---------------------
 *  0x000B  |       0x2803       | Rd               | 0x02, 0x000C, 0x2A29
 *  --------+--------------------+------------------+---------------------
 *  0x000C  |       0x2A29       | Rd               | Mindtree Limited.
 *  --------+--------------------+------------------+---------------------
 *  0x000D  |       0x2803       | Rd               | 0x02, 0x000E, 0x2A24
 *  --------+--------------------+------------------+---------------------
 *  0x000E  |       0x2A24       | Rd               | CSC-1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x000F  |       0x2803       | Rd               | 0x02, 0x0010, 0x2A25
 *  --------+--------------------+------------------+---------------------
 *  0x0010  |       0x2A25       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0011  |       0x2803       | Rd               | 0x02, 0x0012, 0x2A26
 *  --------+--------------------+------------------+---------------------
 *  0x0012  |       0x2A26       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0013  |       0x2803       | Rd               | 0x02, 0x0014, 0x2A27
 *  --------+--------------------+------------------+---------------------
 *  0x0014  |       0x2A27       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0015  |       0x2803       | Rd               | 0x02, 0x0016, 0x2A28
 *  --------+--------------------+------------------+---------------------
 *  0x0016  |       0x2A28       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0017  |       0x2803       | Rd               | 0x02, 0x0018, 0x2A23
 *  --------+--------------------+------------------+---------------------
 *  0x0018  |       0x2A23       | Rd               | 11223344.0
 *  --------+--------------------+------------------+---------------------
 *  0x0019  |       0x2803       | Rd               | 0x02, 0x001A, 0x2A2A
 *  --------+--------------------+------------------+---------------------
 *  0x001A  |       0x2A2A       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x001B  |       0x2803       | Rd               | 0x02, 0x001C, 0x2A50
 *  --------+--------------------+------------------+---------------------
 *  0x001C  |       0x2A50       | Rd               | 0
 *  ========+====================+==================+=====================
 *  0x001D  |       0x2800       | Rd               | 0x180F
 *  --------+--------------------+------------------+---------------------
 *  0x001E  |       0x2803       | Rd               | 0x12, 0x001F, 0x2A19
 *  --------+--------------------+------------------+---------------------
 *  0x001F  |       0x2A19       | Rd, Ntf          | 100.0
 *  --------+--------------------+------------------+---------------------
 *  0x0020  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0021  |       0x2904       | Rd               | VALUE
 *  ========+====================+==================+=====================
 *  0x0022  |       0x2800       | Rd               | 0x1816
 *  --------+--------------------+------------------+---------------------
 *  0x0023  |       0x2803       | Rd               | 0x10, 0x0024, 0x2A5B
 *  --------+--------------------+------------------+---------------------
 *  0x0024  |       0x2A5B       | Ntf              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0025  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0026  |       0x2803       | Rd               | 0x02, 0x0027, 0x2A5C
 *  --------+--------------------+------------------+---------------------
 *  0x0027  |       0x2A5C       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0028  |       0x2803       | Rd               | 0x02, 0x0029, 0x2A5D
 *  --------+--------------------+------------------+---------------------
 *  0x0029  |       0x2A5D       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x002A  |       0x2803       | Rd               | 0x28, 0x002B, 0x2A55
 *  --------+--------------------+------------------+---------------------
 *  0x002B  |       0x2A55       | Wr, Ind          | 0
 *  --------+--------------------+------------------+---------------------
 *  0x002C  |       0x2902       | Rd, Wr           | 0x0000
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
#if ((defined CSCS) && (defined GATT_DB))

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

    /* 30 - DeviceInformation Service UUID */
    0x0AU, 0x18U,

    /* 32 - ManufacturerName Characteristic UUID */
    0x29U, 0x2AU,

    /* 34 - ModelNumber Characteristic UUID */
    0x24U, 0x2AU,

    /* 36 - SerialNumber Characteristic UUID */
    0x25U, 0x2AU,

    /* 38 - FirmwareRevision Characteristic UUID */
    0x26U, 0x2AU,

    /* 40 - HardwareRevision Characteristic UUID */
    0x27U, 0x2AU,

    /* 42 - SoftwareRevision Characteristic UUID */
    0x28U, 0x2AU,

    /* 44 - SystemId Characteristic UUID */
    0x23U, 0x2AU,

    /* 46 - RegCertDataList Characteristic UUID */
    0x2AU, 0x2AU,

    /* 48 - PnPID Characteristic UUID */
    0x50U, 0x2AU,

    /* 50 - Battery Service UUID */
    0x0FU, 0x18U,

    /* 52 - BatteryLevel Characteristic UUID */
    0x19U, 0x2AU,

    /* 54 - CSC Service UUID */
    0x16U, 0x18U,

    /* 56 - CSC Measurement Characteristic UUID */
    0x5BU, 0x2AU,

    /* 58 - CSC Feature Characteristic UUID */
    0x5CU, 0x2AU,

    /* 60 - Sensor Location Characteristic UUID */
    0x5DU, 0x2AU,

    /* 62 - SC Control Point Characteristic UUID */
    0x55U, 0x2AU,

};


UCHAR gatt_value_arr[GATT_VALUE_ARRAY_SIZE] =
{
    /* 0 - BatteryLevel(12); Default: 100.0 */
    0x64U,

    /* 1 - CSC Feature(14); Default: 0 */
    0x07U, 0x00U,

    /* 3 - Sensor Location(15); Default: 0 */
    0x04U,
};


DECL_CONST UCHAR gatt_const_value_arr[GATT_CONST_VALUE_ARRAY_SIZE] =
{

    /* 0 - DeviceName(0); Property: 0x02, Value Handle: 0x0003, UUID: 0x2A00 */
    0x02U, 0x03U, 0x00U, 0x00U, 0x2AU,

    /* 5 - DeviceName(0); Default: Mindtree CSC */
    0x4DU, 0x69U, 0x6EU, 0x64U, 0x74U, 0x72U, 0x65U, 0x65U, 0x20U, 0x43U, 0x53U, 0x43U,

    /* 17 - Appearance(1); Property: 0x02, Value Handle: 0x0005, UUID: 0x2A01 */
    0x02U, 0x05U, 0x00U, 0x01U, 0x2AU,

    /* 22 - Appearance(1); Default: 0 */
    0x00U, 0x00U,

    /* 24 - Service Changed(2); Property: 0x20, Value Handle: 0x0008, UUID: 0x2A05 */
    0x20U, 0x08U, 0x00U, 0x05U, 0x2AU,

    /* 29 - ManufacturerName(3); Property: 0x02, Value Handle: 0x000C, UUID: 0x2A29 */
    0x02U, 0x0CU, 0x00U, 0x29U, 0x2AU,

    /* 34 - ManufacturerName(3); Default: Mindtree Limited. */
    0x4DU, 0x69U, 0x6EU, 0x64U, 0x74U, 0x72U, 0x65U, 0x65U, 0x20U, 0x4CU, 0x69U, 0x6DU,
    0x69U, 0x74U, 0x65U, 0x64U, 0x2EU,

    /* 51 - ModelNumber(4); Property: 0x02, Value Handle: 0x000E, UUID: 0x2A24 */
    0x02U, 0x0EU, 0x00U, 0x24U, 0x2AU,

    /* 56 - ModelNumber(4); Default: CSC-1.0.0 */
    0x43U, 0x53U, 0x43U, 0x2DU, 0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 65 - SerialNumber(5); Property: 0x02, Value Handle: 0x0010, UUID: 0x2A25 */
    0x02U, 0x10U, 0x00U, 0x25U, 0x2AU,

    /* 70 - SerialNumber(5); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 75 - FirmwareRevision(6); Property: 0x02, Value Handle: 0x0012, UUID: 0x2A26 */
    0x02U, 0x12U, 0x00U, 0x26U, 0x2AU,

    /* 80 - FirmwareRevision(6); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 85 - HardwareRevision(7); Property: 0x02, Value Handle: 0x0014, UUID: 0x2A27 */
    0x02U, 0x14U, 0x00U, 0x27U, 0x2AU,

    /* 90 - HardwareRevision(7); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 95 - SoftwareRevision(8); Property: 0x02, Value Handle: 0x0016, UUID: 0x2A28 */
    0x02U, 0x16U, 0x00U, 0x28U, 0x2AU,

    /* 100 - SoftwareRevision(8); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 105 - SystemId(9); Property: 0x02, Value Handle: 0x0018, UUID: 0x2A23 */
    0x02U, 0x18U, 0x00U, 0x23U, 0x2AU,

    /* 110 - SystemId(9); Default: 11223344.0 */
    0x31U, 0x31U, 0x32U, 0x32U, 0x33U, 0x33U, 0x34U, 0x34U,

    /* 118 - RegCertDataList(10); Property: 0x02, Value Handle: 0x001A, UUID: 0x2A2A */
    0x02U, 0x1AU, 0x00U, 0x2AU, 0x2AU,

    /* 123 - RegCertDataList(10); Default: 0 */
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    /* 155 - PnPID(11); Property: 0x02, Value Handle: 0x001C, UUID: 0x2A50 */
    0x02U, 0x1CU, 0x00U, 0x50U, 0x2AU,

    /* 160 - PnPID(11); Default: 0 */
    0x01U, 0x6AU, 0x00U, 0x4DU, 0x01U, 0x00U, 0x01U,

    /* 167 - BatteryLevel(12); Property: 0x12, Value Handle: 0x001F, UUID: 0x2A19 */
    0x12U, 0x1FU, 0x00U, 0x19U, 0x2AU,

    /* 172 - Presentation Fromat for BatteryLevel */
    /**
     * Characteristic Presentation Format for BatteryLevel
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x04U, 0x00U, 0xADU, 0x27U, 0x01U, 0x00U, 0x00U,

    /* 179 - CSC Measurement(13); Property: 0x10, Value Handle: 0x0024, UUID: 0x2A5B */
    0x10U, 0x24U, 0x00U, 0x5BU, 0x2AU,

    /* 184 - CSC Feature(14); Property: 0x02, Value Handle: 0x0027, UUID: 0x2A5C */
    0x02U, 0x27U, 0x00U, 0x5CU, 0x2AU,

    /* 189 - Sensor Location(15); Property: 0x02, Value Handle: 0x0029, UUID: 0x2A5D */
    0x02U, 0x29U, 0x00U, 0x5DU, 0x2AU,

    /* 194 - SC Control Point(16); Property: 0x28, Value Handle: 0x002B, UUID: 0x2A55 */
    0x28U, 0x2BU, 0x00U, 0x55U, 0x2AU,
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

    /* 4 - Client Configuration for CSC Measurement */
    /* Client Instance 0 */
    0x00U, 0x00U,

    /* 6 - Client Configuration for SC Control Point */
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
        42U,

    },
    {
        /* UUID Offset */
        12U,

        /* First Occurence for Type */
        9U,

        /* Last Occurence for Type */
        44U,

    },
    {
        /* UUID Offset */
        16U,

        /* First Occurence for Type */
        33U,

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
        14U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        36U,

        /* First Occurence for Type */
        16U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        38U,

        /* First Occurence for Type */
        18U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        40U,

        /* First Occurence for Type */
        20U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        42U,

        /* First Occurence for Type */
        22U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        44U,

        /* First Occurence for Type */
        24U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        46U,

        /* First Occurence for Type */
        26U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        48U,

        /* First Occurence for Type */
        28U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        50U,

        /* First Occurence for Type */
        29U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        52U,

        /* First Occurence for Type */
        31U,

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
    {
        /* UUID Offset */
        62U,

        /* First Occurence for Type */
        43U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
};


DECL_CONST GATT_DB_DESC_DATA gatt_db_attr_table[GATT_DB_MAX_ATTRIBUTES] =
{
    /* Dummy */
    {
        /* Property */
        GATT_DB_READ,

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
        GATT_DB_READ,

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
        GATT_DB_READ,

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
        12U,

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
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        7U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[17U]),
    },

    /* Handle - 0x0005 */
    /* 1 - Appearance - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        24U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[22U]),
    },

    /* Handle - 0x0006 */
    /* 1 - GATT - Service Declaration */
    {
        /* Property */
        GATT_DB_READ,

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
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        11U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[24U]),
    },

    /* Handle - 0x0008 */
    /* 2 - Service Changed - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_INDICATE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

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
        GATT_DB_READ |
        GATT_DB_WRITE,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        32U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[0U],
    },

    /* Handle - 0x000A */
    /* 2 - DeviceInformation - Service Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        29U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[30U]),
    },

    /* Handle - 0x000B */
    /* 3 - ManufacturerName - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        13U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[29U]),
    },

    /* Handle - 0x000C */
    /* 3 - ManufacturerName - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        17U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        32U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[34U]),
    },

    /* Handle - 0x000D */
    /* 4 - ModelNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        15U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[51U]),
    },

    /* Handle - 0x000E */
    /* 4 - ModelNumber - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        9U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        34U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[56U]),
    },

    /* Handle - 0x000F */
    /* 5 - SerialNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        17U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[65U]),
    },

    /* Handle - 0x0010 */
    /* 5 - SerialNumber - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        36U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[70U]),
    },

    /* Handle - 0x0011 */
    /* 6 - FirmwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        19U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[75U]),
    },

    /* Handle - 0x0012 */
    /* 6 - FirmwareRevision - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        38U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[80U]),
    },

    /* Handle - 0x0013 */
    /* 7 - HardwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        21U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[85U]),
    },

    /* Handle - 0x0014 */
    /* 7 - HardwareRevision - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        40U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[90U]),
    },

    /* Handle - 0x0015 */
    /* 8 - SoftwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        23U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[95U]),
    },

    /* Handle - 0x0016 */
    /* 8 - SoftwareRevision - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        42U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[100U]),
    },

    /* Handle - 0x0017 */
    /* 9 - SystemId - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        25U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[105U]),
    },

    /* Handle - 0x0018 */
    /* 9 - SystemId - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        8U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        44U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[110U]),
    },

    /* Handle - 0x0019 */
    /* 10 - RegCertDataList - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        27U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[118U]),
    },

    /* Handle - 0x001A */
    /* 10 - RegCertDataList - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        32U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        46U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[123U]),
    },

    /* Handle - 0x001B */
    /* 11 - PnPID - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        30U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[155U]),
    },

    /* Handle - 0x001C */
    /* 11 - PnPID - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        48U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[160U]),
    },

    /* Handle - 0x001D */
    /* 3 - Battery - Service Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        34U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[50U]),
    },

    /* Handle - 0x001E */
    /* 12 - BatteryLevel - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        35U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[167U]),
    },

    /* Handle - 0x001F */
    /* 12 - BatteryLevel - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        52U,

        /* Value */
        &gatt_value_arr[0U],
    },

    /* Handle - 0x0020 */
    /* 12 - BatteryLevel - CCD */
    {
        /* Property */
        GATT_DB_READ |
        GATT_DB_WRITE,

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

    /* Handle - 0x0021 */
    /* Characteristic 12 - BatteryLevel - Presentation Format */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        16U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[172U]),
    },

    /* Handle - 0x0022 */
    /* 4 - CSC - Service Declaration */
    {
        /* Property */
        GATT_DB_READ,

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
    /* 13 - CSC Measurement - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        38U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[179U]),
    },

    /* Handle - 0x0024 */
    /* 13 - CSC Measurement - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        0U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        56U,

        /* Value */
        NULL,
    },

    /* Handle - 0x0025 */
    /* 13 - CSC Measurement - CCD */
    {
        /* Property */
        GATT_DB_READ |
        GATT_DB_WRITE,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        44U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[4U],
    },

    /* Handle - 0x0026 */
    /* 14 - CSC Feature - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        40U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[184U]),
    },

    /* Handle - 0x0027 */
    /* 14 - CSC Feature - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        58U,

        /* Value */
        &gatt_value_arr[1U],
    },

    /* Handle - 0x0028 */
    /* 15 - Sensor Location - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        42U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[189U]),
    },

    /* Handle - 0x0029 */
    /* 15 - Sensor Location - Characteristic Value */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        60U,

        /* Value */
        &gatt_value_arr[3U],
    },

    /* Handle - 0x002A */
    /* 16 - SC Control Point - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_READ,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[194U]),
    },

    /* Handle - 0x002B */
    /* 16 - SC Control Point - Characteristic Value */
    {
        /* Property */
        GATT_DB_WRITE |
        GATT_DB_CHAR_INDICATE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        0U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        62U,

        /* Value */
        NULL,
    },

    /* Handle - 0x002C */
    /* 16 - SC Control Point - CCD */
    {
        /* Property */
        GATT_DB_READ |
        GATT_DB_WRITE,

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
        &gatt_db_peer_specific_val_arr[6U],
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

    /* 3 - ManufacturerName */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x000BU,

        /* Characteristic Service Index */
        2U,
    },

    /* 4 - ModelNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x000DU,

        /* Characteristic Service Index */
        2U,
    },

    /* 5 - SerialNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x000FU,

        /* Characteristic Service Index */
        2U,
    },

    /* 6 - FirmwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0011U,

        /* Characteristic Service Index */
        2U,
    },

    /* 7 - HardwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0013U,

        /* Characteristic Service Index */
        2U,
    },

    /* 8 - SoftwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0015U,

        /* Characteristic Service Index */
        2U,
    },

    /* 9 - SystemId */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0017U,

        /* Characteristic Service Index */
        2U,
    },

    /* 10 - RegCertDataList */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0019U,

        /* Characteristic Service Index */
        2U,
    },

    /* 11 - PnPID */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x001BU,

        /* Characteristic Service Index */
        2U,
    },

    /* 12 - BatteryLevel */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4U,
        },

        /* Characteristic Start Handle */
        0x001EU,

        /* Characteristic Service Index */
        3U,
    },

    /* 13 - CSC Measurement */
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

    /* 14 - CSC Feature */
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

    /* 15 - Sensor Location */
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

    /* 16 - SC Control Point */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x002AU,

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

    /* 2 - DeviceInformation */
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
        0x001CU,

        /* Characteristic Start Index */
        3U,

        /* Characteristic End Index */
        11U
    },

    /* 3 - Battery */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x001DU,

        /* Service End Handle */
        0x0021U,

        /* Characteristic Start Index */
        12U,

        /* Characteristic End Index */
        12U
    },

    /* 4 - CSC */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0022U,

        /* Service End Handle */
        0x002CU,

        /* Characteristic Start Index */
        13U,

        /* Characteristic End Index */
        16U
    },
};
#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_cscs_gatt_db_register(void)
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

#endif /* ((defined CSCS) && (defined GATT_DB)) */
#endif /* GATT_DB_DYNAMIC */
#endif /* ATT */
