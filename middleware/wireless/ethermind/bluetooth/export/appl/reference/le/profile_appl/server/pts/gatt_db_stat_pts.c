
/**
 *  \file gatt_db_stat_pts.c
 *  GATT Database.
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
 *  0x0002  |       0x2803       | Rd               | 0x0A, 0x0003, 0x2A00
 *  --------+--------------------+------------------+---------------------
 *  0x0003  |       0x2A00       | Rd, Wr           | MindtreePTSServer
 *  --------+--------------------+------------------+---------------------
 *  0x0004  |       0x2803       | Rd               | 0x0A, 0x0005, 0x2A01
 *  --------+--------------------+------------------+---------------------
 *  0x0005  |       0x2A01       | Rd, Wr           | 768.0
 *  --------+--------------------+------------------+---------------------
 *  0x0006  |       0x2803       | Rd               | 0x0A, 0x0007, 0x2A04
 *  --------+--------------------+------------------+---------------------
 *  0x0007  |       0x2A04       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0008  |       0x2803       | Rd               | 0x02, 0x0009, 0x2AA6
 *  --------+--------------------+------------------+---------------------
 *  0x0009  |       0x2AA6       | Rd               | 1.0
 *  --------+--------------------+------------------+---------------------
 *  0x000A  |       0x2803       | Rd               | 0x02, 0x000B, 0x2AC9
 *  --------+--------------------+------------------+---------------------
 *  0x000B  |       0x2AC9       | Rd               | 255.0
 *  ========+====================+==================+=====================
 *  0x000C  |       0x2800       | Rd               | 0x1801
 *  --------+--------------------+------------------+---------------------
 *  0x000D  |       0x2803       | Rd               | 0x20, 0x000E, 0x2A05
 *  --------+--------------------+------------------+---------------------
 *  0x000E  |       0x2A05       | Ind              | N/A
 *  --------+--------------------+------------------+---------------------
 *  0x000F  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0010  |       0x2803       | Rd               | 0x0A, 0x0011, 0x2B29
 *  --------+--------------------+------------------+---------------------
 *  0x0011  |       0x2B29       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0012  |       0x2803       | Rd               | 0x02, 0x0013, 0x2B3A
 *  --------+--------------------+------------------+---------------------
 *  0x0013  |       0x2B3A       | Rd               | 0
 *  ========+====================+==================+=====================
 *  0x0014  |       0x2800       | Rd               | 0x180F
 *  --------+--------------------+------------------+---------------------
 *  0x0015  |       0x2803       | Rd               | 0x12, 0x0016, 0x2A19
 *  --------+--------------------+------------------+---------------------
 *  0x0016  |       0x2A19       | Rd, Ntf          | 100.0
 *  --------+--------------------+------------------+---------------------
 *  0x0017  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0018  |       0x2904       | Rd               | VALUE
 *  ========+====================+==================+=====================
 *  0x0019  |       0x2800       | Rd               | 0x180A
 *  --------+--------------------+------------------+---------------------
 *  0x001A  |       0x2803       | Rd               | 0x02, 0x001B, 0x2A29
 *  --------+--------------------+------------------+---------------------
 *  0x001B  |       0x2A29       | Rd               | Mindtree Limited.
 *  --------+--------------------+------------------+---------------------
 *  0x001C  |       0x2803       | Rd               | 0x02, 0x001D, 0x2A24
 *  --------+--------------------+------------------+---------------------
 *  0x001D  |       0x2A24       | Rd               | PTS-X.X.X
 *  --------+--------------------+------------------+---------------------
 *  0x001E  |       0x2803       | Rd               | 0x02, 0x001F, 0x2A25
 *  --------+--------------------+------------------+---------------------
 *  0x001F  |       0x2A25       | Rd               | X.X.X
 *  --------+--------------------+------------------+---------------------
 *  0x0020  |       0x2803       | Rd               | 0x02, 0x0021, 0x2A26
 *  --------+--------------------+------------------+---------------------
 *  0x0021  |       0x2A26       | Rd               | X.X.X
 *  --------+--------------------+------------------+---------------------
 *  0x0022  |       0x2803       | Rd               | 0x02, 0x0023, 0x2A27
 *  --------+--------------------+------------------+---------------------
 *  0x0023  |       0x2A27       | Rd               | X.X.X
 *  --------+--------------------+------------------+---------------------
 *  0x0024  |       0x2803       | Rd               | 0x02, 0x0025, 0x2A28
 *  --------+--------------------+------------------+---------------------
 *  0x0025  |       0x2A28       | Rd               | X.X.X
 *  --------+--------------------+------------------+---------------------
 *  0x0026  |       0x2803       | Rd               | 0x02, 0x0027, 0x2A23
 *  --------+--------------------+------------------+---------------------
 *  0x0027  |       0x2A23       | Rd               | 11223344.0
 *  --------+--------------------+------------------+---------------------
 *  0x0028  |       0x2803       | Rd               | 0x02, 0x0029, 0x2A2A
 *  --------+--------------------+------------------+---------------------
 *  0x0029  |       0x2A2A       | Rd               | 0.0
 *  --------+--------------------+------------------+---------------------
 *  0x002A  |       0x2803       | Rd               | 0x02, 0x002B, 0x2A50
 *  --------+--------------------+------------------+---------------------
 *  0x002B  |       0x2A50       | Rd               | 0
 *  ========+====================+==================+=====================
 *  0x002C  |       0x2800       | Rd               | 0xED00
 *  --------+--------------------+------------------+---------------------
 *  0x002D  |       0x2802       | Rd               | 0x0000, 0x0000, 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x002E  |       0x2803       | Rd               | 0x02, 0x002F, 0xED01
 *  --------+--------------------+------------------+---------------------
 *  0x002F  |       0xED01       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0030  |       0x2901       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0031  |       0x2803       | Rd               | 0x18, 0x0032, 0xED02
 *  --------+--------------------+------------------+---------------------
 *  0x0032  |       0xED02       | Wr, Ntf          | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0033  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0034  |       0xDDED       | Rd , Wr          | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0035  |       0xEEED       | Wr               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0036  |       0xFFED       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0037  |       0x2803       | Rd               | 0x8E, 0x0038, 0xED03
 *  --------+--------------------+------------------+---------------------
 *  0x0038  |       0xED03       | Rd, Wr, Wwr      | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0039  |       0x2900       | Rd               | 0x0002
 *  --------+--------------------+------------------+---------------------
 *  0x003A  |       0x2803       | Rd               | 0x0A, 0x003B, 0xED04
 *  --------+--------------------+------------------+---------------------
 *  0x003B  |       0xED04       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x003C  |       0x2904       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x003D  |       0x2803       | Rd               | 0x02, 0x003E, 0xED05
 *  --------+--------------------+------------------+---------------------
 *  0x003E  |       0xED05       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x003F  |       0x2904       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0040  |       0x2803       | Rd               | 0x0A, 0x0041, 0xED06
 *  --------+--------------------+------------------+---------------------
 *  0x0041  |       0xED06       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0042  |       0x2904       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0043  |       0x2803       | Rd               | 0x02, 0x0044, 0xED07
 *  --------+--------------------+------------------+---------------------
 *  0x0044  |       0xED07       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0045  |       0x2904       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0046  |       0x2803       | Rd               | 0x0A, 0x0047, 0xED08
 *  --------+--------------------+------------------+---------------------
 *  0x0047  |       0xED08       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0048  |       0x2904       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0049  |       0x2803       | Rd               | 0x02, 0x004A, 0xED09
 *  --------+--------------------+------------------+---------------------
 *  0x004A  |       0xED09       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x004B  |       0x2905       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x004C  |       0x2803       | Rd               | 0x42, 0x004D, 0xED0A
 *  --------+--------------------+------------------+---------------------
 *  0x004D  |       0xED0A       | Rd, Swr          | 0
 *  --------+--------------------+------------------+---------------------
 *  0x004E  |       0x2905       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x004F  |       0x2803       | Rd               | 0x02, 0x0050, 0xED0B
 *  --------+--------------------+------------------+---------------------
 *  0x0050  |       0xED0B       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0051  |       0x2905       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0052  |       0x2803       | Rd               | 0x06, 0x0053, 0xED0C
 *  --------+--------------------+------------------+---------------------
 *  0x0053  |       0xED0C       | Rd, Wwr          | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0054  |       0x2905       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0055  |       0x2803       | Rd               | 0x02, 0x0056, 0xED0D
 *  --------+--------------------+------------------+---------------------
 *  0x0056  |       0xED0D       | Rd               | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0057  |       0x2905       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0058  |       0xAAED       | Rd , Wr          | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0059  |       0xBBED       | Rd , Wr          | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x005A  |       0xCCED       | Rd , Wr          | VALUE
 *  ========+====================+==================+=====================
 *  0x005B  |       0x2801       | Rd               | 0xEE00
 *  --------+--------------------+------------------+---------------------
 *  0x005C  |       0x2803       | Rd               | 0x0B, 0x005D, 0xEE01
 *  --------+--------------------+------------------+---------------------
 *  0x005D  |       0xEE01       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x005E  |       0x2903       | Rd, Wr           | 0x0000
 *  ========+====================+==================+=====================
 *  0x005F  |       0x2801       | Rd               | 0xEF00
 *  --------+--------------------+------------------+---------------------
 *  0x0060  |       0x2803       | Rd               | 0x0B, 0x0061, 0xEF01
 *  --------+--------------------+------------------+---------------------
 *  0x0061  |       0xEF01       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0062  |       0x2903       | Rd, Wr           | 0x0000
 *  ========+====================+==================+=====================
 *  0x0063  |       0x2800       | Rd               | 0xEA00
 *  --------+--------------------+------------------+---------------------
 *  0x0064  |       0x2803       | Rd               | 0x0A, 0x0065, 0xEA01
 *  --------+--------------------+------------------+---------------------
 *  0x0065  |       0xEA01       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0066  |       0xDDED       | Rd , Wr          | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0067  |       0xEEED       | Wr               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0068  |       0xFFED       | Rd               | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0069  |       0x2803       | Rd               | 0x8A, 0x006A, 0xEA02
 *  --------+--------------------+------------------+---------------------
 *  0x006A  |       0xEA02       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x006B  |       0x2900       | Rd               | 0x0002
 *  --------+--------------------+------------------+---------------------
 *  0x006C  |       0x2901       | Rd, Wr           | VALUE
 *  ========+====================+==================+=====================
 *  0x006D  |       0x2800       | Rd               | 0xEB00
 *  --------+--------------------+------------------+---------------------
 *  0x006E  |       0x2803       | Rd               | 0x0A, 0x006F, 0xEB01
 *  --------+--------------------+------------------+---------------------
 *  0x006F  |       0xEB01       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x0070  |       0xDDED       | Rd , Wr          | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0071  |       0xEEED       | Rd , Wr          | VALUE
 *  --------+--------------------+------------------+---------------------
 *  0x0072  |       0xFFED       | Rd , Wr          | VALUE
 *  ========+====================+==================+=====================
 *  0x0073  |       0x2800       | Rd               | 0xDEAD
 *  --------+--------------------+------------------+---------------------
 *  0x0074  |       0x2803       | Rd               | 0x32, 0x0075, 0xFEED
 *  --------+--------------------+------------------+---------------------
 *  0x0075  |       0xFEED       | Rd, Ntf, Ind     | 100.0
 *  --------+--------------------+------------------+---------------------
 *  0x0076  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0077  |       0x2902       | Rd, Wr           | 0x0000
 *  --------+--------------------+------------------+---------------------
 *  0x0078  |       0x2803       | Rd               | 0x0A, 0x0079, 0xBEEF
 *  --------+--------------------+------------------+---------------------
 *  0x0079  |       0xBEEF       | Rd, Wr           | 0
 *  --------+--------------------+------------------+---------------------
 *  0x007A  |       0x2901       | Rd               | VALUE
 *  ========+====================+==================+=====================
 *  0x007B  |       0x2800       | Rd               | 0xEC00
 *  --------+--------------------+------------------+---------------------
 *  0x007C  |       0x2803       | Rd               | 0x0A, 0x007D, 0xEC01
 *  --------+--------------------+------------------+---------------------
 *  0x007D  |       0xEC01       | Rd, Wr           | 0
 *  ========+====================+==================+=====================
 *  0x007E  |       0x2800       | Rd               | 0xE900
 *  --------+--------------------+------------------+---------------------
 *  0x007F  |       0x2803       | Rd               | 0x42, 0x0080, 0xE901
 *  --------+--------------------+------------------+---------------------
 *  0x0080  |       0xE901       | Rd, Swr          | 0
 *  ========+====================+==================+=====================
 *  0x0081  |       0x2800       | Rd               | 0xE800
 *  --------+--------------------+------------------+---------------------
 *  0x0082  |       0x2803       | Rd               | 0x42, 0x0083, 0xE801
 *  --------+--------------------+------------------+---------------------
 *  0x0083  |       0xE801       | Rd, Swr          | 0
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "BT_gatt_db_api.h"
#include "gatt_db.h"
#include "gatt_defines.h"

#ifndef GATT_DB_DYNAMIC
#if ((defined PTS) && (defined GATT_DB))

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
    0x00, 0x28,

    /* 2 - Secondary Service UUID */
    0x01, 0x28,

    /* 4 - Include UUID */
    0x02, 0x28,

    /* 6 - Characteristic UUID */
    0x03, 0x28,

    /* 8 - Characteristic Extended Properties UUID */
    0x00, 0x29,

    /* 10 - Characteristic User Description UUID */
    0x01, 0x29,

    /* 12 - Client Configuration UUID */
    0x02, 0x29,

    /* 14 - Server Configuration UUID */
    0x03, 0x29,

    /* 16 - Characteristic Format UUID */
    0x04, 0x29,

    /* 18 - Characteristic Aggregate Format UUID */
    0x05, 0x29,

    /* 20 - GAP Service UUID */
    0x00, 0x18,

    /* 22 - DeviceName Characteristic UUID */
    0x00, 0x2A,

    /* 24 - Appearance Characteristic UUID */
    0x01, 0x2A,

    /* 26 - PeripheralPreferredConnectionParameters Characteristic UUID */
    0x04, 0x2A,

    /* 28 - CentralAddressResolution Characteristic UUID */
    0xA6, 0x2A,

    /* 30 - RPAOnly Characteristic UUID */
    0xC9, 0x2A,

    /* 32 - GATT Service UUID */
    0x01, 0x18,

    /* 34 - Service Changed Characteristic UUID */
    0x05, 0x2A,

    /* 36 - Client Supported Features Characteristic UUID */
    0x29, 0x2B,

    /* 38 - Server Supported Features Characteristic UUID */
    0x3A, 0x2B,

    /* 40 - Battery Service UUID */
    0x0F, 0x18,

    /* 42 - BatteryLevel Characteristic UUID */
    0x19, 0x2A,

    /* 44 - DeviceInformation Service UUID */
    0x0A, 0x18,

    /* 46 - ManufacturerName Characteristic UUID */
    0x29, 0x2A,

    /* 48 - ModelNumber Characteristic UUID */
    0x24, 0x2A,

    /* 50 - SerialNumber Characteristic UUID */
    0x25, 0x2A,

    /* 52 - FirmwareRevision Characteristic UUID */
    0x26, 0x2A,

    /* 54 - HardwareRevision Characteristic UUID */
    0x27, 0x2A,

    /* 56 - SoftwareRevision Characteristic UUID */
    0x28, 0x2A,

    /* 58 - SystemId Characteristic UUID */
    0x23, 0x2A,

    /* 60 - RegCertDataList Characteristic UUID */
    0x2A, 0x2A,

    /* 62 - PnPID Characteristic UUID */
    0x50, 0x2A,

    /* 64 - Dummy Service 1 Service UUID */
    0x00, 0xED,

    /* 66 - Dummy Characteristic 1 Characteristic UUID */
    0x01, 0xED,

    /* 68 - Dummy Characteristic 2 Characteristic UUID */
    0x02, 0xED,

    /* 70 - Dummy Characteristic 2 Characteristic HLD UUID */
    0xED, 0xDD,

    /* 72 - Dummy Characteristic 2 Characteristic HLD UUID */
    0xED, 0xEE,

    /* 74 - Dummy Characteristic 2 Characteristic HLD UUID */
    0xED, 0xFF,

    /* 76 - Dummy Characteristic 3 Characteristic UUID */
    0x03, 0xED,

    /* 78 - Dummy Characteristic 4 Characteristic UUID */
    0x04, 0xED,

    /* 80 - Dummy Characteristic 5 Characteristic UUID */
    0x05, 0xED,

    /* 82 - Dummy Characteristic 6 Characteristic UUID */
    0x06, 0xED,

    /* 84 - Dummy Characteristic 7 Characteristic UUID */
    0x07, 0xED,

    /* 86 - Dummy Characteristic 8 Characteristic UUID */
    0x08, 0xED,

    /* 88 - Dummy Characteristic 9 Characteristic UUID */
    0x09, 0xED,

    /* 90 - Dummy Characteristic 10 Characteristic UUID */
    0x0A, 0xED,

    /* 92 - Dummy Characteristic 11 Characteristic UUID */
    0x0B, 0xED,

    /* 94 - Dummy Characteristic 12 Characteristic UUID */
    0x0C, 0xED,

    /* 96 - Dummy Characteristic 13 Characteristic UUID */
    0x0D, 0xED,

    /* 98 - Dummy Characteristic 13 Characteristic HLD UUID */
    0xED, 0xAA,

    /* 100 - Dummy Characteristic 13 Characteristic HLD UUID */
    0xED, 0xBB,

    /* 102 - Dummy Characteristic 13 Characteristic HLD UUID */
    0xED, 0xCC,

    /* 104 - Dummy Service 2 Service UUID */
    0x00, 0xEE,

    /* 106 - Dummy Characteristic 14 Characteristic UUID */
    0x01, 0xEE,

    /* 108 - Dummy Service 3 Service UUID */
    0x00, 0xEF,

    /* 110 - Dummy Characteristic 15 Characteristic UUID */
    0x01, 0xEF,

    /* 112 - Dummy Service 4 Service UUID */
    0x00, 0xEA,

    /* 114 - Dummy Characteristic 16 Characteristic UUID */
    0x01, 0xEA,

    /* 116 - Dummy Characteristic 17 Characteristic UUID */
    0x02, 0xEA,

    /* 118 - Dummy Service 5 Service UUID */
    0x00, 0xEB,

    /* 120 - Dummy Characteristic 18 Characteristic UUID */
    0x01, 0xEB,

    /* 122 - Dummy Service 6 Service UUID */
    0xAD, 0xDE,

    /* 124 - Dummy Characteristic 19 Characteristic UUID */
    0xED, 0xFE,

    /* 126 - Dummy Characteristic 20 Characteristic UUID */
    0xEF, 0xBE,

    /* 128 - Dummy Service 7 Service UUID */
    0x00, 0xEC,

    /* 130 - Dummy Characteristic 21 Characteristic UUID */
    0x01, 0xEC,

    /* 132 - Dummy Service 8 Service UUID */
    0x00, 0xE9,

    /* 134 - Dummy Characteristic 22 Characteristic UUID */
    0x01, 0xE9,

    /* 136 - Dummy Service 9 Service UUID */
    0x00, 0xE8,

    /* 138 - Dummy Characteristic 23 Characteristic UUID */
    0x01, 0xE8,

};


