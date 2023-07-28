/**
 *  \file gatt_db_basic.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_BASIC_
#define _H_GATT_DB_BASIC_

#ifndef GATT_DB_DYNAMIC
/**
 * addgroup gatt_db_module
 */

/**
 * defgroup gatt_db_tuneable_param Tuneable Parameters
 * {
 * This section defines the Tuneable Constants of Data Base Module.
 */

/** Number of Characteristics in the data base */
#define GATT_CHARACTERISTIC_COUNT            13

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   4

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       2

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                32

#define GATT_VALUE_ARRAY_SIZE                 1

#define GATT_CONST_VALUE_ARRAY_SIZE           190

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         4

#define GATT_DB_MAX_ATTRIBUTES                34

#define GATT_UUID_ARRAY_SIZE                  54

#define GATT_DB_MAX_TYPE_COUNT                21

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_GAP_INST                  0

/** GATT Service */
#define GATT_SER_GATT_INST                 1

/** Battery Service */
#define GATT_SER_BATTERY_INST              2

/** DeviceInformation Service */
#define GATT_SER_DEV_INFO_INST             3

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_SER_CHNGD_INST           2

/** BatteryLevel */
#define GATT_CHAR_BATTERY_LVL_INST         3

/** ManufacturerName */
#define GATT_CHAR_MAN_NAME_INST            4

/** ModelNumber */
#define GATT_CHAR_MODEL_NO_INST            5

/** SerialNumber */
#define GATT_CHAR_SL_NO_INST               6

/** FirmwareRevision */
#define GATT_CHAR_FW_REV_INST              7

/** HardwareRevision */
#define GATT_CHAR_HW_REV_INST              8

/** SoftwareRevision */
#define GATT_CHAR_SW_REV_INST              9

/** SystemId */
#define GATT_CHAR_SYS_ID_INST              10

/** RegCertDataList */
#define GATT_CHAR_REG_CERT_DATA_INST       11

/** PnPID */
#define GATT_CHAR_PNP_ID_INST              12

#else

/* Service Instances */
extern UINT16 GATT_SER_BASIC_GAP_INST;
extern UINT16 GATT_SER_BASIC_GATT_INST;
extern UINT16 GATT_SER_BASIC_BATTERY_INST;
extern UINT16 GATT_SER_BASIC_DEV_INFO_INST;
extern UINT16 GATT_SER_BASIC_TEST_SERV_A_INST;
extern UINT16 GATT_SER_BASIC_TEST_SERV_B_INST;

/** GAP Service Characteristic Instances */
extern UINT16 GATT_CHAR_BASIC_DEV_NAME_INST;
extern UINT16 GATT_CHAR_BASIC_APPEARANCE_INST;

/** GATT Service Characteristic Instances */
extern UINT16 GATT_CHAR_BASIC_SER_CHNGD_INST;
extern UINT16 GATT_CHAR_BASIC_CLIENT_SUPPORTED_INST;
extern UINT16 GATT_CHAR_BASIC_DATABASE_HASH_INST;
extern UINT16 GATT_CHAR_BASIC_SERVER_SUPPORTED_INST;

/** Battery Service Characteristic Instances */
extern UINT16 GATT_CHAR_BASIC_BATTERY_LVL_INST;

/** Device Info Service Characteristic Instances */
extern UINT16 GATT_CHAR_BASIC_MAN_NAME_INST;
extern UINT16 GATT_CHAR_BASIC_MODEL_NO_INST;
extern UINT16 GATT_CHAR_BASIC_SL_NO_INST;
extern UINT16 GATT_CHAR_BASIC_FW_REV_INST;
extern UINT16 GATT_CHAR_BASIC_HW_REV_INST;
extern UINT16 GATT_CHAR_BASIC_SW_REV_INST;
extern UINT16 GATT_CHAR_BASIC_SYS_ID_INST;
extern UINT16 GATT_CHAR_BASIC_REG_CERT_DATA_INST;
extern UINT16 GATT_CHAR_BASIC_PNP_ID_INST;

/** Test Service-A Characterisic Instance */
extern UINT16 GATT_CHAR_BASIC_TEST_CHAR_A1_INST;

/** Test Service-B Characteristic Instance */
extern UINT16 GATT_CHAR_BASIC_TEST_CHAR_B1_INST;
extern UINT16 GATT_CHAR_BASIC_TEST_CHAR_B2_INST;

/**
 * GATT DB Client Supported Features Characteristic Value.
 */
extern UCHAR appl_basic_csfc_value[BT_MAX_DEVICE_QUEUE_SIZE];

/**
 * GATT Service: Service Changed CCCD Value.
 */
extern UINT16 appl_basic_serv_chngd_cccd[BT_MAX_DEVICE_QUEUE_SIZE];

/**
 * GATT Service: Maintains the change aware state of each
 * Client device.
 */
extern UCHAR appl_basic_change_aware_state[BT_MAX_DEVICE_QUEUE_SIZE];

/**
 * GATT Service: Maintains the ATT Request counter of each
 * Client device.
 */
extern UCHAR appl_basic_request_counter[BT_MAX_DEVICE_QUEUE_SIZE];

void appl_basic_dyn_db_init(void);
API_RESULT appl_basic_add_gaps(void);
API_RESULT appl_basic_add_gatts(void);
API_RESULT appl_basic_add_bas(void);
API_RESULT appl_basic_add_dis(void);
API_RESULT appl_basic_add_test_serv_a(void);
API_RESULT appl_basic_add_test_serv_b(void);
#endif /* GATT_DB_DYNAMIC */

#endif /* _H_GATT_DB_BASIC_ */

