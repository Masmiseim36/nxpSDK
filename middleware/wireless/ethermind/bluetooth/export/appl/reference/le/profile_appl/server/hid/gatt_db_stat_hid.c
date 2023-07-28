
/**
 *  \file gatt_db_stat_hid.c
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
 *  0x0001  |       0x2800       | Rd               | 0x1801
 *  --------+--------------------+------------------+---------------------
 *  0x0002  |       0x2803       | Rd               | 0x20, 0x0003, 0x2A05
 *  --------+--------------------+------------------+---------------------
 *  0x0003  |       0x2A05       | Ind              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x0004  |       0x2902       | Rd, Wr           | 0x0000
 *  ========+====================+==================+=====================
 *  0x0005  |       0x2800       | Rd               | 0x1800
 *  --------+--------------------+------------------+---------------------
 *  0x0006  |       0x2803       | Rd               | 0x02, 0x0007, 0x2A00
 *  --------+--------------------+------------------+---------------------
 *  0x0007  |       0x2A00       | Rd               | EtherMind
 *  --------+--------------------+------------------+---------------------
 *  0x0008  |       0x2803       | Rd               | 0x02, 0x0009, 0x2A01
 *  --------+--------------------+------------------+---------------------
 *  0x0009  |       0x2A01       | Rd               | 0.0
 *  --------+--------------------+------------------+---------------------
 *  0x000A  |       0x2803       | Rd               | 0x02, 0x000B, 0x2A04
 *  --------+--------------------+------------------+---------------------
 *  0x000B  |       0x2A04       | Rd               | 0.0
 *  ========+====================+==================+=====================
 *  0x000C  |       0x2800       | Rd               | 0x180A
 *  --------+--------------------+------------------+---------------------
 *  0x000D  |       0x2803       | Rd               | 0x02, 0x000E, 0x2A29
 *  --------+--------------------+------------------+---------------------
 *  0x000E  |       0x2A29       | Rd               | MindTreeLimited
 *  --------+--------------------+------------------+---------------------
 *  0x000F  |       0x2803       | Rd               | 0x02, 0x0010, 0x2A24
 *  --------+--------------------+------------------+---------------------
 *  0x0010  |       0x2A24       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0011  |       0x2803       | Rd               | 0x02, 0x0012, 0x2A25
 *  --------+--------------------+------------------+---------------------
 *  0x0012  |       0x2A25       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0013  |       0x2803       | Rd               | 0x02, 0x0014, 0x2A27
 *  --------+--------------------+------------------+---------------------
 *  0x0014  |       0x2A27       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0015  |       0x2803       | Rd               | 0x02, 0x0016, 0x2A26
 *  --------+--------------------+------------------+---------------------
 *  0x0016  |       0x2A26       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0017  |       0x2803       | Rd               | 0x02, 0x0018, 0x2A28
 *  --------+--------------------+------------------+---------------------
 *  0x0018  |       0x2A28       | Rd               | 1.0.0
 *  --------+--------------------+------------------+---------------------
 *  0x0019  |       0x2803       | Rd               | 0x02, 0x001A, 0x2A23
 *  --------+--------------------+------------------+---------------------
 *  0x001A  |       0x2A23       | Rd               | AABBCCDD
 *  --------+--------------------+------------------+---------------------
 *  0x001B  |       0x2803       | Rd               | 0x02, 0x001C, 0x2A2A
 *  --------+--------------------+------------------+---------------------
 *  0x001C  |       0x2A2A       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x001D  |       0x2803       | Rd               | 0x02, 0x001E, 0x2A50
 *  --------+--------------------+------------------+---------------------
 *  0x001E  |       0x2A50       | Rd               | 0
 *  ========+====================+==================+=====================
 *  0x001F  |       0x2800       | Rd               | 0x180F
 *  --------+--------------------+------------------+---------------------
 *  0x0020  |       0x2803       | Rd               | 0x12, 0x0021, 0x2A19
 *  --------+--------------------+------------------+---------------------
 *  0x0021  |       0x2A19       | Rd, Ntf          | 100.0
 *  --------+--------------------+------------------+---------------------
 *  0x0022  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0023  |       0x2904       | Rd               | VALUE
 *  ========+====================+==================+=====================
 *  0x0024  |       0x2800       | Rd               | 0x1812
 *  --------+--------------------+------------------+---------------------
 *  0x0025  |       0x2803       | Rd               | 0x06, 0x0026, 0x2A4E
 *  --------+--------------------+------------------+---------------------
 *  0x0026  |       0x2A4E       | Rd, Wwr          | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0027  |       0x2803       | Rd               | 0x02, 0x0028, 0x2A4B
 *  --------+--------------------+------------------+---------------------
 *  0x0028  |       0x2A4B       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0029  |       0x2907       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x002A  |       0x2803       | Rd               | 0x1A, 0x002B, 0x2A4D
 *  --------+--------------------+------------------+---------------------
 *  0x002B  |       0x2A4D       | Rd, Wr, Ntf      | 0
 *  --------+--------------------+------------------+---------------------
 *  0x002C  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x002D  |       0x2908       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x002E  |       0x2803       | Rd               | 0x0E, 0x002F, 0x2A4D
 *  --------+--------------------+------------------+---------------------
 *  0x002F  |       0x2A4D       | Rd, Wr, Wwr      | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0030  |       0x2908       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0031  |       0x2803       | Rd               | 0x0A, 0x0032, 0x2A4D
 *  --------+--------------------+------------------+---------------------
 *  0x0032  |       0x2A4D       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0033  |       0x2908       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0034  |       0x2803       | Rd               | 0x04, 0x0035, 0x2A4C
 *  --------+--------------------+------------------+---------------------
 *  0x0035  |       0x2A4C       | Wwr              | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0036  |       0x2803       | Rd               | 0x02, 0x0037, 0x2A4A
 *  --------+--------------------+------------------+---------------------
 *  0x0037  |       0x2A4A       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0038  |       0x2803       | Rd               | 0x1A, 0x0039, 0x2A33
 *  --------+--------------------+------------------+---------------------
 *  0x0039  |       0x2A33       | Rd, Wr, Ntf      | 0
 *  --------+--------------------+------------------+---------------------
 *  0x003A  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x003B  |       0x2803       | Rd               | 0x1A, 0x003C, 0x2A22
 *  --------+--------------------+------------------+---------------------
 *  0x003C  |       0x2A22       | Rd, Wr, Ntf      | 0
 *  --------+--------------------+------------------+---------------------
 *  0x003D  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x003E  |       0x2803       | Rd               | 0x0E, 0x003F, 0x2A32
 *  --------+--------------------+------------------+---------------------
 *  0x003F  |       0x2A32       | Rd, Wr, Wwr      | 0
 *  ========+====================+==================+=====================
 *  0x0040  |       0x2800       | Rd               | 0x1813
 *  --------+--------------------+------------------+---------------------
 *  0x0041  |       0x2803       | Rd               | 0x04, 0x0042, 0x2A4F
 *  --------+--------------------+------------------+---------------------
 *  0x0042  |       0x2A4F       | Wwr              | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0043  |       0x2803       | Rd               | 0x12, 0x0044, 0x2A31
 *  --------+--------------------+------------------+---------------------
 *  0x0044  |       0x2A31       | Rd, Ntf          | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0045  |       0x2902       | Rd, Wr           | 0x0000
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
#if ((defined HID) && (defined GATT_DB))

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

    /* 20 - GATT Service UUID */
    0x01U, 0x18U,

    /* 22 - Service Changed Characteristic UUID */
    0x05U, 0x2AU,

    /* 24 - GAP Service UUID */
    0x00U, 0x18U,

    /* 26 - Device Name Characteristic UUID */
    0x00U, 0x2AU,

    /* 28 - Appearance Characteristic UUID */
    0x01U, 0x2AU,

    /* 30 - Peripheral Preferred Connection Paramters Characteristic UUID */
    0x04U, 0x2AU,

    /* 32 - DeviceInformation Service UUID */
    0x0AU, 0x18U,

    /* 34 - ManufacturerName Characteristic UUID */
    0x29U, 0x2AU,

    /* 36 - ModelNumber Characteristic UUID */
    0x24U, 0x2AU,

    /* 38 - SerialNumber Characteristic UUID */
    0x25U, 0x2AU,

    /* 40 - HardwareRevision Characteristic UUID */
    0x27U, 0x2AU,

    /* 42 - FirmwareRevision Characteristic UUID */
    0x26U, 0x2AU,

    /* 44 - SoftwareRevision Characteristic UUID */
    0x28U, 0x2AU,

    /* 46 - SystemId Characteristic UUID */
    0x23U, 0x2AU,

    /* 48 - RegCertDataList Characteristic UUID */
    0x2AU, 0x2AU,

    /* 50 - PnPID Characteristic UUID */
    0x50U, 0x2AU,

    /* 52 - Battery Service UUID */
    0x0FU, 0x18U,

    /* 54 - BatteryLevel Characteristic UUID */
    0x19U, 0x2AU,

    /* 56 - HumanInterfaceDevice Service UUID */
    0x12U, 0x18U,

    /* 58 - HIDProtocolMode Characteristic UUID */
    0x4EU, 0x2AU,

    /* 60 - HIDReportMap Characteristic UUID */
    0x4BU, 0x2AU,

    /* 62 - HIDReportMap Characteristic HLD UUID */
    0x07U, 0x29U,

    /* 64 - Input HIDReport Characteristic UUID */
    0x4DU, 0x2AU,

    /* 66 - Input HIDReport Characteristic HLD UUID */
    0x08U, 0x29U,

    /* 68 - HIDControlPoint Characteristic UUID */
    0x4CU, 0x2AU,

    /* 70 - HIDInformation Characteristic UUID */
    0x4AU, 0x2AU,

    /* 72 - BootMouseInputReport Characteristic UUID */
    0x33U, 0x2AU,

    /* 74 - BootKeyboardInputReport Characteristic UUID */
    0x22U, 0x2AU,

    /* 76 - BootKeyboardOutputReport Characteristic UUID */
    0x32U, 0x2AU,

    /* 78 - ScanParameter Service UUID */
    0x13U, 0x18U,

    /* 80 - ScanIntervalWindow Characteristic UUID */
    0x4FU, 0x2AU,

    /* 82 - ScanRefresh Characteristic UUID */
    0x31U, 0x2AU,

};