UCHAR gatt_value_arr[GATT_VALUE_ARRAY_SIZE] =
{
    /* 0 - DeviceName(0); Default: MindtreePTSServer */
    0x11, 0x4D, 0x69, 0x6E, 0x64, 0x74, 0x72, 0x65, 0x65, 0x50, 0x54, 0x53, 0x53,
    0x65, 0x72, 0x76, 0x65, 0x72,

    /* 18 - Appearance(1); Default: 768.0 */
    0x03, 0x00,

    /* 20 - PeripheralPreferredConnectionParameters(2); Default: 0 */
    0x40, 0x00, 0x40, 0x00, 0x00, 0x00, 0x80, 0x0C,

    /* 28 - CentralAddressResolution(3); Default: 1.0 */
    0x01,

    /* 29 - RPAOnly(4); Default: 255.0 */
    0xFF,

    /* 30 - Server Supported Features(7); Default: 0 */
    0x01,

    /* 31 - HLD for Dummy Characteristic 2 */
    /* Profile Descriptor Data - 1  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ,

    /* 39 - HLD for Dummy Characteristic 2 */
    /* Profile Descriptor Data - 2  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ,

    /* 47 - HLD for Dummy Characteristic 2 */
    /* Profile Descriptor Data - 3  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,

    /* 55 - Dummy Characteristic 5(22); Default: 0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 85 - Dummy Characteristic 7(24); Default: 0 */
    0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 116 - Dummy Characteristic 9(26); Default: 0 */
    0x02, 0x00, 0x00,

    /* 119 - Dummy Characteristic 11(28); Default: 0 */
    0x00,

    /* 120 - Dummy Characteristic 13(30); Default: 0 */
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 126 - HLD for Dummy Characteristic 13 */
    /* Profile Descriptor Data - 1  */
    0x1B, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27,

    /* 154 - HLD for Dummy Characteristic 13 */
    /* Profile Descriptor Data - 2  */
    0x1B, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27,

    /* 182 - HLD for Dummy Characteristic 13 */
    /* Profile Descriptor Data - 3  */
    0x1B, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11,
    0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23,
    0x24, 0x25, 0x26, 0x27,

    /* 210 - HLD for Dummy Characteristic 16 */
    /* Profile Descriptor Data - 1  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ,

    /* 218 - HLD for Dummy Characteristic 16 */
    /* Profile Descriptor Data - 2  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ,

    /* 226 - HLD for Dummy Characteristic 16 */
    /* Profile Descriptor Data - 3  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,

    /* 234 - User Description for Dummy Characteristic 17 */
    0x0A, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10,

    /* 245 - Dummy Characteristic 18(35); Default: 0 */
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 256 - HLD for Dummy Characteristic 18 */
    /* Profile Descriptor Data - 1  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ,

    /* 264 - HLD for Dummy Characteristic 18 */
    /* Profile Descriptor Data - 2  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 ,

    /* 272 - HLD for Dummy Characteristic 18 */
    /* Profile Descriptor Data - 3  */
    0x07, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,

    /* 280 - Dummy Characteristic 19(36); Default: 100.0 */
    0x01, 0x64,

    /* 282 - Dummy Characteristic 20(37); Default: 0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    /* 382 - Dummy Characteristic 21(38); Default: 0 */
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 393 - Dummy Characteristic 22(39); Default: 0 */
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 399 - Dummy Characteristic 23(40); Default: 0 */
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00,
};


