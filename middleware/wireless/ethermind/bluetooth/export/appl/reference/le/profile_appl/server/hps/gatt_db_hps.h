/**
 *  \file gatt_db_hps.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_HPS_
#define _H_GATT_DB_HPS_

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
#define GATT_CHARACTERISTIC_COUNT            19

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   5

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       3

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                50

#define GATT_VALUE_ARRAY_SIZE                 53

#define GATT_CONST_VALUE_ARRAY_SIZE           220

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         6

#define GATT_DB_MAX_ATTRIBUTES                48

#define GATT_UUID_ARRAY_SIZE                  68

#define GATT_DB_MAX_TYPE_COUNT                28

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_HPS_GAP_INST                  0

/** GATT Service */
#define GATT_SER_HPS_GATT_INST                 1

/** Battery Service */
#define GATT_SER_HPS_BATTERY_INST              2

/** DeviceInformation Service */
#define GATT_SER_HPS_DEV_INFO_INST             3

/** HTTPProxyService Service */
#define GATT_SER_HPS_HTTP_PROXY_INST           4

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_HPS_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_HPS_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_HPS_SER_CHNGD_INST           2

/** BatteryLevel */
#define GATT_CHAR_HPS_BATTERY_LVL_INST         3

/** ManufacturerName */
#define GATT_CHAR_HPS_MAN_NAME_INST            4

/** ModelNumber */
#define GATT_CHAR_HPS_MODEL_NO_INST            5

/** SerialNumber */
#define GATT_CHAR_HPS_SL_NO_INST               6

/** FirmwareRevision */
#define GATT_CHAR_HPS_FW_REV_INST              7

/** HardwareRevision */
#define GATT_CHAR_HPS_HW_REV_INST              8

/** SoftwareRevision */
#define GATT_CHAR_HPS_SW_REV_INST              9

/** SystemId */
#define GATT_CHAR_HPS_SYS_ID_INST              10

/** RegCertDataList */
#define GATT_CHAR_HPS_REG_CERT_DATA_INST       11

/** PnPID */
#define GATT_CHAR_HPS_PNP_ID_INST              12

/** HTTPURI */
#define GATT_CHAR_HPS_HTTP_URI_INST            13

/** HTTPHeaders */
#define GATT_CHAR_HPS_HTTP_HEADERS_INST        14

/** HTTPSatusCode */
#define GATT_CHAR_HPS_HTTP_STATUS_CODE_INST    15

/** HTTPEntityBody */
#define GATT_CHAR_HPS_HTTP_ENTITY_BODY_INST    16

/** HTTPControlPoint */
#define GATT_CHAR_HPS_HTTP_CONTROL_POINT_INST  17

/** HTTPSecurity */
#define GATT_CHAR_HPS_HTTP_SECURITY_INST       18

#else

extern UINT16 GATT_SER_HPS_BATTERY_INST;
extern UINT16 GATT_SER_HPS_GAP_INST;
extern UINT16 GATT_SER_HPS_GATT_INST;
extern UINT16 GATT_SER_HPS_DEV_INFO_INST;
extern UINT16 GATT_SER_HPS_HTTP_PROXY_INST;

extern UINT16 GATT_CHAR_HPS_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_HPS_HTTP_URI_INST;
extern UINT16 GATT_CHAR_HPS_HTTP_HEADERS_INST;
extern UINT16 GATT_CHAR_HPS_HTTP_STATUS_CODE_INST;
extern UINT16 GATT_CHAR_HPS_HTTP_ENTITY_BODY_INST;
extern UINT16 GATT_CHAR_HPS_HTTP_CONTROL_POINT_INST;
extern UINT16 GATT_CHAR_HPS_HTTP_SECURITY_INST;

API_RESULT appl_hps_add_gaps(void);
API_RESULT appl_hps_add_gatts(void);
API_RESULT appl_hps_add_bas(void);
API_RESULT appl_hps_add_dis(void);
API_RESULT appl_hps_add_hps(void);

#endif /* GATT_DB_DYNAMIC */
#endif /* _H_GATT_DB_HPS_ */

