/**
 *  \file gatt_db_bps.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_BPS_
#define _H_GATT_DB_BPS_

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
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       4

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                32

#define GATT_VALUE_ARRAY_SIZE                 3

#define GATT_CONST_VALUE_ARRAY_SIZE           201

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         8

#define GATT_DB_MAX_ATTRIBUTES                43

#define GATT_UUID_ARRAY_SIZE                  62

#define GATT_DB_MAX_TYPE_COUNT                25

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_BPS_GAP_INST                  0

/** GATT Service */
#define GATT_SER_BPS_GATT_INST                 1

/** Battery Service */
#define GATT_SER_BPS_BATTERY_INST              2

/** DeviceInformation Service */
#define GATT_SER_BPS_DEV_INFO_INST             3

/** Blood Pressure Service */
#define GATT_SER_BPS_BP_INST                   4

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_BPS_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_BPS_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_BPS_SER_CHNGD_INST           2

/** BatteryLevel */
#define GATT_CHAR_BPS_BATTERY_LVL_INST         3

/** ManufacturerName */
#define GATT_CHAR_BPS_MAN_NAME_INST            4

/** ModelNumber */
#define GATT_CHAR_BPS_MODEL_NO_INST            5

/** SerialNumber */
#define GATT_CHAR_BPS_SL_NO_INST               6

/** FirmwareRevision */
#define GATT_CHAR_BPS_FW_REV_INST              7

/** HardwareRevision */
#define GATT_CHAR_BPS_HW_REV_INST              8

/** SoftwareRevision */
#define GATT_CHAR_BPS_SW_REV_INST              9

/** SystemId */
#define GATT_CHAR_BPS_SYS_ID_INST              10

/** RegCertDataList */
#define GATT_CHAR_BPS_REG_CERT_DATA_INST       11

/** PnPID */
#define GATT_CHAR_BPS_PNP_ID_INST              12

/** BloodPressureMeasurement */
#define GATT_CHAR_BPS_BP_MSRMNT_INST           13

/** IntermediateCuffPressure */
#define GATT_CHAR_BPS_INTRM_CUFF_PRSR_INST     14

/** BloodPressureFeature */
#define GATT_CHAR_BPS_BP_FEATURE_INST          15

#else
/* Service Instances */
extern UINT16 GATT_SER_BPS_GAP_INST;
extern UINT16 GATT_SER_BPS_GATT_INST;
extern UINT16 GATT_SER_BPS_BATTERY_INST;
extern UINT16 GATT_SER_BPS_DEV_INFO_INST;
extern UINT16 GATT_SER_BPS_BP_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_BPS_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_BPS_BP_MSRMNT_INST;
extern UINT16 GATT_CHAR_BPS_INTRM_CUFF_PRSR_INST;
extern UINT16 GATT_CHAR_BPS_BP_FEATURE_INST;
extern UINT16 GATT_DB_MAX_ATTRIBUTES;
API_RESULT appl_bps_add_gaps(void);
API_RESULT appl_bps_add_gatts(void);
API_RESULT appl_bps_add_bas(void);
API_RESULT appl_bps_add_dis(void);
API_RESULT appl_bps_add_bps(void);
#endif /* GATT_DB_DYNAMIC */

#endif /* _H_GATT_DB_BPS_ */