UCHAR gatt_value_arr[GATT_VALUE_ARRAY_SIZE] =
{
    /* 0 - BatteryLevel(13); Default: 100.0 */
    0x64U,

    /* 1 - HIDProtocolMode(14); Default: 0 */
    0x01U,

    /* 2 - HIDReportMap(15); Default: 0 */
    0x3FU, 0x05U, 0x01U, 0x09U, 0x06U, 0xA1U, 0x01U, 0x05U, 0x07U, 0x19U, 0xE0U, 0x29U, 0xE7U,
    0x15U, 0x00U, 0x25U, 0x01U, 0x75U, 0x01U, 0x95U, 0x08U, 0x81U, 0x02U, 0x95U, 0x01U,
    0x75U, 0x08U, 0x81U, 0x03U, 0x95U, 0x05U, 0x75U, 0x01U, 0x05U, 0x08U, 0x19U, 0x01U,
    0x29U, 0x05U, 0x91U, 0x02U, 0x95U, 0x01U, 0x75U, 0x03U, 0x91U, 0x03U, 0x95U, 0x06U,
    0x75U, 0x08U, 0x15U, 0x00U, 0x25U, 0x65U, 0x05U, 0x07U, 0x19U, 0x00U, 0x29U, 0x65U,
    0x81U, 0x00U, 0xC0U,

    /* 66 - HLD for HIDReportMap */
    /* External Report Reference Descriptor  */
    0x00U, 0x00U,

    /* 68 - HLD for Input HIDReport */
    /* Input Report Reference Descriptor  */
    0x00U, 0x01U,

    /* 70 - HLD for Output HIDReport */
    /* Output Report Reference Descriptor  */
    0x00U, 0x02U,

    /* 72 - HLD for Feature HIDReport */
    /* Feature Report Reference Descriptor  */
    0x00U, 0x03U,

    /* 74 - HIDInformation(20); Default: 0 */
    0x04U, 0x13U, 0x02U, 0x40U, 0x01U,

    /* 79 - ScanRefresh(25); Default: 0 */
    0x01U, 0x00U,
};


