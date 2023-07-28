/**
 *  \file gatt_db_pts.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_PTS_
#define _H_GATT_DB_PTS_

#ifndef GATT_DB_DYNAMIC
/**
 * addgroup gatt_db_module
 */

/**
 * defgroup gatt_db_tuneable_param Tunable Parameters
 * {
 * This section defines the Tunable Constants of Data Base Module.
 */

/** Number of Characteristics in the data base */
#define GATT_CHARACTERISTIC_COUNT            41

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   13

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       20

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                100

#define GATT_VALUE_ARRAY_SIZE                 405

#define GATT_CONST_VALUE_ARRAY_SIZE           553

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         147

#define GATT_DB_MAX_ATTRIBUTES                132

#define GATT_UUID_ARRAY_SIZE                  140

#define GATT_DB_MAX_TYPE_COUNT                70

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_PTS_GAP_INST                   0

/** GATT Service */
#define GATT_SER_PTS_GATT_INST                  1

/** Battery Service */
#define GATT_SER_PTS_BATTERY_INST               2

/** DeviceInformation Service */
#define GATT_SER_PTS_DEV_INFO_INST              3

/** Dummy Service 1 Service */
#define GATT_SER_PTS_1_DUMMY_INST               4

/** Dummy Service 2 Service */
#define GATT_SER_PTS_2_DUMMY_INST               5

/** Dummy Service 3 Service */
#define GATT_SER_PTS_3_DUMMY_INST               6

/** Dummy Service 4 Service */
#define GATT_SER_PTS_4_DUMMY_INST               7

/** Dummy Service 5 Service */
#define GATT_SER_PTS_5_DUMMY_INST               8

/** Dummy Service 6 Service */
#define GATT_SER_PTS_6_DUMMY_INST               9

/** Dummy Service 7 Service */
#define GATT_SER_PTS_7_DUMMY_INST               10

/** Dummy Service 8 Service */
#define GATT_SER_PTS_8_DUMMY_INST               11

/** Dummy Service 9 Service */
#define GATT_SER_PTS_9_DUMMY_INST               12

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_PTS_DEV_NAME_INST             0

/** Appearance */
#define GATT_CHAR_PTS_APPEARANCE_INST           1

/** PeripheralPreferredConnectionParameters */
#define GATT_CHAR_PTS_PPCP_INST                 2

/** CentralAddressResolution */
#define GATT_CHAR_PTS_CAR_INST                  3

/** RPAOnly */
#define GATT_CHAR_PTS_RPA_ONLY_INST             4

/** Service Changed */
#define GATT_CHAR_PTS_SER_CHNGD_INST            5

/** Client Supported Features */
#define GATT_CHAR_PTS_CLIENT_SUPPORTED_INST     6

/** Server Supported Features */
#define GATT_CHAR_PTS_SERVER_SUPPORTED_INST     7

/** BatteryLevel */
#define GATT_CHAR_PTS_BATTERY_LVL_INST          8

/** ManufacturerName */
#define GATT_CHAR_PTS_MAN_NAME_INST             9

/** ModelNumber */
#define GATT_CHAR_PTS_MODEL_NO_INST             10

/** SerialNumber */
#define GATT_CHAR_PTS_SL_NO_INST                11

/** FirmwareRevision */
#define GATT_CHAR_PTS_FW_REV_INST               12

/** HardwareRevision */
#define GATT_CHAR_PTS_HW_REV_INST               13

/** SoftwareRevision */
#define GATT_CHAR_PTS_SW_REV_INST               14

/** SystemId */
#define GATT_CHAR_PTS_SYS_ID_INST               15

/** RegCertDataList */
#define GATT_CHAR_PTS_REG_CERT_DATA_INST        16

/** PnPID */
#define GATT_CHAR_PTS_PNP_ID_INST               17

/** Dummy Characteristic 1 */
#define GATT_CHAR_PTS_1_DUMMY_INST              18

/** Dummy Characteristic 2 */
#define GATT_CHAR_PTS_2_DUMMY_INST              19

/** Dummy Characteristic 3 */
#define GATT_CHAR_PTS_3_DUMMY_INST              20

/** Dummy Characteristic 4 */
#define GATT_CHAR_PTS_4_DUMMY_INST              21

/** Dummy Characteristic 5 */
#define GATT_CHAR_PTS_5_DUMMY_INST              22

/** Dummy Characteristic 6 */
#define GATT_CHAR_PTS_6_DUMMY_INST              23

/** Dummy Characteristic 7 */
#define GATT_CHAR_PTS_7_DUMMY_INST              24

/** Dummy Characteristic 8 */
#define GATT_CHAR_PTS_8_DUMMY_INST              25

/** Dummy Characteristic 9 */
#define GATT_CHAR_PTS_9_DUMMY_INST              26

