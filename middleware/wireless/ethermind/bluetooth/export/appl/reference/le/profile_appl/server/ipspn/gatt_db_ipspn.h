/**
 *  \file gatt_db_stat_ipspn.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_IPSPN_
#define _H_GATT_DB_IPSPN_

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
#define GATT_CHARACTERISTIC_COUNT            4

/** Number of Services in the data base */
#define GATT_SERVICE_COUNT                   4

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       2

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                26

#define GATT_VALUE_ARRAY_SIZE                 1

#define GATT_CONST_VALUE_ARRAY_SIZE           54

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         4

#define GATT_DB_MAX_ATTRIBUTES                16

#define GATT_UUID_ARRAY_SIZE                  36

#define GATT_DB_MAX_TYPE_COUNT                14

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_IPSPN_GAP_INST                  0

/** GATT Service */
#define GATT_SER_IPSPN_GATT_INST                 1

/** Battery Service */
#define GATT_SER_IPSPN_BATTERY_INST              2

/** IPSP Service */
#define GATT_SER_IPSPN_IPSP_INST                 3

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_IPSPN_DEV_NAME_INST            0

/** Appearance */
#define GATT_CHAR_IPSPN_APPEARANCE_INST          1

/** Service Changed */
#define GATT_CHAR_IPSPN_SER_CHNGD_INST           2

/** BatteryLevel */
#define GATT_CHAR_IPSPN_BATTERY_LVL_INST         3

#else /* GATT_DB_DYNAMIC */

/* Service Instances */
extern UINT16 GATT_SER_IPSPN_GAP_INST;
extern UINT16 GATT_SER_IPSPN_GATT_INST;
extern UINT16 GATT_SER_IPSPN_BATTERY_INST;
extern UINT16 GATT_SER_IPSPN_IPSP_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_IPSPN_BATTERY_LVL_INST;
extern UINT16 GATT_DB_MAX_ATTRIBUTES;

API_RESULT appl_ipspn_add_gaps(void);
API_RESULT appl_ipspn_add_gatts(void);
API_RESULT appl_ipspn_add_bas(void);
API_RESULT appl_ipspn_add_ipspn(void);
#endif /* GATT_DB_DYNAMIC */

#endif /* _H_GATT_DB_IPSPN_ */