DECL_CONST UCHAR gatt_const_value_arr[GATT_CONST_VALUE_ARRAY_SIZE] =
{

    /* 0 - Service Changed(0); Property: 0x20, Value Handle: 0x0003, UUID: 0x2A05 */
    0x20U, 0x03U, 0x00U, 0x05U, 0x2AU,

    /* 5 - Device Name(1); Property: 0x02, Value Handle: 0x0007, UUID: 0x2A00 */
    0x02U, 0x07U, 0x00U, 0x00U, 0x2AU,

    /* 10 - Device Name(1); Default: EtherMind */
    0x45U, 0x74U, 0x68U, 0x65U, 0x72U, 0x4dU, 0x69U, 0x6eU, 0x64U,

    /* 19 - Appearance(2); Property: 0x02, Value Handle: 0x0009, UUID: 0x2A01 */
    0x02U, 0x09U, 0x00U, 0x01U, 0x2AU,

    /* 24 - Appearance(2); Default: 0.0 */
    0xC2U, 0x03U,

    /* 26 - Peripheral Preferred Connection Paramters(3); Property: 0x02, Value Handle: 0x000B, UUID: 0x2A04 */
    0x02U, 0x0BU, 0x00U, 0x04U, 0x2AU,

    /* 31 - Peripheral Preferred Connection Paramters(3); Default: 0.0 */
    0x0AU, 0x00U, 0x10U, 0x00U, 0x64U, 0x00U, 0xE2U, 0x04U,

    /* 39 - ManufacturerName(4); Property: 0x02, Value Handle: 0x000E, UUID: 0x2A29 */
    0x02U, 0x0EU, 0x00U, 0x29U, 0x2AU,

    /* 44 - ManufacturerName(4); Default: MindTreeLimited */
    0x4DU, 0x69U, 0x6EU, 0x64U, 0x54U, 0x72U, 0x65U, 0x65U, 0x4CU, 0x69U, 0x6DU, 0x69U,
    0x74U, 0x65U, 0x64U,

    /* 59 - ModelNumber(5); Property: 0x02, Value Handle: 0x0010, UUID: 0x2A24 */
    0x02U, 0x10U, 0x00U, 0x24U, 0x2AU,

    /* 64 - ModelNumber(5); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 69 - SerialNumber(6); Property: 0x02, Value Handle: 0x0012, UUID: 0x2A25 */
    0x02U, 0x12U, 0x00U, 0x25U, 0x2AU,

    /* 74 - SerialNumber(6); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 79 - HardwareRevision(7); Property: 0x02, Value Handle: 0x0014, UUID: 0x2A27 */
    0x02U, 0x14U, 0x00U, 0x27U, 0x2AU,

    /* 84 - HardwareRevision(7); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 89 - FirmwareRevision(8); Property: 0x02, Value Handle: 0x0016, UUID: 0x2A26 */
    0x02U, 0x16U, 0x00U, 0x26U, 0x2AU,

    /* 94 - FirmwareRevision(8); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 99 - SoftwareRevision(9); Property: 0x02, Value Handle: 0x0018, UUID: 0x2A28 */
    0x02U, 0x18U, 0x00U, 0x28U, 0x2AU,

    /* 104 - SoftwareRevision(9); Default: 1.0.0 */
    0x31U, 0x2EU, 0x30U, 0x2EU, 0x30U,

    /* 109 - SystemId(10); Property: 0x02, Value Handle: 0x001A, UUID: 0x2A23 */
    0x02U, 0x1AU, 0x00U, 0x23U, 0x2AU,

    /* 114 - SystemId(10); Default: AABBCCDD */
    0x41U, 0x41U, 0x42U, 0x42U, 0x43U, 0x43U, 0x44U, 0x44U,

    /* 122 - RegCertDataList(11); Property: 0x02, Value Handle: 0x001C, UUID: 0x2A2A */
    0x02U, 0x1CU, 0x00U, 0x2AU, 0x2AU,

    /* 127 - RegCertDataList(11); Default: 0 */
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,
    0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    /* 159 - PnPID(12); Property: 0x02, Value Handle: 0x001E, UUID: 0x2A50 */
    0x02U, 0x1EU, 0x00U, 0x50U, 0x2AU,

    /* 164 - PnPID(12); Default: 0 */
    0x01U, 0x25U, 0x00U, 0x00U, 0x00U, 0x01U, 0x00U,

    /* 171 - BatteryLevel(13); Property: 0x12, Value Handle: 0x0021, UUID: 0x2A19 */
    0x12U, 0x21U, 0x00U, 0x19U, 0x2AU,

    /* 176 - Presentation Fromat for BatteryLevel */
    /**
     * Characteristic Presentation Format for BatteryLevel
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x04U, 0x00U, 0xADU, 0x27U, 0x01U, 0x00U, 0x00U,

    /* 183 - HIDProtocolMode(14); Property: 0x06, Value Handle: 0x0026, UUID: 0x2A4E */
    0x06U, 0x26U, 0x00U, 0x4EU, 0x2AU,

    /* 188 - HIDReportMap(15); Property: 0x02, Value Handle: 0x0028, UUID: 0x2A4B */
    0x02U, 0x28U, 0x00U, 0x4BU, 0x2AU,

    /* 193 - Input HIDReport(16); Property: 0x1A, Value Handle: 0x002B, UUID: 0x2A4D */
    0x1AU, 0x2BU, 0x00U, 0x4DU, 0x2AU,

    /* 198 - Output HIDReport(17); Property: 0x0E, Value Handle: 0x002F, UUID: 0x2A4D */
    0x0EU, 0x2FU, 0x00U, 0x4DU, 0x2AU,

    /* 203 - Feature HIDReport(18); Property: 0x0A, Value Handle: 0x0032, UUID: 0x2A4D */
    0x0AU, 0x32U, 0x00U, 0x4DU, 0x2AU,

    /* 208 - HIDControlPoint(19); Property: 0x04, Value Handle: 0x0035, UUID: 0x2A4C */
    0x04U, 0x35U, 0x00U, 0x4CU, 0x2AU,

    /* 213 - HIDInformation(20); Property: 0x02, Value Handle: 0x0037, UUID: 0x2A4A */
    0x02U, 0x37U, 0x00U, 0x4AU, 0x2AU,

    /* 218 - BootMouseInputReport(21); Property: 0x1A, Value Handle: 0x0039, UUID: 0x2A33 */
    0x1AU, 0x39U, 0x00U, 0x33U, 0x2AU,

    /* 223 - BootKeyboardInputReport(22); Property: 0x1A, Value Handle: 0x003C, UUID: 0x2A22 */
    0x1AU, 0x3CU, 0x00U, 0x22U, 0x2AU,

    /* 228 - BootKeyboardOutputReport(23); Property: 0x0E, Value Handle: 0x003F, UUID: 0x2A32 */
    0x0EU, 0x3FU, 0x00U, 0x32U, 0x2AU,

    /* 233 - ScanIntervalWindow(24); Property: 0x04, Value Handle: 0x0042, UUID: 0x2A4F */
    0x04U, 0x42U, 0x00U, 0x4FU, 0x2AU,

    /* 238 - ScanRefresh(25); Property: 0x12, Value Handle: 0x0044, UUID: 0x2A31 */
    0x12U, 0x44U, 0x00U, 0x31U, 0x2AU,
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

    /* 4 - Input HIDReport(16); Default: 0 */
    /* Client Instance 0 */
    0x09U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    /* 14 - Client Configuration for Input HIDReport */
    /* Client Instance 0 */
    0x00U, 0x00U,

    /* 16 - Output HIDReport(17); Default: 0 */
    /* Client Instance 0 */
    0x03U, 0x00U, 0x00U, 0x00U ,

    /* 20 - Feature HIDReport(18); Default: 0 */
    /* Client Instance 0 */
    0x08U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U  ,

    /* 29 - HIDControlPoint(19); Default: 0 */
    /* Client Instance 0 */
    0x00U,

    /* 30 - BootMouseInputReport(21); Default: 0 */
    /* Client Instance 0 */
    0x03U, 0x00U, 0x00U, 0x00U,

    /* 34 - Client Configuration for BootMouseInputReport */
    /* Client Instance 0 */
    0x00U, 0x00U,

    /* 36 - BootKeyboardInputReport(22); Default: 0 */
    /* Client Instance 0 */
    0x08U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U,

    /* 45 - Client Configuration for BootKeyboardInputReport */
    /* Client Instance 0 */
    0x00U, 0x00U,

    /* 47 - BootKeyboardOutputReport(23); Default: 0 */
    /* Client Instance 0 */
    0x03U, 0x00U, 0x00U, 0x00U,

    /* 51 - ScanIntervalWindow(24); Default: 0 */
    /* Client Instance 0 */
    0x40U, 0x00U, 0x10U, 0x00U,

    /* 55 - Client Configuration for ScanRefresh */
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
        64U,

    },
    {
        /* UUID Offset */
        6U,

        /* First Occurence for Type */
        2U,

        /* Last Occurence for Type */
        67U,

    },
    {
        /* UUID Offset */
        12U,

        /* First Occurence for Type */
        4U,

        /* Last Occurence for Type */
        69U,

    },
    {
        /* UUID Offset */
        16U,

        /* First Occurence for Type */
        35U,

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
        7U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        28U,

        /* First Occurence for Type */
        9U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        30U,

        /* First Occurence for Type */
        11U,

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
        30U,

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
        33U,

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
        38U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        60U,

        /* First Occurence for Type */
        40U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        62U,

        /* First Occurence for Type */
        41U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        64U,

        /* First Occurence for Type */
        43U,

        /* Last Occurence for Type */
        50U,

    },
    {
        /* UUID Offset */
        66U,

        /* First Occurence for Type */
        45U,

        /* Last Occurence for Type */
        51U,

    },
    {
        /* UUID Offset */
        68U,

        /* First Occurence for Type */
        53U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        70U,

        /* First Occurence for Type */
        55U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        72U,

        /* First Occurence for Type */
        57U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        74U,

        /* First Occurence for Type */
        60U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        76U,

        /* First Occurence for Type */
        63U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        78U,

        /* First Occurence for Type */
        64U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        80U,

        /* First Occurence for Type */
        66U,

        /* Last Occurence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        82U,

        /* First Occurence for Type */
        68U,

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
    /* 0 - GATT - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        5U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[20U]),
    },

    /* Handle - 0x0002 */
    /* 0 - Service Changed - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        6U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[0U]),
    },

    /* Handle - 0x0003 */
    /* 0 - Service Changed - Characteristic Value */
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
        22U,

        /* Value */
        NULL,
    },

    /* Handle - 0x0004 */
    /* 0 - Service Changed - CCD */
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
        34U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[0U],
    },

    /* Handle - 0x0005 */
    /* 1 - GAP - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        12U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[24U]),
    },

    /* Handle - 0x0006 */
    /* 1 - Device Name - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        8U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[5U]),
    },

    /* Handle - 0x0007 */
    /* 1 - Device Name - Characteristic Value */
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
        26U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[10U]),
    },

    /* Handle - 0x0008 */
    /* 2 - Appearance - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        10U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[19U]),
    },

    /* Handle - 0x0009 */
    /* 2 - Appearance - Characteristic Value */
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
        28U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[24U]),
    },

    /* Handle - 0x000A */
    /* 3 - Peripheral Preferred Connection Paramters - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        13U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[26U]),
    },

    /* Handle - 0x000B */
    /* 3 - Peripheral Preferred Connection Paramters - Characteristic Value */
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
        30U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[31U]),
    },

    /* Handle - 0x000C */
    /* 2 - DeviceInformation - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        31U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[32U]),
    },

    /* Handle - 0x000D */
    /* 4 - ManufacturerName - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        15U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[39U]),
    },

    /* Handle - 0x000E */
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
        34U,

        /* Value */
        (UCHAR *)(&appl_manufacturer_name_ext),
    },

    /* Handle - 0x000F */
    /* 5 - ModelNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        17U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[59U]),
    },

    /* Handle - 0x0010 */
    /* 5 - ModelNumber - Characteristic Value */
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
        36U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[64U]),
    },

    /* Handle - 0x0011 */
    /* 6 - SerialNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        19U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[69U]),
    },

    /* Handle - 0x0012 */
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
        38U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[74U]),
    },

    /* Handle - 0x0013 */
    /* 7 - HardwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        21U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[79U]),
    },

    /* Handle - 0x0014 */
    /* 7 - HardwareRevision - Characteristic Value */
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
        (UCHAR *)(&gatt_const_value_arr[84U]),
    },

    /* Handle - 0x0015 */
    /* 8 - FirmwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        23U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[89U]),
    },

    /* Handle - 0x0016 */
    /* 8 - FirmwareRevision - Characteristic Value */
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
        (UCHAR *)(&gatt_const_value_arr[94U]),
    },

    /* Handle - 0x0017 */
    /* 9 - SoftwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        25U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[99U]),
    },

    /* Handle - 0x0018 */
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
        44U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[104U]),
    },

    /* Handle - 0x0019 */
    /* 10 - SystemId - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        27U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[109U]),
    },

    /* Handle - 0x001A */
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
        46U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[114U]),
    },

    /* Handle - 0x001B */
    /* 11 - RegCertDataList - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        29U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[122U]),
    },

    /* Handle - 0x001C */
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
        48U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[127U]),
    },

    /* Handle - 0x001D */
    /* 12 - PnPID - Characteristic Declaration */
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
        (UCHAR *)(&gatt_const_value_arr[159U]),
    },

    /* Handle - 0x001E */
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
        50U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[164U]),
    },

    /* Handle - 0x001F */
    /* 3 - Battery - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        36U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[52U]),
    },

    /* Handle - 0x0020 */
    /* 13 - BatteryLevel - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        37U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[171U]),
    },

    /* Handle - 0x0021 */
    /* 13 - BatteryLevel - Characteristic Value */
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
        54U,

        /* Value */
        &gatt_value_arr[0U],
    },

    /* Handle - 0x0022 */
    /* 13 - BatteryLevel - CCD */
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
        44U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[2U],
    },

    /* Handle - 0x0023 */
    /* Characteristic 13 - BatteryLevel - Presentation Format */
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
        (UCHAR *)(&gatt_const_value_arr[176U]),
    },

    /* Handle - 0x0024 */
    /* 4 - HumanInterfaceDevice - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        64U,

        /* UUID Offset */
        0U,

        /* Value */
        (UCHAR *)(&gatt_const_uuid_arr[56U]),
    },

    /* Handle - 0x0025 */
    /* 14 - HIDProtocolMode - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        39U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[183U]),
    },

    /* Handle - 0x0026 */
    /* 14 - HIDProtocolMode - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY,

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

    /* Handle - 0x0027 */
    /* 15 - HIDReportMap - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        42U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[188U]),
    },

    /* Handle - 0x0028 */
    /* 15 - HIDReportMap - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (63U + 1U),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        60U,

        /* Value */
        &gatt_value_arr[2U],
    },

    /* Handle - 0x0029 */
    /* Characteristic 15 - HIDReportMap - HLD */
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
        62U,

        /* Value */
        &gatt_value_arr[66U],
    },

    /* Handle - 0x002A */
    /* 16 - Input HIDReport - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        46U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[193U]),
    },

    /* Handle - 0x002B */
    /* 16 - Input HIDReport - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (9U + 1U),

        /* Next Attribute Type Index */
        47U,

        /* UUID Offset */
        64U,

        /* Value */
        &gatt_db_peer_specific_val_arr[4U],
    },

    /* Handle - 0x002C */
    /* 16 - Input HIDReport - CCD */
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
        58U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[14U],
    },

    /* Handle - 0x002D */
    /* Characteristic 16 - Input HIDReport - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        48U,

        /* UUID Offset */
        66U,

        /* Value */
        &gatt_value_arr[68U],
    },

    /* Handle - 0x002E */
    /* 17 - Output HIDReport - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        49U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[198U]),
    },

    /* Handle - 0x002F */
    /* 17 - Output HIDReport - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY,

        /* Auxillary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (3U + 1U),

        /* Next Attribute Type Index */
        50U,

        /* UUID Offset */
        64U,

        /* Value */
        &gatt_db_peer_specific_val_arr[16U],
    },

    /* Handle - 0x0030 */
    /* Characteristic 17 - Output HIDReport - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2U,

        /* Next Attribute Type Index */
        51U,

        /* UUID Offset */
        66U,

        /* Value */
        &gatt_value_arr[70U],
    },

    /* Handle - 0x0031 */
    /* 18 - Feature HIDReport - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        52U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[203U]),
    },

    /* Handle - 0x0032 */
    /* 18 - Feature HIDReport - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxillary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (8U + 1U),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        64U,

        /* Value */
        &gatt_db_peer_specific_val_arr[20U],
    },

    /* Handle - 0x0033 */
    /* Characteristic 18 - Feature HIDReport - HLD */
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
        66U,

        /* Value */
        &gatt_value_arr[72U],
    },

    /* Handle - 0x0034 */
    /* 19 - HIDControlPoint - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        54U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[208U]),
    },

    /* Handle - 0x0035 */
    /* 19 - HIDControlPoint - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        1U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        68U,

        /* Value */
        &gatt_db_peer_specific_val_arr[29U],
    },

    /* Handle - 0x0036 */
    /* 20 - HIDInformation - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        56U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[213U]),
    },

    /* Handle - 0x0037 */
    /* 20 - HIDInformation - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (4U + 1U),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        70U,

        /* Value */
        &gatt_value_arr[74U],
    },

    /* Handle - 0x0038 */
    /* 21 - BootMouseInputReport - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        59U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[218U]),
    },

    /* Handle - 0x0039 */
    /* 21 - BootMouseInputReport - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (3U + 1U),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        72U,

        /* Value */
        &gatt_db_peer_specific_val_arr[30U],
    },

    /* Handle - 0x003A */
    /* 21 - BootMouseInputReport - CCD */
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
        61U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[34U],
    },

    /* Handle - 0x003B */
    /* 22 - BootKeyboardInputReport - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        62U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[223U]),
    },

    /* Handle - 0x003C */
    /* 22 - BootKeyboardInputReport - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (8U + 1U),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        74U,

        /* Value */
        &gatt_db_peer_specific_val_arr[36U],
    },

    /* Handle - 0x003D */
    /* 22 - BootKeyboardInputReport - CCD */
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
        69U,

        /* UUID Offset */
        12U,

        /* Value */
        &gatt_db_peer_specific_val_arr[45U],
    },

    /* Handle - 0x003E */
    /* 23 - BootKeyboardOutputReport - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        65U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[228U]),
    },

    /* Handle - 0x003F */
    /* 23 - BootKeyboardOutputReport - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY,

        /* Auxillary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (3U + 1U),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        76U,

        /* Value */
        &gatt_db_peer_specific_val_arr[47U],
    },

    /* Handle - 0x0040 */
    /* 5 - ScanParameter - Service Declaration */
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
        (UCHAR *)(&gatt_const_uuid_arr[78U]),
    },

    /* Handle - 0x0041 */
    /* 24 - ScanIntervalWindow - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5U,

        /* Next Attribute Type Index */
        67U,

        /* UUID Offset */
        6U,

        /* Value */
        (UCHAR *)(&gatt_const_value_arr[233U]),
    },

    /* Handle - 0x0042 */
    /* 24 - ScanIntervalWindow - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY,

        /* Auxillary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        4U,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        80U,

        /* Value */
        &gatt_db_peer_specific_val_arr[51U],
    },

    /* Handle - 0x0043 */
    /* 25 - ScanRefresh - Characteristic Declaration */
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
        (UCHAR *)(&gatt_const_value_arr[238U]),
    },

    /* Handle - 0x0044 */
    /* 25 - ScanRefresh - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxillary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (1U + 1U),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        82U,

        /* Value */
        &gatt_value_arr[79U],
    },

    /* Handle - 0x0045 */
    /* 25 - ScanRefresh - CCD */
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
        &gatt_db_peer_specific_val_arr[55U],
    },

};