DECL_CONST UCHAR gatt_const_value_arr[GATT_CONST_VALUE_ARRAY_SIZE] =
{

    /* 0 - DeviceName(0); Property: 0x0A, Value Handle: 0x0003, UUID: 0x2A00 */
    0x0A, 0x03, 0x00, 0x00, 0x2A,

    /* 5 - Appearance(1); Property: 0x0A, Value Handle: 0x0005, UUID: 0x2A01 */
    0x0A, 0x05, 0x00, 0x01, 0x2A,

    /* 10 - PeripheralPreferredConnectionParameters(2); Property: 0x0A, Value Handle: 0x0007, UUID: 0x2A04 */
    0x0A, 0x07, 0x00, 0x04, 0x2A,

    /* 15 - CentralAddressResolution(3); Property: 0x02, Value Handle: 0x0009, UUID: 0x2AA6 */
    0x02, 0x09, 0x00, 0xA6, 0x2A,

    /* 20 - RPAOnly(4); Property: 0x02, Value Handle: 0x000B, UUID: 0x2AC9 */
    0x02, 0x0B, 0x00, 0xC9, 0x2A,

    /* 25 - Service Changed(5); Property: 0x20, Value Handle: 0x000E, UUID: 0x2A05 */
    0x20, 0x0E, 0x00, 0x05, 0x2A,

    /* 30 - Client Supported Features(6); Property: 0x0A, Value Handle: 0x0011, UUID: 0x2B29 */
    0x0A, 0x11, 0x00, 0x29, 0x2B,

    /* 35 - Server Supported Features(7); Property: 0x02, Value Handle: 0x0013, UUID: 0x2B3A */
    0x02, 0x13, 0x00, 0x3A, 0x2B,

    /* 40 - BatteryLevel(8); Property: 0x12, Value Handle: 0x0016, UUID: 0x2A19 */
    0x12, 0x16, 0x00, 0x19, 0x2A,

    /* 45 - Presentation Format for BatteryLevel */
    /**
     * Characteristic Presentation Format for BatteryLevel
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x04, 0x00, 0xAD, 0x27, 0x01, 0x00, 0x00,

    /* 52 - ManufacturerName(9); Property: 0x02, Value Handle: 0x001B, UUID: 0x2A29 */
    0x02, 0x1B, 0x00, 0x29, 0x2A,

    /* 57 - ManufacturerName(9); Default: Mindtree Limited. */
    0x4D, 0x69, 0x6E, 0x64, 0x74, 0x72, 0x65, 0x65, 0x20, 0x4C, 0x69, 0x6D,
    0x69, 0x74, 0x65, 0x64, 0x2E,

    /* 74 - ModelNumber(10); Property: 0x02, Value Handle: 0x001D, UUID: 0x2A24 */
    0x02, 0x1D, 0x00, 0x24, 0x2A,

    /* 79 - ModelNumber(10); Default: PTS-X.X.X */
    0x50, 0x54, 0x53, 0x2D, 0x58, 0x2E, 0x58, 0x2E, 0x58,

    /* 88 - SerialNumber(11); Property: 0x02, Value Handle: 0x001F, UUID: 0x2A25 */
    0x02, 0x1F, 0x00, 0x25, 0x2A,

    /* 93 - SerialNumber(11); Default: X.X.X */
    0x58, 0x2E, 0x58, 0x2E, 0x58,

    /* 98 - FirmwareRevision(12); Property: 0x02, Value Handle: 0x0021, UUID: 0x2A26 */
    0x02, 0x21, 0x00, 0x26, 0x2A,

    /* 103 - FirmwareRevision(12); Default: X.X.X */
    0x58, 0x2E, 0x58, 0x2E, 0x58,

    /* 108 - HardwareRevision(13); Property: 0x02, Value Handle: 0x0023, UUID: 0x2A27 */
    0x02, 0x23, 0x00, 0x27, 0x2A,

    /* 113 - HardwareRevision(13); Default: X.X.X */
    0x58, 0x2E, 0x58, 0x2E, 0x58,

    /* 118 - SoftwareRevision(14); Property: 0x02, Value Handle: 0x0025, UUID: 0x2A28 */
    0x02, 0x25, 0x00, 0x28, 0x2A,

    /* 123 - SoftwareRevision(14); Default: X.X.X */
    0x58, 0x2E, 0x58, 0x2E, 0x58,

    /* 128 - SystemId(15); Property: 0x02, Value Handle: 0x0027, UUID: 0x2A23 */
    0x02, 0x27, 0x00, 0x23, 0x2A,

    /* 133 - SystemId(15); Default: 11223344.0 */
    0x31, 0x31, 0x32, 0x32, 0x33, 0x33, 0x34, 0x34,

    /* 141 - RegCertDataList(16); Property: 0x02, Value Handle: 0x0029, UUID: 0x2A2A */
    0x02, 0x29, 0x00, 0x2A, 0x2A,

    /* 146 - RegCertDataList(16); Default: 0.0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 178 - PnPID(17); Property: 0x02, Value Handle: 0x002B, UUID: 0x2A50 */
    0x02, 0x2B, 0x00, 0x50, 0x2A,

    /* 183 - PnPID(17); Default: 0 */
    0x01, 0x6A, 0x00, 0x4D, 0x01, 0x00, 0x01,

    /* 190 - Dummy Service 2 ; Start Handle: 0x005B, End Handle: 0x005E, UUID: 0xEE00 */
    0x5B, 0x00, 0x5E, 0x00, 0x00, 0xEE,

    /* 196 - Dummy Characteristic 1(18); Property: 0x02, Value Handle: 0x002F, UUID: 0xED01 */
    0x02, 0x2F, 0x00, 0x01, 0xED,

    /* 201 - Dummy Characteristic 1(18); Default: 0 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,

    /* 301 - User Description for Dummy Characteristic 1 */
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10,

    /* 311 - Dummy Characteristic 2(19); Property: 0x18, Value Handle: 0x0032, UUID: 0xED02 */
    0x18, 0x32, 0x00, 0x02, 0xED,

    /* 316 - Dummy Characteristic 3(20); Property: 0x8E, Value Handle: 0x0038, UUID: 0xED03 */
    0x8E, 0x38, 0x00, 0x03, 0xED,

    /* 321 - Characteristic Extended Properties for Dummy Characteristic 3 */
    0x02, 0x00,

    /* 323 - Dummy Characteristic 4(21); Property: 0x0A, Value Handle: 0x003B, UUID: 0xED04 */
    0x0A, 0x3B, 0x00, 0x04, 0xED,

    /* 328 - Presentation Format for Dummy Characteristic 4 */
    /**
     * Characteristic Presentation Format - 1
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x03, 0x00, 0x60, 0x27, 0x97, 0x55, 0x55,

    /* 335 - Dummy Characteristic 5(22); Property: 0x02, Value Handle: 0x003E, UUID: 0xED05 */
    0x02, 0x3E, 0x00, 0x05, 0xED,

    /* 340 - Presentation Format for Dummy Characteristic 5 */
    /**
     * Characteristic Presentation Format - 2
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x03, 0x00, 0x34, 0x27, 0x1E, 0x55, 0x55,

    /* 347 - Dummy Characteristic 6(23); Property: 0x0A, Value Handle: 0x0041, UUID: 0xED06 */
    0x0A, 0x41, 0x00, 0x06, 0xED,

    /* 352 - Presentation Format for Dummy Characteristic 6 */
    /**
     * Characteristic Presentation Format - 3
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x03, 0x00, 0x20, 0x27, 0x96, 0x55, 0x55,

    /* 359 - Dummy Characteristic 7(24); Property: 0x02, Value Handle: 0x0044, UUID: 0xED07 */
    0x02, 0x44, 0x00, 0x07, 0xED,

    /* 364 - Presentation Format for Dummy Characteristic 7 */
    /**
     * Characteristic Presentation Format - 4
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x16, 0x00, 0x27, 0x27, 0x86, 0x00, 0x00,

    /* 371 - Dummy Characteristic 8(25); Property: 0x0A, Value Handle: 0x0047, UUID: 0xED08 */
    0x0A, 0x47, 0x00, 0x08, 0xED,

    /* 376 - Presentation Format for Dummy Characteristic 8 */
    /**
     * Characteristic Presentation Format - 5
     * - Format            (1 octet)
     * - Exponent          (1 octet)
     * - Unit              (2 octet)
     * - Name Space        (1 octet)
     * - Description       (2 octet)
     */
    0x16, 0x00, 0x42, 0x27, 0x18, 0x00, 0x00,

    /* 383 - Dummy Characteristic 9(26); Property: 0x02, Value Handle: 0x004A, UUID: 0xED09 */
    0x02, 0x4A, 0x00, 0x09, 0xED,

    /* 388 - Aggregate Format for Dummy Characteristic 9 */
    0x18, 0x00, 0x3C, 0x00, 0x3F, 0x00,

    /* 394 - Dummy Characteristic 10(27); Property: 0x42, Value Handle: 0x004D, UUID: 0xED0A */
    0x42, 0x4D, 0x00, 0x0A, 0xED,

    /* 399 - Aggregate Format for Dummy Characteristic 10 */
    0x3F, 0x00, 0x42, 0x00,

    /* 403 - Dummy Characteristic 11(28); Property: 0x02, Value Handle: 0x0050, UUID: 0xED0B */
    0x02, 0x50, 0x00, 0x0B, 0xED,

    /* 408 - Aggregate Format for Dummy Characteristic 11 */
    0x42, 0x00, 0x45, 0x00, 0x48, 0x00,

    /* 414 - Dummy Characteristic 12(29); Property: 0x06, Value Handle: 0x0053, UUID: 0xED0C */
    0x06, 0x53, 0x00, 0x0C, 0xED,

    /* 419 - Aggregate Format for Dummy Characteristic 12 */
    0x18, 0x00, 0x3C, 0x00, 0x3F, 0x00, 0x42, 0x00,

    /* 427 - Dummy Characteristic 13(30); Property: 0x02, Value Handle: 0x0056, UUID: 0xED0D */
    0x02, 0x56, 0x00, 0x0D, 0xED,

    /* 432 - Aggregate Format for Dummy Characteristic 13 */
    0x18, 0x00, 0x3C, 0x00,

    /* 436 - Dummy Characteristic 14(31); Property: 0x0B, Value Handle: 0x005D, UUID: 0xEE01 */
    0x0B, 0x5D, 0x00, 0x01, 0xEE,

    /* 441 - Dummy Characteristic 15(32); Property: 0x0B, Value Handle: 0x0061, UUID: 0xEF01 */
    0x0B, 0x61, 0x00, 0x01, 0xEF,

    /* 446 - Dummy Characteristic 16(33); Property: 0x0A, Value Handle: 0x0065, UUID: 0xEA01 */
    0x0A, 0x65, 0x00, 0x01, 0xEA,

    /* 451 - Dummy Characteristic 17(34); Property: 0x8A, Value Handle: 0x006A, UUID: 0xEA02 */
    0x8A, 0x6A, 0x00, 0x02, 0xEA,

    /* 456 - Characteristic Extended Properties for Dummy Characteristic 17 */
    0x02, 0x00,

    /* 458 - Dummy Characteristic 18(35); Property: 0x0A, Value Handle: 0x006F, UUID: 0xEB01 */
    0x0A, 0x6F, 0x00, 0x01, 0xEB,

    /* 463 - Dummy Characteristic 19(36); Property: 0x32, Value Handle: 0x0075, UUID: 0xFEED */
    0x32, 0x75, 0x00, 0xED, 0xFE,

    /* 468 - Dummy Characteristic 20(37); Property: 0x0A, Value Handle: 0x0079, UUID: 0xBEEF */
    0x0A, 0x79, 0x00, 0xEF, 0xBE,

    /* 473 - User Description for Dummy Characteristic 20 */
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
    0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40,
    0x41,

    /* 538 - Dummy Characteristic 21(38); Property: 0x0A, Value Handle: 0x007D, UUID: 0xEC01 */
    0x0A, 0x7D, 0x00, 0x01, 0xEC,

    /* 543 - Dummy Characteristic 22(39); Property: 0x42, Value Handle: 0x0080, UUID: 0xE901 */
    0x42, 0x80, 0x00, 0x01, 0xE9,

    /* 548 - Dummy Characteristic 23(40); Property: 0x42, Value Handle: 0x0083, UUID: 0xE801 */
    0x42, 0x83, 0x00, 0x01, 0xE8,
};


