
/**
 *  \file gatt_db_stat_hts.c
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
 *  0x000F  |       0x2800       | Rd               | 0x180A
 *  --------+--------------------+------------------+---------------------
 *  0x0010  |       0x2803       | Rd               | 0x02, 0x0011, 0x2A29
 *  --------+--------------------+------------------+---------------------
 *  0x0011  |       0x2A29       | Rd               | Mindtree Limited.
 *  --------+--------------------+------------------+---------------------
 *  0x0012  |       0x2803       | Rd               | 0x02, 0x0013, 0x2A24
 *  --------+--------------------+------------------+---------------------
 *  0x0013  |       0x2A24       | Rd               | HTS-1.0.0
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
 *  0x0022  |       0x2800       | Rd               | 0x1809
 *  --------+--------------------+------------------+---------------------
 *  0x0023  |       0x2803       | Rd               | 0x20, 0x0024, 0x2A1C
 *  --------+--------------------+------------------+---------------------
 *  0x0024  |       0x2A1C       | Ind              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0025  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0026  |       0x2803       | Rd               | 0x10, 0x0027, 0x2A1E
 *  --------+--------------------+------------------+---------------------
 *  0x0027  |       0x2A1E       | Ntf              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0028  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0029  |       0x2803       | Rd               | 0x02, 0x002A, 0x2A1D
 *  --------+--------------------+------------------+---------------------
 *  0x002A  |       0x2A1D       | Rd               | 1.0
 *  --------+--------------------+------------------+---------------------
 *  0x002B  |       0x2803       | Rd               | 0x2A, 0x002C, 0x2A21
 *  --------+--------------------+------------------+---------------------
 *  0x002C  |       0x2A21       | Rd, Wr, Ind      | 5.0
 *  --------+--------------------+------------------+---------------------
 *  0x002D  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x002E  |       0x2906       | Rd               | VALUE
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
#if ((defined HTS) && (defined GATT_DB))

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

    /* 54 - Health Thermometer Service UUID */
    0x09U, 0x18U,

    /* 56 - TemperatureMeasurement Characteristic UUID */
    0x1CU, 0x2AU,

    /* 58 - IntermediateTemperature Characteristic UUID */
    0x1EU, 0x2AU,

    /* 60 - TemperatureType Characteristic UUID */
    0x1DU, 0x2AU,

    /* 62 - Measurement Interval Characteristic UUID */
    0x21U, 0x2AU,

    /* 64 - Measurement Interval Characteristic HLD UUID */
    0x06U, 0x29U,

};