DECL_CONST GATT_DB_CHARACERISTIC gatt_characteristic[GATT_CHARACTERISTIC_COUNT] =
{

    /* 0 - Service Changed */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0002U,

        /* Characteristic Service Index */
        0U,
    },

    /* 1 - Device Name */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0006U,

        /* Characteristic Service Index */
        1U,
    },

    /* 2 - Appearance */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0008U,

        /* Characteristic Service Index */
        1U,
    },

    /* 3 - Peripheral Preferred Connection Paramters */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x000AU,

        /* Characteristic Service Index */
        1U,
    },

    /* 4 - ManufacturerName */
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

    /* 5 - ModelNumber */
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

    /* 6 - SerialNumber */
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

    /* 8 - FirmwareRevision */
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

    /* 9 - SoftwareRevision */
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

    /* 10 - SystemId */
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

    /* 11 - RegCertDataList */
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

    /* 12 - PnPID */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x001DU,

        /* Characteristic Service Index */
        2U,
    },

    /* 13 - BatteryLevel */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4U,
        },

        /* Characteristic Start Handle */
        0x0020U,

        /* Characteristic Service Index */
        3U,
    },

    /* 14 - HIDProtocolMode */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0025U,

        /* Characteristic Service Index */
        4U,
    },

    /* 15 - HIDReportMap */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0027U,

        /* Characteristic Service Index */
        4U,
    },

    /* 16 - Input HIDReport */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4U,
        },

        /* Characteristic Start Handle */
        0x002AU,

        /* Characteristic Service Index */
        4U,
    },

    /* 17 - Output HIDReport */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x002EU,

        /* Characteristic Service Index */
        4U,
    },

    /* 18 - Feature HIDReport */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0031U,

        /* Characteristic Service Index */
        4U,
    },

    /* 19 - HIDControlPoint */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0034U,

        /* Characteristic Service Index */
        4U,
    },

    /* 20 - HIDInformation */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0036U,

        /* Characteristic Service Index */
        4U,
    },

    /* 21 - BootMouseInputReport */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0038U,

        /* Characteristic Service Index */
        4U,
    },

    /* 22 - BootKeyboardInputReport */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x003BU,

        /* Characteristic Service Index */
        4U,
    },

    /* 23 - BootKeyboardOutputReport */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x003EU,

        /* Characteristic Service Index */
        4U,
    },

    /* 24 - ScanIntervalWindow */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2U,
        },

        /* Characteristic Start Handle */
        0x0041U,

        /* Characteristic Service Index */
        5U,
    },

    /* 25 - ScanRefresh */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3U,
        },

        /* Characteristic Start Handle */
        0x0043U,

        /* Characteristic Service Index */
        5U,
    },
};


