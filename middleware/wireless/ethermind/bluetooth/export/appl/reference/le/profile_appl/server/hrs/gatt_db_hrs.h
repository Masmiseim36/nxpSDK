/**
 *  \file gatt_db_hrs.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_HRS_
#define _H_GATT_DB_HRS_

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
#define GATT_CHARACTERISTIC_COUNT            16

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   5

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       3

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                32

#define GATT_VALUE_ARRAY_SIZE                 2

#define GATT_CONST_VALUE_ARRAY_SIZE           203

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         6

#define GATT_DB_MAX_ATTRIBUTES                42

#define GATT_UUID_ARRAY_SIZE                  62

#define GATT_DB_MAX_TYPE_COUNT                25

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_HRS_GAP_INST                  0

/** GATT Service */
#define GATT_SER_HRS_GATT_INST                 1

/** Battery Service */
#define GATT_SER_HRS_BATTERY_INST              2

/** DeviceInformation Service */
#define GATT_SER_HRS_DEV_INFO_INST             3

/** Heart Rate Service */
#define GATT_SER_HRS_HEART_RATE_INST           4

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_HRS_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_HRS_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_HRS_SER_CHNGD_INST           2

/** BatteryLevel */
#define GATT_CHAR_HRS_BATTERY_LVL_INST         3

/** ManufacturerName */
#define GATT_CHAR_HRS_MAN_NAME_INST            4

/** ModelNumber */
#define GATT_CHAR_HRS_MODEL_NO_INST            5

/** SerialNumber */
#define GATT_CHAR_HRS_SL_NO_INST               6

/** FirmwareRevision */
#define GATT_CHAR_HRS_FW_REV_INST              7

/** HardwareRevision */
#define GATT_CHAR_HRS_HW_REV_INST              8

/** SoftwareRevision */
#define GATT_CHAR_HRS_SW_REV_INST              9

/** SystemId */
#define GATT_CHAR_HRS_SYS_ID_INST              10

/** RegCertDataList */
#define GATT_CHAR_HRS_REG_CERT_DATA_INST       11

/** PnPID */
#define GATT_CHAR_HRS_PNP_ID_INST              12

/** HeartRateMeasurement */
#define GATT_CHAR_HRS_HR_MSRMNT                13

/** BodySensorLocation */
#define GATT_CHAR_HRS_BODY_SNSR_LOC_INST       14

/** HeartRateControlPoint */
#define GATT_CHAR_HRS_HR_CNTRL_PNT_INST        15

#else
extern UINT16 GATT_SER_HRS_GAP_INST;
extern UINT16 GATT_SER_HRS_GATT_INST;
extern UINT16 GATT_SER_HRS_BATTERY_INST;
extern UINT16 GATT_SER_HRS_DEV_INFO_INST;
extern UINT16 GATT_SER_HRS_HEART_RATE_INST;

extern UINT16 GATT_CHAR_HRS_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_HRS_HR_MSRMNT;
extern UINT16 GATT_CHAR_HRS_BODY_SNSR_LOC_INST;
extern UINT16 GATT_CHAR_HRS_HR_CNTRL_PNT_INST;

API_RESULT appl_hrs_add_gaps(void);
API_RESULT appl_hrs_add_gatts(void);
API_RESULT appl_hrs_add_bas(void);
API_RESULT appl_hrs_add_dis(void);
API_RESULT appl_hrs_add_hrs(void);

#endif /* GATT_DB_DYNAMIC */
#endif /* _H_GATT_DB_HRS_ */

