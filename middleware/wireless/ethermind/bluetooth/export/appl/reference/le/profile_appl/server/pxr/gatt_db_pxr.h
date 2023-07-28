/**
 *  \file gatt_db_pxr.h
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_GATT_DB_PXR_
#define _H_GATT_DB_PXR_
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
#define GATT_SERVICE_COUNT                   6

/** Number of Characteristics that are configurable by the client */
#define GATT_DB_MAX_CONFIGUREABLE_CHAR       4

/** Maximum Length of any Characteristic Value/Descriptor */
#define GATT_DB_MAX_VAL_LENGTH                16

#define GATT_VALUE_ARRAY_SIZE                 2

#define GATT_CONST_VALUE_ARRAY_SIZE           66

#define GATT_DB_PEER_VALUE_ARRAY_SIZE         7

#define GATT_DB_MAX_ATTRIBUTES                26

#define GATT_UUID_ARRAY_SIZE                  44

#define GATT_DB_MAX_TYPE_COUNT                16

#define GATT_DB_MAX_PEER_CONFIGURATION        \
        (GATT_DB_PEER_VALUE_ARRAY_SIZE * BT_MAX_DEVICE_QUEUE_SIZE)

/** \} */

/** Service Instance Reference */

/** GAP Service */
#define GATT_SER_PXR_GAP_INST                                                  0

/** GATT Service */
#define GATT_SER_PXR_GATT_INST                                                 1

/** Battery Service */
#define GATT_SER_PXR_BATTERY_INST                                              2

/** Link Loss Service */
#define GATT_SER_PXR_LINK_LOSS_INST                                            3

/** Immediate Alert Service */
#define GATT_SER_PXR_IMMDT_ALRT_INST                                           4

/** Tx Power Level Service */
#define GATT_SER_PXR_TX_POWER_LVL_INST                                         5

/** Characteristic Instance Reference */

/** DeviceName */
#define GATT_CHAR_PXR_DEV_NAME_INST                                            0

/** Appearance */
#define GATT_CHAR_PXR_APPEARANCE_INST                                          1

/** Service Changed */
#define GATT_CHAR_PXR_SER_CHNGD_INST                                           2

/** BatteryLevel */
#define GATT_CHAR_PXR_BATTERY_LVL_INST                                         3

/** AlertLevel */
#define GATT_CHAR_PXR_LL_ALRT_LVL_INST                                         4

/** AlertLevel */
#define GATT_CHAR_PXR_IM_ALRT_LVL_INST                                         5

/** TxPower */
#define GATT_CHAR_PXR_TX_POWER_INST                                            6

#else
/* Service Instances */
extern UINT16 GATT_SER_PXR_GAP_INST;
extern UINT16 GATT_SER_PXR_GATT_INST;
extern UINT16 GATT_SER_PXR_BATTERY_INST;
extern UINT16 GATT_SER_PXR_LINK_LOSS_INST;
extern UINT16 GATT_SER_PXR_IMMDT_ALRT_INST;
extern UINT16 GATT_SER_PXR_TX_POWER_LVL_INST;

/* Characteristic Instances */
extern UINT16 GATT_CHAR_PXR_BATTERY_LVL_INST;
extern UINT16 GATT_CHAR_PXR_LL_ALRT_LVL_INST;
extern UINT16 GATT_CHAR_PXR_IM_ALRT_LVL_INST;
extern UINT16 GATT_CHAR_PXR_TX_POWER_INST;

API_RESULT appl_pxr_add_gaps(void);
API_RESULT appl_pxr_add_gatts(void);
API_RESULT appl_pxr_add_bas(void);
API_RESULT appl_pxr_add_ias(void);
API_RESULT appl_pxr_add_lls(void);
API_RESULT appl_pxr_add_tps(void);
#endif /* GATT_DB_DYNAMIC */
#endif /* _H_GATT_DB_PXR_ */