UCHAR gatt_db_peer_specific_val_arr[GATT_DB_MAX_PEER_CONFIGURATION];

DECL_CONST UCHAR gatt_db_const_peer_specific_val_arr[GATT_DB_PEER_VALUE_ARRAY_SIZE] =
{
    /* 0 - Client Configuration for Service Changed */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 2 - Client Supported Features(6); Default: 0 */
    /* Client Instance 0 */
    0x00,

    /* 3 - BatteryLevel(8); Default: 100.0 */
    /* Client Instance 0 */
    0x01, 0x64,

    /* 5 - Client Configuration for BatteryLevel */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 7 - Dummy Characteristic 2(19); Default: 0 */
    /* Client Instance 0 */
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 18 - Client Configuration for Dummy Characteristic 2 */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 20 - Dummy Characteristic 3(20); Default: 0 */
    /* Client Instance 0 */
    0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 41 - Dummy Characteristic 4(21); Default: 0 */
    /* Client Instance 0 */
    0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 72 - Dummy Characteristic 6(23); Default: 0 */
    /* Client Instance 0 */
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 83 - Dummy Characteristic 8(25); Default: 0 */
    /* Client Instance 0 */
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 89 - Dummy Characteristic 10(27); Default: 0 */
    /* Client Instance 0 */
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 95 - Dummy Characteristic 12(29); Default: 0 */
    /* Client Instance 0 */
    0x04, 0x00, 0x00, 0x00, 0x00,

    /* 100 - Dummy Characteristic 14(31); Default: 0 */
    /* Client Instance 0 */
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 111 - Server Configuration for Dummy Characteristic 14 */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 113 - Dummy Characteristic 15(32); Default: 0 */
    /* Client Instance 0 */
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 124 - Server Configuration for Dummy Characteristic 15 */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 126 - Dummy Characteristic 16(33); Default: 0 */
    /* Client Instance 0 */
    0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 137 - Dummy Characteristic 17(34); Default: 0 */
    /* Client Instance 0 */
    0x05, 0x00, 0x00, 0x00, 0x00, 0x00,

    /* 143 - Client Configuration for Dummy Characteristic 19 */
    /* Client Instance 0 */
    0x00, 0x00,

    /* 145 - Client Configuration for Dummy Characteristic 19 */
    /* Client Instance 0 */
    0x00, 0x00,
};