/** Dummy Characteristic 10 */
#define GATT_CHAR_PTS_10_DUMMY_INST             27

/** Dummy Characteristic 11 */
#define GATT_CHAR_PTS_11_DUMMY_INST             28

/** Dummy Characteristic 12 */
#define GATT_CHAR_PTS_12_DUMMY_INST             29

/** Dummy Characteristic 13 */
#define GATT_CHAR_PTS_13_DUMMY_INST             30

/** Dummy Characteristic 14 */
#define GATT_CHAR_PTS_14_DUMMY_INST             31

/** Dummy Characteristic 15 */
#define GATT_CHAR_PTS_15_DUMMY_INST             32

/** Dummy Characteristic 16 */
#define GATT_CHAR_PTS_16_DUMMY_INST             33

/** Dummy Characteristic 17 */
#define GATT_CHAR_PTS_17_DUMMY_INST             34

/** Dummy Characteristic 18 */
#define GATT_CHAR_PTS_18_DUMMY_INST             35

/** Dummy Characteristic 19 */
#define GATT_CHAR_PTS_19_DUMMY_INST             36

/** Dummy Characteristic 20 */
#define GATT_CHAR_PTS_20_DUMMY_INST             37

/** Dummy Characteristic 21 */
#define GATT_CHAR_PTS_21_DUMMY_INST             38

/** Dummy Characteristic 22 */
#define GATT_CHAR_PTS_22_DUMMY_INST             39

/** Dummy Characteristic 23 */
#define GATT_CHAR_PTS_23_DUMMY_INST             40

#else

/* Service Instances */
extern UINT16 GATT_SER_PTS_GAP_INST;
extern UINT16 GATT_SER_PTS_GATT_INST;
extern UINT16 GATT_SER_PTS_BATTERY_INST;
extern UINT16 GATT_SER_PTS_DEV_INFO_INST;
extern UINT16 GATT_SER_PTS_1_DUMMY_INST;
extern UINT16 GATT_SER_PTS_2_DUMMY_INST;
extern UINT16 GATT_SER_PTS_3_DUMMY_INST;
extern UINT16 GATT_SER_PTS_4_DUMMY_INST;
extern UINT16 GATT_SER_PTS_5_DUMMY_INST;
extern UINT16 GATT_SER_PTS_6_DUMMY_INST;
extern UINT16 GATT_SER_PTS_7_DUMMY_INST;
extern UINT16 GATT_SER_PTS_8_DUMMY_INST;
extern UINT16 GATT_SER_PTS_9_DUMMY_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_PTS_DEV_NAME_INST;
extern UINT16 GATT_CHAR_PTS_APPEARANCE_INST;
extern UINT16 GATT_CHAR_PTS_PPCP_INST;
extern UINT16 GATT_CHAR_PTS_CAR_INST;
extern UINT16 GATT_CHAR_PTS_RPA_ONLY_INST;
extern UINT16 GATT_CHAR_PTS_SER_CHNGD_INST;
extern UINT16 GATT_CHAR_PTS_CLIENT_SUPPORTED_INST;
extern UINT16 GATT_CHAR_PTS_DATABASE_HASH_INST;
extern UINT16 GATT_CHAR_PTS_SERVER_SUPPORTED_INST;

extern UINT16 GATT_CHAR_PTS_BATTERY_LVL_INST;

extern UINT16 GATT_CHAR_PTS_MAN_NAME_INST;
extern UINT16 GATT_CHAR_PTS_MODEL_NO_INST;
extern UINT16 GATT_CHAR_PTS_SL_NO_INST;
extern UINT16 GATT_CHAR_PTS_FW_REV_INST;
extern UINT16 GATT_CHAR_PTS_HW_REV_INST;
extern UINT16 GATT_CHAR_PTS_SW_REV_INST;
extern UINT16 GATT_CHAR_PTS_SYS_ID_INST;
extern UINT16 GATT_CHAR_PTS_REG_CERT_DATA_INST;
extern UINT16 GATT_CHAR_PTS_PNP_ID_INST;

extern UINT16 GATT_CHAR_PTS_1_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_2_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_3_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_4_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_5_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_6_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_7_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_8_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_9_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_10_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_11_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_12_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_13_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_14_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_15_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_16_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_17_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_18_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_19_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_20_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_21_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_22_DUMMY_INST;
extern UINT16 GATT_CHAR_PTS_23_DUMMY_INST;
extern UINT16 GATT_DB_MAX_ATTRIBUTES;

API_RESULT appl_pts_add_gaps(void);
API_RESULT appl_pts_add_gatts(void);
API_RESULT appl_pts_add_bas(void);
API_RESULT appl_pts_add_dis(void);
#endif /* GATT_DB_DYNAMIC */

#endif /* _H_GATT_DB_PTS_ */

