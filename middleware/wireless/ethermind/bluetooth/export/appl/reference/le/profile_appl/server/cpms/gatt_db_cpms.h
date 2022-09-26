/**
 *  \file gatt_db.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_CPMS_
#define _H_GATT_DB_CPMS_

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
#define GATT_CHARACTERISTIC_COUNT            18

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   5

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       6

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                32

#define GATT_VALUE_ARRAY_SIZE                 6

#define GATT_CONST_VALUE_ARRAY_SIZE           202

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         12

#define GATT_DB_MAX_ATTRIBUTES                49

#define GATT_UUID_ARRAY_SIZE                  66

#define GATT_DB_MAX_TYPE_COUNT                28

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_CPMS_GAP_INST                  0

/** GATT Service */
#define GATT_SER_CPMS_GATT_INST                 1

/** Battery Service */
#define GATT_SER_CPMS_BATTERY_INST              2

/** DeviceInformation Service */
#define GATT_SER_CPMS_DEV_INFO_INST             3

/** Cycling Power Meter Service */
#define GATT_SER_CPMS_CPM_INST                  4

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_CPMS_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_CPMS_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_CPMS_SER_CHNGD_INST           2

/** BatteryLevel */
#define GATT_CHAR_CPMS_BATTERY_LVL_INST         3

/** ManufacturerName */
#define GATT_CHAR_CPMS_MAN_NAME_INST            4

/** ModelNumber */
#define GATT_CHAR_CPMS_MODEL_NO_INST            5

/** SerialNumber */
#define GATT_CHAR_CPMS_SL_NO_INST               6

/** FirmwareRevision */
#define GATT_CHAR_CPMS_FW_REV_INST              7

/** HardwareRevision */
#define GATT_CHAR_CPMS_HW_REV_INST              8

/** SoftwareRevision */
#define GATT_CHAR_CPMS_SW_REV_INST              9

/** SystemId */
#define GATT_CHAR_CPMS_SYS_ID_INST              10

/** RegCertDataList */
#define GATT_CHAR_CPMS_REG_CERT_DATA_INST       11

/** PnPID */
#define GATT_CHAR_CPMS_PNP_ID_INST              12

/** CPM Measurment */
#define GATT_CHAR_CPMS_CPM_MSRMT_INST           13

/** CPM Vector */
#define GATT_CHAR_CPMS_CPM_VECTOR_INST          14

/** Sensor Location */
#define GATT_CHAR_CPMS_SNSR_LOC_INST            15

/** CPM Feature */
#define GATT_CHAR_CPMS_CPM_FEATURE_INST         16

/** CPM Control Point */
#define GATT_CHAR_CPMS_CPM_CNTRL_POINT_INST     17


#else
/* Service Instances */
extern UINT16 GATT_SER_CPMS_GAP_INST;
extern UINT16 GATT_SER_CPMS_GATT_INST;
extern UINT16 GATT_SER_CPMS_BATTERY_INST;
extern UINT16 GATT_SER_CPMS_DEV_INFO_INST;
extern UINT16 GATT_SER_CPMS_CPM_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_CPMS_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_CPMS_CPM_MSRMT_INST;
extern UINT16 GATT_CHAR_CPMS_CPM_VECTOR_INST;
extern UINT16 GATT_CHAR_CPMS_SNSR_LOC_INST;
extern UINT16 GATT_CHAR_CPMS_CPM_FEATURE_INST;
extern UINT16 GATT_CHAR_CPMS_CPM_CNTRL_POINT_INST;
extern UINT16 GATT_DB_MAX_ATTRIBUTES;

API_RESULT appl_cpms_add_gaps(void);
API_RESULT appl_cpms_add_gatts(void);
API_RESULT appl_cpms_add_bas(void);
API_RESULT appl_cpms_add_dis(void);
API_RESULT appl_cpms_add_cpms(void);

#endif /* GATT_DB_DYNAMIC */
#endif /* _H_GATT_DB_CPMS_ */