DECL_CONST GATT_ATTR_TYPE gatt_type_table[GATT_DB_MAX_TYPE_COUNT] =
{
    {
        /* UUID Offset */
        0,

        /* First Occurrence for Type */
        1,

        /* Last Occurrence for Type */
        129,

    },
    {
        /* UUID Offset */
        2,

        /* First Occurrence for Type */
        91,

        /* Last Occurrence for Type */
        95,

    },
    {
        /* UUID Offset */
        4,

        /* First Occurrence for Type */
        45,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        6,

        /* First Occurrence for Type */
        2,

        /* Last Occurrence for Type */
        130,

    },
    {
        /* UUID Offset */
        8,

        /* First Occurrence for Type */
        57,

        /* Last Occurrence for Type */
        107,

    },
    {
        /* UUID Offset */
        10,

        /* First Occurrence for Type */
        48,

        /* Last Occurrence for Type */
        122,

    },
    {
        /* UUID Offset */
        12,

        /* First Occurrence for Type */
        15,

        /* Last Occurrence for Type */
        119,

    },
    {
        /* UUID Offset */
        14,

        /* First Occurrence for Type */
        94,

        /* Last Occurrence for Type */
        98,

    },
    {
        /* UUID Offset */
        16,

        /* First Occurrence for Type */
        24,

        /* Last Occurrence for Type */
        72,

    },
    {
        /* UUID Offset */
        18,

        /* First Occurrence for Type */
        75,

        /* Last Occurrence for Type */
        87,

    },
    {
        /* UUID Offset */
        20,

        /* First Occurrence for Type */
        1,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        22,

        /* First Occurrence for Type */
        3,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        24,

        /* First Occurrence for Type */
        5,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        26,

        /* First Occurrence for Type */
        7,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        28,

        /* First Occurrence for Type */
        9,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        30,

        /* First Occurrence for Type */
        11,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        32,

        /* First Occurrence for Type */
        12,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        34,

        /* First Occurrence for Type */
        14,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        36,

        /* First Occurrence for Type */
        17,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        38,

        /* First Occurrence for Type */
        19,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        40,

        /* First Occurrence for Type */
        20,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        42,

        /* First Occurrence for Type */
        22,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        44,

        /* First Occurrence for Type */
        25,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        46,

        /* First Occurrence for Type */
        27,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        48,

        /* First Occurrence for Type */
        29,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        50,

        /* First Occurrence for Type */
        31,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        52,

        /* First Occurrence for Type */
        33,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        54,

        /* First Occurrence for Type */
        35,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        56,

        /* First Occurrence for Type */
        37,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        58,

        /* First Occurrence for Type */
        39,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        60,

        /* First Occurrence for Type */
        41,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        62,

        /* First Occurrence for Type */
        43,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        64,

        /* First Occurrence for Type */
        44,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        66,

        /* First Occurrence for Type */
        47,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        68,

        /* First Occurrence for Type */
        50,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        70,

        /* First Occurrence for Type */
        52,

        /* Last Occurrence for Type */
        112,

    },
    {
        /* UUID Offset */
        72,

        /* First Occurrence for Type */
        53,

        /* Last Occurrence for Type */
        113,

    },
    {
        /* UUID Offset */
        74,

        /* First Occurrence for Type */
        54,

        /* Last Occurrence for Type */
        114,

    },
    {
        /* UUID Offset */
        76,

        /* First Occurrence for Type */
        56,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        78,

        /* First Occurrence for Type */
        59,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        80,

        /* First Occurrence for Type */
        62,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        82,

        /* First Occurrence for Type */
        65,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        84,

        /* First Occurrence for Type */
        68,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        86,

        /* First Occurrence for Type */
        71,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        88,

        /* First Occurrence for Type */
        74,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        90,

        /* First Occurrence for Type */
        77,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        92,

        /* First Occurrence for Type */
        80,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        94,

        /* First Occurrence for Type */
        83,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        96,

        /* First Occurrence for Type */
        86,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        98,

        /* First Occurrence for Type */
        88,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        100,

        /* First Occurrence for Type */
        89,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        102,

        /* First Occurrence for Type */
        90,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        104,

        /* First Occurrence for Type */
        91,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        106,

        /* First Occurrence for Type */
        93,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        108,

        /* First Occurrence for Type */
        95,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        110,

        /* First Occurrence for Type */
        97,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        112,

        /* First Occurrence for Type */
        99,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        114,

        /* First Occurrence for Type */
        101,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        116,

        /* First Occurrence for Type */
        106,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        118,

        /* First Occurrence for Type */
        109,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        120,

        /* First Occurrence for Type */
        111,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        122,

        /* First Occurrence for Type */
        115,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        124,

        /* First Occurrence for Type */
        117,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        126,

        /* First Occurrence for Type */
        121,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        128,

        /* First Occurrence for Type */
        123,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        130,

        /* First Occurrence for Type */
        125,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        132,

        /* First Occurrence for Type */
        126,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        134,

        /* First Occurrence for Type */
        128,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        136,

        /* First Occurrence for Type */
        129,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
    {
        /* UUID Offset */
        138,

        /* First Occurrence for Type */
        131,

        /* Last Occurrence for Type */
        ATT_INVALID_ATTR_HANDLE_VAL,

    },
};


DECL_CONST GATT_DB_DESC_DATA gatt_db_attr_table[GATT_DB_MAX_ATTRIBUTES] =
{
    /* Dummy */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
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

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        12,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 20),
    },

    /* Handle - 0x0002 */
    /* 0 - DeviceName - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        4,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 0),
    },

    /* Handle - 0x0003 */
    /* 0 - DeviceName - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (17 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        22,

        /* Value */
        gatt_value_arr + 0,
    },

    /* Handle - 0x0004 */
    /* 1 - Appearance - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        6,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 5),
    },

    /* Handle - 0x0005 */
    /* 1 - Appearance - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        24,

        /* Value */
        gatt_value_arr + 18,
    },

    /* Handle - 0x0006 */
    /* 2 - PeripheralPreferredConnectionParameters - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        8,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 10),
    },

    /* Handle - 0x0007 */
    /* 2 - PeripheralPreferredConnectionParameters - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        8,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        26,

        /* Value */
        gatt_value_arr + 20,
    },

    /* Handle - 0x0008 */
    /* 3 - CentralAddressResolution - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        10,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 15),
    },

    /* Handle - 0x0009 */
    /* 3 - CentralAddressResolution - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        28,

        /* Value */
        gatt_value_arr + 28,
    },

    /* Handle - 0x000A */
    /* 4 - RPAOnly - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        13,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 20),
    },

    /* Handle - 0x000B */
    /* 4 - RPAOnly - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        30,

        /* Value */
        gatt_value_arr + 29,
    },

    /* Handle - 0x000C */
    /* 1 - GATT - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        20,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 32),
    },

    /* Handle - 0x000D */
    /* 5 - Service Changed - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        16,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 25),
    },

    /* Handle - 0x000E */
    /* 5 - Service Changed - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_INDICATE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length */
        0,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        34,

        /* Value */
        NULL,
    },

    /* Handle - 0x000F */
    /* 5 - Service Changed - CCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        23,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_arr + 0,
    },

    /* Handle - 0x0010 */
    /* 6 - Client Supported Features - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        18,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 30),
    },

    /* Handle - 0x0011 */
    /* 6 - Client Supported Features - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        1,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        36,

        /* Value */
        gatt_db_peer_specific_val_arr + 2,
    },

    /* Handle - 0x0012 */
    /* 7 - Server Supported Features - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        21,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 35),
    },

    /* Handle - 0x0013 */
    /* 7 - Server Supported Features - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        38,

        /* Value */
        gatt_value_arr + 30,
    },

    /* Handle - 0x0014 */
    /* 2 - Battery - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        25,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 40),
    },

    /* Handle - 0x0015 */
    /* 8 - BatteryLevel - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        26,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 40),
    },

    /* Handle - 0x0016 */
    /* 8 - BatteryLevel - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (1 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        42,

        /* Value */
        gatt_db_peer_specific_val_arr + 3,
    },

    /* Handle - 0x0017 */
    /* 8 - BatteryLevel - CCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        51,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_arr + 5,
    },

    /* Handle - 0x0018 */
    /* Characteristic 8 - BatteryLevel - Presentation Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        60,

        /* UUID Offset */
        16,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 45),
    },

    /* Handle - 0x0019 */
    /* 3 - DeviceInformation - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        44,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 44),
    },

    /* Handle - 0x001A */
    /* 9 - ManufacturerName - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        28,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 52),
    },

    /* Handle - 0x001B */
    /* 9 - ManufacturerName - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        17,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        46,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 57),
    },

    /* Handle - 0x001C */
    /* 10 - ModelNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        30,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 74),
    },

    /* Handle - 0x001D */
    /* 10 - ModelNumber - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        9,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        48,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 79),
    },

    /* Handle - 0x001E */
    /* 11 - SerialNumber - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        32,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 88),
    },

    /* Handle - 0x001F */
    /* 11 - SerialNumber - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        50,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 93),
    },

    /* Handle - 0x0020 */
    /* 12 - FirmwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        34,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 98),
    },

    /* Handle - 0x0021 */
    /* 12 - FirmwareRevision - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        52,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 103),
    },

    /* Handle - 0x0022 */
    /* 13 - HardwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        36,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 108),
    },

    /* Handle - 0x0023 */
    /* 13 - HardwareRevision - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        54,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 113),
    },

    /* Handle - 0x0024 */
    /* 14 - SoftwareRevision - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        38,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 118),
    },

    /* Handle - 0x0025 */
    /* 14 - SoftwareRevision - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        56,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 123),
    },

    /* Handle - 0x0026 */
    /* 15 - SystemId - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        40,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 128),
    },

    /* Handle - 0x0027 */
    /* 15 - SystemId - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        8,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        58,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 133),
    },

    /* Handle - 0x0028 */
    /* 16 - RegCertDataList - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        42,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 141),
    },

    /* Handle - 0x0029 */
    /* 16 - RegCertDataList - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        32,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        60,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 146),
    },

    /* Handle - 0x002A */
    /* 17 - PnPID - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        46,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 178),
    },

    /* Handle - 0x002B */
    /* 17 - PnPID - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        62,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 183),
    },

    /* Handle - 0x002C */
    /* 4 - Dummy Service 1 - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        99,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 64),
    },

    /* Handle - 0x002D */
    /* 0 - Dummy Service 2 - Include Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        6,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        4,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 190),
    },

    /* Handle - 0x002E */
    /* 18 - Dummy Characteristic 1 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        49,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 196),
    },

    /* Handle - 0x002F */
    /* 18 - Dummy Characteristic 1 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        100,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        66,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 201),
    },

    /* Handle - 0x0030 */
    /* Characteristic 18 - Dummy Characteristic 1 - User Description */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length */
        10,

        /* Next Attribute Type Index */
        108,

        /* UUID Offset */
        10,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 301),
    },

    /* Handle - 0x0031 */
    /* 19 - Dummy Characteristic 2 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        55,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 311),
    },

    /* Handle - 0x0032 */
    /* 19 - Dummy Characteristic 2 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (10 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        68,

        /* Value */
        gatt_db_peer_specific_val_arr + 7,
    },

    /* Handle - 0x0033 */
    /* 19 - Dummy Characteristic 2 - CCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        118,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_arr + 18,
    },

    /* Handle - 0x0034 */
    /* Characteristic 19 - Dummy Characteristic 2 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        102,

        /* UUID Offset */
        70,

        /* Value */
        gatt_value_arr + 31,
    },

    /* Handle - 0x0035 */
    /* Characteristic 19 - Dummy Characteristic 2 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        103,

        /* UUID Offset */
        72,

        /* Value */
        gatt_value_arr + 39,
    },

    /* Handle - 0x0036 */
    /* Characteristic 19 - Dummy Characteristic 2 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        104,

        /* UUID Offset */
        74,

        /* Value */
        gatt_value_arr + 47,
    },

    /* Handle - 0x0037 */
    /* 20 - Dummy Characteristic 3 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        58,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 316),
    },

    /* Handle - 0x0038 */
    /* 20 - Dummy Characteristic 3 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (20 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        76,

        /* Value */
        gatt_db_peer_specific_val_arr + 20,
    },

    /* Handle - 0x0039 */
    /* Characteristic 20 - Dummy Characteristic 3 - Extended Properties */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        107,

        /* UUID Offset */
        8,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 321),
    },

    /* Handle - 0x003A */
    /* 21 - Dummy Characteristic 4 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        61,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 323),
    },

    /* Handle - 0x003B */
    /* 21 - Dummy Characteristic 4 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (30 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        78,

        /* Value */
        gatt_db_peer_specific_val_arr + 41,
    },

    /* Handle - 0x003C */
    /* Characteristic 21 - Dummy Characteristic 4 - Presentation Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        63,

        /* UUID Offset */
        16,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 328),
    },

    /* Handle - 0x003D */
    /* 22 - Dummy Characteristic 5 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        64,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 335),
    },

    /* Handle - 0x003E */
    /* 22 - Dummy Characteristic 5 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        30,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        80,

        /* Value */
        gatt_value_arr + 55,
    },

    /* Handle - 0x003F */
    /* Characteristic 22 - Dummy Characteristic 5 - Presentation Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        66,

        /* UUID Offset */
        16,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 340),
    },

    /* Handle - 0x0040 */
    /* 23 - Dummy Characteristic 6 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        67,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 347),
    },

    /* Handle - 0x0041 */
    /* 23 - Dummy Characteristic 6 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (10 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        82,

        /* Value */
        gatt_db_peer_specific_val_arr + 72,
    },

    /* Handle - 0x0042 */
    /* Characteristic 23 - Dummy Characteristic 6 - Presentation Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        69,

        /* UUID Offset */
        16,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 352),
    },

    /* Handle - 0x0043 */
    /* 24 - Dummy Characteristic 7 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        70,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 359),
    },

    /* Handle - 0x0044 */
    /* 24 - Dummy Characteristic 7 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (30 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        84,

        /* Value */
        gatt_value_arr + 85,
    },

    /* Handle - 0x0045 */
    /* Characteristic 24 - Dummy Characteristic 7 - Presentation Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        72,

        /* UUID Offset */
        16,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 364),
    },

    /* Handle - 0x0046 */
    /* 25 - Dummy Characteristic 8 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        73,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 371),
    },

    /* Handle - 0x0047 */
    /* 25 - Dummy Characteristic 8 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (5 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        86,

        /* Value */
        gatt_db_peer_specific_val_arr + 83,
    },

    /* Handle - 0x0048 */
    /* Characteristic 25 - Dummy Characteristic 8 - Presentation Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        7,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        16,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 376),
    },

    /* Handle - 0x0049 */
    /* 26 - Dummy Characteristic 9 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        76,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 383),
    },

    /* Handle - 0x004A */
    /* 26 - Dummy Characteristic 9 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (2 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        88,

        /* Value */
        gatt_value_arr + 116,
    },

    /* Handle - 0x004B */
    /* Characteristic 26 - Dummy Characteristic 9 - Aggregate Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        6,

        /* Next Attribute Type Index */
        78,

        /* UUID Offset */
        18,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 388),
    },

    /* Handle - 0x004C */
    /* 27 - Dummy Characteristic 10 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        79,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 394),
    },

    /* Handle - 0x004D */
    /* 27 - Dummy Characteristic 10 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_SIGNED_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (5 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        90,

        /* Value */
        gatt_db_peer_specific_val_arr + 89,
    },

    /* Handle - 0x004E */
    /* Characteristic 27 - Dummy Characteristic 10 - Aggregate Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        4,

        /* Next Attribute Type Index */
        81,

        /* UUID Offset */
        18,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 399),
    },

    /* Handle - 0x004F */
    /* 28 - Dummy Characteristic 11 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        82,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 403),
    },

    /* Handle - 0x0050 */
    /* 28 - Dummy Characteristic 11 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        1,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        92,

        /* Value */
        gatt_value_arr + 119,
    },

    /* Handle - 0x0051 */
    /* Characteristic 28 - Dummy Characteristic 11 - Aggregate Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        6,

        /* Next Attribute Type Index */
        84,

        /* UUID Offset */
        18,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 408),
    },

    /* Handle - 0x0052 */
    /* 29 - Dummy Characteristic 12 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        85,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 414),
    },

    /* Handle - 0x0053 */
    /* 29 - Dummy Characteristic 12 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_WITHOUT_RSP_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (4 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        94,

        /* Value */
        gatt_db_peer_specific_val_arr + 95,
    },

    /* Handle - 0x0054 */
    /* Characteristic 29 - Dummy Characteristic 12 - Aggregate Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        8,

        /* Next Attribute Type Index */
        87,

        /* UUID Offset */
        18,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 419),
    },

    /* Handle - 0x0055 */
    /* 30 - Dummy Characteristic 13 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        92,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 427),
    },

    /* Handle - 0x0056 */
    /* 30 - Dummy Characteristic 13 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (5 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        96,

        /* Value */
        gatt_value_arr + 120,
    },

    /* Handle - 0x0057 */
    /* Characteristic 30 - Dummy Characteristic 13 - Aggregate Format */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        4,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        18,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 432),
    },

    /* Handle - 0x0058 */
    /* Characteristic 30 - Dummy Characteristic 13 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (27 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        98,

        /* Value */
        gatt_value_arr + 126,
    },

    /* Handle - 0x0059 */
    /* Characteristic 30 - Dummy Characteristic 13 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (27 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        100,

        /* Value */
        gatt_value_arr + 154,
    },

    /* Handle - 0x005A */
    /* Characteristic 30 - Dummy Characteristic 13 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (27 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        102,

        /* Value */
        gatt_value_arr + 182,
    },

    /* Handle - 0x005B */
    /* 5 - Dummy Service 2 - Secondary Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        95,

        /* UUID Offset */
        2,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 104),
    },

    /* Handle - 0x005C */
    /* 31 - Dummy Characteristic 14 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        96,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 436),
    },

    /* Handle - 0x005D */
    /* 31 - Dummy Characteristic 14 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_BROADCAST_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (10 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        106,

        /* Value */
        gatt_db_peer_specific_val_arr + 100,
    },

    /* Handle - 0x005E */
    /* 31 - Dummy Characteristic 14 - SCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        98,

        /* UUID Offset */
        14,

        /* Value */
        gatt_db_peer_specific_val_arr + 111,
    },

    /* Handle - 0x005F */
    /* 6 - Dummy Service 3 - Secondary Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        2,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 108),
    },

    /* Handle - 0x0060 */
    /* 32 - Dummy Characteristic 15 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        100,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 441),
    },

    /* Handle - 0x0061 */
    /* 32 - Dummy Characteristic 15 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY |
        GATT_DB_CHAR_BROADCAST_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (10 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        110,

        /* Value */
        gatt_db_peer_specific_val_arr + 113,
    },

    /* Handle - 0x0062 */
    /* 32 - Dummy Characteristic 15 - SCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        14,

        /* Value */
        gatt_db_peer_specific_val_arr + 124,
    },

    /* Handle - 0x0063 */
    /* 7 - Dummy Service 4 - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        109,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 112),
    },

    /* Handle - 0x0064 */
    /* 33 - Dummy Characteristic 16 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        105,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 446),
    },

    /* Handle - 0x0065 */
    /* 33 - Dummy Characteristic 16 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (10 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        114,

        /* Value */
        gatt_db_peer_specific_val_arr + 126,
    },

    /* Handle - 0x0066 */
    /* Characteristic 33 - Dummy Characteristic 16 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        112,

        /* UUID Offset */
        70,

        /* Value */
        gatt_value_arr + 210,
    },

    /* Handle - 0x0067 */
    /* Characteristic 33 - Dummy Characteristic 16 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        113,

        /* UUID Offset */
        72,

        /* Value */
        gatt_value_arr + 218,
    },

    /* Handle - 0x0068 */
    /* Characteristic 33 - Dummy Characteristic 16 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        114,

        /* UUID Offset */
        74,

        /* Value */
        gatt_value_arr + 226,
    },

    /* Handle - 0x0069 */
    /* 34 - Dummy Characteristic 17 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        110,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 451),
    },

    /* Handle - 0x006A */
    /* 34 - Dummy Characteristic 17 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length + Length Header Byte */
        (5 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        116,

        /* Value */
        gatt_db_peer_specific_val_arr + 137,
    },

    /* Handle - 0x006B */
    /* Characteristic 34 - Dummy Characteristic 17 - Extended Properties */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        8,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 456),
    },

    /* Handle - 0x006C */
    /* Characteristic 34 - Dummy Characteristic 17 - User Description */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length */
        (10 + 1),

        /* Next Attribute Type Index */
        122,

        /* UUID Offset */
        10,

        /* Value */
        gatt_value_arr + 234,
    },

    /* Handle - 0x006D */
    /* 8 - Dummy Service 5 - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        115,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 118),
    },

    /* Handle - 0x006E */
    /* 35 - Dummy Characteristic 18 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        116,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 458),
    },

    /* Handle - 0x006F */
    /* 35 - Dummy Characteristic 18 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_AUTHORIZATION_PROPERTY,

        /* Value Length + Length Header Byte */
        (10 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        120,

        /* Value */
        gatt_value_arr + 245,
    },

    /* Handle - 0x0070 */
    /* Characteristic 35 - Dummy Characteristic 18 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_AUTHORIZATION_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        70,

        /* Value */
        gatt_value_arr + 256,
    },

    /* Handle - 0x0071 */
    /* Characteristic 35 - Dummy Characteristic 18 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_AUTHORIZATION_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        72,

        /* Value */
        gatt_value_arr + 264,
    },

    /* Handle - 0x0072 */
    /* Characteristic 35 - Dummy Characteristic 18 - HLD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_AUTHORIZATION_PROPERTY,

        /* Value Length + Length Header Byte */
        (7 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        74,

        /* Value */
        gatt_value_arr + 272,
    },

    /* Handle - 0x0073 */
    /* 9 - Dummy Service 6 - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        123,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 122),
    },

    /* Handle - 0x0074 */
    /* 36 - Dummy Characteristic 19 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        120,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 463),
    },

    /* Handle - 0x0075 */
    /* 36 - Dummy Characteristic 19 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_NOTIFY_PROPERTY |
        GATT_DB_CHAR_INDICATE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (1 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        124,

        /* Value */
        gatt_value_arr + 280,
    },

    /* Handle - 0x0076 */
    /* 36 - Dummy Characteristic 19 - CCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        119,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_arr + 143,
    },

    /* Handle - 0x0077 */
    /* 36 - Dummy Characteristic 19 - CCD */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY |
        GATT_DB_PEER_SPECIFIC_VAL_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        12,

        /* Value */
        gatt_db_peer_specific_val_arr + 145,
    },

    /* Handle - 0x0078 */
    /* 37 - Dummy Characteristic 20 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        124,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 468),
    },

    /* Handle - 0x0079 */
    /* 37 - Dummy Characteristic 20 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        100,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        126,

        /* Value */
        gatt_value_arr + 282,
    },

    /* Handle - 0x007A */
    /* Characteristic 37 - Dummy Characteristic 20 - User Description */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length */
        65,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        10,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 473),
    },

    /* Handle - 0x007B */
    /* 10 - Dummy Service 7 - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        126,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 128),
    },

    /* Handle - 0x007C */
    /* 38 - Dummy Characteristic 21 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        127,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 538),
    },

    /* Handle - 0x007D */
    /* 38 - Dummy Characteristic 21 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (10 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        130,

        /* Value */
        gatt_value_arr + 382,
    },

    /* Handle - 0x007E */
    /* 11 - Dummy Service 8 - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        129,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 132),
    },

    /* Handle - 0x007F */
    /* 39 - Dummy Characteristic 22 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        130,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 543),
    },

    /* Handle - 0x0080 */
    /* 39 - Dummy Characteristic 22 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_SIGNED_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (5 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        134,

        /* Value */
        gatt_value_arr + 393,
    },

    /* Handle - 0x0081 */
    /* 12 - Dummy Service 9 - Service Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        2,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        0,

        /* Value */
        (UCHAR *)(gatt_const_uuid_arr + 136),
    },

    /* Handle - 0x0082 */
    /* 40 - Dummy Characteristic 23 - Characteristic Declaration */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_FIXED_LENGTH_PROPERTY,

        /* Value Length */
        5,

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        6,

        /* Value */
        (UCHAR *)(gatt_const_value_arr + 548),
    },

    /* Handle - 0x0083 */
    /* 40 - Dummy Characteristic 23 - Characteristic Value */
    {
        /* Property */
        GATT_DB_CHAR_READ_PROPERTY |
        GATT_DB_CHAR_SIGNED_WRITE_PROPERTY,

        /* Auxiliary Property */
        GATT_DB_NO_AUXILLARY_PROPERTY,

        /* Value Length + Length Header Byte */
        (5 + 1),

        /* Next Attribute Type Index */
        ATT_INVALID_ATTR_HANDLE_VAL,

        /* UUID Offset */
        138,

        /* Value */
        gatt_value_arr + 399,
    },

};