DECL_CONST GATT_DB_SERVICE gatt_service[GATT_SERVICE_COUNT] =
{

    /* 0 - GATT */
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
        0x0004U,

        /* Characteristic Start Index */
        0U,

        /* Characteristic End Index */
        0U
    },

    /* 1 - GAP */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0005U,

        /* Service End Handle */
        0x000BU,

        /* Characteristic Start Index */
        1U,

        /* Characteristic End Index */
        3U
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
        0x000CU,

        /* Service End Handle */
        0x001EU,

        /* Characteristic Start Index */
        4U,

        /* Characteristic End Index */
        12U
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
        0x001FU,

        /* Service End Handle */
        0x0023U,

        /* Characteristic Start Index */
        13U,

        /* Characteristic End Index */
        13U
    },

    /* 4 - HumanInterfaceDevice */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_LE_LINK_TYPE,

        /* Service Start Handle */
        0x0024U,

        /* Service End Handle */
        0x003FU,

        /* Characteristic Start Index */
        14U,

        /* Characteristic End Index */
        23U
    },

    /* 5 - ScanParameter */
    {
        {
            /* Number of attributes in Service */
            1U,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_LE_LINK_TYPE,

        /* Service Start Handle */
        0x0040U,

        /* Service End Handle */
        0x0045U,

        /* Characteristic Start Index */
        24U,

        /* Characteristic End Index */
        25U
    },
};

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_hid_gatt_db_register(void)
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
#endif /* ((defined HID) && (defined GATT_DB)) */
#endif /* GATT_DB_DYNAMIC */
#endif /* ATT */