UCHAR gatt_value_arr[GATT_VALUE_ARRAY_SIZE] =
{
    /* 0 - BatteryLevel(3); Default: 100.0 */
    0x64U,

    /* 1 - Measurement Interval(16); Default: 5.0 */
    0x05U, 0x00U,

    /* 3 - HLD for Measurement Interval */
    /* Valid Range Descriptor   */
    0x01U, 0x00U, 0x0AU, 0x00U,
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

    /* 54 - ManufacturerName(4); Property: 0x02, Value Handle: 0x0011, UUID: 0x2A29 */
    0x02U, 0x11U, 0x00U, 0x29U, 0x2AU,

    /* 59 - ManufacturerName(4); Default: Mindtree Limited. */
    0x4DU, 0x69U, 0x6EU, 0x64U, 0x74U, 0x72U, 0x65U, 0x65U, 0x20U, 0x4CU, 0x69U, 0x6DU,
    0x69U, 0x74U, 0x65U, 0x64U, 0x2EU,

    /* 76 - ModelNumber(5); Property: 0x02, Value Handle: 0x0013, UUID: 0x2A24 */
    0x02U, 0x13U, 0x00U, 0x24U, 0x2AU,

    /* 81 - ModelNumber(5); Default: HTS-1.0.0 */
    0x48U, 0x54U, 0x53U, 0x2DU, 0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 90 - SerialNumber(6); Property: 0x02, Value Handle: 0x0015, UUID: 0x2A25 */
    0x02U, 0x15U, 0x00U, 0x25U, 0x2AU,

    /* 95 - SerialNumber(6); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 100 - FirmwareRevision(7); Property: 0x02, Value Handle: 0x0017, UUID: 0x2A26 */
    0x02U, 0x17U, 0x00U, 0x26U, 0x2AU,

    /* 105 - FirmwareRevision(7); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 110 - HardwareRevision(8); Property: 0x02, Value Handle: 0x0019, UUID: 0x2A27 */
    0x02U, 0x19U, 0x00U, 0x27U, 0x2AU,

    /* 115 - HardwareRevision(8); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 120 - SoftwareRevision(9); Property: 0x02, Value Handle: 0x001B, UUID: 0x2A28 */
    0x02U, 0x1BU, 0x00U, 0x28U, 0x2AU,

    /* 125 - SoftwareRevision(9); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 130 - SystemId(10); Property: 0x02, Value Handle: 0x001D, UUID: 0x2A23 */
    0x02U, 0x1DU, 0x00U, 0x23U, 0x2AU,

    /* 135 - SystemId(10); Default: 11223344.0 */
    0x31U, 0x31U, 0x32U, 0x32U, 0x33U, 0x33U, 0x34U, 0x34U,

    /* 143 - RegCertDataList(11); Property: 0x02, Value Handle: 0x001F, UUID: 0x2A2A */
    0x02U, 0x1FU, 0x00U, 0x2AU, 0x2AU,

    /* 148 - RegCertDataList(11); Default: 0.0 */
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    /* 180 - PnPID(12); Property: 0x02, Value Handle: 0x0021, UUID: 0x2A50 */
    0x02U, 0x21U, 0x00U, 0x50U, 0x2AU,

    /* 185 - PnPID(12); Default: 0 */
    0x01U, 0x25U, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U,

    /* 192 - TemperatureMeasurement(13); Property: 0x20, Value Handle: 0x0024, UUID: 0x2A1C */
    0x20U, 0x24U, 0x00U, 0x1CU, 0x2AU,

    /* 197 - IntermediateTemperature(14); Property: 0x10, Value Handle: 0x0027, UUID: 0x2A1E */
    0x10U, 0x27U, 0x00U, 0x1EU, 0x2AU,

    /* 202 - TemperatureType(15); Property: 0x02, Value Handle: 0x002A, UUID: 0x2A1D */
    0x02U, 0x2AU, 0x00U, 0x1DU, 0x2AU,

    /* 207 - TemperatureType(15); Default: 1.0 */
    0x01U,

    /* 208 - Measurement Interval(16); Property: 0x2A, Value Handle: 0x002C, UUID: 0x2A21 */
    0x2AU, 0x2CU, 0x00U, 0x21U, 0x2AU,
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

    /* 4 - Client Configuration for TemperatureMeasurement */
    /* Client Instance 0 */
    0x00U, 0x00U,

    /* 6 - Client Configuration for IntermediateTemperature */
    /* Client Instance 0 */
    0x00U, 0x00U,

    /* 8 - Client Configuration for Measurement Interval */
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
        43U,

    },
    {
        /* UUID Offset */
        12U,

        /* First Occurence for Type */
        9U,

        /* Last Occurence for Type */
        45U,

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
        42U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        62U,

        /* First Occurence for Type */
        44U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        64U,

        /* First Occurence for Type */
        46U,

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
        (UCHAR *)(&gatt_const_value_arr[47U]),
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
        (UCHAR *)(&gatt_const_value_arr[54U]),
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
        (UCHAR *)(&gatt_const_value_arr[76U]),
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
        (UCHAR *)(&gatt_const_value_arr[81U]),
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
        (UCHAR *)(&gatt_const_value_arr[90U]),
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
        (UCHAR *)(&gatt_const_value_arr[95U]),
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
        (UCHAR *)(&gatt_const_value_arr[100U]),
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
        (UCHAR *)(&gatt_const_value_arr[105U]),
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
        (UCHAR *)(&gatt_const_value_arr[110U]),
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
        (UCHAR *)(&gatt_const_value_arr[115U]),
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
        (UCHAR *)(&gatt_const_value_arr[120U]),
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
        (UCHAR *)(&gatt_const_value_arr[125U]),
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
        (UCHAR *)(&gatt_const_value_arr[130U]),
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
        (UCHAR *)(&gatt_const_value_arr[135U]),
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
        (UCHAR *)(&gatt_const_value_arr[143U]),
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
        (UCHAR *)(&gatt_const_value_arr[148U]),
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
        (UCHAR *)(&gatt_const_value_arr[180U]),
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
        (UCHAR *)(&gatt_const_value_arr[185U]),
    },

    /* Handle - 0x0022 */
    /* 4 - Health Thermometer - Service Declaration */
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
    /* 13 - TemperatureMeasurement - Characteristic Declaration */
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
        (UCHAR *)(&gatt_const_value_arr[192U]),
    },

    /* Handle - 0x0024 */
    /* 13 - TemperatureMeasurement - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_INDICATE_PROPERTY,

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
    /* 13 - TemperatureMeasurement - CCD */
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
        40U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[4U],
    },

    /* Handle - 0x0026 */
    /* 14 - IntermediateTemperature - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        41U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[197U]),
    },

    /* Handle - 0x0027 */
    /* 14 - IntermediateTemperature - Characteristic Value */
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
        58U,

        /* Value */
        NULL,
    },

    /* Handle - 0x0028 */
    /* 14 - IntermediateTemperature - CCD */
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
        45U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[6U],
    },

    /* Handle - 0x0029 */
    /* 15 - TemperatureType - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        43U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[202U]),
    },

    /* Handle - 0x002A */
    /* 15 - TemperatureType - Characteristic Value */
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
        60U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[207U]),
    },

    /* Handle - 0x002B */
    /* 16 - Measurement Interval - Characteristic Declaration */
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
        (UCHAR *)(&gatt_const_value_arr[208U]),
    },

    /* Handle - 0x002C */
    /* 16 - Measurement Interval - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_INDICATE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        62U,

        /* Value */
        &gatt_value_arr[1U],
    },

    /* Handle - 0x002D */
    /* 16 - Measurement Interval - CCD */
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
        &gatt_db_peer_specific_val_arr[8U],
    },

    /* Handle - 0x002E */
    /* Characteristic 16 - Measurement Interval - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        4U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        64U,

        /* Value */
        &gatt_value_arr[3U],
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

    /* 13 - TemperatureMeasurement */
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

    /* 14 - IntermediateTemperature */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0026U,

        /* Characteristic Service Index */
        4U,
    },

    /* 15 - TemperatureType */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0029U,

        /* Characteristic Service Index */
        4U,
    },

    /* 16 - Measurement Interval */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4U,
        },

        /* Characteristic Start Handle */
        0x002BU,

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

    /* 4 - Health Thermometer */
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
        0x002EU,

        /* Characteristic Start Index */
        13U,

        /* Characteristic End Index */
        16U
    },
};

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_hts_gatt_db_register(void)
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

#endif /* ((defined HTS) && (defined GATT_DB)) */
#endif /* GATT_DB_DYNAMIC */
#endif /* ATT */