DECL_CONST GATT_DB_CHARACERISTIC gatt_characteristic[GATT_CHARACTERISTIC_COUNT] =
{

    /* 0 - DeviceName */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0002,

        /* Characteristic Service Index */
        0,
    },

    /* 1 - Appearance */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0004,

        /* Characteristic Service Index */
        0,
    },

    /* 2 - PeripheralPreferredConnectionParameters */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0006,

        /* Characteristic Service Index */
        0,
    },

    /* 3 - CentralAddressResolution */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0008,

        /* Characteristic Service Index */
        0,
    },

    /* 4 - RPAOnly */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x000A,

        /* Characteristic Service Index */
        0,
    },

    /* 5 - Service Changed */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x000D,

        /* Characteristic Service Index */
        1,
    },

    /* 6 - Client Supported Features */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0010,

        /* Characteristic Service Index */
        1,
    },

    /* 7 - Server Supported Features */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0012,

        /* Characteristic Service Index */
        1,
    },

    /* 8 - BatteryLevel */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4,
        },

        /* Characteristic Start Handle */
        0x0015,

        /* Characteristic Service Index */
        2,
    },

    /* 9 - ManufacturerName */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x001A,

        /* Characteristic Service Index */
        3,
    },

    /* 10 - ModelNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x001C,

        /* Characteristic Service Index */
        3,
    },

    /* 11 - SerialNumber */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x001E,

        /* Characteristic Service Index */
        3,
    },

    /* 12 - FirmwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0020,

        /* Characteristic Service Index */
        3,
    },

    /* 13 - HardwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0022,

        /* Characteristic Service Index */
        3,
    },

    /* 14 - SoftwareRevision */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0024,

        /* Characteristic Service Index */
        3,
    },

    /* 15 - SystemId */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0026,

        /* Characteristic Service Index */
        3,
    },

    /* 16 - RegCertDataList */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0028,

        /* Characteristic Service Index */
        3,
    },

    /* 17 - PnPID */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x002A,

        /* Characteristic Service Index */
        3,
    },

    /* 18 - Dummy Characteristic 1 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x002E,

        /* Characteristic Service Index */
        4,
    },

    /* 19 - Dummy Characteristic 2 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            6,
        },

        /* Characteristic Start Handle */
        0x0031,

        /* Characteristic Service Index */
        4,
    },

    /* 20 - Dummy Characteristic 3 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0037,

        /* Characteristic Service Index */
        4,
    },

    /* 21 - Dummy Characteristic 4 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x003A,

        /* Characteristic Service Index */
        4,
    },

    /* 22 - Dummy Characteristic 5 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x003D,

        /* Characteristic Service Index */
        4,
    },

    /* 23 - Dummy Characteristic 6 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0040,

        /* Characteristic Service Index */
        4,
    },

    /* 24 - Dummy Characteristic 7 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0043,

        /* Characteristic Service Index */
        4,
    },

    /* 25 - Dummy Characteristic 8 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0046,

        /* Characteristic Service Index */
        4,
    },

    /* 26 - Dummy Characteristic 9 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0049,

        /* Characteristic Service Index */
        4,
    },

    /* 27 - Dummy Characteristic 10 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x004C,

        /* Characteristic Service Index */
        4,
    },

    /* 28 - Dummy Characteristic 11 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x004F,

        /* Characteristic Service Index */
        4,
    },

    /* 29 - Dummy Characteristic 12 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0052,

        /* Characteristic Service Index */
        4,
    },

    /* 30 - Dummy Characteristic 13 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            6,
        },

        /* Characteristic Start Handle */
        0x0055,

        /* Characteristic Service Index */
        4,
    },

    /* 31 - Dummy Characteristic 14 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x005C,

        /* Characteristic Service Index */
        5,
    },

    /* 32 - Dummy Characteristic 15 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0060,

        /* Characteristic Service Index */
        6,
    },

    /* 33 - Dummy Characteristic 16 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            5,
        },

        /* Characteristic Start Handle */
        0x0064,

        /* Characteristic Service Index */
        7,
    },

    /* 34 - Dummy Characteristic 17 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4,
        },

        /* Characteristic Start Handle */
        0x0069,

        /* Characteristic Service Index */
        7,
    },

    /* 35 - Dummy Characteristic 18 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            5,
        },

        /* Characteristic Start Handle */
        0x006E,

        /* Characteristic Service Index */
        8,
    },

    /* 36 - Dummy Characteristic 19 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            4,
        },

        /* Characteristic Start Handle */
        0x0074,

        /* Characteristic Service Index */
        9,
    },

    /* 37 - Dummy Characteristic 20 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            3,
        },

        /* Characteristic Start Handle */
        0x0078,

        /* Characteristic Service Index */
        9,
    },

    /* 38 - Dummy Characteristic 21 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x007C,

        /* Characteristic Service Index */
        10,
    },

    /* 39 - Dummy Characteristic 22 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x007F,

        /* Characteristic Service Index */
        11,
    },

    /* 40 - Dummy Characteristic 23 */
    {
        {
            /* Number of attributes in characteristic including the definition */
            2,
        },

        /* Characteristic Start Handle */
        0x0082,

        /* Characteristic Service Index */
        12,
    },
};


