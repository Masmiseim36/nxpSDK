/**
 *  \file gatt_db_pass.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_PASS_
#define _H_GATT_DB_PASS_

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
#define GATT_CHARACTERISTIC_COUNT            7

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   4

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       4

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                14

#define GATT_VALUE_ARRAY_SIZE                 3

#define GATT_CONST_VALUE_ARRAY_SIZE           57

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         8

#define GATT_DB_MAX_ATTRIBUTES                24

#define GATT_UUID_ARRAY_SIZE                  42

#define GATT_DB_MAX_TYPE_COUNT                15

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_PASS_GAP_INST                  0

/** GATT Service */
#define GATT_SER_PASS_GATT_INST                 1

/** Battery Service */
#define GATT_SER_PASS_BATTERY_INST              2

/** PhoneAlertStatus Service */
#define GATT_SER_PASS_PHN_ALRT_STATUS_INST      3

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_PASS_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_PASS_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_PASS_SER_CHNGD_INST           2

/** BatteryLevel */
#define GATT_CHAR_PASS_BATTERY_LVL_INST         3

/** AlertStatus */
#define GATT_CHAR_PASS_ALRT_STATUS_INST         4

/** RingerSetting */
#define GATT_CHAR_PASS_RING_SET_INST            5

/** RingerControlPoint */
#define GATT_CHAR_PASS_RING_CTRL_PNT_INST       6

#else
 /* Service Instances */
extern UINT16 GATT_SER_PASS_GAP_INST;
extern UINT16 GATT_SER_PASS_GATT_INST;
extern UINT16 GATT_SER_PASS_BATTERY_INST;
extern UINT16 GATT_SER_PASS_PHN_ALRT_STATUS_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_PASS_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_PASS_ALRT_STATUS_INST;
extern UINT16 GATT_CHAR_PASS_RING_SET_INST;
extern UINT16 GATT_CHAR_PASS_RING_CTRL_PNT_INST;
extern UINT16 GATT_DB_MAX_ATTRIBUTES;

API_RESULT appl_pass_add_gaps(void);
API_RESULT appl_pass_add_gatts(void);
API_RESULT appl_pass_add_bas(void);
API_RESULT appl_pass_add_pass(void);

#endif /* GATT_DB_DYNAMIC */
#endif /* _H_GATT_DB_PASS_ */

