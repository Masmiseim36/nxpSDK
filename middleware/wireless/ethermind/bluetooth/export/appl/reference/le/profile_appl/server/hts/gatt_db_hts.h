/**
 *  \file gatt_db_hts.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_HTS_
#define _H_GATT_DB_HTS_

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
#define GATT_CHARACTERISTIC_COUNT            17U

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   5U

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       5U

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                32U

#define GATT_VALUE_ARRAY_SIZE                 7U

#define GATT_CONST_VALUE_ARRAY_SIZE           213U

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         10U

#define GATT_DB_MAX_ATTRIBUTES                47U

#define GATT_UUID_ARRAY_SIZE                  66U

#define GATT_DB_MAX_TYPE_COUNT                27U

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_HTS_GAP_INST                  0U

/** GATT Service */
#define GATT_SER_HTS_GATT_INST                 1U

/** Battery Service */
#define GATT_SER_HTS_BATTERY_INST              2U

/** DeviceInformation Service */
#define GATT_SER_HTS_DEV_INFO_INST             3U

/** Health Thermometer Service */
#define GATT_SER_HTS_HEALTH_THERM_INST         4U

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_HTS_DEV_NAME_INST            0U

/** Appearance */
#define GATT_CHAR_HTS_APPEARANCE_INST          1U

/** Service Changed */
#define GATT_CHAR_HTS_SER_CHNGD_INST           2U

/** BatteryLevel */
#define GATT_CHAR_HTS_BATTERY_LVL_INST         3U

/** ManufacturerName */
#define GATT_CHAR_HTS_MAN_NAME_INST            4U

/** ModelNumber */
#define GATT_CHAR_HTS_MODEL_NO_INST            5U

/** SerialNumber */
#define GATT_CHAR_HTS_SL_NO_INST               6U

/** FirmwareRevision */
#define GATT_CHAR_HTS_FW_REV_INST              7U

/** HardwareRevision */
#define GATT_CHAR_HTS_HW_REV_INST              8U

/** SoftwareRevision */
#define GATT_CHAR_HTS_SW_REV_INST              9U

/** SystemId */
#define GATT_CHAR_HTS_SYS_ID_INST              10U

/** RegCertDataList */
#define GATT_CHAR_HTS_REG_CERT_DATA_INST       11U

/** PnPID */
#define GATT_CHAR_HTS_PNP_ID_INST              12U

/** TemperatureMeasurement */
#define GATT_CHAR_HTS_TEMP_MSRMNT_INST         13U

/** IntermediateTemperature */
#define GATT_CHAR_HTS_INTRM_TEMP_INST          14U

/** TemperatureType */
#define GATT_CHAR_HTS_TEMP_TYPE_INST           15U

/** Measurement Interval */
#define GATT_CHAR_HTS_MSRMT_INTERVAL_INST      16U

#else

/* Service Instances */
extern UINT16 GATT_SER_HTS_GAP_INST;
extern UINT16 GATT_SER_HTS_GATT_INST;
extern UINT16 GATT_SER_HTS_BATTERY_INST;
extern UINT16 GATT_SER_HTS_DEV_INFO_INST;
extern UINT16 GATT_SER_HTS_HEALTH_THERM_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_HTS_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_HTS_TEMP_MSRMNT_INST;
extern UINT16 GATT_CHAR_HTS_INTRM_TEMP_INST;
extern UINT16 GATT_CHAR_HTS_MSRMT_INTERVAL_INST;
extern UINT16 GATT_DB_MAX_ATTRIBUTES;

API_RESULT appl_hts_add_gaps(void);
API_RESULT appl_hts_add_gatts(void);
API_RESULT appl_hts_add_bas(void);
API_RESULT appl_hts_add_dis(void);
API_RESULT appl_hts_add_hts(void);
#endif /* GATT_DB_DYNAMIC */

#endif /* _H_GATT_DB_HTS_ */