DECL_CONST GATT_DB_SERVICE gatt_service[GATT_SERVICE_COUNT] =
{

    /* 0 - GAP */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0001,

        /* Service End Handle */
        0x000B,

        /* Characteristic Start Index */
        0,

        /* Characteristic End Index */
        4
    },

    /* 1 - GATT */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x000C,

        /* Service End Handle */
        0x0013,

        /* Characteristic Start Index */
        5,

        /* Characteristic End Index */
        7
    },

    /* 2 - Battery */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0014,

        /* Service End Handle */
        0x0018,

        /* Characteristic Start Index */
        8,

        /* Characteristic End Index */
        8
    },

    /* 3 - DeviceInformation */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0019,

        /* Service End Handle */
        0x002B,

        /* Characteristic Start Index */
        9,

        /* Characteristic End Index */
        17
    },

    /* 4 - Dummy Service 1 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x002C,

        /* Service End Handle */
        0x005A,

        /* Characteristic Start Index */
        18,

        /* Characteristic End Index */
        30
    },

    /* 5 - Dummy Service 2 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SECONDARY_SERVICE_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x005B,

        /* Service End Handle */
        0x005E,

        /* Characteristic Start Index */
        31,

        /* Characteristic End Index */
        31
    },

    /* 6 - Dummy Service 3 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SECONDARY_SERVICE_PROPERTY |
        GATT_DB_SER_SUPPORT_BR_LINK_TYPE,

        /* Service Start Handle */
        0x005F,

        /* Service End Handle */
        0x0062,

        /* Characteristic Start Index */
        32,

        /* Characteristic End Index */
        32
    },

    /* 7 - Dummy Service 4 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_SECURITY_LEVEL2 |
        GATT_DB_SER_SECURITY_MODE1 |
        GATT_DB_SER_ENCRYPT_KEY_SIZE_16 |
        GATT_DB_SER_SUPPORT_LE_LINK_TYPE,

        /* Service Start Handle */
        0x0063,

        /* Service End Handle */
        0x006C,

        /* Characteristic Start Index */
        33,

        /* Characteristic End Index */
        34
    },

    /* 8 - Dummy Service 5 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_NO_SECURITY_PROPERTY |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x006D,

        /* Service End Handle */
        0x0072,

        /* Characteristic Start Index */
        35,

        /* Characteristic End Index */
        35
    },

    /* 9 - Dummy Service 6 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_SECURITY_LEVEL1 |
        GATT_DB_SER_SECURITY_MODE1 |
        GATT_DB_SER_ENCRYPT_KEY_SIZE_16 |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0073,

        /* Service End Handle */
        0x007A,

        /* Characteristic Start Index */
        36,

        /* Characteristic End Index */
        37
    },

    /* 10 - Dummy Service 7 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_SECURITY_LEVEL3 |
        GATT_DB_SER_SECURITY_MODE1 |
        GATT_DB_SER_ENCRYPT_KEY_SIZE_16 |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x007B,

        /* Service End Handle */
        0x007D,

        /* Characteristic Start Index */
        38,

        /* Characteristic End Index */
        38
    },

    /* 11 - Dummy Service 8 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_SECURITY_LEVEL1 |
        GATT_DB_SER_SECURITY_MODE2 |
        GATT_DB_SER_ENCRYPT_KEY_SIZE_16 |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x007E,

        /* Service End Handle */
        0x0080,

        /* Characteristic Start Index */
        39,

        /* Characteristic End Index */
        39
    },

    /* 12 - Dummy Service 9 */
    {
        {
            /* Number of attributes in Service */
            1,
        },

        /* Service Description */
        GATT_DB_SER_SECURITY_LEVEL2 |
        GATT_DB_SER_SECURITY_MODE2 |
        GATT_DB_SER_ENCRYPT_KEY_SIZE_16 |
        GATT_DB_SER_SUPPORT_ANY_LINK_TYPE,

        /* Service Start Handle */
        0x0081,

        /* Service End Handle */
        0x0083,

        /* Characteristic Start Index */
        40,

        /* Characteristic End Index */
        40
    },
};

