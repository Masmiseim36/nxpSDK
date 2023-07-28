/**
 *  \file gatt_db_cscs.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_CSCS_
#define _H_GATT_DB_CSCS_

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
#define GATT_CHARACTERISTIC_COUNT            17

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   5

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       4

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                32

#define GATT_VALUE_ARRAY_SIZE                 4

#define GATT_CONST_VALUE_ARRAY_SIZE           199

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         8

#define GATT_DB_MAX_ATTRIBUTES                45

#define GATT_UUID_ARRAY_SIZE                  64

#define GATT_DB_MAX_TYPE_COUNT                26

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_CSCS_GAP_INST                  0

/** GATT Service */
#define GATT_SER_CSCS_GATT_INST                 1

/** DeviceInformation Service */
#define GATT_SER_CSCS_DEV_INFO_INST             2

/** Battery Service */
#define GATT_SER_CSCS_BATTERY_INST              3

/** CSC Service */
#define GATT_SER_CSCS_CSC_INST                  4

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_CSCS_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_CSCS_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_CSCS_SER_CHNGD_INST           2

/** ManufacturerName */
#define GATT_CHAR_CSCS_MAN_NAME_INST            3

/** ModelNumber */
#define GATT_CHAR_CSCS_MODEL_NO_INST            4

/** SerialNumber */
#define GATT_CHAR_CSCS_SL_NO_INST               5

/** FirmwareRevision */
#define GATT_CHAR_CSCS_FW_REV_INST              6

/** HardwareRevision */
#define GATT_CHAR_CSCS_HW_REV_INST              7

/** SoftwareRevision */
#define GATT_CHAR_CSCS_SW_REV_INST              8

/** SystemId */
#define GATT_CHAR_CSCS_SYS_ID_INST              9

/** RegCertDataList */
#define GATT_CHAR_CSCS_REG_CERT_DATA_INST       10

/** PnPID */
#define GATT_CHAR_CSCS_PNP_ID_INST              11

/** BatteryLevel */
#define GATT_CHAR_CSCS_BATTERY_LVL_INST         12

/** CSC Measurement */
#define GATT_CHAR_CSCS_CSC_MSRMNT_INST          13

/** CSC Feature */
#define GATT_CHAR_CSCS_CSC_FEATURE_INST         14

/** Sensor Location */
#define GATT_CHAR_CSCS_SNSR_LOC_INST            15

/** SC Control Point */
#define GATT_CHAR_CSCS_SC_CNTRL_PNT_INST        16
#else
/* Service Instances */
extern UINT16 GATT_SER_CSCS_GAP_INST;
extern UINT16 GATT_SER_CSCS_GATT_INST;
extern UINT16 GATT_SER_CSCS_BATTERY_INST;
extern UINT16 GATT_SER_CSCS_DEV_INFO_INST;
extern UINT16 GATT_SER_CSCS_CSC_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_CSCS_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_CSCS_CSC_MSRMNT_INST;
extern UINT16 GATT_CHAR_CSCS_CSC_FEATURE_INST;
extern UINT16 GATT_CHAR_CSCS_SNSR_LOC_INST;
extern UINT16 GATT_CHAR_CSCS_SC_CNTRL_PNT_INST;

API_RESULT appl_cscs_add_gaps(void);
API_RESULT appl_cscs_add_gatts(void);
API_RESULT appl_cscs_add_bas(void);
API_RESULT appl_cscs_add_dis(void);
API_RESULT appl_cscs_add_cscs(void);
#endif /* GATT_DB_DYNAMIC */

#endif /* _H_GATT_DB_CSCS_ */