#ifdef GATT_DB_HAVE_REGISTERATION_SUPPORT
void appl_pts_gatt_db_register(void)
{
    GATT_DB_STRUCT gatt_db;
    API_RESULT     retval;

    /* Initialize the GATT DB Registration Entry */
    BT_mem_set(&gatt_db, 0x00U, sizeof(gatt_db));

    gatt_db.gatt_service                        = gatt_service;
    gatt_db.gatt_characteristic                 = gatt_characteristic;
    gatt_db.gatt_const_value_arr                = gatt_const_value_arr;
    gatt_db.gatt_const_uuid_arr                 = gatt_const_uuid_arr;
    gatt_db.gatt_db_attr_table                  = gatt_db_attr_table;
    gatt_db.gatt_type_table                     = gatt_type_table;
    gatt_db.gatt_db_peer_specific_val_arr       = gatt_db_peer_specific_val_arr;
    gatt_db.gatt_db_const_peer_specific_val_arr = gatt_db_const_peer_specific_val_arr;
    gatt_db.gatt_value_arr                      = gatt_value_arr;
    gatt_db.gatt_service_count                  = GATT_SERVICE_COUNT;
    gatt_db.gatt_characteristic_count           = GATT_CHARACTERISTIC_COUNT;
    gatt_db.gatt_db_max_type_count              = GATT_DB_MAX_TYPE_COUNT;
    gatt_db.gatt_db_peer_val_arr_size           = GATT_DB_PEER_VALUE_ARRAY_SIZE;

    /**
     * Reset the GATT DB Handler here. This is place holder for Handler
     * application provided dedicated GATT DB Handler, in future.
     * Currently, if GATT_DB_HAVE_NO_DEFAULT_CHAR_HANDLER is not enabled
     * the default handler would be gatt_char_handler, which is present
     * in the platform extensions.
     */
    gatt_db.gatt_db_char_cb                     = NULL;

#ifdef STORAGE_RETENTION_SUPPORT
    gatt_db.gatt_value_array_size               = GATT_VALUE_ARRAY_SIZE;
    gatt_db.gatt_db_max_peer_config             = GATT_DB_MAX_PEER_CONFIG;
#endif /* STORAGE_RETENTION_SUPPORT */

    retval = BT_gatt_db_register
             (
                 &gatt_db
             );
}
#endif /* GATT_DB_HAVE_REGISTERATION_SUPPORT */

#endif /* ((defined PTS) && (defined GATT_DB)) */
#endif /* GATT_DB_DYNAMIC */

